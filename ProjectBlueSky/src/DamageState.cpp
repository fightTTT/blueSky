#include "DamageState.h"
#include "AICharacter.h"
#include "MoveState.h"

DamageState::DamageState()
{
}

DamageState::~DamageState()
{
}

void DamageState::Init(AICharacter * character)
{
}

void DamageState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->ChangeState(MoveState::GetInstance());
	}
}
