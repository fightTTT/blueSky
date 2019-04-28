#pragma once
#include "VECTOR2.h"
#include "BaseScene.h"
#include "classObj.h"
#include "TurnThePage.h"
#include "GameController.h"
#include "Fader.h"


// ��`
#define PLAYER_MAX 4	// ��ڲ԰��

// ��׸��ID
enum CHAR_ID {
	ID_NON,		// ����
	ID_Umb,		// �P
	ID_Bro,		// �
	ID_Lid,		// �W
	ID_Rnd,		// �����
	ID_Max
};

// ��ݑJ�ڎ���Ӱ�ސؑ֗p
enum MODE {
	MODE_SCENE_BACK,	// �O�̼�݂֑J�ڂ���Ƃ�
	MODE_SCENE_NEXT,	// ���̼�݂֑J�ڂ���Ƃ�
	MODE_SCENE_NOW,		// ��ݑJ�ڂȂ�
	MODE_MAX
};

// ��ڲ԰�p�\����
typedef struct {
	CHAR_ID ID;		// �I���ID
	bool flag;		// �I�����
	int posY;		// �I�����W
	int cnt;		// ���ݓ_�ŗp����
}PlayerInfo;

class SelectScene :
	public BaseScene
{
public:
	SelectScene(bool whiteInFlag = false);	// whiteInFlag:ture�@�ܲĲݗL		 whiteInFlag:false �ܲĲ݂Ȃ�
	~SelectScene();

	unique_Base Update(unique_Base own, const GameController &controller);

private:
	bool PageDraw(void);
	bool Draw(void);
	int SelectEndNum(void);			// ��בI��flag = ture�̐l����Ԃ� 

	int Init(void);

	GameController controller;		// ���۰װ���

	std::array<std::string, ID_Max> charName;			// CHAR_ID�̏��ԂɑΉ�������ׂ̖��O
	PlayerInfo player[PLAYER_MAX];	// ��ڲ԰�p�z��

	int joinPlayerNum;				// �Q���l��

	int flashDrawCnt;				// START�ŃQ�[���J�n�̓_�ŗp

	std::array<CHAR_ID, PLAYER_MAX> playerIdArray;
	TurnThePage turnThePage;

	MODE mode;

	Fader<double> alpha;			// ̪��޲݂̉�ʓ����x
};


