#pragma once
#include "AIState.h"

class AttackState :
	public AIState
{
public:

	static AttackState *GetInstance()
	{
		static AttackState s_Instance;
		return &s_Instance;
	}

private:

	AttackState();
	~AttackState();

	void Init(AICharacter* chara);
	void Update(AICharacter* character);
};

