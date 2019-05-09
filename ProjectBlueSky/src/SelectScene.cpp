#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "GameCtrl.h"
#include "VECTOR2.h"
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
	/* ¹Þ°Ñ¼°Ý‚ÖˆÚ“® */
	//if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
	//{
	//	return std::make_unique<GameScene>();
	//}

	SelectDraw();

	return std::move(own);
}

int SelectScene::Init()
{
	scSize = lpSceneMng.GetScreenSize();
	return 0;
}

void SelectScene::SelectDraw()
{
	VECTOR2 boxSize(100,100);

	DrawBox((scSize.x/2)-(boxSize.x*2), (scSize.y*3/5), ((scSize.x / 2) - (boxSize.x * 2))+boxSize.x, (scSize.y * 3 / 5) + boxSize.y, 0xff0000, true);
	DrawBox((scSize.x / 2) - (boxSize.x), (scSize.y * 3 / 5), ((scSize.x / 2) - (boxSize.x)) + boxSize.x, (scSize.y * 3 / 5) + boxSize.y, 0x0000aa, true);
	DrawBox((scSize.x / 2), (scSize.y * 3 / 5), (scSize.x / 2) + boxSize.x, (scSize.y * 3 / 5) + boxSize.y, 0xffffff, true);
	DrawBox((scSize.x / 2) + boxSize.x, (scSize.y * 3 / 5), ((scSize.x / 2) + boxSize.x) + boxSize.x, (scSize.y * 3 / 5) + boxSize.y, 0xaaaaaa, true);
	
	DrawBox((scSize.x / 2) - (boxSize.x * 2), (scSize.y * 3 / 5)+ boxSize.y, ((scSize.x / 2) - (boxSize.x * 2)) + boxSize.x, ((scSize.y * 3 / 5) + boxSize.y) + boxSize.y, 0xaa00aa, true);
	DrawBox((scSize.x / 2) - (boxSize.x), (scSize.y * 3 / 5) + boxSize.y, ((scSize.x / 2) - (boxSize.x)) + boxSize.x, ((scSize.y * 3 / 5) + boxSize.y) + boxSize.y, 0xff4488, true);
	DrawBox((scSize.x / 2), (scSize.y * 3 / 5) + boxSize.y, (scSize.x / 2) + boxSize.x, ((scSize.y * 3 / 5) + boxSize.y) + boxSize.y, 0xaaaaaa, true);
	DrawBox((scSize.x / 2) + boxSize.x, (scSize.y * 3 / 5) + boxSize.y, ((scSize.x / 2) + boxSize.x) + boxSize.x, ((scSize.y * 3 / 5) + boxSize.y) + boxSize.y, 0xff2222, true);

	//DrawString(1200, 200, "SelectScene", 0xffffff);

	//DrawString(600, 600, "STARTƒ{ƒ^ƒ“ or SpaceKey‚Å‘JˆÚ", 0xffffff);
}
