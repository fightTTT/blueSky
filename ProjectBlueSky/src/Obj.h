#pragma once
#include<string>
#include<map>
#include"VECTOR2.h"
#include"ObjList.h"

enum ANIM_TBL {
	ANIM_TBL_START_ID,		//開始位置
	ANIM_TBL_FRAME,			//ｺﾏ数
	ANIM_TBL_INV,			//間隔
	ANIM_TBL_LOOP,			//ﾙｰﾌﾟするかどうか
	ANIM_TBL_MAX
};

enum OBJ_TYPE
{
	OBJ_TYPE_CHARACTER,
	OBJ_TYPE_MAX
};

class GameCtrl;

class Obj
{
public:
	Obj();
	Obj(VECTOR2 drawOffset);
	virtual ~Obj();

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut);					// 初期化
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos);		// 初期化	引数: ﾌｧｲﾙ名, 分割ｻｲｽﾞ, 分割数, 座標

	// 情報更新
	void UpDate(const GameCtrl &ctl);
	void UpDate(const GameCtrl &ctl, weekListObj objList);

	virtual bool InitAnim(void) { return true; };		// ｱﾆﾒｰｼｮﾝ初期化

	virtual void Draw(void);		// 描画処理

	virtual void Draw(unsigned int id);		// 描画処理 引数付き

	virtual bool CheckDeth(void) { return false; };		// ｵﾌﾞｼﾞｪｸﾄを削除して良いかのﾁｪｯｸ

	virtual bool CheckObjType(OBJ_TYPE type) = 0;		// ｵﾌﾞｼﾞｪｸﾄのﾀｲﾌﾟが引数で指定したﾀｲﾌﾟか確かめる

	void SetPos(VECTOR2 pos);			// 座標をｾｯﾄする　引数: 座標
	const VECTOR2 &GetPos(void);		// posの情報を取得する
	void AddPos(VECTOR2 addPos);		// posに一定値加算する

	std::string GetImageName(void);		// 画像ﾌｧｲﾙ名を取得する

	bool AddAnim(std::string animName, int id_x, int id_y, int frame, int inv, bool loop);		// ｱﾆﾒｰｼｮﾝを追加する
	bool SetAnim(std::string animName);															// ｱﾆﾒｰｼｮﾝをｾｯﾄする
	std::string GetAnim(void);																	// ｱﾆﾒｰｼｮﾝの情報を取得する

private:
	//移動処理
	virtual void SetMove(const GameCtrl &ctl);
	virtual void SetMove(const GameCtrl &ctl, weekListObj objList);

protected:
	VECTOR2 pos;					// 座標

	std::string imageName;			// 表示画像ﾌｧｲﾙ名
	VECTOR2 divSize;				// 分割ｻｲｽﾞ
	VECTOR2 divCut;					// 分割数
	const VECTOR2 drawOffset;		// 描画ｵﾌｾｯﾄ
	unsigned int animCnt;			// ｱﾆﾒｰｼｮﾝｶｳﾝﾀｰ
	bool animEndFlag;				// ｱﾆﾒｰｼｮﾝ最終ｺﾏ到達ﾌﾗｸﾞ
	bool visible;					// 表示・非表示ﾌﾗｸﾞ

	std::map<std::string, int[ANIM_TBL_MAX]> animTable;		// ｱﾆﾒｰｼｮﾝ情報
	std::string animName;									// 表示ｱﾆﾒｰｼｮﾝ名
};
