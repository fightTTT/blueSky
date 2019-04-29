#include "Dxlib.h"
#include "ObjList.h"
#include "Obj.h"
#include "Ryu.h"
//#include "SceneMng.h"
#include "GameScene.h"


GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}


unique_Base GameScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	//�`�揈��
	GameDraw();

	return std::move(own);
}

int GameScene::Init(void)
{
	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objList��S�폜����

	AddObjList()(objList, std::make_unique<Ryu>());

	//AddObjList()(objList, std::make_unique<Ryu>(lpSceneMng.GetDrawOffset(), popNo));
	return 0;
}

bool GameScene::GameDraw(void)
{
	ClsDrawScreen();		// ��ʏ���

	//objList�ɓo�^����Ă���׽�̕`�揈�����s��
	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	ScreenFlip();		// �ް�ٰ�߂̍Ō�ɕK���K�v
	return true;
}
