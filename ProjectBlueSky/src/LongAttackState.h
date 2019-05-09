#pragma once
#include "AIState.h"

class LongAttackState :
	public AIState
{
public:

	static LongAttackState *GetInstance()
	{
		static LongAttackState s_Instance;
		return &s_Instance;
	}

private:

	LongAttackState();
	~LongAttackState();

	void Init(AICharacter* chara);
	void Update(AICharacter* character);
};

