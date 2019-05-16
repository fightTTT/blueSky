#include <stdlib.h>
#include <time.h>
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
		if (lpSceneMng.GetMode() == MODE_1PLAYER)
		{
			if (lpSceneMng.GetDecidFlag(PAD_1))
			{
				// NPCの使用ｷｬﾗを決定
				bool setEnemyIdFlag = false;
				do
				{
					lpSceneMng.SetCharID(PAD_2, (rand() % 8));
					if (lpSceneMng.GetCharID(PAD_1) != lpSceneMng.GetCharID(PAD_2))
					{
						setEnemyIdFlag = true;
					}
				} while (!setEnemyIdFlag);

				return std::make_unique<GameScene>();
			}
		}
		else if (lpSceneMng.GetMode() == MODE_2PLAYER)
		{
			if (lpSceneMng.GetDecidFlag(PAD_1) && lpSceneMng.GetDecidFlag(PAD_2))
			{
				return std::make_unique<GameScene>();
			}
		}
		else
		{
			// 何もしない
		}
	}

	SelectDraw();

	return std::move(own);
}

int SelectScene::Init()
{
	
	flamCnt = 0;
	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objListを全削除する

	// 乱数
	srand((unsigned int)time(NULL));

	AddObjList()(objList, std::make_unique<CharSelCursor>(PAD_1));		// ﾘｽﾄにCursorｸﾗｽを追加

	if (lpSceneMng.GetMode() == MODE_2PLAYER)
	{
		AddObjList()(objList, std::make_unique<CharSelCursor>(PAD_2));		// ﾘｽﾄにCursorｸﾗｽを追加
	}

	scSize = lpSceneMng.GetScreenSize();

	charNameTbl = { "シロ",
					"クロ",
					"アカ",
					"アオ",
					"ミドリ",
					"キイロ",
					"ムラサキ",
					"チャ" };

	//LoadDivGraph("charIcon", CHAR_CNT_MAX, CHAR_CNT_MAX, 1, 400, 400, charNameTbl[]);

	return 0;
}

void SelectScene::SelectDraw()
{
	DrawGraph(0, 0, IMAGE_ID("image/キャラセレ用/charSelBG.png")[0], true);		// haikei

	DrawGraph((scSize.x / 2) - (BOX_SIZE_X * 2), (scSize.y * 3 / 5), IMAGE_ID("image/キャラセレ用/charSel.png")[0], true);		// ｷｬﾗ一覧を描画
			
	// PL1のｷｬﾗ名
	const int id[2] = { lpSceneMng.GetCharID(PAD_1) , lpSceneMng.GetCharID(PAD_2) };
	DrawGraph(150, (scSize.y / 4), IMAGE_ID("image/キャラセレ用/1p.png")[0], true);		// ﾌﾟﾚｲﾔｰ番号(1P)の描画
	if (id[0] != -1)
	{
		DrawGraph(0, (scSize.y-400), IMAGE_DIV_ID("image/キャラセレ用/charIcon.png", VECTOR2(400, 400), VECTOR2(CHAR_CNT_MAX, 1))[id[0]], true);		// ｷｬﾗのｱｲｺﾝを描画
		DrawFormatString(10, (scSize.y / 2), 0xffffff, "%s", charNameTbl[id[0]]);
	}
	if (id[1] != -1)
	{
		DrawGraph((scSize.x-150), (scSize.y / 4), IMAGE_ID("image/キャラセレ用/2p.png")[0], true);		// ﾌﾟﾚｲﾔｰ番号(2P)の描画
		DrawTurnGraph((scSize.x - 400), (scSize.y - 400), IMAGE_DIV_ID("image/キャラセレ用/charIcon.png", VECTOR2(400, 400), VECTOR2(CHAR_CNT_MAX, 1))[id[1]], true);		// ｷｬﾗのｱｲｺﾝを描画
		DrawFormatString(1200, (scSize.y / 2), 0xffffff, "%s", charNameTbl[id[1]]);
	}

	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	//DrawString(1200, 200, "SelectScene", 0xffffff);

	if (lpSceneMng.GetMode() == MODE_1PLAYER)
	{
		if (lpSceneMng.GetDecidFlag(PAD_1))
		{
			flamCnt++;
			if (((flamCnt / 30) % 2) == 0)
			{
				DrawGraph((scSize.x / 2) - 419, (scSize.y / 10), IMAGE_ID("image/キャラセレ用/ready.png")[0], true);		// haikei
				//DrawString(1000, 10, "STARTボタン or SpaceKeyで遷移", 0xffffff);
			}
		}
	}
	else if (lpSceneMng.GetMode() == MODE_2PLAYER)
	{
		if (lpSceneMng.GetDecidFlag(PAD_1) && lpSceneMng.GetDecidFlag(PAD_2))
		{
			flamCnt++;
			if (((flamCnt / 30) % 2) == 0)
			{
				DrawGraph((scSize.x / 2) - 419, (scSize.y / 10), IMAGE_ID("image/キャラセレ用/ready.png")[0], true);		// haikei
				//DrawString(1000, 10, "STARTボタン or SpaceKeyで遷移", 0xffffff);
			}
		}
	}
	else
	{
		// 何もしない
	}
}
