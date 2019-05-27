#include "AICharacter.h"
#include "ImageMng.h"
#include "Shot.h"
#include "CollisionMng.h"
#include "SceneMng.h"

#include "AIState.h"
#include "AttackState.h"
#include "MoveState.h"
#include "WaitState.h"
#include "DamageState.h"
#include "GuardState.h"
#include "JumpState.h"
#include "LongAttackState.h"

#include "DxLib.h"

AICharacter::AICharacter(VECTOR2 offset) : Obj(offset)
{
	longAttackFlag = false;
	dirChangeFlag = true;
	jumpType = JUMP_TYPE_RAND;

	padID = PAD_AI;

	DrawHPCount = 0.0f;

	AddStateObj("Attack", std::move(std::make_shared<AttackState>()));
	AddStateObj("Damage", std::move(std::make_shared<DamageState>()));
	AddStateObj("Guard", std::move(std::make_shared<GuardState>()));
	AddStateObj("Jump", std::move(std::make_shared<JumpState>()));
	AddStateObj("LongAttack", std::move(std::make_shared<LongAttackState>()));
	AddStateObj("Move", std::move(std::make_shared<MoveState>()));
	AddStateObj("Wait", std::move(std::make_shared<WaitState>()));

	ChangeState("Move");
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

	if (stateObj.size())
	{
		stateObj[currentStateName]->Update(this);
	}

	if (longAttackFlag)
	{
		// 遠距離攻撃
		if (dir == DIR_RIGHT)
		{
			AddObjList()(objList, std::make_unique<Shot>(pos + VECTOR2(256, 64), drawOffset, dir, padID));
		}
		else
		{
			AddObjList()(objList, std::make_unique<Shot>(pos + VECTOR2(-128, 64), drawOffset, dir, padID));
		}
	
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
		DrawRotaGraph(drawOffset.x + animOffset.x + pos.x + (divSize.x / 2), drawOffset.y + animOffset.y + pos.y + (divSize.y / 2), 1.0, 0.0, IMAGE_ID(imageName)[0], true, turnFlag);
	}

	auto ssize = lpSceneMng.GetScreenSize();
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
	DrawFormatString(ssize.x - 410, 25, 0xffffff, "AIの残りHP %d \n", playerHP);
	DrawBox(ssize.x - 410, 65, ssize.x - 410 + 300, 85, 0x000000, true);
	DrawBox(ssize.x - 410, 65, ssize.x - 410 + (playerHPOld * 3), 85, 0xff0000, true);
	DrawBox(ssize.x - 410, 65, ssize.x - 410 + (playerHP * 3), 85, hpColor, true);
	DrawTurnGraph((ssize.x - 430), 55, IMAGE_ID("image/ゲームシーン用/hpGage.png")[0], true);
	DrawGraph((ssize.x - 130), 0, IMAGE_ID("image/ゲームシーン用/iconFrame2.png")[0], true);
	DrawTurnGraph(ssize.x - 400 + 275, 10, IMAGE_DIV_ID("image/キャラセレ用/charIcon_small.png", VECTOR2(100, 100), VECTOR2(8, 1))[lpSceneMng.GetCharID(padID)], true);

	if (!animStopFlag)
	{
		animCnt++;
	}
}

void AICharacter::ChangeState(std::string key)
{
	currentStateName = key;
	stateObj[currentStateName]->Init(this);
}

std::string AICharacter::GetSPAttackName(int idx)
{
	return spAttackAnimName[idx];
}

HitData AICharacter::GetHitData() const
{
	return hitData;
}

void AICharacter::CheckHitFlag()
{
	stateObj[currentStateName]->CheckHitFlag(this);
}

bool AICharacter::isSPLongAttack(std::string spAnimName)
{
	if (spAnimName == "ローリングアタック"
		|| spAnimName == "ミサイルアロー"
		|| spAnimName == "ワープ"
		|| spAnimName == "旋風脚"
		|| spAnimName == "タックル"
		|| spAnimName == "アクセル")
	{
		return true;
	}
	return false;
}

bool AICharacter::InitAnim(void)
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

void AICharacter::AddStateObj(std::string key, std::shared_ptr<AIState> state)
{
	if (!stateObj.count(key))
	{
		stateObj[key] = state;
	}
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
	animAttributeTbl["ダメージ_立ち"][1] = ANIM_ATTRIBUTE_DAMAGE;
	animAttributeTbl["ダメージ_立ち"][2] = ANIM_ATTRIBUTE_NON;

	animAttributeTbl["ダメージ_ダウン"][0] = ANIM_ATTRIBUTE_AIR;
	animAttributeTbl["ダメージ_ダウン"][1] = ANIM_ATTRIBUTE_DAMAGE;
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
