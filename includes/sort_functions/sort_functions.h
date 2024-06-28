#ifndef ZAC_SORT_FUNCTIONS_H
#define ZAC_SORT_FUNCTIONS_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

template <class T>
void Swap(T& a, T& b)  //Swap the two items
{
    T temp = a;
    a = b;
    b = temp;
}

enum func_names {BUBBLE_SORT, INSERTION_SORT, QUICK_SORT, MERGE_SORT, HEAP_SORT, SORT_F_COUNT};

//in order to return a function pointer from a function: line 20
//we need to typedef the function pointer we are going to return 
//we also need to typedef the signature of the function that is  
//going to return the function pointer: line 21
typedef void (*sort_f_ptr)(int *, unsigned int);
typedef sort_f_ptr (*get_sort_f) (unsigned int);
//
//^ the weakness of this is it cannot be templated

template <class T>
void bubble_sort(T a[], unsigned int size)
{
    int i, j;
    bool swapped;
    for (i = 0; i < size - 1; i++) 
    {
        swapped = false;
        for (j = 0; j < size - i - 1; j++) 
        {
            if (a[j] > a[j + 1]) 
            {
                Swap(a[j], a[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == false)
            break;
    }
}
template <class T>
void inseration_sort(T a[], unsigned int size)
{
    int i, key, j;
    for (i = 1; i < size; i++) {
        key = a[i];
        j = i - 1;

        // Move elements of arr[0..i-1],
        // that are greater than key, 
        // to one position ahead of their
        // current position
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j = j - 1;
        }
        a[j + 1] = key;
    }
}

//v Helper functions

//Secondary Helpers
// void merge(int array[], int const left, int const mid,
//            int const right)
// {
//     int const subArrayOne = mid - left + 1;
//     int const subArrayTwo = right - mid;

//     // Create temp arrays
//     auto *leftArray = new int[subArrayOne],
//          *rightArray = new int[subArrayTwo];

//     // Copy data to temp arrays leftArray[] and rightArray[]
//     for (auto i = 0; i < subArrayOne; i++)
//         leftArray[i] = array[left + i];
//     for (auto j = 0; j < subArrayTwo; j++)
//         rightArray[j] = array[mid + 1 + j];

//     auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
//     int indexOfMergedArray = left;

//     // Merge the temp arrays back into array[left..right]
//     while (indexOfSubArrayOne < subArrayOne
//            && indexOfSubArrayTwo < subArrayTwo) {
//         if (leftArray[indexOfSubArrayOne]
//             <= rightArray[indexOfSubArrayTwo]) {
//             array[indexOfMergedArray]
//                 = leftArray[indexOfSubArrayOne];
//             indexOfSubArrayOne++;
//         }
//         else {
//             array[indexOfMergedArray]
//                 = rightArray[indexOfSubArrayTwo];
//             indexOfSubArrayTwo++;
//         }
//         indexOfMergedArray++;
//     }

//     // Copy the remaining elements of
//     // left[], if there are any
//     while (indexOfSubArrayOne < subArrayOne) {
//         array[indexOfMergedArray]
//             = leftArray[indexOfSubArrayOne];
//         indexOfSubArrayOne++;
//         indexOfMergedArray++;
//     }

//     // Copy the remaining elements of
//     // right[], if there are any
//     while (indexOfSubArrayTwo < subArrayTwo) {
//         array[indexOfMergedArray]
//             = rightArray[indexOfSubArrayTwo];
//         indexOfSubArrayTwo++;
//         indexOfMergedArray++;
//     }
//     delete[] leftArray;
//     delete[] rightArray;
// }

//v Textbook merge
template <class T>
void merge(T a[], unsigned int s1, unsigned int s2){

    //create a pointer for the temp array;
    int *temp; 
    
    //var copied is for the number of copied has been put into the tmp array
    int copied = 0;
    
    // copied_1 keep track of number of copied element from the first half
    int copied_1 = 0;
    int copied_2 = 0;
    int i;

    // allocate memory for the new array with the size of both sub array
    temp = new int[s1 + s2];

    //This case is when the elements in both the array
    while(copied_1 < s1 && copied_2 < s2){
        if(a[copied_1] < (a + s1)[copied_2]){
            temp[copied] = a[copied_1]; 
            copied++;
            copied_1++;
        }
        else{
            temp[copied] = (a + s1)[copied_2]; 
            copied++;
            copied_2++;
        }
    }

    //here are the case when 1 of them get marked already, so we just sort the other side
    while(copied_1 < s1){
        temp[copied] = a[copied_1]; 
        copied++;
        copied_1++;
    }

    while(copied_2 < s2){
        temp[copied] = (a + s1)[copied_2]; 
        copied++;
        copied_2++;
    }


    //just merge all 2 array together back to the original one

    for(i = 0; i < s1 + s2; i++){
        a[i] = temp[i];
    }

    delete [] temp;
}

int partition(int arr[],int low,int high);

// Generates Random Pivot, swaps pivot with
// end element and calls the partition function
int partition_r(int arr[], int low, int high);
 
//Primary Helpers
template <class T>
void mergeSort(T a[], unsigned int const begin, unsigned int const end)
{
    if(begin < end)
    {
        int mid = begin + (end - begin) / 2;
        mergeSort(a, begin, mid);
        mergeSort(a, mid + 1, end);
        merge(a, begin, mid, end);
    }
}
template<class T>
void quickSort(T arr[],int low,int high)
{
  // when low is less than high
  const bool debug = false;
  if(debug)
    cout<<"quickSort() fired.\n";
  if(low<high)
  {
    // pi is the partition return index of pivot
    
    int pi= partition(arr,low,high);
    
    //Recursion Call
    //smaller element than pivot goes left and
    //higher element goes right
    quickSort(arr,low,pi-1);
    quickSort(arr,pi+1,high);
  }
}
/* The main function that implements
QuickSort
arr[] --> Array to be sorted,
low --> Starting index,
high --> Ending index */
// template<class T>
// void quickSort(T arr[], int low, int high)
// {
//     if (low < high) {
 
//         /* pi is partitioning index,
//         arr[p] is now
//         at right place */
//         int pi = partition_r(arr, low, high);
 
//         // Separately sort elements before
//         // partition and after partition
//         quickSort(arr, low, pi - 1);
//         quickSort(arr, pi + 1, high);
//     }
// }
void heapify(int arr[], int N, int i);

//main funcs
// template <class T>
// void merge_sort(T a[], unsigned int size)
// {
//     mergeSort(a, 0, size -1);
// }
template <class T>
void merge_sort(T a[], unsigned int size){
    //Use recursive
    //The idea is to separate the array into smaller arrays and sort each of them then combine everything
    int size_1, size_2;

    //Base case is when there is only 1 element in the array
    if(size > 1){

        //Calculate the size 2 sub array to recursively sort it

        size_1 = size / 2;
        size_2 = size - size_1;
        //sort the first half
        merge_sort(a, size_1);
        //sort the second half
        merge_sort((a + size_1), size_2);

        //combine all of the subset of each level
        merge(a, size_1, size_2);
    }

}
template <class T>
void quick_sort(T a[], unsigned int size)
{
    quickSort(a, 0, size - 1);
}
    
template <class T>
void heap_sort(T a[], unsigned int size)
{
    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(a, size, i);

    // One by one extract an element
    // from heap
    for (int i = size - 1; i > 0; i--) {

        // Move current root to end
        Swap(a[0], a[i]);

        // call max heapify on the reduced heap
        // reduces the size of the heap on every recursive call
        heapify(a, i, 0);
    }
}
sort_f_ptr get_sort_func_ptr(int func_name);
string print_func_name(int f_name);
int get_sort_func_name(sort_f_ptr f);

#endif // ZAC_SORT_FUNCTIONS_