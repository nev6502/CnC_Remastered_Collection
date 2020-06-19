// Image.h
//

#define MAX_IMAGE_FRAMES 256
#define MAX_HOUSE_COLORS 8
struct Image_t {
	Image_t();
	~Image_t();

	char name[512];
	int64_t namehash;
	unsigned int image[MAX_HOUSE_COLORS][MAX_IMAGE_FRAMES];
	unsigned int numAnimFrames;
	int width;
	int height;
	int renderwidth;
	int renderheight;	
	unsigned char* buffer;
};

__forceinline Image_t::Image_t() {
	buffer = NULL;
	numAnimFrames = 0;
}

__forceinline Image_t::~Image_t() {
	if (buffer) {
		delete buffer;
		buffer = NULL;
	}
}

Image_t* Image_LoadImage(const char* name, bool loadAnims = false, bool loadHouseColor = false);
Image_t* Image_CreateImageFrom8Bit(const char* name, int Width, int Height, unsigned char* data, unsigned char *remap = NULL);