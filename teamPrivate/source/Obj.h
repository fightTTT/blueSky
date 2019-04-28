#pragma once
#pragma once
#include <string>
#include <map>
#include "VECTOR2.h"
#include "classObj.h"
#include "BaseFloor.h"
#include "GameScene.h"
#include "ColMng.h"
#include "DIR.h"
#include "PARAM.h"
#include "LOCATE.h"

#define ANGRY_CNT			(10)	// ぶちぎれﾓｰﾄﾞの秒数

#define PRASS_INV (30)	// 早押し判定用


#define FIX_TIME			(15);		// 硬直時間
#define GROUND_POS_Y	540
#define PLAYER_DEF_SPEED (3)

enum ANIM_TBL {
	ANIM_TBL_START_ID,	// 開始位置
	ANIM_TBL_FRAME,		// ｺﾏ数
	ANIM_TBL_INV,		// 間隔
	ANIM_TBL_LOOP,		// ループするかどうか
	ANIM_TBL_MAX
};

enum OBJ_TYPE {
	OBJ_TYPE_PLAYER,
	OBJ_TYPE_WHEPONN,
	OBJ_TYPE_STAGE_OBJ,
	OBJ_TYPE_MAX
};

enum ANIM_TYPE {
	ANIM_TYPE_NO_ATTACK,		// 攻撃ｱﾆﾒｰｼｮﾝではない
	ANIM_TYPE_ATTACK,			// 連撃ではないｱﾆﾒｰｼｮﾝ
	ANIM_TYPE_MULTI_HIT,		// 連撃ありｱﾆﾒｰｼｮﾝ
};

enum ANIM_STATE {
	ANIM_NOW,			// 現在のｱﾆﾒｰｼｮﾝ
	ANIM_OLD,			// 1ﾌﾚｰﾑ前のｱﾆﾒｰｼｮﾝ
	ANIM_MAX,
};

enum DMG_LATE {
	DMG_HV,			// 大ﾀﾞﾒｰｼﾞ
	DMG_MD,			// 中ﾀﾞﾒｰｼﾞ
	DMG_LW,			// 小ﾀﾞﾒｰｼﾞ
	DMG_MAX,
};

enum PL_STATE {
	PL_STATE_SEWAT,		// 汗(HP少ない)
	PL_STATE_ANGRY,		// 怒りﾌﾙﾁｬｰｼﾞ
	PL_STATE_SPECIAL,	// 必殺技ﾌﾙﾁｬｰｼﾞ
	PL_STATE_MAX
};

enum DERI {
	DERI_NON,	// 派生しない
	DERI_NOMAL,	// 通常攻撃3
	DERI_UP,	// 通常攻撃_上
	DERI_DOWN,	// 通常攻撃_下
	DERI_MAX
};

enum CHARGE_STAGE {
	CHARGE_NON,		// 溜めなし
	CHARGE_LW,		// 溜め1段階
	CHARGE_HV,		// 溜め2段階
	CHARGE_MAX,
};

typedef struct {
	std::array<bool, PL_STATE_MAX>	plState;
	int cnt;			// 一定値から--していく。0になるとplStateItrを進めて次のﾌｷﾀﾞｼにする
}PL_STATE_MULTI;

typedef struct {
	std::string attackName;		// 受けた攻撃名
	int animId;					// 受けた攻撃のｺﾏ数
}HIT_PLAYER_INF;

typedef struct {
	VECTOR2 oldPos;	// 浮いている床の1ﾌﾚｰﾑまえ座標情報
	int itrCnt;		// 浮いている床と当たり判定があった場合の進んだitr数
}PARENT_FLOOR;

typedef struct {
	VECTOR2 insPos;	// ﾀﾞﾒｰｼﾞが表示される始点
	int		cnt;	// cnt--していき、0に近くなるにつれ透明に
	int		damage;	// 受けたﾀﾞﾒｰｼﾞ
}FLY_TEXT;

typedef struct {
	VECTOR2		insPos;			// ﾋｯﾄｴﾌｪｸﾄが表示される始点
	int			cnt;			// cnt--していき、0に近くなると消える1
	DMG_LATE	damageRate;		// 受けたﾀﾞﾒｰｼﾞの大きさ(この大きさによって表示するﾋｯﾄｴﾌｪｸﾄ変える)
	double		rota;			// 回転描画の角度
}HIT_EFFECT;

typedef struct {
	int invCnt;					// 無敵の回数
	std::string animName;		// 無敵中ｱﾆﾒｰｼｮﾝ
}INV_INF;

typedef struct {
	VECTOR2 pos;				// 描画座標
	VECTOR2 offset;				// 描画ｵﾌｾｯﾄ
	std::string animName;		// ｱﾆﾒｰｼｮﾝ名
	unsigned int animid;		// ｱﾆﾒｰｼｮﾝｺﾏ数
	DIR dir;					// 方向
	int cnt;					// 一定値まで行くと消滅
}SHADOW;

class GameController;

class Obj
{
public:
	Obj();
	virtual ~Obj();

	// init(読み込むｷｬﾗﾈｰﾑ)
	virtual bool init(std::string charName);
	virtual bool initAnim(void) = 0;	// ｱﾆﾒｰｼｮﾝの初期追加設定
	bool initSe(void);					// Seの初期設定

	void UpDate(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag);	// 状態更新

	const VECTOR2 &GetPos(void);							// ﾛｰｶﾙ変数は基本、参照で返さない
	const VECTOR2 GetPosOld(void);
	const std::string GetAnim(void);						// 現在Setされているｱﾆﾒｰｼｮﾝ名を返す
	const std::string GetAnim(ANIM_STATE type);				// 1ﾌﾚｰﾑ前か現在のｱﾆﾒｰｼｮﾝ名を返す
	const std::vector<ColInf> GetPlayerCollision(void);		// 他のプレイヤーから自分のｺﾘｼﾞｮﾝを見るための関数
	const DIR GetDir(void);									// 現在の向きを返す
	virtual const int GetPadNum(void) = 0;					// ｺﾝﾄﾛｰﾗｰ番号を返す
	const int GetHp(void);									// hpを返す
	const int GetAnimId(void);								// ｱﾆﾒｰｼｮﾝidを返す
	const int GetAnimFrame(void);
	virtual const VECTOR2 GetFlyHighPower(void);
	const std::string GetCharName(void);
	const RESULT_INFO GetResultInfo(void);					// ﾘｻﾞﾙﾄ画面で必要な情報を返す
	const bool CheckSpAttackCnt(void);						// 必殺技中ならtrue

															// AddAnim(ｱﾆﾒｰｼｮﾝ名,開始ｺﾏ目x,開始ｺﾏ目y,総ｺﾏ数,切り替えﾌﾚｰﾑ数,ループするか)
	bool AddAnim(std::string animName, int inv, bool loop, std::string animFile);

	bool SetAnim(std::string animName);				// ｱﾆﾒｰｼｮﾝの設定	設定したｱﾆﾒｰｼｮﾝを行う
	void AddSpecialPoint(int specialPoint);			// 必殺ｹﾞｰｼﾞの加算を行う
	void AddAngryPoint(int angryPoint);				// 怒りｹﾞｰｼﾞの加算(主に減算)を行う
	void AddChickenKillCnt(void);					// resultInfoのchickenKillCntを+1する

	bool CheckDeath(void) { return deathFlag; }	// 生死を確認	 死:true	生:false
	virtual bool CheckObjType(OBJ_TYPE type) = 0;
	void		 CheckHit(weakListObj objList, std::list<ExpOrb*>& expOrbList, bool entryFlag);	// 当たり判定(自分の体と相手の武器)
	bool		 CheckAngryMode(void);				// true:ぶちぎれﾓｰﾄﾞ			false:通常
	CHARGE_STAGE CheckCharge(void);					// 溜め段階を返す(溜めなし = 0, 溜め1 = 1, 溜め2 = 2)
	CUT_IN_INFO	 CheckSpecialCutIn(void);			// 必殺技を発動したﾊﾟｯﾄﾞ番号と座標を返す

	virtual void Draw(bool moveStopFlag, CUT_IN_INFO cutInInfo);		// 描画

	// SE用関数
	const bool		CheckAnimSe(std::string anim);		// 1ﾌﾚｰﾑ前と現在のｱﾆﾒｰｼｮﾝが違うならtrueを返す(PlayerのSE用)
	const DMG_LATE	CheckDamageSe(void);				// 一度のﾀﾞﾒｰｼﾞ量を計算し大ﾀﾞﾒｰｼﾞなどを判定する

	static void ResultInfoInit();
	static void AddChickenCnt(int padNum);					// chickenCnt++する
	static int GetChickenCnt(int padNum);

private:
	int shadowPosY;

	// ｱﾆﾒｰｼｮﾝごとの開始位置,ｺﾏ数,間隔を格納
	std::map<std::string, int[ANIM_TBL_MAX]>animTable;
	std::map<std::string, std::string>		animFileName;
	std::map<std::string, int>				charNumbering;		// charNameをﾅﾝﾊﾞﾘﾝｸﾞする　雨野降太 = 0, 双葉青砥 = 1

	std::array<std::string[2], DMG_MAX>		hitEffectFileName;	// 小ﾀﾞﾒ中ﾀﾞﾒの画像の名前を管理する用

	std::string animName;				// 表示ｱﾆﾒｰｼｮﾝ名
	std::string charName;

	std::list<SHADOW>	shadowList;		// 残像の情報を格納
	std::list<FLY_TEXT> flyTextList;	// ﾌﾗｲﾃｷｽﾄの情報を格納するよ

	std::list<PL_STATE>	plStateList;	// ﾌﾟﾚｲﾔの状態が保存されたlist　吹き出しの表示に使用する
	std::list<PL_STATE>::iterator plStateItr;

	std::array<std::string, PL_STATE_MAX> plStateFileName;		// ﾌｷﾀﾞｼの画像の名前を管理する用

	std::map<std::string, std::vector<ColInf>> colMap;	// playerのｺﾘｼﾞｮﾝを格納(ｱﾆﾒｰｼｮﾝ名で管理)

	std::array<HIT_PLAYER_INF, 4> hitPlayerInf;		// 各ﾌﾟﾚｲﾔｰから受けた最後の攻撃を格納(ﾉｯｸﾊﾞｯｸ時にﾘｾｯﾄ)

	std::array<int, 5>	spGaugeColorTbl;			// 必殺技ｹﾞｰｼﾞの色ﾃｰﾌﾞﾙ（青→緑→黄→橙→赤）

	static RESULT_INFO resultInfo;	// ﾘｻﾞﾙﾄ画面で必要な情報

	virtual void SetMove(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag) = 0;

protected:

	std::map<std::string, ANIM_TYPE>		animPtnTbl;		// ｱﾆﾒｰｼｮﾝが戦闘系(攻撃や防御)ならtrue

	virtual int GetGuardTimeMax(void);						// ｷｬﾗごとの最大ｶﾞｰﾄﾞ時間を得る
	virtual int GetJumpCntMax(void);						// ｷｬﾗごとの最大ｼﾞｬﾝﾌﾟ数を得る
	virtual int GetSpAttackTimeMax(void);

	bool AlwaysUpDate(const GameController &controller, weakListChicken chickenList);			// 毎ﾌﾚｰﾑ更新
	int  GetColMaxEndPosY(std::vector <ColInf> collisionVec);		// 現在のｺﾗｲﾀﾞｰの中で最も大きなY座標を返す
	virtual void PosUpDate(VECTOR2& tmpPos, int maxEndPosY);		// 座標の補正や重力による影響の計算を行う
	void DeriAttack(const GameController &controller);			// 各ﾌﾟﾚｲﾔｰ共通の派生攻撃を行う
	void Attack(const GameController &controller, VECTOR2 &tmpPpos, bool entryFlag);				// 攻撃ｱﾆﾒｰｼｮﾝのｾｯﾄ
	virtual void UniqueAttack(VECTOR2& tmpPos);					// 固有攻撃の動きの設定
	void UpDateAnimId(void);		// ｱﾆﾒｰｼｮﾝidの計算
	void Dash(const GameController &controller, VECTOR2 &tmpPos);				// ﾀﾞｯｼｭの処理
	void JumpFnc(const GameController &controller, VECTOR2 &tmpPos, int maxEndPosY);		// ｼﾞｬﾝﾌﾟした際に呼ばれる関数
	bool ColGround(VECTOR2& tmpPos, std::vector<ColInf> colVec,int maxEndPosY);			// 地面と床との当たり判定
	virtual void UniqueColGround(void);							// ｷｬﾗごとの地面や床と会ったった時の処理を行う
	void PosAdjust(VECTOR2 &tmpPos, int maxEndPosY);			// 座標補正
	virtual void SetNextAnim(bool animEndFlag);					// animEndFlagがtrueなら次のｱﾆﾒｰｼｮﾝをｾｯﾄする
	void MoveEndUpDate(VECTOR2 tmpPos, const GameController &controller, int maxEndPosY);			// player行動後に行う更新
	void PosOutOfRangeCtrl(VECTOR2& pos, int maxEndPosY);		// 座標範囲外制御(補正する座標,地面からposまでの距離)
	void FlyUpDate(const GameController &controller);			// 傘浮遊中の状態更新
	void FlyTextUpDate(void);		// ﾌﾗｲﾃｷｽﾄの状態更新
	void HitEffectUpDate(void);		// ﾋｯﾄｴﾌｪｸﾄの状態更新
	void PlStateUpDate(void);		// plStateListの状態更新

	void AngryGaugeDraw(void);		// 怒りｹﾞｰｼﾞUIの描画
	void SpecialGaugeDraw(bool cutInFlag);	// 必殺ｹﾞｰｼﾞUIの描画
	void GuardDraw(void);			// ｶﾞｰﾄﾞの玉の描画
	void StateIconDraw(void);		// ｽﾃｰﾀｽｱｲｺﾝ(ﾌｷﾀﾞｼ)の描画
	void FlyTextDraw(void);			// ﾌﾗｲﾃｷｽﾄの描画
	void CharacterDraw(std::vector<ColInf> collisonVec, bool moveStopFlag, CUT_IN_INFO cutInInfo);		// 操作ｷｬﾗの描画

	// Se用
	void PlaySe(void);				// Seの再生
	virtual void PlaySPSe(void);	// 必殺技の再生

	int plStateCnt;

	DIR dir;						// 向いている方向
	DIR dirOld;						// 1ﾌﾚｰﾑ前向いていた方向
	VECTOR2 pos;					// 座標
	VECTOR2 posOld;					// 1ﾌﾚｰﾑ前の座標

	// SE用
	std::string animNameOld;		// 1ﾌﾚｰﾑ前のｱﾆﾒｰｼｮﾝ名
	int oldHp;						// 1ﾌﾚｰﾑ前のHP

	// UI表示系
	int hp;							// 体力
	float angryPoint;				// 怒りｹﾞｰｼﾞ
	int specialPoint;				// 必殺ｹﾞｰｼﾞ
	int padNum;						// ｼﾞｮｲﾊﾟｯﾄﾞの番号 0から3まで

	INV_INF invInf;					// 無敵状態のｶｳﾝﾄ回数分攻撃を食らっても効かない
	unsigned int visible;			// 表示:非表示切り替え
	int speed;						// 移動速度
	int jumpCnt;					// ｼﾞｬﾝﾌﾟできる回数
	PARAM param;					// ﾌﾟﾚｲﾔの状態　ｼﾞｬﾝﾌﾟ、ﾌｫｰﾙ、ｸﾞﾗｳﾝﾄﾞ
	LOCATE locate;					// 今いる場所を格納
	float vX;						// x軸加速
	float vY;						// y軸速度

	float jumpTime;
	bool fallFlag;					// 空中で下キーを押すとtrueになり落下速度が二倍になる

	bool floorCheckFlag;			// ﾌﾟﾚｲﾔｰの足元が床より高い位置にあるならtrue

	sharedListFloor floorList;
	PARENT_FLOOR parentFloor;		// 自分が着地しているｻｶﾞの情報

	int   framCnt;					// 経過ﾌﾚｰﾑ格納用変数

	bool dashFlag;					// 左右方向ｷｰ二度押しでﾀﾞｯｼｭ。ﾀﾞｯｼｭ中ならtrue
	bool downFlag;					// 下ｷｰを素早く2度押ししたらtrue
	int nowTimeLR;
	int nowTimeDown;

	unsigned int animId;			// ｱﾆﾒｰｼｮﾝのidを格納
	unsigned int animIdOld;			// 1ﾌﾚｰﾑ前のｱﾆﾒｰｼｮﾝのidを格納
	std::string imageName;			// 表示画像ﾌｧｲﾙ名
	unsigned int animCnt;			// ｺﾏ数ｶｳﾝﾀｰ
	bool animEndFlag;				// ｱﾆﾒｰｼｮﾝ最終ｺﾏ到達ﾌﾗｸﾞ
	unsigned int jumpEffectCnt;		// ｼﾞｬﾝﾌﾟｱﾆﾒｰｼｮﾝ用ｶｳﾝﾄ
	bool jumpEffectFlag;			// ｼﾞｬﾝﾌﾟｴﾌｪｸﾄ発生中はtrue
	VECTOR2 jumpPos;				// ｼﾞｬﾝﾌﾟした座標

	unsigned int damageInv;			// 毎ﾌﾚｰﾑ減算され1になったら受けた攻撃の情報を消す
	unsigned int multiHitFlag;		// 毎ｱﾆﾒｰｼｮﾝｺﾏ当たり判定するかどうか	true:する
	unsigned int nextMultHitInv;	// 多段ヒット攻撃で1度攻撃されてから一定時間攻撃がなけれvisibleを有効にする　もっと良い名前つけて
	DIR attackDir;					// 攻撃された方向を格納

	DERI deri;						//通常攻撃からの派生
	int  multiAttackNum;			// 連撃のﾅﾝﾊﾞﾘﾝｸﾞ　	通常攻撃0～通常攻撃2
	int  hitCnt;					// 攻撃を受けたら加算	一定数になったら0にして無敵状態へ 又は 一定時間攻撃がないと0へ

	bool angryFlag;					// true:ぶちぎれﾓｰﾄﾞ　false:通常
	int  angryCnt;					// ぶちぎれﾓｰﾄﾞの時間(ﾌﾚｰﾑ数)

	CHARGE_STAGE chargeStage;		// 溜め段階　
	int  chargeCnt;					// 溜め攻撃で溜めている時間　一定時間溜めたら攻撃力UP 溜めｱﾆﾒｰｼｮﾝのｶｳﾝﾀｰ

	bool spAttackFlag;				// 必殺技をしたらtrue
	int  spAttackCnt;				// 必殺技中なら数値が入っている		0なら必殺技中ではない
	bool spAttackHitFlag;			// 必殺技を食らっているか	true:食らってる		false:食らっていない
	unsigned int spAttackInv;		// 必殺技を食らってから一定時間攻撃を受けなかったら無敵状態にして移動を可能にする

	int guardTime;					// ｶﾞｰﾄﾞ時間　範囲は0～180　攻撃を受けると攻撃力に比例して値が減る　何もなければ徐々に回復

	float gravity;					// 毎ﾌﾚｰﾑ加算する　velocityYの量　傘浮遊の時以外は0.4

	int guardImage;					// ｶﾞｰﾄﾞ画像

	int fixCtrlCnt;					// 硬直ｶｳﾝﾄ数値がある間は動けない

									// ﾋｯﾄｴﾌｪｸﾄ
	HIT_EFFECT hitEffect;			// ﾋｯﾄｴﾌｪｸﾄのﾘｽﾄ

	bool playerNumSprite;			// 1P2Pの表示ﾌﾗｸﾞ

	bool deathFlag;					// 死亡ﾌﾗｸﾞ		true:死亡	false:生存

	XINPUT_STATE xstate[PLAYER_MAX];

	// SE用
	int walkSeCnt;
	int angChargeSeCnt;
	int umbFloatCnt;
	int spSeCnt;
	bool spmaxSeFlag;
	bool chargeSeFlag;
	bool angerMaxSeFlag;

};