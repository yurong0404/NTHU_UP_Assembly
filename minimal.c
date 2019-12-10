#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
extern char __data_start;
extern char __bss_start;

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
		unsigned long output;
		unsigned long addr;
		if(waitpid(child, &status, 0) < 0) errquit("waitpid");
		ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_EXITKILL|PTRACE_O_TRACEEXIT);
		ptrace(PTRACE_CONT, child, 0, 0);
		waitpid(child, &status, 0);
		while(1)
		{
		printf("pid: %d\ndata section: \n", child);
		scanf("%lx", &addr);
		output = ptrace(PTRACE_PEEKTEXT, child, addr, 0);
//		if(output = ptrace(PTRACE_PEEKTEXT, child, 0x824, NULL) == -1) errquit("peektext");
		printf("%lx\n", output);
		}
		ptrace(PTRACE_CONT, child, 0, 0);
		waitpid(child, &status, 0);
		perror("done");
	}
	return 0;
}

