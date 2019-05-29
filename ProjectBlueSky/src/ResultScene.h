#pragma once

#include "BaseScene.h"
#include "ObjList.h"
#include "GameCtrl.h"
#include <vector>
<<<<<<< HEAD
#include <array>
#include <string>

//enum PAD_ID;

=======
#include <string>
>>>>>>> d43b9c39c50989296496f9c7899fb9b27eaa382c

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
	std::vector<std::string> charName;
	std::array<int, 2> animFrame;
};