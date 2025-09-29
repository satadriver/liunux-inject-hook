#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

/**
 * 读取目标进程指定地址的内存数据（通过 /proc 文件系统）
 * @param pid 目标进程PID
 * @param addr 要读取的内存地址（十六进制）
 * @param buf 接收数据的缓冲区
 * @param size 要读取的字节数
 * @return 成功返回读取的字节数，失败返回-1
 */
ssize_t read_process_mem(pid_t pid, uintptr_t addr, void *buf, size_t size) {
    if (buf == NULL || size == 0) {
        fprintf(stderr, "无效的缓冲区或大小\n");
        return -1;
    }

    // 构建 /proc/<pid>/mem 文件路径
    char mem_path[64];
    snprintf(mem_path, sizeof(mem_path), "/proc/%d/mem", pid);

    // 打开目标进程的内存文件（需root权限）
    int fd = open(mem_path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "打开 %s 失败: %s\n", mem_path, strerror(errno));
        return -1;
    }

    // 定位到目标地址
    off_t off = lseek(fd, addr, SEEK_SET);
    if (off == -1) {
        fprintf(stderr, "定位到地址 0x%lx 失败: %s\n", addr, strerror(errno));
        close(fd);
        return -1;
    }

    // 读取数据
    ssize_t bytes_read = read(fd, buf, size);
    if (bytes_read == -1) {
        fprintf(stderr, "读取内存失败: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    close(fd);
    return bytes_read;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "用法: %s <PID> <地址(十六进制)> <读取字节数>\n", argv[0]);
        fprintf(stderr, "示例: %s 1234 0x7f000000 16\n", argv[0]);
        return 1;
    }

    // 解析命令行参数
    pid_t pid = atoi(argv[1]);
    uintptr_t addr = strtoull(argv[2], NULL, 16);
    size_t size = atoi(argv[3]);

    // 分配缓冲区
    uint8_t *buf = malloc(size);
    if (buf == NULL) {
        perror("malloc 失败");
        return 1;
    }

	size_t i = 0;
    // 读取内存
    printf("读取进程 %d 的地址 0x%lx，共 %zu 字节...\n", pid, addr, size);
    ssize_t ret = read_process_mem(pid, addr, buf, size);
    if (ret > 0) {
        // 打印十六进制格式
        printf("十六进制数据:\n");
        for ( i = 0; i < ret; i++) {
            printf("%02x ", buf[i]);
            if ((i + 1) % 16 == 0) printf("\n");
        }
        if (ret % 16 != 0) printf("\n");

        // 打印ASCII格式
        printf("ASCII数据:\n");
        for ( i = 0; i < ret; i++) {
            if (buf[i] >= 32 && buf[i] <= 126) {
                printf("%c", buf[i]);
            } else {
                printf(".");
            }
            if ((i + 1) % 16 == 0) printf("\n");
        }
        if (ret % 16 != 0) printf("\n");
    } else if (ret == 0) {
        printf("未读取到数据（可能地址无效或已超出进程内存范围）\n");
    }

    free(buf);
    return 0;
}


