#include "SelectScene.h"
#include "GameScene.h"
#include "GameCtrl.h"

#include "DxLib.h"

SelectScene::SelectScene()
{
	Init();
}

SelectScene::~SelectScene()
{
}

unique_Base SelectScene::UpDate(unique_Base own, const GameCtrl & controller)
{

	if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
	{
		return std::make_unique<GameScene>();
	}

	Draw();

	return std::move(own);
}

int SelectScene::Init()
{
	return 0;
}

void SelectScene::Draw()
{
	DrawString(1200, 200, "SelectScene", 0xffffff);

	DrawString(600, 600, "STARTƒ{ƒ^ƒ“ or SpaceKey‚Å‘JˆÚ", 0xffffff);
}
