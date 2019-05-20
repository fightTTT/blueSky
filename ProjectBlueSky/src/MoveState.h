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

	void Init(AICharacter* character);
	void Update(AICharacter* character);

	bool moveDirFlag;				// true : 前移動 false : 後ろ移動
	int changeGuardCount;			// ステートをガードに変更するカウント
	int changeGuardStateCount;		// ガード状態に入るカウント
	int changeWaitStateCount;		// 待機状態に入るカウント
	int notAttackCount;				// 攻撃していない状態のカウント
	int moveFrontCount;				// 前移動持続時間のカウント

	MoveState(const MoveState&);
	void operator=(const MoveState&) {}
};

