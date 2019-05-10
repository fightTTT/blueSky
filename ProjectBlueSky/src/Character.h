#pragma once
#include "Obj.h"

#include <map>
#include <list>
#include <array>
#include <vector>

enum PAD_ID;

enum COM_DIR
{
	COM_DIR_CENTER,
	COM_DIR_UP,
	COM_DIR_RIGHT_UP,
	COM_DIR_RIGHT,
	COM_DIR_RIGHT_DOWN,
	COM_DIR_DOWN,
	COM_DIR_LEFT_DOWN,
	COM_DIR_LEFT,
	COM_DIR_LEFT_UP,
	COM_DIR_MAX
};

class Character :
	public Obj
{
public:
	Character(VECTOR2 offset);
	~Character();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	void Draw(void);		// 描画処理

private:
	bool InitAnim(void);
	void CommandUpDate(const GameCtrl & ctl);
	bool CheckCommand(int skillNum);

	std::map<std::string, std::string> animFileName;		// ｱﾆﾒｰｼｮﾝの画像ﾌｧｲﾙ名 (ｷｰ: ｱﾆﾒｰｼｮﾝ名)

	std::list<COM_DIR> comList;
	COM_DIR comDir;
	COM_DIR comDirOld;
	int comClearCnt;

	bool jumpFlag;
	VECTOR2 jumpSpeed;

protected:
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn, PAD_ID id);		// 初期化	引数: ﾌｧｲﾙ名, 分割ｻｲｽﾞ, 分割数, 座標, 反転ﾌﾗｸﾞ, パッド番号

	std::string characterName;
	std::string spAttackAnimName[3] = {"技1", "技2" , "技3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
	std::array<std::array<std::vector<COM_DIR>, DIR_MAX>, 3> spAttackCommand;		// spAttackCommand[技番号][自分の方向][コマンド]
};

