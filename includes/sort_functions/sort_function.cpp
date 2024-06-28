#ifndef ZAC_SORT_FUNCTION_CPP
#define ZAC_SORT_FUNCTION_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "sort_functions.h"
using namespace std;

//Precondi: func will only work with int arrays
int get_sort_func_name(sort_f_ptr f)
{
    //have to static_cast because sort_f_ptr is not templated
    if(f == static_cast<sort_f_ptr>(bubble_sort))
        return BUBBLE_SORT;
    else if(f == static_cast<sort_f_ptr>(inseration_sort))
        return INSERTION_SORT;
    else if(f == static_cast<sort_f_ptr>(quick_sort))
        return QUICK_SORT;
    else if(f == static_cast<sort_f_ptr>(merge_sort))
        return MERGE_SORT;
    else if(f == static_cast<sort_f_ptr>(heap_sort))
        return HEAP_SORT;
    else 
        return -1;
}

string print_func_name(int f_name)
{
    switch (f_name)
    {
    case BUBBLE_SORT:
        return "bubbleSort: ";
    case INSERTION_SORT:
        return "insertionSort: ";
    case MERGE_SORT:
        return "mergeSort: ";
    case QUICK_SORT:
        return "quickSort: ";
    case HEAP_SORT:
        return "heapSort: ";
    default:
        cout<<"default: get_sort_func_ptr\n";
        break;
    }
}


//returns a function pointer from a function
sort_f_ptr get_sort_func_ptr(int func_name)
{
    switch (func_name)
    {
    case BUBBLE_SORT:
        return bubble_sort;
    case INSERTION_SORT:
        return inseration_sort;
    case MERGE_SORT:
        return merge_sort;
    case QUICK_SORT:
        return quick_sort;
    case HEAP_SORT:
        return heap_sort;
    default:
        cout<<"default: get_sort_func_ptr\n";
        break;
    }
}

int partition(int arr[],int low,int high)
{
  //choose the pivot
  
  int pivot=arr[high];
  //Index of smaller element and Indicate
  //the right position of pivot found so far
  int i=(low-1);
  
  for(int j=low;j<=high;j++)
  {
    //If current element is smaller than the pivot
    // if(arr[j]<pivot)
    if(arr[j] < pivot)
    {
      //Increment index of smaller element
      i++;
      Swap(arr[i],arr[j]);
    }
  }
  Swap(arr[i+1],arr[high]);
  return (i+1);
}

// Generates Random Pivot, swaps pivot with
// end element and calls the partition function
int partition_r(int arr[], int low, int high)
{
    // Generate a random number in between
    // low .. high
    int random = low + rand() % (high - low);
 
    // Swap A[random] with A[high]
    Swap(arr[random], arr[high]);
 
    return partition(arr, low, high);
}

void heapify(int arr[], int N, int i)
{
    // Initialize largest as root
    int largest = i;

    // left = 2*i + 1
    int l = 2 * i + 1;

    // right = 2*i + 2
    int r = 2 * i + 2;

    // If left child is larger than root
    if (l < N && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest
    // so far
    if (r < N && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected
        // sub-tree
        heapify(arr, N, largest);
    }
}

#endif // ZAC_SORT_FUNCTION_