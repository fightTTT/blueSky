#pragma once
#include "VECTOR2.h"
#include "BaseScene.h"
#include "ObjList.h"

#define CHAR_CNT_MAX (8)

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	VECTOR2 scSize;

	int Init(void);
	void SelectDraw(void);
	int charID;
	sharedListObj objList;

	std::array<VECTOR2, CHAR_CNT_MAX> posTbl;			// ·¬×ID‚É‘Î‰ž‚µ‚½ˆÊ’u‚ÌÃ°ÌÞÙ
};

