#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <string>
#include <ctime>

#include "utils.h"
#include "imageexporter.h"

int BitsPerPixel = 24;



FIBITMAP* ImageExporter::convertArrayToBitmap(int image[], int width, int height) 
{
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

void ImageExporter::saveImage(int image[], char filename[], int width, int height)
{
	std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [120];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer, 120, "%Y-%m-%d-%H-%M-%S", timeinfo);
	
#ifdef CMAKE_BINARY_DIR
	char ef[] = "/export/";
	char dir[] = CMAKE_BINARY_DIR;
	char *folder = merge(dir, ef);
#else
	char folder[] = "export/";
#endif
	char *new_buffer = merge(buffer, filename);
	char *fend = merge(new_buffer, (char*)".png");
// 	fend = merge(filename, fend);
	//char *fend = merge(filename, (char*)".png");
	char *file = merge(folder, fend);
std::cout << "Received filename: " << file << "\n";
	FIBITMAP *bitmap = convertArrayToBitmap(image, width, height);
	FreeImage_Save(FIF_PNG, bitmap, file, BitsPerPixel);
}

