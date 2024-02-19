#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Mergesort algorithm for integers
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<int> L(n1);
    vector<int> R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// Quicksort algorithm for strings
int partition(vector<string>& arr, int low, int high) {
    string pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<string>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <i|s> <input_file1> <input_file2>\n";
        return 1;
    }

    string mode = argv[1];
    string file1 = argv[2];
    string file2 = argv[3];

    ifstream infile1(file1);
    ifstream infile2(file2);

    if (!infile1 || !infile2) {
        cerr << "Error: Unable to open input file.\n";
        return 1;
    }

    vector<string> strings;
    vector<int> integers;
    string item;

    // Read and store data from the first file
    while (infile1 >> item) {
        if (mode == "s")
            strings.push_back(item);
        else if (mode == "i")
            integers.push_back(stoi(item));
    }

    // Read and store data from the second file
    while (infile2 >> item) {
        if (mode == "s")
            strings.push_back(item);
        else if (mode == "i")
            integers.push_back(stoi(item));
    }

    infile1.close();
    infile2.close();

    // Sort data
    if (mode == "s")
        quickSort(strings, 0, strings.size() - 1);
    else if (mode == "i")
        mergeSort(integers, 0, integers.size() - 1);

    // Remove duplicates
    if (mode == "s")
        strings.erase(unique(strings.begin(), strings.end()), strings.end());
    else if (mode == "i")
        integers.erase(unique(integers.begin(), integers.end()), integers.end());

    // Output sorted and unique values
    if (mode == "s") {
        for (const auto& str : strings)
            cout << str << '\n';
    } else if (mode == "i") {
        for (const auto& num : integers)
            cout << num << '\n';
    }

    return 0;
}
