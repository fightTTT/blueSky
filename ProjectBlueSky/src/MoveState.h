#pragma once

#include "AIState.h"

class MoveState :
	public AIState
{
public:

	MoveState();
	~MoveState();

private:


	void Init(AICharacter* character);
	void Update(AICharacter* character);

	bool moveDirFlag;						// true : 前移動 false : 後ろ移動
	unsigned int changeGuardCount;			// ステートをガードに変更するカウント
	unsigned int changeGuardStateCount;		// ガード状態に入るカウント
	unsigned int changeWaitStateCount;		// 待機状態に入るカウント
	unsigned int notAttackCount;			// 攻撃していない状態のカウント
	unsigned int moveFrontCount;			// 前移動持続時間のカウント
	unsigned int guardHitCount;				// ガードを連続でした回数
};

