#include "FBXConverter.h"

int main()
{
	CFBXConverter* converter = new CFBXConverter();

	converter->LoadFBX("Crab");

	delete converter;
}