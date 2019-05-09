#include "Dxlib.h"
#include "GameCtrl.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "Shot.h"
#include "Character.h"

#define DEF_COM_CLEAR_CNT (60)


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

	jumpSpeed = { 0, 0 };
	jumpFlag = false;
	padNum = id;

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
	animFileName["�K�[�h_���Ⴊ��"] = "guard_squat";

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
										  "�K�[�h_���Ⴊ��" };		// ��Ұ��ݖ���v�f�Ƃ��Ď���vector

	// �K�E�Z�n
	if ((spAttackAnimName[0] != "�Z1") && (spAttackAnimFileName[0] != "waza_1"))
	{
		animFileName[spAttackAnimName[0]] = spAttackAnimFileName[0];
		animName.push_back(spAttackAnimName[0]);
	}
	if ((spAttackAnimName[1] != "�Z2") && (spAttackAnimFileName[1] != "waza_2"))
	{
		animFileName[spAttackAnimName[1]] = spAttackAnimFileName[1];
		animName.push_back(spAttackAnimName[1]);
	}
	if ((spAttackAnimName[2] != "�Z3") && (spAttackAnimFileName[2] != "waza_3"))
	{
		animFileName[spAttackAnimName[2]] = spAttackAnimFileName[2];
		animName.push_back(spAttackAnimName[2]);
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
	centerFlag = false;
	comClearCnt = DEF_COM_CLEAR_CNT;

	return true;
}

bool Character::InitAnim(void)
{
	AddAnim("�ҋ@", 0, 0, 10, 5, true);
	AddAnim("�O�ړ�", 0, 0, 11, 5, true);
	AddAnim("���ړ�", 0, 0, 11, 5, true);
	AddAnim("�W�����v_��", 0, 0, 12, 5, true);
	AddAnim("�W�����v_�O", 0, 0, 14, 5, true);
	AddAnim("�W�����v_���", 0, 0, 14, 5, true);
	AddAnim("���Ⴊ��", 0, 0, 4, 5, true);
	AddAnim("���Ⴊ��_���", 0, 0, 1, 5, true);
	AddAnim("���Ⴊ�ݎn��", 0, 0, 2, 5, false);
	AddAnim("�����n��", 0, 0, 2, 5, false);
	AddAnim("�p���`_��", 0, 0, 4, 5, false);
	AddAnim("�p���`_��", 0, 0, 7, 5, false);
	AddAnim("�K�[�h_���Ⴊ��", 0, 0, 1, 5, true);
	SetAnim("�ҋ@");
	return true;
}

bool Character::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_CHARACTER);
}

void Character::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	auto ssize = lpSceneMng.GetScreenSize();

	// �R�}���h�̊i�[����
	comDirOld = comDir;

	if (ctl.GetPadData(padNum, THUMB_L_UP))
	{
		if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
		{
			comDir = COM_DIR_RIGHT_UP;
		}
		else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
		{
			comDir = COM_DIR_LEFT_UP;
		}
		else
		{
			comDir = COM_DIR_UP;
		}
	}
	else if (ctl.GetPadData(padNum, THUMB_L_DOWN))
	{
		if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
		{
			comDir = COM_DIR_RIGHT_DOWN;
		}
		else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
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
		if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
		{
			comDir = COM_DIR_RIGHT;
		}
		else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
		{
			comDir = COM_DIR_LEFT;
		}
		else
		{
			comDir = COM_DIR_CENTER;
			centerFlag = true;
		}
	}

	if (comDir != COM_DIR_CENTER)
	{
		if ((comDir != comDirOld) || centerFlag)
		{
			comList.push_back(comDir);
			centerFlag = false;
			comClearCnt = DEF_COM_CLEAR_CNT;
		}
	}

	comClearCnt--;
	if ((!comList.empty()) && (comClearCnt < 0))
	{
		comList.clear();
	}

	// �L�����N�^�[����
	if (!jumpFlag)
	{
		dir = tmpDir;

		if (animTable[GetAnim()][ANIM_TBL_LOOP] || animEndFlag)
		{
			if (ctl.GetPadData(padNum, THUMB_L_UP))		// �W�����v
			{
				if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
				{
					jumpSpeed = { 2, -30 };
					jumpFlag = true;

					if (dir == DIR_LEFT)
					{
						SetAnim("�W�����v_���");
					}
					else
					{
						SetAnim("�W�����v_�O");
					}
				}
				else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
				{
					jumpSpeed = { -2, -30 };
					jumpFlag = true;

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
					jumpSpeed = { 0, -30 };
					jumpFlag = true;
					SetAnim("�W�����v_��");
				}
			}
			else if (ctl.GetPadData(padNum, THUMB_L_DOWN))		// ���Ⴊ��
			{
				if ((GetAnim() == "���Ⴊ�ݎn��") || (GetAnim() == "���Ⴊ��") || ((GetAnim() == "���Ⴊ��_���")))
				{
					if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
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
					else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
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
				if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
				{
					pos.x += 4;
					if (dir == DIR_LEFT)
					{
						SetAnim("���ړ�");
					}
					else
					{
						SetAnim("�O�ړ�");
					}
				}
				else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
				{
					pos.x -= 4;
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
			if (ctl.GetPadData(padNum, BUTTON_X))
			{
				AddObjList()(objList, std::make_unique<Shot>(pos, dir));
				SetAnim(spAttackAnimName[0]);
			}
			else if (ctl.GetPadData(padNum, BUTTON_A))
			{
				SetAnim("�p���`_��");
			}
			else if (ctl.GetPadData(padNum, BUTTON_B))
			{
				SetAnim("�p���`_��");
			}
		}
	}
	else
	{
		// �W�����v��
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	if (pos.y > ssize.y)
	{
		pos.y = ssize.y;
		jumpFlag = false;
	}
}

void Character::Draw(void)
{
	if (imageName.length() == 0)
	{
		return;
	}
	int id = 0;
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
	}

	std::string nowAnim = GetAnim();
	imageName = ("image/" + characterName + "/" + nowAnim + "/" + animFileName[nowAnim] + "_" + std::to_string(id) + ".png");

	// DIR��RIGHT�̏ꍇ�摜�𔽓]
	if (dir == DIR_RIGHT)
	{
		turnFlag = true;
	}
	else
	{
		turnFlag = false;
	}

	if (visible)
	{
		DrawRotaGraph(pos.x, pos.y - 178 / 2, 1.0, 0.0, IMAGE_ID(imageName)[0], true, turnFlag);
	}
	animCnt++;

	int i = 0;
	for (auto& data : comList)
	{
		switch (data)
		{
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
		default:
			break;
		}

		i++;
	}
}
