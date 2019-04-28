#pragma once
#include "VECTOR2.h"

class BaseFloor
{
public:
	BaseFloor();
	~BaseFloor();

	VECTOR2 GetPos(void);
	VECTOR2 GetSize(void);
	virtual void Draw(void);

	virtual void Update(void) = 0;

protected:
	VECTOR2 pos;
	VECTOR2 size;
	VECTOR2 speed;
};

