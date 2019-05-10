#pragma once

#include "VECTOR2.h"
#include "BaseScene.h"
#include "ObjList.h"


class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	VECTOR2 scSize;

	int Init(void);
	void SelectDraw(void);
	sharedListObj objList;
};

