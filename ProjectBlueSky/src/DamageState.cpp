#include "DamageState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "SceneMng.h"

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

	if (comboCnt >= COMBO_BREAK_CNT)
	{
		character->SetAnim("�_���[�W_�_�E��");

		if (dir == DIR_RIGHT)
		{
			knockBackSpeed = { -FALL_SPEED_X, -FALL_SPEED_Y };
		}
		else
		{
			knockBackSpeed = { FALL_SPEED_X, -FALL_SPEED_Y };
		}

		comboCnt = 0;
	}
	else
	{
		character->SetAnim("�_���[�W_����");

		if (dir == DIR_RIGHT)
		{
			knockBackSpeed = { -KNOCK_BACK_SPEED, 0 };
		}
		else
		{
			knockBackSpeed = { KNOCK_BACK_SPEED, 0 };
		}
	}
}

void DamageState::Update(AICharacter * character)
{
	auto dir = character->GetDir();
	auto pos = character->GetPos();
	auto ssize = lpSceneMng.GetScreenSize();

	if (character->GetAnim() == "�_���[�W_����")
	{
		if (character->GetAnimEndFlag())
		{
			character->ChangeState(MoveState::GetInstance());
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
	else if (character->GetAnim() == "�_���[�W_�_�E��")
	{
		if (character->GetAnimEndFlag())
		{
			character->SetAnim("�N���オ��");
		}

		knockBackSpeed.y++;
		pos.y += knockBackSpeed.y;

		if (pos.y > ssize.y)
		{
			pos.y = ssize.y;

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
	else if (character->GetAnim() == "�N���オ��")
	{
		if (character->GetAnimEndFlag())
		{
			character->ChangeState(MoveState::GetInstance());
		}

	}

	character->SetPos(pos);
}
