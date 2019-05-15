#pragma once

#include "AIState.h"

class GuardState :
	public AIState
{
public:

	static GuardState *GetInstance()
	{
		static GuardState s_Instance;
		return &s_Instance;
	}

private:

	GuardState();
	~GuardState();

	void Init(AICharacter* character);
	void Update(AICharacter* character);

	bool guardHitFlag;
	int knockBackSpeed;
};

