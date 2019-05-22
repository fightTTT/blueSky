#include "WaitState.h"
#include "AICharacter.h"
#include "LongAttackState.h"
#include "JumpState.h"
#include "MoveState.h"
#include "Collision.h"
#include "DamageState.h"

#include "DxLib.h"

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

	character->SetAnim("‘Ò‹@");
	int rand = 0;

	if(distance < 300)
	{
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState(JumpState::GetInstance());
		return;
	}

	if (stateTime == 15)
	{
		if (distance > 10)
		{
			character->ChangeState(LongAttackState::GetInstance());
		}
		else
		{
			character->ChangeState(MoveState::GetInstance());
		}
	}

	stateTime++;
}
