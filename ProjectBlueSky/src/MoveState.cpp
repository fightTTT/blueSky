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

#define WAITSTATE_INV (60)

MoveState::MoveState()
{
	// 最初だけ初期化
	moveDirFlag = true;
	guardHitCount = 0;
}

MoveState::~MoveState()
{
}

void MoveState::Init(AICharacter * character)
{
	changeGuardStateCount = 0;
	stateTime = 0;
	moveFrontCount = 0;
	changeWaitStateCount = 0;
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto divSize = character->GetDivSize();

	VECTOR2 vec = enemy.enemyPos - pos;
	int rand = 0;
	notAttackCount++;

	if (guardHitCount >= 50)
	{
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState(JumpState::GetInstance());
		guardHitCount = 0;
		moveDirFlag = true;
		return;
	}

	if (character->GetAnimAttribute(1) == ANIM_ATTRIBUTE_GUARD)
	{
		character->ChangeState(GuardState::GetInstance());
		guardHitCount++;
		moveDirFlag = true;
		return;
	}

	if ( static_cast<int>(changeGuardCount) > GetRand(30))
	{
		moveDirFlag = false;
		changeGuardCount = 0;
	}

	changeWaitStateCount++;

	if (moveDirFlag)
	{
		// 前移動をしている場合カウントを加算
		moveFrontCount++;
	}
	else
	{
		moveFrontCount = 0;
	}

	// 攻撃をしていない状態で前に移動し続けていたら状態を変更
	if (moveFrontCount > 40 && notAttackCount > 80)
	{
		if (changeWaitStateCount > WAITSTATE_INV)
		{
			character->ChangeState(WaitState::GetInstance());
			changeWaitStateCount = 0;
			moveDirFlag = true;
			return;
		}
	}

	// 後ろ移動で距離がはなれすぎていたら状態を変更
	if (!moveDirFlag && (abs(vec.x) > 300))
	{
		if (changeWaitStateCount > WAITSTATE_INV)
		{
			character->ChangeState(WaitState::GetInstance());
			changeWaitStateCount = 0;
			moveDirFlag = true;
			return;
		}
	}

	// ランダムで方向を逆方向に切り替え
	if (!(abs(vec.x) < ATTACK_RANGE - GetRand(200)) && GetRand(50) == 0)
	{
		moveDirFlag = !moveDirFlag;
	}

	if (abs(vec.x) < ATTACK_RANGE && ((enemy.enemyAnimAttribute[1] == ANIM_ATTRIBUTE_NON) || (enemy.enemyAnimAttribute[1] == ANIM_ATTRIBUTE_MOVE)))
	{
		changeGuardCount++;
	}
	else
	{
		changeGuardCount = 0;
	}

	// 近距離攻撃が当たる距離の場合攻撃
	if (abs(vec.x) < ATTACK_RANGE - GetRand(120))
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
	if (notAttackCount >= 120)
	{
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState(JumpState::GetInstance());
		notAttackCount = 0;
		moveDirFlag = true;
		return;
	}

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

	stateTime++;

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

	VECTOR2 ssize = lpSceneMng.GetScreenSize();

	if (pos.x > ssize.x - divSize.x / 4)
	{
		pos.x -= MOVE_SPEED;
	}

	character->SetPos(pos);
}
