#include "TitleScene.h"
#include "GameCtrl.h"
#include "SelectScene.h"

#include "DxLib.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

unique_Base TitleScene::UpDate(unique_Base own, const GameCtrl & controller)
{

	if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
	{
		return std::make_unique<SelectScene>();
	}

	Draw();

	return std::move(own);
}

int TitleScene::Init()
{
	return 0;
}

void TitleScene::Draw()
{
	DrawString(1200, 200, "TitleScene", 0xffffff);

	DrawString(600, 600, "STARTƒ{ƒ^ƒ“ or SpaceKey‚Å‘JˆÚ", 0xffffff);
}
