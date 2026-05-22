#include <array>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <iostream>
#include <iomanip>

using Byte  = std::uint8_t;
using Block = std::array<Byte, 16>;
using RoundKeys = std::array<Block, 11>; // AES-128: 10 Runden + Startschlüssel

// ------------------------------------------------------------
// 1. AES-State: 4x4 Bytes, spaltenweise gespeichert
// index = 4 * column + row
// ------------------------------------------------------------

struct State {
    std::array<Byte, 16> data{};

    Byte& at(std::size_t row, std::size_t col) {
        return data[4 * col + row];
    }

    const Byte& at(std::size_t row, std::size_t col) const {
        return data[4 * col + row];
    }

    static State fromBlock(const Block& input) {
        State s;
        s.data = input;
        return s;
    }

    Block toBlock() const {
        return data;
    }
};

// ------------------------------------------------------------
// 2. Multiplikation in GF(2^8)
// ------------------------------------------------------------

constexpr Byte galoisMultiply(Byte a, Byte b) {
    Byte p = 0;

    for (int i = 0; i < 8; ++i) {
        if (b & 1) {
            p ^= a;
        }

        bool carry = (a & 0x80) != 0;
        a = static_cast<Byte>(a << 1);

        if (carry) {
            a ^= 0x1B;
        }

        b >>= 1;
    }

    return p;
}

// ------------------------------------------------------------
// 3. Multiplikatives Inverses für S-Box
// ------------------------------------------------------------

constexpr Byte galoisPow(Byte base, unsigned exponent) {
    Byte result = 1;

    while (exponent > 0) {
        if (exponent & 1) {
            result = galoisMultiply(result, base);
        }

        base = galoisMultiply(base, base);
        exponent >>= 1;
    }

    return result;
}

constexpr Byte galoisInverse(Byte x) {
    if (x == 0x00) {
        return 0x00;
    }

    return galoisPow(x, 254);
}

// ------------------------------------------------------------
// 4. AES-S-Box algorithmisch erzeugen
// ------------------------------------------------------------

constexpr Byte rotateLeft8(Byte x, unsigned shift) {
    return static_cast<Byte>((x << shift) | (x >> (8 - shift)));
}

constexpr Byte aesSBoxByte(Byte x) {
    Byte inv = galoisInverse(x);

    return static_cast<Byte>(
        inv ^
        rotateLeft8(inv, 1) ^
        rotateLeft8(inv, 2) ^
        rotateLeft8(inv, 3) ^
        rotateLeft8(inv, 4) ^
        0x63
    );
}

constexpr std::array<Byte, 256> makeSBox() {
    std::array<Byte, 256> sbox{};

    for (std::size_t i = 0; i < 256; ++i) {
        sbox[i] = aesSBoxByte(static_cast<Byte>(i));
    }

    return sbox;
}

constexpr auto AES_SBOX = makeSBox();

// ------------------------------------------------------------
// 5. AES-Rundenfunktionen
// ------------------------------------------------------------

void subBytes(State& state) {
    for (Byte& b : state.data) {
        b = AES_SBOX[b];
    }
}

void shiftRows(State& state) {
    State old = state;

    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t col = 0; col < 4; ++col) {
            state.at(row, col) = old.at(row, (col + row) % 4);
        }
    }
}

void mixColumns(State& state) {
    for (std::size_t col = 0; col < 4; ++col) {
        Byte a0 = state.at(0, col);
        Byte a1 = state.at(1, col);
        Byte a2 = state.at(2, col);
        Byte a3 = state.at(3, col);

        state.at(0, col) =
            galoisMultiply(0x02, a0) ^
            galoisMultiply(0x03, a1) ^
            a2 ^
            a3;

        state.at(1, col) =
            a0 ^
            galoisMultiply(0x02, a1) ^
            galoisMultiply(0x03, a2) ^
            a3;

        state.at(2, col) =
            a0 ^
            a1 ^
            galoisMultiply(0x02, a2) ^
            galoisMultiply(0x03, a3);

        state.at(3, col) =
            galoisMultiply(0x03, a0) ^
            a1 ^
            a2 ^
            galoisMultiply(0x02, a3);
    }
}

void addRoundKey(State& state, const Block& roundKey) {
    for (std::size_t i = 0; i < 16; ++i) {
        state.data[i] ^= roundKey[i];
    }
}

// ------------------------------------------------------------
// 6. Key Expansion für AES-128
// ------------------------------------------------------------

constexpr std::array<Byte, 10> RCON = {
    0x01, 0x02, 0x04, 0x08, 0x10,
    0x20, 0x40, 0x80, 0x1B, 0x36
};

void rotateWord(std::array<Byte, 4>& word) {
    Byte temp = word[0];

    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

void subWord(std::array<Byte, 4>& word) {
    for (Byte& b : word) {
        b = AES_SBOX[b];
    }
}

RoundKeys expandKey(const Block& key) {
    std::array<Byte, 176> expanded{};
    RoundKeys roundKeys{};

    for (std::size_t i = 0; i < 16; ++i) {
        expanded[i] = key[i];
    }

    std::size_t bytesGenerated = 16;
    std::size_t rconIndex = 0;

    while (bytesGenerated < expanded.size()) {
        std::array<Byte, 4> temp = {
            expanded[bytesGenerated - 4],
            expanded[bytesGenerated - 3],
            expanded[bytesGenerated - 2],
            expanded[bytesGenerated - 1]
        };

        if (bytesGenerated % 16 == 0) {
            rotateWord(temp);
            subWord(temp);
            temp[0] ^= RCON[rconIndex];
            ++rconIndex;
        }

        for (std::size_t i = 0; i < 4; ++i) {
            expanded[bytesGenerated] =
                expanded[bytesGenerated - 16] ^ temp[i];

            ++bytesGenerated;
        }
    }

    for (std::size_t round = 0; round < 11; ++round) {
        for (std::size_t i = 0; i < 16; ++i) {
            roundKeys[round][i] = expanded[16 * round + i];
        }
    }

    return roundKeys;
}

// ------------------------------------------------------------
// 7. AES-128: einen 16-Byte-Block verschlüsseln
// ------------------------------------------------------------

Block encryptBlock(const Block& input, const Block& key) {
    RoundKeys roundKeys = expandKey(key);
    State state = State::fromBlock(input);

    addRoundKey(state, roundKeys[0]);

    for (std::size_t round = 1; round < 10; ++round) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, roundKeys[round]);
    }

    // Letzte AES-Runde ohne MixColumns
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKeys[10]);

    return state.toBlock();
}

// ------------------------------------------------------------
// 8. Test
// ------------------------------------------------------------

static_assert(AES_SBOX[0x00] == 0x63);
static_assert(AES_SBOX[0x53] == 0xED);

int main() {
    Block input = {
        0x32, 0x43, 0xf6, 0xa8,
        0x88, 0x5a, 0x30, 0x8d,
        0x31, 0x31, 0x98, 0xa2,
        0xe0, 0x37, 0x07, 0x34
    };

    Block key = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
    };

    Block expected = {
        0x39, 0x25, 0x84, 0x1d,
        0x02, 0xdc, 0x09, 0xfb,
        0xdc, 0x11, 0x85, 0x97,
        0x19, 0x6a, 0x0b, 0x32
    };

    Block encrypted = encryptBlock(input, key);

    assert(encrypted == expected);

    for (Byte b : encrypted) {
        std::cout
            << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(b);
    }

    std::cout << '\n';

    return 0;
}