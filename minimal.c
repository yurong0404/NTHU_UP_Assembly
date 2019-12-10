#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <string.h>

void errquit(const char *msg) {
	perror(msg);
	exit(-1);
}

int main(int argc, char *argv[]) {
	pid_t child;
	if(argc < 2) {
		fprintf(stderr, "usage: %s program\n", argv[0]);
		return -1;
	}
	if((child = fork()) < 0) errquit("fork");
	if(child == 0) {
		if(ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) errquit("ptrace");
		execvp(argv[1], argv+1);
		errquit("execvp");
	} else {
		int status;
		long output;
		long addr;
		if(waitpid(child, &status, 0) < 0) errquit("waitpid");
		ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_EXITKILL|PTRACE_O_TRACEEXIT);
		ptrace(PTRACE_CONT, child, 0, 0);
		waitpid(child, &status, 0);
		ptrace(PTRACE_CONT, child, 0, 0);
		waitpid(child, &status, 0);	
		printf("pid: %d\ndata section: \n", child);
		scanf("%lx", &addr);
		char str[100];
		for (int i=0; i<10;i++)
		{
			output = ptrace(PTRACE_PEEKDATA, child, addr+i*8, 0);
			memcpy(str+i*8, &output, 8);
			//printf("%s", &output);
		}
		printf("%s", str);
		printf("\n");
		perror("done");
	}
	return 0;
}
