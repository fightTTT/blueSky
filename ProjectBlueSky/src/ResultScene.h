#pragma once

#include "BaseScene.h"

class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	~ResultScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:

	int Init();
	void ResultDraw();
};

