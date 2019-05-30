#pragma once

#include "BaseScene.h"
#include <vector>

class ExplanationScene :
	public BaseScene
{
public:
	ExplanationScene();
	~ExplanationScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	int Init();
	void ExplanationDraw();

	int frame;

	int page;

	bool sceneChangeFlag;
	int sceneChangeTime;

	std::vector<int> imageHandle;
};

