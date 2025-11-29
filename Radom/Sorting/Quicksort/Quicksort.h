#ifndef ALGORITHMEN_QUICKSORT_H
#define ALGORITHMEN_QUICKSORT_H

#include <vector>
#include <algorithm> // for std::swap

class Quicksort {
private:
    // returns the index of the median of A[low], A[mid], A[high]
    int medianOfThree(std::vector<int>& A, int low, int high) {
        int mid = low + (high - low) / 2;

        int a = A[low];
        int b = A[mid];
        int c = A[high];

        int medianIndex;

        // check if b is median
        if ((a <= b && b <= c) || (c <= b && b <= a)) {
            medianIndex = mid;
        }
            // check if a is median
        else if ((b <= a && a <= c) || (c <= a && a <= b)) {
            medianIndex = low;
        }
            // otherwise c is median
        else {
            medianIndex = high;
        }

        return medianIndex;
    }

public:
    int Partition(std::vector<int>& A, int low, int high) {
        // choose pivot as median of low, mid, high
        int pivotIndex = medianOfThree(A, low, high);

        std::swap(A[low], A[pivotIndex]);

        int pivot = A[low];
        int leftwall = low;

        for (int i = low + 1; i <= high; ++i) {
            if (A[i] < pivot) {
                ++leftwall;
                std::swap(A[i], A[leftwall]);
            }
        }

        std::swap(A[low], A[leftwall]);
        return leftwall;
    }

    void Sort(std::vector<int>& A, int low, int high) {
        if (low < high) {
            int pivot_location = Partition(A, low, high);
            Sort(A, low, pivot_location - 1);
            Sort(A, pivot_location + 1, high);
        }
    }
};

#endif // ALGORITHMEN_QUICKSORT_H
