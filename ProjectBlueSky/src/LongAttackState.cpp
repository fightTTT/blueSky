#include "LongAttackState.h"
#include "AICharacter.h"
#include "MoveState.h"

LongAttackState::LongAttackState()
{
}

LongAttackState::~LongAttackState()
{
}

void LongAttackState::Init(AICharacter * character)
{
	character->SetLongAttackFlag(true);
	character->SetAnim("”g“®");
}

void LongAttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->ChangeState(MoveState::GetInstance());
	}
}
