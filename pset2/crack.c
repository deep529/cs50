#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<crypt.h>
#include<time.h>
clock_t start,end;

int main(int argc, char *argv[])
{
	start = clock();
	if(argc!=2)
	{
		printf("error\n");
		return 1;
	}

	char s[6] = "a";
	char salt[3] = "\0";
	strncpy(salt, argv[1], 2);
	int i=0,operations=52,index=0,temp;

	// printf("%s\n",crypt(s,salt));
	for(i=0; i<operations; i++)
	{
	    if(strcmp(argv[1],crypt(s,salt)) == 0)
			break;

		// printf("%s\n",s);

		if(i==operations-1)
		{
			index++;
			for(temp=0; temp<=index; temp++)
				s[temp] = 'a'; 
		
			i=0;
			
			if(operations==52*52*52*52)
				operations = 0;
			else
				operations = 52*operations;
		}
		
		if(s[index]=='z')
		{
			s[index] = 'A';
			continue;
		}
		
		if(s[index]=='Z')
		{
			s[index] = 'a';
			if(s[index-1]=='z')
				s[index-1] = 'A';
			else if(s[index-1]=='Z')
			{
				s[index-1] = 'a';
				if( index > 1 )
				{
					if(s[index-2]=='z')
						s[index-2] = 'A';
					else if(s[index-2]=='Z')
					{
						s[index-2] = 'a';
						if(index>2)
						{
							if(s[index-3]=='z')
								s[index-3] = 'A';
							else
								s[index-3]++;
						}
					}
					else
						s[index-2]++;
				}
			}
			else 
				s[index-1]++;
			continue;
		}

		s[index]++;
	}


	printf("%s\n",s);


	end = clock();
	printf("%lfms\n",( 1000.0*(double)end-(double)start ) / CLOCKS_PER_SEC ); 
return 0;	
}
