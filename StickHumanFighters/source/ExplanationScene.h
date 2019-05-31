#pragma once
#include "BaseScene.h"

#include <vector>

class ExplanationScene :
	public BaseScene
{
public:
	ExplanationScene();
	~ExplanationScene();

	// 毎ﾌﾚｰﾑ行う処理
	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	std::vector<int> imageHandle;	// 画像のﾊﾝﾄﾞﾙ

	int page;						// 現在のﾍﾟｰｼﾞ

	int frame;						// ﾌﾚｰﾑ数 (ｼｰﾝ移行直前は5倍速にしている)

	bool sceneChangeFlag;			// ｼｰﾝを切り替えてもいいかのﾌﾗｸﾞ (true: 切り替えてもよい, false: 切り替えてはいけない)
	int sceneChangeTime;			// sceneChangeFlagがtrueになってからｼｰﾝ移行するまでの時間

	// 初期化関数
	int Init();

	// 描画関数
	void ExplanationDraw();
};
