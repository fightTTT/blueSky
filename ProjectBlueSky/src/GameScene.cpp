#include "Dxlib.h"
#include "ObjList.h"
#include "Obj.h"
#include "Ryu.h"
//#include "SceneMng.h"
#include "GameScene.h"
#include "GameCtrl.h"


GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}


unique_Base GameScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	for (auto& data : *objList)
	{
		data->UpDate(controller, objList);
	}

	sharedObj sObj[2];
	int charaCount = 0;

	for (auto& data : *objList)
	{
		if (data->CheckObjType(OBJ_TYPE_CHARACTER))
		{
			sObj[charaCount] = data;
			charaCount++;

		}

		if (charaCount == 2)
		{
			if (sObj[0]->GetPos().x > sObj[1]->GetPos().x)
			{
				sObj[0]->SetDir(DIR_LEFT);
				sObj[1]->SetDir(DIR_RIGHT);
			}
			else
			{
				sObj[0]->SetDir(DIR_RIGHT);
				sObj[1]->SetDir(DIR_LEFT);
			}
		}
	}

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

	AddObjList()(objList, std::make_unique<Ryu>(VECTOR2(1440 - (290 / 2), 810), VECTOR2(-(290 / 2), -178), PAD_1, DIR_RIGHT));
	AddObjList()(objList, std::make_unique<Ryu>(VECTOR2((290 / 2), 810), VECTOR2(-(290 / 2), -178), PAD_2, DIR_LEFT));

	//AddObjList()(objList, std::make_unique<Ryu>(lpSceneMng.GetDrawOffset(), popNo));
	return 0;
}

bool GameScene::GameDraw(void)
{
	//objList‚É“o˜^‚³‚ê‚Ä‚¢‚é¸×½‚Ì•`‰æˆ—‚ğs‚¤
	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	return true;
}
