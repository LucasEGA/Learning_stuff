#ifndef ALGORITHMEN_SIEVE_OF_ERATOSTHENES_H
#define ALGORITHMEN_SIEVE_OF_ERATOSTHENES_H

#include <vector>
#include <cmath>

class Sieve_of_Eratosthenes {
public:
    explicit Sieve_of_Eratosthenes(int n)
            : limit(n),
              isPrime(n + 1, true)
    {
        if (limit >= 0) isPrime[0] = false;
        if (limit >= 1) isPrime[1] = false;

        int bound = static_cast<int>(std::sqrt(limit));

        for (int p = 2; p <= bound; ++p) {
            if (isPrime[p]) {
                for (int multiple = p * p; multiple <= limit; multiple += p) {
                    isPrime[multiple] = false;
                }
            }
        }
    }

    bool is_prime(int n) const {
        if (n < 0 || n > limit) return false;
        return isPrime[n];
    }

    std::vector<int> primes() const {
        std::vector<int> result;
        for (int i = 2; i <= limit; ++i) {
            if (isPrime[i]) {
                result.push_back(i);
            }
        }
        return result;
    }

private:
    int limit;
    std::vector<bool> isPrime;
};

#endif // ALGORITHMEN_SIEVE_OF_ERATOSTHENES_H

