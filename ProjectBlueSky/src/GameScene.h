#pragma once
#include "BaseScene.h"

#include"ObjList.h"

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);		// ���ڰя���������̂��܂Ƃ߂��֐��Astate

private:
	int Init(void);
	int bgPos_x;
	VECTOR2 ssize;

	bool GameDraw(void);		// �`�揈��
	int id[2];			// �����蔻��̃t���[����

	sharedListObj objList;
};

