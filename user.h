struct stat;
struct rtcdate;

// This structure is used to store user-level process information retrieved via the custom system call getprocs() 
// It mirrors some fields from the kernel's struct proc, but is safe and clean for user-space access
struct uproc {
  int pid;
  int ppid;
  char state[16];
  char name[16];
  int time_quantum;
  int execution_count;
};

int getprocs(int, struct uproc *);
// This declares the getprocs() function so that user programs can call it just like fork(), write(), etc.
// This line is needed so the linker can resolve the function when compiling user-level C programs.


// system calls
int getprocs(int max, struct uproc*);
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int yield(void); // This makes the yield() system call accessible from user space


// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
