#pragma once
#include "Obj.h"

#include <map>
#include <list>
#include <array>
#include <vector>

enum PAD_ID;

enum SP_COM
{
	SP_COM_CENTER,
	SP_COM_UP,
	SP_COM_RIGHT_UP,
	SP_COM_RIGHT,
	SP_COM_RIGHT_DOWN,
	SP_COM_DOWN,
	SP_COM_LEFT_DOWN,
	SP_COM_LEFT,
	SP_COM_LEFT_UP,

	SP_COM_ACCUMULATE,

	SP_COM_PUNCH,
	SP_COM_KICK,

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

	void Draw(void);		// 描画処理

private:
	bool InitAnim(void);
	void CommandUpDate(const GameCtrl & ctl);
	bool CheckCommand(int skillNum);

	VECTOR2 ssize;

	std::map<std::string, std::string> animFileName;		// ｱﾆﾒｰｼｮﾝの画像ﾌｧｲﾙ名 (ｷｰ: ｱﾆﾒｰｼｮﾝ名)

	std::list<SP_COM> comList;
	SP_COM spCom;
	SP_COM spComOld;
	int comClearCnt;

	int comboCnt;

	VECTOR2 jumpSpeed;

	VECTOR2 fallSpeed;
	
	int knockBackSpeed;
	bool knockBackFlag;

protected:
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn, PAD_ID id);		// 初期化	引数: ﾌｧｲﾙ名, 分割ｻｲｽﾞ, 分割数, 座標, 反転ﾌﾗｸﾞ, パッド番号

	std::string characterName;
	std::string spAttackAnimName[3] = {"技1", "技2" , "技3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
	std::array<std::array<std::vector<SP_COM>, DIR_MAX>, 3> spAttackCommand;		// spAttackCommand[技番号][自分の方向][コマンド]
};

