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

	Obj::init("�o�t�u");
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
	// �ײ�ް�Ɖ摜��۰��
	auto loadImageAndCol = [=](std::string animName, std::string dataName, int inv, bool loop, ANIM_TYPE animType)
	{
		lpImageMng.LoadContinueImageId(GetCharName() + "/" + animName, dataName);
		lpColMng.ColLoad(GetCharName(), animName);
		AddAnim(animName, inv, loop, dataName);
		animPtnTbl[animName] = animType;
	};

	//				��Ұ��ݖ�			�ް���				�ϐ�	ٰ��	��Ұ�������
	loadImageAndCol("�W�����v�n�܂�", "jumpStart", 6, false, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("�W�����v��", "jump", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("�ҋ@", "wait", 8, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("����", "walk", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("����", "dash", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("�K�[�h", "guard", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("�P���V", "fly", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("���S", "death", 120, false, ANIM_TYPE_NO_ATTACK);

	loadImageAndCol("�ʏ�U��1", "attack", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("�ʏ�U��2", "attackSecond", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("�ʏ�U��3", "attackThird", 6, false, ANIM_TYPE_MULTI_HIT);

	loadImageAndCol("�ʏ�U��_��", "attackUp", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�ʏ�U��_���n�܂�", "attackDownStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�ʏ�U��_����_��", "attackDown", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("�ʏ�U��_����_�n��", "attackDown", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�_���[�W", "damage", 45, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�K�E�Z", "specialAttack", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("���ߍU��", "chargeAttack", 3, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("���ߎn�܂�", "chargeStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("���ߒ�", "charge", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("�{��Q�[�W����", "angryCharge", 1, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("���", "avoid", 10, false, ANIM_TYPE_ATTACK);

	SetAnim("�ҋ@");
	
	//SE�̓o�^
	lpSoundMng.SoundInit("sound/SE��/Elefant.mp3", "�ߵ��");

	spSeCnt = 0;

	return true;
}

void Lid::Draw(bool pauseFlag, CUT_IN_INFO cutInInfo)
{
	Obj::Draw(pauseFlag, cutInInfo);
	if (GetAnim() == "���ߍU��")
	{
		int cnt = animCnt / 2;	// ++������[ 
		DrawRectGraph(
			pos.x - 150 / 2 + (static_cast<int>(dir) * 2 - 1) * 40, pos.y - 150 / 2,
			150 * (cnt % 5), 150 * (cnt / 5 % 2),
			150, 150,
			lpImageMng.GetID("image/UI/�Ռ�24.png")[0], true, static_cast<int>(dir));
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
	if (GetAnim() == "���ߍU��")
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
		lpSoundMng.SetSound("�ߵ��");
	}
}
