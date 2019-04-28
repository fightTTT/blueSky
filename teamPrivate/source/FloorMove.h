#pragma once
#include "BaseFloor.h"

enum FLOOR_MOVE
{
	MOVE_DOWN,
	MOVE_UP,
	MOVE_MAX
};

class FloorMove :
	public BaseFloor
{
public:
	FloorMove(VECTOR2 startPos, VECTOR2 siz);
	~FloorMove();
	void Update(void);

private:
	FLOOR_MOVE moveType;
};

