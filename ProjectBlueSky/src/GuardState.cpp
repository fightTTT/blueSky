#include "GuardState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "Collision.h"

GuardState::GuardState()
{
}

GuardState::~GuardState()
{
}

void GuardState::Init(AICharacter * character)
{
	stateTime = 0;
	guardHitFlag = false;
	knockBackSpeed = 10;
}

void GuardState::Update(AICharacter * character)
{
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto hitData = character->GetHitData();

	guardHitFlag = guardHitFlag || (hitData.hitFlag && hitData.colType == COLTYPE_GUARD);

	if (stateTime > 30)
	{
		character->ChangeState(MoveState::GetInstance());
	}
	else
	{
		if (dir == DIR_RIGHT)
		{
			pos.x -= knockBackSpeed;
		}
		else
		{
			pos.x += knockBackSpeed;
		}
	}

	if (knockBackSpeed > 1)
	{
		knockBackSpeed--;
	}

	character->SetPos(pos);

	stateTime++;
}
