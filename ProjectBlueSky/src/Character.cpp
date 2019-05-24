#include "Dxlib.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "Shot.h"
#include "Character.h"
#include "CollisionMng.h"

#define DEF_COM_CLEAR_CNT (60)

#define JUMP_SPEED_X (4)
#define JUMP_SPEED_Y (30)

#define KNOCK_BACK_SPEED (10)

#define FALL_SPEED_X (7)
#define FALL_SPEED_Y (15)

#define COMBO_BREAK_CNT (3)

Character::Character(VECTOR2 offset) : Obj(offset)
{
}

Character::~Character()
{
}

bool Character::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn, PAD_ID id)
{
	Obj::Init(fileName, divSize, divCut, pos, turn);
	InitAnim();

	ssize = lpSceneMng.GetScreenSize();
	jumpSpeed = { 0, 0 };
	fallSpeed = { 0, 0 };
	padID = id;
	hitData.hitFlag = false;
	comboCnt = 0;
	knockBackSpeed = 0;
	knockBackFlag = false;
	spEndCnt = 0;
	jumpInterval = 0;

	// �ʏ�̃A�N�V����
	animFileName["�ҋ@"] = "stand";
	animFileName["�O�ړ�"] = "move_front";
	animFileName["���ړ�"] = "move_back";
	animFileName["�W�����v_��"] = "jump";
	animFileName["�W�����v_�O"] = "jump_front";
	animFileName["�W�����v_���"] = "jump_back";
	animFileName["���Ⴊ��"] = "squat";
	animFileName["���Ⴊ��_���"] = "squat_back";
	animFileName["���Ⴊ�ݎn��"] = "squat_start";
	animFileName["�����n��"] = "stand_start";
	animFileName["�p���`_��"] = "punch_small";
	animFileName["�p���`_��"] = "punch_big";
	animFileName["�p���`_��_��"] = "punch_small_air";
	animFileName["�p���`_��_��"] = "punch_big_air";
	animFileName["�p���`_��_���Ⴊ��"] = "punch_small_squat";
	animFileName["�p���`_��_���Ⴊ��"] = "punch_big_squat";
	animFileName["�L�b�N_��"] = "kick_small";
	animFileName["�L�b�N_��"] = "kick_big";
	animFileName["�L�b�N_��_��"] = "kick_small_air";
	animFileName["�L�b�N_��_��"] = "kick_big_air";
	animFileName["�L�b�N_��_���Ⴊ��"] = "kick_small_squat";
	animFileName["�L�b�N_��_���Ⴊ��"] = "kick_big_squat";
	animFileName["�K�[�h_����"] = "guard";
	animFileName["�K�[�h_���Ⴊ��"] = "guard_squat";
	animFileName["�_���[�W_����"] = "damage";
	animFileName["�_���[�W_�_�E��"] = "damage_down";
	animFileName["�N���オ��"] = "get_up";

	// ��Ұ��݂̑�����ð���
	animAttributeTbl["�ҋ@"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�ҋ@"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�ҋ@"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�O�ړ�"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�O�ړ�"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["�O�ړ�"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["���ړ�"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["���ړ�"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["���ړ�"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�W�����v_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�W�����v_��"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["�W�����v_��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�W�����v_�O"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�W�����v_�O"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["�W�����v_�O"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�W�����v_���"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�W�����v_���"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["�W�����v_���"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["���Ⴊ��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���Ⴊ��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["���Ⴊ��_���"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["���Ⴊ��_���"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���Ⴊ��_���"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["���Ⴊ�ݎn��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["���Ⴊ�ݎn��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���Ⴊ�ݎn��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�����n��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�����n��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�����n��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�p���`_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�p���`_��"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["�p���`_��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�p���`_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�p���`_��"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["�p���`_��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�p���`_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�p���`_��_��"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["�p���`_��_��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�p���`_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�p���`_��_��"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["�p���`_��_��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�p���`_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�p���`_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["�p���`_��_���Ⴊ��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�p���`_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�p���`_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["�p���`_��_���Ⴊ��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�L�b�N_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�L�b�N_��"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["�L�b�N_��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�L�b�N_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�L�b�N_��"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["�L�b�N_��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�L�b�N_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�L�b�N_��_��"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["�L�b�N_��_��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�L�b�N_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�L�b�N_��_��"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["�L�b�N_��_��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�L�b�N_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�L�b�N_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�K�[�h_����"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�K�[�h_����"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["�K�[�h_����"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�K�[�h_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�K�[�h_���Ⴊ��"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["�K�[�h_���Ⴊ��"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�_���[�W_����"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�_���[�W_����"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�_���[�W_����"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["�_���[�W_�_�E��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�_���[�W_�_�E��"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["�_���[�W_�_�E��"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

	animAttributeTbl["�N���オ��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�N���オ��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�N���オ��"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

	std::vector<std::string> animName = { "�ҋ@",
										  "�O�ړ�",
										  "���ړ�",
										  "�W�����v_��",
										  "�W�����v_�O",
										  "�W�����v_���",
										  "���Ⴊ��",
										  "���Ⴊ��_���",
										  "���Ⴊ�ݎn��",
										  "�����n��",
										  "�p���`_��", 
										  "�p���`_��",
										  "�p���`_��_��",
										  "�p���`_��_��",
										  "�p���`_��_���Ⴊ��",
										  "�p���`_��_���Ⴊ��",
										  "�L�b�N_��",
										  "�L�b�N_��",
										  "�L�b�N_��_��",
										  "�L�b�N_��_��",
										  "�L�b�N_��_���Ⴊ��",
										  "�L�b�N_��_���Ⴊ��",
										  "�K�[�h_����",
										  "�K�[�h_���Ⴊ��",
										  "�_���[�W_����",
										  "�_���[�W_�_�E��",
										  "�N���オ��" };		// ��Ұ��ݖ���v�f�Ƃ��Ď���vector

	for (int i = 0; i < animName.size(); i++)
	{
		lpColMng.ColLoad("�_�l��", animName[i], animTable[animName[i]][ANIM_TBL_FRAME]);
	}
	// �K�E�Z�n
	if ((spAttackAnimName[0] != "�Z1") && (spAttackAnimFileName[0] != "waza_1"))
	{
		animFileName[spAttackAnimName[0]] = spAttackAnimFileName[0];
		animName.push_back(spAttackAnimName[0]);
		lpColMng.ColLoad("�_�l��", spAttackAnimName[0], animTable[spAttackAnimName[0]][ANIM_TBL_FRAME]);
	}
	if ((spAttackAnimName[1] != "�Z2") && (spAttackAnimFileName[1] != "waza_2"))
	{
		animFileName[spAttackAnimName[1]] = spAttackAnimFileName[1];
		animName.push_back(spAttackAnimName[1]);
		lpColMng.ColLoad("�_�l��", spAttackAnimName[1], animTable[spAttackAnimName[1]][ANIM_TBL_FRAME]);
	}
	if ((spAttackAnimName[2] != "�Z3") && (spAttackAnimFileName[2] != "waza_3"))
	{
		animFileName[spAttackAnimName[2]] = spAttackAnimFileName[2];
		animName.push_back(spAttackAnimName[2]);
		lpColMng.ColLoad("�_�l��", spAttackAnimName[2], animTable[spAttackAnimName[2]][ANIM_TBL_FRAME]);
	}

	// �ꊇ�ǂݍ���
	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	// comList�̏�����
	if (!comList.empty())
	{
		comList.clear();
	}

	spCom = SP_COM_CENTER;
	spComOld = SP_COM_CENTER;
	comClearCnt = DEF_COM_CLEAR_CNT;

	return true;
}

bool Character::InitAnim(void)
{
	AddAnim("�ҋ@", 0, 0, 6, 4, true, 0, 0);
	AddAnim("�O�ړ�", 0, 0, 8, 4, true, 0, 0);
	AddAnim("���ړ�", 0, 0, 8, 4, true, 0, 0);
	AddAnim("�W�����v_��", 0, 0, 10, 4, true, 0, 0);
	AddAnim("�W�����v_�O", 0, 0, 6, 4, true, 0, 0);
	AddAnim("�W�����v_���", 0, 0, 6, 4, true, 0, 0);
	AddAnim("���Ⴊ��", 0, 0, 1, 4, true, 0, 0);
	AddAnim("���Ⴊ��_���", 0, 0, 1, 4, true, 0, 0);
	AddAnim("���Ⴊ�ݎn��", 0, 0, 2, 4, false, 0, 0);
	AddAnim("�����n��", 0, 0, 3, 4, false, 0, 0);
	AddAnim("�p���`_��", 0, 0, 6, 4, false, 0, 0);
	AddAnim("�p���`_��", 0, 0, 8, 4, false, 0, 0);
	AddAnim("�p���`_��_��", 0, 0, 4, 4, false, 0, 0);
	AddAnim("�p���`_��_��", 0, 0, 5, 4, false, 0, 0);
	AddAnim("�p���`_��_���Ⴊ��", 0, 0, 6, 4, false, 0, 0);
	AddAnim("�p���`_��_���Ⴊ��", 0, 0, 9, 4, false, 0, 0);
	AddAnim("�L�b�N_��", 0, 0, 6, 4, false, 64, 0);
	AddAnim("�L�b�N_��", 0, 0, 8, 4, false, 0, 0);
	AddAnim("�L�b�N_��_��", 0, 0, 4, 4, false, 0, 0);
	AddAnim("�L�b�N_��_��", 0, 0, 5, 4, false, 0, 0);
	AddAnim("�L�b�N_��_���Ⴊ��", 0, 0, 6, 4, false, 0, 0);
	AddAnim("�L�b�N_��_���Ⴊ��", 0, 0, 9, 4, false, 0, 0);
	AddAnim("�K�[�h_����", 0, 0, 1, 4, true, 0, 0);
	AddAnim("�K�[�h_���Ⴊ��", 0, 0, 1, 4, true, 0, 0);
	AddAnim("�_���[�W_����", 0, 0, 5, 4, false, 0, 0);
	AddAnim("�_���[�W_�_�E��", 0, 0, 13, 4, false, 0, 0);
	AddAnim("�N���オ��", 0, 0, 10, 4, false, 0, 0);
	SetAnim("�ҋ@");
	return true;
}

void Character::CommandUpDate(const GameCtrl & ctl)
{
	if (comList.empty())
	{
		comList.push_back(SP_COM_CENTER);
	}

	// ���͕����̏����X�V
	spComOld = spCom;

	if (ctl.GetPadData(padID, THUMB_L_UP))
	{
		if (ctl.GetPadData(padID, THUMB_L_RIGHT))
		{
			spCom = SP_COM_RIGHT_UP;
		}
		else if (ctl.GetPadData(padID, THUMB_L_LEFT))
		{
			spCom = SP_COM_LEFT_UP;
		}
		else
		{
			spCom = SP_COM_UP;
		}
	}
	else if (ctl.GetPadData(padID, THUMB_L_DOWN))
	{
		if (ctl.GetPadData(padID, THUMB_L_RIGHT))
		{
			spCom = SP_COM_RIGHT_DOWN;
		}
		else if (ctl.GetPadData(padID, THUMB_L_LEFT))
		{
			spCom = SP_COM_LEFT_DOWN;
		}
		else
		{
			spCom = SP_COM_DOWN;
		}
	}
	else
	{
		if (ctl.GetPadData(padID, THUMB_L_RIGHT))
		{
			spCom = SP_COM_RIGHT;
		}
		else if (ctl.GetPadData(padID, THUMB_L_LEFT))
		{
			spCom = SP_COM_LEFT;
		}
		else
		{
			spCom = SP_COM_CENTER;
		}
	}

	if ((ctl.GetPadData(padID, BUTTON_A)) || (ctl.GetPadData(padID, BUTTON_B)))
	{
		spCom = SP_COM_PUNCH;
	}
	else
	{
		if ((ctl.GetPadData(padID, BUTTON_X)) || (ctl.GetPadData(padID, BUTTON_Y)))
		{
			spCom = SP_COM_KICK;
		}
	}

	// ���X�g�ɓ����
	if (spCom != spComOld)
	{
		if (spComOld != SP_COM_ACCUMULATE)
		{
			comList.push_back(spCom);
			comClearCnt = DEF_COM_CLEAR_CNT;
		}
	}
	else
	{
		if (comClearCnt <= 0)
		{
			if (!comList.empty())
			{
				auto itr = comList.end();
				itr--;
				if ((*itr) != SP_COM_ACCUMULATE)
				{
					comList.push_back(SP_COM_ACCUMULATE);
				}
				comClearCnt = DEF_COM_CLEAR_CNT;
			}
		}
	}

	// ���͎�t���Ԃ̏���
	if ((!comList.empty()) && (comClearCnt <= 0))
	{
		comList.clear();
	}
	comClearCnt--;
}

bool Character::CheckCommand(int skillNum)
{
	if (comList.size() < spAttackCommand[skillNum][dir].size())
	{
		return false;
	}
	else
	{
		auto itr = comList.end();
		int comNum = 0;

		SP_COM com;
		SP_COM leftSideCom;
		SP_COM rightSideCom;

		// �R�}���h���������߂�
		for (unsigned int buckCnt = 0; buckCnt < spAttackCommand[skillNum][dir].size(); buckCnt++)
		{
			itr--;
		}

		// ���Ԃɔ�r
		while (itr != comList.end())
		{
			com = (*itr);
			if ((com == SP_COM_CENTER)
			 || (com == SP_COM_ACCUMULATE)
			 || (com == SP_COM_PUNCH)
			 || (com == SP_COM_KICK))
			{
				leftSideCom = com;
				rightSideCom = com;
			}
			else
			{
				leftSideCom = (SP_COM)(com - 1);
				if (leftSideCom == SP_COM_CENTER)
				{
					leftSideCom = SP_COM_LEFT_UP;
				}

				rightSideCom = (SP_COM)(com + 1);
				if (rightSideCom == SP_COM_ACCUMULATE)
				{
					rightSideCom = SP_COM_UP;
				}
			}

			if ((com			 != spAttackCommand[skillNum][dir][comNum])
			 && (leftSideCom	 != spAttackCommand[skillNum][dir][comNum])
			 && (rightSideCom	 != spAttackCommand[skillNum][dir][comNum]))
			{
				break;
			}
			else
			{
				itr++;
				comNum++;

				// �R�}���h�������[�v�𔲂��Ȃ������琬��
				if (comNum == spAttackCommand[skillNum][dir].size())
				{
					comList.clear();
					return true;
				}
			}
		}
	}

	return false;
}

bool Character::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_CHARACTER);
}

void Character::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	CommandUpDate(ctl);

	if ((knockBackFlag) || (animName == "�_���[�W_����"))
	{
		if (dir == DIR_RIGHT)
		{
			knockBackSpeed++;
			if (knockBackSpeed > 0)
			{
				knockBackSpeed = 0;
			}
		}
		else
		{
			knockBackSpeed--;
			if (knockBackSpeed < 0)
			{
				knockBackSpeed = 0;
			}
		}
		pos.x += knockBackSpeed;

		if (knockBackSpeed == 0)
		{
			knockBackFlag = false;
			if (animEndFlag)
			{
				SetAnim("�ҋ@");
			}
		}
	}
	else if (animName == "�_���[�W_�_�E��")
	{
		fallSpeed.y++;
		pos.y += fallSpeed.y;

		if (pos.y > ssize.y)
		{
			if (dir == DIR_RIGHT)
			{
				fallSpeed.x++;
				if (fallSpeed.x > 0)
				{
					fallSpeed.x = 0;
				}
			}
			else
			{
				fallSpeed.x--;
				if (fallSpeed.x < 0)
				{
					fallSpeed.x = 0;
				}
			}
		}

		pos.x += fallSpeed.x;

		if (animEndFlag && playerHP != 0)
		{
			SetAnim("�N���オ��");
		}
	}
	else if (animName == "�N���オ��")
	{
		if (animEndFlag)
		{
			SetAnim("�ҋ@");
		}
	}
	else if (animAttribute[2] == ANIM_ATTRIBUTE_SHOT)
	{
		if (animCnt == shotCreateCnt)
		{
			if (dir == DIR_RIGHT)
			{
				AddObjList()(objList, std::make_unique<Shot>(pos + VECTOR2(256 - 30,64), drawOffset, dir, padID));
			}
			else
			{
				AddObjList()(objList, std::make_unique<Shot>(pos + VECTOR2(-128 + 30, 64), drawOffset, dir, padID));
			}
		}

		if (animEndFlag)
		{
			SetAnim("�ҋ@");
		}
	}
	else if (animName == "���[�����O�A�^�b�N")
	{
		if (animCnt > 20)
		{
			if (dir == DIR_RIGHT)
			{
				pos.x += 20;
			}
			else
			{
				pos.x -= 20;
			}
		}

		if (animCnt > 60)
		{
			SetAnim("�ҋ@");
		}
	}
	else if (animName == "�����r")
	{
		if (dir == DIR_RIGHT)
		{
			pos.x += 6;
		}
		else
		{
			pos.x -= 6;
		}

		if (animCnt > 60)
		{
			SetAnim("�ҋ@");
		}
	}
	else if (animName == "�J���t�[�L�b�N")
	{
		if (animCnt < 4)
		{
			if (dir == DIR_RIGHT)
			{
				pos.x += 6;
			}
			else
			{
				pos.x -= 6;
			}
		}

		if (animEndFlag)
		{
			SetAnim("�ҋ@");
		}
	}
	else if (animName == "���b�V��")
	{
		if (animCnt > 40)
		{
			SetAnim("�ҋ@");
		}
	}
	else if ((animName == "�~�T�C���A���[") || (animName == "�^�b�N��"))
	{
		if (animStopFlag)
		{
			spEndCnt++;

			if (spEndCnt < 45)
			{
				if (spEndCnt > 4)
				{
					if (dir == DIR_RIGHT)
					{
						pos.x += 20;
					}
					else
					{
						pos.x -= 20;
					}
				}
			}
			else
			{
				animStopFlag = false;
			}
		}
		else
		{
			if (animCnt == 9)
			{
				animStopFlag = true;
				spEndCnt = 0;
			}
		}

		if (animEndFlag)
		{
			SetAnim("�ҋ@");
		}
	}
	else if (animName == "���[�v")
	{
		if (animCnt == 31)
		{
			animStopFlag = true;
		}

		if (animStopFlag)
		{
			pos.y = (ssize.y * 2);

			if (dir == DIR_RIGHT)
			{
				if (pos.x > ((ssize.x * 3) / 5))
				{
					dir = tmpDir;
					animStopFlag = false;
				}
				else
				{
					pos.x += 30;
				}
			}
			else
			{
				if (pos.x < ((ssize.x * 2) / 5))
				{
					dir = tmpDir;
					animStopFlag = false;
				}
				else
				{
					pos.x -= 30;
				}
			}
		}

		if (animEndFlag)
		{
			SetAnim("�ҋ@");
		}
	}
	else if (animName == "�R��Ԃ�")
	{
		if (animCnt < 20)
		{
			if (dir == DIR_RIGHT)
			{
				pos.x += 8;
			}
			else
			{
				pos.x -= 8;
			}

			if (animCnt < 10)
			{
				pos.y -= 5;
			}
			else
			{
				pos.y += 5;
			}
		}
		else
		{
			// �U��������������ȉ��̂悤�ɖ߂��Ă��鏈�������A������Ȃ������炻�̂܂ܒn�ʂɗ����Ă��܂��悤�ɂ���
			if (dir == DIR_RIGHT)
			{
				pos.x -= 8;
			}
			else
			{
				pos.x += 8;
			}

			if (animCnt < 33)
			{
				pos.y -= 8;
			}
			else
			{
				pos.y += 8;
			}
		}

		if (animEndFlag)
		{
			SetAnim("�ҋ@");
		}
	}
	else if (animName == "�A�N�Z��")
	{
		if (animCnt < 60)
		{
			if (dir == DIR_RIGHT)
			{
				pos.x += 5;
			}
			else
			{
				pos.x -= 5;
			}
		}
		

		if (animEndFlag)
		{
			SetAnim("�ҋ@");
		}
	}
	else
	{
		// �L�����N�^�[����
		if (animAttribute[0] == ANIM_ATTRIBUTE_AIR)
		{
			if (animName == "�����L���N")
			{
				if (dir == DIR_RIGHT)
				{
					pos.x++;
				}
				else
				{
					pos.x--;
				}

				pos.y--;

				if (animEndFlag)
				{
					SetAnim("�ҋ@");
				}
			}
			else if ((animName == "�n�ʊ�") || (animName == "�����Ɨ��Ƃ�"))		// �n�ʊ��ʹ̪�Ă��t������Ԃ̉摜��p�ӂ��Ă���ɓ����蔻���t����K�v����
			{
				if (animCnt < 30)
				{
					if (dir == DIR_RIGHT)
					{
						pos.x++;
					}
					else
					{
						pos.x--;
					}

					pos.y -= 8;
				}
				else
				{
					pos.y += 20;
				}

				if (animEndFlag)
				{
					SetAnim("�ҋ@");
				}
			}
			else
			{
				if (animTable[animName][ANIM_TBL_LOOP])
				{
					if (ctl.GetPadDataTrg(padID, BUTTON_A))
					{
						if (ctl.GetPadData(padID, THUMB_L_RIGHT))
						{
							dir = DIR_RIGHT;
						}
						else if (ctl.GetPadData(padID, THUMB_L_LEFT))
						{
							dir = DIR_LEFT;
						}
						else
						{
							// ���̂܂܂̕���
						}
						SetAnim("�p���`_��_��");
					}
					else if (ctl.GetPadDataTrg(padID, BUTTON_B))
					{
						if (ctl.GetPadData(padID, THUMB_L_RIGHT))
						{
							dir = DIR_RIGHT;
						}
						else if (ctl.GetPadData(padID, THUMB_L_LEFT))
						{
							dir = DIR_LEFT;
						}
						else
						{
							// ���̂܂܂̕���
						}
						SetAnim("�p���`_��_��");
					}
					else if (ctl.GetPadDataTrg(padID, BUTTON_X))
					{
						if (ctl.GetPadData(padID, THUMB_L_RIGHT))
						{
							dir = DIR_RIGHT;
						}
						else if (ctl.GetPadData(padID, THUMB_L_LEFT))
						{
							dir = DIR_LEFT;
						}
						else
						{
							// ���̂܂܂̕���
						}
						SetAnim("�L�b�N_��_��");
					}
					else if (ctl.GetPadDataTrg(padID, BUTTON_Y))
					{
						if (ctl.GetPadData(padID, THUMB_L_RIGHT))
						{
							dir = DIR_RIGHT;
						}
						else if (ctl.GetPadData(padID, THUMB_L_LEFT))
						{
							dir = DIR_LEFT;
						}
						else
						{
							// ���̂܂܂̕���
						}
						SetAnim("�L�b�N_��_��");
					}
					else
					{
						// �������Ȃ�
					}
				}

				// �W�����v��
				jumpSpeed.y += 1;
				pos += jumpSpeed;

				if (jumpSpeed.y == JUMP_SPEED_Y)
				{
					SetAnim("�ҋ@");
				}
			}
		}
		else
		{
			if ((animTable[animName][ANIM_TBL_LOOP]) || animEndFlag)
			{
				dir = tmpDir;

				if (CheckCommand(2))
				{
					SetAnim(spAttackAnimName[2]);
				}
				else if (CheckCommand(1))
				{
					SetAnim(spAttackAnimName[1]);
				}
				else if (CheckCommand(0))
				{
					SetAnim(spAttackAnimName[0]);
				}
				else
				{
					if (ctl.GetPadData(padID, THUMB_L_UP))		// �W�����v
					{
						if (jumpInterval == 0)
						{
							if (ctl.GetPadDataTrg(padID, THUMB_L_UP))
							{
								jumpInterval++;
							}
						}
						else
						{
							jumpInterval++;

							if (jumpInterval > 4)
							{
								jumpInterval = 0;

								if (ctl.GetPadData(padID, THUMB_L_RIGHT))
								{
									jumpSpeed = { JUMP_SPEED_X, -JUMP_SPEED_Y };

									if (dir == DIR_LEFT)
									{
										SetAnim("�W�����v_���");
									}
									else
									{
										SetAnim("�W�����v_�O");
									}
								}
								else if (ctl.GetPadData(padID, THUMB_L_LEFT))
								{
									jumpSpeed = { -JUMP_SPEED_X, -JUMP_SPEED_Y };

									if (dir == DIR_LEFT)
									{
										SetAnim("�W�����v_�O");
									}
									else
									{
										SetAnim("�W�����v_���");
									}
								}
								else
								{
									jumpSpeed = { 0, -JUMP_SPEED_Y };
									SetAnim("�W�����v_��");
								}
							}
						}
					}
					else if (ctl.GetPadData(padID, THUMB_L_DOWN))		// ���Ⴊ��
					{
						jumpInterval = 0;

						if (animAttribute[0] == ANIM_ATTRIBUTE_SQUAT)
						{
							if (ctl.GetPadData(padID, THUMB_L_RIGHT))
							{
								if (dir == DIR_LEFT)
								{
									SetAnim("���Ⴊ��_���");
								}
								else
								{
									SetAnim("���Ⴊ��");
								}
							}
							else if (ctl.GetPadData(padID, THUMB_L_LEFT))
							{
								if (dir == DIR_LEFT)
								{
									SetAnim("���Ⴊ��");
								}
								else
								{
									SetAnim("���Ⴊ��_���");
								}
							}
							else
							{
								SetAnim("���Ⴊ��");
							}
						}
						else
						{
							SetAnim("���Ⴊ�ݎn��");
						}
					}
					else
					{
						jumpInterval = 0;

						// �ړ�
						if (ctl.GetPadData(padID, THUMB_L_RIGHT))
						{
							if (animAttribute[1] != ANIM_ATTRIBUTE_GUARD)
							{
								pos.x += 4;
							}
							if (dir == DIR_LEFT)
							{
								SetAnim("���ړ�");
							}
							else
							{
								SetAnim("�O�ړ�");
							}
						}
						else if (ctl.GetPadData(padID, THUMB_L_LEFT))
						{
							if (animAttribute[1] != ANIM_ATTRIBUTE_GUARD)
							{
								pos.x -= 4;
							}
							if (dir == DIR_LEFT)
							{
								SetAnim("�O�ړ�");
							}
							else
							{
								SetAnim("���ړ�");
							}

						}
						else
						{
							SetAnim("�ҋ@");
						}
					}

					// �U��
					if (ctl.GetPadDataTrg(padID, BUTTON_A))
					{
						if (ctl.GetPadData(padID, THUMB_L_DOWN))
						{
							SetAnim("�p���`_��_���Ⴊ��");
						}
						else
						{
							SetAnim("�p���`_��");
						}
					}
					else if (ctl.GetPadDataTrg(padID, BUTTON_B))
					{
						if (ctl.GetPadData(padID, THUMB_L_DOWN))
						{
							SetAnim("�p���`_��_���Ⴊ��");
						}
						else
						{
							SetAnim("�p���`_��");
						}
					}
					else if (ctl.GetPadDataTrg(padID, BUTTON_X))
					{
						if (ctl.GetPadData(padID, THUMB_L_DOWN))
						{
							SetAnim("�L�b�N_��_���Ⴊ��");
						}
						else
						{
							SetAnim("�L�b�N_��");
						}
					}
					else if (ctl.GetPadDataTrg(padID, BUTTON_Y))
					{
						if (ctl.GetPadData(padID, THUMB_L_DOWN))
						{
							SetAnim("�L�b�N_��_���Ⴊ��");
						}
						else
						{
							SetAnim("�L�b�N_��");
						}
					}
					else
					{
						// �������Ȃ�
					}
				}
			}
		}
	}
}

void Character::CheckHitFlag(void)
{
	if (animAttribute[2] != ANIM_ATTRIBUTE_INVINCIBLE)
	{
		if (hitData.hitFlag && (hitData.colType == COLTYPE_HIT))
		{
			dir = tmpDir;

			if (animName != "�_���[�W_����")
			{
				AddPlayerHP(-10);
				WaitTimer(65);

				comboCnt++;
				if ((comboCnt >= COMBO_BREAK_CNT) || (animAttribute[0] == ANIM_ATTRIBUTE_AIR) || (playerHP <= 0))
				{
					comboCnt = 0;

					SetAnim("�_���[�W_�_�E��");

					if (dir == DIR_RIGHT)
					{
						fallSpeed = { -FALL_SPEED_X, -FALL_SPEED_Y };
					}
					else
					{
						fallSpeed = { FALL_SPEED_X, -FALL_SPEED_Y };
					}

					pos += fallSpeed;
					if (pos.y > ssize.y)
					{
						pos.y = ssize.y;
					}
				}
				else
				{
					SetAnim("�_���[�W_����");
					if (dir == DIR_RIGHT)
					{
						knockBackSpeed = -KNOCK_BACK_SPEED;
					}
					else
					{
						knockBackSpeed = KNOCK_BACK_SPEED;
					}
					pos.x += knockBackSpeed;
				}
			}
		}
	}

	if (animAttribute[1] == ANIM_ATTRIBUTE_GUARD)
	{
		if (hitData.hitFlag && (hitData.colType == COLTYPE_GUARD))
		{
			if (!knockBackFlag)
			{
				if (dir == DIR_RIGHT)
				{
					knockBackSpeed = -KNOCK_BACK_SPEED;
				}
				else
				{
					knockBackSpeed = KNOCK_BACK_SPEED;
				}
				pos.x += knockBackSpeed;
				knockBackFlag = true;
			}
		}
	}
}

void Character::Draw(void)
{
	if (imageName.length() == 0)
	{
		return;
	}
	int id = 0;
	VECTOR2 animOffset(0, 0);
	if (animTable.find(animName) != animTable.end())
	{
		id = animCnt / animTable[animName][ANIM_TBL_INV];
		if (animTable[animName][ANIM_TBL_LOOP] || id < animTable[animName][ANIM_TBL_FRAME])
		{
			id %= animTable[animName][ANIM_TBL_FRAME];
		}
		else
		{
			id = animTable[animName][ANIM_TBL_FRAME] - 1;
			animEndFlag = true;
		}

		if (dir == DIR_RIGHT)
		{
			animOffset = { animTable[animName][ANIM_TBL_OFFSET_X] , animTable[animName][ANIM_TBL_OFFSET_Y] };
		}
		else
		{
			animOffset = { -animTable[animName][ANIM_TBL_OFFSET_X] , animTable[animName][ANIM_TBL_OFFSET_Y] };
		}
	}

	std::string nowAnim = GetAnim();
	imageName = ("image/" + characterName + "/" + nowAnim + "/" + animFileName[nowAnim] + "_" + std::to_string(id) + ".png");

	// DIR��RIGHT�̏ꍇ�摜�𔽓]
	if (dir == DIR_RIGHT)
	{
		turnFlag = false;
	}
	else
	{
		turnFlag = true;
	}

	if (visible)
	{
		DrawRotaGraph(drawOffset.x + animOffset.x + pos.x + (divSize.x / 2), drawOffset.y + animOffset.y + pos.y + (divSize.y / 2), 1.0, 0.0, IMAGE_ID(imageName)[0], true, turnFlag);
	}

	/*if (lpColMng.GetColFlag(animName))
	{
		ColInfo colData = lpColMng.GetCollisionData("�_�l��", animName, id);

		int colColor;

		for (int i = 0; i < colData.hitBox.size(); i++)
		{
			colData.hitBox[i].rect.startPos.x *= static_cast<int>(dir) * -2 + 1;
			colData.hitBox[i].rect.endPos.x *= static_cast<int>(dir) * -2 + 1;

			colColor = (colData.hitBox[i].type == COLTYPE_ATTACK ? 0xff0000 : (colData.hitBox[i].type == COLTYPE_HIT ? 0x0000ff : 0x00ff00));

			DrawBox(drawOffset.x + animOffset.x + pos.x + (divSize.x / 2) + colData.hitBox[i].rect.startPos.x, drawOffset.y + animOffset.y + pos.y + divSize.y + colData.hitBox[i].rect.startPos.y, ]
				drawOffset.x + animOffset.x + pos.x + (divSize.x / 2) + colData.hitBox[i].rect.endPos.x, drawOffset.y + animOffset.y + pos.y + divSize.y + colData.hitBox[i].rect.endPos.y, colColor, false);
		}
	}*/

	int hpColor = 0;

	if (playerHP != playerHPOld)
	{
		DrawHPCount += 0.2f;

		playerHPOld -= static_cast<int>(DrawHPCount);

		if (static_cast<int>(DrawHPCount) == 1)
		{
			DrawHPCount = 0.0f;
		}
	}

	// HP�\��
	if (playerHP <= 50 && playerHP > 25)
	{
		hpColor = 0xffd900;
	}
	else if (playerHP <= 25)
	{
		hpColor = 0xff0000;
	}
	else
	{
		hpColor = 0x00ff00;
	}

	if (padID == PAD_1)
	{
		DrawFormatString(110, 25, 0xffffff, "player�̎c��HP %d \n", playerHP);
		DrawBox(100, 65, 100 + 300, 85, 0x000000, true);
		DrawBox(100 + (300 - playerHPOld * 3), 65, 100 + 300 /*+ (playerHPOld * 3)*/, 85, 0xff0000, true);
		DrawBox(100 + (300 - playerHP * 3), 65, 100 + 300 /*+ (playerHP * 3)*/, 85, hpColor, true);
		DrawGraph(70, 55, IMAGE_ID("image/�Q�[���V�[���p/hpGage.png")[0], true);
		DrawGraph(0, 0, IMAGE_ID("image/�Q�[���V�[���p/iconFrame1.png")[0], true);
		DrawGraph(25, 10, IMAGE_DIV_ID("image/�L�����Z���p/charIcon_small.png", VECTOR2(100, 100), VECTOR2(8, 1))[lpSceneMng.GetCharID(padID)], true);
	}
	else if (padID == PAD_2)
	{
		DrawFormatString(ssize.x - 410, 25, 0xffffff, "AI�̎c��HP %d \n", playerHP);
		DrawBox(ssize.x - 410, 65, ssize.x - 410 + 300, 85, 0x000000, true);
		DrawBox(ssize.x - 410, 65, ssize.x - 410 + (playerHPOld * 3), 85, 0xff0000, true);
		DrawBox(ssize.x - 410, 65, ssize.x - 410 + (playerHP * 3), 85, hpColor, true);
		DrawTurnGraph((ssize.x - 430), 55, IMAGE_ID("image/�Q�[���V�[���p/hpGage.png")[0], true);
		DrawGraph((ssize.x-130), 0, IMAGE_ID("image/�Q�[���V�[���p/iconFrame2.png")[0], true);
		DrawTurnGraph(ssize.x - 400 + 275, 10, IMAGE_DIV_ID("image/�L�����Z���p/charIcon_small.png", VECTOR2(100, 100), VECTOR2(8, 1))[lpSceneMng.GetCharID(padID)], true);
	}
	else
	{
		// �Ȃɂ����Ȃ�
	}

	if (!animStopFlag)
	{
		animCnt++;
	}

	int i = 0;
	for (auto& data : comList)
	{
		switch (data)
		{
		case SP_COM_CENTER:
			DrawString((i * 40) + 15, 200, "��", 0xffffff);
			break;
		case SP_COM_UP:
			DrawString((i * 40) + 15, 200, "��", 0xffffff);
			break;
		case SP_COM_RIGHT_UP:
			DrawString((i * 40) + 15, 200, "�E��", 0xffffff);
			break;
		case SP_COM_RIGHT:
			DrawString((i * 40) + 15, 200, "�E", 0xffffff);
			break;
		case SP_COM_RIGHT_DOWN:
			DrawString((i * 40) + 15, 200, "�E��", 0xffffff);
			break;
		case SP_COM_DOWN:
			DrawString((i * 40) + 15, 200, "��", 0xffffff);
			break;
		case SP_COM_LEFT_DOWN:
			DrawString((i * 40) + 15, 200, "����", 0xffffff);
			break;
		case SP_COM_LEFT:
			DrawString((i * 40) + 15, 200, "��", 0xffffff);
			break;
		case SP_COM_LEFT_UP:
			DrawString((i * 40) + 15, 200, "����", 0xffffff);
			break;
		case SP_COM_ACCUMULATE:
			DrawString((i * 40) + 15, 200, "�^��", 0xffffff);
			break;
		case SP_COM_PUNCH:
			DrawString((i * 40) + 15, 200, "P", 0xffffff);
			break;
		case SP_COM_KICK:
			DrawString((i * 40) + 15, 200, "K", 0xffffff);
			break;
		default:
			break;
		}

		i++;
	}
}
