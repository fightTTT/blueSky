#pragma once
#include <array>
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
	int itvCnt;				// ｲﾝﾀｰﾊﾞﾙ用ｶｳﾝﾄ
	int flamCnt;			// 点滅用
	bool sceneChangeFlag;	// ｼｰﾝを変更するﾌﾗｸﾞ
	bool commandTableFlag1;	// 1Pのｺﾏﾝﾄﾞ表を表示するﾌﾗｸﾞ
	bool commandTableFlag2;	// 1Pのｺﾏﾝﾄﾞ表を表示するﾌﾗｸﾞ

	int Init(void);
	void SelectDraw(void);
	sharedListObj objList;

	std::array<std::array<char, 12>, CHAR_CNT_MAX> charNameTbl;				// ｷｬﾗIDに対応した名前のﾃｰﾌﾞﾙ
};