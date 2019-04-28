#pragma once
#include "BaseScene.h"
#include "classObj.h"
#include "SelectScene.h"
#include "TurnThePage.h"
#include "VECTOR2.h"
#include "GameController.h"

using namespace std;

// ｽﾃｰｼﾞID
enum STAGE_ID
{
	STAGE_SCHOOL,			// 校舎
	STAGE_TREE_OF_LEGEND,	// 伝説の木
	STAGE_ARCADE,			// 商店街
	STAGE_RANDUM,			// ﾗﾝﾀﾞﾑ
	STAGE_MAX
};

typedef struct {
	STAGE_ID	id;
	VECTOR2		drawPos;
	int			imageL;
	int			imageR;
	VECTOR2		startPos;
	VECTOR2		endPos;
}StageTbl;



class StageSelectScene :
	public BaseScene
{
public:
	StageSelectScene(std::array<CHAR_ID,PLAYER_MAX>ID, int num);
	~StageSelectScene();

	unique_Base Update(unique_Base own, const GameController &ctrl);
	
private:

	bool PageDraw();

	bool Draw(void);

	int Init(void);

	STAGE_ID stageID;	// 選択ｽﾃｰｼﾞID
	STAGE_ID stageDrawID;
	CHAR_ID ID[4];
	int num;

	array< string, static_cast<size_t>(STAGE_MAX) > sceneName;			// CHAR_IDの順番に対応したｷｬﾗの名前

	array<int, (static_cast<int>(STAGE_MAX)-1)>	gameSceneImage;

	array<array<StageTbl, 2>, 2>	stageTbl;

	array<array<VECTOR2, 2>, 4>		dirPosTbl;		// 矢印画像の座標用
	array<array<double,  2>, 4>		dirRotaTbl;		// 矢印画像の回転角度

	TurnThePage turnThePage;	// ｷｬﾗｾﾚに戻るときに使用
	int pageImage;

	MODE mode;
	bool endFlag;		// ステージを決定していればtrue;
	int flashDrawCnt;	// 点滅処理用ｶｳﾝﾄ

	VECTOR2 cursor;
	VECTOR2 tblPos;

	double flashCnt;

	GameController controller;		// ｺﾝﾄﾛｰﾗｰ情報
};

