#pragma once
#include <list>
#include "classObj.h"
#include "classFloor.h"
#include "classChicken.h"
#include "Chicken.h"
#include "BaseScene.h"
#include "SelectScene.h"
#include "StageSelectScene.h"
#include "ExpOrb.h"
#include "Gamecontroller.h"
#include "Fader.h"

#define HP	200

using namespace std;

enum GAME_STATE {
	ENTRY,
	TUTORIAL,
	GRAYOUT_SLIDE,
	INSTRUCTION,
	COUNT_DOWN,
	PLAY,
	PAUSE,
	CUT_IN,
	GAME_FINISH,
	GAME_STATE_MAX
};

enum IMAGE_ID
{
	IMAGE_SCHOOL,
	IMAGE_TREE,
	IMAGE_ARCADE,
	IMAGE_MAX
};

enum CHANGE_SCENE {
	CHANGE_SCENE_NON,			// ｼｰﾝ移行しない
	CHANGE_SCENE_CHAR_SEL,		// ｷｬﾗｾﾚへ
	CHANGE_SCENE_RESULT,		// ﾘｻﾞﾙﾄへ
	CHANGE_SCENE_INSTRUCTION,	// 操作方法を表示
	CHANGE_SCENE_MAX
};

typedef struct {
	std::array<int, PLAYER_MAX> totalGiveDamage;	// ﾄｰﾀﾙで与えたﾀﾞﾒｰｼﾞ
	std::array<int, PLAYER_MAX> deathEnemyCnt;		// ﾄｰﾀﾙで倒した敵の数
	std::array<int, PLAYER_MAX> chickenKillCnt;		// 鶏の殺した数
	std::array<int, PLAYER_MAX> chickenCnt;			// 鳥の繁栄数	殺される度復活するので殺された回数
	std::array<int, PLAYER_MAX> deathOrder;			// 死んだ順番	0=生きている　1以上=死んだ順番
	std::array<std::string, PLAYER_MAX> charName;	// ｷｬﾗ名
}RESULT_INFO;

typedef struct {
	VECTOR2 pos;			// 必殺技発動者の座標
	int padNum;				// 必殺技発動者のﾊﾟｯﾄﾞ番号
	int cnt;
	bool flag;
}CUT_IN_INFO;


class GameScene :
	public BaseScene
{
public:
	GameScene();
	GameScene(CHAR_ID playerID[PLAYER_MAX], int joinPlayerNum, STAGE_ID ID);
	~GameScene();
	unique_Base Update(unique_Base own, const GameController &controller);

private:
	void GamePlay(bool doCollision);		// 引数(true:当たり判定を行う false:当たり判定を行わない)
	void PlayerIns();						// ﾌﾟﾚｲﾔを1ｷｬﾗずつｲﾝｽﾀﾝｽする
	void GameEntry(void);					// ｴﾝﾄﾘｰ
	void Tutorial(void);					// ﾁｭｰﾄﾘｱﾙ
	void GrayOutSlide(void);				// ｸﾞﾚｰｱｳﾄを上にｽﾗｲﾄﾞする処理
	void Instruction(void);					// 操作説明表示
	void CountDown(void);					// 3・2・1のｶｳﾝﾄ中の処理
	void GameFunc(void);					// ｹﾞｰﾑ中の処理
	void PauseFunc(void);					// ﾎﾟｰｽﾞﾓｰﾄﾞ中の処理
	void CutInFunc(void);					// ｶｯﾄｲﾝ中の処理

	bool CheckGameEnd(void);				// ｹﾞｰﾑ終了かどうかを調べる
	bool GameDraw();

	bool WhiteOut(void);					// ﾎﾜｲﾄｱｳﾄの描画
	void PageInstance(void);				// ﾍﾟｰｼﾞめくりの初期化(ﾘｻﾞﾙﾄ用)
	void BufferChange(void);				// DX_SCREEN_BACKにtmpBufferの描画を写す
	int Init(void);

	sharedListObj objList;
	sharedListFloor floorList;
	sharedListChicken chickenList;

	IMAGE_ID imageID;
	std::array < std::string, IMAGE_MAX > imageName;	// プログラマのモチベ維持の為の画像である
	VECTOR2 drawOffset;

	std::array<CHAR_ID, PLAYER_MAX> players;			// ﾌﾟﾚｲﾔが選んだｷｬﾗの情報
	std::array<VECTOR2, PLAYER_MAX> gameStartPos;		// ｹﾞｰﾑ開始時の座標
	std::array<bool, PLAYER_MAX> entryFlag;			// ｹﾞｰﾑ参加ﾌﾗｸﾞ　falseで初期化し、trueになったらｷｬﾗをｲﾝｽﾀﾝｽする

	std::list<ExpOrb*> expOrbList;						// 経験値玉を入れるlist

	GameController controller;		// ｺﾝﾄﾛｰﾗｰ情報

	GAME_STATE gameState;	// ｹﾞｰﾑｼｰﾝの状態遷移

	int gameSceneCnt;		// ｹﾞｰﾑｼｰﾝに入ってからの経過ﾌﾚｰﾑ	0から++されていく
	int gameCnt;			// ｹﾞｰﾑが始まってからの経過ﾌﾚｰﾑ		値を--していき0で終了
	int gameStartCnt;		// 3・2・1の表示用 0から++していく
	int resSceneCnt;		// 戦闘終了条件からﾘｻﾞﾙﾄｼｰﾝに遷移するまでのｶｳﾝﾄ
	int grayOutCnt;			// ｸﾞﾚｰｱｳﾄをｽﾗｲﾄﾞするときに使用するｶｳﾝﾄ		ﾏｲﾅｽ値で初期化しておく(待ち時間)
	int gemeEndCnt;			// 勝者の時間

	int joinPlayerNum;		// 参加人数
	int pausePadNum;		// ﾎﾟｰｽﾞへ移行したﾊﾟｯﾄの番号を格納

	CHANGE_SCENE changeSnece;	// ﾎﾟｰｽﾞﾓｰﾄﾞでｷｬﾗｾﾚに戻ったりﾘｻﾞﾙﾄに進んだりするときに使用

	CUT_IN_INFO cutInInfo;	// ｶｯﾄｲﾝの際に必要な情報

	STAGE_ID stageID;		// 選択したｽﾃｰｼﾞ

	// 雲の描画座標
	VECTOR2 cloudFrontPos1;
	VECTOR2 cloudFrontPos2;
	VECTOR2 cloudBackPos1;
	VECTOR2 cloudBackPos2;

	// SE用
	int PreCnt;
	bool sleepFlag;
	bool chengFlag;

	RESULT_INFO resultInfo;			// ﾘｻﾞﾙﾄ画面で必要な情報

	bool showInstruction;			// true:操作説明画像を表示する(ﾁｭｰﾄﾘｱﾙで使用)

	bool reStartFlag;				// ｹﾞｰﾑ→ﾎﾟｰｽﾞ→ｹﾞｰﾑの時にtrue　　この時ｶｳﾝﾄﾀﾞｳﾝ中ｷｬﾗ操作不可

	array<ListFloor_itr, FLOOR_MAX> floorTable;

	TurnThePage turnThePage;
	int lastGameScreen;				// 終了時のｹﾞｰﾑｽｸﾘｰﾝ画像
	Fader<double> alpha;			// ﾌｪｰﾄﾞｱｳﾄの画面透明度
	MODE mode;

	int captureImage;				// 画像をｷｬﾌﾟﾁｬして保存する		ｷｬﾗｾﾚ遷移の時に使用
	float ExtRate;
	bool ExtendEndF;				// ture:縮小終わり				ｷｬﾗｾﾚ遷移の時に使用

	// 文字の長さ取得用
	std::string StageName;
	int len;
};

