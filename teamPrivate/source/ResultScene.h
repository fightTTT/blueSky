#pragma once
#include <array>
#include "BaseScene.h"
#include "GameScene.h"
#include "Fader.h"

#define PAGE_CNT_MAX (4)

enum AWARD {
	AWARD_GIVE_DAMAGE,		// 与えたﾀﾞﾒｰｼﾞ
	AWARD_DEATH_ENEMY_CNT,	// 倒した敵の数 
	AWARD_DEATH_ODER,		// 死んだ順番
	AWARD_CHICKEN,			// 繁栄数
	AWARD_MAX,
};

enum GRAY_OUT {
	GRAY_OUT_NON,
	GRAY_OUT_UP,			// 上げる
	GRAY_OUT_DOWN,			// 下げる
	GRAY_OUT_MAX
};

enum STATE {
	STATE_FADE_IN,			// ﾌｪｰﾄﾞｲﾝ中
	STATE_AWARD,			// 結果発表中
	STATE_FADE_OUT,			// ﾌｪｰﾄﾞｱｳﾄ中
	STATE_NEXT,				// 次のｼｰﾝに遷移
	STATE_MAX,
};

class ResultScene :
	public BaseScene
{
public:
	ResultScene(RESULT_INFO resultInfo, int joinPlayerNum);
	~ResultScene();

	unique_Base Update(unique_Base own, const GameController &ctrl);

private:
	void StateFadeInFunc(void);
	void StateAwardFunc(const GameController &ctrl);
	void StateFadeOutFunc(void);

	bool AwardFunc(int pNum, AWARD awardState);

	void NoJoinPadNumPointInit(RESULT_INFO& c_resultInfo);	// ｹﾞｰﾑに参加していないﾊﾟｯﾄﾞ番号の点数を-1で初期化
	void SortByRank(void);				// resultInfoを降順に並び替えrankInfoに格納する

	void DrawGrayOut(void);
	void GrayOutUpdate(void);

	int	 GetRankColor(int rank);		// 順位に対応した色を16進数で返す
	int	 GetPointColor(int point);		// 得点に対応した色を16進数で返す　rankPointTblに対応した色

	void DrawFade(void);		// ﾌｪｰﾄﾞｲﾝ・ﾌｪｰﾄﾞｱｳﾄ描画用

	int Init(void);
	void ResultDraw(void);

	int resultCnt;

	int grayOutCnt;
	GRAY_OUT grayOut;

	RESULT_INFO resultInfo;		// 各部門ｹﾞｰﾑ結果情報
	RESULT_INFO rankInfo;		// 各部門順位情報
	RESULT_INFO pointInfo;		// 各部門獲得ﾎﾟｲﾝﾄ情報

	float pointUnit;			// 1ﾎﾟｲﾝﾄ辺りのｸﾞﾗﾌの長さ

	std::array<int, PLAYER_MAX>		PosXTbl;			// 各棒ｸﾞﾗﾌの中心X座標
	std::array<int, PLAYER_MAX>		graphAddPoint;		// 現在の棒ｸﾞﾗﾌのﾎﾟｲﾝﾄ
	std::array<int, AWARD_MAX>		GraphColorTbl;		// 棒ｸﾞﾗﾌの色
	std::array<int, PLAYER_MAX>		rankPointTbl;		// 順位の基礎点

	std::array<std::array<float, AWARD_MAX>, PLAYER_MAX>		graphPoint;
	std::array<std::array<int, AWARD_MAX>, PLAYER_MAX>			graphLength;

	AWARD awardState;			// 各部門の状態
	STATE resultState;			// ﾘｻﾞﾙﾄｼｰﾝの状態

	bool blackFlag;

	bool awardEndFlag;			// ture:各部門の棒ｸﾞﾗﾌ加算まで終了
	bool resEndFlag;			// true:全部門の棒ｸﾞﾗﾌ加算まで終了
	int  awardCnt;				// 各部門ごとのｶｳﾝﾀｰ　0から++される

	int scoreMax;				// 最高得点

	// SE用
	bool winSeFlag;
	int seCnt;
	bool seFlag;
	bool seFlag2;
	bool seFlag3;

	int ajustCnt;				// 画面変わって即ボタン押してしまう等を防ぐ為のカウンター

	Fader<double> alpha;		// ﾌｪｰﾄﾞｲﾝ・ﾌｪｰﾄﾞｱｳﾄの画面透明度
};