#pragma once
#include "Obj.h"

#include <map>
#include <list>
#include <array>
#include <vector>

enum PAD_ID;

enum SP_COM					// 技ｺﾏﾝﾄﾞ
{
	SP_COM_CENTER,			// 中央
	SP_COM_UP,				// 上
	SP_COM_RIGHT_UP,		// 右上
	SP_COM_RIGHT,			// 右
	SP_COM_RIGHT_DOWN,		// 右下
	SP_COM_DOWN,			// 下
	SP_COM_LEFT_DOWN,		// 左下
	SP_COM_LEFT,			// 左
	SP_COM_LEFT_UP,			// 左上

	SP_COM_ACCUMULATE,		// ﾀﾒ

	SP_COM_PUNCH,			// ﾊﾟﾝﾁ
	SP_COM_KICK,			// ｷｯｸ

	SP_COM_MAX
};

class Character :
	public Obj
{
public:
	Character(VECTOR2 offset);
	~Character();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);			// 情報更新
	void CheckHitFlag(void);										// 当たり判定の情報のﾁｪｯｸと、それに伴い追加で行う処理

private:
	VECTOR2 ssize;		// ｽｸﾘｰﾝのｻｲｽﾞ

	std::map<std::string, std::string> animFileName;		// ｱﾆﾒｰｼｮﾝの画像ﾌｧｲﾙ名 (ｷｰ: ｱﾆﾒｰｼｮﾝ名)

	std::list<SP_COM> comList;
	SP_COM spCom;
	SP_COM spComOld;
	int comClearCnt;

	int comboCnt;

	int jumpInterval;
	VECTOR2 jumpSpeed;

	VECTOR2 fallSpeed;
	
	int knockBackSpeed;
	bool knockBackFlag;

	int spEndCnt;		// 必殺技の終了ﾀｲﾐﾝｸﾞ管理用のｶｳﾝﾄ
	int guardEndCnt;

	// ｱﾆﾒｰｼｮﾝの初期化関数
	bool InitAnim(void);

	// ｷｬﾗｸﾀｰの操作のﾒﾝﾊﾞ関数ﾎﾟｲﾝﾀ
	void (Character::*act)(const GameCtrl &, weekListObj);

	// ｷｬﾗｸﾀｰの操作 (基本)
	void NeutralState(const GameCtrl & ctl, weekListObj objList);

	// ｷｬﾗｸﾀｰの操作 (空中)
	void AirState(const GameCtrl & ctl, weekListObj objList);

	// ｷｬﾗｸﾀｰの操作 (ダメージ)
	void DamageState(const GameCtrl & ctl, weekListObj objList);

	// ｷｬﾗｸﾀｰの操作 (必殺技)
	void SpAttackState(const GameCtrl & ctl, weekListObj objList);

	// ﾊﾟｯﾄﾞの入力情報を元にｺﾏﾝﾄﾞとしてﾘｽﾄに格納する (引数: ｹﾞｰﾑｺﾝﾄﾛｰﾙｸﾗｽのﾎﾟｲﾝﾀ)
	void CommandUpDate(const GameCtrl & ctl);

	// ｺﾏﾝﾄﾞﾘｽﾄと引数で渡された番号のｺﾏﾝﾄﾞを比較し、一致したらtrue、不一致ならfalseを返す
	bool CheckCommand(int skillNum);

	// ﾌﾟﾚｲﾔｰに与えるﾀﾞﾒｰｼﾞ量をﾁｪｯｸする　(引数: 敵のｱﾆﾒｰｼｮﾝの属性)
	void CheckDamage(ANIM_ATTRIBUTE att);

	// ｶﾞｰﾄﾞのｱﾆﾒｰｼｮﾝの最低表示時間をｾｯﾄ
	void GuardEndCnt(int cnt);

	// 描画処理
	void Draw(void);

protected:
	std::string characterName;
	std::string spAttackAnimName[3];
	std::string spAttackAnimFileName[3];
	std::map<std::string, int> spAttackDamage;
	std::array<std::array<std::vector<SP_COM>, DIR_MAX>, 3> spAttackCommand;		// spAttackCommand[技番号][自分の方向][コマンド]

	int shotCreateCnt;		// ｼｮｯﾄを生成するまでのｶｳﾝﾄ

	// 初期化関数 (引数: ﾌｧｲﾙ名, 分割ｻｲｽﾞ, 分割数, 座標, 反転ﾌﾗｸﾞ, パッド番号)
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn, PAD_ID id);
};

