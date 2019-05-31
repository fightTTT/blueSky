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
	void Update(AICharacter* character, const int level);

	unsigned int attackCount;
};

