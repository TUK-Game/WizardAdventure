#include "pch.h"
#include "PlayWidgetWindow.h"
#include "GaugeWidget.h"
#include "MeshRenderer.h"
#include "AssetManager.h"
#include "Transform.h"

CPlayWidgetWindow::CPlayWidgetWindow()
{
}

CPlayWidgetWindow::~CPlayWidgetWindow()
{
}

bool CPlayWidgetWindow::Init()
{
	CWidgetWindow::Init();
	
	CGaugeWidget* widget = CreateWidget<CGaugeWidget>(L"HPBar");
	widget->SetTexture(L"Kita");
	widget->GetTransform()->SetRelativePosition(-0.65f, -0.8f, 1.f);
	widget->GetTransform()->SetRelativeScale(0.4f, 0.05f, 0.2f);
	widget->SetGaugeScale(0.4f);
	widget->SetStartX(-0.65f);


	return true;
}
