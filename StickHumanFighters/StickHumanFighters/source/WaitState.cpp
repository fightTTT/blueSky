#include "WaitState.h"
#include "AICharacter.h"
#include "Collision.h"

#include "DxLib.h"

#define JUMP_DISTANCE (300)
#define ATTACK_TIME (15)

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

void WaitState::Update(AICharacter * character)
{
	auto pos = character->GetPos();
	auto enemyPos = character->GetEnemyState().enemyPos;

	auto vec = pos - enemyPos;
	int distance = abs(vec.x);

	character->SetAnim("�ҋ@");
	int rand = 0;

	// �G�Ƃ̋������߂Â��Ă�����O�փW�����v
	if(distance < JUMP_DISTANCE)
	{
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState("Jump");
		return;
	}

	if (stateTime >= ATTACK_TIME)
	{
		if (distance > 10)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (character->isSPLongAttack(character->GetSPAttackName(i)))
				{
					rand = GetRand(2);
					if (rand == 0)
					{
						character->SetAnim(character->GetSPAttackName(i));
						character->ChangeState("LongAttack");
						return;
					}
				}
			}

			// �����I������Ȃ�������SHOT�ɃZ�b�g
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
