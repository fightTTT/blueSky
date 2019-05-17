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

	animAttributeTbl["�ҋ@"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�ҋ@"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�O�ړ�"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�O�ړ�"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���ړ�"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["���ړ�"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�W�����v_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�W�����v_��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�W�����v_�O"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�W�����v_�O"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�W�����v_���"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�W�����v_���"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["���Ⴊ��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���Ⴊ��_���"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["���Ⴊ��_���"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["���Ⴊ�ݎn��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["���Ⴊ�ݎn��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�����n��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�����n��"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�p���`_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�p���`_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�p���`_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�p���`_��_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�p���`_��_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�p���`_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�p���`_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�p���`_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�L�b�N_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�L�b�N_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�L�b�N_��_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��_��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�L�b�N_��_��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�L�b�N_��_���Ⴊ��"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["�K�[�h_����"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�K�[�h_����"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["�K�[�h_���Ⴊ��"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["�K�[�h_���Ⴊ��"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["�_���[�W_����"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�_���[�W_����"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["�_���[�W_�_�E��"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["�_���[�W_�_�E��"][1] = ANIM_ATTRIBUTE_INVINCIBLE;
	animAttributeTbl["�N���オ��"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["�N���オ��"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

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
		lpColMng.ColLoad(characterName, animName[i], animTable[animName[i]][ANIM_TBL_FRAME]);
	}
	// �K�E�Z�n
	if ((spAttackAnimName[0] != "�Z1") && (spAttackAnimFileName[0] != "waza_1"))
	{
		animFileName[spAttackAnimName[0]] = spAttackAnimFileName[0];
		animName.push_back(spAttackAnimName[0]);
		lpColMng.ColLoad(characterName, spAttackAnimName[0], animTable[spAttackAnimName[0]][ANIM_TBL_FRAME]);
	}
	if ((spAttackAnimName[1] != "�Z2") && (spAttackAnimFileName[1] != "waza_2"))
	{
		animFileName[spAttackAnimName[1]] = spAttackAnimFileName[1];
		animName.push_back(spAttackAnimName[1]);
		lpColMng.ColLoad(characterName, spAttackAnimName[1], animTable[spAttackAnimName[1]][ANIM_TBL_FRAME]);
	}
	if ((spAttackAnimName[2] != "�Z3") && (spAttackAnimFileName[2] != "waza_3"))
	{
		animFileName[spAttackAnimName[2]] = spAttackAnimFileName[2];
		animName.push_back(spAttackAnimName[2]);
		lpColMng.ColLoad(characterName, spAttackAnimName[2], animTable[spAttackAnimName[2]][ANIM_TBL_FRAME]);
	}

	// �ꊇ�ǂݍ���
	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	// comList�̏�����
	if (!comList.empty())
	{
		comList.clear();
	}

	comDir = COM_DIR_CENTER;
	comDirOld = COM_DIR_CENTER;
	comClearCnt = DEF_COM_CLEAR_CNT;

	playerHP = 100;

	return true;
}

bool Character::InitAnim(void)
{
	AddAnim("�ҋ@", 0, 0, 6, 5, true, 0, 0);
	AddAnim("�O�ړ�", 0, 0, 8, 5, true, 0, 0);
	AddAnim("���ړ�", 0, 0, 8, 5, true, 0, 0);
	AddAnim("�W�����v_��", 0, 0, 10, 6, true, 0, 0);
	AddAnim("�W�����v_�O", 0, 0, 6, 6, true, 0, 0);
	AddAnim("�W�����v_���", 0, 0, 6, 6, true, 0, 0);
	AddAnim("���Ⴊ��", 0, 0, 6, 5, true, 0, 0);
	AddAnim("���Ⴊ��_���", 0, 0, 1, 5, true, 0, 0);
	AddAnim("���Ⴊ�ݎn��", 0, 0, 3, 5, false, 0, 0);
	AddAnim("�����n��", 0, 0, 3, 5, false, 0, 0);
	AddAnim("�p���`_��", 0, 0, 7, 5, false, 0, 0);
	AddAnim("�p���`_��", 0, 0, 11, 5, false, 0, 0);
	AddAnim("�p���`_��_��", 0, 0, 4, 5, false, 0, 0);
	AddAnim("�p���`_��_��", 0, 0, 5, 5, false, 0, 0);
	AddAnim("�p���`_��_���Ⴊ��", 0, 0, 8, 5, false, 0, 0);
	AddAnim("�p���`_��_���Ⴊ��", 0, 0, 10, 5, false, 0, 0);
	AddAnim("�L�b�N_��", 0, 0, 9, 5, false, 64, 0);
	AddAnim("�L�b�N_��", 0, 0, 11, 5, false, 0, 0);
	AddAnim("�L�b�N_��_��", 0, 0, 4, 5, false, 0, 0);
	AddAnim("�L�b�N_��_��", 0, 0, 5, 5, false, 0, 0);
	AddAnim("�L�b�N_��_���Ⴊ��", 0, 0, 7, 5, false, 0, 0);
	AddAnim("�L�b�N_��_���Ⴊ��", 0, 0, 10, 5, false, 0, 0);
	AddAnim("�K�[�h_����", 0, 0, 1, 5, true, 0, 0);
	AddAnim("�K�[�h_���Ⴊ��", 0, 0, 1, 5, true, 0, 0);
	AddAnim("�_���[�W_����", 0, 0, 5, 5, false, 0, 0);
	AddAnim("�_���[�W_�_�E��", 0, 0, 13, 5, false, 0, 0);
	AddAnim("�N���オ��", 0, 0, 13, 5, false, 0, 0);
	SetAnim("�ҋ@");
	return true;
}

void Character::CommandUpDate(const GameCtrl & ctl)
{
	// ���͕����̏����X�V
	comDirOld = comDir;

	if (ctl.GetPadData(padID, THUMB_L_UP))
	{
		if (ctl.GetPadData(padID, THUMB_L_RIGHT))
		{
			comDir = COM_DIR_RIGHT_UP;
		}
		else if (ctl.GetPadData(padID, THUMB_L_LEFT))
		{
			comDir = COM_DIR_LEFT_UP;
		}
		else
		{
			comDir = COM_DIR_UP;
		}
	}
	else if (ctl.GetPadData(padID, THUMB_L_DOWN))
	{
		if (ctl.GetPadData(padID, THUMB_L_RIGHT))
		{
			comDir = COM_DIR_RIGHT_DOWN;
		}
		else if (ctl.GetPadData(padID, THUMB_L_LEFT))
		{
			comDir = COM_DIR_LEFT_DOWN;
		}
		else
		{
			comDir = COM_DIR_DOWN;
		}
	}
	else
	{
		if (ctl.GetPadData(padID, THUMB_L_RIGHT))
		{
			comDir = COM_DIR_RIGHT;
		}
		else if (ctl.GetPadData(padID, THUMB_L_LEFT))
		{
			comDir = COM_DIR_LEFT;
		}
		else
		{
			comDir = COM_DIR_CENTER;
		}
	}

	// ���X�g�ɓ����
	if (comDir != comDirOld)
	{
		if (comDirOld != COM_DIR_ACCUMULATE)
		{
			comList.push_back(comDir);
			comClearCnt = DEF_COM_CLEAR_CNT;
		}
	}
	else
	{
		if (comDir != COM_DIR_CENTER)
		{
			if (comClearCnt <= 0)
			{
				if (!comList.empty())
				{
					auto itr = comList.end();
					itr--;
					if ((*itr) != COM_DIR_ACCUMULATE)
					{
						comList.push_back(COM_DIR_ACCUMULATE);
					}
					comClearCnt = DEF_COM_CLEAR_CNT;
				}
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

		// �R�}���h���������߂�
		for (unsigned int buckCnt = 0; buckCnt < spAttackCommand[skillNum][dir].size(); buckCnt++)
		{
			itr--;
		}

		// ���Ԃɔ�r
		while (itr != comList.end())
		{
			if ((*itr) != spAttackCommand[skillNum][dir][comNum])
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

	if ((knockBackFlag) || (GetAnim() == "�_���[�W_����"))
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
	else if (GetAnim() == "�_���[�W_�_�E��")
	{
		fallSpeed.y += 1;
		pos.y += fallSpeed.y;
		if (pos.y > ssize.y)
		{
			pos.y = ssize.y;

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

		if (animEndFlag && (pos.y == ssize.y))
		{
			SetAnim("�N���オ��");
		}
	}
	else if (GetAnim() == "�N���オ��")
	{
		if (animEndFlag)
		{
			SetAnim("�ҋ@");
		}
	}
	else if (GetAnim() == "�g��")
	{
		if (animCnt == (6 * animTable["�g��"][ANIM_TBL_INV]))
		{
			if (dir == DIR_RIGHT)
			{
				AddObjList()(objList, std::make_unique<Shot>(pos + VECTOR2(256,64), drawOffset, dir, padID));
			}
			else
			{
				AddObjList()(objList, std::make_unique<Shot>(pos + VECTOR2(-128, 64), drawOffset, dir, padID));
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
			if (animTable[GetAnim()][ANIM_TBL_LOOP])
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

			if (pos.y > ssize.y)
			{
				pos.y = ssize.y;
				SetAnim("�ҋ@");
			}
		}
		else
		{
			if ((animTable[GetAnim()][ANIM_TBL_LOOP]) || animEndFlag)
			{
				dir = tmpDir;

				if (ctl.GetPadDataTrg(padID, THUMB_L_UP))		// �W�����v
				{
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
				else if (ctl.GetPadData(padID, THUMB_L_DOWN))		// ���Ⴊ��
				{
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
					if ((spAttackType[0] == SKILL_TYPE_PUNCH) && CheckCommand(0))
					{
						SetAnim(spAttackAnimName[0]);
					}
					else if ((spAttackType[1] == SKILL_TYPE_PUNCH) && CheckCommand(1))
					{
						SetAnim(spAttackAnimName[1]);
					}
					else if ((spAttackType[2] == SKILL_TYPE_PUNCH) && CheckCommand(2))
					{
						SetAnim(spAttackAnimName[2]);
					}
					else
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
				}
				else if (ctl.GetPadDataTrg(padID, BUTTON_B))
				{
					if ((spAttackType[0] == SKILL_TYPE_PUNCH) && CheckCommand(0))
					{
						SetAnim(spAttackAnimName[0]);
					}
					else if ((spAttackType[1] == SKILL_TYPE_PUNCH) && CheckCommand(1))
					{
						SetAnim(spAttackAnimName[1]);
					}
					else if ((spAttackType[2] == SKILL_TYPE_PUNCH) && CheckCommand(2))
					{
						SetAnim(spAttackAnimName[2]);
					}
					else
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
				}
				else if (ctl.GetPadDataTrg(padID, BUTTON_X))
				{
					if ((spAttackType[0] == SKILL_TYPE_KICK) && CheckCommand(0))
					{
						SetAnim(spAttackAnimName[0]);
					}
					else if ((spAttackType[1] == SKILL_TYPE_KICK) && CheckCommand(1))
					{
						SetAnim(spAttackAnimName[1]);
					}
					else if ((spAttackType[2] == SKILL_TYPE_KICK) && CheckCommand(2))
					{
						SetAnim(spAttackAnimName[2]);
					}
					else
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
				}
				else if (ctl.GetPadDataTrg(padID, BUTTON_Y))
				{
					if ((spAttackType[0] == SKILL_TYPE_KICK) && CheckCommand(0))
					{
						SetAnim(spAttackAnimName[0]);
					}
					else if ((spAttackType[1] == SKILL_TYPE_KICK) && CheckCommand(1))
					{
						SetAnim(spAttackAnimName[1]);
					}
					else if ((spAttackType[2] == SKILL_TYPE_KICK) && CheckCommand(2))
					{
						SetAnim(spAttackAnimName[2]);
					}
					else
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
				}
				else
				{
					// �������Ȃ�
				}
			}
		}
	}
}

void Character::CheckHitFlag(void)
{
	if (animAttribute[1] != ANIM_ATTRIBUTE_INVINCIBLE)
	{
		if (hitData.hitFlag && (hitData.colType == COLTYPE_HIT))
		{
			dir = tmpDir;

			if (GetAnim() != "�_���[�W_����")
			{
				comboCnt++;
				if ((comboCnt >= COMBO_BREAK_CNT) || (animAttribute[0] == ANIM_ATTRIBUTE_AIR))
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

	if (lpColMng.GetColFlag(animName))
	{
		ColInfo colData = lpColMng.GetCollisionData(characterName, animName, id);

		int colColor;

		for (int i = 0; i < colData.hitBox.size(); i++)
		{
			colData.hitBox[i].rect.startPos.x *= static_cast<int>(dir) * -2 + 1;
			colData.hitBox[i].rect.endPos.x *= static_cast<int>(dir) * -2 + 1;

			colColor = (colData.hitBox[i].type == COLTYPE_ATTACK ? 0xff0000 : (colData.hitBox[i].type == COLTYPE_HIT ? 0x0000ff : 0x00ff00));

			DrawBox(drawOffset.x + animOffset.x +  pos.x + (divSize.x / 2) + colData.hitBox[i].rect.startPos.x, drawOffset.y + animOffset.y + pos.y + divSize.y + colData.hitBox[i].rect.startPos.y, 
				drawOffset.x + animOffset.x + pos.x + (divSize.x / 2) + colData.hitBox[i].rect.endPos.x, drawOffset.y + animOffset.y + pos.y + divSize.y + colData.hitBox[i].rect.endPos.y, colColor, false);
		}
	}


	animCnt++;

	int i = 0;
	for (auto& data : comList)
	{
		switch (data)
		{
		case COM_DIR_CENTER:
			DrawString((i * 40) + 15, 200, "��", 0xffffff);
			break;
		case COM_DIR_UP:
			DrawString((i * 40) + 15, 200, "��", 0xffffff);
			break;
		case COM_DIR_RIGHT_UP:
			DrawString((i * 40) + 15, 200, "�E��", 0xffffff);
			break;
		case COM_DIR_RIGHT:
			DrawString((i * 40) + 15, 200, "�E", 0xffffff);
			break;
		case COM_DIR_RIGHT_DOWN:
			DrawString((i * 40) + 15, 200, "�E��", 0xffffff);
			break;
		case COM_DIR_DOWN:
			DrawString((i * 40) + 15, 200, "��", 0xffffff);
			break;
		case COM_DIR_LEFT_DOWN:
			DrawString((i * 40) + 15, 200, "����", 0xffffff);
			break;
		case COM_DIR_LEFT:
			DrawString((i * 40) + 15, 200, "��", 0xffffff);
			break;
		case COM_DIR_LEFT_UP:
			DrawString((i * 40) + 15, 200, "����", 0xffffff);
			break;
		case COM_DIR_ACCUMULATE:
			DrawString((i * 40) + 15, 200, "�^��", 0xffffff);
			break;
		default:
			break;
		}

		i++;
	}
}
