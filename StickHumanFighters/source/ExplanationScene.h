#pragma once
#include "BaseScene.h"

#include <vector>

class ExplanationScene :
	public BaseScene
{
public:
	ExplanationScene();
	~ExplanationScene();

	// ���ڰэs������
	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	std::vector<int> imageHandle;	// �摜�������

	int page;						// ���݂��߰��

	int frame;						// �ڰѐ� (��݈ڍs���O��5�{���ɂ��Ă���)

	bool sceneChangeFlag;			// ��݂�؂�ւ��Ă����������׸� (true: �؂�ւ��Ă��悢, false: �؂�ւ��Ă͂����Ȃ�)
	int sceneChangeTime;			// sceneChangeFlag��true�ɂȂ��Ă��缰݈ڍs����܂ł̎���

	// �������֐�
	int Init();

	// �`��֐�
	void ExplanationDraw();
};
