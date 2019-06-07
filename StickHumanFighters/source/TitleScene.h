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

	int frame;						// ÌÚ°Ñ” (¼°İˆÚs’¼‘O‚Í5”{‘¬‚É‚µ‚Ä‚¢‚é)

	bool sceneChangeFlag;			// ¼°İ‚ğØ‚è‘Ö‚¦‚Ä‚à‚¢‚¢‚©‚ÌÌ×¸Ş (true: Ø‚è‘Ö‚¦‚Ä‚à‚æ‚¢, false: Ø‚è‘Ö‚¦‚Ä‚Í‚¢‚¯‚È‚¢)
	int sceneChangeTime;			// sceneChangeFlag‚ªtrue‚É‚È‚Á‚Ä‚©‚ç¼°İˆÚs‚·‚é‚Ü‚Å‚ÌŠÔ

	int cutInFlag;
	bool flashFlag;

	VECTOR2 redManPos;
	VECTOR2 blueManPos;

	std::array<VECTOR2, MODE_MAX> posTbl;
};

