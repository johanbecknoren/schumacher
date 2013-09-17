#ifndef IMAGEEXPORTER_H
#define IMAGEEXPORTER_H

#include <string.h>

#include "FreeImage.h"

namespace ImageExporter
{

    static float fps = 30;
    static float timeTrack = 0;
	void saveImage(char filename[], int width, int height);
	void saveImage(int image[], char filename[], int width, int height);

	char* merge(char* folder, char* filename);
	FIBITMAP* convertScreenToBitmap(int width, int height);
	FIBITMAP* convertArrayToBitmap(int image[], int width, int height);

}//End namespace

#endif //IMAGEEXPORTER_H
