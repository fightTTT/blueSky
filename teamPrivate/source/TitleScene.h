#pragma once
#include <list>
#include "BaseScene.h"
#include "classObj.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own, const GameController &controller);

private:
	bool TitleDraw(void);	// �`��
	bool PageDraw(void);

	int Init(void);	// �������p

	sharedListObj objList;
	int Cnt;				// ���ٓ_�ŗp����
	int flasing;			// �_�ł̌�����
	int ChangeSceneFlag;	// ��ʑJ�ڗp
	int startPadNum;		// �������݂�������PAD�m�F�p

	TurnThePage turnThePage;
	MODE mode;
};

