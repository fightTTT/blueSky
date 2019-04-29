#pragma once
#include "BaseScene.h"

#include"ObjList.h"

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);		// –ˆÌÚ°Ñˆ—‚·‚é‚à‚Ì‚ğ‚Ü‚Æ‚ß‚½ŠÖ”Astate

private:
	int Init(void);

	bool GameDraw(void);		// •`‰æˆ—

	sharedListObj objList;
};

