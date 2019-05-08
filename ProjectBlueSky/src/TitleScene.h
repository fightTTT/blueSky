#pragma once

#include "BaseScene.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	
	int Init();
	void Draw();
};

