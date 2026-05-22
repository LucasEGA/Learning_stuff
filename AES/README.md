# AES-128 Block Cipher

Kleine C++-Implementierung eines AES-128-Blockciphers zu Lernzwecken.

## Was implementiert ist

Diese Implementierung verschlüsselt genau einen 16-Byte-Block mit einem 16-Byte-Key.

Enthalten sind:

- AES-State-Darstellung im spaltenweisen AES-Layout
- algorithmische Erzeugung der AES-S-Box
- `SubBytes`
- `ShiftRows`
- `MixColumns`
- `AddRoundKey`
- AES-128 Key Expansion
- `encryptBlock(input, key)`

