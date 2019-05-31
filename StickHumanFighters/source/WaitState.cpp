#include "WaitState.h"
#include "AICharacter.h"
#include "Collision.h"

#include "DxLib.h"

#define ATTACK_TIME (15)
#define LONG_ATTACK_DISTANCE (300)

WaitState::WaitState()
{
}

WaitState::~WaitState()
{
}

void WaitState::Init(AICharacter * character)
{
	stateTime = 0;
}

void WaitState::Update(AICharacter * character, const int level)
{
	auto pos = character->GetPos();
	auto enemyPos = character->GetEnemyState().enemyPos;

	auto vec = pos - enemyPos;
	int distance = abs(vec.x);

	character->SetAnim("待機");
	int rand = 0;

	if (stateTime >= ATTACK_TIME)
	{
		if (distance > LONG_ATTACK_DISTANCE)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (character->isSPLongAttack(character->GetSPAttackName(i)))
				{
					rand = GetRand(10 - (level * 3));
					if (rand == 0)
					{
						character->SetAnim(character->GetSPAttackName(i));
						character->ChangeState("LongAttack");
						return;
					}
				}
			}

			// 何も選択されなかったらSHOTにセット
			character->SetAnim(character->GetSPAttackName(0));
			character->ChangeState("LongAttack");
			return;
		}
		else
		{
			character->ChangeState("Move");
		}
	}

	stateTime++;
}
