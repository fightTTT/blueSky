#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "GameCtrl.h"
#include "VECTOR2.h"
#include "DxLib.h"
#include "CharSelCursor.h"

#define BOX_SIZE_X (100)
#define BOX_SIZE_Y (100)

SelectScene::SelectScene()
{
	Init();
}

SelectScene::~SelectScene()
{
}

unique_Base SelectScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	for (auto& data : *objList)
	{
		data->UpDate(controller, objList);
	}

	/* πﬁ∞—º∞›Ç÷à⁄ìÆ */
	//if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
	//{
	//	return std::make_unique<GameScene>();
	//}

	SelectDraw();

	return std::move(own);
}

int SelectScene::Init()
{
	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objListÇëSçÌèúÇ∑ÇÈ

	AddObjList()(objList, std::make_unique<CharSelCursor>());		// ÿΩƒÇ…Cursor∏◊ΩÇí«â¡

	charID = 0;

	// √∞ÃﬁŸ
	posTbl = { VECTOR2((scSize.x / 2) - (BOX_SIZE_X * 2),	(scSize.y * 3 / 5)),
			   VECTOR2((scSize.x / 2) - (BOX_SIZE_X),		(scSize.y * 3 / 5)),
			   VECTOR2((scSize.x / 2),						(scSize.y * 3 / 5)),
			   VECTOR2((scSize.x / 2) + BOX_SIZE_X,			(scSize.y * 3 / 5)),
			   VECTOR2((scSize.x / 2) - (BOX_SIZE_X * 2),	(scSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((scSize.x / 2) - (BOX_SIZE_X),		(scSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((scSize.x / 2),						(scSize.y * 3 / 5) + BOX_SIZE_Y),
			   VECTOR2((scSize.x / 2) + BOX_SIZE_X,			(scSize.y * 3 / 5) + BOX_SIZE_Y) };

	scSize = lpSceneMng.GetScreenSize();
	return 0;
}

void SelectScene::SelectDraw()
{

	DrawBox((scSize.x/2)-(BOX_SIZE_X*2), (scSize.y*3/5), ((scSize.x / 2) - (BOX_SIZE_X * 2))+ BOX_SIZE_X, (scSize.y * 3 / 5) + BOX_SIZE_Y, 0xff0000, true);
	DrawBox((scSize.x / 2) - (BOX_SIZE_X), (scSize.y * 3 / 5), ((scSize.x / 2) - (BOX_SIZE_X)) + BOX_SIZE_X, (scSize.y * 3 / 5) + BOX_SIZE_Y, 0x0000aa, true);
	DrawBox((scSize.x / 2), (scSize.y * 3 / 5), (scSize.x / 2) + BOX_SIZE_X, (scSize.y * 3 / 5) + BOX_SIZE_Y, 0xffffff, true);
	DrawBox((scSize.x / 2) + BOX_SIZE_X, (scSize.y * 3 / 5), ((scSize.x / 2) + BOX_SIZE_X) + BOX_SIZE_X, (scSize.y * 3 / 5) + BOX_SIZE_Y, 0xaaaaaa, true);
	
	DrawBox((scSize.x / 2) - (BOX_SIZE_X * 2), (scSize.y * 3 / 5)+ BOX_SIZE_Y, ((scSize.x / 2) - (BOX_SIZE_X * 2)) + BOX_SIZE_X, ((scSize.y * 3 / 5) + BOX_SIZE_Y) + BOX_SIZE_Y, 0xaa00aa, true);
	DrawBox((scSize.x / 2) - (BOX_SIZE_X), (scSize.y * 3 / 5) + BOX_SIZE_Y, ((scSize.x / 2) - (BOX_SIZE_X)) + BOX_SIZE_X, ((scSize.y * 3 / 5) + BOX_SIZE_Y) + BOX_SIZE_Y, 0xff4488, true);
	DrawBox((scSize.x / 2), (scSize.y * 3 / 5) + BOX_SIZE_Y, (scSize.x / 2) + BOX_SIZE_X, ((scSize.y * 3 / 5) + BOX_SIZE_Y) + BOX_SIZE_Y, 0xaaaaaa, true);
	DrawBox((scSize.x / 2) + BOX_SIZE_X, (scSize.y * 3 / 5) + BOX_SIZE_Y, ((scSize.x / 2) + BOX_SIZE_X) + BOX_SIZE_X, ((scSize.y * 3 / 5) + BOX_SIZE_Y) + BOX_SIZE_Y, 0xff2222, true);

	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	//DrawString(1200, 200, "SelectScene", 0xffffff);

	//DrawString(600, 600, "STARTÉ{É^Éì or SpaceKeyÇ≈ëJà⁄", 0xffffff);

}
