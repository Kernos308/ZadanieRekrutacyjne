#include <iostream>
#include <stdio.h>




class PolyLineZ {
public:

    int32_t filecodes, fileLengths, shapeTypes, versions;
    double xmin, ymin, xmax, ymax, zmin, zmax; //wartosci w ktorych beda przechowywane wspolrzedne lini do rysowania 
    long fileSize;
    

    static long sizeOfFiles(FILE* file);
    static int32_t bigEndianIntRead(char* fileBuf, int startIndex);
    static int32_t littleEndianIntRead(char* fileBuf, int startIndex);
    static double littleEndianDoubleRead(char* fileBuf, int startIndex);
    static int32_t fileCode(char* fileBuf, int startIndex);
    static int32_t fileLength(char* fileBuf, int startIndex);
    static int32_t version(char* fileBuf, int startIndex);
    static int32_t shapeType(char* fileBuf, int startIndex);


};
