#pragma once
#include "ButtonWidget.h"



class CItemButtonWidget 
	: public CButtonWidget
{
	friend class CWidgetWindow;
public:
	CItemButtonWidget();
	virtual ~CItemButtonWidget();

public:
	void SetItemId(uint32 id) { m_ItemId = id; }
	void SetItem(std::shared_ptr<class CItem> item) { m_Item = item; }

	uint32 GetItemId() { return m_ItemId; }
	std::shared_ptr<class CItem> GetItem() { return m_Item; }
public:
	virtual bool Init(class CPlayer* none) override;
	virtual void Update() override;

public:
	virtual CItemButtonWidget* Clone() override { return new CItemButtonWidget(*this); }

private:
	uint32 m_ItemId;
	std::shared_ptr<class CItem> m_Item;
};

