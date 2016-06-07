//
//  CGTexture.cpp
//  TextureTest
//
//  Created by Benjamin Johnson on 4/06/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#include "CGTexture.hpp"

#include <iostream>

/* fread example: read an entire file */
#include <stdio.h>
#include <stdlib.h>

typedef int LONG;
typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;

typedef struct tagBITMAPFILEHEADER
{
    WORD    bfType; // 2  /* Magic identifier */
    DWORD   bfSize; // 4  /* File size in bytes */
    WORD    bfReserved1; // 2
    WORD    bfReserved2; // 2
    DWORD   bfOffBits; // 4 /* Offset to image data, bytes */
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD    biSize; // 4 /* Header size in bytes */
    LONG     biWidth; // 4 /* Width of image */
    LONG     biHeight; // 4 /* Height of image */
    WORD     biPlanes; // 2 /* Number of colour planes */
    WORD     biBitCount; // 2 /* Bits per pixel */
    DWORD    biCompress; // 4 /* Compression type */
    DWORD    biSizeImage; // 4 /* Image size in bytes */
    LONG     biXPelsPerMeter; // 4
    LONG     biYPelsPerMeter; // 4 /* Pixels per meter */
    DWORD    biClrUsed; // 4 /* Number of colours */
    DWORD    biClrImportant; // 4 /* Important colours */
} __attribute__((packed)) BITMAPINFOHEADER;

// This function reads a RGB bitmap file
unsigned char*  CGTexture::loadBMPFile(const char *filename)
{
    BITMAPINFOHEADER BMPInfoHeader;     //bitmap info header
    BITMAPFILEHEADER BMPFileHeader;	  //bitmap file header
    FILE *filePtr;					  //the file pointer
    unsigned char *bitmapImage;		  //bitmap image data
    unsigned char temp;				  //temp variable for swapping RGB values
    int imageSize, i = 0;		              //counter
    
    // open filename for reading in binary mode
    if ((filePtr = fopen(filename, "rb")) == NULL)
    {
        return NULL;
    }
    
    // read bitmap file header
    fread(&BMPFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
    
    // check whether file is really a bitmap file
    if (BMPFileHeader.bfType != 0x4D42)
    {
        fclose(filePtr); // if file is not a bitmap file the close and return NULL
        return NULL;
    }
    
    // read bitmap information header
    fread(&BMPInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
    
    // locate beginning of bitmap data
    fseek(filePtr, BMPFileHeader.bfOffBits, SEEK_SET);
    
    imageSize = BMPInfoHeader.biSizeImage;      // store image size
    
    
    
    size.width = BMPInfoHeader.biWidth;    // store image width
    size.height = BMPInfoHeader.biHeight;  // store image height
    
    // allocate memory for image data, check if not allocated properly then close file and return NULL
    if((bitmapImage = (unsigned char*)malloc(imageSize)) == NULL)
    {
        fclose(filePtr);
        return NULL;
    }
    
    // read bitmap data
    fread(bitmapImage, 1, imageSize, filePtr);
    
    // colours in bitmap are of the format BGR, hence need to swap B and R to get RGB
    for (i = 0; i < imageSize; i += 3)
    {
        temp = bitmapImage[i];
        bitmapImage[i] = bitmapImage[i+2];
        bitmapImage[i+2] = temp;
    }
    
    fclose(filePtr);    // close file
    return bitmapImage; // return pointer to image data
}