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

	// 通常のアクション
	animFileName["待機"] = "stand";
	animFileName["前移動"] = "move_front";
	animFileName["後ろ移動"] = "move_back";
	animFileName["ジャンプ_上"] = "jump";
	animFileName["ジャンプ_前"] = "jump_front";
	animFileName["ジャンプ_後ろ"] = "jump_back";
	animFileName["しゃがみ"] = "squat";
	animFileName["しゃがみ_後ろ"] = "squat_back";
	animFileName["しゃがみ始め"] = "squat_start";
	animFileName["立ち始め"] = "stand_start";
	animFileName["パンチ_小"] = "punch_small";
	animFileName["パンチ_大"] = "punch_big";
	animFileName["パンチ_小_空中"] = "punch_small_air";
	animFileName["パンチ_大_空中"] = "punch_big_air";
	animFileName["パンチ_小_しゃがみ"] = "punch_small_squat";
	animFileName["パンチ_大_しゃがみ"] = "punch_big_squat";
	animFileName["キック_小"] = "kick_small";
	animFileName["キック_大"] = "kick_big";
	animFileName["キック_小_空中"] = "kick_small_air";
	animFileName["キック_大_空中"] = "kick_big_air";
	animFileName["キック_小_しゃがみ"] = "kick_small_squat";
	animFileName["キック_大_しゃがみ"] = "kick_big_squat";
	animFileName["ガード_立ち"] = "guard";
	animFileName["ガード_しゃがみ"] = "guard_squat";
	animFileName["ダメージ_立ち"] = "damage";
	animFileName["ダメージ_ダウン"] = "damage_down";
	animFileName["起き上がり"] = "get_up";

	animAttributeTbl["待機"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["待機"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["前移動"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["前移動"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["後ろ移動"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["後ろ移動"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["ジャンプ_上"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["ジャンプ_上"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["ジャンプ_前"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["ジャンプ_前"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["ジャンプ_後ろ"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["ジャンプ_後ろ"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["しゃがみ"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["しゃがみ_後ろ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["しゃがみ_後ろ"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["しゃがみ始め"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["しゃがみ始め"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["立ち始め"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["立ち始め"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["パンチ_小"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["パンチ_小"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["パンチ_大"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["パンチ_大"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["パンチ_小_空中"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["パンチ_小_空中"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["パンチ_大_空中"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["パンチ_大_空中"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["パンチ_小_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["パンチ_小_しゃがみ"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["パンチ_大_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["パンチ_大_しゃがみ"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["キック_小"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["キック_小"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["キック_大"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["キック_大"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["キック_小_空中"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["キック_小_空中"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["キック_大_空中"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["キック_大_空中"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["キック_小_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["キック_小_しゃがみ"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["キック_大_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["キック_大_しゃがみ"][1] = ANIM_ATTRIBUTE_ATTACK;
	animAttributeTbl["ガード_立ち"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["ガード_立ち"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["ガード_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["ガード_しゃがみ"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["ダメージ_立ち"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["ダメージ_立ち"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["ダメージ_ダウン"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["ダメージ_ダウン"][1] = ANIM_ATTRIBUTE_INVINCIBLE;
	animAttributeTbl["起き上がり"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["起き上がり"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

	std::vector<std::string> animName = { "待機",
										  "前移動",
										  "後ろ移動",
										  "ジャンプ_上",
										  "ジャンプ_前",
										  "ジャンプ_後ろ",
										  "しゃがみ",
										  "しゃがみ_後ろ",
										  "しゃがみ始め",
										  "立ち始め",
										  "パンチ_小", 
										  "パンチ_大",
										  "パンチ_小_空中",
										  "パンチ_大_空中",
										  "パンチ_小_しゃがみ",
										  "パンチ_大_しゃがみ",
										  "キック_小",
										  "キック_大",
										  "キック_小_空中",
										  "キック_大_空中",
										  "キック_小_しゃがみ",
										  "キック_大_しゃがみ",
										  "ガード_立ち",
										  "ガード_しゃがみ",
										  "ダメージ_立ち",
										  "ダメージ_ダウン",
										  "起き上がり" };		// ｱﾆﾒｰｼｮﾝ名を要素として持つvector

	for (int i = 0; i < animName.size(); i++)
	{
		lpColMng.ColLoad(characterName, animName[i], animTable[animName[i]][ANIM_TBL_FRAME]);
	}
	// 必殺技系
	if ((spAttackAnimName[0] != "技1") && (spAttackAnimFileName[0] != "waza_1"))
	{
		animFileName[spAttackAnimName[0]] = spAttackAnimFileName[0];
		animName.push_back(spAttackAnimName[0]);
		lpColMng.ColLoad(characterName, spAttackAnimName[0], animTable[spAttackAnimName[0]][ANIM_TBL_FRAME]);
	}
	if ((spAttackAnimName[1] != "技2") && (spAttackAnimFileName[1] != "waza_2"))
	{
		animFileName[spAttackAnimName[1]] = spAttackAnimFileName[1];
		animName.push_back(spAttackAnimName[1]);
		lpColMng.ColLoad(characterName, spAttackAnimName[1], animTable[spAttackAnimName[1]][ANIM_TBL_FRAME]);
	}
	if ((spAttackAnimName[2] != "技3") && (spAttackAnimFileName[2] != "waza_3"))
	{
		animFileName[spAttackAnimName[2]] = spAttackAnimFileName[2];
		animName.push_back(spAttackAnimName[2]);
		lpColMng.ColLoad(characterName, spAttackAnimName[2], animTable[spAttackAnimName[2]][ANIM_TBL_FRAME]);
	}

	// 一括読み込み
	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	// comListの初期化
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
	AddAnim("待機", 0, 0, 6, 5, true, 0, 0);
	AddAnim("前移動", 0, 0, 8, 5, true, 0, 0);
	AddAnim("後ろ移動", 0, 0, 8, 5, true, 0, 0);
	AddAnim("ジャンプ_上", 0, 0, 10, 6, true, 0, 0);
	AddAnim("ジャンプ_前", 0, 0, 6, 6, true, 0, 0);
	AddAnim("ジャンプ_後ろ", 0, 0, 6, 6, true, 0, 0);
	AddAnim("しゃがみ", 0, 0, 6, 5, true, 0, 0);
	AddAnim("しゃがみ_後ろ", 0, 0, 1, 5, true, 0, 0);
	AddAnim("しゃがみ始め", 0, 0, 3, 5, false, 0, 0);
	AddAnim("立ち始め", 0, 0, 3, 5, false, 0, 0);
	AddAnim("パンチ_小", 0, 0, 7, 5, false, 0, 0);
	AddAnim("パンチ_大", 0, 0, 11, 5, false, 0, 0);
	AddAnim("パンチ_小_空中", 0, 0, 4, 5, false, 0, 0);
	AddAnim("パンチ_大_空中", 0, 0, 5, 5, false, 0, 0);
	AddAnim("パンチ_小_しゃがみ", 0, 0, 8, 5, false, 0, 0);
	AddAnim("パンチ_大_しゃがみ", 0, 0, 10, 5, false, 0, 0);
	AddAnim("キック_小", 0, 0, 9, 5, false, 64, 0);
	AddAnim("キック_大", 0, 0, 11, 5, false, 0, 0);
	AddAnim("キック_小_空中", 0, 0, 4, 5, false, 0, 0);
	AddAnim("キック_大_空中", 0, 0, 5, 5, false, 0, 0);
	AddAnim("キック_小_しゃがみ", 0, 0, 7, 5, false, 0, 0);
	AddAnim("キック_大_しゃがみ", 0, 0, 10, 5, false, 0, 0);
	AddAnim("ガード_立ち", 0, 0, 1, 5, true, 0, 0);
	AddAnim("ガード_しゃがみ", 0, 0, 1, 5, true, 0, 0);
	AddAnim("ダメージ_立ち", 0, 0, 5, 5, false, 0, 0);
	AddAnim("ダメージ_ダウン", 0, 0, 13, 5, false, 0, 0);
	AddAnim("起き上がり", 0, 0, 13, 5, false, 0, 0);
	SetAnim("待機");
	return true;
}

void Character::CommandUpDate(const GameCtrl & ctl)
{
	// 入力方向の情報を更新
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

	// リストに入れる
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

	// 入力受付時間の処理
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

		// コマンド数分だけ戻す
		for (unsigned int buckCnt = 0; buckCnt < spAttackCommand[skillNum][dir].size(); buckCnt++)
		{
			itr--;
		}

		// 順番に比較
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

				// コマンド数分ループを抜けなかったら成功
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

	if ((knockBackFlag) || (GetAnim() == "ダメージ_立ち"))
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
				SetAnim("待機");
			}
		}
	}
	else if (GetAnim() == "ダメージ_ダウン")
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
			SetAnim("起き上がり");
		}
	}
	else if (GetAnim() == "起き上がり")
	{
		if (animEndFlag)
		{
			SetAnim("待機");
		}
	}
	else if (GetAnim() == "波動")
	{
		if (animCnt == (6 * animTable["波動"][ANIM_TBL_INV]))
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
			SetAnim("待機");
		}
	}
	else
	{
		// キャラクター操作
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
						// そのままの方向
					}
					SetAnim("パンチ_小_空中");
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
						// そのままの方向
					}
					SetAnim("パンチ_大_空中");
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
						// そのままの方向
					}
					SetAnim("キック_小_空中");
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
						// そのままの方向
					}
					SetAnim("キック_大_空中");
				}
				else
				{
					// 何もしない
				}
			}

			// ジャンプ中
			jumpSpeed.y += 1;
			pos += jumpSpeed;

			if (pos.y > ssize.y)
			{
				pos.y = ssize.y;
				SetAnim("待機");
			}
		}
		else
		{
			if ((animTable[GetAnim()][ANIM_TBL_LOOP]) || animEndFlag)
			{
				dir = tmpDir;

				if (ctl.GetPadDataTrg(padID, THUMB_L_UP))		// ジャンプ
				{
					if (ctl.GetPadData(padID, THUMB_L_RIGHT))
					{
						jumpSpeed = { JUMP_SPEED_X, -JUMP_SPEED_Y };

						if (dir == DIR_LEFT)
						{
							SetAnim("ジャンプ_後ろ");
						}
						else
						{
							SetAnim("ジャンプ_前");
						}
					}
					else if (ctl.GetPadData(padID, THUMB_L_LEFT))
					{
						jumpSpeed = { -JUMP_SPEED_X, -JUMP_SPEED_Y };

						if (dir == DIR_LEFT)
						{
							SetAnim("ジャンプ_前");
						}
						else
						{
							SetAnim("ジャンプ_後ろ");
						}
					}
					else
					{
						jumpSpeed = { 0, -JUMP_SPEED_Y };
						SetAnim("ジャンプ_上");
					}
				}
				else if (ctl.GetPadData(padID, THUMB_L_DOWN))		// しゃがみ
				{
					if (animAttribute[0] == ANIM_ATTRIBUTE_SQUAT)
					{
						if (ctl.GetPadData(padID, THUMB_L_RIGHT))
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
						else if (ctl.GetPadData(padID, THUMB_L_LEFT))
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
					// 移動
					if (ctl.GetPadData(padID, THUMB_L_RIGHT))
					{
						if (animAttribute[1] != ANIM_ATTRIBUTE_GUARD)
						{
							pos.x += 4;
						}
						if (dir == DIR_LEFT)
						{
							SetAnim("後ろ移動");
						}
						else
						{
							SetAnim("前移動");
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

				// 攻撃
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
							SetAnim("パンチ_小_しゃがみ");
						}
						else
						{
							SetAnim("パンチ_小");
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
							SetAnim("パンチ_大_しゃがみ");
						}
						else
						{
							SetAnim("パンチ_大");
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
							SetAnim("キック_小_しゃがみ");
						}
						else
						{
							SetAnim("キック_小");
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
							SetAnim("キック_大_しゃがみ");
						}
						else
						{
							SetAnim("キック_大");
						}
					}
				}
				else
				{
					// 何もしない
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

			if (GetAnim() != "ダメージ_立ち")
			{
				comboCnt++;
				if ((comboCnt >= COMBO_BREAK_CNT) || (animAttribute[0] == ANIM_ATTRIBUTE_AIR))
				{
					comboCnt = 0;

					SetAnim("ダメージ_ダウン");

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
					SetAnim("ダメージ_立ち");
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

	// DIRがRIGHTの場合画像を反転
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
			DrawString((i * 40) + 15, 200, "中", 0xffffff);
			break;
		case COM_DIR_UP:
			DrawString((i * 40) + 15, 200, "上", 0xffffff);
			break;
		case COM_DIR_RIGHT_UP:
			DrawString((i * 40) + 15, 200, "右上", 0xffffff);
			break;
		case COM_DIR_RIGHT:
			DrawString((i * 40) + 15, 200, "右", 0xffffff);
			break;
		case COM_DIR_RIGHT_DOWN:
			DrawString((i * 40) + 15, 200, "右下", 0xffffff);
			break;
		case COM_DIR_DOWN:
			DrawString((i * 40) + 15, 200, "下", 0xffffff);
			break;
		case COM_DIR_LEFT_DOWN:
			DrawString((i * 40) + 15, 200, "左下", 0xffffff);
			break;
		case COM_DIR_LEFT:
			DrawString((i * 40) + 15, 200, "左", 0xffffff);
			break;
		case COM_DIR_LEFT_UP:
			DrawString((i * 40) + 15, 200, "左上", 0xffffff);
			break;
		case COM_DIR_ACCUMULATE:
			DrawString((i * 40) + 15, 200, "タメ", 0xffffff);
			break;
		default:
			break;
		}

		i++;
	}
}
