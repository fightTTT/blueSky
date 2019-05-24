#include "LongAttackState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "DamageState.h"
#include "Collision.h"
#include "SceneMng.h"

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
	character->SetDirChange(false);
}

void LongAttackState::Update(AICharacter * character)
{
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto charaAnim = character->GetAnim();
	auto ssize = lpSceneMng.GetScreenSize();

	if (character->GetAnimEndFlag())
	{
		character->ChangeState(MoveState::GetInstance());
		character->SetAnimStopFlag(false);
	}
	if (charaAnim == "ローリングアタック")
	{
		if (character->GetAnimCount() > 20)
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

		if (character->GetAnimCount() > 60)
		{
			character->SetDirChange(true);
			character->SetAnim("待機");
			character->ChangeState(MoveState::GetInstance());
			return;
		}
	}
	else if (charaAnim == "ワープ")
	{
		if (character->GetAnimCount() == 31)
		{
			character->SetAnimStopFlag(true);
		}

		if (character->GetAnimStopFlag())
		{
			pos.y = (ssize.y * 2);

			if (dir == DIR_RIGHT)
			{
				if (pos.x > ((ssize.x * 3) / 5))
				{
					dir = character->GetTmpDir();
					character->SetAnimStopFlag(false);
				}
				else
				{
					pos.x += 30;
				}
			}
			else
			{
				if (pos.x < ((ssize.x * 2) / 5))
				{
					dir = character->GetTmpDir();
					character->SetAnimStopFlag(false);
				}
				else
				{
					pos.x -= 30;
				}
			}
		}
	}
	else if(charaAnim == "旋風脚")
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
	else if (charaAnim == "アクセル")
	{
	if (character->GetAnimCount() < 60)
	{
		if (dir == DIR_RIGHT)
		{
			pos.x += 5;
		}
		else
		{
			pos.x -= 5;
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
