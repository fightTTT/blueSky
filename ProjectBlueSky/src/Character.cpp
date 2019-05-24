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

	// ｱﾆﾒｰｼｮﾝの属性のﾃｰﾌﾞﾙ
	animAttributeTbl["待機"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["待機"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["待機"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["前移動"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["前移動"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["前移動"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["後ろ移動"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["後ろ移動"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["後ろ移動"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["ジャンプ_上"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["ジャンプ_上"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["ジャンプ_上"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["ジャンプ_前"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["ジャンプ_前"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["ジャンプ_前"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["ジャンプ_後ろ"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["ジャンプ_後ろ"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["ジャンプ_後ろ"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["しゃがみ"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["しゃがみ"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["しゃがみ_後ろ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["しゃがみ_後ろ"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["しゃがみ_後ろ"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["しゃがみ始め"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["しゃがみ始め"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["しゃがみ始め"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["立ち始め"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["立ち始め"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["立ち始め"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["パンチ_小"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["パンチ_小"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["パンチ_小"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["パンチ_大"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["パンチ_大"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["パンチ_大"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["パンチ_小_空中"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["パンチ_小_空中"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["パンチ_小_空中"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["パンチ_大_空中"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["パンチ_大_空中"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["パンチ_大_空中"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["パンチ_小_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["パンチ_小_しゃがみ"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["パンチ_小_しゃがみ"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["パンチ_大_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["パンチ_大_しゃがみ"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["パンチ_大_しゃがみ"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["キック_小"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["キック_小"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["キック_小"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["キック_大"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["キック_大"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["キック_大"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["キック_小_空中"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["キック_小_空中"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["キック_小_空中"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["キック_大_空中"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["キック_大_空中"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["キック_大_空中"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["キック_小_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["キック_小_しゃがみ"][1] = ANIM_ATTRIBUTE_ATTACK_SMALL;
	animAttributeTbl["キック_小_しゃがみ"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["キック_大_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["キック_大_しゃがみ"][1] = ANIM_ATTRIBUTE_ATTACK_BIG;
	animAttributeTbl["キック_大_しゃがみ"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["ガード_立ち"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["ガード_立ち"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["ガード_立ち"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["ガード_しゃがみ"][0] = ANIM_ATTRIBUTE_SQUAT;
	animAttributeTbl["ガード_しゃがみ"][1] = ANIM_ATTRIBUTE_GUARD;
	animAttributeTbl["ガード_しゃがみ"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["ダメージ_立ち"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["ダメージ_立ち"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["ダメージ_立ち"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["ダメージ_ダウン"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["ダメージ_ダウン"][1] = ANIM_ATTRIBUTE_MOVE;
	animAttributeTbl["ダメージ_ダウン"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

	animAttributeTbl["起き上がり"][0] = ANIM_ATTRIBUTE_STAND;
	animAttributeTbl["起き上がり"][1] = ANIM_ATTRIBUTE_NON;
	animAttributeTbl["起き上がり"][2] = ANIM_ATTRIBUTE_INVINCIBLE;

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
		lpColMng.ColLoad("棒人間", animName[i], animTable[animName[i]][ANIM_TBL_FRAME]);
	}
	// 必殺技系
	if ((spAttackAnimName[0] != "技1") && (spAttackAnimFileName[0] != "waza_1"))
	{
		animFileName[spAttackAnimName[0]] = spAttackAnimFileName[0];
		animName.push_back(spAttackAnimName[0]);
		lpColMng.ColLoad("棒人間", spAttackAnimName[0], animTable[spAttackAnimName[0]][ANIM_TBL_FRAME]);
	}
	if ((spAttackAnimName[1] != "技2") && (spAttackAnimFileName[1] != "waza_2"))
	{
		animFileName[spAttackAnimName[1]] = spAttackAnimFileName[1];
		animName.push_back(spAttackAnimName[1]);
		lpColMng.ColLoad("棒人間", spAttackAnimName[1], animTable[spAttackAnimName[1]][ANIM_TBL_FRAME]);
	}
	if ((spAttackAnimName[2] != "技3") && (spAttackAnimFileName[2] != "waza_3"))
	{
		animFileName[spAttackAnimName[2]] = spAttackAnimFileName[2];
		animName.push_back(spAttackAnimName[2]);
		lpColMng.ColLoad("棒人間", spAttackAnimName[2], animTable[spAttackAnimName[2]][ANIM_TBL_FRAME]);
	}

	// 一括読み込み
	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	// comListの初期化
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
	AddAnim("待機", 0, 0, 6, 4, true, 0, 0);
	AddAnim("前移動", 0, 0, 8, 4, true, 0, 0);
	AddAnim("後ろ移動", 0, 0, 8, 4, true, 0, 0);
	AddAnim("ジャンプ_上", 0, 0, 10, 4, true, 0, 0);
	AddAnim("ジャンプ_前", 0, 0, 6, 4, true, 0, 0);
	AddAnim("ジャンプ_後ろ", 0, 0, 6, 4, true, 0, 0);
	AddAnim("しゃがみ", 0, 0, 1, 4, true, 0, 0);
	AddAnim("しゃがみ_後ろ", 0, 0, 1, 4, true, 0, 0);
	AddAnim("しゃがみ始め", 0, 0, 2, 4, false, 0, 0);
	AddAnim("立ち始め", 0, 0, 3, 4, false, 0, 0);
	AddAnim("パンチ_小", 0, 0, 6, 4, false, 0, 0);
	AddAnim("パンチ_大", 0, 0, 8, 4, false, 0, 0);
	AddAnim("パンチ_小_空中", 0, 0, 4, 4, false, 0, 0);
	AddAnim("パンチ_大_空中", 0, 0, 5, 4, false, 0, 0);
	AddAnim("パンチ_小_しゃがみ", 0, 0, 6, 4, false, 0, 0);
	AddAnim("パンチ_大_しゃがみ", 0, 0, 9, 4, false, 0, 0);
	AddAnim("キック_小", 0, 0, 6, 4, false, 64, 0);
	AddAnim("キック_大", 0, 0, 8, 4, false, 0, 0);
	AddAnim("キック_小_空中", 0, 0, 4, 4, false, 0, 0);
	AddAnim("キック_大_空中", 0, 0, 5, 4, false, 0, 0);
	AddAnim("キック_小_しゃがみ", 0, 0, 6, 4, false, 0, 0);
	AddAnim("キック_大_しゃがみ", 0, 0, 9, 4, false, 0, 0);
	AddAnim("ガード_立ち", 0, 0, 1, 4, true, 0, 0);
	AddAnim("ガード_しゃがみ", 0, 0, 1, 4, true, 0, 0);
	AddAnim("ダメージ_立ち", 0, 0, 5, 4, false, 0, 0);
	AddAnim("ダメージ_ダウン", 0, 0, 13, 4, false, 0, 0);
	AddAnim("起き上がり", 0, 0, 10, 4, false, 0, 0);
	SetAnim("待機");
	return true;
}

void Character::CommandUpDate(const GameCtrl & ctl)
{
	if (comList.empty())
	{
		comList.push_back(SP_COM_CENTER);
	}

	// 入力方向の情報を更新
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

	// リストに入れる
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

		SP_COM com;
		SP_COM leftSideCom;
		SP_COM rightSideCom;

		// コマンド数分だけ戻す
		for (unsigned int buckCnt = 0; buckCnt < spAttackCommand[skillNum][dir].size(); buckCnt++)
		{
			itr--;
		}

		// 順番に比較
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

	if ((knockBackFlag) || (animName == "ダメージ_立ち"))
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
	else if (animName == "ダメージ_ダウン")
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
			SetAnim("起き上がり");
		}
	}
	else if (animName == "起き上がり")
	{
		if (animEndFlag)
		{
			SetAnim("待機");
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
			SetAnim("待機");
		}
	}
	else if (animName == "ローリングアタック")
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
			SetAnim("待機");
		}
	}
	else if (animName == "旋風脚")
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
			SetAnim("待機");
		}
	}
	else if (animName == "カンフーキック")
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
			SetAnim("待機");
		}
	}
	else if (animName == "ラッシュ")
	{
		if (animCnt > 40)
		{
			SetAnim("待機");
		}
	}
	else if ((animName == "ミサイルアロー") || (animName == "タックル"))
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
			SetAnim("待機");
		}
	}
	else if (animName == "ワープ")
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
			SetAnim("待機");
		}
	}
	else if (animName == "蹴り返し")
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
			// 攻撃が当たったら以下のように戻ってくる処理を入れ、当たらなかったらそのまま地面に落ちてしまうようにする
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
			SetAnim("待機");
		}
	}
	else if (animName == "アクセル")
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
			SetAnim("待機");
		}
	}
	else
	{
		// キャラクター操作
		if (animAttribute[0] == ANIM_ATTRIBUTE_AIR)
		{
			if (animName == "ランキャク")
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
					SetAnim("待機");
				}
			}
			else if ((animName == "地面割") || (animName == "かかと落とし"))		// 地面割はｴﾌｪｸﾄが付いた状態の画像を用意してそれに当たり判定を付ける必要あり
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
					SetAnim("待機");
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

				if (jumpSpeed.y == JUMP_SPEED_Y)
				{
					SetAnim("待機");
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
					if (ctl.GetPadData(padID, THUMB_L_UP))		// ジャンプ
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
						}
					}
					else if (ctl.GetPadData(padID, THUMB_L_DOWN))		// しゃがみ
					{
						jumpInterval = 0;

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
						jumpInterval = 0;

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
						if (ctl.GetPadData(padID, THUMB_L_DOWN))
						{
							SetAnim("パンチ_小_しゃがみ");
						}
						else
						{
							SetAnim("パンチ_小");
						}
					}
					else if (ctl.GetPadDataTrg(padID, BUTTON_B))
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
					else if (ctl.GetPadDataTrg(padID, BUTTON_X))
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
					else if (ctl.GetPadDataTrg(padID, BUTTON_Y))
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
					else
					{
						// 何もしない
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

			if (animName != "ダメージ_立ち")
			{
				AddPlayerHP(-10);
				WaitTimer(65);

				comboCnt++;
				if ((comboCnt >= COMBO_BREAK_CNT) || (animAttribute[0] == ANIM_ATTRIBUTE_AIR) || (playerHP <= 0))
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

	/*if (lpColMng.GetColFlag(animName))
	{
		ColInfo colData = lpColMng.GetCollisionData("棒人間", animName, id);

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

	// HP表示
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
		DrawFormatString(110, 25, 0xffffff, "playerの残りHP %d \n", playerHP);
		DrawBox(100, 65, 100 + 300, 85, 0x000000, true);
		DrawBox(100 + (300 - playerHPOld * 3), 65, 100 + 300 /*+ (playerHPOld * 3)*/, 85, 0xff0000, true);
		DrawBox(100 + (300 - playerHP * 3), 65, 100 + 300 /*+ (playerHP * 3)*/, 85, hpColor, true);
		DrawGraph(70, 55, IMAGE_ID("image/ゲームシーン用/hpGage.png")[0], true);
		DrawGraph(0, 0, IMAGE_ID("image/ゲームシーン用/iconFrame1.png")[0], true);
		DrawGraph(25, 10, IMAGE_DIV_ID("image/キャラセレ用/charIcon_small.png", VECTOR2(100, 100), VECTOR2(8, 1))[lpSceneMng.GetCharID(padID)], true);
	}
	else if (padID == PAD_2)
	{
		DrawFormatString(ssize.x - 410, 25, 0xffffff, "AIの残りHP %d \n", playerHP);
		DrawBox(ssize.x - 410, 65, ssize.x - 410 + 300, 85, 0x000000, true);
		DrawBox(ssize.x - 410, 65, ssize.x - 410 + (playerHPOld * 3), 85, 0xff0000, true);
		DrawBox(ssize.x - 410, 65, ssize.x - 410 + (playerHP * 3), 85, hpColor, true);
		DrawTurnGraph((ssize.x - 430), 55, IMAGE_ID("image/ゲームシーン用/hpGage.png")[0], true);
		DrawGraph((ssize.x-130), 0, IMAGE_ID("image/ゲームシーン用/iconFrame2.png")[0], true);
		DrawTurnGraph(ssize.x - 400 + 275, 10, IMAGE_DIV_ID("image/キャラセレ用/charIcon_small.png", VECTOR2(100, 100), VECTOR2(8, 1))[lpSceneMng.GetCharID(padID)], true);
	}
	else
	{
		// なにもしない
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
			DrawString((i * 40) + 15, 200, "中", 0xffffff);
			break;
		case SP_COM_UP:
			DrawString((i * 40) + 15, 200, "上", 0xffffff);
			break;
		case SP_COM_RIGHT_UP:
			DrawString((i * 40) + 15, 200, "右上", 0xffffff);
			break;
		case SP_COM_RIGHT:
			DrawString((i * 40) + 15, 200, "右", 0xffffff);
			break;
		case SP_COM_RIGHT_DOWN:
			DrawString((i * 40) + 15, 200, "右下", 0xffffff);
			break;
		case SP_COM_DOWN:
			DrawString((i * 40) + 15, 200, "下", 0xffffff);
			break;
		case SP_COM_LEFT_DOWN:
			DrawString((i * 40) + 15, 200, "左下", 0xffffff);
			break;
		case SP_COM_LEFT:
			DrawString((i * 40) + 15, 200, "左", 0xffffff);
			break;
		case SP_COM_LEFT_UP:
			DrawString((i * 40) + 15, 200, "左上", 0xffffff);
			break;
		case SP_COM_ACCUMULATE:
			DrawString((i * 40) + 15, 200, "タメ", 0xffffff);
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
