#pragma once
#include <list>
#include "classObj.h"
#include "classFloor.h"
#include "classChicken.h"
#include "Chicken.h"
#include "BaseScene.h"
#include "SelectScene.h"
#include "StageSelectScene.h"
#include "ExpOrb.h"
#include "Gamecontroller.h"
#include "Fader.h"

#define HP	200

using namespace std;

enum GAME_STATE {
	ENTRY,
	TUTORIAL,
	GRAYOUT_SLIDE,
	INSTRUCTION,
	COUNT_DOWN,
	PLAY,
	PAUSE,
	CUT_IN,
	GAME_FINISH,
	GAME_STATE_MAX
};

enum IMAGE_ID
{
	IMAGE_SCHOOL,
	IMAGE_TREE,
	IMAGE_ARCADE,
	IMAGE_MAX
};

enum CHANGE_SCENE {
	CHANGE_SCENE_NON,			// ��݈ڍs���Ȃ�
	CHANGE_SCENE_CHAR_SEL,		// ��׾ڂ�
	CHANGE_SCENE_RESULT,		// ػ��Ă�
	CHANGE_SCENE_INSTRUCTION,	// ������@��\��
	CHANGE_SCENE_MAX
};

typedef struct {
	std::array<int, PLAYER_MAX> totalGiveDamage;	// İ�قŗ^������Ұ��
	std::array<int, PLAYER_MAX> deathEnemyCnt;		// İ�قœ|�����G�̐�
	std::array<int, PLAYER_MAX> chickenKillCnt;		// �{�̎E������
	std::array<int, PLAYER_MAX> chickenCnt;			// ���̔ɉh��	�E�����x��������̂ŎE���ꂽ��
	std::array<int, PLAYER_MAX> deathOrder;			// ���񂾏���	0=�����Ă���@1�ȏ�=���񂾏���
	std::array<std::string, PLAYER_MAX> charName;	// ��ז�
}RESULT_INFO;

typedef struct {
	VECTOR2 pos;			// �K�E�Z�����҂̍��W
	int padNum;				// �K�E�Z�����҂��߯�ޔԍ�
	int cnt;
	bool flag;
}CUT_IN_INFO;


class GameScene :
	public BaseScene
{
public:
	GameScene();
	GameScene(CHAR_ID playerID[PLAYER_MAX], int joinPlayerNum, STAGE_ID ID);
	~GameScene();
	unique_Base Update(unique_Base own, const GameController &controller);

private:
	void GamePlay(bool doCollision);		// ����(true:�����蔻����s�� false:�����蔻����s��Ȃ�)
	void PlayerIns();						// ��ڲԂ�1��ׂ��²ݽ�ݽ����
	void GameEntry(void);					// ���ذ
	void Tutorial(void);					// ����ر�
	void GrayOutSlide(void);				// ��ڰ��Ă���ɽײ�ނ��鏈��
	void Instruction(void);					// ��������\��
	void CountDown(void);					// 3�E2�E1�̶��Ē��̏���
	void GameFunc(void);					// �ްђ��̏���
	void PauseFunc(void);					// �߰��Ӱ�ޒ��̏���
	void CutInFunc(void);					// ��Ĳݒ��̏���

	bool CheckGameEnd(void);				// �ްяI�����ǂ����𒲂ׂ�
	bool GameDraw();

	bool WhiteOut(void);					// �ܲı�Ă̕`��
	void PageInstance(void);				// �߰�ނ߂���̏�����(ػ��ėp)
	void BufferChange(void);				// DX_SCREEN_BACK��tmpBuffer�̕`����ʂ�
	int Init(void);

	sharedListObj objList;
	sharedListFloor floorList;
	sharedListChicken chickenList;

	IMAGE_ID imageID;
	std::array < std::string, IMAGE_MAX > imageName;	// �v���O���}�̃��`�x�ێ��ׂ̈̉摜�ł���
	VECTOR2 drawOffset;

	std::array<CHAR_ID, PLAYER_MAX> players;			// ��ڲԂ��I�񂾷�ׂ̏��
	std::array<VECTOR2, PLAYER_MAX> gameStartPos;		// �ްъJ�n���̍��W
	std::array<bool, PLAYER_MAX> entryFlag;			// �ްюQ���׸ށ@false�ŏ��������Atrue�ɂȂ����緬ׂ�ݽ�ݽ����

	std::list<ExpOrb*> expOrbList;						// �o���l�ʂ�����list

	GameController controller;		// ���۰װ���

	GAME_STATE gameState;	// �ްѼ�݂̏�ԑJ��

	int gameSceneCnt;		// �ްѼ�݂ɓ����Ă���̌o���ڰ�	0����++����Ă���
	int gameCnt;			// �ްт��n�܂��Ă���̌o���ڰ�		�l��--���Ă���0�ŏI��
	int gameStartCnt;		// 3�E2�E1�̕\���p 0����++���Ă���
	int resSceneCnt;		// �퓬�I����������ػ��ļ�݂ɑJ�ڂ���܂ł̶���
	int grayOutCnt;			// ��ڰ��Ă�ײ�ނ���Ƃ��Ɏg�p���鶳��		ϲŽ�l�ŏ��������Ă���(�҂�����)
	int gemeEndCnt;			// ���҂̎���

	int joinPlayerNum;		// �Q���l��
	int pausePadNum;		// �߰�ނֈڍs�����߯Ă̔ԍ����i�[

	CHANGE_SCENE changeSnece;	// �߰��Ӱ�ނŷ�׾ڂɖ߂�����ػ��Ăɐi�񂾂肷��Ƃ��Ɏg�p

	CUT_IN_INFO cutInInfo;	// ��Ĳ݂̍ۂɕK�v�ȏ��

	STAGE_ID stageID;		// �I�������ð��

	// �_�̕`����W
	VECTOR2 cloudFrontPos1;
	VECTOR2 cloudFrontPos2;
	VECTOR2 cloudBackPos1;
	VECTOR2 cloudBackPos2;

	// SE�p
	int PreCnt;
	bool sleepFlag;
	bool chengFlag;

	RESULT_INFO resultInfo;			// ػ��ĉ�ʂŕK�v�ȏ��

	bool showInstruction;			// true:��������摜��\������(����رقŎg�p)

	bool reStartFlag;				// �ްс��߰�ށ��ްт̎���true�@�@���̎������޳ݒ���ב���s��

	array<ListFloor_itr, FLOOR_MAX> floorTable;

	TurnThePage turnThePage;
	int lastGameScreen;				// �I�����̹ްѽ�ذ݉摜
	Fader<double> alpha;			// ̪��ޱ�Ẳ�ʓ����x
	MODE mode;

	int captureImage;				// �摜���������ĕۑ�����		��׾ڑJ�ڂ̎��Ɏg�p
	float ExtRate;
	bool ExtendEndF;				// ture:�k���I���				��׾ڑJ�ڂ̎��Ɏg�p

	// �����̒����擾�p
	std::string StageName;
	int len;
};

