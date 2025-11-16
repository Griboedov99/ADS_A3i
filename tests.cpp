//
// Created by Nick on 16.11.2025.
//

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <queue>

class ArrayGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;

public:
    ArrayGenerator() : gen(rd()) {}

    // Generate random array
    std::vector<int> generateRandom(int size, int min_val = 0, int max_val = 6000) {
        std::vector<int> arr(size);
        std::uniform_int_distribution<int> dist(min_val, max_val);

        for (int i = 0; i < size; ++i) {
            arr[i] = dist(gen);
        }
        return arr;
    }

    // Generate reverse sorted array (worst case for quick sort)
    std::vector<int> generateReverseSorted(int size) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = size - i;
        }
        return arr;
    }

    // Generate nearly sorted array
    std::vector<int> generateNearlySorted(int size, double swap_ratio = 0.01) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = i + 1;
        }

        int num_swaps = static_cast<int>(size * swap_ratio);
        std::uniform_int_distribution<int> dist(0, size - 1);

        for (int i = 0; i < num_swaps; ++i) {
            int idx1 = dist(gen);
            int idx2 = dist(gen);
            std::swap(arr[idx1], arr[idx2]);
        }

        return arr;
    }

    // Generate array with many duplicates
    std::vector<int> generateManyDuplicates(int size, int unique_values = 10) {
        std::vector<int> arr(size);
        std::uniform_int_distribution<int> dist(0, unique_values - 1);

        for (int i = 0; i < size; ++i) {
            arr[i] = dist(gen);
        }
        return arr;
    }

    // Get subarray from a larger array
    std::vector<int> getSubarray(const std::vector<int>& source, int size) {
        return std::vector<int>(source.begin(), source.begin() + size);
    }
};

class IntroSortTester {
private:
    std::random_device rd;
    std::mt19937 gen;

    const int INSERTION_THRESHOLD = 16;

    // Insertion Sort implementation
    void insertionSort(std::vector<int>& arr, int left, int right) {
        for (int i = left + 1; i <= right; ++i) {
            int key = arr[i];
            int j = i - 1;

            while (j >= left && arr[j] > key) {
                arr[j + 1] = arr[j];
                --j;
            }
            arr[j + 1] = key;
        }
    }

    // Heap Sort implementation
    void heapify(std::vector<int>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] > arr[largest]) {
            largest = left;
        }

        if (right < n && arr[right] > arr[largest]) {
            largest = right;
        }

        if (largest != i) {
            std::swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }

    void heapSort(std::vector<int>& arr, int left, int right) {
        int n = right - left + 1;

        // Build max heap
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(arr, n, i);
        }

        // Extract elements from heap
        for (int i = n - 1; i > 0; --i) {
            std::swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }

    // Partition for Quick Sort
    int partition(std::vector<int>& arr, int left, int right) {
        // Random pivot selection
        std::uniform_int_distribution<int> dist(left, right);
        int pivot_idx = dist(gen);
        std::swap(arr[pivot_idx], arr[right]);

        int pivot = arr[right];
        int i = left - 1;

        for (int j = left; j < right; ++j) {
            if (arr[j] <= pivot) {
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[right]);
        return i + 1;
    }

    // Standard Quick Sort
    void quickSort(std::vector<int>& arr, int left, int right) {
        if (left < right) {
            int pivot_idx = partition(arr, left, right);
            quickSort(arr, left, pivot_idx - 1);
            quickSort(arr, pivot_idx + 1, right);
        }
    }

    // Hybrid IntroSort
    void introSort(std::vector<int>& arr, int left, int right, int depth_limit) {
        if (left < right) {
            // Switch to insertion sort for small arrays
            if (right - left + 1 < INSERTION_THRESHOLD) {
                insertionSort(arr, left, right);
                return;
            }

            // Switch to heap sort if depth limit exceeded
            if (depth_limit <= 0) {
                heapSort(arr, left, right);
                return;
            }

            // Quick sort partition
            int pivot_idx = partition(arr, left, right);

            // Recursive calls with reduced depth limit
            introSort(arr, left, pivot_idx - 1, depth_limit - 1);
            introSort(arr, pivot_idx + 1, right, depth_limit - 1);
        }
    }

public:
    IntroSortTester() : gen(rd()) {}

    // Test standard Quick Sort
    long long testQuickSort(std::vector<int> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(arr, 0, arr.size() - 1);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    // Test hybrid IntroSort
    long long testIntroSort(std::vector<int> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        int depth_limit = 2 * static_cast<int>(std::log2(arr.size()));
        introSort(arr, 0, arr.size() - 1, depth_limit);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    // Test individual components for analysis
    long long testHeapSort(std::vector<int> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        heapSort(arr, 0, arr.size() - 1);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    long long testInsertionSort(std::vector<int> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        insertionSort(arr, 0, arr.size() - 1);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    // Run multiple tests and return median time
    long long runMultipleTests(const std::vector<int>& original_arr, bool use_intro, int num_tests = 7) {
        std::vector<long long> times;

        for (int i = 0; i < num_tests; ++i) {
            std::vector<int> arr = original_arr;

            long long time_taken;
            if (use_intro) {
                time_taken = testIntroSort(arr);
            } else {
                time_taken = testQuickSort(arr);
            }
            times.push_back(time_taken);
        }

        // Return median time
        std::sort(times.begin(), times.end());
        return times[times.size() / 2];
    }
};

// Function to run complete analysis
void runIntroSortAnalysis() {
    ArrayGenerator generator;
    IntroSortTester tester;

    // Generate large arrays for each type
    std::vector<int> random_large = generator.generateRandom(100000);
    std::vector<int> reverse_large = generator.generateReverseSorted(100000);
    std::vector<int> nearly_large = generator.generateNearlySorted(100000);
    std::vector<int> duplicates_large = generator.generateManyDuplicates(100000, 50);

    // Open files for output
    std::ofstream quick_file("quick_sort_results.csv");
    std::ofstream intro_file("intro_sort_results.csv");
    std::ofstream components_file("components_analysis.csv");

    // Write headers
    quick_file << "Size,Random,Reverse,NearlySorted,ManyDuplicates\n";
    intro_file << "Size,Random,Reverse,NearlySorted,ManyDuplicates\n";
    components_file << "Size,QuickSort,HeapSort,InsertionSort,IntroSort\n";

    // Test standard quick sort
    std::cout << "Testing Standard Quick Sort...\n";
    for (int size = 500; size <= 100000; size += 500) {
        std::vector<int> random_arr = generator.getSubarray(random_large, size);
        std::vector<int> reverse_arr = generator.getSubarray(reverse_large, size);
        std::vector<int> nearly_arr = generator.getSubarray(nearly_large, size);
        std::vector<int> duplicates_arr = generator.getSubarray(duplicates_large, size);

        long long random_time = tester.runMultipleTests(random_arr, false);
        long long reverse_time = tester.runMultipleTests(reverse_arr, false);
        long long nearly_time = tester.runMultipleTests(nearly_arr, false);
        long long duplicates_time = tester.runMultipleTests(duplicates_arr, false);

        quick_file << size << "," << random_time << "," << reverse_time << ","
                   << nearly_time << "," << duplicates_time << "\n";

        if (size % 10000 == 0) {
            std::cout << "Completed size: " << size << std::endl;
        }
    }

    // Test hybrid intro sort
    std::cout << "\nTesting Hybrid IntroSort...\n";
    for (int size = 500; size <= 100000; size += 500) {
        std::vector<int> random_arr = generator.getSubarray(random_large, size);
        std::vector<int> reverse_arr = generator.getSubarray(reverse_large, size);
        std::vector<int> nearly_arr = generator.getSubarray(nearly_large, size);
        std::vector<int> duplicates_arr = generator.getSubarray(duplicates_large, size);

        long long random_time = tester.runMultipleTests(random_arr, true);
        long long reverse_time = tester.runMultipleTests(reverse_arr, true);
        long long nearly_time = tester.runMultipleTests(nearly_arr, true);
        long long duplicates_time = tester.runMultipleTests(duplicates_arr, true);

        intro_file << size << "," << random_time << "," << reverse_time << ","
                   << nearly_time << "," << duplicates_time << "\n";

        if (size % 10000 == 0) {
            std::cout << "Completed size: " << size << std::endl;
        }
    }

    // Test individual components
    std::cout << "\nTesting Individual Components...\n";
    for (int size = 100; size <= 10000; size += 100) {
        std::vector<int> test_arr = generator.generateRandom(size);

        long long quick_time = tester.testQuickSort(test_arr);
        long long heap_time = tester.testHeapSort(test_arr);
        long long insertion_time = tester.testInsertionSort(test_arr);
        long long intro_time = tester.testIntroSort(test_arr);

        components_file << size << "," << quick_time << "," << heap_time << ","
                       << insertion_time << "," << intro_time << "\n";

        if (size % 2000 == 0) {
            std::cout << "Completed size: " << size << std::endl;
        }
    }

    quick_file.close();
    intro_file.close();
    components_file.close();
    std::cout << "\nAnalysis complete! Results saved to CSV files.\n";
}

int main() {
    runIntroSortAnalysis();
    return 0;
}
