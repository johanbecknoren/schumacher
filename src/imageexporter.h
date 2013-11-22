#ifndef IMAGEEXPORTER_H
#define IMAGEEXPORTER_H

#include <string.h>

#include "FreeImage.h"

namespace ImageExporter
{
	void saveImage(char filename[], int width, int height);
	void saveImage(int image[], char filename[], int width, int height);

	
	FIBITMAP* convertScreenToBitmap(int width, int height);
	FIBITMAP* convertArrayToBitmap(int image[], int width, int height);

}//End namespace

 char* merge(char* folder, char* filename);

#endif //IMAGEEXPORTER_H
