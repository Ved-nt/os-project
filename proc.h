// These are header guards to prevent the file from being included multiple times during compilation
#ifndef PROC_H
#define PROC_H

// It is essential for mutual exclusion especially in the process table declared as the variable 'ptable'
#include "spinlock.h"

// Maximum processes
#define NPROC 64

// Process states
enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID
  struct context *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  struct segdesc gdt[NSEGS];   // x86 global descriptor table
  volatile uint started;       // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting
  int intena;                  // Were interrupts enabled before pushcli?
  struct proc *proc;           // The process running on this CPU or null
};

extern struct cpu cpus[NCPU]; // It declares an array of CPU structures. cpus[] is an array that holds the state of each CPU
                              // It is defined as 'extern' as the actual definition of this variable is in another source file (proc.c)

extern int ncpu;  // It declares the total number of CPU cores available to the system. Its actual definition is also defined in another file

// Context for context switching
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)

  // Added dynamic scheduling fields
  int time_quantum;            // Current time quantum
  int execution_count;         // Number of times process was scheduled
  int last_runtime;            // Last time process ran
  int total_cpu_ticks;         // Total CPU time consumed
};

// Process table
struct ptable_struct {
  struct spinlock lock;
  struct proc proc[NPROC];
};

// It declares the process table as an external variable, allowing it to be accessible across all kernel source files
extern struct ptable_struct ptable;

#endif // PROC_H
// It ends the header guard, initialised at the top of the file
