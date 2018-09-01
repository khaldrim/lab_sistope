#include "Bmp.hpp"

/* Constructor and Destructor */
Bmp::Bmp(){
    cout << "Object Bmp created." << endl;
}

Bmp::~Bmp(){ cout << "Object Bmp deleted." << endl; }

/* Setters and Getters for File Header */
void Bmp::setType(FILE *fp){ fread(Bmp::type, 1, 2, fp); }
char* Bmp::getType(){ return Bmp::type; }

void Bmp::setFileSize(FILE *fp){ Bmp::fileSize = (unsigned int) ReadLE4(fp); }
DWORD Bmp::getFileSize(){ return Bmp::fileSize; }

void Bmp::setReserved1(FILE *fp){ unsigned short r1; fread(&r1, 2, 1, fp); Bmp::reserved1 = r1; }
void Bmp::setReserved2(FILE *fp){ unsigned short r2; fread(&r2, 2, 1, fp); Bmp::reserved2 = r2; }

void Bmp::setOffbits(FILE *fp){ unsigned long ob; ob = ReadLE4(fp); Bmp::offbits = ob; }
DWORD Bmp::getOffbits(){ return Bmp::offbits; }

/* Setters and Getters for Info Header */
void Bmp::setSize(FILE *fp){ Bmp::size = (unsigned int) ReadLE4(fp); }
DWORD Bmp::getSize(){ return Bmp::size; }

void Bmp::setWidth(FILE *fp){ Bmp::width = (unsigned int) ReadLE4(fp); }
DWORD Bmp::getWidth(){ return Bmp::width; }

void Bmp::setHeight(FILE *fp){ Bmp::height = (unsigned int) ReadLE4(fp); }
DWORD Bmp::getHeight(){ return Bmp::height; }

void Bmp::setPlanes(FILE *fp){ Bmp::planes = (unsigned short) ReadLE2(fp); }

void Bmp::setBitPerPixel(FILE *fp){ Bmp::bitPerPixel = (unsigned short) ReadLE2(fp); }

void Bmp::setCompression(FILE *fp){ Bmp::compression = (unsigned int) ReadLE4(fp); }

void Bmp::setSizeImage(FILE *fp){ Bmp::sizeImage = (unsigned int) ReadLE4(fp); }

void Bmp::setxPelsPerMeter(FILE *fp){ Bmp::xPelsPerMeter = (unsigned long) ReadLE8(fp); }

void Bmp::setyPelsPerMeter(FILE *fp){ Bmp::yPelsPerMeter = (unsigned long) ReadLE8(fp); }

void Bmp::setUsed(FILE *fp){ Bmp::used = (unsigned int) ReadLE4(fp); }

void Bmp::setImportant(FILE *fp){ Bmp::important = (unsigned int) ReadLE4(fp); }

void Bmp::setRedMask(FILE *fp){ Bmp::redMask = (unsigned int) ReadLE4(fp); }

void Bmp::setGreenMask(FILE *fp){ Bmp::greenMask = (unsigned int) ReadLE4(fp); }

void Bmp::setBlueMask(FILE *fp){ Bmp::blueMask = (unsigned int) ReadLE4(fp); }

void Bmp::setAlphaMask(FILE *fp){ Bmp::alphaMask = (unsigned int) ReadLE4(fp); }

void Bmp::setCsType(FILE *fp){ Bmp::csType = (unsigned int) ReadLE4(fp); }

void Bmp::setXRed(FILE *fp){ Bmp::ciexyzXRed = (unsigned int) ReadLE4(fp); }

void Bmp::setYRed(FILE *fp){ Bmp::ciexyzYRed = (unsigned int) ReadLE4(fp); }

void Bmp::setZRed(FILE *fp){ Bmp::ciexyzZRed = (unsigned int) ReadLE4(fp); }

void Bmp::setXGreen(FILE *fp){ Bmp::ciexyzXGreen = (unsigned int) ReadLE4(fp); }

void Bmp::setYGreen(FILE *fp){ Bmp::ciexyzYGreen = (unsigned int) ReadLE4(fp); }

void Bmp::setZGreen(FILE *fp){ Bmp::ciexyzZGreen = (unsigned int) ReadLE4(fp); }

void Bmp::setXBlue(FILE *fp){ Bmp::ciexyzXBlue = (unsigned int) ReadLE4(fp); }

void Bmp::setYBlue(FILE *fp){ Bmp::ciexyzYBlue = (unsigned int) ReadLE4(fp); }

void Bmp::setZBlue(FILE *fp){ Bmp::ciexyzZBlue = (unsigned int) ReadLE4(fp); }

void Bmp::setGammaRed(FILE *fp){ Bmp::gammaRed = (unsigned int) ReadLE4(fp); }

void Bmp::setGammaGreen(FILE *fp){ Bmp::gammaGreen = (unsigned int) ReadLE4(fp); }

void Bmp::setGammaBlue(FILE *fp){ Bmp::gammaBlue = (unsigned int) ReadLE4(fp); }

void Bmp::setIntent(FILE *fp){ Bmp::intent = (unsigned int) ReadLE4(fp); }

void Bmp::setProfileData(FILE *fp){ Bmp::profileData = (unsigned int) ReadLE4(fp); }

void Bmp::setProfileSize(FILE *fp){ Bmp::profileSize = (unsigned int) ReadLE4(fp); }

void Bmp::setReserved(FILE *fp){ Bmp::reserved = (unsigned int) ReadLE4(fp); }


/*
 * Descripcion: Funcion que permite mover bits desde 'Big-Endian' a 'Litle-Endian',
 *              de tamaño 2 bytes.
 * 
 * Entrada:     Puntero al archivo 'fp'
 * Salida:      Resultado en 'unsigned short'
 */
unsigned short Bmp::ReadLE2(FILE *fp)
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
 * Descripcion: Funcion que permite mover bits desde 'Big-Endian' a 'Litle-Endian',
 *              de tamaño 4 bytes.
 * 
 * Entrada:     Puntero al archivo 'fp'
 * Salida:      Resultado en 'unsigned int'
 */
unsigned int Bmp::ReadLE4(FILE *fp)
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
 * Descripcion: Funcion que permite mover bits desde 'Big-Endian' a 'Litle-Endian',
 *              de tamaño 8 bytes.
 * 
 * Entrada:     Puntero al archivo 'fp'
 * Salida:      Resultado en 'unsigned int'
 */
unsigned int Bmp::ReadLE8(FILE *fp)
{
    unsigned char buf[8];
    unsigned int result = 0;
    int i;

    fread(buf, 1, 8, fp);
    for (i = 7; i >= 0; i--) {
        result = (result << 8) | (unsigned int) buf[i];
    }

    return result;
}

unsigned char** Bmp::createPixelMatrix(int width, int height){
    unsigned char** data = NULL;
    int colSize, i;

    colSize = width * 4;

    data = (unsigned char**)malloc(sizeof(unsigned char*) * height);

    if(data != NULL)
    {
        for(i=0; i < height; i++)
        {
            data[i] = (unsigned char*)malloc(sizeof(unsigned char) * colSize);
            if(data[i] == NULL)
            {
                printf("No existe espacio para asignar memoria a las filas de la matriz.\n");
                exit(1);
            }
        }

        return data;
    }
    else
    {
        printf("No hay espacio para los datos de la imagen.\n");
        exit(1);
    }
}

unsigned int* Bmp::createGreyMatrix(int width, int height){
    int totalSize = width * height * 4;
    unsigned int* data = NULL;

    data = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);
    return data;
}

unsigned int* Bmp::createBinMatrix(int width, int height){
    int totalSize = width * height * 4;
    unsigned int* data = NULL;

    data = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);
    return data;
}