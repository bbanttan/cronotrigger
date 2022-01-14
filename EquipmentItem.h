#pragma once
#include "gameNode.h"
#include "item.h"


class EquipmentItem :public item
{
private : 
	
public:
	EquipmentItem() {};
	~EquipmentItem() {};

	HRESULT init(string _name)override;
	void release()override;
	void update()override;
	void render()override;

	void itemlist();
};

