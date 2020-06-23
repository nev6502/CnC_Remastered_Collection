// FileBinary.h
//

class WwFile {
public:
	static bool FileExists(const char* name) {
		FILE* f = fopen(name, "rb");
		if (f) {
			fclose(f);
			return true;
		}

		return false;
	}

	WwFile() {
		file = NULL;
		length = -1;
	}

	~WwFile() {
		if (file != NULL) {
			fclose(file);
			file = NULL;
		}
	}

	void InitOpen(const char* name) {
		file = fopen(name, "rb");
		GetFileLength();
	}

	void InitWrite(const char* name) {
		file = fopen(name, "wb");
	}

	void InitAppend(const char* name) {
		file = fopen(name, "ab");
	}

	void Scanf(char* format, ...) {
		va_list arg;
		int done;
		va_start(arg, format);
		done = vfscanf(file, format, arg);
		va_end(arg);
	}

	void Printf(char* format, ...) {
		va_list argptr;
		static char string[4096];

		va_start(argptr, format);
		vsprintf(string, format, argptr);
		va_end(argptr);

		fprintf(file, string);
	}

	void Seek(int64_t pos, int set) {
		_fseeki64(file, pos, set);
	}

	void Read(void* data, int size) {
		fread(data, 1, size, file);
	}

	void Write(void* data, int size) {
		fwrite(data, 1, size, file);
	}

	template<class type> size_t ReadBig(type& c) {
		size_t r = Read(&c, sizeof(c));
		return r;
	}

	template<class type> size_t ReadBigArray(type* c, int count) {
		size_t r = Read(c, sizeof(c[0]) * count);
		return r;
	}

	template<class type> size_t WriteBig(const type& c) {
		type b = c;
		return Write(&b, sizeof(b));
	}

	template<class type> size_t WriteBigArray(const type* c, int count) {
		size_t r = 0;
		for (int i = 0; i < count; i++) {
			r += Write(c[i], sizeof(type));
		}
		return r;
	}

	bool EndOfFile(void) {
		return Tell() >= length;
	}

	int64_t Tell(void) {
		return _ftelli64(file);
	}

	void Flush(void) {
		fflush(file);
	}
private:
	void GetFileLength(void) {
		int c = ftell(file);
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, c, SEEK_SET);
	}

	FILE* file;
	int length;
};
