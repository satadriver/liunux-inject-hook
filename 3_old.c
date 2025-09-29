#define _NETBSD_SOURCE
#include <sys/param.h>
#include <kvm.h>
#include <fcntl.h>      // 包含 O_RDWR, O_RDONLY 等常量

#define _NETBSD_SOURCE
#include <sys/param.h>
#include <kvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kvm.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/sysctl.h>
#include <errno.h>

// 读取进程内存
int read_process_memory(kvm_t *kd, pid_t pid, unsigned long addr, void *buf, size_t len) {
    struct kinfo_proc *proc;
    int count;
    
    // 获取进程信息
    proc = kvm_getprocs(kd, KERN_PROC_PID, pid, &count);
    if (proc == NULL || count <= 0) {
        fprintf(stderr, "无法获取进程 %d 的信息: %s\n", pid, kvm_geterr(kd));
        return -1;
    }
    
    // 读取内存
    ssize_t bytes_read = kvm_read(kd, addr, buf, len);
    if (bytes_read < 0) {
        fprintf(stderr, "读取内存失败: %s\n", kvm_geterr(kd));
        return -1;
    }
    
    if ((size_t)bytes_read != len) {
        fprintf(stderr, "只读取了 %zd 字节，预期 %zu 字节\n", bytes_read, len);
        return -1;
    }
    
    return 0;
}

// 写入进程内存
int write_process_memory(kvm_t *kd, pid_t pid, unsigned long addr, const void *buf, size_t len) {
    struct kinfo_proc *proc;
    int count;
    
    // 获取进程信息
    proc = kvm_getprocs(kd, KERN_PROC_PID, pid, &count);
    if (proc == NULL || count <= 0) {
        fprintf(stderr, "无法获取进程 %d 的信息: %s\n", pid, kvm_geterr(kd));
        return -1;
    }
    
    // 写入内存
    ssize_t bytes_written = kvm_write(kd, addr, buf, len);
    if (bytes_written < 0) {
        fprintf(stderr, "写入内存失败: %s\n", kvm_geterr(kd));
        return -1;
    }
    
    if ((size_t)bytes_written != len) {
        fprintf(stderr, "只写入了 %zd 字节，预期 %zu 字节\n", bytes_written, len);
        return -1;
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    kvm_t *kd;
    pid_t pid;
    unsigned long addr;
    int value, new_value;
    char errbuf[1024];
    
    // 检查命令行参数
    if (argc != 3) {
        fprintf(stderr, "用法: %s <pid> <address>\n", argv[0]);
        return 1;
    }
    
    // 解析PID和地址
    pid = atoi(argv[1]);
    addr = strtoul(argv[2], NULL, 16);  // 假设地址是十六进制
    
    // 打开kvm
    kd = kvm_openfiles(NULL, NULL, NULL, O_RDWR, errbuf);
    if (kd == NULL) {
        fprintf(stderr, "无法打开kvm: %s\n", errbuf);
        return 1;
    }
    
    // 读取内存值
    printf("读取进程 %d 地址 0x%lx 的值...\n", pid, addr);
    if (read_process_memory(kd, pid, addr, &value, sizeof(value)) == 0) {
        printf("当前值: %d\n", value);
        
        // 写入新值
        new_value = value + 1;  // 简单地加1
        printf("写入新值: %d...\n", new_value);
        if (write_process_memory(kd, pid, addr, &new_value, sizeof(new_value)) == 0) {
            // 验证写入结果
            int verify_value;
            if (read_process_memory(kd, pid, addr, &verify_value, sizeof(verify_value)) == 0) {
                printf("验证写入结果: %d\n", verify_value);
                if (verify_value == new_value) {
                    printf("写入成功\n");
                } else {
                    printf("写入验证失败\n");
                }
            }
        }
    }
    
    // 关闭kvm
    kvm_close(kd);
    return 0;
}
