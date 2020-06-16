// Image.h
//

struct Image_t {
	Image_t();
	~Image_t();

	char name[512];
	unsigned int image;
	int width;
	int height;
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