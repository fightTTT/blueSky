#pragma once

#include "Obj.h"

enum AI_STATE_TYPE
{
	AI_STATE_NONE,
	AI_STATE_MOVE,
	AI_STATE_ATTACK,
	AI_STATE_JUMP,
	AI_STATE_MAX
};

class AICharacter :
	public Obj
{
public:
	AICharacter();
	~AICharacter();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	void Draw();

private:

	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ｱﾆﾒｰｼｮﾝの画像ﾌｧｲﾙ名 (ｷｰ: ｱﾆﾒｰｼｮﾝ名)


protected:

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// 初期化	引数: ﾌｧｲﾙ名, 分割ｻｲｽﾞ, 分割数, 座標, 反転ﾌﾗｸﾞ, パッド番号

	AI_STATE_TYPE type;		// 現在実行中の状態

	bool jumpFlag;
	VECTOR2 jumpSpeed;

	std::string characterName;
	std::string spAttackAnimName[3] = { "技1", "技2" , "技3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
};

