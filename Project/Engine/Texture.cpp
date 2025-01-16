#include "pch.h"
#include "Texture.h"
#include "Device.h"

CTexture::CTexture()
	: CAsset(EAsset_Type::Texture)
	, m_SrvHandle{}
{
}

CTexture::~CTexture()
{
}

int CTexture::Init(const std::wstring& path, UINT textureType)
{
	if (FAILED(CreateTexture(path)))
		return E_FAIL;

	if (FAILED(CreateView(textureType)))
		return E_FAIL;

	return S_OK;
}

int CTexture::CreateTexture(const std::wstring& path)
{
	// 파일 확장자 얻기
	std::wstring ext = std::filesystem::path(path).extension();

	if (ext == L".dds" || ext == L".DDS")
		::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	else if (ext == L".tga" || ext == L".TGA")
		::LoadFromTGAFile(path.c_str(), nullptr, m_Image);
	else // png, jpg, jpeg, bmp
		::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);

	HRESULT hr = ::CreateTexture(DEVICE.Get(), m_Image.GetMetadata(), &m_Tex2D);
	if (FAILED(hr))
	{
		assert(nullptr);
		return E_FAIL;
	}

	std::vector<D3D12_SUBRESOURCE_DATA> subResources;

	hr = ::PrepareUpload(DEVICE.Get(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		subResources);

	if (FAILED(hr))
	{
		assert(nullptr);
		return E_FAIL;
	}

	const UINT64 bufferSize = ::GetRequiredIntermediateSize(m_Tex2D.Get(), 0, static_cast<UINT>(subResources.size()));

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Resource> textureUploadHeap;
	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureUploadHeap.GetAddressOf()));

	if (FAILED(hr))
	{
		assert(nullptr);
		return E_FAIL;
	}

	::UpdateSubresources(RESOURCE_CMD_LIST.Get(),
		m_Tex2D.Get(),
		textureUploadHeap.Get(),
		0, 0,
		static_cast<unsigned int>(subResources.size()),
		subResources.data());

	CDevice::GetInst()->GetCmdQueue()->FlushResourceCommandQueue();

	return S_OK;
}

int CTexture::CreateView(UINT textureType)
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (FAILED(DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_SrvHeap))))
		return E_FAIL;

	m_SrvHandle = m_SrvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = m_Image.GetMetadata().format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	switch (textureType)
	{
	case RESOURCE_TEXTURE2D: 
	case RESOURCE_TEXTURE2D_ARRAY:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.PlaneSlice = 0;
		break;
	case RESOURCE_TEXTURE_CUBE: 
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		break;
	}

	DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &srvDesc, m_SrvHandle);

	return S_OK;
}
