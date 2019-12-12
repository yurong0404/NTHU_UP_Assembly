#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <fcntl.h>

void errquit(const char *msg)
{
	perror(msg);
	exit(-1);
}

int main(int argc, char *argv[])
{
	pid_t child;
	if(argc < 2)
	{
		fprintf(stderr, "usage: %s program\n", argv[0]);
		return -1;
	}
	if((child = fork()) < 0)
		errquit("fork");
	if(child == 0)
	{
		if(ptrace(PTRACE_TRACEME, 0, 0, 0) < 0)
			errquit("ptrace");
		execvp(argv[1], argv+1);
		errquit("execvp");
	}
	else
	{
		int status;
		long output;
		long opcode;
		long addr;
		long rip;
		
		struct user_regs_struct regs;
		int fd;
		char buffer[3];

		fd = open("./no_more_traps.txt", O_RDONLY);

		if(waitpid(child, &status, 0) < 0)  // execvp
			errquit("waitpid");
		ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_EXITKILL|PTRACE_O_TRACEEXIT);
		ptrace(PTRACE_CONT, child, 0, 0);
		waitpid(child, &status, 0); // first 0xcc
		int counter=0;
		while(!WIFEXITED(status))
		{
			if(ptrace(PTRACE_GETREGS, child, 0, &regs) == -1)
			{
				fprintf(stderr, "get register fail\n");
				pause();
			}
			output = ptrace(PTRACE_PEEKDATA, child, regs.rip-1, 0);
			if ((output&255) == 0xcc)
			{
				counter++;
				read(fd, buffer, 2);
				buffer[2] = '\0';
				opcode = strtol(buffer, NULL, 16);
				opcode |= (output & 0xffffffffffffff00);
				if (ptrace(PTRACE_POKETEXT, child, regs.rip-1, opcode) == -1)
				{
					fprintf(stderr, "write fail\n");
					pause();
				}

				regs.rip -= 1;
				if (ptrace(PTRACE_SETREGS, child, 0, &regs) == -1)
				{
					fprintf(stderr, "rip setting fail\n");
				}
			}
			ptrace(PTRACE_CONT, child, 0, 0);
			waitpid(child, &status, 0);
		}
		close(fd);
		printf("replace total %d 0xcc\n", counter);
	}
	return 0;
}
