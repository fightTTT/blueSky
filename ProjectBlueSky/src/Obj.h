#pragma once
#include<string>
#include<map>
#include"VECTOR2.h"
#include"ObjList.h"

enum ANIM_TBL {
	ANIM_TBL_START_ID,		//�J�n�ʒu
	ANIM_TBL_FRAME,			//�ϐ�
	ANIM_TBL_INV,			//�Ԋu
	ANIM_TBL_LOOP,			//ٰ�߂��邩�ǂ���
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

struct EnemyState
{
	EnemyState() : enemyPos(VECTOR2(0, 0)), enemyAnimName("none") {}
	EnemyState(VECTOR2 pos, std::string animName) : enemyPos(pos), enemyAnimName(animName) {}

	VECTOR2 enemyPos;
	std::string enemyAnimName;
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

	bool AddAnim(std::string animName, int id_x, int id_y, int frame, int inv, bool loop);		// ��Ұ��݂�ǉ�����
	bool SetAnim(std::string animName);															// ��Ұ��݂�Ă���
	std::string GetAnim(void);			// ��Ұ��݂̏����擾����

	// �T�C�Y�擾
	VECTOR2 GetDivSize() const { return divSize; }

	// �����Ă�������Z�b�g
	void SetDir(DIR d);
	// �����Ă�������擾
	DIR GetDir() const { return dir; }

	// �G�̏��Z�b�g
	void SetEnemyState(EnemyState state);
	// �G�̏��擾
	EnemyState GetEnemyState() const { return enemyState; }

	// �A�j���[�V�������I�����Ă��邩
	bool GetAnimEndFlag() const { return animEndFlag; }

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

	std::map<std::string, int[ANIM_TBL_MAX]> animTable;		// ��Ұ��ݏ��
	std::string animName;									// �\����Ұ��ݖ�
};
