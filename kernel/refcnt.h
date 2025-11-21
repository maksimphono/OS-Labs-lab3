// kernel/refcnt.h
#ifndef REFCNT_H
#define REFCNT_H

#include "spinlock.h"  // Assuming spinlock is defined here

// Reference counter structure for physical pages
typedef struct {
    struct spinlock lock;  // Spinlock to protect access
    int count[(PGROUNDUP(PHYSTOP) - KERNBASE)/PGSIZE];  // Count array for physical pages
} refcnt_t;

// Declare a global variable to hold the reference counter
extern refcnt_t refcnt; 

void refcnt_init();  // Function to initialize the reference counter
void increment_ref(int page_num);  // Increment the reference count for a page
void decrement_ref(int page_num);  // Decrement the reference count for a page
int get_ref(int page_num);  // Get the current reference count for a page

#endif // REFCNT_H
