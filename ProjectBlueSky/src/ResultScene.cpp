#include "ResultScene.h"
#include "GameCtrl.h"
#include "TitleScene.h"

#include "DxLib.h"

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

	return std::move(own);
}

int ResultScene::Init()
{
	return 0;
}

void ResultScene::ResultDraw()
{
	if (victoryFlag)
	{
		DrawString(500, 300, "1Pの勝ち！", 0xff0000);
	}
	else
	{
		DrawString(500, 300, "2Pの勝ち！", 0xff0000);
	}
	DrawString(500, 350, "STARTボタンでタイトルへ遷移", 0xffffff);
}
