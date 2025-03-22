#include "FBXConverter.h"

int main()
{
	CFBXConverter* converter = new CFBXConverter();

	converter->LoadFBX("crab");

	delete converter;
}