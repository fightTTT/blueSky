#include "AICharacter.h"
#include "ImageMng.h"
#include "Shot.h"
#include "AIState.h"
#include "MoveState.h"

#include "DxLib.h"

AICharacter::AICharacter()
{
	AIStateTime = 0;
	moveStateTime = 0;
	LongAttackFlag = false;
	moveDirFlag = true;
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
	dir = tmpDir;

	if (state)
	{
		state->Update(this);
	}

	ChangeState(MoveState::GetInstance());

	switch (AIStateType)
	{
	case AI_STATE_NONE:
		SetAnim("待機");
		break;
	case AI_STATE_MOVE:
		Move();
		break;
	case AI_STATE_ATTACK:
		if (!(LongAttackFlag) && (animTable[GetAnim()][ANIM_TBL_LOOP] || animEndFlag))
		{
			AddObjList()(objList, std::make_unique<Shot>(pos, dir));
			SetAnim(spAttackAnimName[0]);
			LongAttackFlag = true;
		}
		break;
	case AI_STATE_JUMP:
		break;
	case AI_STATE_MAX:
		break;
	default:
		break;
	}

	if (LongAttackFlag && animEndFlag)
	{
		AIStateType = AI_STATE_MOVE;
		LongAttackFlag = false;
		AIStateTime = 0;
	}

	if (AIStateTime > 100 && AIStateType == AI_STATE_MOVE)
	{
		AIStateType = AI_STATE_ATTACK;
		
		AIStateTime = 0;
	}

	AIStateTime++;
}

void AICharacter::Draw()
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

	// DIRがRIGHTの場合画像を反転
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
}

void AICharacter::ChangeState(AIState * s)
{
	state = s;
	state->Init(this);
}

void AICharacter::Move()
{
	if (enemyState.enemyPos < 0)
	{
		return;
	}

	VECTOR2 vec = enemyState.enemyPos - pos;

	int rand = GetRand(10);

	if (abs(vec.x) < 100)
	{
		moveDirFlag = !(abs(vec.x) < 100);
		moveStateTime = 0;
	}
	else if (moveStateTime > 150)
	{

		if (rand == 0)
		{
			moveDirFlag = !moveDirFlag;
			moveStateTime = 0;
		}
	}

	if (moveDirFlag)
	{
		// 前に進む
		if (vec.x < 0)
		{
			pos.x -= 2;

			if (dir == DIR_RIGHT)
			{
				SetAnim("後ろ移動");
			}
			else
			{
				SetAnim("前移動");
			}
		}
		else
		{
			pos.x += 2;

			if (dir == DIR_RIGHT)
			{
				SetAnim("前移動");
			}
			else
			{
				SetAnim("後ろ移動");
			}

		}
	}
	else
	{
		// 後ろに下がる

		if (vec.x < 0)
		{
			pos.x += 2;

			if (dir == DIR_RIGHT)
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
			pos.x -= 2;

			if (dir == DIR_RIGHT)
			{
				SetAnim("後ろ移動");
			}
			else
			{
				SetAnim("前移動");
			}
		}
	}

	moveStateTime++;
}

bool AICharacter::InitAnim(void)
{
	AddAnim("待機", 0, 0, 10, 5, true);
	AddAnim("前移動", 0, 0, 11, 5, true);
	AddAnim("後ろ移動", 0, 0, 11, 5, true);
	AddAnim("ジャンプ_上", 0, 0, 12, 5, true);
	AddAnim("ジャンプ_前", 0, 0, 14, 5, true);
	AddAnim("ジャンプ_後ろ", 0, 0, 14, 5, true);
	AddAnim("しゃがみ", 0, 0, 4, 5, true);
	AddAnim("しゃがみ_後ろ", 0, 0, 1, 5, true);
	AddAnim("しゃがみ始め", 0, 0, 2, 5, false);
	AddAnim("立ち始め", 0, 0, 2, 5, false);
	AddAnim("パンチ_小", 0, 0, 4, 5, false);
	AddAnim("パンチ_大", 0, 0, 7, 5, false);
	AddAnim("ガード_しゃがみ", 0, 0, 1, 5, true);
	SetAnim("待機");

	return true;
}

bool AICharacter::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn)
{
	Obj::Init(fileName, divSize, divCut, pos, turn);
	InitAnim();

	jumpSpeed = { 0, 0 };
	jumpFlag = false;

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
