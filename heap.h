/* Based on "Type-safe generic data structures in C" by Ian Fisher (https://iafisher.com/blog/2020/06/type-safe-generics-in-c) */

#include <stdlib.h>
#include <stdio.h>

/*
 * Declare a min heap with elements of type "type".
 * All declared functions will have the signature type_name_heap_FUNCTION.
 * The heap struct will have the signature type_name_Heap.
 * type_compare should return 0 if elements are equal, <0 if first element is bigger, >0 otherwise.
 * 
 * WARNING: DECL_HEAP with a specific type_name may only be used once in a program.
 * Recommended usage:
 * #ifndef TYPE_NAME_HEAP
 * #define TYPE_NAME_HEAP
 * DECL_HEAP(type, type_name, type_compare)
 * #endif
 * 
 * @param type
 * @param type_name
 * @param type_compare
 */
#define DECL_HEAP(type, type_name, type_compare)                                                           \
    typedef struct type_name##_Heap                                                                        \
    {                                                                                                      \
        size_t capacity;                                                                                   \
        size_t size;                                                                                       \
        type *elements;                                                                                    \
    } type_name##_Heap;                                                                                    \
                                                                                                           \
    /*                                                                                                     \
     * Instanciates a dynamically allocated heap struct and returns a pointer to it.                       \
     *                                                                                                     \
     * @param size_t capacity                                                                              \
     * @return Heap *heap                                                                                  \
     */                                                                                                    \
    type_name##_Heap *type_name##_heap_create(size_t capacity)                                             \
    {                                                                                                      \
        type_name##_Heap *heap = malloc(sizeof(type_name##_Heap));                                         \
                                                                                                           \
        if (heap == NULL)                                                                                  \
        {                                                                                                  \
            return NULL;                                                                                   \
        }                                                                                                  \
                                                                                                           \
        /* Note: To easily compute parent and child indecies of elements,                                  \
           heap->elements[0] is not used, the top element is at index 1. */                                \
        heap->elements = malloc(sizeof(type) * (capacity + 1));                                            \
                                                                                                           \
        if (heap->elements == NULL)                                                                        \
        {                                                                                                  \
            free(heap);                                                                                    \
                                                                                                           \
            return NULL;                                                                                   \
        }                                                                                                  \
                                                                                                           \
        heap->size = 0;                                                                                    \
        heap->capacity = capacity;                                                                         \
                                                                                                           \
        return heap;                                                                                       \
    }                                                                                                      \
                                                                                                           \
    /*                                                                                                     \
     * Deallocates the heap struct and its elements.                                                       \
     *                                                                                                     \
     * @param Heap *heap                                                                                   \
     */                                                                                                    \
    void type_name##_heap_destroy(type_name##_Heap *heap)                                                  \
    {                                                                                                      \
        free(heap->elements);                                                                              \
        free(heap);                                                                                        \
                                                                                                           \
        heap = NULL;                                                                                       \
                                                                                                           \
        return;                                                                                            \
    }                                                                                                      \
                                                                                                           \
    /*                                                                                                     \
     * Sorting from the last heap element upwards.                                                         \
     *                                                                                                     \
     * @param Heap *heap                                                                                   \
     */                                                                                                    \
    void type_name##_heap_sort_up(type_name##_Heap *heap)                                                  \
    {                                                                                                      \
        /* sorting is only necessary if there is more than one element */                                  \
        if (heap->size > 1)                                                                                \
        {                                                                                                  \
            /* start at the bottom */                                                                      \
            size_t currentIndex = heap->size;                                                              \
            size_t parentIndex;                                                                            \
            /* temporarily save last element as the one to be sorted up */                                 \
            type sortElement = heap->elements[currentIndex];                                               \
                                                                                                           \
            while (currentIndex > 1)                                                                       \
            {                                                                                              \
                parentIndex = currentIndex / 2;                                                            \
                                                                                                           \
                if (type_compare(heap->elements[parentIndex], sortElement) < 0)                            \
                {                                                                                          \
                    /* move parent element down if it's greater than the current one */                    \
                    heap->elements[currentIndex] = heap->elements[parentIndex];                            \
                    currentIndex = parentIndex;                                                            \
                }                                                                                          \
                else                                                                                       \
                {                                                                                          \
                    /* currentIndex is correct position for sortElement */                                 \
                    break;                                                                                 \
                }                                                                                          \
            }                                                                                              \
                                                                                                           \
            /* insert sortElement at determined position */                                                \
            heap->elements[currentIndex] = sortElement;                                                    \
        }                                                                                                  \
                                                                                                           \
        return;                                                                                            \
    }                                                                                                      \
                                                                                                           \
    /*                                                                                                     \
     * Sorting from the top heap element downwards.                                                        \
     *                                                                                                     \
     * @param Heap *heap                                                                                   \
     */                                                                                                    \
    void type_name##_heap_sort_down(type_name##_Heap *heap)                                                \
    {                                                                                                      \
        /* sorting is only necessary if there is more than one element */                                  \
        if (heap->size > 1)                                                                                \
        {                                                                                                  \
            /* start at the top */                                                                         \
            size_t currentIndex = 1;                                                                       \
            size_t leftChildIndex, rightChildIndex;                                                        \
            /* temporarily save top element as the one to be sorted down */                                \
            type sortElement = heap->elements[currentIndex];                                               \
                                                                                                           \
            while (1)                                                                                      \
            {                                                                                              \
                /* for element n: left child at 2n, right child at 2n+1 */                                 \
                leftChildIndex = 2 * currentIndex;                                                         \
                rightChildIndex = leftChildIndex + 1;                                                      \
                                                                                                           \
                if (leftChildIndex <= heap->size)                                                          \
                {                                                                                          \
                    /* left child exists */                                                                \
                    if (rightChildIndex <= heap->size &&                                                   \
                        type_compare(sortElement, heap->elements[rightChildIndex]) < 0 &&                  \
                        type_compare(heap->elements[rightChildIndex], heap->elements[leftChildIndex]) > 0) \
                    {                                                                                      \
                        /* right child exists, is smaller than sortElement and left child                  \
                           => move right child up, move on from right child index */                       \
                        heap->elements[currentIndex] = heap->elements[rightChildIndex];                    \
                        currentIndex = rightChildIndex;                                                    \
                    }                                                                                      \
                    else if (type_compare(sortElement, heap->elements[leftChildIndex]) < 0)                \
                    {                                                                                      \
                        /* left child is smaller than sortElement                                          \
                           => move left child up, move on from left child index */                         \
                        heap->elements[currentIndex] = heap->elements[leftChildIndex];                     \
                        currentIndex = leftChildIndex;                                                     \
                    }                                                                                      \
                    else                                                                                   \
                    {                                                                                      \
                        /* currentIndex is correct position for sortElement */                             \
                        break;                                                                             \
                    }                                                                                      \
                }                                                                                          \
                else                                                                                       \
                {                                                                                          \
                    /* reached the top of the heap */                                                      \
                    break;                                                                                 \
                }                                                                                          \
            }                                                                                              \
                                                                                                           \
            /* insert sortElement at determined position */                                                \
            heap->elements[currentIndex] = sortElement;                                                    \
        }                                                                                                  \
                                                                                                           \
        return;                                                                                            \
    }                                                                                                      \
                                                                                                           \
    /*                                                                                                     \
     * Inserts a new element into the heap.                                                                \
     *                                                                                                     \
     * @param Heap *heap                                                                                   \
     * @param type element                                                                                 \
     */                                                                                                    \
    void type_name##_heap_push(type_name##_Heap *heap, type element)                                       \
    {                                                                                                      \
        /*increment heap size, insert element at the bottom of the heap and sort up */                     \
        heap->elements[++heap->size] = element;                                                            \
        type_name##_heap_sort_up(heap);                                                                    \
                                                                                                           \
        return;                                                                                            \
    }                                                                                                      \
                                                                                                           \
    /*                                                                                                     \
     * Removes and returns the top element from the heap.                                                  \
     *                                                                                                     \
     * @param Heap *heap                                                                                   \
     * @return type element                                                                                \
     */                                                                                                    \
    type type_name##_heap_pop(type_name##_Heap *heap)                                                      \
    {                                                                                                      \
        type element;                                                                                      \
                                                                                                           \
        if (heap->size > 0)                                                                                \
        {                                                                                                  \
            element = heap->elements[1];                                                                   \
                                                                                                           \
            /* move last element of the heap to the top, decrement heap size and sort down */              \
            heap->elements[1] = heap->elements[heap->size--];                                              \
            type_name##_heap_sort_down(heap);                                                              \
        }                                                                                                  \
        else                                                                                               \
        {                                                                                                  \
            printf("[Error]: attempting to get element from empty heap\nTerminating...\n");                \
            exit(1);                                                                                       \
        }                                                                                                  \
                                                                                                           \
        return element;                                                                                    \
    }
