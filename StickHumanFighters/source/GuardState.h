#pragma once

#include "AIState.h"

class GuardState :
	public AIState
{
public:

	GuardState();
	~GuardState();

private:

	void Init(AICharacter* character);
	void Update(AICharacter* character, const int level);
	void CheckHitFlag(AICharacter* character);

	bool guardHitFlag;
	int knockBackSpeed;
};

