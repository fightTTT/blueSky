#include "Ryu.h"
#include "GameCtrl.h"
#include "SceneMng.h"
#include "Shot.h"

Ryu::Ryu(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	auto ssize = lpSceneMng.GetScreenSize();

	characterName = "�����E";

	AddAnim("�g����", 0, 0, 11, 5, false);
	spAttackAnimName[0] = "�g����";
	spAttackAnimFileName[0] = "hadoken";

	AddAnim("���������r", 0, 0, 13, 5, false);
	spAttackAnimName[1] = "���������r";
	spAttackAnimFileName[1] = "tatsumaki_senpukyaku";

	AddAnim("������", 0, 0, 14, 5, false);
	spAttackAnimName[2] = "������";
	spAttackAnimFileName[2] = "syoryuken";

	Init("image/�����E/�ҋ@/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false);

	jumpSpeed = { 0, 0 };
	jumpFlag = false;
	padNum = id;
	dir = charaDir;
}

Ryu::~Ryu()
{
}

void Ryu::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	auto ssize = lpSceneMng.GetScreenSize();

	if (!jumpFlag)
	{
		dir = tmpDir;

		if (animTable[GetAnim()][ANIM_TBL_LOOP] || animEndFlag)
		{
			if (ctl.GetPadData(padNum, THUMB_L_DOWN))
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

			// �΂ߍ���W�����v
			if (ctl.GetPadData(padNum, THUMB_L_UP) && ctl.GetPadData(padNum, THUMB_L_LEFT))
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
			// �΂߉E��W�����v
			else if (ctl.GetPadData(padNum, THUMB_L_UP) && ctl.GetPadData(padNum, THUMB_L_RIGHT))
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
			// ��W�����v
			else if (ctl.GetPadData(padNum, THUMB_L_UP))
			{
				jumpSpeed = { 0, -30 };
				jumpFlag = true;
				SetAnim("�W�����v_��");
			}
			else if (ctl.GetPadData(padNum, BUTTON_X))
			{
				AddObjList()(objList, std::make_unique<Shot>(pos, dir));
				SetAnim("�g����");
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

	//if (longAttackFlag)
	//{
	//	VECTOR2 speed;
	//	if (dir == DIR_LEFT)
	//	{
	//		speed = { -4, 0 };
	//	}
	//	else
	//	{
	//		speed = { 4, 0 };
	//	}
	//	longAttackPos += speed;
	//}
}
