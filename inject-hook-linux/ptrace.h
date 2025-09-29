

#include <sys/ptrace.h>    // ptrace 相关函数
#include <sys/types.h>     // pid_t 等类型定义
#include <sys/wait.h>      // waitpid 等函数

#ifdef __NetBSD__
#define PTRACE_ATTACH_MACRO PT_ATTACH
#define PTRACE_PEEKDATA_MACRO PT_PEEKDATA
#define PTRACE_PEEKTEXT_MACRO PT_PEEKTEXT
#define PTRACE_PEEKDATA_MACRO PT_PEEKDATA
#define PTRACE_GETSIGINFO_MACRO PT_GETSIGINFO
#define PTRACE_POKETEXT_MACRO PT_POKETEXT
#define PTRACE_CONT_MACRO PT_CONT
#define PTRACE_DETACH_MACRO PT_DETACH
#else
#define PTRACE_ATTACH_MACRO PTRACE_ATTACH
#define PTRACE_PEEKDATA_MACRO PTRACE_PEEKDATA
#define PTRACE_PEEKTEXT_MACRO PTRACE_PEEKTEXT
#define PTRACE_PEEKDATA_MACRO PTRACE_PEEKDATA
#define PTRACE_GETSIGINFO_MACRO PTRACE_GETSIGINFO
#define PTRACE_POKETEXT_MACRO PTRACE_POKETEXT
#define PTRACE_CONT_MACRO PTRACE_CONT
#define PTRACE_DETACH_MACRO PTRACE_DETACH
#endif

void ptrace_attach(pid_t target);
void remote_stop(pid_t target);
void ptrace_detach(pid_t target);
void ptrace_getregs(pid_t target, struct user_regs_struct *regs);
void ptrace_cont(pid_t target);
void ptrace_setregs(pid_t target, const struct user_regs_struct *regs);
siginfo_t ptrace_getsiginfo(pid_t target);
void ptrace_read(int pid, unsigned long addr, void *vptr, int len);
void ptrace_write(int pid, unsigned long addr, void *vptr, int len);
void checktargetsig(int pid);
void restoreStateAndDetach(pid_t target, unsigned long addr, void* backup, int datasize, struct user_regs_struct oldregs);
