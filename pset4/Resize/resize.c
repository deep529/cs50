#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]);
	if(argc !=  4 || n>100)
	{
		fprintf(stderr, "give proper input (ie. n <input file> <output file>  n should be less than 101)\n");
		return 1;
	}

	char *infile = argv[2];
	char *outfile = argv[3];

	FILE *inptr = fopen(infile, "r");
	if(inptr == NULL)
	{
		fprintf(stderr, "Unable to open %s.\n",infile);
		return 2;
	}

	FILE *outptr = fopen(outfile, "w");
	if(outptr == NULL)
	{
		fprintf(stderr, "Could not create %s.\n",outfile);
		fclose(inptr);
		return 3;
	}

	BITMAPFILEHEADER bf;
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

	BITMAPINFOHEADER bi;
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

	if( bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize!=40 || bi.biBitCount != 24 || bi.biCompression != 0 )
	{
		fprintf(stderr, "Unsupported input file format.\n");
		fclose(inptr);
		fclose(outptr);
		return 4;		
	}

	int padding_infile = (4 - (bi.biWidth*sizeof(RGBTRIPLE)) % 4) % 4;
	int padding_outfile = (4 - (n*bi.biWidth*sizeof(RGBTRIPLE)) % 4) % 4;
	
	bi.biHeight = bi.biHeight*n;  //adjusting to new height
	bi.biWidth = bi.biWidth*n; //adjusting to new width
	
	bi.biSizeImage = abs(bi.biHeight) * ( abs(bi.biWidth)*sizeof(RGBTRIPLE) + padding_outfile ); //adjusting size of image
	bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); //adjusting size of file

	//writing into output file
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


	for(int i=0,bh = abs(bi.biHeight)/n; i<bh; i++)
	{
		for(int j=0; j<n; j++)
		{
			for(int k=0,bw = abs(bi.biWidth)/n; k<bw; k++)
			{
				RGBTRIPLE rgb;
				fread(&rgb, sizeof(RGBTRIPLE), 1, inptr);

				for(int p=0; p<n; p++)
					fwrite(&rgb, sizeof(RGBTRIPLE), 1, outptr);
			}
			for(int p=0; p<padding_outfile; p++)
				fputc(0x00, outptr);
		
			fseek(inptr, -((int)sizeof(RGBTRIPLE))*abs(bi.biWidth)/n, SEEK_CUR);
		}
		fseek(inptr, abs(bi.biWidth)*((int)sizeof(RGBTRIPLE))/n + padding_infile, SEEK_CUR);
	}

	fclose(inptr);
	fclose(outptr);

return 0;	
}
