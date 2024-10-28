# 2D-MSS (2D Maximum Subsequence Sum)

This code was submitted as an assignment for the Design and Analysis of Algorithms (CSE3081) course in the Fall 2024 semester at Sogang University.


It provides three solutions to the 2D Maximum Subsequence Sum Problem using the Summed Area Table, Divide and Conquer, and a modified version of Kadane's algorithm.
This code also calculates and displays CPU usage time, illustrating the impact of time complexity for each algorithm. Each file with 'average' in the name contains the average of numbers in pgm file. The input for all the algorithms is the 2D array of 'original number - average.' Given inputs have the same number of rows and columns.

This code mainly contains three functions - algo3, algo4, algo5.
algo3 is for the Sumed Area Table, algo4 is for Divide and Conquer, and algo5 is for Modified Kadane's Algorithm.

## 1D Maximum Subsequence Sum Problem 

The 1D Maximum Subsequence Sum Problem aims to find the largest possible sum of contiguous elements in a 1D array. Given a sequence, the algorithm identifies the subsequence (or subarray) with the highest sum.

## 2D Maximum Subsequence Sum Problem

The 2D Maximum Subsequence Sum Problem is an extension of the 1D version. This problem seeks the maximum sum within a 2D subarray. The increased dimension allows the maximum subsequence to be a rectangular region, adding complexity to the search while preserving similar principles for identifying maximum sums.

### Summed Area Table

Summed-Area Table (SAT) approach first computes an SAT[size][size] array from a 2D array container[size][size] containing values of (pixel value - mean). SAT[x][y] represents the cumulative sum from (0,0) to (x,y). The sum of any region with corners (a, b) as the top-left and (c, d) as the bottom-right is computed as SAT[c][d] - SAT[a-1][d] - SAT[c][b-1] + SAT[a-1][b-1]. (When a or b is 0, the corresponding terms are omitted.) This formula allows O(1) time complexity for sum calculations.

The algorithm’s time complexity is O(n²) for creating SAT, and O(n⁴) for calculating all possible subarray sums with SAT, resulting in an overall time complexity of O(n⁴ + n²) = O(n⁴).

### Divide and Conquer

This algorithm is an adaptation of the Divide-and-Conquer Algorithm used to find the maximum subsequence sum in a 1D array. First, the sums of values (pixel value - mean) from column i to column j are stored in a 1D array called column[size]. The algorithm then applies the Divide-and-Conquer method to column[size], identifying the rectangular subarray with the maximum sum between columns i and j and comparing it with the existing maximum.

Here, column[x] represents the sum from container[x][i] to container[x][j]. For each i, j, column[x] is computed with O(n²) complexity. Then, the Divide-and-Conquer Algorithm is applied to column[size], where each column is recursively split in half to find the maximum sum. The maximum sum extending upwards and downwards from the middle is also computed, requiring O(n) time per division. The log_2(n) recursive depth gives a total complexity of O(n × log(n)). Consequently, the algorithm's overall time complexity is O(n² × n × log(n)) = O(n³ log(n)).

### Modified Kadane's Algorithm (Dynamic Programming)

This algorithm is a modified version of Kadane's Algorithm for finding the maximum subsequence sum in a 1D array. It calculates sums from column i to column j in each row, storing them in a 1D array, column[size]. Kadane’s Algorithm is then applied to column[size] to find the rectangle with the largest sum between columns i and j and compare it with the current maximum.

In this context, column[x] is the sum from container[x][i] to container[x][j]. Since column[x] is calculated once for each i, j, this step takes O(n²). Then, Kadane’s Algorithm scans column[size], updating the starting index and resetting the sum to 0 if it becomes negative; otherwise, it updates maximum values if the current sum exceeds the previous maximum. This scan takes O(n) time. Thus, the algorithm’s time complexity is O(n × n²) = O(n³).
