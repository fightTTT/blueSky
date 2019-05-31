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
	void Update(AICharacter* character, const int level);

	int shotCount;		// �e�̔��˃J�E���g
	int spEndCnt;		// �K�E�Z�̏I���J�E���g
};

