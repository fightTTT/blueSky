#pragma once
#include <string>
#include <map>
#include <vector>
#include"VECTOR2.h"
#include"ObjList.h"
#include "GameCtrl.h"

enum ColType;

enum ANIM_TBL {
	ANIM_TBL_START_ID,		// 開始位置
	ANIM_TBL_FRAME,			// ｺﾏ数
	ANIM_TBL_INV,			// 間隔
	ANIM_TBL_LOOP,			// ﾙｰﾌﾟするかどうか
	ANIM_TBL_OFFSET_X,		// ｱﾆﾒｰｼｮﾝの横軸のｵﾌｾｯﾄ
	ANIM_TBL_OFFSET_Y,		// ｱﾆﾒｰｼｮﾝの縦軸のｵﾌｾｯﾄ
	ANIM_TBL_MAX
};

enum OBJ_TYPE
{
	OBJ_TYPE_CHARACTER,
	OBJ_TYPE_AICHARACTER,
	OBJ_TYPE_SHOT,
	OBJ_TYPE_MAX
};

enum DIR
{
	DIR_RIGHT,
	DIR_LEFT,
	DIR_MAX
};

struct ShotData
{
	ShotData(VECTOR2 p, PAD_ID padID) : pos(p), id(padID) {}
	VECTOR2 pos;	// 現在のpos
	PAD_ID id;		// 発射したキャラクターが使用していたPADID
};

struct EnemyState
{
	EnemyState() : enemyPos(VECTOR2(0, 0)), enemyAnimName("none") {}
	EnemyState(VECTOR2 pos, std::string animName) : enemyPos(pos), enemyAnimName(animName) {}

	void pushBackShotData(ShotData data) { shotData.push_back(data); }

	VECTOR2 enemyPos;
	std::string enemyAnimName;
	std::vector<ShotData> shotData;
};

struct HitData
{
	bool hitFlag;		// 攻撃が当たったか true...あたった　false...あたってない
	ColType colType;
};

class GameCtrl;

class Obj
{
public:
	Obj();
	Obj(VECTOR2 drawOffset);
	virtual ~Obj();

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, bool turn);					// 初期化
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// 初期化	引数: ﾌｧｲﾙ名, 分割ｻｲｽﾞ, 分割数, 座標

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

	bool AddAnim(std::string animName, int id_x, int id_y, int frame, int inv, bool loop, int offset_x, int offset_y);		// ｱﾆﾒｰｼｮﾝを追加する
	bool SetAnim(std::string animName);															// ｱﾆﾒｰｼｮﾝをｾｯﾄする
	const int GetAnimFrame(std::string animName);
	std::string GetAnim(void);			// ｱﾆﾒｰｼｮﾝの情報を取得する

	// サイズ取得
	VECTOR2 GetDivSize() const { return divSize; }

	// 向いている方向セット
	void SetDir(DIR d);
	// 向いている方向取得
	DIR GetDir() const { return dir; }

	// 敵の情報セット
	void SetEnemyState(EnemyState state);
	// 敵の情報取得
	EnemyState GetEnemyState() const { return enemyState; }

	// アニメーションが終了しているか
	bool GetAnimEndFlag() const { return animEndFlag; }

	void SetHitData(bool flag, ColType type);

	PAD_ID GetPadID() const { return padID; }

	const VECTOR2 GetAnimOffSet(std::string animName);

	int GetCount(std::string animName);

	// 当たり判定の情報のﾁｪｯｸと、それに伴い追加で行う処理
	virtual void CheckHitFlag(void);

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
	DIR dir;						// 向いている方向
	DIR tmpDir;						// 向いている方向(一時的に保存)
	bool turnFlag;					// 画像反転フラグ
	EnemyState enemyState;			// 敵の現在の座標, 状態
	PAD_ID padID;					// 使用しているPAD番号
	HitData hitData;				// 当たった情報
	int hitAnimFlag;				// ヒット状態のアニメーション再生フラグ

	std::map<std::string, int[ANIM_TBL_MAX]> animTable;		// ｱﾆﾒｰｼｮﾝ情報
	std::string animName;									// 表示ｱﾆﾒｰｼｮﾝ名
};
