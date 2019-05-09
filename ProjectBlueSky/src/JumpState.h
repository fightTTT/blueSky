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

	void Init(AICharacter* chara);
	void Update(AICharacter* character);


	bool jumpFlag;
	VECTOR2 jumpSpeed;
};

