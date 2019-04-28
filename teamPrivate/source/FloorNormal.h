#pragma once
#include "BaseFloor.h"
class FloorNormal :
	public BaseFloor
{
public:
	FloorNormal(VECTOR2 startPos, VECTOR2 size);
	~FloorNormal();
	void Update(void);
};
