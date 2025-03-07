#include "FBXConverter.h"

int main()
{
	CFBXConverter* converter = new CFBXConverter();

	// 모델이 유니티 상에서 변환이 적용되었을 때 -> TRUE
	// 애니메이션 -> FALSE
	converter->LoadFBX("level_bossroom", false);

	delete converter;
}