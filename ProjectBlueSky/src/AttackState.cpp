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

	// ���łɍU���A�j���[�V�������Z�b�g����Ă����ꍇreturn
	if (charaAnim != "�O�ړ�" && charaAnim != "���ړ�" && charaAnim != "�ҋ@")
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
		character->SetAnim("�p���`_��");
	}
	else if (rand == 1)
	{
		character->SetAnim("�p���`_��");
	}
	else if (rand == 2)
	{
		character->SetAnim("�L�b�N_��");
	}
	else if (rand == 3)
	{
		character->SetAnim("�L�b�N_��");
	}
	else if (rand == 4)
	{
		character->SetAnim("�p���`_��_���Ⴊ��");
	}
	else if (rand == 5)
	{
		character->SetAnim("�p���`_��_���Ⴊ��");
	}
	else if (rand == 6)
	{
		character->SetAnim("�L�b�N_��_���Ⴊ��");
	}
	else if (rand == 7)
	{
		character->SetAnim("�L�b�N_��_���Ⴊ��");
	}

	character->SetDirChange(false);
}

void AttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->SetDirChange(true);
		character->SetAnim("�ҋ@");
		character->ChangeState(MoveState::GetInstance());
		return;
	}

	auto ssize = lpSceneMng.GetScreenSize();
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto charaAnim = character->GetAnim();

	if (charaAnim == "���[�����O�A�^�b�N")
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
			character->SetAnim("�ҋ@");
			character->ChangeState(MoveState::GetInstance());
			return;
		}
	}
	else if (charaAnim == "�J���t�[�L�b�N")
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
	else if (charaAnim == "���b�V��")
	{
		if (character->GetAnimCount() > 40)
		{
			character->SetDirChange(true);
			character->SetAnim("�ҋ@");
			character->ChangeState(MoveState::GetInstance());
			return;
		}
	}
	else if (charaAnim == "�����L���N")
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
	else if (charaAnim == "�n�ʊ�" || charaAnim == "�����Ɨ��Ƃ�")
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
