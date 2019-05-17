#pragma once

#include "AIState.h"
#include "VECTOR2.h"

class JumpState :
	public AIState
{
public:

	static JumpState *GetInstance()
	{
		static JumpState s_Instance;
		return &s_Instance;
	}

private:

	JumpState();
	~JumpState();

	void Init(AICharacter* character);
	void Update(AICharacter* character);


	bool jumpFlag;
	VECTOR2 jumpSpeed;
};

