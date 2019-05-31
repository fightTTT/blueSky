#include "GuardState.h"
#include "AICharacter.h"
#include "Collision.h"
#include "SoundMng.h"

#include "DxLib.h"

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

void GuardState::Update(AICharacter * character, const int level)
{
	auto enemy = character->GetEnemyState();

	if (stateTime >= 20 && knockBackSpeed == 0)
	{
		character->ChangeState("Move");
		character->SetAnim("後ろ移動");
		return;
	}

	if (enemy.enemyAnimAttribute[0] == ANIM_ATTRIBUTE_SQUAT)
	{
		character->SetAnim("しゃがみ_後ろ");
	}

	if (enemy.enemyAnimAttribute[0] == ANIM_ATTRIBUTE_AIR)
	{
		character->SetAnim("しゃがみ_後ろ");
	}

	if ((enemy.enemyAnimAttribute[1] != ANIM_ATTRIBUTE_ATTACK_SMALL)
		&& (enemy.enemyAnimAttribute[1] != ANIM_ATTRIBUTE_ATTACK_BIG)
		&& (enemy.enemyAnimAttribute[1] != ANIM_ATTRIBUTE_ATTACK_SP))
	{
		stateTime++;
	}

}

void GuardState::CheckHitFlag(AICharacter * character)
{
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto hitData = character->GetHitData();

	auto hitFlag = hitData.hitFlag && (hitData.colType == COLTYPE_GUARD);

	// 攻撃を受けた時のノックバック
	if (!guardHitFlag && hitFlag)
	{
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
		PlaySoundMem(SOUND_ID("se/battle/guard.wav"), DX_PLAYTYPE_BACK);

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

	AIState::CheckHitFlag(character);
}
