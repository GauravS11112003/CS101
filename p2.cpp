#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int SegregateArray(int data[], int start, int end) {
   int low;
   int high;
   int mid;
   int pivot;
   int temp;
   bool sorted;
   
   mid = start + (end - start) / 2;
   pivot = data[mid];
   
   sorted = false;
   low = start;
   high = end;
   
   while (!sorted) {
      while (data[low] < pivot) {
         ++low;
      }
      
      while (pivot < data[high]) {
         --high;
      }
      
      if (low >= high) {
         sorted = true;
      }
      else {
         temp = data[low];
         data[low] = data[high];
         data[high] = temp;
         
         ++low;
         --high;
      }
   }
   
   return high;
}

void SortArray(int data[], int start, int end) {
   int partition;
   
   if (start >= end) {
      return;
   }
   
   partition = SegregateArray(data, start, end);
   
   SortArray(data, start, partition);
   SortArray(data, partition + 1, end);
}

int SegregateStrings(string data[], int start, int end) {
   int low;
   int high;
   int mid;
   string pivot;
   string temp;
   bool sorted;
   
   mid = start + (end - start) / 2;
   pivot = data[mid];
   
   sorted = false;
   low = start;
   high = end;
   
   while (!sorted) {
      while (data[low].compare(pivot) < 0) {
         ++low;
      }
      
      while (pivot.compare(data[high]) < 0) {
         --high;
      }
      
      if (low >= high) {
         sorted = true;
      }
      else {
         temp = data[low];
         data[low] = data[high];
         data[high] = temp;
         
         ++low;
         --high;
      }
   }
   
   return high;
}

void SortStrings(string data[], int start, int end) {
   int partition;
   
   if (start >= end) {
      return;
   }
   
   partition = SegregateStrings(data, start, end);
   
   SortStrings(data, start, partition);
   SortStrings(data, partition + 1, end);
}

bool BinarySearchInt(int arr[], int size, int key){
    int begin = 0;
    int finish = size;
    while(begin <= finish){
        int middle = (finish + begin)/2;
        if(arr[middle] == key){
            return true;
        }
        else if(arr[middle] > key){
            finish = middle - 1;
        }
        else {
            begin = middle + 1;
        }
    }
    return false;
}

bool BinarySearchString(string arr[], int size, string key){
    int begin = 0;
    int finish = size;
    while(begin <= finish){
        int middle = (finish + begin)/2;
        if(arr[middle] == key){
            return true;
        }
        else if(arr[middle] > key){
            finish = middle - 1;
        }
        else {
            begin = middle + 1;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    string option;
    string filename1;
    string filename2;
    fstream file1;
    fstream file2;
    int numLines1 = 0;
    int numLines2 = 0;

    if (argc != 4) {
        cout << "Syntax: ./prog (\"i\", \"s\") file1.txt file2.txt" << endl;
        return 1;
    }

    option = argv[1];
    filename1 = argv[2];
    filename2 = argv[3];

    file1.open(filename1);
    if(file1.is_open()){
        string line;
        while(getline(file1, line)){
            numLines1++;
        }
        file1.close();
    }

    else{
        cout << "Failed to open \"" << filename1 << "\"" << endl;
        return 1;
    }

    file2.open(filename2);
    if(file2.is_open()){
        string line;
        while(getline(file2, line)){
            numLines2++;
        }
        file2.close();
    }

    else{
        cout << "Failed to open \"" << filename2 << "\"" << endl;
        return 1;
    }
    
    if(option == "i"){
        int array1[numLines1];
        int array2[numLines2];
        int sameArray[numLines1];
        string line1;
        string line2;
        int count1 = 0;
        int count2 = 0;
        int sameCount = 0;
        file1.open(filename1);
        file2.open(filename2);

        while(getline(file1, line1)){
            array1[count1] = stoi(line1);
            count1++;
        }

        SortArray(array1, 0, count1-1);

        int size = numLines1-1;
        while(getline(file2, line2)){
            array2[count2] = stoi(line2);
            if (BinarySearchInt(array1, size, array2[count2])){
                sameArray[sameCount] = stoi(line2);
                sameCount++;
            }
            count2++;
        }

        SortArray(array2, 0, count2-1);

        SortArray(sameArray, 0, sameCount-1);

        for(int i = 0; i < sameCount; i++){

            if(i > 0 && sameArray[i] == sameArray[i-1]){
                continue;
            }
            else {
                cout << sameArray[i] << endl;
            }
        }
    }

    else if(option == "s"){       
        string array1[numLines1];
        string array2[numLines2];
        string sameArray[numLines1];
        string line1;
        string line2;
        int count1 = 0;
        int count2 = 0;
        int sameCount = 0;
        file1.open(filename1);
        file2.open(filename2);

        while(getline(file1, line1)){
            array1[count1] = line1;
            count1++;
        }

        SortStrings(array1, 0, count1-1);

        int size = numLines1-1;

        while(getline(file2, line2)){
            array2[count2] = line2;
            if (BinarySearchString(array1, size, array2[count2])){
                sameArray[sameCount] = line2;
                sameCount++;
            }
            count2++;
        }

        SortStrings(array2, 0, count2-1);

        SortStrings(sameArray, 0, sameCount-1);

        for(int i = 0; i < sameCount; i++){

            if(i > 0 && sameArray[i] == sameArray[i-1]){
                continue;
            }
            else {
                cout << sameArray[i] << endl;
            }
        }
    }

    else {
        cout << "Syntax: ./prog (\"i\", \"s\") file1.txt file2.txt" << endl;
        return 1;
    }

    return 0;
}
