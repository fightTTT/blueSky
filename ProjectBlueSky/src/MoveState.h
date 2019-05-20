#pragma once

#include "AIState.h"

class MoveState :
	public AIState
{
public:

	static MoveState *GetInstance()
	{
		static MoveState s_Instance;
		return &s_Instance;
	}

private:

	MoveState();
	~MoveState();

	void Init(AICharacter* character);
	void Update(AICharacter* character);

	bool moveDirFlag;				// true : �O�ړ� false : ���ړ�
	int changeGuardCount;			// �X�e�[�g���K�[�h�ɕύX����J�E���g
	int changeGuardStateCount;		// �K�[�h��Ԃɓ���J�E���g
	int changeWaitStateCount;		// �ҋ@��Ԃɓ���J�E���g
	int notAttackCount;				// �U�����Ă��Ȃ���Ԃ̃J�E���g
	int moveFrontCount;				// �O�ړ��������Ԃ̃J�E���g

	MoveState(const MoveState&);
	void operator=(const MoveState&) {}
};

