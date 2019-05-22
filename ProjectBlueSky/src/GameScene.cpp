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

#define STICK_HUMAN_IMAGE_SIZE_X (256)
#define STICK_HUMAN_IMAGE_SIZE_Y (256)

#define BG_IMAGE_SIZE_X (1920)
#define BG_IMAGE_SIZE_Y (720)

#define DEF_BG_POS_X (-(BG_IMAGE_SIZE_X / 6))
#define DEF_BG_POS_Y (-64)

#define DEF_CENTER_POS_X (640)

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
	Init();
}

GameScene::~GameScene()
{
}


unique_Base GameScene::UpDate(unique_Base own, const GameCtrl & controller)
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
				return std::make_unique<ResultScene>();
			}
			else
			{
				Init();
			}
		}
	}
	else
	{
		// 情報更新
		for (auto& data : *objList)
		{
			data->UpDate(controller, objList);
		}

		auto deth_itr = std::remove_if(objList->begin(), objList->end(), [](std::shared_ptr<Obj> obj) {return obj->CheckDeth(); });
		objList->erase(deth_itr, objList->end());

		// shotの情報をセット
		sharedObj shotObj;
		EnemyState eState;

		for (auto& data : *objList)
		{
			if (data->CheckObjType(OBJ_TYPE_SHOT))
			{
				shotObj = data;
				ShotData shot(data->GetPos(), data->GetPadID());
				eState.pushBackShotData(shot);
			}
		}

		// キャラクター同士を向い合せる
		if (charaObj[0].charaObj->GetPos().x > charaObj[1].charaObj->GetPos().x)
		{
			charaObj[0].charaObj->SetDir(DIR_LEFT);
			charaObj[1].charaObj->SetDir(DIR_RIGHT);
		}
		else
		{
			charaObj[0].charaObj->SetDir(DIR_RIGHT);
			charaObj[1].charaObj->SetDir(DIR_LEFT);
		}

		// 一人が後ろ歩きでもう一人が攻撃系のアニメーションの時に、後ろ歩きをしている方をガード状態にする
		if ((charaObj[0].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SMALL)
		 || (charaObj[0].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_BIG)
		 || (charaObj[0].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SP))
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
		if ((charaObj[1].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SMALL)
		 || (charaObj[1].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_BIG)
		 || (charaObj[1].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SP))
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

		// キャラクターの状態を相手に渡す
		if (!(charaObj[0].charaType == charaObj[1].charaType))
		{
			if (charaObj[0].charaType == OBJ_TYPE_CHARACTER)
			{
				eState.enemyPos = charaObj[0].charaObj->GetPos();
				eState.enemyAnimAttribute[0] = charaObj[0].charaObj->GetAnimAttribute(0);
				eState.enemyAnimAttribute[1] = charaObj[0].charaObj->GetAnimAttribute(1);
				eState.enemyAnimAttribute[2] = charaObj[0].charaObj->GetAnimAttribute(2);
				charaObj[1].charaObj->SetEnemyState(eState);
			}
			else
			{
				eState.enemyPos = charaObj[1].charaObj->GetPos();
				eState.enemyAnimAttribute[0] = charaObj[1].charaObj->GetAnimAttribute(0);
				eState.enemyAnimAttribute[1] = charaObj[1].charaObj->GetAnimAttribute(1);
				eState.enemyAnimAttribute[2] = charaObj[1].charaObj->GetAnimAttribute(2);
				charaObj[0].charaObj->SetEnemyState(eState);
			}
		}

		std::string animName[2];

		for (int i = 0; i < 2; i++)
		{
			animName[i] = charaObj[i].charaObj->GetAnim();

			// 当たり判定をfalseにする
			charaObj[i].charaObj->SetHitData(false, COLTYPE_NON);
		}

		// 当たり判定処理
		{
			ColInfo colData[2];			// 当たり判定の情報を格納する変数(2キャラ分)

			if (lpColMng.GetColFlag(charaObj[0].charaObj->GetAnim())
				&& lpColMng.GetColFlag(charaObj[1].charaObj->GetAnim()))
			{
				// 当たり判定の情報を取得
				for (int i = 0; i < 2; i++)
				{
					id[i] = charaObj[i].charaObj->GetCount(charaObj[i].charaObj->GetAnim());
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

						int b;

						colData[i].hitBox[a].rect.startPos.x *= static_cast<int>(charaObj[i].charaObj->GetDir()) * -2 + 1;
						colData[i].hitBox[a].rect.endPos.x *= static_cast<int>(charaObj[i].charaObj->GetDir()) * -2 + 1;

						// startPosがendPosよりも大きかった場合、座標を交換する
						if (colData[i].hitBox[a].rect.startPos.x > colData[i].hitBox[a].rect.endPos.x)
						{
							b = colData[i].hitBox[a].rect.endPos.x;
							colData[i].hitBox[a].rect.endPos.x = colData[i].hitBox[a].rect.startPos.x;
							colData[i].hitBox[a].rect.startPos.x = b;
						}
						if (colData[i].hitBox[a].rect.startPos.y > colData[i].hitBox[a].rect.endPos.y)
						{
							b = colData[i].hitBox[a].rect.endPos.y;
							colData[i].hitBox[a].rect.endPos.y = colData[i].hitBox[a].rect.startPos.y;
							colData[i].hitBox[a].rect.startPos.y = b;
						}

						// 現在のプレイヤーのpos
						colData[i].hitBox[a].rect.startPos.x = (charaObj[i].charaObj->GetPos().x + colData[i].hitBox[a].rect.startPos.x) + (charaObj[i].charaObj->GetDivSize().x / 2);
						colData[i].hitBox[a].rect.endPos.x = (charaObj[i].charaObj->GetPos().x + colData[i].hitBox[a].rect.endPos.x) + (charaObj[i].charaObj->GetDivSize().x / 2);
						colData[i].hitBox[a].rect.startPos.y = (charaObj[i].charaObj->GetPos().y + colData[i].hitBox[a].rect.startPos.y) + (charaObj[i].charaObj->GetDivSize().y);
						colData[i].hitBox[a].rect.endPos.y = (charaObj[i].charaObj->GetPos().y + colData[i].hitBox[a].rect.endPos.y) + (charaObj[i].charaObj->GetDivSize().y);

					}
				}

				// 攻撃時の当たり判定
				for (int i = 0; i < 2; i++)
				{
					for (int a = 0; a < colData[i].hitBox.size(); a++)
					{
						if (a >= 1)
						{
							if (colData[i].hitBox[a - 1].type == COLTYPE_ATTACK)
							{
								break;
							}
						}

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
										charaObj[(i + 1) % 2].charaObj->SetHitData(true, colData[(i + 1) % 2].hitBox[b].type);

										if (colData[(i + 1) % 2].hitBox[b].type == COLTYPE_GUARD)
										{
											break;
										}
										break;
									}
									else
									{
										charaObj[(i + 1) % 2].charaObj->SetHitData(false, colData[(i + 1) % 2].hitBox[b].type);
									}
								}
							}
							else
							{
								charaObj[(i + 1) % 2].charaObj->SetHitData(false, colData[(i + 1) % 2].hitBox[b].type);
							}
						}
					}
				}
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
										break;
									}
									else
									{
										charaObj[i].charaObj->SetHitData(false, colData[i].hitBox[a].type);
									}
								}
							}
						}
					}
				}

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
	loseCharacter = -1;
	winCharacter = -1;
	drawflag = false;
	koDrawCount = 0;

	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objListを全削除する
	for (int a = 0; a < 2; a++)
	{
		id[a] = 0;
	}

	charaObj[0].charaObj = *AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2((ssize.x / 4), ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_1, DIR_LEFT));
	charaObj[0].charaType = OBJ_TYPE_CHARACTER;

	if (mode == MODE_1PLAYER)
	{
		charaObj[1].charaObj = *AddObjList()(objList, std::make_unique<AIStickHuman>(VECTOR2(ssize.x * 3 / 4, ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), DIR_RIGHT));
		charaObj[1].charaType = OBJ_TYPE_AICHARACTER;
	}
	else if (mode == MODE_2PLAYER)
	{
		charaObj[1].charaObj = *AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2(ssize.x * 3 / 4, ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_2, DIR_RIGHT));
		charaObj[1].charaType = OBJ_TYPE_CHARACTER;
	}
	else
	{
		// MODE_MAXが来ている(エラー)
		AST();
	}
	
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

	

	// 横軸の処理
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
	}
}

void GameScene::ExtrusionUpdata(VECTOR2 beforPos[2], VECTOR2 afterPos[2])
{

	
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
	DrawGraph(bgPos.x, bgPos.y, IMAGE_ID("image/ゲームシーン用/bluesky_背景.png")[0], true);

	//objListに登録されているｸﾗｽの描画処理を行う
	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	// KOの文字の描画
	if (koDrawCount)
	{
		DrawString(500, 300, "KO", 0xff0000);
	}

	DrawFormatString(0, 300, 0xff0000, "chara 0 : winCount %d\nchara 1 : winCount %d\n", charaObj[0].winCount, charaObj[1].winCount);

	return true;
}
