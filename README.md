# Heap
An implementation of a generic min heap in (ANSI) C.

This is inspired by and based on "Type-safe generic data structures in C" by Ian Fisher (https://iafisher.com/blog/2020/06/type-safe-generics-in-c).

# General
The implementation is based on a makro:
DECL_HEAP(type, type_name, type_compare)

This declares a min heap with elements of type "type".
All declared functions will have the signature type_name_heap_FUNCTION.
The heap struct will have the signature type_name_Heap.
The type_compare function should accept two elements of type "type" as arguments and
return 0 if elements are equal, <0 if first element is bigger, >0 otherwise.

# Usage (example for int min heap)
```c
/* Comparison function for the chosen type. */
int int_compare(int a, int  b)
{
    return b - a;
}

/* invoke the makro */

#ifndef INT_HEAP
#define INT_HEAP
DECL_HEAP(int, Int, int_compare)
#endif

int main(void) {
  Int_Heap* heap;
  int popped;
  
  /* initialize the heap with a capacity of 5 elements */
  heap = Int_heap_create(5);
  
  /* push and pop some values */
  Int_heap_push(2);
  Int_heap_push(19);
  Int_heap_push(1);
  
  popped = Int_heap_pop();
  
  /* destroy the heap (deallocates memory) */
  Int_heap_destroy(heap);
  
  return 0;
}

```
