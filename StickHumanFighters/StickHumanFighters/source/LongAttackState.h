#pragma once
#include "AIState.h"

class LongAttackState :
	public AIState
{
public:

	LongAttackState();
	~LongAttackState();

private:


	void Init(AICharacter* character);
	void Update(AICharacter* character);

	int shotCount;		// �e�̔��˃J�E���g
	int spEndCnt;		// �K�E�Z�̏I���J�E���g
};

