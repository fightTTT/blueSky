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
#include "TitleScene.h"
#include "SoundMng.h"
#include "ExplanationScene.h"

#define BOX_SIZE_X (100)
#define BOX_SIZE_Y (100)

SelectScene::SelectScene()
{
	Init();
	lpSceneMng.SetDecidFlag(PAD_1, false);
	lpSceneMng.SetDecidFlag(PAD_2, false);
}

SelectScene::~SelectScene()
{
}

unique_Base SelectScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	if (!sceneChangeFlag)
	{
		for (auto& data : *objList)
		{
			data->UpDate(controller, objList);
		}
	}

	if (aiLevel != lpSceneMng.GetAILevel())
	{
		aiLevel = lpSceneMng.GetAILevel();
	}

	if (itvCnt > 0)
	{
		itvCnt++;
		if (!CheckSoundMem(SOUND_ID("se/ui/sceneChange.wav")))
		{
			PlaySoundMem(SOUND_ID("se/ui/sceneChange.wav"), DX_PLAYTYPE_BACK);
		}

		if (itvCnt > 120)
		{
			StopSoundMem(SOUND_ID("se/ui/sceneChange.wav"));
			StopSoundMem(SOUND_ID("bgm/select.mp3"));
			return std::make_unique<ExplanationScene>();
		}
	}


	/* ｹﾞｰﾑｼｰﾝへ移動 */
	if ((controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START)) && !sceneChangeFlag)
	{				
		// 音
		if (lpSceneMng.GetMode() == MODE_1PLAYER)
		{
			/*if (lpSceneMng.GetDecidFlag(PAD_1))
			{*/
				int setEnemyIdFlag = false;
				// NPCの使用ｷｬﾗを決定
				do
				{
					lpSceneMng.SetCharID(PAD_AI, (rand() % 8));
					if (lpSceneMng.GetCharID(PAD_1) != lpSceneMng.GetCharID(PAD_AI))
					{
						setEnemyIdFlag = true;
					}
				} while (!setEnemyIdFlag);
				itvCnt = 1;
				flamCnt = 0;
				sceneChangeFlag = true;


			//}
		}
		else if (lpSceneMng.GetMode() == MODE_2PLAYER)
		{
			if (lpSceneMng.GetDecidFlag(PAD_1) && lpSceneMng.GetDecidFlag(PAD_2))
			{
				itvCnt = 1;
				flamCnt = 0;
				sceneChangeFlag = true;
				/*return std::make_unique<GameScene>();*/
			}
		}
		else
		{
			// 何もしない
		}
	}

	/* ﾀｲﾄﾙｼｰﾝへ移動 */
	if (lpSceneMng.GetTitleChangeFlag())
	{
		return std::make_unique<TitleScene>();
	}

	SelectDraw();
	return std::move(own);
}

int SelectScene::Init()
{
	lpSceneMng.SetCharID(PAD_2, -1);		// idの初期化
	itvCnt = 0;
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

	sceneChangeFlag = false;

	PlaySoundMem(SOUND_ID("bgm/select.mp3"), DX_PLAYTYPE_LOOP);

	aiLevel = 1;
	lpSceneMng.SetAILevel(aiLevel);

	return 0;
}

void SelectScene::SelectDraw(void)
{
	DrawGraph(0, 0, IMAGE_ID("image/キャラセレ用/charSelBG.png")[0], true);		// haikei

	DrawGraph((scSize.x / 2) - (BOX_SIZE_X * 2), (scSize.y * 3 / 5), IMAGE_ID("image/キャラセレ用/charSel.png")[0], true);		// ｷｬﾗ一覧を描画
			
	/* PL1のｷｬﾗ名 */
	const int id[2] = { lpSceneMng.GetCharID(PAD_1) , lpSceneMng.GetCharID(PAD_2) };
	DrawGraph(100, (scSize.y / 3) + 25, IMAGE_ID("image/キャラセレ用/1p.png")[0], true);		// ﾌﾟﾚｲﾔｰ番号(1P)の描画
	if (id[0] != -1)
	{
		DrawGraph(0, (scSize.y-400), IMAGE_DIV_ID("image/キャラセレ用/charIcon.png", VECTOR2(400, 400), VECTOR2(CHAR_CNT_MAX, 1))[id[0]], true);		// ｷｬﾗのｱｲｺﾝを描画
		//DrawFormatString(10, (scSize.y / 2), 0xffffff, "%s", charNameTbl[id[0]]);
	}
	if (id[1] != -1)
	{
		DrawGraph((scSize.x-190), (scSize.y / 3) + 25, IMAGE_ID("image/キャラセレ用/2p.png")[0], true);		// ﾌﾟﾚｲﾔｰ番号(2P)の描画
		DrawTurnGraph((scSize.x - 400), (scSize.y - 400), IMAGE_DIV_ID("image/キャラセレ用/charIcon.png", VECTOR2(400, 400), VECTOR2(CHAR_CNT_MAX, 1))[id[1]], true);		// ｷｬﾗのｱｲｺﾝを描画
		//DrawFormatString(1200, (scSize.y / 2), 0xffffff, "%s", charNameTbl[id[1]]);
	}

	/* 確認ｳｨﾝﾄﾞｳ */
	if (lpSceneMng.GetSceneBackFlag())
	{
		DrawGraph((scSize.x / 2) - 400, (scSize.y / 5), IMAGE_ID("image/キャラセレ用/backWindow.png")[0], true);		// 確認ｳｨﾝﾄﾞｳ
	}

	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	if (lpSceneMng.GetSceneBackFlag())
	{
		DrawGraph((scSize.x / 2) - 300, (scSize.y / 2) + 50, IMAGE_ID("image/キャラセレ用/yes.png")[0], true);		// はい
		DrawGraph((scSize.x / 2) + 50, (scSize.y / 2) + 50, IMAGE_ID("image/キャラセレ用/no.png")[0], true);		// いいえ
	}

	if (lpSceneMng.GetMode() == MODE_1PLAYER)
	{
		if (lpSceneMng.GetDecidFlag(PAD_1) && !sceneChangeFlag)
		{
			// CPUレベル選択の画像を描画
			DrawGraph((scSize.x - 350), (scSize.y / 3) + 45, IMAGE_DIV_ID("image/キャラセレ用/CPUレベル選択.png", VECTOR2(340, 425), VECTOR2(3, 1))[(aiLevel - 1)], true);
		}

		if (lpSceneMng.GetDecidFlag(PAD_1))
		{
			DrawGraph((scSize.x / 2) - 419, (scSize.y / 3), IMAGE_ID("image/キャラセレ用/allGreen.png")[0], true);		// キャラ確定表示

			flamCnt++;
			if (sceneChangeFlag == false)
			{
				if (((flamCnt / 30) % 2) == 0)
				{
					DrawGraph((scSize.x / 2) - 419, (scSize.y / 10), IMAGE_ID("image/キャラセレ用/ready.png")[0], true);		// 準備完了の描画
				}
			}
			else
			{
				if (((flamCnt / 4) % 2) == 0)
				{
					DrawGraph((scSize.x / 2) - 419, (scSize.y / 10), IMAGE_ID("image/キャラセレ用/ready.png")[0], true);		// 準備完了の描画
				}
			}
		}
		if(!lpSceneMng.GetDecidFlag(PAD_1) && !lpSceneMng.GetSceneBackFlag())
		{
			DrawGraph(0, 75, IMAGE_DIV_ID("image/キャラセレ用/コマンド表/command.png", VECTOR2(630, 195), VECTOR2(CHAR_CNT_MAX, 1))[id[0]], true);		// ｺﾏﾝﾄﾞ表を描画
		}
	}
	else if (lpSceneMng.GetMode() == MODE_2PLAYER)
	{
		if (lpSceneMng.GetDecidFlag(PAD_1) && !lpSceneMng.GetSceneBackFlag())
		{
			DrawGraph((scSize.x / 2) - 419, (scSize.y / 3), IMAGE_ID("image/キャラセレ用/allGreen.png")[0], true);		// キャラ確定表示
		}

		if (!lpSceneMng.GetDecidFlag(PAD_1) && !lpSceneMng.GetSceneBackFlag())
		{
			DrawGraph(0, 75, IMAGE_DIV_ID("image/キャラセレ用/コマンド表/command.png", VECTOR2(630, 195), VECTOR2(CHAR_CNT_MAX, 1))[id[0]], true);		// ｺﾏﾝﾄﾞ表を描画
		}

		if (lpSceneMng.GetDecidFlag(PAD_2) && !lpSceneMng.GetSceneBackFlag())
		{
			DrawGraph((scSize.x / 2) + (BOX_SIZE_X * 2) + 75, (scSize.y / 3), IMAGE_ID("image/キャラセレ用/allGreen.png")[0], true);		// キャラ確定表示
		}

		if (!lpSceneMng.GetDecidFlag(PAD_2) && !lpSceneMng.GetSceneBackFlag())
		{
			DrawGraph((scSize.x - 630), 75, IMAGE_DIV_ID("image/キャラセレ用/コマンド表/command.png", VECTOR2(630, 195), VECTOR2(CHAR_CNT_MAX, 1))[id[1]], true);		// ｺﾏﾝﾄﾞ表を描画
		}

		if (lpSceneMng.GetDecidFlag(PAD_1) && lpSceneMng.GetDecidFlag(PAD_2))
		{
			flamCnt++;
			if (sceneChangeFlag == false)
			{
				if (((flamCnt / 30) % 2) == 0)
				{
					DrawGraph((scSize.x / 2) - 419, (scSize.y / 10), IMAGE_ID("image/キャラセレ用/ready.png")[0], true);		// 準備完了の描画
				}
			}
			else
			{
				if (((flamCnt / 4) % 2) == 0)
				{
					DrawGraph((scSize.x / 2) - 419, (scSize.y / 10), IMAGE_ID("image/キャラセレ用/ready.png")[0], true);		// 準備完了の描画
				}
			}
		}
	}
	else
	{
		// 何もしない
	}

}
