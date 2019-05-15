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
	void CheckHitFlag(AICharacter* character);

	bool moveDirFlag;				// true : �O�ړ� false : ���ړ�
	bool changeAttackFlag;			// �X�e�[�g��Attack�ɕύX����t���O
	int changeGuardStateCount;		// �K�[�h��Ԃɓ���J�E���g
};

