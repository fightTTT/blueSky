#include "DxLib.h"
#include "BaseFloor.h"

BaseFloor::BaseFloor()
{
}


BaseFloor::~BaseFloor()
{
}

VECTOR2 BaseFloor::GetPos(void)
{
	return pos;
}

VECTOR2 BaseFloor::GetSize(void)
{
	return size;
}

void BaseFloor::Draw(void)
{
	DrawBox(pos.x,pos.y,pos.x + size.x,pos.y + size.y,0xffff00,true);
}
