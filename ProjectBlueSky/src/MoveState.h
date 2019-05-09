#pragma once

#include "AIState.h"

class MoveState :
	public AIState
{
public:

	static MoveState *GetInstance()
	{
		static MoveState s_Instance;
		return &s_Instance;
	}

private:

	MoveState();
	~MoveState();

	void Init(AICharacter* chara);
	void Update(AICharacter* character);

	bool moveDirFlag;				// true : ‘OˆÚ“® false : Œã‚ëˆÚ“®
};

