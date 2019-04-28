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

	Obj::init("�J��~��");
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

	loadImageAndCol("�ʏ�U��1", "attack", 6, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("�ʏ�U��2", "attackSecond", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("�ʏ�U��3", "attackThird", 4, false, ANIM_TYPE_MULTI_HIT);

	loadImageAndCol("�ʏ�U��_��", "attackUp", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�ʏ�U��_���n�܂�", "attackDownStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�ʏ�U��_����_�n��", "attackDown", 12, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�ʏ�U��_����_��", "attackDown", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("�_���[�W", "damage", 45, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�K�E�Z", "specialAttack", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("���ߍU��", "chargeAttack", 3, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("���ߎn�܂�", "chargeStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("���ߒ�", "charge", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("�{��Q�[�W����", "angryCharge", 1, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("���", "avoid", 10, false, ANIM_TYPE_ATTACK);

	SetAnim("�ҋ@");

	//SE�̓o�^
	lpSoundMng.SoundInit("sound/SE��/�f���N�V.mp3", "�ޭ��");
	lpSoundMng.SoundInit("sound/SE��/�f���N�V2.mp3", "�ޭ��2");
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
	if (GetAnim() == "���ߍU��")
	{
		tmpPos.x += (2 * (static_cast<int>(dir) * -2 + 1) * ((angryFlag || spAttackCnt) + 1));
	}
	if (GetAnim() == "�P���V")
	{
		vY = 1;
	}
}

void Umbrella::UniqueColGround(void)
{
	if (GetAnim() == "�P���V")
	{
		SetAnim("�ҋ@");
	}
}

void Umbrella::SetNextAnim(bool animEndFlag)
{
	if (!animEndFlag)
	{
		return;
	}
	if (GetAnim() == "�W�����v�n�܂�")
	{
		SetAnim("�W�����v��");
	}

	// �U���n��Ұ��݂��I��������
	if (animPtnTbl[GetAnim()] != ANIM_TYPE_NO_ATTACK)
	{
		// ���ߎn�܂��Ұ��݂��I�������痭�ߒ���Ұ��݂ֈڍs
		if (GetAnim() == "���ߎn�܂�")
		{
			SetAnim("���ߒ�");
		}
		else if (GetAnim() == "�ʏ�U��_���n�܂�")
		{
			(locate == LOCATE::NON) ? SetAnim("�ʏ�U��_����_��") : SetAnim("�ʏ�U��_����_�n��");
		}
		else if (GetAnim() == "�ʏ�U��_��")
		{
			SetAnim("�P���V");
		}
		// �ʏ�U���n��Ұ��݂��I��������ҋ@��Ұ��݂ֈڍs
		else
		{
			fixCtrlCnt = FIX_TIME;
			// �U����Ұ��ݏI�����ɘA���׸ނ������Ă��邩
			std::string str;
			switch (deri)
			{
			case DERI_NOMAL:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				multiAttackNum++;
				str = "�ʏ�U��" + std::to_string(multiAttackNum + 1);
				SetAnim(str);
				break;

			case DERI_UP:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				SetAnim("�ʏ�U��_��");
				multiAttackNum = 0;
				break;

			case DERI_DOWN:
				deri = DERI_NON;
				fixCtrlCnt = 0;
				SetAnim("�ʏ�U��_���n�܂�");
				multiAttackNum = 0;
				break;

			case DERI_NON:
			default:
				deri = DERI_NON;
				if (GetAnim() == "�_���[�W")
				{
					fixCtrlCnt = 0;
					if (!spAttackHitFlag)
					{
						SetAnim("�ҋ@");
					}
				}
				else
				{
					SetAnim("�ҋ@");
				}
				multiAttackNum = 0;
				break;
			}

			if (multiAttackNum > 2)
			{
				fixCtrlCnt = 0;
				multiAttackNum = 0;	// 3�A���ŏI��
				SetAnim("�ҋ@");
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
		lpSoundMng.SetSound("�ޭ��");
	}
	if (spSeCnt == 100)
	{
		lpSoundMng.SetSound("�ޭ��2");
	}
}
