==============================================
  QUICK SORT (MEDIAN-OF-THREE PIVOT) - README
==============================================

This project implements the Quicksort algorithm using:

 - Median-of-three pivot selection (smart pivot)
 - Left-wall partition method
 - Recursive divide-and-conquer sorting

Files:
    Quicksort.h    -> class declaration + implementation
    Quicksort.cpp  -> optional test runner if you add a main()


----------------------------------------------
1. HOW THE ALGORITHM WORKS
----------------------------------------------

STEP 1 - Choose a good pivot (median-of-three)
----------------------------------------------
Instead of always using the first or last element as the pivot,
the algorithm selects the median value of:

    A[low], A[mid], A[high]

This improves performance on sorted or nearly sorted arrays.

After finding the median index, that element is swapped to A[low],
because the partition method expects the pivot at the start.


STEP 2 - Partitioning (Left-Wall Method)
----------------------------------------
The partition algorithm rearranges elements so that:

 - All elements smaller than the pivot are placed to the left
 - All elements greater than or equal remain to the right

Procedure:

    1. Set leftwall = low
    2. Scan from low+1 to high
    3. If A[i] < pivot:
           increase leftwall
           swap A[i] with A[leftwall]
    4. Swap pivot into its final position at A[leftwall]

After this, the pivot is in its correct sorted position.


STEP 3 - Recursive Sorting
--------------------------
Quicksort now sorts the two sides separately:

    Sort(A, low, pivot_index - 1)
    Sort(A, pivot_index + 1, high)

The pivot itself is never touched again.

Recursion stops when low >= high.


----------------------------------------------
2. WHY MEDIAN-OF-THREE?
----------------------------------------------
Selecting the pivot as the median of:
    first, middle, last
reduces bad cases and improves stability.

Advantages:
 - Avoids worst-case behavior on sorted input
 - Cheap to compute
 - Very efficient in practice


----------------------------------------------
3. HOW TO USE THE CLASS
----------------------------------------------

Example:

    #include "Quicksort.h"
    #include <vector>

    int main() {
        std::vector<int> A = {9, 3, 5, 1, 7};

        Quicksort qs;
        qs.Sort(A, 0, A.size() - 1);

        // A is now sorted
    }


----------------------------------------------
4. NOTES
----------
