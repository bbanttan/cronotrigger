#pragma once
#include "gameNode.h"
#include "item.h"


class ExpendableItem : public item
{
private:

public:
	ExpendableItem() {};
	~ExpendableItem() {};

	HRESULT init(string _name)override;
	void release()override;
	void update()override;
	void render()override;

	void itemlist();
};

