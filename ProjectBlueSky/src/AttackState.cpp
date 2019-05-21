#include "AttackState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "Collision.h"
#include "DamageState.h"
#include "JumpState.h"

#include "DxLib.h"

AttackState::AttackState()
{
	attackCount = 0;
}

AttackState::~AttackState()
{
}

void AttackState::Init(AICharacter * character)
{
	character->SetDirChange(false);

	if (attackCount >= GetRand(10) + 4)
	{
		attackCount = 0;
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState(JumpState::GetInstance());
		return;
	}
	else
	{
		attackCount++;
	}

	int rand = GetRand(7);

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
	else if (rand == 4)
	{
		character->SetAnim("パンチ_小_しゃがみ");
	}
	else if (rand == 5)
	{
		character->SetAnim("パンチ_大_しゃがみ");
	}
	else if (rand == 6)
	{
		character->SetAnim("キック_小_しゃがみ");
	}
	else if (rand == 7)
	{
		character->SetAnim("キック_大_しゃがみ");
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
