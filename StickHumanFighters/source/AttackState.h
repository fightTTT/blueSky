#pragma once
#include "AIState.h"

#include <string>

class AttackState :
	public AIState
{
public:

	AttackState();
	~AttackState();

private:


	void Init(AICharacter* character);
	void Update(AICharacter* character, const int level);

	unsigned int attackCount;
	std::string attackAnimOld;		// 1回前の攻撃のアニメーション情報
};

