#include "DxLib.h"
#include "SceneMng.h"
#include "Umbrella.h"
#include "Collision.h"
#include "SoundMng.h"

Umbrella::Umbrella()
{
}

Umbrella::Umbrella(sharedListFloor floorList, VECTOR2 pos, int padNum)
{
	Obj::floorList = floorList;
	Obj::pos = pos;
	Obj::padNum = padNum;

	Obj::init("雨野降太");
}

Umbrella::~Umbrella()
{
}

const int Umbrella::GetPadNum(void)
{
	return Obj::padNum;
}

bool Umbrella::initAnim(void)
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

	loadImageAndCol("通常攻撃1", "attack", 6, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("通常攻撃2", "attackSecond", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("通常攻撃3", "attackThird", 4, false, ANIM_TYPE_MULTI_HIT);

	loadImageAndCol("通常攻撃_上", "attackUp", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("通常攻撃_下始まり", "attackDownStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("通常攻撃_下中_地上", "attackDown", 12, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("通常攻撃_下中_空中", "attackDown", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("ダメージ", "damage", 45, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("必殺技", "specialAttack", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("溜め攻撃", "chargeAttack", 3, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("溜め始まり", "chargeStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("溜め中", "charge", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("怒りゲージ溜め", "angryCharge", 1, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("回避", "avoid", 10, false, ANIM_TYPE_ATTACK);

	SetAnim("待機");

	//SEの登録
	lpSoundMng.SoundInit("sound/SE類/デュクシ.mp3", "ﾃﾞｭｸｼ");
	lpSoundMng.SoundInit("sound/SE類/デュクシ2.mp3", "ﾃﾞｭｸｼ2");
	return true;
}

void Umbrella::Draw(void)
{
	//Obj::Draw();
}

bool Umbrella::CheckObjType(OBJ_TYPE type)
{
	return (OBJ_TYPE_PLAYER == type);
}

void Umbrella::SetMove(const GameController & controller, weakListObj objList, weakListChicken chickenList, bool entryFlag)
{
}

void Umbrella::UniqueAttack(VECTOR2 & tmpPos)
{
	if (spAttackFlag)
	{
		spAttackFlag = false;
	}
	if (GetAnim() == "溜め攻撃")
	{
		tmpPos.x += (2 * (static_cast<int>(dir) * -2 + 1) * ((angryFlag || spAttackCnt) + 1));
	}
	if (GetAnim() == "傘浮遊")
	{
		vY = 1;
	}
}

void Umbrella::UniqueColGround(void)
{
	if (GetAnim() == "傘浮遊")
	{
		SetAnim("待機");
	}
}

void Umbrella::SetNextAnim(bool animEndFlag)
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
		else if (GetAnim() == "通常攻撃_上")
		{
			SetAnim("傘浮遊");
		}
		// 通常攻撃系ｱﾆﾒｰｼｮﾝが終了したら待機ｱﾆﾒｰｼｮﾝへ移行
		else
		{
			fixCtrlCnt = FIX_TIME;
			// 攻撃ｱﾆﾒｰｼｮﾝ終了時に連撃ﾌﾗｸﾞが立っているか
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

void Umbrella::PlaySPSe(void)
{
	if (spSeCnt % 7 == 3 && spSeCnt <= 72)
	{
		lpSoundMng.SetSound("ﾃﾞｭｸｼ");
	}
	if (spSeCnt == 100)
	{
		lpSoundMng.SetSound("ﾃﾞｭｸｼ2");
	}
}
