#pragma once

#include "BaseScene.h"
#include "ObjList.h"
#include <map>
#include <vector>

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
	int animCnt;
	
	std::vector<std::string> charName;
};