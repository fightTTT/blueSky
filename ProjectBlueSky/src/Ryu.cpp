#include "Ryu.h"
#include "GameCtrl.h"
#include "SceneMng.h"
#include "Shot.h"

Ryu::Ryu(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	auto ssize = lpSceneMng.GetScreenSize();

	characterName = "リュウ";

	AddAnim("波動拳", 0, 0, 11, 5, false);
	spAttackAnimName[0] = "波動拳";
	spAttackAnimFileName[0] = "hadoken";

	AddAnim("竜巻旋風脚", 0, 0, 13, 5, false);
	spAttackAnimName[1] = "竜巻旋風脚";
	spAttackAnimFileName[1] = "tatsumaki_senpukyaku";

	AddAnim("昇竜拳", 0, 0, 14, 5, false);
	spAttackAnimName[2] = "昇竜拳";
	spAttackAnimFileName[2] = "syoryuken";

	Init("image/リュウ/待機/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false);

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
				if ((GetAnim() == "しゃがみ始め") || (GetAnim() == "しゃがみ") || ((GetAnim() == "しゃがみ_後ろ")))
				{
					if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
					{
						if (dir == DIR_LEFT)
						{
							SetAnim("しゃがみ_後ろ");
						}
						else
						{
							SetAnim("しゃがみ");
						}
					}
					else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
					{
						if (dir == DIR_LEFT)
						{
							SetAnim("しゃがみ");
						}
						else
						{
							SetAnim("しゃがみ_後ろ");
						}
					}
					else
					{
						SetAnim("しゃがみ");
					}
				}
				else
				{
					SetAnim("しゃがみ始め");
				}
			}
			else
			{
				if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
				{
					pos.x += 4;
					if (dir == DIR_LEFT)
					{
						SetAnim("後ろ移動");
					}
					else
					{
						SetAnim("前移動");
					}
				}
				else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
				{
					pos.x -= 4;
					if (dir == DIR_LEFT)
					{
						SetAnim("前移動");
					}
					else
					{
						SetAnim("後ろ移動");
					}

				}
				else
				{
					SetAnim("待機");
				}
			}

			// 斜め左上ジャンプ
			if (ctl.GetPadData(padNum, THUMB_L_UP) && ctl.GetPadData(padNum, THUMB_L_LEFT))
			{
				jumpSpeed = { -2, -30 };
				jumpFlag = true;

				if (dir == DIR_LEFT)
				{
					SetAnim("ジャンプ_前");
				}
				else
				{
					SetAnim("ジャンプ_後ろ");
				}
			}
			// 斜め右上ジャンプ
			else if (ctl.GetPadData(padNum, THUMB_L_UP) && ctl.GetPadData(padNum, THUMB_L_RIGHT))
			{
				jumpSpeed = { 2, -30 };
				jumpFlag = true;

				if (dir == DIR_LEFT)
				{
					SetAnim("ジャンプ_後ろ");
				}
				else
				{
					SetAnim("ジャンプ_前");
				}
			}
			// 上ジャンプ
			else if (ctl.GetPadData(padNum, THUMB_L_UP))
			{
				jumpSpeed = { 0, -30 };
				jumpFlag = true;
				SetAnim("ジャンプ_上");
			}
			else if (ctl.GetPadData(padNum, BUTTON_X))
			{
				AddObjList()(objList, std::make_unique<Shot>(pos, dir));
				SetAnim("波動拳");
			}
			else if (ctl.GetPadData(padNum, BUTTON_A))
			{
				SetAnim("パンチ_小");
			}
			else if (ctl.GetPadData(padNum, BUTTON_B))
			{
				SetAnim("パンチ_大");
			}
		}
	}
	else
	{
		// ジャンプ中
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
