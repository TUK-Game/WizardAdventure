#include "pch.h"
#include "TextWindow.h"
#include "TextWidget.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Device.h"

CTextWindow::CTextWindow()
{
}

CTextWindow::~CTextWindow()
{
}

bool CTextWindow::Init()
{
	CWidgetWindow::Init();
		
	m_BrushMap = CDevice::GetInst()->GetBrushMap();
	m_FontMap = CDevice::GetInst()->GetFontMap();

	{
		CTextWidget* widget = CreateWidget<CTextWidget>(L"Test");
		widget->SetSize(100, 100);
		widget->SetPosition(-0.8f, 0.8f);
		widget->SetText(L"test");
		widget->SetFont(m_FontMap[L"叡辞端"]);
		widget->SetColor(m_BrushMap[L"Green"]);
	}

	{
		CTextWidget* widget = CreateWidget<CTextWidget>(L"Test");
		widget->SetSize(200, 200);
		widget->SetPosition(0.5f, 0.5f);
		widget->SetText(L"ぞしぞし");
		widget->SetFont(m_FontMap[L"Arial"]);
		widget->SetColor(m_BrushMap[L"Blue"]);
	}


	return true;
}
