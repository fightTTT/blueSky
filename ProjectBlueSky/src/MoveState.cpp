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
		// �O�ɐi��
		if (vec.x < 0)
		{
			pos.x -= 2;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("���ړ�");
			}
			else
			{
				character->SetAnim("�O�ړ�");
			}
		}
		else
		{
			pos.x += 2;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("�O�ړ�");
			}
			else
			{
				character->SetAnim("���ړ�");
			}

		}
	}
	else
	{
		// ���ɉ�����

		if (vec.x < 0)
		{
			pos.x += 2;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("�O�ړ�");
			}
			else
			{
				character->SetAnim("���ړ�");
			}
		}
		else
		{
			pos.x -= 2;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("���ړ�");
			}
			else
			{
				character->SetAnim("�O�ړ�");
			}
		}
	}

	//auto ssize = lpSceneMng.GetScreenSize();

	character->SetPos(pos);
	stateTime++;
}
