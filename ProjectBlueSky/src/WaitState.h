#pragma once

#include "AIState.h"

class WaitState :
	public AIState
{
public:

	static WaitState *GetInstance()
	{
		static WaitState s_Instance;
		return &s_Instance;
	}

private:

	WaitState();
	~WaitState();

	void Init(AICharacter* character);
	void Update(AICharacter* character);

	WaitState(const WaitState&);
	void operator=(const WaitState&) {}
};

