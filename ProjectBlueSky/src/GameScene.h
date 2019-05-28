#pragma once
#include "BaseScene.h"

#include "VECTOR2.h"
#include "ObjList.h"
#include "Obj.h"

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);		// ���ڰя���������̂��܂Ƃ߂��֐��Astate

private:
	int Init(void);
	void BgPosUpDate(void);		// �w�i�̈ʒu���W�̍X�V�ƁA����ɔ�����׸װ�̍Ĕz�u
	bool CheckGameEnd();		// ���s���`�F�b�N		
	void ExtrusionUpdata();				// �����o���̓����蔻��

	// �L�����N�^�[��Obj���
	struct CharacterObj
	{
		CharacterObj() : charaObj(nullptr), charaType(OBJ_TYPE_MAX), winCount(0) {}
		CharacterObj(sharedObj obj, OBJ_TYPE type) : charaObj(obj), charaType(type), winCount(0) {}

		sharedObj charaObj;
		OBJ_TYPE charaType;
		bool AttackHitOld;	// �O�̃t���[���ōU�����������Ă������ǂ���
		int winCount;		// �������̃J�E���g
	};

	VECTOR2 ssize;				// ��ʻ���
	VECTOR2 bgPos;				// �w�i�̈ʒu���W
	VECTOR2 bgPosOld;			// 1�ڰёO�̔w�i�̈ʒu���W
	VECTOR2 beforPos[2];
	VECTOR2 afterPos[2];
	bool GameDraw(void);		// �`�揈��
	int id[2];					// �����蔻��̃t���[����

	//sharedObj sObj[2];			// �L�����N�^�[��Obj�ϐ��ۑ�
	//OBJ_TYPE type[2];			// �L�����N�^�[�̃^�C�v
	CharacterObj charaObj[2];
	ColInfo colData[2];			// �����蔻��̏����i�[����ϐ�(2�L������)

	bool hitStopFlag;			// �q�b�g�X�g�b�v�p���׸�
	bool gameEndFlag;			// �Q�[���̏I���t���O

	int loseCharacter;
	int winCharacter;
	bool drawflag;
	bool damageFlag[2];			// ��Ұ�ނ��d��

	int koDrawCount;			// ko��\�����鎞�Ԃ̃J�E���g
	int koImageHandle;			// ko�`�掞�̃n���h��

	
	sharedListObj objList;
};

