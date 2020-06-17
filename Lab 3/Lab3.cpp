#include <iostream>
#include "libbmp.h"

using namespace std;

bool getCharR(BmpImg* img, int x, int y, int& count, int& decrypt) {
	decrypt = decrypt + (img->red_at(x, y) % 2) * pow(2, count);
	return true;
}

bool getCharG(BmpImg* img, int x, int y, int& count, int& decrypt) {
	decrypt = decrypt + (img->green_at(x, y) % 2) * pow(2, count);
	return true;
}

bool getCharB(BmpImg* img, int x, int y, int& count, int& decrypt) {
	decrypt = decrypt + (img->blue_at(x, y) % 2) * pow(2, count);
	return true;
}

bool concl(int& count, int& decrypt) {

	if (count == 0)
	{
		if (char(decrypt) == '\0') return false;

		std::cout << char(decrypt);

		count = 8;
		decrypt = 0;
	}
	return true;
}

int main()
{
	BmpImg img;
	img.read("pic5.bmp"); //11r 11b 11g 01r 01b 01g 10r 10b
	int count = 7;
	int decrypt = 0;

	for (int i = img.get_height() - 1; i >= 0; i--) {
		for (int j = img.get_width() - 1; j >= 0; j--) { 
			if (!(getCharR(&img, j, i, count, decrypt) && concl(count, decrypt))) return 0;
			count--;

			if (!(getCharB(&img, j, i, count, decrypt) && concl(count, decrypt))) return 0;
			count--;

			if (!(getCharG(&img, j, i, count, decrypt) && concl(count, decrypt))) return 0;
			count--;

		}
	}
}
