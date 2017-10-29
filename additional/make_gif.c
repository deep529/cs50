//http://www.fileformat.info/format/gif/egff.htm
//http://www.matthewflickinger.com/lab/whatsinagif/bits_and_bytes.asp
//http://www.onicos.com/staff/iz/formats/gif.html
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "data_types.h"

void convertDecimalToBinary(int , FILE *, int);
struct gifHeader gifHead;
struct graphicsControlExtensionBlock gceb;
int main(int argc, char* argv[])
{
	if(argc!=2)
	{
		fprintf(stderr, "incorrect input\n");
		return 1;
	}

	FILE *fp = fopen(argv[1], "r");
	if(fp==NULL)
	{	
		fprintf(stderr, "unable to open GIF file\n");
		return 2;
	}
	
	//copied header data from file to struct gifHead
	fread(&gifHead, sizeof(gifHead), 1, fp);
	//abort everything if file format is not supported
	if( gifHead.signature[0]!='G' || gifHead.signature[1]!='I' || gifHead.signature[2]!='F' || 
		gifHead.version[0]!='8' || gifHead.version[2]!='a' )
	{
		fprintf(stderr, "file format not supported\n");
		return 3;
	}

	printf("%c%c%c %c%c%c\n",gifHead.signature[0], gifHead.signature[1], gifHead.signature[2], gifHead.version[0], gifHead.version[1], gifHead.version[2]);
	printf("width=%d height=%d\n",gifHead.screenWidth,gifHead.screenHeight );
	convertDecimalToBinary(gifHead.packed, fp, 0);
	printf("bckgnd color index = %d\n",gifHead.backgroundColor);
	printf("Aspect Ratio = %d\nPixel Aspect Ratio = %d\n",gifHead.aspectRatio, (gifHead.aspectRatio+15)/64);

	fread(&gceb, sizeof(gceb), 1, fp);
	printf("\nintroducer=%02x label=%02x block size=%02x\n",gceb.introducer, gceb.label, gceb.blockSize);
	convertDecimalToBinary(gceb.packed, fp, 1);
	printf("delay=%04x trans color index=%02x terminator=%02x\n",gceb.delayTime, gceb.colorIndex, gceb.terminator);

	fclose(fp);
return 0;	
}


void convertDecimalToBinary(int n, FILE *fp, int k)
{
    int binaryNumber = 0;
    int remainder, i = 1, step = 1;

    while (n!=0)
    {
        remainder = n%2;
        n = n/2;
        binaryNumber = binaryNumber + remainder*i;
        i = i*10;
    }
    
    char s[50];
    sprintf(s, "%d000000000000", binaryNumber);

    if(k==0)
    {
	    printf("GFCT = %c\ncolor reso = %c%c%c\n", s[0], s[1], s[2], s[3]);
	    printf("sort flag = %c\n", s[4]);
	    int noOfcolors = pow(2, 4*(s[5]-'0') + 2*(s[6]-'0') + (s[7]-'0') + 1);
	    printf("no of colors in GCT = %d\n",noOfcolors);
	    struct RGBTRIPLE rgb;
		for(int i=0; i<noOfcolors; i++)
		{
			fread(&rgb, sizeof(rgb), 1, fp);
			printf("#%02x%02x%02x\n",rgb.red, rgb.green, rgb.blue);
		}
	}
	else
	{
		printf("transparency=%c user input=%c disposal method=%c%c%c\n",s[7], s[6], s[3], s[4], s[5]);
	}	
}