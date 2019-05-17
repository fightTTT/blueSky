#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "JumpState.h"
#include "AttackState.h"
#include "LongAttackState.h"
#include "GuardState.h"
#include "Collision.h"
#include "DamageState.h"
#include "WaitState.h"

#include "DxLib.h"

#define MOVE_SPEED (4)
#define ATTACK_RANGE (180)

MoveState::MoveState()
{
	// 最初だけ初期化
	moveDirFlag = true;
}

MoveState::~MoveState()
{
}

void MoveState::Init(AICharacter * character)
{
	changeGuardStateCount = 0;
	stateTime = 0;
	notAttackCount = 0;
	moveFrontCount = 0;
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto divSize = character->GetDivSize();

	VECTOR2 vec = enemy.enemyPos - pos;
	int rand = 0;

	if (character->GetAnimAttribute(1) == ANIM_ATTRIBUTE_GUARD)
	{
		character->ChangeState(GuardState::GetInstance());
		moveDirFlag = true;
		return;
	}

	if (moveDirFlag)
	{
		moveFrontCount++;
	}
	else
	{
		moveFrontCount = 0;
	}

	if (moveFrontCount > 50 && notAttackCount > 100)
	{
		character->ChangeState(WaitState::GetInstance());
		moveDirFlag = true;
		return;
	}

	if (!moveDirFlag && (abs(vec.x) > 300))
	{
		character->ChangeState(WaitState::GetInstance());
		moveDirFlag = true;
		return;
	}

	if (!(abs(vec.x) < ATTACK_RANGE - 80) && GetRand(100) == 0)
	{
		moveDirFlag = !moveDirFlag;
	}

	if (abs(vec.x) < ATTACK_RANGE && (enemy.enemyAnimAttribute[1] == ANIM_ATTRIBUTE_ATTACK))
	{
		moveDirFlag = false;
	}

	// 近距離攻撃が当たる距離の場合攻撃
	if (abs(vec.x) < ATTACK_RANGE - 80)
	{
		rand = GetRand(100);
		if (rand <= 5)
		{
			character->SetJumpType(JUMP_TYPE_FRONT);
			character->ChangeState(JumpState::GetInstance());
			moveDirFlag = true;
			return;
		}
		character->ChangeState(AttackState::GetInstance());
		notAttackCount = 0;
		return;
	}
	// 距離が遠い場合
	else if (abs(vec.x) > 250)
	{
		rand = GetRand(100);

		if (rand == 0 && notAttackCount > 180)
		{
			character->ChangeState(LongAttackState::GetInstance());
			notAttackCount = 0;
			return;
		}
	}

	// 攻撃していない状態が続いた場合
	if (notAttackCount >= 180)
	{
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState(JumpState::GetInstance());
		notAttackCount = 0;
		moveDirFlag = true;
		return;
	}

	notAttackCount++;

	// 弾をジャンプ回避
	for (auto data : enemy.shotData)
	{
		// 自分が撃った弾の場合
		if (data.id == character->GetPadID())
		{
			continue;
		}

		auto distance = pos - data.pos;

		// 弾が自分より前にある場合実行
		if ((charaDir == DIR_RIGHT && distance.x > -divSize.x) || (charaDir == DIR_LEFT && distance.x < divSize.x))
		{
			if (data.pos.x > 0 && abs(distance.x) < 200)
			{
				character->SetJumpType(JUMP_TYPE_RAND);
				character->ChangeState(JumpState::GetInstance());
				moveDirFlag = true;
				return;
			}
		}
		else
		{
			continue;
		}
	}

	// 画面端に近づいたら方向を切り替える
	VECTOR2 ssize = lpSceneMng.GetScreenSize();
	int screenDistance = 0;

	if (charaDir == DIR_RIGHT)
	{
		screenDistance = pos.x;
	}
	else
	{
		screenDistance = abs(pos.x - ssize.x);
	}

	if (abs(vec.x < 250) && screenDistance < 200)
	{
		rand = GetRand(1);
		if (rand == 0)
		{
			character->SetJumpType(JUMP_TYPE_FRONT);
			character->ChangeState(JumpState::GetInstance());
			moveDirFlag = true;
			return;
		}
		else
		{
			moveDirFlag = true;
		}
	}

	// ガード状態の場合は移動しない
	if (character->GetAnimAttribute(1) == ANIM_ATTRIBUTE_GUARD)
	{
		return;
	}

	// 移動処理
	if (moveDirFlag)
	{
		// 前に進む
		if (vec.x < 0)
		{
			pos.x -= MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("後ろ移動");
			}
			else
			{
				character->SetAnim("前移動");
			}
		}
		else
		{
			pos.x += MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("前移動");
			}
			else
			{
				character->SetAnim("後ろ移動");
			}

		}
	}
	else
	{
		// 後ろに下がる

		if (vec.x < 0)
		{
			pos.x += MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("前移動");
			}
			else
			{
				character->SetAnim("後ろ移動");
			}
		}
		else
		{
			pos.x -= MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("後ろ移動");
			}
			else
			{
				character->SetAnim("前移動");
			}
		}
	}

	if (pos.x > ssize.x - divSize.x / 4)
	{
		pos.x -= MOVE_SPEED;
	}

	character->SetPos(pos);
	stateTime++;
}

void MoveState::CheckHitFlag(AICharacter * character)
{
	auto dir = character->GetDir();
	auto hitData = character->GetHitData();
	auto anim = character->GetAnim();

	auto hitFlag = hitData.hitFlag && hitData.colType == COLTYPE_HIT;

	if (hitFlag && character->GetAnimAttribute(1) != ANIM_ATTRIBUTE_INVINCIBLE)
	{
		character->SetAnim("ダメージ_立ち");
		character->ChangeState(DamageState::GetInstance());
	}
}
