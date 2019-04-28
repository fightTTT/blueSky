#include "FloorMove.h"

FloorMove::FloorMove(VECTOR2 startPos, VECTOR2 siz)
{
	BaseFloor::pos  = startPos;
	BaseFloor::size = siz;

	moveType = MOVE_DOWN;
}

FloorMove::~FloorMove()
{

}

void FloorMove::Update(void)
{
	if (moveType == MOVE_DOWN)
	{
		pos.y += 2;

		if (pos.y >= 500)
		{
			moveType = MOVE_UP;
		}
	}
	else
	{
		pos.y -= 2;

		if (pos.y <= 300)
		{
			moveType = MOVE_DOWN;
		}
	}
}
