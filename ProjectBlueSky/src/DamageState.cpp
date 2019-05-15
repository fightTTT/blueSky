#include "DamageState.h"
#include "AICharacter.h"
#include "MoveState.h"

#define KNOCK_BACK_SPEED (10)

DamageState::DamageState()
{
}

DamageState::~DamageState()
{
}

void DamageState::Init(AICharacter * character)
{
	auto dir = character->GetDir();

	if (dir == DIR_RIGHT)
	{
		knockBackSpeed = -KNOCK_BACK_SPEED;
	}
	else
	{
		knockBackSpeed = KNOCK_BACK_SPEED;
	}
}

void DamageState::Update(AICharacter * character)
{
	auto dir = character->GetDir();
	auto pos = character->GetPos();

	if (character->GetAnimEndFlag())
	{
		character->ChangeState(MoveState::GetInstance());
	}
	else
	{
		pos.x += knockBackSpeed;

		if (dir == DIR_RIGHT)
		{
			knockBackSpeed++;
			if (knockBackSpeed > 0)
			{
				knockBackSpeed = 0;
			}
		}
		else
		{
			knockBackSpeed--;
			if (knockBackSpeed < 0)
			{
				knockBackSpeed = 0;
			}
		}
	}

	character->SetPos(pos);
}
