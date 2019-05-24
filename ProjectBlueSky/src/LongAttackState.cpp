#include "LongAttackState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "DamageState.h"
#include "Collision.h"

LongAttackState::LongAttackState()
{
}

LongAttackState::~LongAttackState()
{
}

void LongAttackState::Init(AICharacter * character)
{
	shotCount = 0;
	spEndCnt = 0;
}

void LongAttackState::Update(AICharacter * character)
{
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto charaAnim = character->GetAnim();

	if (character->GetAnimEndFlag())
	{
		character->ChangeState(MoveState::GetInstance());
	}

	if(charaAnim == "旋風脚")
	{
		if (dir == DIR_RIGHT)
		{
			pos.x += 6;
		}
		else
		{
			pos.x -= 6;
		}

		if (character->GetAnimCount() > 60)
		{
			character->SetDirChange(true);
			character->SetAnim("待機");
			character->ChangeState(MoveState::GetInstance());
			return;
		}
	}
	else if (charaAnim == "ミサイルアロー" || charaAnim == "タックル")
	{
		if (character->GetAnimStopFlag())
		{
			spEndCnt++;

			if (spEndCnt < 45)
			{
				if (spEndCnt > 4)
				{
					if (dir == DIR_RIGHT)
					{
						pos.x += 20;
					}
					else
					{
						pos.x -= 20;
					}
				}
			}
			else
			{
				character->SetAnimStopFlag(false);
			}
		}
		else
		{
			if (character->GetAnimCount() == 9)
			{
				character->SetAnimStopFlag(true);
				spEndCnt = 0;
			}
		}
	}
	else
	{
		shotCount++;
	}

	if (shotCount == character->GetShotCreateCount())
	{
		character->SetLongAttackFlag(true);
	}

	character->SetPos(pos);
}
