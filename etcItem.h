#pragma once
#include "gameNode.h"
#include "item.h"

class etcItem : public item
{
private:

public:
	etcItem() {};
	~etcItem() {};

	HRESULT init(string _name)override;
	void release()override;
	void update()override;
	void render()override;

	void itemlist();
};
