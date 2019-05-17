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

void LongAttackState::CheckHitFlag(AICharacter * character)
{
	auto dir = character->GetDir();
	auto hitData = character->GetHitData();
	auto anim = character->GetAnim();

	auto hitFlag = hitData.hitFlag && hitData.colType == COLTYPE_HIT;

	if (hitFlag && character->GetAnimAttribute(1) != ANIM_ATTRIBUTE_INVINCIBLE)
	{
		character->SetAnim("ƒ_ƒ[ƒW_—§‚¿");
		character->ChangeState(DamageState::GetInstance());
	}
}
