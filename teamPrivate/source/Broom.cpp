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

	Obj::init("�|���");
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
	loadImageAndCol("�ʏ�U��_����_�n��", "attackDown", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�ʏ�U��_����_��", "attackDown", 3, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("�_���[�W", "damage", 45, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("�K�E�Z", "specialAttack", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("���ߍU��", "chargeAttack", 5, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("���ߎn�܂�", "chargeStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("���ߒ�", "charge", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("�{��Q�[�W����", "angryCharge", 1, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("���", "avoid", 10, false, ANIM_TYPE_ATTACK);

	SetAnim("�ҋ@");

	lpSoundMng.SoundInit("sound/SE��/Chicken1.mp3", "������");

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
	if (GetAnim() != "�K�E�Z" && !spAttackHitFlag && GetAnim() != "�ʏ�U��_��")
	{
		// ���W�␳
		PosAdjust(tmpPos, maxEndPosY);

		// ���R����
		if (vY > 0.0 && locate == LOCATE::NON)
		{
			param = PARAM::FALLING;
		}

		vY += gravity;

		// ���x��pos�����Z
		tmpPos.y += static_cast<int>(vY) * (static_cast<int>(fallFlag && param == PARAM::FALLING) + 1);
		tmpPos.x += static_cast<int>(vX);
	}
}

void Broom::UniqueAttack(VECTOR2 & tmpPos)
{
	if (GetAnim() == "�ʏ�U��_��")
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
		// �ʏ�U���n��Ұ��݂��I��������ҋ@��Ұ��݂ֈڍs
		else
		{
			if (!jumpCnt && deri == DERI_UP)
			{
				deri = DERI_NOMAL;	// jumpCnt��0�Ȃ�NOMAL�̔h���U�����s��
			}
			fixCtrlCnt = FIX_TIME;

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
				downFlag = false;
				param = PARAM::JUMPING;
				floorCheckFlag = false;
				jumpCnt--;
				locate = LOCATE::NON;
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

void Broom::PlaySPSe(void)
{
	if (spSeCnt == 70)
	{
		lpSoundMng.SetSound("������");
	}
}

int Broom::GetJumpCntMax(void)
{
	return JUMP_CNT_MAX;
}
