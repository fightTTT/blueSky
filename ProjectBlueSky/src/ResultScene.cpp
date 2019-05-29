#include "DxLib.h"
#include "ResultScene.h"
#include "TitleScene.h"
#include "ImageMng.h"
#include "SceneMng.h"
#include <string>

ResultScene::ResultScene()
{
}

ResultScene::ResultScene(PAD_ID padData)
{
	victoryPad = padData;
	
	switch (victoryPad)
	{
	case PAD_1:
		if (lpSceneMng.GetMode() == MODE_1PLAYER)
		{
			defeatPad = PAD_AI;
		}
		else
		{
			defeatPad = PAD_2;
		}
		break;
	case PAD_2:
		defeatPad = PAD_1;
		break;
	case PAD_AI:
		defeatPad = PAD_1;
		break;
	default:
		break;
	}

	Init();
}

ResultScene::~ResultScene()
{
}

unique_Base ResultScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_1, BUTTON_A)
		|| controller.GetPadDataTrg(PAD_2, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_A))
	{
		return std::make_unique<TitleScene>();
	}

	ResultDraw();
	animCnt++;
	return std::move(own);
}

int ResultScene::Init()
{
	victoryCharID = lpSceneMng.GetCharID(victoryPad);
	defeatCharID  = lpSceneMng.GetCharID(defeatPad);
	

	charName = { "ñ_êlä‘",
				"ñ_êlä‘_çï",
				"ñ_êlä‘_ê‘",
				"ñ_êlä‘_ê¬",
				"ñ_êlä‘_óŒ",
				"ñ_êlä‘_â©",
				"ñ_êlä‘_ÉsÉìÉN",
				"ñ_êlä‘_êÖêF" };

	char tmpImagePass[50];
	imagePass[0].resize(5);
	for (unsigned int i = 0; i < imagePass[0].size(); i++)
	{
		sprintf_s(tmpImagePass, "image/%s/èüÇø/win_%d.png", charName[lpSceneMng.GetCharID(victoryPad)].c_str(), i);
		imagePass[0][i] = tmpImagePass;
	}
	imagePass[1].resize(9);
	for (unsigned int i = 0; i < imagePass[1].size(); i++)
	{
		sprintf_s(tmpImagePass, "image/%s/ïâÇØ/lose_%d.png", charName[lpSceneMng.GetCharID(defeatPad)].c_str(), i);
		imagePass[1][i] = tmpImagePass;
	}
	animCnt = 0;

	return 0;
}

void ResultScene::ResultDraw()
{
	DrawString(500, 350, "STARTÉ{É^ÉìÇ≈É^ÉCÉgÉãÇ÷ëJà⁄", 0xffffff);

	//imagePass[0] = ("image/" + charName[lpSceneMng.GetCharID(victoryPad)] + "/èüÇø/" + "win_" + std::to_string(animCnt / 4 % 5) + ".png");
	//imagePass[1] = ("image/" + charName[lpSceneMng.GetCharID(defeatPad)] + "/ïâÇØ/" + "lose_" + std::to_string(animCnt / 8 % 9) + ".png");

	animFrame = {4 - abs(((animCnt / 4) % 9) - 4)  ,animCnt / 8 % 9 };

	int charDrawPos_x;
	bool turnFlag;

	for (int num = 0; num < 2; num++)
	{
		if (victoryPad == PAD_1) 
		{
			charDrawPos_x = 1280 / 4 * (1 + (num * 2));
			turnFlag = num;
		}
		else
		{
			charDrawPos_x = 1280 / 4 * (1 + ((1 - num) * 2));
			turnFlag = 1 - num;
		}
		DrawRotaGraph(charDrawPos_x, 720 / 2, 1.0, 0.0, lpImageMng.GetID(imagePass[num][animFrame[num]])[0], true, turnFlag);
	}

}
