#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "JumpState.h"
#include "AttackState.h"
#include "LongAttackState.h"
#include "GuardState.h"

#include "DxLib.h"

#define MOVE_SPEED (2)
#define ATTACK_RANGE (150)

MoveState::MoveState()
{
	// 最初だけ初期化
	initFlag = false;
}

MoveState::~MoveState()
{
}

void MoveState::Init(AICharacter * character)
{
	if (!initFlag)
	{
		moveDirFlag = true;
		initFlag = true;
	}

	stateTime = 0;
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto divSize = character->GetDivSize();

	VECTOR2 vec = enemy.enemyPos - pos;
	int rand;

	if (abs(vec.x) < ATTACK_RANGE && (enemy.enemyAnimName == "キック_大" || enemy.enemyAnimName == "キック_小"))
	{
		character->SetAnim("ガード_立ち");
		character->ChangeState(GuardState::GetInstance());
		return;
	}

	if (abs(vec.x) < ATTACK_RANGE && (enemy.enemyAnimName == "キック_大_しゃがみ" || enemy.enemyAnimName == "キック_小_しゃがみ"))
	{
		character->SetAnim("ガード_しゃがみ");
		character->ChangeState(GuardState::GetInstance());
		return;
	}

	// 近距離攻撃が当たる距離の場合攻撃
	if (abs(vec.x) < 100)
	{
		rand = GetRand(5);

		if (rand == 0)
		{
			character->SetAnim("パンチ_大");
			character->ChangeState(AttackState::GetInstance());
			return;
		}
		else if (rand == 1)
		{
			character->SetAnim("パンチ_小");
			character->ChangeState(AttackState::GetInstance());
			return;
		}
		else if (rand == 2)
		{
			character->SetAnim("キック_大");
			character->ChangeState(AttackState::GetInstance());
			return;
		}
		else if (rand == 3)
		{
			character->SetAnim("キック_小");
			character->ChangeState(AttackState::GetInstance());
			return;
		}
	}
	// 距離が遠い場合
	else if (abs(vec.x) > 250)
	{
		rand = GetRand(500);

		if (rand == 0)
		{
			character->ChangeState(LongAttackState::GetInstance());
			return;
		}
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
				return;
			}
		}
		else
		{
			continue;
		}
	}

	rand = GetRand(500);

	// 移動方向にジャンプ
	if (moveDirFlag)
	{
		if (rand == 0)
		{
			character->SetJumpType(JUMP_TYPE_FRONT);
			character->ChangeState(JumpState::GetInstance());
			return;
		}
	}
	else
	{
		if (rand == 0)
		{
			character->SetJumpType(JUMP_TYPE_BACK);
			character->ChangeState(JumpState::GetInstance());
			return;
		}
	}

	//if (abs(vec.x) < 100)
	//{
	//	moveDirFlag = !(abs(vec.x) < 100);
	//	stateTime = 0;
	//}

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

	auto ssize = lpSceneMng.GetScreenSize();

	if (pos.x > ssize.x - divSize.x / 4)
	{
		pos.x -= MOVE_SPEED;
	}

	character->SetPos(pos);
	stateTime++;
}
