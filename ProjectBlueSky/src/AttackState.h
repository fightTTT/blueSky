#pragma once
#include "AIState.h"

class AttackState :
	public AIState
{
public:

	AttackState();
	~AttackState();

private:


	void Init(AICharacter* character);
	void Update(AICharacter* character);

	unsigned int attackCount;
	bool warpFlag;
};

