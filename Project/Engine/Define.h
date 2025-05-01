#pragma once

#define DECLARE_SINGLETON(type)\
private:\
	friend class CSingleton<type>;\
	type();\
	type(const type& other) = delete;\
	type(type&& other) = delete;\
	~type();\
	type& operator = (const type& other) = delete;\
	type& operator = (type&& other) = delete;

/*----------
	Device
-----------*/
#define DEVICE				CDevice::GetInst()->GetDevice()
#define GRAPHICS_CMD_LIST	CDevice::GetInst()->GetCmdQueue()->GetGraphicsCmdList()
#define COMPUTE_CMD_LIST	CDevice::GetInst()->GetComputeCmdQueue()->GetComputeCmdList()
#define RESOURCE_CMD_LIST	CDevice::GetInst()->GetCmdQueue()->GetResourceCmdList()
#define GRAPHICS_ROOT_SIGNATURE		CDevice::GetInst()->GetRootSignature()->GetGraphicsRootSignature()
#define COMPUTE_ROOT_SIGNATURE		CDevice::GetInst()->GetRootSignature()->GetComputeRootSignature()
#define CONST_BUFFER(type)	CDevice::GetInst()->GetConstantBuffer(type)

#define SWAP_CHAIN_BUFFER_COUNT		2
#define CBV_REGISTER_COUNT			(int)CBV_REGISTER::END
#define SRV_REGISTER_COUNT			(int)(static_cast<unsigned char>(SRV_REGISTER::END) - CBV_REGISTER_COUNT)
#define CBV_SRV_REGISTER_COUNT		(CBV_REGISTER_COUNT + SRV_REGISTER_COUNT)
#define UAV_REGISTER_COUNT			(int)(static_cast<unsigned char>(UAV_REGISTER::END) - CBV_SRV_REGISTER_COUNT)
#define TOTAL_REGISTER_COUNT		CBV_SRV_REGISTER_COUNT + UAV_REGISTER_COUNT

	
/*----------
	Path
-----------*/
#define ROOT_PATH		L"RootPath"
#define HLSL_PATH		L"HLSLPath"
#define TEXTURE_PATH	L"TexturePath"
#define SOUND_PATH		L"SoundPath"
#define MAP_PATH		L"MapPath"
#define JSON_PATH		L"JsonPath"


/*----------
	Time
-----------*/
#define DELTA_TIME			CEngine::GetInst()->GetDeltaTime()


/*----------
	Input
-----------*/
#define KEY_CHECK(key)		(CInputManager::GetInst()->GetKeyState(key))
#define KEY_DOWN(key)		(CInputManager::GetInst()->GetKeyState(key) == EKeyState::Down)
#define KEY_PUSH(key)		(CInputManager::GetInst()->GetKeyState(key) == EKeyState::Push)
#define KEY_UP(key)			(CInputManager::GetInst()->GetKeyState(key) == EKeyState::Up)
#define KEY_NONE(key)		(CInputManager::GetInst()->GetKeyState(key) == EKeyState::None)


/*----------
	Level
-----------*/
#define MAX_LAYER	32

/*----------
    Texture
-----------*/
#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05



#define READY_IMGUI

#define AUTO_SERVER_CONNECT

//#define DEBUG_SOLOPLAY

#define MAX_OBJECTS_SUBLEVEL 200

#define MAX_PLAYERS 4

#define INTERATION_DISTANCE 200

/*----------
	Random
-----------*/

static std::random_device _rd;
static std::default_random_engine _rng(_rd());

inline int RandomInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(_rng);
}

inline float RandomFloat(float min = 0.f, float max = 1.f)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(_rng);
}

// ·£´ý bool ¹ÝÈ¯ (50% È®·ü)
inline bool RandomBool()
{
	std::bernoulli_distribution dist(0.5);
	return dist(_rng);
}

// Æ¯Á¤ È®·ü·Î true ¹ÝÈ¯ (¿¹: È®·ü 0.25f ¡æ 25%)
inline bool RandomChance(float probability)
{
	std::bernoulli_distribution dist(probability);
	return dist(_rng);
}
