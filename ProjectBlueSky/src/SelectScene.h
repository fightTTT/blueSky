#pragma once

#include "BaseScene.h"

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:

	int Init();
	void Draw();
};

