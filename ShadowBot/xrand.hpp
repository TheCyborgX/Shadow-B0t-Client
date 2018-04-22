#include <random>
#include <sstream>
#include <Windows.h>

#define BYTE_MAX_VALUE 255
#define BYTE_MIN_VALUE 1


class xrand
{
public:
	BYTE GetRandByte(int min, int max);
	BYTE GetRandCharInByte();
	BYTE GetRandIntInByte();
	void GetRandHexInChars(char* buff, int len);
	void GetRandBytesInVector(BYTE* vector, int len, int min, int max);
	unsigned long CharToHex(char* buff);
	void HexToChar(char* buff, int buffsz, BYTE* vector, int len);
	void InvertVector(BYTE* oVector, BYTE* nVector, int len);
};


class AsciiTypes
{
public:
	bool IsInt(BYTE value);
	bool IsChar(BYTE value);
	bool IsSymbol(BYTE value);
};