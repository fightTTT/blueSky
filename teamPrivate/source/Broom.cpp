#include "DxLib.h"
#include "SceneMng.h"
#include "Broom.h"
#include "Collision.h"
#include "SoundMng.h"

#define JUMP_CNT_MAX	(3)

Broom::Broom()
{
}

Broom::Broom(sharedListFloor floorList, VECTOR2 pos, int padNum)
{
	Obj::floorList = floorList;
	Obj::pos = pos;
	Obj::padNum = padNum;

	Obj::init("竹部箒");
}


Broom::~Broom()
{
}

const int Broom::GetPadNum(void)
{
	return Obj::padNum;
}

bool Broom::initAnim(void)
{
	// ｺﾗｲﾀﾞｰと画像のﾛｰﾄﾞ
	auto loadImageAndCol = [=](std::string animName, std::string dataName, int inv, bool loop, ANIM_TYPE animType)
	{
		lpImageMng.LoadContinueImageId(GetCharName() + "/" + animName, dataName);
		lpColMng.ColLoad(GetCharName(), animName);
		AddAnim(animName, inv, loop, dataName);
		animPtnTbl[animName] = animType;
	};

	//				ｱﾆﾒｰｼｮﾝ名			ﾃﾞｰﾀ名				ｺﾏ数	ﾙｰﾌﾟ	ｱﾆﾒｰｼｮﾝﾀｲﾌﾟ
	loadImageAndCol("ジャンプ始まり", "jumpStart", 6, false, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("ジャンプ中", "jump", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("待機", "wait", 8, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("歩く", "walk", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("走る", "dash", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("ガード", "guard", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("傘浮遊", "fly", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("死亡", "death", 120, false, ANIM_TYPE_NO_ATTACK);

	loadImageAndCol("通常攻撃1", "attack", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("通常攻撃2", "attackSecond", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("通常攻撃3", "attackThird", 6, false, ANIM_TYPE_MULTI_HIT);

	loadImageAndCol("通常攻撃_上", "attackUp", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("通常攻撃_下始まり", "attackDownStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("通常攻撃_下中_地上", "attackDown", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("通常攻撃_下中_空中", "attackDown", 3, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("ダメージ", "damage", 45, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("必殺技", "specialAttack", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("溜め攻撃", "chargeAttack", 5, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("溜め始まり", "chargeStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("溜め中", "charge", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("怒りゲージ溜め", "angryCharge", 1, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("回避", "avoid", 10, false, ANIM_TYPE_ATTACK);

	SetAnim("待機");

	lpSoundMng.SoundInit("sound/SE類/Chicken1.mp3", "ｺｹｺｯｺｰ");

	spSeCnt = 0;

	return true;
}

void Broom::Draw(void)
{
}

bool Broom::CheckObjType(OBJ_TYPE type)
{
	return OBJ_TYPE_PLAYER == type;
}

void Broom::SetMove(const GameController & controller, weakListObj objList, weakListChicken chickenList, bool entryFlag)
{
}

void Broom::PosUpDate(VECTOR2 & tmpPos, int maxEndPosY)
{
	if (GetAnim() != "必殺技" && !spAttackHitFlag && GetAnim() != "通常攻撃_上")
	{
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
	}
}

void Broom::UniqueAttack(VECTOR2 & tmpPos)
{
	if (GetAnim() == "通常攻撃_上")
	{
		vY = 0.0;
		vX = 0.0;
		tmpPos.x += 5 * (static_cast<int>(dir) * -2 + 1) * ((angryFlag || spAttackCnt) + 1);
		tmpPos.y -= 5;
	}
}

void Broom::SetNextAnim(bool animEndFlag)
{
	if (!animEndFlag)
	{
		return;
	}
	if (GetAnim() == "ジャンプ始まり")
	{
		SetAnim("ジャンプ中");
	}

	// 攻撃系ｱﾆﾒｰｼｮﾝが終了した時
	if (animPtnTbl[GetAnim()] != ANIM_TYPE_NO_ATTACK)
	{
		// 溜め始まりｱﾆﾒｰｼｮﾝが終了したら溜め中ｱﾆﾒｰｼｮﾝへ移行
		if (GetAnim() == "溜め始まり")
		{
			SetAnim("溜め中");
		}
		else if (GetAnim() == "通常攻撃_下始まり")
		{
			(locate == LOCATE::NON) ? SetAnim("通常攻撃_下中_空中") : SetAnim("通常攻撃_下中_地上");
		}
		// 通常攻撃系ｱﾆﾒｰｼｮﾝが終了したら待機ｱﾆﾒｰｼｮﾝへ移行
		else
		{
			if (!jumpCnt && deri == DERI_UP)
			{
				deri = DERI_NOMAL;	// jumpCntが0ならNOMALの派生攻撃を行う
			}
			fixCtrlCnt = FIX_TIME;

			std::string str;
			switch (deri)
			{
			case DERI_NOMAL:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				multiAttackNum++;
				str = "通常攻撃" + std::to_string(multiAttackNum + 1);
				SetAnim(str);
				break;

			case DERI_UP:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				downFlag = false;
				param = PARAM::JUMPING;
				floorCheckFlag = false;
				jumpCnt--;
				locate = LOCATE::NON;
				SetAnim("通常攻撃_上");
				multiAttackNum = 0;
				break;

			case DERI_DOWN:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				SetAnim("通常攻撃_下始まり");
				multiAttackNum = 0;
				break;

			case DERI_NON:
			default:
				deri = DERI_NON;
				if (GetAnim() == "ダメージ")
				{
					fixCtrlCnt = 0;
					if (!spAttackHitFlag)
					{
						SetAnim("待機");
					}
				}
				else
				{
					SetAnim("待機");
				}
				multiAttackNum = 0;
				break;
			}

			if (multiAttackNum > 2)
			{
				fixCtrlCnt = 0;
				multiAttackNum = 0;	// 3連撃で終了
				SetAnim("待機");
			}

			chargeStage = CHARGE_NON;
			gravity = 0.4f;
		}
	}
}

void Broom::PlaySPSe(void)
{
	if (spSeCnt == 70)
	{
		lpSoundMng.SetSound("ｺｹｺｯｺｰ");
	}
}

int Broom::GetJumpCntMax(void)
{
	return JUMP_CNT_MAX;
}
