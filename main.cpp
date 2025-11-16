#include <iostream>
#include <vector>
#include <cmath>

const int INSERTION_THRESHOLD = 16;

void insertion_sort(std::vector<int>& arr, const int left, const int right) {
    for (int i = left + 1; i <= right; ++i) {
        const int key = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void heapify(std::vector<int>& arr, const int n, const int i) {
    int largest = i;
    const int left = 2 * i + 1;
    const int right = 2 * i + 2;

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

void heap_sort(std::vector<int>& arr, const int left, const int right) {
    const int n = right - left + 1;

    // Создаем копию подмассива для heap sort
    std::vector<int> heap_arr(arr.begin() + left, arr.begin() + right + 1);

    // Построение max-heap
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(heap_arr, n, i);
    }

    // Извлечение элементов из кучи
    for (int i = n - 1; i > 0; --i) {
        std::swap(heap_arr[0], heap_arr[i]);
        heapify(heap_arr, i, 0);
    }

    // Копируем отсортированный подмассив обратно
    for (int i = 0; i < n; ++i) {
        arr[left + i] = heap_arr[i];
    }
}

int partition(std::vector<int>& arr, const int left, const int right) {
    const int pivot = arr[right];
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

void hybrid_quick_sort(std::vector<int>& arr, const int left, const int right, const int depth_limit) {
    if (left < right) {
        // Переключение на Insertion Sort для маленьких массивов
        if (right - left + 1 < INSERTION_THRESHOLD) {
            insertion_sort(arr, left, right);
            return;
        }

        // Переключение на Heap Sort при превышении глубины рекурсии
        if (depth_limit <= 0) {
            heap_sort(arr, left, right);
            return;
        }

        // Разбиение Quick Sort
        const int pivot_index = partition(arr, left, right);

        // Рекурсивная сортировка левой и правой частей
        hybrid_quick_sort(arr, left, pivot_index - 1, depth_limit - 1);
        hybrid_quick_sort(arr, pivot_index + 1, right, depth_limit - 1);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    if (n > 0) {
        // Вычисляем максимальную глубину рекурсии: 2 * log2(n)
        const int depth_limit = 2 * static_cast<int>(std::log2(n));
        hybrid_quick_sort(arr, 0, n - 1, depth_limit);
    }

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i];
        if (i < n - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
