#pragma once
#include "BaseScene.h"
#include "classObj.h"
#include "SelectScene.h"
#include "TurnThePage.h"
#include "VECTOR2.h"
#include "GameController.h"

using namespace std;

// �ð��ID
enum STAGE_ID
{
	STAGE_SCHOOL,			// �Z��
	STAGE_TREE_OF_LEGEND,	// �`���̖�
	STAGE_ARCADE,			// ���X�X
	STAGE_RANDUM,			// �����
	STAGE_MAX
};

typedef struct {
	STAGE_ID	id;
	VECTOR2		drawPos;
	int			imageL;
	int			imageR;
	VECTOR2		startPos;
	VECTOR2		endPos;
}StageTbl;



class StageSelectScene :
	public BaseScene
{
public:
	StageSelectScene(std::array<CHAR_ID,PLAYER_MAX>ID, int num);
	~StageSelectScene();

	unique_Base Update(unique_Base own, const GameController &ctrl);
	
private:

	bool PageDraw();

	bool Draw(void);

	int Init(void);

	STAGE_ID stageID;	// �I��ð��ID
	STAGE_ID stageDrawID;
	CHAR_ID ID[4];
	int num;

	array< string, static_cast<size_t>(STAGE_MAX) > sceneName;			// CHAR_ID�̏��ԂɑΉ�������ׂ̖��O

	array<int, (static_cast<int>(STAGE_MAX)-1)>	gameSceneImage;

	array<array<StageTbl, 2>, 2>	stageTbl;

	array<array<VECTOR2, 2>, 4>		dirPosTbl;		// ���摜�̍��W�p
	array<array<double,  2>, 4>		dirRotaTbl;		// ���摜�̉�]�p�x

	TurnThePage turnThePage;	// ��׾ڂɖ߂�Ƃ��Ɏg�p
	int pageImage;

	MODE mode;
	bool endFlag;		// �X�e�[�W�����肵�Ă����true;
	int flashDrawCnt;	// �_�ŏ����p����

	VECTOR2 cursor;
	VECTOR2 tblPos;

	double flashCnt;

	GameController controller;		// ���۰װ���
};

