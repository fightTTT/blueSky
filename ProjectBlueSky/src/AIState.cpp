#include "AIState.h"
#include "Collision.h"
#include "DamageState.h"
#include "AICharacter.h"

#include "DxLib.h"

AIState::AIState()
{
	stateTime = 0;
}

AIState::~AIState()
{
}

void AIState::CheckHitFlag(AICharacter * character)
{
	auto dir = character->GetDir();
	auto hitData = character->GetHitData();
	auto anim = character->GetAnim();

	auto hitFlag = hitData.hitFlag && hitData.colType == COLTYPE_HIT;

	if(hitFlag && character->GetAnimAttribute(1) != ANIM_ATTRIBUTE_INVINCIBLE)
	{
		WaitTimer(WAIT_TIMER_COUNT);
		character->AddPlayerHP(-10);
		character->ChangeState(DamageState::GetInstance());
	}
}
