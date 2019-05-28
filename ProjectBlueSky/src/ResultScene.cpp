#include "DxLib.h"
#include "ResultScene.h"
#include "GameCtrl.h"
#include "TitleScene.h"
#include "ImageMng.h"
#include "SceneMng.h"
#include <string>

ResultScene::ResultScene(bool leftFlag)
{
	victoryFlag = leftFlag;
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

	charName = { "�_�l��",
				"�_�l��_��",
				"�_�l��_��",
				"�_�l��_��",
				"�_�l��_��",
				"�_�l��_��",
				"�_�l��_�s���N",
				"�_�l��_���F" };

	animCnt = 0;

	return 0;
}

void ResultScene::ResultDraw()
{
	
	DrawString(500, 350, "START�{�^���Ń^�C�g���֑J��", 0xffffff);

	std::string imagePass[2];

	if (victoryFlag)
	{
		imagePass[0] = ("image/" + charName[lpSceneMng.GetCharID(PAD_1)] + "/����/"+"win_" + std::to_string(animCnt / 4 % 5) + ".png");
		imagePass[1] = ("image/" + charName[lpSceneMng.GetCharID(PAD_2)] + "/����/"+"lose_" + std::to_string(animCnt / 8 % 9) + ".png");
	}
	else
	{
		imagePass[0] = ("image/" + charName[lpSceneMng.GetCharID(PAD_1)] + "/����/"+"lose_" + std::to_string(animCnt / 8 % 9) + ".png");
		imagePass[1] = ("image/" + charName[lpSceneMng.GetCharID(PAD_2)] + "/����/"+"win_" + std::to_string(animCnt / 4 % 5) + ".png");
	}



	for (int num = 0; num < 2; num++)
	{
		int x = 1280 / 4 * (1 + (num * 2));
		DrawRotaGraph(x, 720 / 2, 1.0, 0.0, lpImageMng.GetID(imagePass[num])[0], true, num);
	}

}
