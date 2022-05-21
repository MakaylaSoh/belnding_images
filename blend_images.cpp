/* Makayla Soh 9/30/2021
*  File name: blend_images.cpp
*  Description: Accepts two input bmp images, a ratio, and an output image file. Blends the two input images using 
*    the provided ratio to create an output image, placed in the provided output file, that is a blend of the two input images.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;

struct BITMAPFILEHEADER
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
};

struct BITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
};


/* 
** Function: get_color
** Parameters: pointer to image data, int pixel coordinates, real width in bytes, and desired color (rbg)
** Returns: color at pixel (x, y)
*/ 
BYTE get_color(BYTE *data, int x, int y, int real_wid, char rbg)
{
    //find index
    int index;
    if(rbg == 'b') 
        index = (3 * x) + (y * real_wid);
    else if(rbg == 'g')
        index = (3 * x) + (y * real_wid) + 1;
    else
        index = (3 * x) + (y * real_wid) + 2;
    
    return data[index];
}

/* 
** Function: get_color_bilinear
** Parameters: pointer to image data, float pixel coordinates, image pixel height, image pixel width, image's real width in bytes, and desired color (rbg)
** Returns: returns color at coordinate (ratio of 4 different pixels)
*/
BYTE get_color_bilinear(BYTE *data, float x, float y, int height, int width, int real_wid, char rbg)
{
    //surrounding pixels
    int x1 = (int)x, x2 = (int)x;
    int y1 = (int)y, y2 = (int)y;

    if(x != 0 && x != (height-1))
        x2 = (int)x + 1;

    if(y != 0 && y != (height - 1))
        y2 = (int)y + 1;

    //getting color of each coordinate
    float color_lu = (float)get_color(data, x1, y2, real_wid, rbg);
    float color_ru = (float)get_color(data, x2, y2, real_wid, rbg);
    float color_ll = (float)get_color(data, x1, y1, real_wid, rbg);
    float color_rl = (float)get_color(data, x2, y1, real_wid, rbg);

    //ratio of colors
    float dx = x - (int)x;
    float dy = y - (int)y;

    float color_l = (color_lu * (1 - dy)) + (color_ll * dy);
    float color_r = (color_ru * (1 - dy)) + (color_rl * dy);
    BYTE color = BYTE((color_l * (1 - dx)) + (color_r * dx));

    return color;
}


int main(int argc, char *argv[])
{
    //checking number of parameters
    if(argc > 5 || argc < 5)
    {
        printf("Wrong number of arguments. \nPlease enter: [program name] [image file 1] [image file 2] [ratio] [output file] \n");
        return -1;
    }

    //set/check ratio
    float ratio = atof(argv[3]);
    if(ratio > 1 || ratio < 0)
    {
        printf("Invalid ratio. \nPlease enter: [program name] [image file 1] [image file 2] [ratio] [output file] \n");
        return -1;
    } 
    
    //read first image
    FILE *image1 = fopen(argv[1], "rb");
    //check image1 exists
    if(image1 == NULL)
    {
        printf("Invalid file 1 input. \nPlease enter: [program name] [image file 1] [image file 2] [ratio] [output file] \n");
        fclose(image1);
        return -1;
    }
    
    struct BITMAPFILEHEADER fh1; 
    fread(&fh1.bfType, 2, 1, image1);
    fread(&fh1.bfSize, 4, 1, image1);
    fread(&fh1.bfReserved1, 2, 1, image1);
    fread(&fh1.bfReserved2, 2, 1, image1);
    fread(&fh1.bfOffBits, 4, 1, image1);

    struct BITMAPINFOHEADER fih1;
    fread(&fih1.biSize, 4, 1, image1);
    fread(&fih1.biWidth, 4, 1, image1); //number of pixels wide
    fread(&fih1.biHeight, 4, 1, image1); //number of pixels high
    fread(&fih1.biPlanes, 2, 1, image1);
    fread(&fih1.biBitCount, 2, 1, image1);
    fread(&fih1.biCompression, 4, 1, image1);
    fread(&fih1.biSizeImage, 4, 1, image1);
    fread(&fih1.biXPelsPerMeter, 4, 1, image1);
    fread(&fih1.biYPelsPerMeter, 4, 1, image1);
    fread(&fih1.biClrUsed, 4, 1, image1);
    fread(&fih1.biClrImportant, 4, 1, image1);

    //read image2
    FILE *image2 = fopen(argv[2], "rb");
    //check image2 exists
    if(image2 == NULL)
    {
        printf("Invalid file 2 input. \nPlease enter: [program name] [image file 1] [image file 2] [ratio] [output file]");
        fclose(image1);
        fclose(image2);
        return -1;
    }
    struct BITMAPFILEHEADER fh2; 
    fread(&fh2.bfType, 2, 1, image2);
    fread(&fh2.bfSize, 4, 1, image2);
    fread(&fh2.bfReserved1, 2, 1, image2);
    fread(&fh2.bfReserved2, 2, 1, image2);
    fread(&fh2.bfOffBits, 4, 1, image2);

    struct BITMAPINFOHEADER fih2;
    fread(&fih2.biSize, 4, 1, image2);
    fread(&fih2.biWidth, 4, 1, image2); //num pixels
    fread(&fih2.biHeight, 4, 1, image2); //num pixels
    fread(&fih2.biPlanes, 2, 1, image2);
    fread(&fih2.biBitCount, 2, 1, image2);
    fread(&fih2.biCompression, 4, 1, image2);
    fread(&fih2.biSizeImage, 4, 1, image2);
    fread(&fih2.biXPelsPerMeter, 4, 1, image2);
    fread(&fih2.biYPelsPerMeter, 4, 1, image2);
    fread(&fih2.biClrUsed, 4, 1, image2);
    fread(&fih2.biClrImportant, 4, 1, image2);


    //find bigger/smaller image
    FILE *imageBig, *imageSmall;
    struct BITMAPFILEHEADER fhBig, fhSmall;
    struct BITMAPINFOHEADER fihBig, fihSmall;
    if(fih1.biWidth > fih2.biWidth)
    {                
        imageBig = image1; imageSmall = image2;
        fhBig = fh1; fihBig = fih1;
        fhSmall = fh2; fihSmall = fih2;
    }
    else
    {
        imageBig = image2; imageSmall = image1;
        fhBig = fh2; fihBig = fih2;
        fhSmall = fh1; fihSmall = fih1;
        ratio = 1 - ratio;
    }

    //bigger image bytes wide
    int rwBig = 3 * fihBig.biWidth;
    if(rwBig % 4 != 0)
        rwBig = (rwBig + 4) - ((rwBig + 4) % 4);
    //smaller image bytes wide
    int rwSmall = 3 * fihSmall.biWidth;
    if(rwSmall % 4 != 0) 
        rwSmall = (rwSmall + 4) - ((rwSmall + 4) % 4);

    //allocate space
    BYTE *dataBig = (BYTE *)malloc(fihBig.biSizeImage);
    if(dataBig == NULL) printf("dataBig is NULL");
    BYTE *dataSmall = (BYTE *)malloc(fihSmall.biSizeImage);
    if(dataSmall == NULL) printf("dataSmall is NULL");
    BYTE *dataNew = (BYTE *)malloc(fihBig.biSizeImage);
    if(dataNew == NULL) printf("dataNew is NULL");

    //images data
    fread(dataBig, fihBig.biSizeImage, 1, imageBig);
    fread(dataSmall, fihSmall.biSizeImage, 1, imageSmall);

    for(int yB = 0; yB < fihBig.biHeight; yB++)
    {
        for(int xB = 0; xB < fihBig.biWidth; xB++)
        {
            //colors for imageBig
            BYTE blue1 = get_color(dataBig, xB, yB, rwBig, 'b');
            BYTE green1 = get_color(dataBig, xB, yB, rwBig, 'g');
            BYTE red1 = get_color(dataBig, xB, yB, rwBig, 'r'); 

            //imageSmall pixel coordinates
            float xScale = (float)(fihSmall.biWidth) / (float)(fihBig.biWidth);
            float yScale = (float)(fihSmall.biHeight) / (float)(fihBig.biHeight);
            float xS = xScale * (float)xB;
            float yS = yScale * (float)yB;
            
            //colors for imageSmall
            BYTE blue2 = get_color_bilinear(dataSmall, xS, yS, fihSmall.biHeight, fihSmall.biWidth, rwSmall, 'b');
            BYTE green2 = get_color_bilinear(dataSmall, xS, yS, fihSmall.biHeight, fihSmall.biWidth, rwSmall, 'g'); 
            BYTE red2 = get_color_bilinear(dataSmall, xS, yS, fihSmall.biHeight, fihSmall.biWidth, rwSmall, 'r'); 

            //set colors for new_image
            dataNew[(3 * xB) + (yB * rwBig)] = BYTE((ratio * float(blue1)) + ((1-ratio) * float(blue2)));
            dataNew[(3 * xB) + (yB * rwBig) + 1] = BYTE((ratio * float(green1)) + ((1-ratio) * float(green2)));
            dataNew[(3 * xB) + (yB * rwBig) + 2] = BYTE((ratio * float(red1)) + ((1-ratio) * float(red2)));
        }
    }


    //create new file
    FILE *fileNew = fopen(argv[4], "wb");
    //fwrite(&fhBig, sizeof(fhBig), 1, fileNew);
    fwrite(&fhBig.bfType, 2, 1, fileNew);
    fwrite(&fhBig.bfSize, 4, 1, fileNew);
    fwrite(&fhBig.bfReserved1, 2, 1, fileNew);
    fwrite(&fhBig.bfReserved2, 2, 1, fileNew);
    fwrite(&fhBig.bfOffBits, 4, 1, fileNew);

    //fwrite(&fihBig, sizeof(fihBig), 1, fileNew);
    fwrite(&fihBig.biSize, 4, 1, fileNew);
    fwrite(&fihBig.biWidth, 4, 1, fileNew); //number of pixels wide
    fwrite(&fihBig.biHeight, 4, 1, fileNew); //number of pixels high
    fwrite(&fihBig.biPlanes, 2, 1, fileNew);
    fwrite(&fihBig.biBitCount, 2, 1, fileNew);
    fwrite(&fihBig.biCompression, 4, 1, fileNew);
    fwrite(&fihBig.biSizeImage, 4, 1, fileNew);
    fwrite(&fihBig.biXPelsPerMeter, 4, 1, fileNew);
    fwrite(&fihBig.biYPelsPerMeter, 4, 1, fileNew);
    fwrite(&fihBig.biClrUsed, 4, 1, fileNew);
    fwrite(&fihBig.biClrImportant, 4, 1, fileNew);

    fwrite(dataNew, fihBig.biSizeImage, 1, fileNew);

    printf("The file finished running. \n");

    //close and free everything
    fclose(image1);
    fclose(image2);
    fclose(fileNew);

    free(dataBig);
    free(dataSmall);
    free(dataNew);

    return 0;
}
