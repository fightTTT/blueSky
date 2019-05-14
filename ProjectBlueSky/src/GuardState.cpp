#include "GuardState.h"
#include "AICharacter.h"
#include "MoveState.h"


GuardState::GuardState()
{
}

GuardState::~GuardState()
{
}

void GuardState::Init(AICharacter * character)
{
	stateTime = 0;
}

void GuardState::Update(AICharacter * character)
{
	auto pos = character->GetPos();
	auto dir = character->GetDir();

	if (stateTime > 45)
	{
		character->ChangeState(MoveState::GetInstance());
	}
	else
	{
		if (dir == DIR_RIGHT)
		{
			pos.x -= 1;
		}
		else
		{
			pos.x += 1;
		}
	}

	character->SetPos(pos);

	stateTime++;
}
