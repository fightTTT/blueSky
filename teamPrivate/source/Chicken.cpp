#include "SceneMng.h"
#include "ImageMng.h"
#include "ColMng.h"
#include "Collision.h"
#include "Chicken.h"
#include "SoundMng.h"

#define JUMP_CNT_MAX	(2)
#define INVISIBLE_TIME	(60)
#define CHICKEN_FIX_TIME		(15)
#define CHICKEN_SIZE	(48)

#define EXP_SIZE		(96)
#define EXP_FRAM_X		(6)
#define EXP_FRAM_Y		(4)
#define EXP_INV			(3)
#define EXP_CNT_MAX		(EXP_FRAM_X * EXP_FRAM_Y)
#define UI_UP_CNT		(150)

Chicken::Chicken(sharedListFloor floorList, VECTOR2 pos, int padNum, CHICKEN_COLOR chickenColor)
{
	Obj::AddChickenCnt(padNum);

	Chicken::color		= chickenColor;
	Chicken::floorList	= floorList;
	Chicken::padNum		= padNum;
	Chicken::pos		= pos;

	chickenImagePass	= {
		"oniwatorisama01",
		"oniwatorisama02",
		"nwtr"
	};

	imageName = "image/鶏/" + chickenImagePass[chickenColor] + ".png";
	lpImageMng.GetID("image/鶏/blast.png", VECTOR2(EXP_SIZE, EXP_SIZE), VECTOR2(EXP_FRAM_X, EXP_FRAM_Y));

	hp			= 2;
	speed		= 4;
	gravity		= 0.4f;

	dirOld		= DIR::RIGHT;
	vY			= 0;
	vX			= 0;
	locate		= LOCATE::NON;
	param		= PARAM::FALLING;
	parentFloor = {};
	fallFlag	= false;
	jumpCnt		= JUMP_CNT_MAX;
	downFlag	= false;
	nowTimeDown = 0;
	nowTimeLR	= 0;
	framCnt		= 0;
	dashFlag	= false;
	visible		= 0;
	fixCtrl		= 0;
	damageInv	= 0;
	expCnt		= 0;
	deathFlag	= false;
	chickenSoul = {};
	chickenSoul.image	= -1;	// 未だ読み込んでいないことを表すため、ｴﾗｰ値を入れておく
	floorCheckFlag		= true;

	// UI
	UICnt		= 0;

	// 鳴き声追加
	lpSoundMng.SoundInit("sound/SE類/Chicken1.mp3", "ｺｹｺｯｺｰ");
	lpSoundMng.SoundInit("sound/SE類/Chicken2.mp3", "ｺｹｰ");
	lpSoundMng.SoundInit("sound/SE類/Chicken3.mp3", "ｺｯｺ");
	lpSoundMng.SoundInit("sound/SE類/Chicken4.mp3", "ｺｯｺ歩き");
	lpSoundMng.SoundInit("sound/SE類/Bomb.mp3", "爆発",85); 
	lpSoundMng.SoundInit("sound/SE類/Dead.mp3", "死亡");
	lpSoundMng.SoundInit("sound/SE類/Continue.mp3", "復活",85);

	// 再生
	lpSoundMng.SetSound("復活");
	lpSoundMng.SetSound("ｺｹｺｯｺｰ");
	
	chickenWalkSeCnt = 0;
	deathSeFlag = false;
};


Chicken::~Chicken()
{
}

void Chicken::UpDate(const GameController & controller, weakListChicken chickenList)
{

	if (visible)
	{
		visible--;
	}

	if (deathFlag && expCnt)
	{
		return;
	}

	// UIの表示位置を指定位置まで上昇
	if (UICnt < UI_UP_CNT)
	{
		UICnt+=2;
	}

	// 死亡 && 昇天済み && 転生していない
	if (deathFlag && chickenSoul.alpha <= 0 && !chickenSoul.endFlag)
	{
		chickenSoul.endFlag = true;
		AddChickenList()(chickenList, std::make_unique<Chicken>(floorList, VECTOR2(pos.x,-CHICKEN_SIZE), padNum, color));
	}

	if (fixCtrl)
	{
		fixCtrl--;
	}

	if (damageInv)
	{
		damageInv--;
	}

	if (damageInv == 1)
	{
		hitPlayerInf = {};
	}

	framCnt++;
	dirOld = dir;

	VECTOR2 tmpPos = pos;

	// 自分のｺﾘｼﾞｮﾝの取得
	auto myCollision = GetMyCol();

	int maxEndPosY = 0;				// 現ﾌﾚｰﾑのｺﾗｲﾀﾞｰの最大Y座標
	for (auto &col : myCollision)
	{
		if (col.weponFlag)
		{
			continue;				// 武器ｺﾗｲﾀﾞｰはmaxEndPosYに含まない
		}

		if (col.startPos.y > maxEndPosY)
		{
			maxEndPosY = col.startPos.y;
		}
		if (col.endPos.y > maxEndPosY)
		{
			maxEndPosY = col.endPos.y;
		}
	}


	// 座標補正
	PosAdjust(tmpPos, maxEndPosY);

	// 自由落下
	if (vY > 0.0 && locate == LOCATE::NON)
	{
		param = PARAM::FALLING;
	}

	vY += gravity;

	// 速度分posを加算
	tmpPos.y += static_cast<int>(vY) * (static_cast<int>(fallFlag && param == PARAM::FALLING) + 1);
	tmpPos.x += static_cast<int>(vX);

	if (param == PARAM::FALLING && !downFlag)
	{
		floorCheckFlag = true;
	}

	if (!fixCtrl && !deathFlag)
	{
		// ﾀﾞｯｼｭ
		Dash(controller, tmpPos);

		// ｼﾞｬﾝﾌﾟ
		if (controller.GetInputDown(GPAD_INPUT_A, padNum) && (jumpCnt > 0))
		{
			// 再生
			lpSoundMng.SetSound("ｺｹｰ");

			jumpCnt--;
			downFlag = false;
			param = PARAM::JUMPING;
			floorCheckFlag = false;

			// 下キーを押している && 地に足がついていれば ハイジャンプ
			vY = (controller.GetInputHold(PAD_INPUT_DOWN, padNum) && locate != LOCATE::NON ? -15.0f : -10.0f);
			locate = LOCATE::NON;
		}


		// ↓↓キーの処理
		if (controller.GetInputDown(PAD_INPUT_DOWN, padNum))
		{
			if ((framCnt - nowTimeDown) < PRASS_INV && locate != LOCATE::NON)
			{
				floorCheckFlag = false;
				downFlag = true;
			}
			nowTimeDown = framCnt;
		}
		if (controller.GetInputHold(PAD_INPUT_DOWN, padNum))
		{
			// 空中↓キーで急下降
			if ((locate == LOCATE::NON) && (param == PARAM::FALLING))
			{
				fallFlag = true;
			}
		}
	}

	if (param != PARAM::JUMPING)
	{
		bool hitFlag = false;		// 床か地面につけばtrue

		auto GetColPos = [&](VECTOR2 pos)
		{
			pos.x *= static_cast<int>(dir) * -2 + 1;
			return VECTOR2(tmpPos + pos);
		};

		// 浮いている床との当たり判定
		if (floorCheckFlag)
		{
			bool flag = false;
			int itrCnt = 0;
			for (auto itr : *floorList)
			{
				VECTOR2 floorStartPos = itr->GetPos();
				VECTOR2 floorEndPos = itr->GetPos() + itr->GetSize();
				for (auto &col : myCollision)
				{
					if (col.weponFlag)
					{
						continue;
					}

					VECTOR2 startPos	= GetColPos(col.startPos);
					VECTOR2 endPos		= GetColPos(col.endPos);

					// 1ﾄﾞｯﾄ加算することで丁度床の上にいるときも当たるようにしている
					endPos.y++;

					if (Square(VECTOR2(startPos.x, endPos.y - 20), endPos, floorStartPos, floorEndPos))
					{
						// 浮いている壁と衝突した場合
						flag = true;
						parentFloor.oldPos = itr->GetPos();
						parentFloor.itrCnt = itrCnt;
					}
				}
				itrCnt++;
			}
			if (flag)
			{
				hitFlag = true;
				tmpPos.y = parentFloor.oldPos.y - maxEndPosY;
				locate = LOCATE::FLOOR;
			}
			else
			{
				parentFloor.itrCnt = 0;
			}
		}


		// 地面との当たり判定
		bool flag = false;
		VECTOR2 groundStartPos(0, GROUND_POS_Y);
		VECTOR2 groundEndPos = groundStartPos + VECTOR2(1280, 120);

		for (auto &col : myCollision)
		{
			if (col.weponFlag)
			{
				continue;
			}

			VECTOR2 startPos = GetColPos(col.startPos);
			VECTOR2 endPos = GetColPos(col.endPos);
			// 2ﾄﾞｯﾄ加算することで丁度床の上にいるときも当たるようにしている
			endPos.y += 2;

			if (Square(startPos, endPos, groundStartPos, groundEndPos))
			{
				if (col.weponFlag)
				{
					continue;
				}
				flag = true;
			}
		}
		if (flag)
		{
			hitFlag = true;
			locate = LOCATE::GROUND;
			tmpPos.y = GROUND_POS_Y - maxEndPosY;
		}

		if (hitFlag)
		{
			fallFlag = false;
			jumpCnt = JUMP_CNT_MAX;
			vY = 0.0f;
			vX = 0.0f;
		}
	}

	// 画面外に行かないように
	auto screenSize = lpSceneMng.GetScreenSize();
	if (tmpPos.x <= 0)
	{
		tmpPos.x = 0;
	}
	if (tmpPos.x > screenSize.x)
	{
		tmpPos.x = screenSize.x;
	}
	if (tmpPos.y <= 0)
	{
		tmpPos.y = 0;
	}
	if (tmpPos.y > screenSize.y)
	{
		tmpPos.y = GROUND_POS_Y - maxEndPosY;
	}

	if (vY > 0.0f)
	{
		locate = LOCATE::NON;
	}

	// 座標移動
	pos = tmpPos;

}

void Chicken::CheckHit(weakListObj objList)
{
	if (expCnt)
	{
		expCnt--;
	}
	else if (deathFlag && chickenSoul.alpha > 0)
	{
		chickenSoul.pos.y--;
		chickenSoul.alpha -= 2;
	}
	else
	{
	}

	if (deathFlag)
	{
		return;
	}

	if (visible)
	{
		return;					// 無敵時間中なら当たり判定を行わない
	}

	for (auto &obj : *objList.lock())
	{
		// 判定しようとしている攻撃がそのﾌﾟﾚｲﾔｰから受けた最後の攻撃と同じなら
		if (hitPlayerInf[(*obj).GetPadNum()].attackName == (*obj).GetAnim())
		{
			if (hitPlayerInf[(*obj).GetPadNum()].animId == (*obj).GetAnimId())
			{
				continue;			// 判定しようとしている攻撃がそのﾌﾟﾚｲﾔｰから受けた最後の攻撃と同じｺﾏなら判定を行わない
			}
		}


		// 自分のｺﾘｼﾞｮﾝの取得
		auto myCollision = GetMyCol();

		for (auto myCol : myCollision)
		{
			if (myCol.weponFlag)
			{
				continue;		// 自分自身の武器とは当たり判定を行わない
			}

			auto otherCollision = (*obj).GetPlayerCollision();
			for (auto otherCol : otherCollision)
			{
				if (!otherCol.weponFlag)
				{
					continue;	// 相手の武器ｺﾗｲﾀﾞｰ以外とは当たり判定を行わない
				}

				//////////////////////////////////////////////////////////////////////////
				// ｺｺまで抜けた時点で　													//
				// myCol	= 自分の武器ではないｺﾗｲﾀﾞｰがVectorで格納されている			//
				// otherCol = 敵の武器ｺﾗｲﾀﾞｰがVectorで格納されている					//
				//////////////////////////////////////////////////////////////////////////

				// dir == LEFT なら当たり判定の向きを逆にする
				myCol.startPos.x *= static_cast<int>(dir) * -2 + 1;
				myCol.endPos.x *= static_cast<int>(dir) * -2 + 1;
				VECTOR2 myStartPos = pos + myCol.startPos;
				VECTOR2 myEndPos = pos + myCol.endPos;

				otherCol.startPos.x *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				otherCol.endPos.x *= static_cast<int>((*obj).GetDir()) * -2 + 1;
				VECTOR2 otherStartPos = (*obj).GetPos() + otherCol.startPos;
				VECTOR2 otherEndPos = (*obj).GetPos() + otherCol.endPos;

				// 当たり判定を行う
				if (Square(myStartPos, myEndPos, otherStartPos, otherEndPos))
				{
					// 攻撃を受けた側 --------------------------
					hitPlayerInf[(*obj).GetPadNum()] = {	// 受けた攻撃の情報を格納
						(*obj).GetAnim(),
						(*obj).GetAnimId()
					};

					vY = -10.0f;
					vX =  10.0f * (static_cast<int>((*obj).GetDir()) * -2 + 1);
					damageInv = 60;
					locate = LOCATE::NON;
					visible = INVISIBLE_TIME;
					fixCtrl = CHICKEN_FIX_TIME;

					hp--;
					if (!hp)
					{
						// 再生
						lpSoundMng.SetSound("爆発");
						deathFlag = true;
						(*obj).AddChickenKillCnt();
						expCnt = EXP_FRAM_X * EXP_FRAM_Y * EXP_INV - 1;
						vY = 0.0f;
						vX = 0.0f;
					}

					// 再生
					lpSoundMng.SetSound("ｺｹｰ");


					return;
				}
			}
		}
	}
}

void Chicken::Draw(void)
{
	if (!((visible / 5) % 2))
	{
		double rate = 1.0;
		int r, g, b;
		GetDrawBright(&r,&g,&b);
		if ( hp <= 1 && (framCnt/5 %2) && !deathFlag)
		{
			SetDrawBright(255, 128, 128);
			rate = 1.25;
		}
		// ﾁｷﾝ本体
		DrawRotaGraph(
			pos,
			rate,
			static_cast<double>(chickenSoul.image != -1) * 3.141592,
			lpImageMng.GetID(imageName)[0],
			true,
			static_cast<int>(dir));
		SetDrawBright(r,g,b);
	}

	if (expCnt)
	{
		// 爆発
		int animCnt = ((EXP_CNT_MAX * EXP_INV - 1) - expCnt) / EXP_INV;
		DrawRotaGraph(
			pos,
			animCnt * 3.0 / EXP_CNT_MAX,
			0.0,
			lpImageMng.GetID("image/鶏/blast.png")[animCnt],
			true);
	}
	else
	{
		if (deathFlag)
		{
			if (chickenSoul.image == -1)
			{
				// chickenSoulの初期化
				chickenSoul.pos	  = pos;
				chickenSoul.alpha = 255;

				// 真っ白なchickenを作る
				chickenSoul.image = MakeScreen(CHICKEN_SIZE, CHICKEN_SIZE,true);
				SetDrawScreen(chickenSoul.image);
				ClsDrawScreen();
				SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
				DrawRotaGraph(
					CHICKEN_SIZE / 2,
					CHICKEN_SIZE / 2,
					1.0,
					0.0,
					lpImageMng.GetID(imageName)[0],
					true,
					static_cast<int>(dir));
				SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				DrawRotaGraph(
					CHICKEN_SIZE / 2,
					CHICKEN_SIZE / 2,
					1.0,
					0.0,
					lpImageMng.GetID(imageName)[0],
					true,
					static_cast<int>(dir));
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				SetDrawScreen(lpSceneMng.GetTmpBuffer());
			}

			if (chickenSoul.alpha > 0)
			{
				// 再生
				if (!deathSeFlag)
				{
					lpSoundMng.SetSound("死亡");
					deathSeFlag = true;
				}

				// 死亡後の爆発が終わった時 && 空中ではない
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, chickenSoul.alpha);
				DrawRotaGraph(
					chickenSoul.pos,
					1.0,
					0.0,
					chickenSoul.image,
					true,
					static_cast<int>(dir));
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
		}
	}

	// UI描画
	if (!deathFlag)
	{
		int chickenCnt = Obj::GetChickenCnt(padNum);
		int drawLengh;
		drawLengh = GetDrawFormatStringWidthToHandle(
			lpFontMng.SetFont("EP50"), 
			chickenCnt == 1 ? "初代" : "%d代目", chickenCnt);
		DrawFormatStringToHandle(320 * (padNum+1) - drawLengh, SCREEN_SIZE_Y - UICnt, 0xff0000, 
			lpFontMng.SetFont("EP50"), 
			chickenCnt == 1 ? "初代" : "%d代目", chickenCnt, true);
		DrawGraph(VECTOR2(320 * padNum, SCREEN_SIZE_Y - UICnt), lpImageMng.GetID("image/鶏/アイコン.png")[0], true);
	}
}

const int Chicken::GetPadNum(void)
{
	return padNum;
}

bool Chicken::CheckExpCnt(void)
{
	return (expCnt > 0);
}

void Chicken::Dash(const GameController & controller, VECTOR2 & tmpPos)
{
	if (controller.GetInputHold(PAD_INPUT_RIGHT, padNum))
	{
		dir = DIR::RIGHT;
		tmpPos.x += speed * (1 + static_cast<int>(dashFlag));
		
		chickenWalkSeCnt++;
	}
	else if (controller.GetInputHold(PAD_INPUT_LEFT, padNum))
	{
		dir = DIR::LEFT;
		tmpPos.x -= speed * (1 + static_cast<int>(dashFlag));

		chickenWalkSeCnt++;
	}
	else
	{
		dashFlag = false;

		chickenWalkSeCnt = 0;
	}

	if (dir != dirOld)
	{
		if (locate != LOCATE::NON)
		{
			dashFlag = false;
		}
	}

	if ((controller.GetInputDown(PAD_INPUT_RIGHT, padNum) || controller.GetInputDown(PAD_INPUT_LEFT, padNum)))
	{
		if (locate != LOCATE::NON)
		{
			if ((framCnt - nowTimeLR < PRASS_INV) && (dir == dirOld))
			{
				dashFlag = true;

				chickenWalkSeCnt++;
			}
			else
			{
				dashFlag = false;

				chickenWalkSeCnt = 0;
			}
		}
		nowTimeLR = framCnt;
	}
	if (locate == LOCATE::GROUND || locate == LOCATE::FLOOR)
	{
		if (chickenWalkSeCnt % 13 == 1)
		{
			lpSoundMng.SetSound("ｺｯｺ歩き");
		}
	}
}

std::vector<ColInf> Chicken::GetMyCol(void)
{
	std::vector<ColInf> myCollision;
	myCollision.resize(1);
	myCollision[0] = {};
	myCollision[0].boxFlag = true;
	myCollision[0].id = 0;
	myCollision[0].startPos = { -CHICKEN_SIZE / 2,-CHICKEN_SIZE / 2 };
	myCollision[0].endPos = { CHICKEN_SIZE / 2,CHICKEN_SIZE / 2 };

	return myCollision;
}

void Chicken::PosAdjust(VECTOR2 & tmpPos, int maxEndPosY)
{
	// 地面に乗っている場合の座標補正
	if (locate == LOCATE::GROUND)
	{
		tmpPos.y = GROUND_POS_Y - maxEndPosY;
		vY = 0.0f;
		vX = 0.0f;
	}

	// 浮いている床に乗っている場合の座標補正
	if ((locate == LOCATE::FLOOR) && (parentFloor.itrCnt != 99999))
	{
		auto floor = floorList->begin();
		for (int i = 0; i < parentFloor.itrCnt; i++)
		{
			floor++;
		}
		// 浮いている床が動いた座標だけﾌﾟﾚｲﾔの座標を動かす
		//tmpPos += (parentFloor.oldPos - (*itr)->GetPos());
		tmpPos.y =  (*floor)->GetPos().y - maxEndPosY;
		tmpPos.x += (*floor)->GetPos().x - parentFloor.oldPos.x;
		vY = 0.0f;
		vX = 0.0f;
		//tmpPos.y = (*itr)->GetPos().y - maxEndPosY;
		parentFloor.itrCnt = 99999;	// ｴﾗｰｶｳﾝﾄを入れておく
	}
}
