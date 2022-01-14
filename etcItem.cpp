#include "stdafx.h"
#include "etcItem.h"

HRESULT etcItem::init(string _name)
{
	itemName = _name;
	itemlist();
	return S_OK;
}

void etcItem::release()
{
}

void etcItem::update()
{
}

void etcItem::render()
{
}

void etcItem::itemlist()
{
	
}
