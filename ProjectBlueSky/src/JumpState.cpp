#include "JumpState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "SceneMng.h"
#include "Collision.h"
#include "DamageState.h"

#include "DxLib.h"

#define JUMP_SPEED_X (4)
#define JUMP_SPEED_Y (30)

JumpState::JumpState()
{
}

JumpState::~JumpState()
{
}

void JumpState::Init(AICharacter * character)
{
	stateTime = 0;
	jumpSpeed = { 0, 0 };
	jumpFlag = false;
}

void JumpState::Update(AICharacter * character)
{
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto enemy = character->GetEnemyState();
	auto jumpType = character->GetJumpType();

	// �W�����v�����t���O
	bool jumpLeft = true;
	bool jumpRight = true;

	// �W�����v�^�C�v�t���O
	bool jumpTypeLeft = false;
	bool jumpTypeRight = false;

	int randLeft = 0;
	int randRight = 0;

	if (jumpType == JUMP_TYPE_RAND)
	{
		// �O�ɃW�����v����悤�Ɍ����Ă�������ȊO��false
		if (charaDir == DIR_LEFT)
		{
			jumpLeft = false;
			randRight = GetRand(10);
		}
		else if (charaDir == DIR_RIGHT)
		{
			jumpRight = false;
			randLeft = GetRand(10);
		}
	}
	else if (jumpType == JUMP_TYPE_FRONT)
	{
		// �O�ɃW�����v����悤�Ɍ����Ă�������ȊO��false
		if (charaDir == DIR_LEFT)
		{
			jumpLeft = false;
			randRight = 10;
		}
		else if (charaDir == DIR_RIGHT)
		{
			jumpRight = false;
			randLeft = 10;
		}
	}
	else if (jumpType == JUMP_TYPE_BACK)
	{
		// ���ɃW�����v����悤�Ɍ����Ă��������true
		if (charaDir == DIR_LEFT)
		{
			jumpLeft = true;
			randLeft = 10;
		}
		else if (charaDir == DIR_RIGHT)
		{
			jumpRight = true;
			randRight = 10;
		}
	}
	else if (jumpType == JUMP_TYPE_UP)
	{
		randLeft = 0;
		randRight = 0;
	}

	if (!jumpFlag)
	{
		if (randLeft >= 7 && jumpLeft)
		{
			// �E��
			jumpSpeed = { JUMP_SPEED_X, -JUMP_SPEED_Y };
			jumpFlag = true;
			character->SetDirChange(false);

			if (charaDir == DIR_LEFT)
			{
				character->SetAnim("�W�����v_���");
			}
			else
			{
				character->SetAnim("�W�����v_�O");
			}
		}
		else if (randRight >= 7 && jumpRight)
		{
			// ����
			jumpSpeed = { -JUMP_SPEED_X, -JUMP_SPEED_Y };
			jumpFlag = true;
			character->SetDirChange(false);

			if (charaDir == DIR_LEFT)
			{
				character->SetAnim("�W�����v_�O");
			}
			else
			{
				character->SetAnim("�W�����v_���");
			}
		}
		else
		{
			// ��
			jumpSpeed = { 0, -JUMP_SPEED_Y };
			jumpFlag = true;
			character->SetDirChange(false);
			character->SetAnim("�W�����v_��");
		}
	}
	else
	{
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	auto ssize = lpSceneMng.GetScreenSize();

	// ���n
	if (jumpSpeed.y == JUMP_SPEED_Y)
	{
		pos.y = ssize.y;
		jumpFlag = false;
		character->SetDirChange(true);
		character->ChangeState(MoveState::GetInstance());
	}

	// �W�����v���ɉ�ʊO�ɂ�������߂�
	if (jumpFlag && (pos.x > ssize.x || pos.y < 0))
	{
		pos.x -= jumpSpeed.x;
	}

	character->SetPos(pos);

	auto distance = enemy.enemyPos - pos;

	if (abs(distance.x) < 200 && jumpSpeed.y > -10)
	{
		if (distance.x < 0)
		{
			character->SetDir(DIR_LEFT);
		}
		else
		{
			character->SetDir(DIR_RIGHT);
		}
		character->SetAnim("�L�b�N_��_��");
	}
}
