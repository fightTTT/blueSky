#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "JumpState.h"
#include "AttackState.h"
#include "LongAttackState.h"

#include "DxLib.h"

#define MOVE_SPEED (2)

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
	if (initFlag)
	{
		auto pos = character->GetPos();
		auto enemy = character->GetEnemyState();

		auto distance = enemy.enemyPos - pos;

		if (abs(distance.x) < 100)
		{
			// 敵と距離が近い場合はなれる
			moveDirFlag = false;
		}
		else
		{
			moveDirFlag = true;
		}

		stateTime = 0;

		initFlag = true;
	}
	moveDirFlag = true;
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto divSize = character->GetDivSize();

	VECTOR2 vec = enemy.enemyPos - pos;
	int rand;

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

	// 距離を再計算
	vec = enemy.enemyPos - pos;

	// 近距離攻撃が当たる距離の場合攻撃
	if (abs(vec.x) < 100)
	{
		rand = GetRand(5);

		if (rand == 0)
		{
			character->ChangeState(AttackState::GetInstance());
			character->SetAnim("パンチ_大");
			return;
		}
	}
	// 距離が遠い場合
	else/* if(abs(vec.x) > 100)*/
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

	rand = GetRand(99);

	if (abs(vec.x) < 100)
	{
		moveDirFlag = !(abs(vec.x) < 100);
		stateTime = 0;
	}

}
