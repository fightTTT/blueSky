#pragma once

#include "BaseScene.h"
#include "ObjList.h"
#include "GameCtrl.h"
#include <vector>
#include <array>
#include <string>

class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	ResultScene(PAD_ID padData);
	~ResultScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	PAD_ID victoryPad;
	PAD_ID defeatPad;
	int victoryCharID;
	int defeatCharID;
	int Init();
	void ResultDraw();
	int animCnt;
	std::array<std::vector<std::string>, 2> imagePass;
	std::array <std::string,2> playerfontPass;
	std::vector<std::string> charName;
	std::array<int, 2> animFrame;
};