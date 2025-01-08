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


#define DEVICE				CDevice::GetInst()->GetDevice()
#define CMD_LIST			CDevice::GetInst()->GetCmdQueue()->GetCmdList()
#define RESOURCE_CMD_LIST	CDevice::GetInst()->GetCmdQueue()->GetResourceCmdList()
#define ROOT_SIGNATURE		CDevice::GetInst()->GetRootSignature()->GetSignature()
#define CONST_BUFFER(type)	CDevice::GetInst()->GetConstantBuffer(type)


#define SWAP_CHAIN_BUFFER_COUNT		2
#define CBV_REGISTER_COUNT			(int)CBV_REGISTER::END
#define SRV_REGISTER_COUNT			(int)(static_cast<unsigned char>(SRV_REGISTER::END) - CBV_REGISTER_COUNT)
#define REGISTER_COUNT				CBV_REGISTER_COUNT + SRV_REGISTER_COUNT


#define MAX_LAYER	32