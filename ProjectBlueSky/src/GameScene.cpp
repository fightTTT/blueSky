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

#define STICK_HUMAN_IMAGE_SIZE_X (256)
#define STICK_HUMAN_IMAGE_SIZE_Y (256)

#define BG_IMAGE_SIZE_X (1920)
#define BG_IMAGE_SIZE_Y (720)

#define DEF_BG_POS_X (-(BG_IMAGE_SIZE_X / 6))

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
	// ｷｬﾗｸﾀｰの情報をｾｯﾄ
	sharedObj sObj[2];	// キャラクターのObj変数保存
	OBJ_TYPE type[2];	// キャラクターのタイプ

	int charaCount = 0;	// 見つかったキャラクターの数

	for (auto& data : *objList)
	{
		if (data->CheckObjType(OBJ_TYPE_CHARACTER))
		{
			sObj[charaCount] = data;
			type[charaCount] = OBJ_TYPE_CHARACTER;
			charaCount++;
		}
		else if (data->CheckObjType(OBJ_TYPE_AICHARACTER))
		{
			sObj[charaCount] = data;
			type[charaCount] = OBJ_TYPE_AICHARACTER;
			charaCount++;
		}
		else
		{
			// 何もしない
		}

		if (charaCount >= 2)
		{
			break;
		}
	}

	// 更新前のｷｬﾗｸﾀｰの位置情報格納
	VECTOR2 characterPosOld[2] = { sObj[0]->GetPos(), sObj[1]->GetPos() };

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
	if (sObj[0]->GetPos().x > sObj[1]->GetPos().x)
	{
		sObj[0]->SetDir(DIR_LEFT);
		sObj[1]->SetDir(DIR_RIGHT);
	}
	else
	{
		sObj[0]->SetDir(DIR_RIGHT);
		sObj[1]->SetDir(DIR_LEFT);
	}

	// 一人が後ろ歩きでもう一人が攻撃系のアニメーションの時に、後ろ歩きをしている方をガード状態にする
	if (sObj[0]->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK)
	{
		if (sObj[1]->GetAnim() == "後ろ移動")
		{
			sObj[1]->SetAnim("ガード_立ち");
		}

		if (sObj[1]->GetAnim() == "しゃがみ_後ろ")
		{
			sObj[1]->SetAnim("ガード_しゃがみ");
		}
	}
	if (sObj[1]->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK)
	{
		if (sObj[0]->GetAnim() == "後ろ移動")
		{
			sObj[0]->SetAnim("ガード_立ち");
		}

		if (sObj[0]->GetAnim() == "しゃがみ_後ろ")
		{
			sObj[0]->SetAnim("ガード_しゃがみ");
		}
	}

	// キャラクターの状態を相手に渡す
	if (!(type[0] == type[1]))
	{
		if (type[0] == OBJ_TYPE_CHARACTER)
		{
			eState.enemyPos = sObj[0]->GetPos();
			eState.enemyAnimAttribute[0] = sObj[0]->GetAnimAttribute(0);
			eState.enemyAnimAttribute[1] = sObj[0]->GetAnimAttribute(1);
			sObj[1]->SetEnemyState(eState);
		}
		else
		{
			eState.enemyPos = sObj[1]->GetPos();
			eState.enemyAnimAttribute[0] = sObj[1]->GetAnimAttribute(0);
			eState.enemyAnimAttribute[1] = sObj[1]->GetAnimAttribute(1);
			sObj[0]->SetEnemyState(eState);
		}
	}

	std::string animName[2];

	for (int i = 0; i < 2; i++)
	{
		animName[i] = sObj[i]->GetAnim();

		// 当たり判定をfalseにする
		sObj[i]->SetHitData(false, COL_TYPE_NON);
	}

	// 当たり判定処理
	{
		ColInfo colData[2];			// 当たり判定の情報を格納する変数(2キャラ分)

		if (lpColMng.GetColFlag(sObj[0]->GetAnim())
		   && lpColMng.GetColFlag(sObj[1]->GetAnim()))
		{
			// 当たり判定の情報を取得
			for (int i = 0; i < 2; i++)
			{
				id[i] = sObj[i]->GetCount(sObj[i]->GetAnim());
				if (id[i] < sObj[i]->GetAnimFrame(sObj[i]->GetAnim()))
				{
					colData[i] = lpColMng.GetCollisionData("棒人間", sObj[i]->GetAnim(), id[i]);
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

					colData[i].hitBox[a].rect.startPos.x += sObj[i]->GetAnimOffSet(animName[i]).x;
					colData[i].hitBox[a].rect.endPos.x += sObj[i]->GetAnimOffSet(animName[i]).x;
					colData[i].hitBox[a].rect.startPos.y += sObj[i]->GetAnimOffSet(animName[i]).y;
					colData[i].hitBox[a].rect.endPos.y += sObj[i]->GetAnimOffSet(animName[i]).y;

					int b;

					colData[i].hitBox[a].rect.startPos.x *= static_cast<int>(sObj[i]->GetDir()) * -2 + 1;
					colData[i].hitBox[a].rect.endPos.x *= static_cast<int>(sObj[i]->GetDir()) * -2 + 1;

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
					colData[i].hitBox[a].rect.startPos.x = (sObj[i]->GetPos().x + colData[i].hitBox[a].rect.startPos.x) + (sObj[i]->GetDivSize().x/2);
					colData[i].hitBox[a].rect.endPos.x = (sObj[i]->GetPos().x + colData[i].hitBox[a].rect.endPos.x) + (sObj[i]->GetDivSize().x / 2);
					colData[i].hitBox[a].rect.startPos.y =(sObj[i]->GetPos().y + colData[i].hitBox[a].rect.startPos.y) + (sObj[i]->GetDivSize().y);
					colData[i].hitBox[a].rect.endPos.y = (sObj[i]->GetPos().y + colData[i].hitBox[a].rect.endPos.y) + (sObj[i]->GetDivSize().y);

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
									sObj[(i + 1) % 2]->SetHitData(true, colData[(i + 1) % 2].hitBox[b].type);
									
									if (colData[(i + 1) % 2].hitBox[b].type == COLTYPE_GUARD)
									{
										break;
									}
									break;
								}
								else
								{
									sObj[(i + 1) % 2]->SetHitData(false, colData[(i + 1) % 2].hitBox[b].type);
								}
							}
						}
						else
						{
							sObj[(i + 1) % 2]->SetHitData(false, colData[(i + 1) % 2].hitBox[b].type);
						}
					}
				}
			}

			for (int i = 0; i < 2; i++)
			{
				for (int a = 0; a < colData[i].hitBox.size(); a++)
				{
					// 波動拳の当たり判定
					if (eState.shotData.size())
					{
						VECTOR2 startPos = { 0,0 };
						VECTOR2 endPos = { 0,0 };

						if (colData[i].hitBox[a].type != COLTYPE_ATTACK)
						{
							for (int s = 0; s < eState.shotData.size(); s++)
							{
								startPos = { eState.shotData[s].pos.x + (shotObj->GetDivSize().x / 2) - 50,eState.shotData[s].pos.y + (shotObj->GetDivSize().y / 2) - 50 };
								endPos = { eState.shotData[s].pos.x + (shotObj->GetDivSize().x / 2) + 50,eState.shotData[s].pos.y + (shotObj->GetDivSize().y / 2) + 50 };

								if (colData[i].hitBox[a].rect.endPos.x >= startPos.x
									&& colData[i].hitBox[a].rect.startPos.x <= endPos.x
									&& colData[i].hitBox[a].rect.endPos.y >= startPos.y
									&& colData[i].hitBox[a].rect.startPos.y <= endPos.y)
								{

									sObj[i]->SetHitData(true, colData[i].hitBox[a].type);
									shotObj->SetHitData(true, colData[i].hitBox[a].type);
									break;
								}
								else
								{
									sObj[i]->SetHitData(false, colData[i].hitBox[a].type);
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

	// 更新後のｷｬﾗｸﾀｰの位置情報格納
	VECTOR2 characterPos[2] = { sObj[0]->GetPos(), sObj[1]->GetPos() };

	// 横軸の処理
	if (((characterPos[0].x - characterPosOld[0].x) == 0) && ((characterPos[1].x - characterPosOld[1].x) == 0))
	{
		// どちらも動いていない
	}
	else
	{
		// 少なくともどちらか一方は動いている
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

		int edgePosLeft;		// 移動制限の左側
		int edgePosRight;		// 移動制限の右側

		edgePosLeft = (ssize.x / 5);
		edgePosRight = ((ssize.x * 4) / 5);

		edgePosLeft = (ssize.x / 5) + bgPos_x;
		if (edgePosLeft < (STICK_HUMAN_IMAGE_SIZE_X / 2))
		{
			edgePosLeft = (STICK_HUMAN_IMAGE_SIZE_X / 2);
		}
		if (edgePosLeft > (ssize.x / 5))
		{
			edgePosLeft = (ssize.x / 5);
		}
		
		edgePosRight = (((ssize.x * 4) / 5) + bgPos_x - (ssize.x - BG_IMAGE_SIZE_X));
		if (edgePosRight < ((ssize.x * 4) / 5))
		{
			edgePosRight = ((ssize.x * 4) / 5);
		}
		if (edgePosRight > (ssize.x - (STICK_HUMAN_IMAGE_SIZE_X / 2)))
		{
			edgePosRight = (ssize.x - (STICK_HUMAN_IMAGE_SIZE_X / 2));
		}

		if (characterPos[leftCharacter].x < edgePosLeft)
		{
			if (characterPos[rightCharacter].x > edgePosRight)
			{
				sObj[leftCharacter]->SetPos(VECTOR2(edgePosLeft, characterPos[leftCharacter].y));
				sObj[rightCharacter]->SetPos(VECTOR2(edgePosRight, characterPos[rightCharacter].y));
			}
			else
			{
				sObj[leftCharacter]->SetPos(VECTOR2(edgePosLeft, characterPos[leftCharacter].y));

				if ((bgPos_x + (edgePosLeft - characterPos[leftCharacter].x)) > 0)
				{
					if ((characterPos[rightCharacter].x + (0 - bgPos_x)) > edgePosRight)
					{
						bgPos_x += edgePosRight - characterPos[rightCharacter].x;
						sObj[rightCharacter]->SetPos(VECTOR2(edgePosRight, characterPos[rightCharacter].y));
					}
					else
					{
						bgPos_x = 0;
					}
				}
				else if ((characterPos[rightCharacter].x + (edgePosLeft - characterPos[leftCharacter].x)) > edgePosRight)
				{
					bgPos_x += edgePosRight - characterPos[rightCharacter].x;
					sObj[rightCharacter]->SetPos(VECTOR2(edgePosRight, characterPos[rightCharacter].y));
				}
				else
				{
					bgPos_x += (edgePosLeft - characterPos[leftCharacter].x);
					sObj[rightCharacter]->AddPos(VECTOR2(edgePosLeft - characterPos[leftCharacter].x, 0));
				}
			}
		}
		else if (characterPos[rightCharacter].x > edgePosRight)
		{
			sObj[rightCharacter]->SetPos(VECTOR2(edgePosRight, characterPos[rightCharacter].y));

			if (bgPos_x + (edgePosRight - characterPos[rightCharacter].x) < (ssize.x - BG_IMAGE_SIZE_X))
			{
				if (characterPos[leftCharacter].x + ((ssize.x - BG_IMAGE_SIZE_X) - bgPos_x) < edgePosLeft)
				{
					bgPos_x += edgePosLeft - characterPos[leftCharacter].x;
					sObj[leftCharacter]->SetPos(VECTOR2(edgePosLeft, characterPos[leftCharacter].y));
				}
				else
				{
					bgPos_x = (ssize.x - BG_IMAGE_SIZE_X);
				}
			}
			else if (characterPos[leftCharacter].x + (edgePosRight - characterPos[rightCharacter].x) < edgePosLeft)
			{
				bgPos_x += edgePosLeft - characterPos[leftCharacter].x;
				sObj[leftCharacter]->SetPos(VECTOR2(edgePosLeft, characterPos[leftCharacter].y));
			}
			else
			{
				bgPos_x += (edgePosRight - characterPos[rightCharacter].x);
				sObj[leftCharacter]->AddPos(VECTOR2(edgePosRight - characterPos[rightCharacter].x, 0));
			}
		}
		else
		{
			// 何もしない
		}
	}

	//描画処理
	GameDraw();

	return std::move(own);
}

int GameScene::Init(void)
{
	MODE mode = lpSceneMng.GetMode();
	ssize = lpSceneMng.GetScreenSize();
	bgPos_x = DEF_BG_POS_X;

	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objListを全削除する
	for (int a = 0; a < 2; a++)
	{
		id[a] = 0;
	}

	AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2((ssize.x / 4), ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_1, DIR_LEFT));

	if (mode == MODE_1PLAYER)
	{
		AddObjList()(objList, std::make_unique<AIStickHuman>(VECTOR2(ssize.x * 3 / 4, ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), DIR_RIGHT));
	}
	else if (mode == MODE_2PLAYER)
	{
		AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2(ssize.x * 3 / 4, ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_2, DIR_RIGHT));
	}
	else
	{
		// MODE_MAXが来ている(エラー)
		AST();
	}
	
	return 0;
}

bool GameScene::GameDraw(void)
{
	DrawGraph(bgPos_x, 0, IMAGE_ID("image/bluesky_背景.png")[0], true);

	//objListに登録されているｸﾗｽの描画処理を行う
	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	return true;
}
