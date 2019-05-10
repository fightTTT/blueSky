#include "JumpState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "SceneMng.h"

#include "DxLib.h"

#define JUMP_SPEED_X (4)
#define JUMP_SPEED_Y (30)

JumpState::JumpState()
{
}

JumpState::~JumpState()
{
}

void JumpState::Init(AICharacter * chara)
{
	stateTime = 0;
	jumpSpeed = { 0, 0 };
	jumpFlag = false;
}

void JumpState::Update(AICharacter * character)
{
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();

	if (!jumpFlag)
	{
		int rand = GetRand(3);

		if (rand == 0)
		{
			// �E��
			jumpSpeed = { JUMP_SPEED_X, -JUMP_SPEED_Y };
			jumpFlag = true;

			if (charaDir == DIR_LEFT)
			{
				character->SetAnim("�W�����v_���");
			}
			else
			{
				character->SetAnim("�W�����v_�O");
			}
		}
		else if (rand == 1)
		{
			// ����
			jumpSpeed = { -JUMP_SPEED_X, -JUMP_SPEED_Y };
			jumpFlag = true;

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
			character->SetAnim("�W�����v_��");
		}
	}
	else
	{
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	auto ssize = lpSceneMng.GetScreenSize();

	if (pos.y > ssize.y)
	{
		pos.y = ssize.y;
		jumpFlag = false;
		character->ChangeState(MoveState::GetInstance());
	}

	character->SetPos(pos);
}
