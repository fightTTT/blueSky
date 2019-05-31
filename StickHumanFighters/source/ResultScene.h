#pragma once

#include "BaseScene.h"
#include "ObjList.h"
#include "GameCtrl.h"
#include "VECTOR2.h"
#include <vector>
#include <array>
#include <string>

class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	ResultScene(PAD_ID padData,bool drawFlag);
	~ResultScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	int Init();
	void ResultDraw();
	PAD_ID victoryPad;
	PAD_ID defeatPad;
	int victoryCharID;
	int defeatCharID;
	bool drawflag;
	int animCnt;
	int itvCnt;
	bool returnTitle;
	char tmpImagePass[50];
	std::array<std::vector<std::string>, 2> imagePass;
	std::array <std::string,2> playerfontPass;
	std::array<std::string, 2> victoryPass;
	std::array<std::string, 2> shadowPass;
	std::vector<std::string> charName;
	std::array<int, 2> animFrame;
	std::array<VECTOR2, 2> animOffSet;
};