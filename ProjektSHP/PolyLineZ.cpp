#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include "PolyLineZ.h"

using namespace std;

long PolyLineZ::sizeOfFiles(FILE* file)
{
    long l, e;
    l = ftell(file);
    fseek(file, 0, 2);
    e = ftell(file);
    fseek(file, l, 0);
    return e;
}


//Funkcja konwertujaca typ BigEndian32, w ktorym przechowywane sa niektore dane w pliku na typ int 
int32_t PolyLineZ::bigEndianIntRead(char* fileBuf, int startIndex)
{
    return (((fileBuf[startIndex + 0] & 0xff) << 24) | ((fileBuf[startIndex + 1] & 0xff) << 16) | ((fileBuf[startIndex + 2] & 0xff) << 8) | ((fileBuf[startIndex + 0] & 0xff)));
}

//Funkcja konwertujaca typ LittleEndian32, w ktorym przechowywane sa niektore dane w pliku na typ int
int32_t PolyLineZ::littleEndianIntRead(char* fileBuf, int startIndex)
{
    return (((fileBuf[startIndex + 3] & 0xff) << 24) | ((fileBuf[startIndex + 2] & 0xff) << 16) | ((fileBuf[startIndex + 1] & 0xff) << 8) | ((fileBuf[startIndex + 0] & 0xff)));
}

//Funkcja konwertujaca 64 bitowy littleEndian na int
double PolyLineZ::littleEndianDoubleRead(char* fileBuf, int startIndex)
{
    double convert;
    char* add;
    int j;
    add = new char();
    j = -1;
    for (int i = startIndex; i < startIndex + 8; i++)
    {
        j++;
        add[j] = fileBuf[i];
    }
    convert = *reinterpret_cast<double* const>(add);
    return convert;
}

int32_t PolyLineZ::fileCode(char* fileBuf, int startIndex)
{
    return bigEndianIntRead(fileBuf, startIndex);
}


int32_t PolyLineZ::fileLength(char* fileBuf, int startIndex)
{
    return bigEndianIntRead(fileBuf, startIndex);
}

int32_t PolyLineZ::version(char* fileBuf, int startIndex)
{
    return littleEndianIntRead(fileBuf, startIndex);
}


int32_t PolyLineZ::shapeType(char* fileBuf, int startIndex)
{
    return littleEndianIntRead(fileBuf, startIndex);
}



