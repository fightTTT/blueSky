#include "AttackState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "Collision.h"
#include "DamageState.h"
#include "JumpState.h"
#include "SceneMng.h"

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
	auto charaAnim = character->GetAnim();

	// すでに攻撃アニメーションがセットされていた場合return
	if (charaAnim != "前移動" && charaAnim != "後ろ移動" && charaAnim != "待機")
	{
		character->SetDirChange(false);
		return;
	}

	if (attackCount >= static_cast<unsigned int>(GetRand(30) + 4))
	{
		attackCount = 0;
		character->SetDirChange(false);
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

	character->SetDirChange(false);
}

void AttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->SetDirChange(true);
		character->SetAnim("待機");
		character->ChangeState(MoveState::GetInstance());
		return;
	}

	auto ssize = lpSceneMng.GetScreenSize();
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto charaAnim = character->GetAnim();

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
	else if (charaAnim == "カンフーキック")
	{
		if (character->GetAnimCount() < 4)
		{
			if (dir == DIR_RIGHT)
			{
				pos.x += 6;
			}
			else
			{
				pos.x -= 6;
			}
		}
	}
	else if (charaAnim == "ラッシュ")
	{
		if (character->GetAnimCount() > 40)
		{
			character->SetDirChange(true);
			character->SetAnim("待機");
			character->ChangeState(MoveState::GetInstance());
			return;
		}
	}
	else if (charaAnim == "ランキャク")
	{
		if (dir == DIR_RIGHT)
		{
			pos.x++;
		}
		else
		{
			pos.x--;
		}

		pos.y--;
	}
	else if (charaAnim == "地面割" || charaAnim == "かかと落とし")
	{
		if (character->GetAnimCount() < 30)
		{
			if (dir == DIR_RIGHT)
			{
				pos.x++;
			}
			else
			{
				pos.x--;
			}

			pos.y -= 8;
		}
		else
		{
			pos.y += 20;
		}
	}

	character->SetPos(pos);
}
