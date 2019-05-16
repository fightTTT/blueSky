#include "AttackState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "Collision.h"
#include "DamageState.h"

AttackState::AttackState()
{
}

AttackState::~AttackState()
{
}

void AttackState::Init(AICharacter * character)
{
	character->SetDirChange(false);
}

void AttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->SetDirChange(true);
		character->ChangeState(MoveState::GetInstance());
	}
}

void AttackState::CheckHitFlag(AICharacter * character)
{
	auto dir = character->GetDir();
	auto hitData = character->GetHitData();
	auto anim = character->GetAnim();

	auto hitFlag = hitData.hitFlag && hitData.colType == COLTYPE_HIT;

	if (hitFlag && !(anim == "ダメージ_立ち") && !(anim == "ダメージ_ダウン"))
	{
		character->SetAnim("ダメージ_立ち");
		character->ChangeState(DamageState::GetInstance());
	}
}
