#include "AICharacter.h"
#include "ImageMng.h"
#include "Shot.h"

#include "AIState.h"
#include "MoveState.h"
#include "WaitState.h"

#include "DxLib.h"

AICharacter::AICharacter(VECTOR2 offset) : Obj(offset)
{
	longAttackFlag = false;
	shotJumpFlag = false;
	dirChangeFlag = true;
	jumpType = JUMP_TYPE_RAND;

	padID = PAD_AI;

	ChangeState(MoveState::GetInstance());
}

AICharacter::~AICharacter()
{
}

bool AICharacter::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_AICHARACTER);
}

void AICharacter::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	if (dirChangeFlag)
	{
		// キャラの向き変更
		dir = tmpDir;
	}

	if (state)
	{
		state->Update(this);
	}

	if (longAttackFlag)
	{
		// 遠距離攻撃
		AddObjList()(objList, std::make_unique<Shot>(pos, drawOffset, dir, padID));
		longAttackFlag = false;
	}
}

void AICharacter::Draw()
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
	if (dir == DIR_LEFT)
	{
		turnFlag = true;
	}
	else
	{
		turnFlag = false;
	}

	if (visible)
	{
		SetDrawBright(100, 255, 100);
		DrawRotaGraph(drawOffset.x + animOffset.x + pos.x + (divSize.x / 2), drawOffset.y + animOffset.y + + pos.y + (divSize.y / 2), 1.0, 0.0, IMAGE_ID(imageName)[0], true, turnFlag);
		SetDrawBright(255, 255, 255);
	}
	animCnt++;
}

void AICharacter::ChangeState(AIState * s)
{
	state = s;
	state->Init(this);
}

bool AICharacter::InitAnim(void)
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
	AddAnim("パンチ_小_空中", 0, 0, 8, 5, false, 0, 0);
	AddAnim("パンチ_大_空中", 0, 0, 10, 5, false, 0, 0);
	AddAnim("パンチ_小_しゃがみ", 0, 0, 8, 5, false, 0, 0);
	AddAnim("パンチ_大_しゃがみ", 0, 0, 10, 5, false, 0, 0);
	AddAnim("キック_小", 0, 0, 9, 5, false, 64, 0);
	AddAnim("キック_大", 0, 0, 11, 5, false, 0, 0);
	AddAnim("キック_小_空中", 0, 0, 5, 5, false, 0, 0);
	AddAnim("キック_大_空中", 0, 0, 7, 5, false, 0, 0);
	AddAnim("キック_小_しゃがみ", 0, 0, 7, 5, false, 0, 0);
	AddAnim("キック_大_しゃがみ", 0, 0, 10, 5, false, 0, 0);
	AddAnim("ガード_立ち", 0, 0, 1, 5, true, 0, 0);
	AddAnim("ガード_しゃがみ", 0, 0, 1, 5, true, 0, 0);
	SetAnim("待機");

	return true;
}

bool AICharacter::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn)
{
	Obj::Init(fileName, divSize, divCut, pos, turn);
	InitAnim();

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
										  "ガード_しゃがみ" };		// ｱﾆﾒｰｼｮﾝ名を要素として持つvector

	// 必殺技系
	if ((spAttackAnimName[0] != "技1") && (spAttackAnimFileName[0] != "waza_1"))
	{
		animFileName[spAttackAnimName[0]] = spAttackAnimFileName[0];
		animName.push_back(spAttackAnimName[0]);
	}
	if ((spAttackAnimName[1] != "技2") && (spAttackAnimFileName[1] != "waza_2"))
	{
		animFileName[spAttackAnimName[1]] = spAttackAnimFileName[1];
		animName.push_back(spAttackAnimName[1]);
	}
	if ((spAttackAnimName[2] != "技3") && (spAttackAnimFileName[2] != "waza_3"))
	{
		animFileName[spAttackAnimName[2]] = spAttackAnimFileName[2];
		animName.push_back(spAttackAnimName[2]);
	}

	// 一括読み込み
	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	return true;
}
