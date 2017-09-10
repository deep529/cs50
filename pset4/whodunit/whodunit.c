#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
	if(argc!=3)
	{
		fprintf(stderr, "incorrect input\n");
		return 1;
	}

	char *infile = argv[1]; 
	char *outfile = argv[2];

	FILE *inptr = fopen(infile, "r");
	if(inptr == NULL)
	{
		fprintf(stderr, "could not open input file\n");
		return 2;
	}

	FILE *outptr = fopen(outfile, "w");
	if(outptr == NULL)
	{
		fclose(inptr);
		fprintf(stderr, "could not create output file\n");
		return 3;
	}

	BITMAPFILEHEADER bf;    //BITMAPOFILEHEADER is a structure defined in bmp.h
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);	//reads header bytes of input file & store in structure bf  

	BITMAPINFOHEADER bi;	//BITMAPINFOHEADER is a structure defined in bmp.h
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);	//reads header bytes of input files & store in structure bi
	
	//make sure bmp file we are processing is uncompressed BMP 4.0
	// ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

	fwrite(&bf, sizeof(bf), 1, outptr);
	fwrite(&bi, sizeof(bi), 1, outptr);

	int padding = ( 4 - (bi.biWidth * sizeof(RGBTRIPLE))%4 )%4;

	for(int i=0,bh = abs(bi.biHeight); i<bh; i++)
	{
		for(int j=0,wd = abs(bi.biWidth); j<wd; j++)
		{
			RGBTRIPLE rgb;

			fread(&rgb, sizeof(RGBTRIPLE), 1, inptr);
			if(rgb.rgbtBlue==0x00 && rgb.rgbtGreen==0x00)
			{
				rgb.rgbtGreen = 0xff;
				rgb.rgbtBlue = 0xff;	
				rgb.rgbtRed = 0xff;
			}
			fwrite(&rgb, sizeof(RGBTRIPLE), 1, outptr);
		}

		// skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        for(int k=0; k<padding; k++)
        {
        	fputc(0x00, outptr);
        }
	}	

	fclose(inptr);
	fclose(outptr);	
	
return 0;
}
