#pragma once

#include "AIState.h"
#include "VECTOR2.h"

class DamageState :
	public AIState
{
public:

	static DamageState *GetInstance()
	{
		static DamageState s_Instance;
		return &s_Instance;
	}

private:

	DamageState();
	~DamageState();

	void Init(AICharacter* character);
	void Update(AICharacter* character);
	void CheckHitFlag(AICharacter* character) {}

	VECTOR2 knockBackSpeed;
	int comboCnt;

	DamageState(const DamageState&);
	void operator=(const DamageState&) {}
};