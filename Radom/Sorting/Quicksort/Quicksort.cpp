#include <iostream>
#include <vector>
#include "Quicksort.h"

int main() {
    int n;
    std::cout << "How many numbers? ";
    std::cin >> n;

    std::vector<int> A(n);

    std::cout << "Enter " << n << " numbers:\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    Quicksort qs;
    qs.Sort(A, 0, n - 1);

    std::cout << "Sorted numbers:\n";
    for (int x : A) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}


