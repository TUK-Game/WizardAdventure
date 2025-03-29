#include "pch.h"
#include "TextWidget.h"
#include "AssetManager.h"
#include "Device.h"

CTextWidget::CTextWidget()
{
}

CTextWidget::~CTextWidget()
{
}

bool CTextWidget::Init()
{
	m_d2dDeviceContext = CDevice::GetInst()->m_d2dDeviceContext.Get();
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	D2D1_SIZE_F szRenderTarget = D2D1_SIZE_F(200, 200);

	D2D1_RECT_F rcUpperText = D2D1::RectF(100, 100, szRenderTarget.width, szRenderTarget.height * 0.25f);
	m_d2dDeviceContext->DrawTextW(L"w", (UINT32)wcslen(L"w"), CDevice::GetInst()->m_dwFont.Get(), &rcUpperText, CDevice::GetInst()->m_d2dbrText.Get());
	D2D1_RECT_F rcLowerText = D2D1::RectF(0, szRenderTarget.height * 0.5f, szRenderTarget.width, szRenderTarget.height);
	m_d2dDeviceContext->DrawTextW(L"한글 테스트", (UINT32)wcslen(L"한글 테스트"), CDevice::GetInst()->m_dwFont.Get(), &rcLowerText, CDevice::GetInst()->m_d2dbrText.Get());


	return true;
}
