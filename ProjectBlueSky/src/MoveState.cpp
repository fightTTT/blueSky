#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"

#include "DxLib.h"

MoveState::MoveState()
{
}

MoveState::~MoveState()
{
}

void MoveState::Init(AICharacter * chara)
{
	moveDirFlag = true;
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();

	VECTOR2 vec = enemy.enemyPos - pos;

	int rand = GetRand(100);

	if (abs(vec.x) < 100)
	{
		moveDirFlag = !(abs(vec.x) < 100);
		stateTime = 0;
	}
	else if (stateTime > 150)
	{
		if (rand == 0)
		{
			moveDirFlag = !moveDirFlag;
			stateTime = 0;
		}
	}

	if (moveDirFlag)
	{
		// 前に進む
		if (vec.x < 0)
		{
			pos.x -= 2;

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
			pos.x += 2;

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
			pos.x += 2;

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
			pos.x -= 2;

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

	//auto ssize = lpSceneMng.GetScreenSize();

	character->SetPos(pos);
	stateTime++;
}
