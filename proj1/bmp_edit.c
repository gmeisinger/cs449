//George Meisinger
//gem56
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#pragma pack(1)

typedef struct BMPHeader {
	char 	fmt_id[2];
	int		file_size;
	short	reserved_1;
	short	reserved_2;
	int		offset;
} BMPHeader;

typedef struct DIBHeader {
	int 	header_size;
	int 	img_width;
	int		img_height;
	short	num_color_planes;
	short	bits_per_pixel;
	int 	comp_scheme;
	int		img_size;
	int		horz_res;
	int		vert_res;
	int		colors_in_palette;
	int		important_colors;
} DIBHeader;

typedef struct Pixel {
	unsigned char	blue;
	unsigned char	green;
	unsigned char	red;
} Pixel;

/*BMPHeader readBMPHeader(FILE* f)
{
	BMPHeader bmp_header;
	fread(&bmp_header, sizeof(bmp_header), 1, f);
	return bmp_header;
}

DIBHeader readDIBHeader(FILE* f)
{
	DIBHeader dib_header;
	fread(&dib_header, sizeof(dib_header), 1, f);
	return dib_header;
}*/

void to_grayscale(Pixel* pix)
{
	//cast to float
	double b = (double)pix->blue;
	double g = (double)pix->green;
	double r = (double)pix->red;
	//normalize
	b = b/255.0;
	g = g/255.0;
	r = r/255.0;
	//get brightness Y
	double Y = (0.2126 * r) + (0.7152 * g) + (0.0722 * b);
	//get gray value
	double gray_val = 12.92 * Y;
	if(Y > 0.0031308)
	{
		double exp = 1.0/2.4;
		double power = pow(Y,exp);
		gray_val = (power * 1.055) - 0.055;
	}
	//printf("grayval is %f\n", gray_val);
	//get back to 0-255
	gray_val = gray_val * 255.0;
	//set pixel values
	pix->blue = (unsigned char)gray_val;
	pix->green = (unsigned char)gray_val;
	pix->red = (unsigned char)gray_val;
}

void invert_img(Pixel* pix)
{
	pix->blue = ~pix->blue;
	pix->green = ~pix->green;
	pix->red = ~pix->red;
}

int main(int argc, char** argv)
{
	//check arg
	char* invert = "--invert";
	char* grayscale = "--grayscale";
	char* transform = argv[1];
	if(strcmp(invert, transform) != 0 && strcmp(grayscale, transform) != 0)
	{
		printf("%s : invalid argument.", transform);
		return 1;
	}

	FILE* f = fopen(argv[2], "rb+");
	//check if file exists
	if(f == NULL)
	{
		printf("File does not exist.\n");
		printf("argv[1] = '%s'\n", argv[1]);
		return 1;
	}
	//read in bmp_header
	BMPHeader bmp_header;
	fread(&bmp_header, sizeof(bmp_header), 1, f);
	char valid_fmt[2] = {'B', 'M'};
	//Check fmt_id == BM
	if(strncmp(bmp_header.fmt_id, valid_fmt, 2) != 0)
	{
		printf("fmt_id bad\n");
		fclose(f);
		return 1;
	}
	//read in dib_header
	DIBHeader dib_header;
	fread(&dib_header, sizeof(dib_header), 1, f);
	//check header_size == 40
	if(dib_header.header_size != 40)
	{
		printf("dib_header value not 40. Its %d\n", dib_header.header_size);
		fclose(f);
		return 1;
	}
	int padding = 0;
	int byte_width = (3 * dib_header.img_width);
	if((byte_width % 4) != 0)
		padding = 4 - (byte_width % 4);

	if(dib_header.bits_per_pixel != 24)
	{
		printf("bits_per_pixel value not 24.\n");
		fclose(f);
		return 1;
	}
	//Now start reading pixel data
	
	fseek(f, bmp_header.offset, SEEK_SET);
	for(int i=0;i<dib_header.img_height;i++)
	{
		for(int i=0;i<dib_header.img_width;i++)
		{
			Pixel pix;
			fread(&pix, sizeof(pix), 1, f);
			//transform based on argv
			if(strcmp(transform, invert) == 0)
			{
				invert_img(&pix);
			}
			else if(strcmp(transform, grayscale) == 0)
			{
				to_grayscale(&pix);
			}
			//fseek back
			fseek(f, -sizeof(pix), SEEK_CUR);
			//write pixel data
			fwrite(&pix, sizeof(pix), 1, f);
		}
		//padding
		fseek(f, padding * 1L, SEEK_CUR);
	}



	printf("transform %s complete!\n", transform);
	//close file
	fclose(f);
	return 0;
}