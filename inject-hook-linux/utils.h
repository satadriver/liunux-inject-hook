#define INTEL_RET_INSTRUCTION 0xc3
#define INTEL_INT3_INSTRUCTION 0xcc

//#define LIBC_PATH "libc.so.6"
#define LIBC_PATH "libc.so"

#define PATH_MAX 1024


#ifdef __NetBSD__
#define PTRACE_ATTACH_MACRO PT_ATTACH

#define PTRACE_PEEKTEXT_MACRO PT_PEEKTEXT
#define PTRACE_PEEKDATA_MACRO PT_READ_D
#define PTRACE_GETSIGINFO_MACRO PT_GETSIGINFO
#define PTRACE_POKETEXT_MACRO PT_POKETEXT
#define PTRACE_CONT_MACRO PT_CONT
#define PTRACE_DETACH_MACRO PT_DETACH
#else
#define PTRACE_ATTACH_MACRO PTRACE_ATTACH

#define PTRACE_PEEKTEXT_MACRO PTRACE_PEEKTEXT
#define PTRACE_PEEKDATA_MACRO PTRACE_PEEKDATA

#define PTRACE_GETSIGINFO_MACRO PTRACE_GETSIGINFO
#define PTRACE_POKETEXT_MACRO PTRACE_POKETEXT
#define PTRACE_CONT_MACRO PTRACE_CONT
#define PTRACE_DETACH_MACRO PTRACE_DETACH
#endif

pid_t findProcessByName(char* processName);
long freespaceaddr(pid_t pid);
long getlibcaddr(pid_t pid);
int checkloaded(pid_t pid, char* libname);
long getFunctionAddress(char* funcName);
unsigned char* findRet(void* endAddr);
void usage(char* name);
int dump_memory(pid_t pid, uintptr_t startAddress, uintptr_t endAddress);
