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

	void ChangeState(std::string key);

	void CheckHitFlag();

	std::string GetSPAttackName(int idx);

	// 遠距離攻撃フラグセット
	void SetLongAttackFlag(bool flag) { longAttackFlag = flag; }

	// 方向変更フラグセット
	void SetDirChange(bool flag) { dirChangeFlag = flag; }

	// ジャンプのタイプ
	void SetJumpType(JUMP_TYPE type) { jumpType = type; }
	JUMP_TYPE GetJumpType() { return jumpType; }

	// 当たり判定情報を取得
	HitData GetHitData() const;

	// 弾発射までのカウントを取得
	const int GetShotCreateCount() const { return shotCreateCount; }

	bool isSPLongAttack(std::string spAnimName);

	void CheckDamage(ANIM_ATTRIBUTE att);

private:

	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ｱﾆﾒｰｼｮﾝの画像ﾌｧｲﾙ名 (ｷｰ: ｱﾆﾒｰｼｮﾝ名)

	void AddStateObj(std::string key, std::shared_ptr<AIState> state);

	// stateクラスを保存
	std::map<std::string, std::shared_ptr<AIState>> stateObj;

	// 現在のステートのkey
	std::string currentStateName;

protected:

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// 初期化	引数: ﾌｧｲﾙ名, 分割ｻｲｽﾞ, 分割数, 座標, 反転ﾌﾗｸﾞ, パッド番号

	bool longAttackFlag;			// 遠距離攻撃フラグ
	bool dirChangeFlag;				// 方向変更フラグ
	JUMP_TYPE jumpType;				// ジャンプ方向のタイプ
	int shotCreateCount;			// shotを生成するまでのカウント

	std::string characterName;
	std::string spAttackAnimName[3];
	std::string spAttackAnimFileName[3];
};

