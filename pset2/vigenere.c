#include<stdio.h>
#include<string.h>
#include<ctype.h>

int allalpha(char string[])
{
    int i = strlen(string)-1;

    while(i>=0 && isalpha(string[i])!=0)
    {
        string[i] = toupper(string[i]);
        i--;
    }    
    if(i==-1)
        return 1;
    else
        return 0;
}

int main(int argc, char  *argv[])
{
    if(argc!=2 || allalpha(argv[1])==0)
    {
        printf("error\n");
        return 1;
    }
    
    char s[1000],temp;
    printf("plaintext:");
    fgets(s, sizeof(s), stdin);
    int l=strlen(argv[1]), len=strlen(s);
    if(s[len-1]=='\n')
    {
        s[len-1] = '\0';
        len--;
    }
    
    printf("ciphertext:");
    int p=0,i=0;
    
    while(s[i]!='\0')
    {
        while(i<len && isalpha(s[i])==0)
        {
            printf("%c",s[i]);
            i++;
        }
       
        if(i<len)
        {
        	if(s[i]<='Z')
        	{	
            	temp = (s[i]+argv[1][p]-65)%91;
            	if(temp<65)
            		printf("%c", temp + 65);
            	else
            		printf("%c", temp);
            }
            else
            {	
            	temp = (s[i]+argv[1][p]-65)%123;
            	if(temp<97)
            		printf("%c", temp + 97);
            	else
            		printf("%c", temp);
            }
            p++;
            p = p%l;
            i++;
        }
        else
            break;
    }
    
    printf("\n");
    
return 0;    
}