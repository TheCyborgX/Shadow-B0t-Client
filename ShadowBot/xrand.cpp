#include "main.hpp"



void xrand::GetRandBytesInVector(BYTE* vector, int len, int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> BYTES_RANGE(min, max);

	for (int i = 0; i < len; i++)
		vector[i] = BYTES_RANGE(gen);
	
}


void xrand::GetRandHexInChars(char* buff, int len)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> HEX_CHAR_RANGE(97, 102), HEX_INT_RANGE(48, 57), RAND_TYPE(0, 1);

	for (int i = 0; i < len; i++)
		buff[i] = RAND_TYPE(gen) ? HEX_CHAR_RANGE(gen) : HEX_INT_RANGE(gen);

	buff[len] = 0x0;
}



unsigned long xrand::CharToHex(char* buff)
{
	std::stringstream ss;
	ss << std::hex << buff;
	unsigned long result;
	ss >> result;
	return result;
}


void xrand::HexToChar(char* buff, int buffsz, BYTE* vector, int len)
{
	char* tmpbuff = new char[buffsz];
	strcpy_s(buff, sizeof(""), "");
	for (int i = 0; i < len; i++)
		sprintf_s(tmpbuff, buffsz, "%x", vector[i]), strcat_s(buff, buffsz, tmpbuff);
}

void xrand::InvertVector(BYTE* oVector, BYTE* nVector, int len)
{
	for (int i = len - 1, j = 0; i > -1; i--, j++)
		nVector[j] = oVector[i];
}

BYTE xrand::GetRandByte(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> BYTES_RANGE(min, max);
	return BYTES_RANGE(gen);
}



BYTE xrand::GetRandCharInByte()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> UP_CHAR_RANGE(65, 90), LOW_CHAR_RANGE(97, 122), RAND_TYPE(0, 1);

	return (BYTE)RAND_TYPE(gen) ? UP_CHAR_RANGE(gen) : LOW_CHAR_RANGE(gen);
}

BYTE xrand::GetRandIntInByte()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> INT_RANGE(48, 57);

	return (BYTE)INT_RANGE(gen);
}


// another class


bool AsciiTypes::IsInt(BYTE value)
{
	return ((int)value > 47 && (int)value < 58);
}

bool AsciiTypes::IsChar(BYTE value)
{
	return ((int)value > 63 && (int)value < 90 || (int)value > 96 && (int)value < 123);
}
