#pragma once
#include "BaseScene.h"

#include <array>
#include "VECTOR2.h"
#include "SceneMng.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	int Init();
	void TitleDraw();

	VECTOR2 ssize;
	bool m_selFlag;
	MODE mode;

	int frame;						// �ڰѐ� (��݈ڍs���O��5�{���ɂ��Ă���)

	bool sceneChangeFlag;			// ��݂�؂�ւ��Ă����������׸� (true: �؂�ւ��Ă��悢, false: �؂�ւ��Ă͂����Ȃ�)
	int sceneChangeTime;			// sceneChangeFlag��true�ɂȂ��Ă��缰݈ڍs����܂ł̎���

	int cutInFlag;
	bool flashFlag;

	VECTOR2 redManPos;
	VECTOR2 blueManPos;

	std::array<VECTOR2, MODE_MAX> posTbl;
};

