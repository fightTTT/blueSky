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
	if (stateTime > 30)
	{
		character->ChangeState(MoveState::GetInstance());
	}

	stateTime++;
}

void GuardState::CheckHitFlag(AICharacter * character)
{
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto hitData = character->GetHitData();

	auto hitFlag = hitData.hitFlag && hitData.colType == COLTYPE_GUARD;

	if (!guardHitFlag && hitFlag)
	{
		stateTime = 0;
	}

	guardHitFlag = guardHitFlag || hitFlag;

	if (guardHitFlag)
	{
		if (dir == DIR_RIGHT)
		{
			pos.x -= knockBackSpeed;
		}
		else
		{
			pos.x += knockBackSpeed;
		}

		if (knockBackSpeed > 1)
		{
			knockBackSpeed--;
		}
	}

	character->SetPos(pos);
}
