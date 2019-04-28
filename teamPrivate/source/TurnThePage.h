#pragma once
#include "VECTOR2.h"
#include <array>

class GameController;


// 画像の自由変形描画
int	DrawModiGraph(
	VECTOR2 leftUp,
	VECTOR2 rightUP,
	VECTOR2 rightDown,
	VECTOR2 leftDown,
	int GrHandle);


// ﾍﾟｰｼﾞをめくる方向
enum TURN_DIR {
	TURN_DIR_LEFT,		// 左にめくる
	TURN_DIR_RIGHT,		// 右にめくる
	TURN_DIR_MAX,
};

class TurnThePage
{
public:
	TurnThePage();
	// 3秒後に捲れる	(TrunFlag = ture)・・・右めくり
	TurnThePage(VECTOR2 imageSize, VECTOR2 offset, int frontImage, int backImage, bool TurnFlag = false);
	// 捲れるﾍﾟｰｼﾞの時間の指定(秒)
	TurnThePage(VECTOR2 imageSize, VECTOR2 offset, int frontImage, int backImage, float waitTime, bool TurnFlag = false);

	~TurnThePage();

	bool Draw(void);

private:
	// 右めくり左めくり共通の初期化
	void Init(VECTOR2 imageSize, VECTOR2 offset, int frontImage, int backImage, bool TurnFlag);
	// 右めくり固有の初期化
	void InitTurnLeft();
	// 左めくり固有の初期化
	void InitTurnRight();

	std::array<std::array<VECTOR2, 4>, 12>	pos;		// 変形描画の際の座標を格納

	VECTOR2 imageSize;	// 画像の大きさ		引数として受け取る
	VECTOR2 offset;		// 描画ｵﾌｾｯﾄ値		引数として受け取る
	int frontImage;		// 前ﾍﾟｰｼﾞのﾊﾝﾄﾞﾙ	引数として受け取る
	int backImage;		// 後ﾍﾟｰｼﾞのﾊﾝﾄﾞﾙ	引数として受け取る

	int frontBuffer;	// 前ﾍﾟｰｼﾞの右半分の描画
	int backBuffer;		// 後ﾍﾟｰｼﾞの左半分の描画
	int slideImage;		// めくっているﾍﾟｰｼﾞのﾊﾝﾄﾞﾙ(前半はflontBuffer後半はbackBuffer(左めくりの場合))

	int cnt;			// ｶｳﾝﾀｰ
	int slideNum;		// 現在めくっているﾍﾟｰｼﾞの番号	最大数は#define slideImageMax

	float waitCnt;		// ﾍﾟｰｼﾞが捲れるまでのｶｳﾝﾄ

	TURN_DIR turnDir;	// ﾍﾟｰｼﾞをめくる方向

	bool seFlag;
};