#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>

#include <sys/wait.h> 

int main(int argc, char* argv[])
{
	pid_t attack_pid = -1;
	char  val [8]       = {0x12,0x34,0x56,0x78};
	
	int p_type = 0;
	
	printf("usage: %s pid(pid < 0 is PTRACE_TRACEME)\n",argv[0]);
 
	if (argc < 2 || argv[1] <= 0)
	{
		attack_pid = 0;
		p_type = PT_TRACE_ME;
	}
	else{
		
		attack_pid = strtoul(argv[1], 0, 10);
		
		p_type = PT_ATTACH;
	}
	
    if (ptrace(p_type, attack_pid, NULL, NULL) < 0)
	{
		printf("attach failed\n");
		return 0;
	}
	printf("attach ok\n");
	//ptrace (PT_DETACH, attack_pid, NULL, NULL);
	//waitpid(attack_pid, NULL, WUNTRACED);
	//return 0;
	
    int status;
    waitpid(attack_pid, &status, WUNTRACED);
    if (!WIFSTOPPED(status)) {
        //std::cerr << "进程未正常暂停" << std::endl;
		printf("waitpid error\r\n");
        ptrace(PT_DETACH, attack_pid, 0, 0);
        return 0;
    }

	//0x3286708
	//0x025E9BC8
	char * address = 0x3286708;
	if(argc >= 3){
		address = strtoul(argv[2], 0, 16);	
	}
    //读取数据
	
	unsigned int value = ptrace(PT_READ_D , attack_pid, (void*)address, NULL);
	
	//unsigned int value = ptrace(PT_READ_D , attack_pid, (void*)0x025E9BC8, NULL);
	printf("0x025E9BC8: %x\n", value);

    //修改数据
	//ptrace(PT_POKETEXT , attack_pid, (void*)0x3286708, val[0]);

	if(value && value!= 0xffffffff){
		int i = 0;
		
		unsigned int * lpptr = (unsigned int *)value;
		unsigned int * ptr = *lpptr;

		for(i = 0;i < 0x40;i ++){
			unsigned int v = ptr[i];
			printf("address:%x, value:%x\n", ptr + i,v);
		}
		
	}
 
 	//printf("global1 %x\n", ptrace(PT_READ_D , attack_pid, (void*)0x025E9BC8, NULL));

	
	ptrace (PT_DETACH, attack_pid, NULL, NULL);
	waitpid(attack_pid, NULL, WUNTRACED);
	return 0;
}
 
 
