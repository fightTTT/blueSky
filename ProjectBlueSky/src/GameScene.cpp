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
	//•`‰æˆ—
	GameDraw();

	return std::move(own);
}

int GameScene::Init(void)
{
	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objList‚ğ‘Síœ‚·‚é

	AddObjList()(objList, std::make_unique<Ryu>());

	//AddObjList()(objList, std::make_unique<Ryu>(lpSceneMng.GetDrawOffset(), popNo));
	return 0;
}

bool GameScene::GameDraw(void)
{
	ClsDrawScreen();		// ‰æ–ÊÁ‹

	//objList‚É“o˜^‚³‚ê‚Ä‚¢‚é¸×½‚Ì•`‰æˆ—‚ğs‚¤
	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	ScreenFlip();		// ¹Ş°ÑÙ°Ìß‚ÌÅŒã‚É•K‚¸•K—v
	return true;
}
