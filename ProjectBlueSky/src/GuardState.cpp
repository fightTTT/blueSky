#include "GuardState.h"
#include "AICharacter.h"
#include "Collision.h"

#define KNOCK_BACK_SPEED (10)

GuardState::GuardState()
{
}

GuardState::~GuardState()
{
}

void GuardState::Init(AICharacter * character)
{
	stateTime = 0;
	guardHitFlag = false;
	knockBackSpeed = 0;
}

void GuardState::Update(AICharacter * character)
{
	if ((character->GetAnimAttribute(1) != ANIM_ATTRIBUTE_ATTACK_SMALL)
	 && (character->GetAnimAttribute(1) != ANIM_ATTRIBUTE_ATTACK_BIG)
	 && (character->GetAnimAttribute(1) != ANIM_ATTRIBUTE_ATTACK_SP)
	 && knockBackSpeed == 0)
	{
		character->ChangeState("Move");
		character->SetAnim("後ろ移動");
	}

	auto enemy = character->GetEnemyState();

	if (enemy.enemyAnimAttribute[0] == ANIM_ATTRIBUTE_SQUAT)
	{
		character->SetAnim("しゃがみ_後ろ");
	}

	stateTime++;
}

void GuardState::CheckHitFlag(AICharacter * character)
{
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto hitData = character->GetHitData();

	auto hitFlag = hitData.hitFlag && hitData.colType == COLTYPE_GUARD;

	// 攻撃を受けた時のノックバック
	if (!guardHitFlag && hitFlag)
	{
		stateTime = 0;

		if (dir == DIR_RIGHT)
		{
			knockBackSpeed = -KNOCK_BACK_SPEED;
		}
		else
		{
			knockBackSpeed = KNOCK_BACK_SPEED;
		}
	}

	guardHitFlag = guardHitFlag || hitFlag;

	// ガード時のノックバック
	if (guardHitFlag)
	{
		pos.x += knockBackSpeed;

		if (dir == DIR_RIGHT)
		{
			knockBackSpeed++;
			if (knockBackSpeed > 0)
			{
				knockBackSpeed = 0;
			}
		}
		else
		{
			knockBackSpeed--;
			if (knockBackSpeed < 0)
			{
				knockBackSpeed = 0;
			}
		}
	}

	character->SetPos(pos);
}
