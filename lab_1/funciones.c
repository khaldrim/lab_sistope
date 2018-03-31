#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "struct.h"
#include "funciones.h"


/*
 * Input      : Puntero hacia el archivo bmp que se leera.
 * Output     : Puntero hacia la estructura BMPFILEHEADER que almacena la informacion de cabezera.
 * Description: Inicializa las variables 
 */
BMPFILEHEADER *ReadBMPFileHeader(FILE *fp, BMPFILEHEADER  *header)
{
    char           filetype[3] = {'\0', '\0', '\0'};
    unsigned int   filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long  offset;

    /* File type (2 bytes) */
    fread(&filetype, 1, 2, fp);

    /* File size (4 bytes) */
    filesize = (unsigned int) ReadLE4(fp);

    /* Reserved 1 (2 bytes) */
    fread(&reserved1, 2, 1, fp);

    /* Reserved 2 (2 bytes) */
    fread(&reserved2, 2, 1, fp);

    /* Offset (4 bytes) */
    offset = (unsigned long) ReadLE4(fp);

    strcpy(header->fileType, filetype);
    header->filesize   = filesize;
    header->reserved1 = reserved1;
    header->reserved2 = reserved2;
    header->offBits    = offset;

    return header;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
int SizeOfInformationHeader(FILE *fp)
{
    int headersize;
    unsigned char buf[4];
    int i;

    fread(buf, 1, 4, fp);
    for (i = 3; i >= 0; i--) {
        headersize = (headersize << 8) | (int) buf[i];
    }

    fseek(fp, 14, SEEK_SET);

    return headersize;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
BMPINFOOSHEADER *ReadBMPOSInfoHeader(FILE *fp, BMPINFOOSHEADER *header)
{
    unsigned int   headersize;
    int            width;
    int            height;
    unsigned short planes;
    unsigned short bitcount;

    /* Header size (4 bytes) */
    headersize = (unsigned int) ReadLE4(fp);

    /* Width (2 bytes) */
    width = (int) ReadLE2(fp);

    /* Height (2 bytes) */
    height = (int) ReadLE2(fp);

    /* Planes (2 bytes) */
    planes = (unsigned short) ReadLE2(fp);

    /* Bit Count (2 bytes) */
    bitcount = (unsigned short) ReadLE2(fp);

    header->osSize         = headersize;
    header->osWidth        = width;
    header->osHeight       = height;
    header->osColorPlanes  = planes;
    header->osBitsPerPixel = bitcount;

    return header;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
BMPINFOWINHEADER *ReadBMPWinInfoHeader(FILE *fp, BMPINFOWINHEADER *header)
{
    unsigned int     headersize;
    int              width;
    int              height;
    unsigned short   planes;
    unsigned short   bitcount;
    unsigned int     compression;
    unsigned int     size_image;
    int              x_pix_per_meter;
    int              y_pix_per_meter;
    unsigned int     clr_palette;
    unsigned int     clr_used;

    /* Header size (4 bytes) */
    headersize = (unsigned int) ReadLE4(fp);

    /* Width (4 bytes) */
    width = (int) ReadLE4(fp);

    /* Height (4 bytes) */
    height = (int) ReadLE4(fp);

    /* Planes (2 bytes) */
    planes = (unsigned short) ReadLE2(fp);

    /* Bit Count (2 bytes) */
    bitcount = (unsigned short) ReadLE2(fp);

    /* Compression (4 bytes) */
    compression = (unsigned int) ReadLE4(fp);

    /* Size image (4 bytes) */
    size_image = (unsigned int) ReadLE4(fp);

    /* X pix per meter (4 bytes) */
    x_pix_per_meter = (int) ReadLE4(fp);

    /* Y pix per meter (4 bytes) */
    y_pix_per_meter = (int) ReadLE4(fp);

    /* Color used (4 bytes) */
    clr_palette = (unsigned int) ReadLE4(fp);

    /* Color important (4 bytes) */
    clr_used = (unsigned int) ReadLE4(fp);

    header->winSize         = headersize;
    header->winWidth        = width;
    header->winHeight       = height;
    header->winColorPlanes  = planes;
    header->winBitsPerPixel = bitcount;
    header->winCompression  = compression;
    header->winImgSize      = size_image;
    header->winXPixPerMeter = x_pix_per_meter;
    header->winYPixPerMeter = y_pix_per_meter;
    header->winColorPalette = clr_palette;
    header->winColorUsed    = clr_used;

    return header;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
unsigned short ReadLE2(FILE *fp)
{
    unsigned char buf[2];
    unsigned short result = 0;
    int i;

    fread(buf, 1, 2, fp);
    for (i = 1; i >= 0; i--) {
        result = (result << 8) | (unsigned short) buf[i];
    }

    return result;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
unsigned int ReadLE4(FILE *fp)
{
    unsigned char buf[4];
    unsigned int result = 0;
    int i;

    fread(buf, 1, 4, fp);
    for (i = 3; i >= 0; i--) {
        result = (result << 8) | (unsigned int) buf[i];
    }

    return result;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
char* readPixelData(FILE *fp, unsigned char *data,int width, int height, int offset)
{
    int size = width*height;
    fseek(fp, offset, SEEK_SET); 
    fread(data, sizeof(unsigned char), (3*size), fp);

    int i;
    for(i = 0; i < (size); i+= 3)
    {
        /* Intercambio BGR a RGB, debido a que asi lo almacena la imagen windows.*/

        unsigned char tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;

        //printf("N_%i R: %i G: %i B: %i.\n", i, data[i], data[i+1], data[i+2]);
    }

    return data;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
char* scaleGreyData(int uflag, unsigned char *data, unsigned char *binary_data, int width, int height)
{
    int size = (width*height);
    int i, j = 0;

    for(i=0;i<size;i+=3)
    {
        float scale = (((int)data[i])*0.3) + (((int)data[i+1])*0.59) + (((int)data[i+2])*0.11);
        if(scale > uflag)
        {
            binary_data[j] = 0;
        }
        else
        {
            binary_data[j] = 1;
        }

        //printf("N_%i Scale: %f \n R: |%i|%f| B: |%i|%f| G: |%i|%f|\n", i,scale, data[i],(data[i]*0.3), data[i+1],(data[i+1]*0.59), data[i+2],(data[i+2]*0.11));
        j++;
    }

    return binary_data;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */


void writeBinaryImageWin(int img_count, unsigned char *binary_data, BMPFILEHEADER *bmpfh, BMPINFOWINHEADER *bmpWinIH)
{   
    /* Primero escribir el archivo de cabecera, luego la informacion de cabezera y despues los pixeles binarizados. 
     * Cabe recordar que cada imagen tiene propiedades diferentes. Por lo que puede ir variando el headersize por ej.
     */

    FILE *fp = NULL;
    char fileNumber[5];
    char fileName[30] = "imagenes/resultado_imagen_";

    sprintf(fileNumber, "%d", img_count);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    if((fp=fopen(fileName, "wb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        abort();   
    }

    /* File Header */

    /* fileType */
    fwrite();

    /* filesize */
    fwrite();

    /* reserved1  */
    fwrite();

    /* reserved2 */
    fwrite();

    /* offBits */
    fwrite();
    
    /* Info Header of Os or Win */
    if(bmpfh->headersize == 12)
    {

    }
    else if(bmpfh->headersize == 40)
    {

    }
    else
    {

    }

    /* Write Pixel Data from binary_data */
    fwrite();
}


/*
 * Input      : 
 * Output     : 
 * Description: 
 */
FILE* readImageHeader(int img_num, FILE *fp,BMPFILEHEADER *bmpfh, BMPINFOOSHEADER *bmpOSIH, BMPINFOWINHEADER *bmpWinIH)
{
    char fileNumber[5];
    char fileName[30] = "imagenes/imagen_";

    sprintf(fileNumber, "%d", img_num);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    if((fp = fopen(fileName,"rb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        abort();
    }

    bmpfh = ReadBMPFileHeader(fp, bmpfh);
    if(strcmp(bmpfh->fileType, "BM") != 0)
    {
        printf("El archivo no es un Bitmap.\n");
        abort();
    }

    bmpfh->headersize = SizeOfInformationHeader(fp);
    if(bmpfh->headersize == 12)
    {
        bmpOSIH = ReadBMPOSInfoHeader(fp, bmpOSIH);
    }
    else if(bmpfh->headersize == 40)
    {
        bmpWinIH = ReadBMPWinInfoHeader(fp, bmpWinIH);
    }
    else
    {
        printf("Bitmap no soportado.\n");
        abort();
    }


    printf("\n\nFile type          = %s\n", bmpfh->fileType);
    printf("File size          = %d bytes\n", bmpfh->filesize);
    printf("Data offset        = %ld bytes\n", bmpfh->offBits);
    if (bmpfh->headersize == 12) 
    {
        printf("Info header size   = %d bytes\n", bmpOSIH->osSize);
        printf("Width              = %d pixels\n", bmpOSIH->osWidth);
        printf("Height             = %d pixels\n", bmpOSIH->osHeight);
        printf("Planes             = %d\n", bmpOSIH->osColorPlanes);
        printf("Bits per Pixel     = %d bits/pixel\n", bmpOSIH->osBitsPerPixel);
    } 
    
    if (bmpfh->headersize == 40) 
    {
        printf("Info header size   = %d bytes\n", bmpWinIH->winSize);
        printf("Width              = %ld pixels\n", bmpWinIH->winWidth);
        printf("Height             = %ld pixels\n", bmpWinIH->winHeight);
        printf("Color Planes       = %d\n", bmpWinIH->winColorPlanes);
        printf("Bits per Pixel     = %d bits/pixel\n", bmpWinIH->winBitsPerPixel);
        printf("Compression        = %d\n", bmpWinIH->winCompression);
        printf("Size image         = %d bytes\n", bmpWinIH->winImgSize);
        printf("X pixels per meter = %ld\n", bmpWinIH->winXPixPerMeter);
        printf("Y pixels per meter = %ld\n", bmpWinIH->winYPixPerMeter);
        printf("Color Palette      = %ld colors\n", bmpWinIH->winColorPalette);
        printf("Color Used         = %ld colors\n", bmpWinIH->winColorUsed);
    }

    return fp;
}

/*
    Entrada    : Ingresan los valores de las banderas solicitadas como parametros al usuario.
    Salida     : Retorna un entero, con dos valores posibles 1 y 0. Retorna 1 si la funcion cumplio su flujo normal y retorna 0 si existe algun error.
    Descripcion: Funcion que bla bla... 
*/
void mainMenu(int cflag, int uflag, int nflag, int bflag)
{
    int cvalue = 0, i = 1;
    cvalue = cflag;
    
    /* Etapas del Pipeline:
            1. Leer el la informacion de cabezera de un archivo bitmap.
            2. Leer pixel por pixel la imagen, aplicando formula del enunciado.
            3. Determinar si el pixel debe ser transformado a blanco o a negro, dependiendo del umbral.
            4. Clasificar la imagen como 'nearly black' si esta supera el umbral.
            5. Crear la imagen binarizada.
            6. Repetir Pipeline si aun quedan imagenes por leer.      
            7. Imprimir por pantalla la informacion solicitada si la bandera -b esta activa.
    */
    
    while(cvalue > 0)
    {
        FILE *fp = NULL;
        unsigned char *data = NULL;
        unsigned char *binary_data = NULL;
        BMPFILEHEADER    *bmpFileHeader    = NULL;
        BMPINFOOSHEADER  *bmpOsInfoHeader  = NULL;
        BMPINFOWINHEADER *bmpWinInfoHeader = NULL;
        
        bmpFileHeader    = (BMPFILEHEADER *) malloc(sizeof(BMPFILEHEADER));
        bmpOsInfoHeader  = (BMPINFOOSHEADER *) malloc(sizeof(BMPINFOOSHEADER));
        bmpWinInfoHeader = (BMPINFOWINHEADER*) malloc(sizeof(BMPINFOWINHEADER));
        
        fp = readImageHeader(i, fp,bmpFileHeader, bmpOsInfoHeader, bmpWinInfoHeader);
        
        if(bmpFileHeader->headersize == 40)
        {   
            /* La variable data contiene los valores RGB en un arreglo, binary_data contiene 0 o 1 dependiendo del umbral. */
    
            unsigned char *data = (char*)calloc((3*bmpWinInfoHeader->winImgSize),sizeof(char));
            unsigned char *binary_data = (char*)calloc(bmpWinInfoHeader->winImgSize, sizeof(char));
            
            data = readPixelData(fp,data,bmpWinInfoHeader->winWidth, bmpWinInfoHeader->winHeight, bmpFileHeader->offBits);
            binary_data = scaleGreyData(uflag, data, binary_data, bmpWinInfoHeader->winWidth, bmpWinInfoHeader->winHeight);
            
            

                /* Codigo para mostrar contenido de binary_data 
            int i, j = 0;
            for(i=0; i<bmpWinInfoHeader->winImgSize; i++)
            {
                if(j == 200)
                {
                    j = 0;
                    printf("\n");
                }
                else
                {
                    printf("%i", binary_data[i]);
                    j++;
                }
            }
            printf("\n");
            */
        }
        else if()
        {
            
        }
        else
        {
            
        }

        cvalue--;
        i++;

        fclose(fp);
        free(data);
        free(binary_data);
        free(bmpFileHeader);
        free(bmpOsInfoHeader);
        free(bmpWinInfoHeader);
    }
}