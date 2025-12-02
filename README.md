# Author: 马克西姆. Notes:

## Main changes in the files:
### Pages copying
* file: `kernel/vm.c`
* function: `uvmcopy`

    Created a way of copying PTEs from old pagetable to new, setting COW flag and unsetting R/W flag, pages content is not copied just PTEs. New created PTEs are mapped to same memory as the old ones. Reference counter in increased for every PTE.

### Page fault handling
* file: `kernel/trap.c`
* function: `usertrap`

    Implemented a way of handling page fault, caused by the process attempting to write to the page, shared via COW. Can distinguish between actual COW page fault and regular attempt to write into read-only page by examining the flags. When creating the copies of pages, corresponding PTEs immediately are mappoed to these pages using `mappages` (core of the COW mechanism). Lastly COW flag is unset and R/W flag is set for this PTE, reference counter for this page is decreased. 
    When memory isn't enough or PTEs are inconsistent - process is killed.

### Kernel to user copying
* file: `kernel/vm.c`
* function: `copyout`

    Implemented a way of copying several bytes from kernel space into the user space, respecting COW mechanism and creating copies of pages only when necessary (like in the page fault handler). After copying COW flag is unset, R/W flag is set, reference counter for that page is decreased.
    When memory isn't enough or PTEs are inconsistent - return error.

## Changes to forbidden files:

1.  File: `kernel/refcnt.c`
    Reason: This is a dedicated file to define global structure for the reference counter, including all corresponding methods definition. This was added to store definition of reference counter in a separate place.

2.  File: `kernel/main.c`
    Reason: Since I can't add source files and compile the kernel with them (it's not allowed to change compilation script), I have to implement all function, related to the reference counter here in the place where is't easy to find

3.  File: `kernel/spinlock.c`
    Reason: I have to use standard C technique "ifndef - define" for defining header files to avoid error
    ```
    kernel/spinlock.h:5:8: error: redefinition of 'struct spinlock'
    ```
    This technique is considered a good practice, so I didn't see the reason not to use it

---

# OS Lab Based on Xv6 for RISC-V

This repository contains the Xv6 operating system, which is used for the Operating Systems lab course at Nanjing University. Xv6 is a simple Unix-like teaching operating system developed by MIT for educational purposes. For more information, see [6.1810: Operating System Engineering](https://pdos.csail.mit.edu/6.1810/2024/index.html).

## Requirements

This project requires the following tools to be installed: RISC-V versions of QEMU 7.2+, GDB 8.3+, GCC, and Binutils. For more information, see [Tools Used in 6.1810](https://pdos.csail.mit.edu/6.828/2024/tools.html).

## Basic Usage

To get started with Xv6, follow these steps:

1. **Clone the repository:**
    ```bash
    $ git clone git@git.nju.edu.cn/oslab/xv6-riscv.git
    $ cd xv6-riscv
    ```

2. **Run Xv6 in QEMU:**
    ```bash
    $ make qemu
    ```

## Course Webpage

For more details about the course, visit the [Operating Systems Webpage](https://gist.nju.edu.cn/course-os/).
