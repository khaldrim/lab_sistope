#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "function.h"

int mainMenu(int cflag, int uflag, int nflag, int bflag)
{
	int cont, imgCount;

	imgCount = 1;
	cont = cflag;

	while(cont > 0)
	{
		FILE *fp                           = NULL;
		unsigned char **data               = NULL;
		int           *binData             = NULL;
		BMPFILEHEADER    *bmpFileHeader    = NULL;
		BMPINFOWINHEADER *bmpInfoHeader    = NULL;
		RGB *pixel                         = NULL;

		bmpFileHeader    = (BMPFILEHEADER *)   malloc(sizeof(BMPFILEHEADER));
		bmpInfoHeader    = (BMPINFOHEADER*) malloc(sizeof(BMPINFOWINHEADER));

		fp = readImageHeader(imgCount, fp, bmpFileHeader,bmpInfoHeader);


	}

}


unsigned char** readImageData(FILE *fp, BMPINFOHEADER *bmpInfoHeader, BMPFILEHEADER *bmpFileHeader)
{
    int i, j, pad;
	long scale; 
    unsigned char **data;
    RGB *pixel;
    
    data = createBuffer(bmpInfoHeader->width, bmpInfoHeader->height);

    if(data != NULL)
    {
        pixel = (RGB*)malloc(sizeof(RGB));
        for(j=0;j<bmpIH->winHeight;j++)
        {
            pad = 0;
            for(i=0;i<bmpIH->winWidth;i++)
            {
                if(fread(pixel, 1, sizeof(RGB),fp) != sizeof(RGB))
                {
                   printf("Error leyendo los pixeles.\n");
                   abort();
                }

                data[j][i]   = pixel->blue;
                data[j][i+1] = pixel->green;
                data[j][i+2] = pixel->red;

                pad += sizeof(RGB);
            }

            if(pad % 4 != 0)
            {
                int z;
                pad = 4 - (pad%4);
                fread(pixel, pad, 1, fp);
                for(z=0; z<pad; z++)
                {
                    data[j][i+z] = 0;
                }
            } 
        }
        return data;
    }
    else
    {
        printf("No se pudo asignar memoria para leer los datos de la imagen.\n");
        return NULL;
    }
}

unsigned char** createBuffer(int width, int height)
{
    unsigned char** data = NULL;
    int padding = 0;
    int totalWidthSize = 0;
    int i;

    padding = (4 - (width * 3) % 4) % 4;
    totalWidthSize = padding + (width * 3);

    data = (unsigned char**)malloc(sizeof(unsigned char*) * totalWidthSize);

    for(i=0;i<width;i++)
    {
        data[i] = (unsigned char*)malloc(sizeof(unsigned char) * height);
    }

    if(data == NULL)
        return NULL;
    else
        return data;
}