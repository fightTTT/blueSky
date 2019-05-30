#pragma once

#include "AIState.h"
#include "VECTOR2.h"

class JumpState :
	public AIState
{
public:

	JumpState();
	~JumpState();

private:

	void Init(AICharacter* character);
	void Update(AICharacter* character);

	bool jumpFlag;
	VECTOR2 jumpSpeed;
};

