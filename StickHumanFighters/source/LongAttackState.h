#pragma once
#include "AIState.h"

class LongAttackState :
	public AIState
{
public:

	LongAttackState();
	~LongAttackState();

private:


	void Init(AICharacter* character);
	void Update(AICharacter* character, const int level);

	int shotCount;		// 弾の発射カウント
	int spEndCnt;		// 必殺技の終了カウント
};

