// Image.h
//

struct Image_t {
	Image_t();
	~Image_t();

	char name[512];
	int64_t namehash;
	unsigned int image;
	int width;
	int height;
	int renderwidth;
	int renderheight;	
	unsigned char* buffer;
};

__forceinline Image_t::Image_t() {
	buffer = NULL;
}

__forceinline Image_t::~Image_t() {
	if (buffer) {
		delete buffer;
		buffer = NULL;
	}
}

Image_t* Image_LoadImage(const char* name);
Image_t* Image_CreateImageFrom8Bit(const char* name, int Width, int Height, unsigned char* data, unsigned char *remap = NULL);