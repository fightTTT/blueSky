#pragma once

class AICharacter;

class AIState
{
public:
	AIState();
	virtual ~AIState();

	virtual void Init(AICharacter* chara) = 0;
	virtual void Update(AICharacter* chara) = 0;
	virtual void CheckHitFlag(AICharacter* chara) = 0;

protected:

	int stateTime;		// ���݂̃X�e�[�g�̎��s��ԃJ�E���g
};

