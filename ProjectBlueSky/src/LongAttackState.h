#pragma once
#include "AIState.h"

class LongAttackState :
	public AIState
{
public:

	static LongAttackState *GetInstance()
	{
		static LongAttackState s_Instance;
		return &s_Instance;
	}

private:

	LongAttackState();
	~LongAttackState();

	void Init(AICharacter* character);
	void Update(AICharacter* character);

	int shotCount;		// 弾の発射カウント
	int spEndCnt;		// 必殺技の終了カウント

	LongAttackState(const LongAttackState&);
	void operator=(const LongAttackState&) {}
};

