// Image.h
//

struct Image_t {
	char name[512];
	unsigned int image;
	int width;
	int height;
};

Image_t* Image_LoadImage(const char* name);