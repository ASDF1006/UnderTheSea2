#include "DXUT.h"
#include "Item.h"


Item::Item(ItemType type): type(type)
{
	switch (type)
	{
	case I_HP: ani = new Animation("image/Item/heal/", 21, 21, 1, 1); break;
	case I_ADDON: ani = new Animation("image/Item/addon/", 21, 21, 1, 1); break;
	case I_TORPEDO: ani = new Animation("image/Item/nuclear/", 21, 21, 1, 1); break;
	case I_SHIELD: ani = new Animation("image/Item/shield/", 21, 21, 1, 1); break;
	case I_COIN: ani = new Animation("image/Item/coin/", 21, 21, 1, 1); break;
	}

	addChild(ani);
	rect = ani->rect;
}


Item::~Item()
{
}