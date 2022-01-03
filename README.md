# Heap
An implementation of a generic min heap in (ANSI) C.

This is inspired by and based on "Type-safe generic data structures in C" by Ian Fisher (https://iafisher.com/blog/2020/06/type-safe-generics-in-c).

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
  
  /* initialize the heap */
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
