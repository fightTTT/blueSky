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

	VECTOR2 ssize;				// ��ʻ���
	VECTOR2 bgPos;				// �w�i�̈ʒu���W
	VECTOR2 bgPosOld;			// 1�ڰёO�̔w�i�̈ʒu���W

	bool GameDraw(void);		// �`�揈��
	int id[2];			// �����蔻��̃t���[����

	sharedObj sObj[2];	// �L�����N�^�[��Obj�ϐ��ۑ�
	OBJ_TYPE type[2];	// �L�����N�^�[�̃^�C�v

	sharedListObj objList;
};

