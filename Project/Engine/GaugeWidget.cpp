#include "pch.h"
#include "GaugeWidget.h"
#include "LevelManager.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "AssetManager.h"
#include "Engine.h"

bool CGaugeWidget::Init(CPlayer* player)
{
    CImageWidget::Init(player);
	return true;
}

void CGaugeWidget::Update()
{
    if (m_Gauge >= 0)
    {
        //m_Gauge -= DELTA_TIME * 5;
        float HPPercentage = m_Gauge / m_MaxGauge;

        float OriginalWidth = m_GaugeScale;

        float NewWidth = OriginalWidth * HPPercentage;

        float OffsetX = -(OriginalWidth - NewWidth) / 2.0f;

        Vec3 pos = GetTransform()->GetRelativePosition();
        Vec3 scale = GetTransform()->GetRelativeScale();
        scale.x = NewWidth;
        pos.x = OffsetX + m_StartX;
        GetTransform()->SetRelativePosition(pos);
        GetTransform()->SetRelativeScale(scale);
    }
}
