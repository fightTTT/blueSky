#pragma once

#include "BaseScene.h"

class ResultScene :
	public BaseScene
{
public:
	ResultScene(bool leftFlag);
	~ResultScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	bool victoryFlag;
	int Init();
	void ResultDraw();
};

