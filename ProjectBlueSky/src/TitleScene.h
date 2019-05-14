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

	std::array<VECTOR2, MODE_MAX> posTbl;
};

