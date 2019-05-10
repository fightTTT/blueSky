#include "AttackState.h"
#include "AICharacter.h"
#include "MoveState.h"

AttackState::AttackState()
{
}

AttackState::~AttackState()
{
}

void AttackState::Init(AICharacter * chara)
{
	chara->SetAnim("�p���`_��");
}

void AttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->ChangeState(MoveState::GetInstance());
	}
}
