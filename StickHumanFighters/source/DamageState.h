#pragma once

#include "AIState.h"
#include "VECTOR2.h"

class DamageState :
	public AIState
{
public:

	DamageState();
	~DamageState();

private:

	void Init(AICharacter* character);
	void Update(AICharacter* character, const int level);
	void CheckHitFlag(AICharacter* character) {}

	VECTOR2 knockBackSpeed;
	int comboCnt;
};