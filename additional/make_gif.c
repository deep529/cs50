#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "data_types.h"

struct gifHeader{
	//header
	BYTE signature[3];	//header signature (always "GIF")
	BYTE version[3];	//gif version ie. 87a or 89a
	//logical screen descriptor
	WORD screenWidth;	
	WORD screenHeight;	//display parameters in pixels
	BYTE packed;	//screen & color map info
	BYTE backgroundColor;	//backgnd color index
	BYTE aspectRatio;	//pixel aspect ratio
}	gifHead;

void convertDecimalToBinary(int n)
{
    int binaryNumber = 0;
    int remainder, i = 1, step = 1;

    while (n!=0)
    {
        remainder = n%2;
        n /= 2;
        binaryNumber += remainder*i;
        i *= 10;
    }
    printf("%d\n", binaryNumber);
    printf("GFCT = %d\ncolor reso = %d\n",binaryNumber/10000000, (binaryNumber/10000)%1000 );
    printf("sort flag = %d\n",(binaryNumber/1000)%10);
    printf("size of GCT = %d\n",(binaryNumber)%1000);
}


int main(int argc, char* argv[])
{
	if(argc!=2)
		return 0;

	FILE *fp = fopen(argv[1], "r");
	if(fp==NULL)
		return 0;
	
	fread(&gifHead, sizeof(gifHead), 1, fp);
	printf("%c %c %c\n%c %c %c\n",gifHead.signature[0], gifHead.signature[1], gifHead.signature[2], gifHead.version[0], gifHead.version[1], gifHead.version[2]);
	printf("width=%d height=%d\n",gifHead.screenWidth,gifHead.screenHeight );
	printf("%d\n",gifHead.packed);
	convertDecimalToBinary(gifHead.packed);



	fclose(fp);
return 0;	
}