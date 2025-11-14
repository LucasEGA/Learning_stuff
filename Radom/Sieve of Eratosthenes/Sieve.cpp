#include <iostream>
#include "Sieve_of_Eratosthenes.h"

int main() {
    int n;
    std::cout << "Enter a number n: ";
    std::cin >> n;

    if (n < 2) {
        std::cout << "No primes below 2.\n";
        return 0;
    }

    Sieve_of_Eratosthenes sieve(n);

    std::vector<int> primes = sieve.primes();

    std::cout << "Primes up to " << n << ":\n";
    for (int p : primes) {
        std::cout << p << " ";
    }
    std::cout << "\n";

    return 0;
}
