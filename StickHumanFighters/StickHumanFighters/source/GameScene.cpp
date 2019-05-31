#include <stdlib.h>
#include <time.h>
#include "Dxlib.h"
#include "ObjList.h"
#include "Obj.h"
#include "StickHuman.h"
#include "AIStickHuman.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "GameScene.h"
#include "GameCtrl.h"
#include "CollisionMng.h"
#include "ResultScene.h"
#include "HitEffect.h"
#include "SoundMng.h"

#define STICK_HUMAN_IMAGE_SIZE_X (256)
#define STICK_HUMAN_IMAGE_SIZE_Y (256)

#define BG_IMAGE_SIZE_X (1920)
#define BG_IMAGE_SIZE_Y (720)

#define DEF_BG_POS_X (-(BG_IMAGE_SIZE_X / 6))
#define DEF_BG_POS_Y (-64)

#define DEF_CENTER_POS_X (640)

#define PI2 (3.141592654f*6)	// ｶｰｿﾙの回転用
#define TOTAL_BACK_GROUND (2)

// ﾃﾞﾊﾞｯｸﾒｯｾｰｼﾞ用定義
#ifdef _DEBUG		// 失敗時の処理
#define AST() {\
	CHAR ast_mes[256];\
	wsprintf(ast_mes, "%s %d行目\0", __FILE__, __LINE__);\
	MessageBox(0, ast_mes, "ｱｻｰﾄ表示", MB_OK);\
	}
#else				// 成功時の処理
#define AST()
#endif		// _DEBUG

GameScene::GameScene()
{
	sprintf_s(stageImgName, "image/ゲームシーン用/backGround%d.png", GetRand(TOTAL_BACK_GROUND));
	Init();
}

GameScene::~GameScene()
{
}


unique_Base GameScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	if (operableFlag)
	{
		if (CheckGameEnd())
		{
			if (koDrawCount == 0)
			{
				WaitTimer(650);
				charaObj[0].charaObj->SetAnimStopFlag(true);
				charaObj[1].charaObj->SetAnimStopFlag(true);

				if (drawflag)
				{
					charaObj[0].winCount++;
					charaObj[1].winCount++;
				}
				else
				{
					charaObj[winCharacter].winCount++;
				}
			}

			koDrawCount++;

			if (koDrawCount >= 120)
			{
				if (gameEndFlag)
				{
					StopSoundMem(SOUND_ID("bgm/battle.mp3"));
					return std::make_unique<ResultScene>(charaObj[winCharacter].charaObj->GetPadID(),drawflag);
				}
				else
				{
					Init();
				}
			}
		}
		else
		{

			for (int i = 0; i < 2; i++)
			{
				beforPos[i] = charaObj[i].charaObj->GetPos();
			}

			// 情報更新
			for (auto& data : *objList)
			{
				data->UpDate(controller, objList);
			}

			// 同じキャラクターのショットが2つ以上あったら削除
			std::array<bool, PAD_MAX> shotFlag = { false, false, false };
			for (auto& data : *objList)
			{
				if (data->CheckObjType(OBJ_TYPE_SHOT))
				{
					if (shotFlag[data->GetPadID()] == true)
					{
						data->SetDeleteFlag(true);
					}
					else
					{
						shotFlag[data->GetPadID()] = true;
					}
				}
			}

			auto deth_itr = std::remove_if(objList->begin(), objList->end(), [](std::shared_ptr<Obj> obj) {return obj->CheckDeth(); });
			objList->erase(deth_itr, objList->end());

			for (int i = 0; i < 2; i++)
			{
				afterPos[i] = charaObj[i].charaObj->GetPos();
			}

			ExtrusionUpdata();

			std::vector<sharedObj> shotObj;
			EnemyState eState;

			for (auto& data : *objList)
			{
				if (data->CheckObjType(OBJ_TYPE_SHOT))
				{
					shotObj.push_back(data);
					ShotData shot(data->GetPos(), data->GetPadID());
					eState.pushBackShotData(shot);
				}
			}

			// キャラクター同士を向い合せる
			if (charaObj[0].charaObj->GetPos().x > charaObj[1].charaObj->GetPos().x)
			{
				charaObj[0].charaObj->SetTmpDir(DIR_LEFT);
				charaObj[1].charaObj->SetTmpDir(DIR_RIGHT);
			}
			else
			{
				charaObj[0].charaObj->SetTmpDir(DIR_RIGHT);
				charaObj[1].charaObj->SetTmpDir(DIR_LEFT);
			}

			// 一人が後ろ歩きでもう一人が攻撃系のアニメーションの時に、後ろ歩きをしている方をガード状態にする
			if ((charaObj[0].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SMALL)
				|| (charaObj[0].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_BIG)
				|| (charaObj[0].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SP))
			{
				if (abs(afterPos[0].x - afterPos[1].x) <= (STICK_HUMAN_IMAGE_SIZE_X * 2))
				{
					if (charaObj[1].charaObj->GetAnim() == "後ろ移動")
					{
						charaObj[1].charaObj->SetAnim("ガード_立ち");
					}

					if (charaObj[1].charaObj->GetAnim() == "しゃがみ_後ろ")
					{
						charaObj[1].charaObj->SetAnim("ガード_しゃがみ");
					}
				}
			}
			if ((charaObj[1].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SMALL)
				|| (charaObj[1].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_BIG)
				|| (charaObj[1].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SP))
			{
				if (abs(afterPos[0].x - afterPos[1].x) <= (STICK_HUMAN_IMAGE_SIZE_X * 2))
				{
					if (charaObj[0].charaObj->GetAnim() == "後ろ移動")
					{
						charaObj[0].charaObj->SetAnim("ガード_立ち");
					}

					if (charaObj[0].charaObj->GetAnim() == "しゃがみ_後ろ")
					{
						charaObj[0].charaObj->SetAnim("ガード_しゃがみ");
					}
				}
			}

			// キャラクターの状態を相手に渡す
			eState.enemyPos = charaObj[0].charaObj->GetPos();
			eState.enemyAnimAttribute[0] = charaObj[0].charaObj->GetAnimAttribute(0);
			eState.enemyAnimAttribute[1] = charaObj[0].charaObj->GetAnimAttribute(1);
			eState.enemyAnimAttribute[2] = charaObj[0].charaObj->GetAnimAttribute(2);
			eState.enemyAnim = charaObj[0].charaObj->GetAnim();
			charaObj[1].charaObj->SetEnemyState(eState);

			eState.enemyPos = charaObj[1].charaObj->GetPos();
			eState.enemyAnimAttribute[0] = charaObj[1].charaObj->GetAnimAttribute(0);
			eState.enemyAnimAttribute[1] = charaObj[1].charaObj->GetAnimAttribute(1);
			eState.enemyAnimAttribute[2] = charaObj[1].charaObj->GetAnimAttribute(2);
			eState.enemyAnim = charaObj[1].charaObj->GetAnim();
			charaObj[0].charaObj->SetEnemyState(eState);

			std::string animName[2];

			for (int i = 0; i < 2; i++)
			{
				animName[i] = charaObj[i].charaObj->GetAnim();

				charaObj[i].AttackHitOld = charaObj[i].charaObj->GetHitFlag();
				// 当たり判定をfalseにする
				charaObj[i].charaObj->SetHitData(false, COLTYPE_NON);
				damageFlag[i] = false;
			}

			// 当たり判定処理
			{
				colJudgment(shotObj,animName);
				// いらない部分,万が一のために残しておきます
				{
					//if (lpColMng.GetColFlag(charaObj[0].charaObj->GetAnim())
					//	&& lpColMng.GetColFlag(charaObj[1].charaObj->GetAnim()))
					//{
					//	// 当たり判定の情報を取得
					//	for (int i = 0; i < 2; i++)
					//	{
					//		id[i] = charaObj[i].charaObj->GetFrame();
					//		if (id[i] < charaObj[i].charaObj->GetAnimFrame(charaObj[i].charaObj->GetAnim()))
					//		{
					//			colData[i] = lpColMng.GetCollisionData("棒人間", charaObj[i].charaObj->GetAnim(), id[i]);
					//		}
					//		else
					//		{
					//			id[i] = 0;
					//		}
					//	}

					//	// 当たり判定のPOSの反転処理
					//	for (int i = 0; i < 2; i++)
					//	{
					//		for (int a = 0; a < colData[i].hitBox.size(); a++)
					//		{

					//			colData[i].hitBox[a].rect.startPos.x += charaObj[i].charaObj->GetAnimOffSet(animName[i]).x;
					//			colData[i].hitBox[a].rect.endPos.x += charaObj[i].charaObj->GetAnimOffSet(animName[i]).x;
					//			colData[i].hitBox[a].rect.startPos.y += charaObj[i].charaObj->GetAnimOffSet(animName[i]).y;
					//			colData[i].hitBox[a].rect.endPos.y += charaObj[i].charaObj->GetAnimOffSet(animName[i]).y;

					//			int oneTimePos;

					//			colData[i].hitBox[a].rect.startPos.x *= static_cast<int>(charaObj[i].charaObj->GetDir()) * -2 + 1;
					//			colData[i].hitBox[a].rect.endPos.x *= static_cast<int>(charaObj[i].charaObj->GetDir()) * -2 + 1;

					//			// startPosがendPosよりも大きかった場合、座標を交換する
					//			if (colData[i].hitBox[a].rect.startPos.x > colData[i].hitBox[a].rect.endPos.x)
					//			{
					//				oneTimePos = colData[i].hitBox[a].rect.endPos.x;
					//				colData[i].hitBox[a].rect.endPos.x = colData[i].hitBox[a].rect.startPos.x;
					//				colData[i].hitBox[a].rect.startPos.x = oneTimePos;
					//			}
					//			if (colData[i].hitBox[a].rect.startPos.y > colData[i].hitBox[a].rect.endPos.y)
					//			{
					//				oneTimePos = colData[i].hitBox[a].rect.endPos.y;
					//				colData[i].hitBox[a].rect.endPos.y = colData[i].hitBox[a].rect.startPos.y;
					//				colData[i].hitBox[a].rect.startPos.y = oneTimePos;
					//			}

					//			// 現在のプレイヤーのpos
					//			colData[i].hitBox[a].rect.startPos.x = (charaObj[i].charaObj->GetPos().x + colData[i].hitBox[a].rect.startPos.x) + (charaObj[i].charaObj->GetDivSize().x / 2);
					//			colData[i].hitBox[a].rect.endPos.x = (charaObj[i].charaObj->GetPos().x + colData[i].hitBox[a].rect.endPos.x) + (charaObj[i].charaObj->GetDivSize().x / 2);
					//			colData[i].hitBox[a].rect.startPos.y = (charaObj[i].charaObj->GetPos().y + colData[i].hitBox[a].rect.startPos.y) + (charaObj[i].charaObj->GetDivSize().y);
					//			colData[i].hitBox[a].rect.endPos.y = (charaObj[i].charaObj->GetPos().y + colData[i].hitBox[a].rect.endPos.y) + (charaObj[i].charaObj->GetDivSize().y);

					//		}
					//	}

					//	// ヒットエフェクト表示矩形
					//	VECTOR2 hitRectPos(0, 0);

					//	// 攻撃時の当たり判定
					//	for (int i = 0; i < 2; i++)
					//	{
					//		for (int a = 0; a < colData[i].hitBox.size(); a++)
					//		{
					//			for (int b = 0; b < colData[(i + 1) % 2].hitBox.size(); b++)
					//			{
					//				if (colData[i].hitBox[a].type == COLTYPE_ATTACK)
					//				{
					//					if (colData[(i + 1) % 2].hitBox[b].type != COLTYPE_ATTACK)
					//					{
					//						if (colData[i].hitBox[a].rect.endPos.x >= colData[(i + 1) % 2].hitBox[b].rect.startPos.x
					//							&& colData[i].hitBox[a].rect.startPos.x <= colData[(i + 1) % 2].hitBox[b].rect.endPos.x
					//							&& colData[i].hitBox[a].rect.endPos.y >= colData[(i + 1) % 2].hitBox[b].rect.startPos.y
					//							&& colData[i].hitBox[a].rect.startPos.y <= colData[(i + 1) % 2].hitBox[b].rect.endPos.y)
					//						{
					//							if (charaObj[i].charaObj->GetHitBoxType() != COLTYPE_HIT)
					//							{
					//								charaObj[i].charaObj->SetHitData(true, colData[i].hitBox[a].type);
					//							}
					//							if (charaObj[(i + 1) % 2].charaObj->GetHitBoxType() != COLTYPE_GUARD)
					//							{
					//								charaObj[(i + 1) % 2].charaObj->SetHitData(true, colData[(i + 1) % 2].hitBox[b].type);
					//								hitRectPos = { colData[(i + 1) % 2].hitBox[b].rect.startPos.x, colData[i].hitBox[a].rect.startPos.y };
					//							}						
					//						}
					//					}
					//				}
					//			}
					//		}
					//		if ((charaObj[(i + 1) % 2].charaObj->GetHitFlag()) && (charaObj[(i + 1) % 2].charaObj->GetHitBoxType() == COLTYPE_HIT) && !damageFlag[(i + 1) % 2])
					//		{
					//			charaObj[(i + 1) % 2].charaObj->CheckDamage(charaObj[i].charaObj->GetAnimAttribute(1));
					//			damageFlag[(i + 1) % 2] = true;
					//		}

					//		if (!(hitRectPos) && !(charaObj[(i + 1) % 2].AttackHitOld)
					//			&& (charaObj[(i + 1) % 2].charaObj->GetAnimAttribute(2) != ANIM_ATTRIBUTE_INVINCIBLE)
					//			&& (charaObj[(i + 1) % 2].charaObj->GetInvincibleTime() == 0))
					//		{
					//			bool colorChange = charaObj[(i + 1) % 2].charaObj->GetHitBoxType() == COLTYPE_GUARD;
					//			// ヒットエフェクト表示
					//			AddObjList()(objList, std::make_shared<HitEffect>(hitRectPos, VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), colorChange));
					//			hitRectPos = { 0,0 };
					//		}
					//	}

					//	for (int shot1 = 0; shot1 < shotObj.size(); shot1++)
					//	{
					//		if (shotObj[shot1]->GetHitFlag())
					//		{
					//			break;
					//		}
					//		for (int shot2 = 0; shot2 < shotObj2.size(); shot2++)
					//		{

					//			if (shotObj[shot2]->GetHitFlag())
					//			{
					//				break;
					//			}
					//			shotObj[shot1]->SetHitData(false, COLTYPE_NON);
					//			shotObj[shot2]->SetHitData(false, COLTYPE_NON);

					//			if (shotObj[shot1] != shotObj2[shot2])
					//			{
					//				if (shotObj[shot1]->GetPos().x + (shotObj[shot1]->GetDivSize().x / 2) + 50 >= shotObj[shot2]->GetPos().x + (shotObj[shot2]->GetDivSize().x / 2) - 50
					//					&& shotObj[shot1]->GetPos().x + (shotObj[shot1]->GetDivSize().x / 2) - 50 <= shotObj[shot2]->GetPos().x + (shotObj[shot2]->GetDivSize().x / 2) + 50)
					//				{
					//					shotObj[shot1]->SetHitData(true, COLTYPE_NON);
					//					shotObj2[shot2]->SetHitData(true, COLTYPE_NON);
					//					if (!CheckSoundMem(SOUND_ID("se/battle/kick.wav")))
					//					{
					//						PlaySoundMem(SOUND_ID("se/battle/kick.wav"), DX_PLAYTYPE_BACK);
					//					}
					//				}
					//			}
					//		}
					//	}

					//	hitRectPos = { 0,0 };

					//	for (int i = 0; i < 2; i++)
					//	{
					//		for (int a = 0; a < colData[i].hitBox.size(); a++)
					//		{
					//			// 波動拳の当たり判定
					//			for (auto& data : *objList)
					//			{
					//				if (data->CheckObjType(OBJ_TYPE_SHOT))
					//				{
					//					if (data->GetHitFlag())
					//					{
					//						break;
					//					}
					//					VECTOR2 startPos = { 0,0 };
					//					VECTOR2 endPos = { 0,0 };

					//					if (colData[i].hitBox[a].type != COLTYPE_ATTACK)
					//					{
					//						startPos = { data->GetPos().x + (data->GetDivSize().x / 2) - 50,data->GetPos().y + (data->GetDivSize().y / 2) - 50 };
					//						endPos = { data->GetPos().x + (data->GetDivSize().x / 2) + 50,data->GetPos().y + (data->GetDivSize().y / 2) + 50 };

					//						if (colData[i].hitBox[a].rect.endPos.x >= startPos.x
					//							&& colData[i].hitBox[a].rect.startPos.x <= endPos.x
					//							&& colData[i].hitBox[a].rect.endPos.y >= startPos.y
					//							&& colData[i].hitBox[a].rect.startPos.y <= endPos.y)
					//						{
					//							charaObj[i].charaObj->SetHitData(true, colData[i].hitBox[a].type);
					//							data->SetHitData(true, colData[i].hitBox[a].type);

					//							auto hitBoxRect = colData[i].hitBox[a].rect;

					//							hitRectPos = { hitBoxRect.startPos.x + ((hitBoxRect.endPos.x - hitBoxRect.startPos.x) / 4),
					//								data->GetPos().y + data->GetDivSize().y / 2 };

					//							break;
					//						}
					//					}
					//				}
					//			}
					//		}
					//		if ((charaObj[i].charaObj->GetHitFlag()) && (charaObj[i].charaObj->GetHitBoxType() == COLTYPE_HIT) && !damageFlag[i])
					//		{
					//			charaObj[i].charaObj->CheckDamage(ANIM_ATTRIBUTE_SHOT);
					//			damageFlag[i] = true;
					//		}

					//		if (!(hitRectPos) && !(charaObj[i].AttackHitOld)
					//			&& (charaObj[i].charaObj->GetAnimAttribute(2) != ANIM_ATTRIBUTE_INVINCIBLE)
					//			&& (charaObj[i].charaObj->GetInvincibleTime() == 0))
					//		{
					//			bool colorChange = charaObj[i].charaObj->GetHitBoxType() == COLTYPE_GUARD;

					//			// ヒットエフェクト表示
					//			AddObjList()(objList, std::make_shared<HitEffect>(hitRectPos, VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), colorChange));
					//			hitRectPos = { 0,0 };
					//		}
					//	}
					//}
				}
			}

			for (auto& data : *objList)
			{
				data->CheckHitFlag();
			}

			deth_itr = std::remove_if(objList->begin(), objList->end(), [](std::shared_ptr<Obj> obj) {return obj->CheckDeth(); });
			objList->erase(deth_itr, objList->end());

			// 背景の位置情報更新
			BgPosUpDate();

			for (auto& data : *objList)
			{
				if (data->CheckObjType(OBJ_TYPE_SHOT))
				{
					data->AddPos(bgPos - bgPosOld);
				}
			}
		}
	}
	else
	{
		opeCnt++;
	}

	/* 宣言効果音 */
	if ((charaObj[0].winCount + charaObj[1].winCount) == 0)
	{
		/* 開始宣言 */
		if (opeCnt < 75)
		{
			if (!CheckSoundMem(SOUND_ID("se/battle/round1.wav")))
			{
				PlaySoundMem(SOUND_ID("se/battle/round1.wav"), DX_PLAYTYPE_BACK);
			}
		}
	}
	if ((charaObj[0].winCount + charaObj[1].winCount) == 1)
	{
		if (opeCnt < 75)
		{
			if (!CheckSoundMem(SOUND_ID("se/battle/round2.wav")))
			{
				PlaySoundMem(SOUND_ID("se/battle/round2.wav"), DX_PLAYTYPE_BACK);
			}
		}
	}
	if ((charaObj[0].winCount + charaObj[1].winCount) == 2)
	{
		if (opeCnt < 75)
		{
			if (!CheckSoundMem(SOUND_ID("se/battle/final.wav")))
			{
				PlaySoundMem(SOUND_ID("se/battle/final.wav"), DX_PLAYTYPE_BACK);
			}
		}
	}
	if (!operableFlag &&  (opeCnt > 120 && opeCnt < 195))
	{
		if (!CheckSoundMem(SOUND_ID("se/battle/fight.wav")))
		{
			PlaySoundMem(SOUND_ID("se/battle/fight.wav"), DX_PLAYTYPE_BACK);
		}
	}

	if (koDrawCount)
	{
		finishCnt++;
		// ｼﾝｸﾞﾙﾌﾟﾚｲ時
		if (lpSceneMng.GetMode() == MODE_1PLAYER && winCharacter == 0)
		{
			if (!CheckSoundMem(SOUND_ID("se/battle/win.wav")) && finishCnt < 90)
			{
				PlaySoundMem(SOUND_ID("se/battle/win.wav"), DX_PLAYTYPE_BACK);
			}
		}
		if (lpSceneMng.GetMode() == MODE_1PLAYER && winCharacter != 0)
		{
			if (!CheckSoundMem(SOUND_ID("se/battle/lose.wav")) && finishCnt < 90)
			{
				PlaySoundMem(SOUND_ID("se/battle/lose.wav"), DX_PLAYTYPE_BACK);
			}
		}
		// ﾏﾙﾁﾌﾟﾚｲ時
		if (lpSceneMng.GetMode() == MODE_2PLAYER && winCharacter == 0)
		{
			if (!CheckSoundMem(SOUND_ID("se/battle/foo.mp3")) && finishCnt < 90)
			{
				PlaySoundMem(SOUND_ID("se/battle/foo.mp3"), DX_PLAYTYPE_BACK);
			}
		}
		if (lpSceneMng.GetMode() == MODE_2PLAYER && winCharacter != 0)
		{
			if (!CheckSoundMem(SOUND_ID("se/battle/foo.mp3")) && finishCnt < 90)
			{
				PlaySoundMem(SOUND_ID("se/battle/foo.mp3"), DX_PLAYTYPE_BACK);
			}
		}
	}




	if (opeCnt >= 240)
	{
		operableFlag = true;
	}

	maskCnt++;

	// 描画処理
	GameDraw();

	return std::move(own);
}

int GameScene::Init(void)
{
	MODE mode = lpSceneMng.GetMode();
	ssize = lpSceneMng.GetScreenSize();
	bgPos = VECTOR2(DEF_BG_POS_X, DEF_BG_POS_Y);
	hitStopFlag = false;
	gameEndFlag = false;
	operableFlag = false;
	loseCharacter = -1;
	winCharacter = -1;
	flashCnt = 0;
	opeCnt = 0;
	drawflag = false;
	koDrawCount = 0;
	finishCnt = 0;
	charaObj[0].AttackHitOld = false;
	charaObj[1].AttackHitOld = false;

	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objListを全削除する
	for (int a = 0; a < 2; a++)
	{
		id[a] = 0;
	}

	charaObj[0].charaObj = *AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2((ssize.x / 4), ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_1, DIR_RIGHT));
	charaObj[0].charaType = OBJ_TYPE_CHARACTER;

	if (mode == MODE_1PLAYER)
	{
		charaObj[1].charaObj = *AddObjList()(objList, std::make_unique<AIStickHuman>(VECTOR2(ssize.x * 3 / 4, ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), DIR_LEFT));
		charaObj[1].charaType = OBJ_TYPE_AICHARACTER;
	}
	else if (mode == MODE_2PLAYER)
	{
		charaObj[1].charaObj = *AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2(ssize.x * 3 / 4, ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_2, DIR_LEFT));
		charaObj[1].charaType = OBJ_TYPE_CHARACTER;
	}
	else
	{
		// MODE_MAXが来ている(エラー)
		AST();
	}

	
	// BGM
	if (!CheckSoundMem(SOUND_ID("bgm/battle.mp3")))
	{
		PlaySoundMem(SOUND_ID("bgm/battle.mp3"), DX_PLAYTYPE_LOOP);
	}

	maskCnt = 0;
	smallStarMask = LoadMask("image/ゲームシーン用/winStar_mask1.png");

	return 0;
}

void GameScene::BgPosUpDate(void)
{
	// ｷｬﾗｸﾀｰの位置情報格納
	VECTOR2 characterPos[2] = { charaObj[0].charaObj->GetPos(), charaObj[1].charaObj->GetPos() };

	// 1ﾌﾚｰﾑ前の背景の位置座標の格納
	bgPosOld = bgPos;

	// 横軸の処理
	bgPos.x += (DEF_CENTER_POS_X - ((characterPos[0].x + characterPos[1].x) / 2));
	if (bgPos.x > 0)
	{
		bgPos.x = 0;
	}
	if (bgPos.x < (ssize.x - BG_IMAGE_SIZE_X))
	{
		bgPos.x = (ssize.x - BG_IMAGE_SIZE_X);
	}

	int leftCharacter;		// 左にいる方のcharacterPosのindex
	int rightCharacter;		// 右にいる方のcharacterPosのindex

	if (characterPos[0].x < characterPos[1].x)
	{
		leftCharacter = 0;
		rightCharacter = 1;
	}
	else
	{
		leftCharacter = 1;
		rightCharacter = 0;
	}

	int characterDistanceHalf = ((characterPos[rightCharacter].x - characterPos[leftCharacter].x) / 2);

	if ((bgPos.x != 0) && (bgPos.x != (ssize.x - BG_IMAGE_SIZE_X)))
	{
		characterPos[leftCharacter].x = DEF_CENTER_POS_X - characterDistanceHalf;
		characterPos[rightCharacter].x = DEF_CENTER_POS_X + characterDistanceHalf;
	}

	

	// 縦軸の処理
	int highCharacter;		// 上にいる方のcharacterPosのindex
	int lowCharacter;		// 下にいる方のcharacterPosのindex
	if (characterPos[0].y < characterPos[1].y)
	{
		highCharacter = 0;
		lowCharacter = 1;
	}
	else
	{
		highCharacter = 1;
		lowCharacter = 0;
	}
	
	bgPos.y = (DEF_BG_POS_Y + ((ssize.y - characterPos[highCharacter].y) / 4));
	if (bgPos.y > 0)
	{
		bgPos.y = 0;
	}
	if (bgPos.y < DEF_BG_POS_Y)
	{
		bgPos.y = DEF_BG_POS_Y;
	}

	if (bgPosOld.y != bgPos.y)
	{
		characterPos[highCharacter].y += (bgPos.y - bgPosOld.y);
		characterPos[lowCharacter].y += (bgPos.y - bgPosOld.y);
	}

	// 範囲外の処理
	int edgePosLeft;		// 移動制限の左側
	int edgePosRight;		// 移動制限の右側

	edgePosLeft = (ssize.x / 5) + bgPos.x;
	if (edgePosLeft < (STICK_HUMAN_IMAGE_SIZE_X / 2))
	{
		edgePosLeft = (STICK_HUMAN_IMAGE_SIZE_X / 2);
	}
	if (edgePosLeft > (ssize.x / 5))
	{
		edgePosLeft = (ssize.x / 5);
	}

	edgePosRight = (((ssize.x * 4) / 5) + bgPos.x - (ssize.x - BG_IMAGE_SIZE_X));
	if (edgePosRight < ((ssize.x * 4) / 5))
	{
		edgePosRight = ((ssize.x * 4) / 5);
	}
	if (edgePosRight > (ssize.x - (STICK_HUMAN_IMAGE_SIZE_X / 2)))
	{
		edgePosRight = (ssize.x - (STICK_HUMAN_IMAGE_SIZE_X / 2));
	}

	int edgePosDown;		// 移動制限の下側

	edgePosDown = ssize.y + (bgPos.y - DEF_BG_POS_Y);
	if (edgePosDown < ssize.y)
	{
		edgePosDown = ssize.y;
	}
	if (edgePosDown > (ssize.y - DEF_BG_POS_Y))
	{
		edgePosDown = (ssize.y - DEF_BG_POS_Y);
	}

	bool rightCharacterExtrusionFlag = false;

	if (characterPos[leftCharacter].x < edgePosLeft)
	{
		if ((characterPos[leftCharacter].y > edgePosDown) || (charaObj[leftCharacter].charaObj->GetAnimAttribute(0) != ANIM_ATTRIBUTE_AIR))
		{
			charaObj[leftCharacter].charaObj->SetPos(VECTOR2(edgePosLeft, edgePosDown));
		}
		else
		{
			charaObj[leftCharacter].charaObj->SetPos(VECTOR2(edgePosLeft, characterPos[leftCharacter].y));
		}

		if (charaObj[leftCharacter].charaObj->GetHitBoxType() == COLTYPE_GUARD)
		{
			rightCharacterExtrusionFlag = true;
		}
	}
	else
	{
		if ((characterPos[leftCharacter].y > edgePosDown) || (charaObj[leftCharacter].charaObj->GetAnimAttribute(0) != ANIM_ATTRIBUTE_AIR))
		{
			charaObj[leftCharacter].charaObj->SetPos(VECTOR2(characterPos[leftCharacter].x, edgePosDown));
		}
		else
		{
			charaObj[leftCharacter].charaObj->SetPos(VECTOR2(characterPos[leftCharacter].x, characterPos[leftCharacter].y));
		}
	}

	if (characterPos[rightCharacter].x > edgePosRight)
	{
		if ((characterPos[rightCharacter].y > edgePosDown) || (charaObj[rightCharacter].charaObj->GetAnimAttribute(0) != ANIM_ATTRIBUTE_AIR))
		{
			charaObj[rightCharacter].charaObj->SetPos(VECTOR2(edgePosRight, edgePosDown));
		}
		else
		{
			charaObj[rightCharacter].charaObj->SetPos(VECTOR2(edgePosRight, characterPos[rightCharacter].y));
		}

		if (charaObj[rightCharacter].charaObj->GetHitBoxType() == COLTYPE_GUARD)
		{
			charaObj[leftCharacter].charaObj->AddPos(VECTOR2(-8, 0));
			WaitTimer(40);
		}
	}
	else
	{
		if ((characterPos[rightCharacter].y > edgePosDown) || (charaObj[rightCharacter].charaObj->GetAnimAttribute(0) != ANIM_ATTRIBUTE_AIR))
		{
			charaObj[rightCharacter].charaObj->SetPos(VECTOR2(characterPos[rightCharacter].x, edgePosDown));
		}
		else
		{
			charaObj[rightCharacter].charaObj->SetPos(VECTOR2(characterPos[rightCharacter].x, characterPos[rightCharacter].y));
		}

		if (rightCharacterExtrusionFlag)
		{
			charaObj[rightCharacter].charaObj->AddPos(VECTOR2(8, 0));
			WaitTimer(40);
		}
	}
}

void GameScene::ExtrusionUpdata()
{
	int leftCharacter;		// 左にいる方のcharacterPosのindex
	int rightCharacter;		// 右にいる方のcharacterPosのindex

	if (afterPos[0].x < afterPos[1].x)
	{
		leftCharacter = 0;
		rightCharacter = 1;
	}
	else
	{
		leftCharacter = 1;
		rightCharacter = 0;
	}

	if ((afterPos[leftCharacter].x + (STICK_HUMAN_IMAGE_SIZE_X / 8)) > (afterPos[rightCharacter].x - (STICK_HUMAN_IMAGE_SIZE_X / 8)))
	{
		int highCharacter;		// 上にいる方のcharacterPosのindex
		int lowCharacter;		// 下にいる方のcharacterPosのindex

		if (afterPos[0].y < afterPos[1].y)
		{
			highCharacter = 0;
			lowCharacter = 1;
		}
		else
		{
			highCharacter = 1;
			lowCharacter = 0;
		}

		if (afterPos[highCharacter].y > (afterPos[lowCharacter].y - (STICK_HUMAN_IMAGE_SIZE_Y * 3 / 5)))
		{
			// ここまで来たら押し出し
			int moveSum = (afterPos[leftCharacter].x - beforPos[leftCharacter].x) + (afterPos[rightCharacter].x - beforPos[rightCharacter].x);
			int addPosX = (afterPos[leftCharacter].x + (STICK_HUMAN_IMAGE_SIZE_X / 8)) - (afterPos[rightCharacter].x - (STICK_HUMAN_IMAGE_SIZE_X / 8));

			if (moveSum > 0)
			{
				charaObj[rightCharacter].charaObj->AddPos({ addPosX, 0 });

				if (charaObj[rightCharacter].charaObj->GetPos().x > (ssize.x * 4 / 5))
				{
					charaObj[rightCharacter].charaObj->SetPos({ (ssize.x * 4 / 5), afterPos[rightCharacter].y });
					charaObj[leftCharacter].charaObj->SetPos({ (ssize.x * 4 / 5) - (STICK_HUMAN_IMAGE_SIZE_X / 4), afterPos[leftCharacter].y });
				}
			}
			else if (moveSum < 0)
			{
				charaObj[leftCharacter].charaObj->AddPos({ -addPosX, 0 });

				if (charaObj[leftCharacter].charaObj->GetPos().x < (ssize.x / 5))
				{
					charaObj[leftCharacter].charaObj->SetPos({ (ssize.x / 5), afterPos[leftCharacter].y });
					charaObj[rightCharacter].charaObj->SetPos({ (ssize.x / 5) + (STICK_HUMAN_IMAGE_SIZE_X / 4), afterPos[rightCharacter].y });
				}
			}
			else
			{
				addPosX /= 2;

				charaObj[rightCharacter].charaObj->AddPos({ addPosX, 0 });
				charaObj[leftCharacter].charaObj->AddPos({ -addPosX, 0 });
			}
		}
	}
}

void GameScene::colJudgment(std::vector<sharedObj>& shotObj,std::string (&animName)[2])
{

	if (lpColMng.GetColFlag(charaObj[0].charaObj->GetAnim())
		&& lpColMng.GetColFlag(charaObj[1].charaObj->GetAnim()))
	{
		// 当たり判定の情報を取得
		for (int i = 0; i < 2; i++)
		{
			id[i] = charaObj[i].charaObj->GetFrame();
			if (id[i] < charaObj[i].charaObj->GetAnimFrame(charaObj[i].charaObj->GetAnim()))
			{
				colData[i] = lpColMng.GetCollisionData("棒人間", charaObj[i].charaObj->GetAnim(), id[i]);
			}
			else
			{
				id[i] = 0;
			}
		}

		// 当たり判定のPOSの反転処理
		for (int i = 0; i < 2; i++)
		{
			for (int a = 0; a < colData[i].hitBox.size(); a++)
			{

				colData[i].hitBox[a].rect.startPos.x += charaObj[i].charaObj->GetAnimOffSet(animName[i]).x;
				colData[i].hitBox[a].rect.endPos.x += charaObj[i].charaObj->GetAnimOffSet(animName[i]).x;
				colData[i].hitBox[a].rect.startPos.y += charaObj[i].charaObj->GetAnimOffSet(animName[i]).y;
				colData[i].hitBox[a].rect.endPos.y += charaObj[i].charaObj->GetAnimOffSet(animName[i]).y;

				int oneTimePos;

				colData[i].hitBox[a].rect.startPos.x *= static_cast<int>(charaObj[i].charaObj->GetDir()) * -2 + 1;
				colData[i].hitBox[a].rect.endPos.x *= static_cast<int>(charaObj[i].charaObj->GetDir()) * -2 + 1;

				// startPosがendPosよりも大きかった場合、座標を交換する
				if (colData[i].hitBox[a].rect.startPos.x > colData[i].hitBox[a].rect.endPos.x)
				{
					oneTimePos = colData[i].hitBox[a].rect.endPos.x;
					colData[i].hitBox[a].rect.endPos.x = colData[i].hitBox[a].rect.startPos.x;
					colData[i].hitBox[a].rect.startPos.x = oneTimePos;
				}
				if (colData[i].hitBox[a].rect.startPos.y > colData[i].hitBox[a].rect.endPos.y)
				{
					oneTimePos = colData[i].hitBox[a].rect.endPos.y;
					colData[i].hitBox[a].rect.endPos.y = colData[i].hitBox[a].rect.startPos.y;
					colData[i].hitBox[a].rect.startPos.y = oneTimePos;
				}

				// 現在のプレイヤーのpos
				colData[i].hitBox[a].rect.startPos.x = (charaObj[i].charaObj->GetPos().x + colData[i].hitBox[a].rect.startPos.x) + (charaObj[i].charaObj->GetDivSize().x / 2);
				colData[i].hitBox[a].rect.endPos.x = (charaObj[i].charaObj->GetPos().x + colData[i].hitBox[a].rect.endPos.x) + (charaObj[i].charaObj->GetDivSize().x / 2);
				colData[i].hitBox[a].rect.startPos.y = (charaObj[i].charaObj->GetPos().y + colData[i].hitBox[a].rect.startPos.y) + (charaObj[i].charaObj->GetDivSize().y);
				colData[i].hitBox[a].rect.endPos.y = (charaObj[i].charaObj->GetPos().y + colData[i].hitBox[a].rect.endPos.y) + (charaObj[i].charaObj->GetDivSize().y);

			}
		}

		// ヒットエフェクト表示矩形
		VECTOR2 hitRectPos(0, 0);

		// 攻撃時の当たり判定
		for (int i = 0; i < 2; i++)
		{
			for (int a = 0; a < colData[i].hitBox.size(); a++)
			{
				for (int b = 0; b < colData[(i + 1) % 2].hitBox.size(); b++)
				{
					if (colData[i].hitBox[a].type == COLTYPE_ATTACK)
					{
						if (colData[(i + 1) % 2].hitBox[b].type != COLTYPE_ATTACK)
						{
							if (colData[i].hitBox[a].rect.endPos.x >= colData[(i + 1) % 2].hitBox[b].rect.startPos.x
								&& colData[i].hitBox[a].rect.startPos.x <= colData[(i + 1) % 2].hitBox[b].rect.endPos.x
								&& colData[i].hitBox[a].rect.endPos.y >= colData[(i + 1) % 2].hitBox[b].rect.startPos.y
								&& colData[i].hitBox[a].rect.startPos.y <= colData[(i + 1) % 2].hitBox[b].rect.endPos.y)
							{
								if (charaObj[i].charaObj->GetHitBoxType() != COLTYPE_HIT)
								{
									charaObj[i].charaObj->SetHitData(true, colData[i].hitBox[a].type);
								}
								if (charaObj[(i + 1) % 2].charaObj->GetHitBoxType() != COLTYPE_GUARD)
								{
									charaObj[(i + 1) % 2].charaObj->SetHitData(true, colData[(i + 1) % 2].hitBox[b].type);
									hitRectPos = { colData[(i + 1) % 2].hitBox[b].rect.startPos.x, colData[i].hitBox[a].rect.startPos.y };
								}
							}
						}
					}
				}
			}
			if ((charaObj[(i + 1) % 2].charaObj->GetHitFlag()) && (charaObj[(i + 1) % 2].charaObj->GetHitBoxType() == COLTYPE_HIT) && !damageFlag[(i + 1) % 2])
			{
				charaObj[(i + 1) % 2].charaObj->CheckDamage(charaObj[i].charaObj->GetAnimAttribute(1));
				damageFlag[(i + 1) % 2] = true;
			}

			if (!(hitRectPos) && !(charaObj[(i + 1) % 2].AttackHitOld)
				&& (charaObj[(i + 1) % 2].charaObj->GetAnimAttribute(2) != ANIM_ATTRIBUTE_INVINCIBLE)
				&& (charaObj[(i + 1) % 2].charaObj->GetInvincibleTime() == 0))
			{
				bool colorChange = charaObj[(i + 1) % 2].charaObj->GetHitBoxType() == COLTYPE_GUARD;
				// ヒットエフェクト表示
				AddObjList()(objList, std::make_shared<HitEffect>(hitRectPos, VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), colorChange));
				hitRectPos = { 0,0 };
			}
		}

		for (auto &shot1 : shotObj)
		{
			if (shot1->GetHitFlag())
			{
				break;
			}
			for (auto &shot2 : shotObj)
			{

				if (shot2->GetHitFlag())
				{
					break;
				}
				shot1->SetHitData(false, COLTYPE_NON);
				shot2->SetHitData(false, COLTYPE_NON);

				if (shot1 != shot2)
				{
					if (shot1->GetPos().x + (shot1->GetDivSize().x / 2) + 50 >= shot2->GetPos().x + (shot2->GetDivSize().x / 2) - 50
						&& shot1->GetPos().x + (shot1->GetDivSize().x / 2) - 50 <= shot2->GetPos().x + (shot2->GetDivSize().x / 2) + 50)
					{
						shot1->SetHitData(true, COLTYPE_NON);
						shot2->SetHitData(true, COLTYPE_NON);
						if (!CheckSoundMem(SOUND_ID("se/battle/kick.wav")))
						{
							PlaySoundMem(SOUND_ID("se/battle/kick.wav"), DX_PLAYTYPE_BACK);
						}
					}
				}
			}
		}

		hitRectPos = { 0,0 };

		for (int i = 0; i < 2; i++)
		{
			for (int a = 0; a < colData[i].hitBox.size(); a++)
			{
				// 波動拳の当たり判定
				for (auto& data : *objList)
				{
					if (data->CheckObjType(OBJ_TYPE_SHOT))
					{
						if (data->GetHitFlag())
						{
							break;
						}
						VECTOR2 startPos = { 0,0 };
						VECTOR2 endPos = { 0,0 };

						if (colData[i].hitBox[a].type != COLTYPE_ATTACK)
						{
							startPos = { data->GetPos().x + (data->GetDivSize().x / 2) - 50,data->GetPos().y + (data->GetDivSize().y / 2) - 50 };
							endPos = { data->GetPos().x + (data->GetDivSize().x / 2) + 50,data->GetPos().y + (data->GetDivSize().y / 2) + 50 };

							if (colData[i].hitBox[a].rect.endPos.x >= startPos.x
								&& colData[i].hitBox[a].rect.startPos.x <= endPos.x
								&& colData[i].hitBox[a].rect.endPos.y >= startPos.y
								&& colData[i].hitBox[a].rect.startPos.y <= endPos.y)
							{
								charaObj[i].charaObj->SetHitData(true, colData[i].hitBox[a].type);
								data->SetHitData(true, colData[i].hitBox[a].type);

								auto hitBoxRect = colData[i].hitBox[a].rect;

								hitRectPos = { hitBoxRect.startPos.x + ((hitBoxRect.endPos.x - hitBoxRect.startPos.x) / 4),
									data->GetPos().y + data->GetDivSize().y / 2 };

								break;
							}
						}
					}
				}
			}
			if ((charaObj[i].charaObj->GetHitFlag()) && (charaObj[i].charaObj->GetHitBoxType() == COLTYPE_HIT) && !damageFlag[i])
			{
				charaObj[i].charaObj->CheckDamage(ANIM_ATTRIBUTE_SHOT);
				damageFlag[i] = true;
			}

			if (!(hitRectPos) && !(charaObj[i].AttackHitOld)
				&& (charaObj[i].charaObj->GetAnimAttribute(2) != ANIM_ATTRIBUTE_INVINCIBLE)
				&& (charaObj[i].charaObj->GetInvincibleTime() == 0))
			{
				bool colorChange = charaObj[i].charaObj->GetHitBoxType() == COLTYPE_GUARD;

				// ヒットエフェクト表示
				AddObjList()(objList, std::make_shared<HitEffect>(hitRectPos, VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), colorChange));
				hitRectPos = { 0,0 };
			}
		}
	}
}

bool GameScene::CheckGameEnd()
{
	if (charaObj[0].winCount >= 2 || charaObj[1].winCount >= 2)
	{
		gameEndFlag = true;
		return true;
	}
	else
	{
		if (hitStopFlag)
		{
			if (drawflag)
			{
				if (charaObj[0].charaObj->GetAnim() == "ダメージ_ダウン" && charaObj[0].charaObj->GetAnimEndFlag())
				{
					return true;
				}
			}
			else
			{
				if (charaObj[loseCharacter].charaObj->GetAnim() == "ダメージ_ダウン" && charaObj[loseCharacter].charaObj->GetAnimEndFlag())
				{
					return true;
				}
			}
			
			WaitTimer(50);
		}
		else
		{
			for (int i = 0; i < 2; ++i)
			{
				if (charaObj[i].charaObj->GetPlayerHP() == 0)
				{
					if (winCharacter == i)
					{
						// 引き分け
						drawflag = true;
					}
					else
					{
						loseCharacter = i;
						winCharacter = (1 - i);
					}

					hitStopFlag = true;
				}
			}
		}
	}

	return false;
}

bool GameScene::GameDraw(void)
{
	DrawGraph(bgPos.x, bgPos.y, IMAGE_ID(stageImgName)[0], true);

	DrawGraph((ssize.x/2)-175, 0, IMAGE_ID("image/ゲームシーン用/centerBer.png")[0], true);

	//objListに登録されているｸﾗｽの描画処理を行う
	for (auto &data : (*objList))
	{
		(*data).Draw();
	}
	
	//std::string animName[2];

	//for (int charNum = 0; charNum < 2; charNum++)
	//{
	//	animName[charNum] = charaObj[charNum].charaObj->GetAnim();
	//	if (lpColMng.GetColFlag(charaObj[charNum].charaObj->GetAnim()))
	//	{
	//		int colColor;
	//		for (int i = 0; i < colData[charNum].hitBox.size(); i++)
	//		{

	//			colColor = (colData[charNum].hitBox[i].type == COLTYPE_ATTACK ? 0xff0000 : (colData[charNum].hitBox[i].type == COLTYPE_HIT ? 0x0000ff : 0x00ff00));

	//			DrawBox(charaObj[charNum].charaObj->GetDrawOffSet().x + colData[charNum].hitBox[i].rect.startPos.x, charaObj[charNum].charaObj->GetDrawOffSet().y + colData[charNum].hitBox[i].rect.startPos.y,
	//					charaObj[charNum].charaObj->GetDrawOffSet().x + colData[charNum].hitBox[i].rect.endPos.x, charaObj[charNum].charaObj->GetDrawOffSet().y + colData[charNum].hitBox[i].rect.endPos.y, colColor, false);
	//		}
	//	}
	//}

	// KOの文字の描画
	if (koDrawCount)
	{
		// ｼﾝｸﾞﾙﾌﾟﾚｲ時
		if (lpSceneMng.GetMode() == MODE_1PLAYER && winCharacter == 0)
		{
			DrawGraph((ssize.x / 2) - 100, 200, IMAGE_DIV_ID("image/ゲームシーン用/singleResult.png", VECTOR2(200, 65), VECTOR2(PLAYER_CNT_MAX, 1))[0], true);		// 勝利画像を描画
		}
		if (lpSceneMng.GetMode() == MODE_1PLAYER && winCharacter != 0)
		{
			DrawGraph((ssize.x / 2) - 100, 200, IMAGE_DIV_ID("image/ゲームシーン用/singleResult.png", VECTOR2(200, 65), VECTOR2(PLAYER_CNT_MAX, 1))[1], true);		// 勝利画像を描画
		}
		// ﾏﾙﾁﾌﾟﾚｲ時
		if (lpSceneMng.GetMode() == MODE_2PLAYER && winCharacter == 0)
		{
			DrawGraph((ssize.x / 2) - 100, 200, IMAGE_DIV_ID("image/ゲームシーン用/multiResult.png", VECTOR2(200, 65), VECTOR2(PLAYER_CNT_MAX, 1))[0], true);		// 勝利画像を描画
		}
		if (lpSceneMng.GetMode() == MODE_2PLAYER && winCharacter != 0)
		{
			DrawGraph((ssize.x / 2) - 100, 200, IMAGE_DIV_ID("image/ゲームシーン用/multiResult.png", VECTOR2(200, 65), VECTOR2(PLAYER_CNT_MAX, 1))[1], true);		// 勝利画像を描画
		}

	}

	/*DrawFormatString(0, 300, 0xff0000, "chara 0 : winCount %d\nchara 1 : winCount %d\n", charaObj[0].winCount, charaObj[1].winCount);*/
	flashCnt++;
	if (charaObj[0].winCount == 1)
	{
		CreateMaskScreen();     // ﾏｽｸ開始
		DrawMask( ((ssize.x / 2) - 175 + 70 - 15), (69 - 15), smallStarMask, DX_MASKTRANS_BLACK); // 黒色の場所だけ描画
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);               // 加算ブレンドに設定
		DrawRotaGraph2( ((ssize.x / 2) - 175 + 70 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * maskCnt, IMAGE_ID("image/ゲームシーン用/starBack1.png")[0], true);			
		DrawRotaGraph2( ((ssize.x / 2) - 175 + 70 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 120), IMAGE_ID("image/ゲームシーン用/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 70 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 60), IMAGE_ID("image/ゲームシーン用/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 70 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 180), IMAGE_ID("image/ゲームシーン用/starBack1.png")[0], true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);     // ﾌﾞﾚﾝﾄﾞをﾘｾｯﾄ
		DeleteMaskScreen();		// ﾏｽｸ終了

		DrawGraph((ssize.x / 2) - 175 +70, 69, IMAGE_ID("image/ゲームシーン用/winStar1.png")[0], true);
	}
	if (charaObj[0].winCount == 2)
	{
		if (((flashCnt / 4) % 2) == 0)
		{
			DrawGraph(((ssize.x / 2) - 175 + 70 - 15), (69 - 15), IMAGE_ID("image/ゲームシーン用/winStar_flash1.png")[0], true);		// 強調点滅の描画
			DrawGraph(((ssize.x / 2) - 175 + 20 - 20), (10 - 20), IMAGE_ID("image/ゲームシーン用/winStar_flash2.png")[0], true);
		}
		DrawGraph((ssize.x / 2) - 175 +70, 69, IMAGE_ID("image/ゲームシーン用/winStar1.png")[0], true);
		DrawGraph((ssize.x / 2) - 175 +20, 10, IMAGE_ID("image/ゲームシーン用/winStar2.png")[0], true);
	}

	if (charaObj[1].winCount == 1)
	{
		CreateMaskScreen();     // ﾏｽｸ開始
		DrawMask(((ssize.x / 2) - 175 + 232 - 15), (69 - 15), smallStarMask, DX_MASKTRANS_BLACK); // 黒色の場所だけ描画
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);               // 加算ブレンドに設定
		DrawRotaGraph2(((ssize.x / 2) - 175 + 232 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * maskCnt, IMAGE_ID("image/ゲームシーン用/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 232 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 120), IMAGE_ID("image/ゲームシーン用/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 232 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 60), IMAGE_ID("image/ゲームシーン用/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 232 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 180), IMAGE_ID("image/ゲームシーン用/starBack1.png")[0], true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);     // ﾌﾞﾚﾝﾄﾞをﾘｾｯﾄ
		DeleteMaskScreen();		// ﾏｽｸ終了

		DrawGraph((ssize.x / 2) - 175 + 232, 69, IMAGE_ID("image/ゲームシーン用/winStar1.png")[0], true);
	}
	if (charaObj[1].winCount == 2)
	{
		if (((flashCnt / 4) % 2) == 0)
		{
			DrawGraph(((ssize.x / 2) - 175 + 232 - 15), (69 - 15), IMAGE_ID("image/ゲームシーン用/winStar_flash1.png")[0], true);		// 強調点滅の描画
			DrawGraph(((ssize.x / 2) - 175 + 262 - 20), (10 - 20), IMAGE_ID("image/ゲームシーン用/winStar_flash2.png")[0], true);
		}
		DrawGraph((ssize.x / 2) - 175 + 232, 69, IMAGE_ID("image/ゲームシーン用/winStar1.png")[0], true);
		DrawGraph((ssize.x / 2) - 175 + 262, 10, IMAGE_ID("image/ゲームシーン用/winStar2.png")[0], true);
	}

	/* 開始宣言 */
	if ((charaObj[0].winCount + charaObj[1].winCount) == 0)
	{
		if (opeCnt < 120)
		{
			DrawGraph((ssize.x / 2) - 125, 200, IMAGE_ID("image/ゲームシーン用/round1.png")[0], true);
		}
	}

	if ((charaObj[0].winCount + charaObj[1].winCount) == 1)
	{
		if (opeCnt < 120)
		{
			DrawGraph((ssize.x / 2) - 125, 200, IMAGE_ID("image/ゲームシーン用/round2.png")[0], true);
		}
	}

	if ((charaObj[0].winCount + charaObj[1].winCount) == 2)
	{
		if (opeCnt < 120)
		{
			DrawGraph((ssize.x / 2) - 125, 200, IMAGE_ID("image/ゲームシーン用/round3.png")[0], true);
		}
	}

	if (!operableFlag && opeCnt > 120)
	{
		DrawGraph((ssize.x / 2) - 125, 200, IMAGE_ID("image/ゲームシーン用/fight.png")[0], true);
	}





	return true;
}
