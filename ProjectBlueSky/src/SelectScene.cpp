#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "GameCtrl.h"
#include "VECTOR2.h"
#include "DxLib.h"
#include "CharSelCursor.h"
#include "ImageMng.h"

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

	/* ｹﾞｰﾑｼｰﾝへ移動 */
	if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
	{
		return std::make_unique<GameScene>();
	}

	SelectDraw();

	return std::move(own);
}

int SelectScene::Init()
{
	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objListを全削除する

	AddObjList()(objList, std::make_unique<CharSelCursor>(PAD_1));		// ﾘｽﾄにCursorｸﾗｽを追加
	AddObjList()(objList, std::make_unique<CharSelCursor>(PAD_2));		// ﾘｽﾄにCursorｸﾗｽを追加

	scSize = lpSceneMng.GetScreenSize();

	charNameTbl = { "シロ",
					"クロ",
					"アカ",
					"アオ",
					"ミドリ",
					"キイロ",
					"ムラサキ",
					"チャ" };

	return 0;
}

void SelectScene::SelectDraw()
{
	DrawGraph((scSize.x / 2) - (BOX_SIZE_X * 2), (scSize.y * 3 / 5), IMAGE_ID("image/charSel.png")[0], true);		// ｷｬﾗ一覧を描画
			
	// PL1のｷｬﾗ名
	const int id[2] = { lpSceneMng.GetCharID(PAD_1) , lpSceneMng.GetCharID(PAD_2) };
	if (id[0] != -1)
	{
		DrawFormatString(10, (scSize.y / 2), 0xffffff, "%s", charNameTbl[id[0]]);
	}
	if (id[1] != -1)
	{
		DrawFormatString(1200, (scSize.y / 2), 0xffffff, "%s", charNameTbl[id[1]]);
	}

	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	//DrawString(1200, 200, "SelectScene", 0xffffff);

	DrawString(1000, 10, "STARTボタン or SpaceKeyで遷移", 0xffffff);

}
