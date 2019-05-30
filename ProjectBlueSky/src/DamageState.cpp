#include "DamageState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "SoundMng.h"
#include "DxLib.h"

#define KNOCK_BACK_SPEED (10)

#define FALL_SPEED_X (7)
#define FALL_SPEED_Y (15)

DamageState::DamageState()
{
	comboCnt = 0;
}

DamageState::~DamageState()
{
}

void DamageState::Init(AICharacter * character)
{
	auto dir = character->GetDir();

	comboCnt++;

	auto tmp = character->GetAnimAttribute(0) == ANIM_ATTRIBUTE_AIR;

	if (comboCnt >= COMBO_BREAK_CNT || (character->GetAnimAttribute(0) == ANIM_ATTRIBUTE_AIR) || character->GetPlayerHP() == 0)
	{
		character->SetAnim("ダメージ_ダウン");

		PlaySoundMem(SOUND_ID("se/battle/critical.mp3"), DX_PLAYTYPE_BACK);


		if (dir == DIR_RIGHT)
		{
			knockBackSpeed = { -FALL_SPEED_X, -FALL_SPEED_Y };
		}
		else
		{
			knockBackSpeed = { FALL_SPEED_X, -FALL_SPEED_Y };
		}

		WaitTimer(WAIT_TIMER_COUNT);

		comboCnt = 0;
	}
	else
	{
		character->SetAnim("ダメージ_立ち");

		PlaySoundMem(SOUND_ID("se/battle/punch.wav"), DX_PLAYTYPE_BACK);

		if (dir == DIR_RIGHT)
		{
			knockBackSpeed = { -KNOCK_BACK_SPEED, 0 };
		}
		else
		{
			knockBackSpeed = { KNOCK_BACK_SPEED, 0 };
		}

		WaitTimer(WAIT_TIMER_COUNT);
	}
}

void DamageState::Update(AICharacter * character)
{
	auto dir = character->GetDir();
	auto pos = character->GetPos();
	auto ssize = lpSceneMng.GetScreenSize();

	if (character->GetAnim() == "ダメージ_立ち")
	{
		if (character->GetAnimEndFlag())
		{
			character->SetInvincibleTime(10);
			character->ChangeState("Move");
			return;
		}

		pos.x += knockBackSpeed.x;

		if (dir == DIR_RIGHT)
		{
			knockBackSpeed.x++;
			if (knockBackSpeed.x > 0)
			{
				knockBackSpeed.x = 0;
			}
		}
		else
		{
			knockBackSpeed.x--;
			if (knockBackSpeed.x < 0)
			{
				knockBackSpeed.x = 0;
			}
		}
	}
	else if (character->GetAnim() == "ダメージ_ダウン")
	{
		if (character->GetAnimEndFlag() && character->GetPlayerHP() != 0)
		{
			character->SetAnim("起き上がり");
		}

		knockBackSpeed.y++;
		pos.y += knockBackSpeed.y;

		if (pos.y > ssize.y)
		{
			if (dir == DIR_RIGHT)
			{
				knockBackSpeed.x++;
				if (knockBackSpeed.x > 0)
				{
					knockBackSpeed.x = 0;
				}
			}
			else
			{
				knockBackSpeed.x--;
				if (knockBackSpeed.x < 0)
				{
					knockBackSpeed.x = 0;
				}
			}
		}

		pos.x += knockBackSpeed.x;
	}
	else if (character->GetAnim() == "起き上がり")
	{
		if (character->GetAnimEndFlag())
		{
			character->ChangeState("Move");
			character->SetInvincibleTime(20);
		}

	}

	character->SetPos(pos);
}
