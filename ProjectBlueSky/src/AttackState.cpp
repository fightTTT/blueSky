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

	int rand = GetRand(7);

	if (rand == 0)
	{
		character->SetAnim("�p���`_��");
	}
	else if (rand == 1)
	{
		character->SetAnim("�p���`_��");
	}
	else if (rand == 2)
	{
		character->SetAnim("�L�b�N_��");
	}
	else if (rand == 3)
	{
		character->SetAnim("�L�b�N_��");
	}
	else if (rand == 4)
	{
		character->SetAnim("�p���`_��_���Ⴊ��");
	}
	else if (rand == 5)
	{
		character->SetAnim("�p���`_��_���Ⴊ��");
	}
	else if (rand == 6)
	{
		character->SetAnim("�L�b�N_��_���Ⴊ��");
	}
	else if (rand == 7)
	{
		character->SetAnim("�L�b�N_��_���Ⴊ��");
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
