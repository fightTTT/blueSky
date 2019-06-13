#pragma once
#include "Obj.h"

#include <map>
#include <list>
#include <array>
#include <vector>

enum PAD_ID;

enum SP_COM					// �Z�����
{
	SP_COM_CENTER,			// ����
	SP_COM_UP,				// ��
	SP_COM_RIGHT_UP,		// �E��
	SP_COM_RIGHT,			// �E
	SP_COM_RIGHT_DOWN,		// �E��
	SP_COM_DOWN,			// ��
	SP_COM_LEFT_DOWN,		// ����
	SP_COM_LEFT,			// ��
	SP_COM_LEFT_UP,			// ����

	SP_COM_ACCUMULATE,		// ��

	SP_COM_PUNCH,			// ����
	SP_COM_KICK,			// ���

	SP_COM_MAX
};

class Character :
	public Obj
{
public:
	Character(VECTOR2 offset);
	~Character();

	bool CheckObjType(OBJ_TYPE type);

	void SetMove(const GameCtrl &ctl, weekListObj objList);			// ���X�V
	void CheckHitFlag(void);										// �����蔻��̏��������ƁA����ɔ����ǉ��ōs������

private:
	VECTOR2 ssize;		// ��ذ݂̻���

	std::map<std::string, std::string> animFileName;		// ��Ұ��݂̉摜̧�ٖ� (��: ��Ұ��ݖ�)

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

	int spEndCnt;		// �K�E�Z�̏I�����ݸފǗ��p�̶���
	int guardEndCnt;

	// ��Ұ��݂̏������֐�
	bool InitAnim(void);

	// ��׸���̑�������ފ֐��߲��
	void (Character::*act)(const GameCtrl &, weekListObj);

	// ��׸���̑��� (��{)
	void NeutralState(const GameCtrl & ctl, weekListObj objList);

	// ��׸���̑��� (��)
	void AirState(const GameCtrl & ctl, weekListObj objList);

	// ��׸���̑��� (�_���[�W)
	void DamageState(const GameCtrl & ctl, weekListObj objList);

	// ��׸���̑��� (�K�E�Z)
	void SpAttackState(const GameCtrl & ctl, weekListObj objList);

	// �߯�ނ̓��͏������ɺ���ނƂ���ؽĂɊi�[���� (����: �ްѺ��۰ٸ׽���߲��)
	void CommandUpDate(const GameCtrl & ctl);

	// �����ؽĂƈ����œn���ꂽ�ԍ��̺���ނ��r���A��v������true�A�s��v�Ȃ�false��Ԃ�
	bool CheckCommand(int skillNum);

	// ��ڲ԰�ɗ^������Ұ�ޗʂ���������@(����: �G�̱�Ұ��݂̑���)
	void CheckDamage(ANIM_ATTRIBUTE att);

	// �ް�ނ̱�Ұ��݂̍Œ�\�����Ԃ��
	void GuardEndCnt(int cnt);

	// �`�揈��
	void Draw(void);

protected:
	std::string characterName;
	std::string spAttackAnimName[3];
	std::string spAttackAnimFileName[3];
	std::map<std::string, int> spAttackDamage;
	std::array<std::array<std::vector<SP_COM>, DIR_MAX>, 3> spAttackCommand;		// spAttackCommand[�Z�ԍ�][�����̕���][�R�}���h]

	int shotCreateCnt;		// ���Ă𐶐�����܂ł̶���

	// �������֐� (����: ̧�ٖ�, ��������, ������, ���W, ���]�׸�, �p�b�h�ԍ�)
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn, PAD_ID id);
};

