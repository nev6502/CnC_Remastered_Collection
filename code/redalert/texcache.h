// Texcache.h
//

#include "FILEBINARY.H"
#include <vector>

/*
================
return a hash value for the filename
================
*/
__forceinline int64_t generateHashValue(const char* fname, const int size) {
	uint32_t hash = 0x811c9dc5;
	uint32_t prime = 0x1000193;

	for (int i = 0; i < size; ++i) {
		uint8_t value = fname[i];
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

struct CacheEntry_t {
	char name[2048];
	int64_t hash;
	int width;
	int height;
	int bpp;
	int64_t cachePosition;
};

class TexCache {
public:
	void			Init(void);
	void			AddImage(const char* name, int width, int height, int bpp, byte* data);
	bool			FindCachedImage(int64_t hash, int &width, int &height, int &bpp, byte *data);
private:
	WwFile*			texCacheHeaderFile;
	WwFile*			texCacheHeaderFileWrite;
	WwFile*			texCacheDataFile;
	WwFile*			texCacheDataFileWrite;
private:
	std::vector< CacheEntry_t> cacheEntries;
};

extern TexCache texCache;