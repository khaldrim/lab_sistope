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
    header->reserverd1 = reserved1;
    header->reserverd2 = reserved2;
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
void readPixelData(FILE *fp, int width, int height)
{
    unsigned char *data = (char*)calloc((3*width*height),sizeof(char));
    fread(data, sizeof(unsigned char), (3*width*height), fp);

    printf("Size: %i\n", (width*height));

    int i;
    for(i = 0; i < (width*height); i+= 3)
    {
        unsigned char tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;

        printf("R: %i G: %i B: %i.\n", data[i], data[i+1], data[i+2]);
    }
    
    /*
    for (i = 2; i >= 0; i--) {
        result = (result << 8) | (unsigned int) buf[i];
    }*/

    //printf("Pixel: %i %i %i .\n", buf[0], buf[1], buf[2]);
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

    if((fp = fopen(fileName,"r")) == NULL)
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
        BMPFILEHEADER    *bmpFileHeader    = NULL;
        BMPINFOOSHEADER  *bmpOsInfoHeader  = NULL;
        BMPINFOWINHEADER *bmpWinInfoHeader = NULL;

        bmpFileHeader    = (BMPFILEHEADER *) malloc(sizeof(BMPFILEHEADER));
        bmpOsInfoHeader  = (BMPINFOOSHEADER *) malloc(sizeof(BMPINFOOSHEADER));
        bmpWinInfoHeader = (BMPINFOWINHEADER*) malloc(sizeof(BMPINFOWINHEADER));
        
        fp = readImageHeader(i, fp,bmpFileHeader, bmpOsInfoHeader, bmpWinInfoHeader);
        
        if(bmpFileHeader->headersize == 40)
        {   
            
            readPixelData(fp,bmpWinInfoHeader->winWidth, bmpWinInfoHeader->winHeight);
            
        }

        cvalue--;
        i++;

        fclose(fp);
        free(bmpFileHeader);
        free(bmpOsInfoHeader);
        free(bmpWinInfoHeader);
    }
}