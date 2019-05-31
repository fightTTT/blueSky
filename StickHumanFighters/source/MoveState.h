#pragma once

#include "AIState.h"

class MoveState :
	public AIState
{
public:

	MoveState();
	~MoveState();

private:


	void Init(AICharacter* character);
	void Update(AICharacter* character, const int level);

	bool moveDirFlag;						// true : �O�ړ� false : ���ړ�
	unsigned int changeGuardCount;			// �X�e�[�g���K�[�h�ɕύX����J�E���g
	unsigned int changeWaitStateCount;		// �ҋ@��Ԃɓ���J�E���g
	unsigned int notAttackCount;			// �U�����Ă��Ȃ���Ԃ̃J�E���g
	unsigned int moveFrontCount;			// �O�ړ��������Ԃ̃J�E���g
};

