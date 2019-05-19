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
	int bgPos_x;
	VECTOR2 ssize;

	bool GameDraw(void);		// •`‰æˆ—
	int id[2];			// “–‚½‚è”»’è‚ÌƒtƒŒ[ƒ€”

	sharedListObj objList;
};

