#pragma once
#include <list>
#include <map>
#include "Gamecontroller.h"
#include "BaseScene.h"
#include "ColMng.h"

#define P_CHAR_MAX	(3)		// �o�귬ׂ̑���
#define ANIM_MAX	(21)	// ��Ұ�������݂̑���

class CollisionEditScene :
	public BaseScene
{
public:
	CollisionEditScene();
	~CollisionEditScene();
	unique_Base Update(unique_Base own, const GameController &controller);
	void Draw(void);

private:
	bool ColSave(void);				// �ײ�ް���̾���

	bool ColSave(std::string CharName,std::string AnimName);// �ײ�ް���̾���(���c�V�X�e���Q�e�X�g����)

	void ChangePicture(void);								//�R���C�_�[��ݒ肷��摜�̕ύX(���c�V�X�e��_�e�X�g����)

	int Init(void);					// �������֌W
	void nowIdListInit(int id);		// nowIdList�̏�����	id���ς�邽�тɌĂ�
	void ColInfoInit(void);			// �ײ�ް���̏�����

	void AddCol(void);				// �ײ�ް���̒ǉ�

	void DrawGrid(void);			// �r���̕`��
	void DrawInfo(int colColor);	// �ð�����̕`��@pos�l�Ȃ�

	std::vector<ColInf> colVector;	// �ײ�ް��ۑ�����Vector
	std::list<ColInf> colList;		// �ײ�ް��ۑ�����List
	std::list<ColInf> nowIdList;		// ���݂�id�����������Ă���List

	std::string NowCharName;		//���ݐݒ蒆�̃L�����N�^�[���@(���c�V�X�e���Q�e�X�g����)
	std::string NowAnimName;		//���ݐݒ蒆�̃A�j���[�V������(���c�V�X�e���Q�e�X�g����)

	std::string NowUseDataName;		//���ݎg�p���̘A�ԃf�[�^��(���c�V�X�e���Q�e�X�g����)

	int id;							// �\���摜��ID

	VECTOR2 nowPos;

	ColInf data; 
	
	std::array<std::string, P_CHAR_MAX>	charNameTbl;	// ��ז����i�[
	std::array<std::string, ANIM_MAX>	animNameTbl;	// ��Ұ��ݖ����i�[
	std::array<std::string, ANIM_MAX>	dataNameTbl;	// ��Ұ���̧�ٖ����i�[
	std::array<int, 3> maxTblPtn;						// �eNameTbl�̍ő�v�f�����i�[
	int tblCnt;						// NameTbl�̗v�f�ɱ������邽�߂̶���
	bool changePictureFlag;			// ��ި�ĉ摜�ύX��ԊǗ��׸�	true:�ύX
	int changeCnt;					// �ύX������̂�\������	0:��ז�	1:��Ұ��ݖ�	���̑�:�ύX�I��

	bool newEditFlag;				// true:�ؼޮ݂̐V�K�ǉ�	false:�����̺ؼޮ݂̏C��
	std::list<ColInf>::iterator			editItr;		// ���ݕҏW���̺ؼޮ݂��w��
	int editCnt;					// ���ݕҏW���̺ײ�ް��_�ŕ\�������邽�߂̶����
};
