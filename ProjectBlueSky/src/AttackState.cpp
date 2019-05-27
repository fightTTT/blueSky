#include "AttackState.h"
#include "AICharacter.h"
#include "Collision.h"
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
		character->ChangeState("Jump");
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
		character->ChangeState("Move");
		return;
	}

	auto ssize = lpSceneMng.GetScreenSize();
	auto pos = character->GetPos();
	auto dir = character->GetDir();
	auto charaAnim = character->GetAnim();
	auto hitData = character->GetHitData();

	if (charaAnim == "�J���t�[�L�b�N")
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
			character->ChangeState("Move");
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
	else if (charaAnim == "�R��Ԃ�")
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
