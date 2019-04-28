#pragma once
#include "VECTOR2.h"
#include "BaseScene.h"
#include "classObj.h"
#include "TurnThePage.h"
#include "GameController.h"
#include "Fader.h"


// 定義
#define PLAYER_MAX 4	// ﾌﾟﾚｲﾔｰ数

// ｷｬﾗｸﾀｰID
enum CHAR_ID {
	ID_NON,		// 無し
	ID_Umb,		// 傘
	ID_Bro,		// 箒
	ID_Lid,		// 蓋
	ID_Rnd,		// ﾗﾝﾀﾞﾑ
	ID_Max
};

// ｼｰﾝ遷移時のﾓｰﾄﾞ切替用
enum MODE {
	MODE_SCENE_BACK,	// 前のｼｰﾝへ遷移するとき
	MODE_SCENE_NEXT,	// 次のｼｰﾝへ遷移するとき
	MODE_SCENE_NOW,		// ｼｰﾝ遷移なし
	MODE_MAX
};

// ﾌﾟﾚｲﾔｰ用構造体
typedef struct {
	CHAR_ID ID;		// 選択ｷｬﾗID
	bool flag;		// 選択状態
	int posY;		// 選択座標
	int cnt;		// ｱｲｺﾝ点滅用ｶｳﾝﾄ
}PlayerInfo;

class SelectScene :
	public BaseScene
{
public:
	SelectScene(bool whiteInFlag = false);	// whiteInFlag:ture　ﾎﾜｲﾄｲﾝ有		 whiteInFlag:false ﾎﾜｲﾄｲﾝなし
	~SelectScene();

	unique_Base Update(unique_Base own, const GameController &controller);

private:
	bool PageDraw(void);
	bool Draw(void);
	int SelectEndNum(void);			// ｷｬﾗ選択flag = tureの人数を返す 

	int Init(void);

	GameController controller;		// ｺﾝﾄﾛｰﾗｰ情報

	std::array<std::string, ID_Max> charName;			// CHAR_IDの順番に対応したｷｬﾗの名前
	PlayerInfo player[PLAYER_MAX];	// ﾌﾟﾚｲﾔｰ用配列

	int joinPlayerNum;				// 参加人数

	int flashDrawCnt;				// STARTでゲーム開始の点滅用

	std::array<CHAR_ID, PLAYER_MAX> playerIdArray;
	TurnThePage turnThePage;

	MODE mode;

	Fader<double> alpha;			// ﾌｪｰﾄﾞｲﾝの画面透明度
};


