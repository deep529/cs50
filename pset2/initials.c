#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int main()
{
	char c,prev;
	c = getchar();
	prev = ' ';
	
	while(c==' ')
		c = getchar();

	while(c!='\n')
	{
		if(prev==' ' && c!=' ')
			printf("%c",toupper(c));
		prev = c;
		c = getchar();
	}
	printf("\n");
return 0;	
}