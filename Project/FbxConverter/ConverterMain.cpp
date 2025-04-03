#include "FBXConverter.h"

int main()
{
	CFBXConverter* converter = new CFBXConverter();

	converter->LoadFBX("Level_1");

	delete converter;
}