#include "AttackState.h"
#include "AICharacter.h"
#include "Collision.h"
#include "SceneMng.h"

#include "DxLib.h"

#define JUMP_RAND (10)
#define JUMP_RAND_OFFSET (5)

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
		return;
	}

	// 攻撃を行った回数が多くなったらジャンプに切り替える
	if (attackCount >= static_cast<unsigned int>(GetRand(JUMP_RAND) + JUMP_RAND_OFFSET))
	{
		attackCount = 0;
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState("Jump");
		return;
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
}

void AttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		attackCount++;
		character->SetAnim("待機");
		character->ChangeState("Move");
		return;
	}

	auto ssize = lpSceneMng.GetScreenSize();
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto charaAnim = character->GetAnim();
	auto hitData = character->GetHitData();

	if (charaAnim == "カンフーキック")
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
			attackCount++;
			character->SetAnim("待機");
			character->ChangeState("Move");
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
	else if (charaAnim == "蹴り返し")
	{
		if (character->GetAnimCount() < 20)
		{
			if (dir == DIR_RIGHT)
			{
				pos.x += 8;
			}
			else
			{
				pos.x -= 8;
			}

			if (character->GetAnimCount() < 10)
			{
				pos.y -= 5;
			}
			else
			{
				pos.y += 5;
			}
		}
		else
		{
			if ((hitData.hitFlag && (hitData.colType == COLTYPE_ATTACK)))
			{
				if (dir == DIR_RIGHT)
				{
					pos.x -= 8;
				}
				else
				{
					pos.x += 8;
				}
			}

			if (character->GetAnimCount() < 33)
			{
				pos.y -= 8;
			}
			else
			{
				pos.y += 8;
			}
		}
	}

	character->SetPos(pos);
}
