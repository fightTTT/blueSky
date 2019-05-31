#pragma once

#include "AIState.h"

class WaitState :
	public AIState
{
public:

	WaitState();
	~WaitState();

private:

	void Init(AICharacter* character);
	void Update(AICharacter* character);
};

