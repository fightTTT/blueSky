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

	bool GameDraw(void);		// �`�揈��

	sharedListObj objList;
};

