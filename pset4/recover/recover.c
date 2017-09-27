#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SIZE 512
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "run with imagename as command line argument\n");
		return 1;
	}

	FILE *raw_file = fopen(argv[1], "r");
	if(raw_file == NULL)
	{
		fprintf(stderr, "unable to open file\n");
		return 2;
	}

	char imagename[6];
	int jpeg_count = 0, jpeg_found = 0;
	BYTE buffer[SIZE];
	FILE *image = NULL;
	while( fread(buffer, sizeof(buffer), 1, raw_file) > 0 )
	{
		if(buffer[0]==0xff && buffer[1]==0xd8 && buffer[2]==0xff && (buffer[3] & 0xf0) == 0xe0)
		{
			if(jpeg_found == 1)
			{
				fclose(image);
			}

			printf("*");

			jpeg_found = 1;
			jpeg_count++;
			sprintf(imagename, "%03d.jpg", jpeg_count);
			image = fopen(imagename, "w");
			if(image == NULL)
			{
				fprintf(stderr, "unable to create image\n");
				return 3;			
			}
			fwrite(buffer, sizeof(buffer), 1, image);	
		}
		else if(jpeg_found == 1)
			fwrite(buffer, sizeof(buffer), 1, image);	
	}
	printf("%d\n", jpeg_count);
	if(image!=NULL)
		fclose(image);

	fclose(raw_file);

return 0;	
}