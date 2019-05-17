#include "AttackState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "Collision.h"
#include "DamageState.h"

#include "DxLib.h"

AttackState::AttackState()
{
}

AttackState::~AttackState()
{
}

void AttackState::Init(AICharacter * character)
{
	character->SetDirChange(false);

	int rand = GetRand(3);

	if (rand == 0)
	{
		character->SetAnim("パンチ_大");
	}
	else if (rand == 1)
	{
		character->SetAnim("パンチ_小");
	}
	else if (rand == 2)
	{
		character->SetAnim("キック_大");
	}
	else if (rand == 3)
	{
		character->SetAnim("キック_小");
	}
}

void AttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->SetDirChange(true);
		character->ChangeState(MoveState::GetInstance());
	}
}
