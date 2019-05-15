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
	void CheckHitFlag(AICharacter* character);

	bool moveDirFlag;				// true : 前移動 false : 後ろ移動
	bool changeAttackFlag;			// ステートをAttackに変更するフラグ
	int changeGuardStateCount;		// ガード状態に入るカウント
};

