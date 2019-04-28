#include "DxLib.h"
#include "SceneMng.h"
#include "Collision.h"
#include "SoundMng.h"
#include "Lid.h"

#define SP_ATTACK_TIME_MAX	(900)
#define GUARD_TIME_MAX	(360)
#define FLY_HIGH_POWER	(VECTOR2(10,10))

Lid::Lid()
{
}

Lid::Lid(sharedListFloor floorList, VECTOR2 pos, int padNum)
{
	Obj::floorList		= floorList;
	Obj::pos			= pos;
	Obj::padNum			= padNum;

	Obj::init("ëoótê¬ìu");
}

Lid::~Lid()
{
}

const int Lid::GetPadNum(void)
{
	return Obj::padNum;
}

bool Lid::initAnim(void)
{
	// ∫◊≤¿ﬁ∞Ç∆âÊëúÇÃ€∞ƒﬁ
	auto loadImageAndCol = [=](std::string animName, std::string dataName, int inv, bool loop, ANIM_TYPE animType)
	{
		lpImageMng.LoadContinueImageId(GetCharName() + "/" + animName, dataName);
		lpColMng.ColLoad(GetCharName(), animName);
		AddAnim(animName, inv, loop, dataName);
		animPtnTbl[animName] = animType;
	};

	//				±∆“∞ºÆ›ñº			√ﬁ∞¿ñº				∫œêî	Ÿ∞Ãﬂ	±∆“∞ºÆ›¿≤Ãﬂ
	loadImageAndCol("ÉWÉÉÉìÉvénÇ‹ÇË", "jumpStart", 6, false, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("ÉWÉÉÉìÉvíÜ", "jump", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("ë“ã@", "wait", 8, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("ï‡Ç≠", "walk", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("ëñÇÈ", "dash", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("ÉKÅ[Éh", "guard", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("éPïÇóV", "fly", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("éÄñS", "death", 120, false, ANIM_TYPE_NO_ATTACK);

	loadImageAndCol("í èÌçUåÇ1", "attack", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("í èÌçUåÇ2", "attackSecond", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("í èÌçUåÇ3", "attackThird", 6, false, ANIM_TYPE_MULTI_HIT);

	loadImageAndCol("í èÌçUåÇ_è„", "attackUp", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("í èÌçUåÇ_â∫énÇ‹ÇË", "attackDownStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("í èÌçUåÇ_â∫íÜ_ãÛíÜ", "attackDown", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("í èÌçUåÇ_â∫íÜ_ínè„", "attackDown", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("É_ÉÅÅ[ÉW", "damage", 45, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("ïKéEãZ", "specialAttack", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("ó≠ÇﬂçUåÇ", "chargeAttack", 3, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("ó≠ÇﬂénÇ‹ÇË", "chargeStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("ó≠ÇﬂíÜ", "charge", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("ì{ÇËÉQÅ[ÉWó≠Çﬂ", "angryCharge", 1, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("âÒî", "avoid", 10, false, ANIM_TYPE_ATTACK);

	SetAnim("ë“ã@");
	
	//SEÇÃìoò^
	lpSoundMng.SoundInit("sound/SEóﬁ/Elefant.mp3", " ﬂµ∞›");

	spSeCnt = 0;

	return true;
}

void Lid::Draw(bool pauseFlag, CUT_IN_INFO cutInInfo)
{
	Obj::Draw(pauseFlag, cutInInfo);
	if (GetAnim() == "ó≠ÇﬂçUåÇ")
	{
		int cnt = animCnt / 2;	// ++Ç≥ÇÍÇÈÇÊÅ[ 
		DrawRectGraph(
			pos.x - 150 / 2 + (static_cast<int>(dir) * 2 - 1) * 40, pos.y - 150 / 2,
			150 * (cnt % 5), 150 * (cnt / 5 % 2),
			150, 150,
			lpImageMng.GetID("image/UI/è’åÇ24.png")[0], true, static_cast<int>(dir));
	}
}

const VECTOR2 Lid::GetFlyHighPower(void)
{
	return VECTOR2(FLY_HIGH_POWER.x * (static_cast<int>(dir) * -2 + 1), FLY_HIGH_POWER.y);
}

bool Lid::CheckObjType(OBJ_TYPE type)
{
	return (OBJ_TYPE_PLAYER == type);
}

void Lid::SetMove(const GameController & controller, weakListObj objList, weakListChicken chickenList, bool entryFlag)
{
}

void Lid::UniqueAttack(VECTOR2 & tmpPos)
{
	if (GetAnim() == "ó≠ÇﬂçUåÇ")
	{
		tmpPos.x += speed * ((static_cast<int>(CheckCharge()) + 1) * 7 / CHARGE_MAX) * (static_cast<int>(dir) * -2 + 1) * ((angryFlag || spAttackCnt) + 1);
	}
}

int Lid::GetSpAttackTimeMax(void)
{
	return SP_ATTACK_TIME_MAX;
}

int Lid::GetGuardTimeMax(void)
{
	return GUARD_TIME_MAX;
}

void Lid::PlaySPSe(void)
{
	if (spSeCnt == 10)
	{
		lpSoundMng.SetSound(" ﬂµ∞›");
	}
}
