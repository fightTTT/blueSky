#include "LongAttackState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "DamageState.h"
#include "Collision.h"

#define SHOT_INV (30)

LongAttackState::LongAttackState()
{
}

LongAttackState::~LongAttackState()
{
}

void LongAttackState::Init(AICharacter * character)
{
	shotCount = 0;
	character->SetAnim("”g“®");
}

void LongAttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->ChangeState(MoveState::GetInstance());
	}

	if (shotCount == SHOT_INV)
	{
		character->SetLongAttackFlag(true);
	}

	shotCount++;
}
