#pragma once
#include <string>
#include <map>
#include <vector>
#include"VECTOR2.h"
#include"ObjList.h"
#include "GameCtrl.h"
#include "Collision.h"

enum ANIM_TBL {
	ANIM_TBL_START_ID,		// �J�n�ʒu
	ANIM_TBL_FRAME,			// �ϐ�
	ANIM_TBL_INV,			// �Ԋu
	ANIM_TBL_LOOP,			// ٰ�߂��邩�ǂ���
	ANIM_TBL_OFFSET_X,		// ��Ұ��݂̉����̵̾��
	ANIM_TBL_OFFSET_Y,		// ��Ұ��݂̏c���̵̾��
	ANIM_TBL_MAX
};

enum ANIM_ATTRIBUTE
{
	ANIM_ATTRIBUTE_NON,
	ANIM_ATTRIBUTE_STAND,
	ANIM_ATTRIBUTE_SQUAT,
	ANIM_ATTRIBUTE_AIR,
	ANIM_ATTRIBUTE_MOVE,
	ANIM_ATTRIBUTE_ATTACK_SMALL,
	ANIM_ATTRIBUTE_ATTACK_BIG,
	ANIM_ATTRIBUTE_ATTACK_SP,
	ANIM_ATTRIBUTE_GUARD,
	ANIM_ATTRIBUTE_DAMAGE,
	ANIM_ATTRIBUTE_SHOT,
	ANIM_ATTRIBUTE_INVINCIBLE,
	ANIM_ATTRIBUTE_MAX
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
	VECTOR2 pos;	// ���݂�pos
	PAD_ID id;		// ���˂����L�����N�^�[���g�p���Ă���PADID
};

struct EnemyState
{
	EnemyState() : enemyPos(VECTOR2(0, 0)) {}

	void pushBackShotData(ShotData data) { shotData.push_back(data); }

	VECTOR2 enemyPos;
	ANIM_ATTRIBUTE enemyAnimAttribute[3];
	std::string enemyAnim;
	std::vector<ShotData> shotData;
};

struct HitData
{
	bool hitFlag;		// �U�������������� true...���������@false...�������ĂȂ�
	ColType colType;
};

class GameCtrl;

class Obj
{
public:
	Obj();
	Obj(VECTOR2 drawOffset);
	virtual ~Obj();

	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, bool turn);					// ������
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn);		// ������	����: ̧�ٖ�, ��������, ������, ���W

	// ���X�V
	void UpDate(const GameCtrl &ctl);
	void UpDate(const GameCtrl &ctl, weekListObj objList);

	virtual bool InitAnim(void) { return true; };		// ��Ұ��ݏ�����

	virtual void Draw(void);		// �`�揈��

	virtual void Draw(unsigned int id);		// �`�揈�� �����t��

	virtual bool CheckDeth(void) { return false; };		// ��޼ު�Ă��폜���ėǂ���������

	virtual bool CheckObjType(OBJ_TYPE type) = 0;		// ��޼ު�Ă����߂������Ŏw�肵�����߂��m���߂�

	void SetPos(VECTOR2 pos);			// ���W��Ă���@����: ���W
	const VECTOR2 &GetPos(void);		// pos�̏����擾����
	void AddPos(VECTOR2 addPos);		// pos�Ɉ��l���Z����

	std::string GetImageName(void);		// �摜̧�ٖ����擾����

	bool AddAnim(std::string animName, int id_x, int id_y, int frame, int inv, bool loop, int offset_x, int offset_y);		// ��Ұ��݂�ǉ�����
	bool SetAnim(std::string animName);															// ��Ұ��݂�Ă���
	const int GetAnimFrame(std::string animName);
	std::string GetAnim(void);			// ��Ұ��݂̏����擾����
	const ANIM_ATTRIBUTE GetAnimAttribute(int index);		// ��Ұ��݂̑����̹ޯ��

	// �T�C�Y�擾
	VECTOR2 GetDivSize() const { return divSize; }

	// �����Ă�������Z�b�g
	void SetDir(DIR d);
	DIR GetTmpDir() const { return tmpDir; }
	// �����Ă�������擾
	DIR GetDir() const { return dir; }

	// �G�̏��Z�b�g
	void SetEnemyState(EnemyState state);
	// �G�̏��擾
	EnemyState GetEnemyState() const { return enemyState; }

	// ��ڲ԰�ɗ^������Ұ�ޗʂ���������@(����: �G�̱�Ұ��݂̑���)
	virtual void CheckDamage(ANIM_ATTRIBUTE att);

	// �v���C���[��HP�����Z
	void AddPlayerHP(int addHP);
	void AddPlayerHPOld(int addHP);

	// �v���C���[��HP���擾
	int GetPlayerHP();

	// �A�j���[�V�������~�߂�t���O���Z�b�g
	void SetAnimStopFlag(bool flag) { animStopFlag = flag; }
	bool GetAnimStopFlag() const { return animStopFlag; }

	// �A�j���[�V�������I�����Ă��邩
	bool GetAnimEndFlag() const { return animEndFlag; }

	// �A�j���[�V�����J�E���g���擾
	const int GetAnimCount() const { return animCnt; }
	const VECTOR2 GetDrawOffSet() const { return drawOffset; }

	void SetHitData(bool flag, ColType type);
	bool GetHitFlag() const { return hitData.hitFlag; }
	ColType GetHitBoxType() const { return hitData.colType; }

	PAD_ID GetPadID() const { return padID; }

	const VECTOR2 GetAnimOffSet(std::string animName);

	// ���݂̃A�j���[�V�����̃t���[�����擾
	int GetFrame();

	// �����蔻��̏��������ƁA����ɔ����ǉ��ōs������
	virtual void CheckHitFlag(void);

private:
	//�ړ�����
	virtual void SetMove(const GameCtrl &ctl);
	virtual void SetMove(const GameCtrl &ctl, weekListObj objList);

protected:
	VECTOR2 pos;					// ���W

	std::string imageName;			// �\���摜̧�ٖ�
	VECTOR2 divSize;				// ��������
	VECTOR2 divCut;					// ������
	const VECTOR2 drawOffset;		// �`��̾��
	unsigned int animCnt;			// ��Ұ��ݶ����
	bool animEndFlag;				// ��Ұ��ݍŏI�ϓ��B�׸�
	bool visible;					// �\���E��\���׸�
	DIR dir;						// �����Ă������
	DIR tmpDir;						// �����Ă������(�ꎞ�I�ɕۑ�)
	bool turnFlag;					// �摜���]�t���O
	EnemyState enemyState;			// �G�̌��݂̍��W, ���
	PAD_ID padID;					// �g�p���Ă���PAD�ԍ�
	HitData hitData;				// �����������
	int playerHP;					// �v���C���[��HP
	int playerHPOld;				// �v���C���[HP�ύX�O�̒l
	float DrawHPCount;				// HP�`��̃J�E���g
	bool animStopFlag;				// �A�j���[�V�������Ƃ߂�t���O

	std::map<std::string, int[ANIM_TBL_MAX]> animTable;				// ��Ұ��ݏ��
	std::string animName;											// �\����Ұ��ݖ�
	ANIM_ATTRIBUTE animAttribute[3];								// ��Ұ��݂̑���
	std::map<std::string, ANIM_ATTRIBUTE[3]> animAttributeTbl;		// ��Ұ��݂̑�����ð���
};
