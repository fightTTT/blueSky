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

	void Init(AICharacter* character);
	void Update(AICharacter* character);
	void CheckHitFlag(AICharacter* character);
};

