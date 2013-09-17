#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <string>

#include <GL/glfw.h>
#include <GL/freeglut.h>

#include "imageexporter.h"

int BitsPerPixel = 24;

char* ImageExporter::merge(char* folder, char* filename)
{
	char *newArray = new char[std::strlen(folder)+std::strlen(filename)+1];
	std::strcpy(newArray,folder);
    std::strcat(newArray,filename);

	return newArray;
}

FIBITMAP* ImageExporter::convertScreenToBitmap(int width, int height)
{
	BYTE* pixels = new BYTE[3 * width * height];

    glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

	FIBITMAP * image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, BitsPerPixel, 0x0000FF, 0xFF0000, 0x00FF00, false);

	return image;
}

FIBITMAP* ImageExporter::convertArrayToBitmap(int image[], int width, int height) 
{
	BYTE *pixels = new BYTE[3 * width * height];
	RGBQUAD color;
	FIBITMAP *bitmap = FreeImage_Allocate(width, height, BitsPerPixel);
	for (int w = 0; w < width; ++w) {
		for (int h = 0; h < height; ++h) {
			int id = h * width * 3 + w * 3;
			color.rgbRed = image[id];
			color.rgbGreen = image[id + 1];
			color.rgbBlue = image[id + 2];
			FreeImage_SetPixelColor(bitmap, w, h, &color);
		}
	}
	return bitmap;
}

void ImageExporter::saveImage(char filename[], int width, int height)
{
	//På windows måste det finnas en mapp för att den ska sparas. 
	//Borde ändras till att be användaren att välja mapp
	char folder[] = "export/";
    char *fend = merge(filename, (char*)".png");
	char *file = merge(folder, fend);
	
	FIBITMAP * bitmap = convertScreenToBitmap(width, height);
	FreeImage_Save(FIF_PNG, bitmap, file, BitsPerPixel);
}

void ImageExporter::saveImage(int image[], char filename[], int width, int height)
{
	char folder[] = "export/";
	char *fend = merge(filename, (char*)".png");
	char *file = merge(folder, fend);

	FIBITMAP *bitmap = convertArrayToBitmap(image, width, height);
	FreeImage_Save(FIF_PNG, bitmap, file, BitsPerPixel);
}

bool ImageExporter::saveSequence(int &n, float dt, int range, int width, int height)
{
    if(ImageExporter::timeTrack > 1.0 / ImageExporter::fps) {
		std::stringstream strs;
		strs << n;
		std::string temp_str = strs.str();
		saveImage((char*) temp_str.c_str(),
				  width, height);
        n++;
        ImageExporter::timeTrack = 0.0f;
    }
    else
        ImageExporter::timeTrack += dt;
	if(n == range)
		return false;
	return true;
}

void ImageExporter::saveSequence(int &n, int width, int height)
{
	std::stringstream strs;
	strs << n;
	std::string temp_str = strs.str();

	saveImage((char*) temp_str.c_str(),
			  width, height);
	std::cout << "saved "  << temp_str << std::endl;
    n++;
}

