#pragma once
#include "BaseScene.h"

#include "VECTOR2.h"
#include "ObjList.h"
#include "Obj.h"

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);		// 毎ﾌﾚｰﾑ処理するものをまとめた関数、state

private:
	int Init(void);
	void BgPosUpDate(void);		// 背景の位置座標の更新と、それに伴うｷｬﾗｸﾗｰの再配置

	VECTOR2 ssize;				// 画面ｻｲｽﾞ
	VECTOR2 bgPos;				// 背景の位置座標
	VECTOR2 bgPosOld;			// 1ﾌﾚｰﾑ前の背景の位置座標

	bool GameDraw(void);		// 描画処理
	int id[2];			// 当たり判定のフレーム数

	sharedObj sObj[2];	// キャラクターのObj変数保存
	OBJ_TYPE type[2];	// キャラクターのタイプ

	sharedListObj objList;
};

