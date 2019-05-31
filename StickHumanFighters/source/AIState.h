#pragma once

#define COMBO_BREAK_CNT (3)
#define WAIT_TIMER_COUNT 65

class AICharacter;

class AIState
{
public:
	AIState();
	virtual ~AIState();

	virtual void Init(AICharacter* chara) = 0;
	virtual void Update(AICharacter* chara) = 0;
	virtual void CheckHitFlag(AICharacter* character);

protected:

	int stateTime;		// ���݂̃X�e�[�g�̎��s��ԃJ�E���g
};

