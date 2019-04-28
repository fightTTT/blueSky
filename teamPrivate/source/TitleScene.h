#pragma once
#include <list>
#include "BaseScene.h"
#include "classObj.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own, const GameController &controller);

private:
	bool TitleDraw(void);	// 描画
	bool PageDraw(void);

	int Init(void);	// 初期化用

	sharedListObj objList;
	int Cnt;				// ﾀｲﾄﾙ点滅用ｶｳﾝﾄ
	int flasing;			// 点滅の激しさ
	int ChangeSceneFlag;	// 画面遷移用
	int startPadNum;		// ｽﾀｰﾄﾎﾞﾀﾝを押したPAD確認用

	TurnThePage turnThePage;
	MODE mode;
};

