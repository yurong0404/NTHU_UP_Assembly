#include <capstone/capstone.h>
#include <stdio.h>
#include <string.h>

char* byteStrToInt(char *input)
{
	char *result;
	int strsz = strlen(input);
	result = malloc(sizeof(char)*((strsz/2)+1));
	int i;
	for(i=0; i<(strsz/2);i++)
	{
		char **ptr;
		char bytestr[2];
		bytestr[0] = input[2*i];
		bytestr[1] = input[2*i+1];
		result[i] = (unsigned char) strtol(bytestr, ptr, 16);
	}
	result[i] = '\0';
	return result;
}
	
int main()
{
	static csh cshandle = 0;
	if(cs_open(CS_ARCH_X86, CS_MODE_64, &cshandle) != CS_ERR_OK)
		return -1;

	unsigned char buf[400];
	int count;
	
	while(1)
	{
		char *code;
		memset(buf, 0, 400);
		printf("quiz: ");
		gets(buf);
	
		code = byteStrToInt(buf);
		
		cs_insn *insn;
		if ((count = cs_disasm(cshandle, (uint8_t*) code, strlen(code), 0, 0, &insn)) > 0)
		{
			for (int i=0; i<count; i++)
			{
				for(int x=0; insn[i].mnemonic[x]!='\0'; x++)
					printf("%02x", insn[i].mnemonic[x]);
				
				if (strlen(insn[i].op_str)>0)
					printf("%02x", ' ');

				for(int x=0; insn[i].op_str[x]!='\0'; x++)
					printf("%02x", insn[i].op_str[x]);

				printf("%02x", '\n');
			}
			cs_free(insn, count);
		}
		printf("\n");
	}
	cs_close(&cshandle);
	return 0;
}
