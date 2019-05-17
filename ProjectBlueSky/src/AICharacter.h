#pragma once

#include "Obj.h"

enum JUMP_TYPE
{
	JUMP_TYPE_FRONT,
	JUMP_TYPE_BACK,
	JUMP_TYPE_UP,
	JUMP_TYPE_RAND,
	JUMP_TYPE_MAX
};

class AIState;

class AICharacter :
	public Obj
{
public:
	AICharacter(VECTOR2 offset);
	~AICharacter();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	void Draw();

	void ChangeState(AIState* s);

	// 遠距離攻撃フラグセット
	void SetLongAttackFlag(bool flag) { longAttackFlag = flag; }

	// 方向変更フラグセット
	void SetDirChange(bool flag) { dirChangeFlag = flag; }

	void SetJumpType(JUMP_TYPE type) { jumpType = type; }
	JUMP_TYPE GetJumpType() { return jumpType; }

	HitData GetHitData() const;

	void CheckHitFlag();

private:

	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ｱﾆﾒｰｼｮﾝの画像ﾌｧｲﾙ名 (ｷｰ: ｱﾆﾒｰｼｮﾝ名)


protected:

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// 初期化	引数: ﾌｧｲﾙ名, 分割ｻｲｽﾞ, 分割数, 座標, 反転ﾌﾗｸﾞ, パッド番号

	AIState* state;

	bool longAttackFlag;			// 遠距離攻撃フラグ
	bool dirChangeFlag;				// 方向変更フラグ
	JUMP_TYPE jumpType;				// ジャンプ方向のタイプ

	std::string characterName;
	std::string spAttackAnimName[3] = { "技1", "技2" , "技3" };
	std::string spAttackAnimFileName[3] = { "waza_1", "waza_2" , "waza_3" };
};

