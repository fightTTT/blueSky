#include "Dxlib.h"
#include "ObjList.h"
#include "Obj.h"
#include "Ryu.h"
//#include "SceneMng.h"
#include "GameScene.h"
#include "GameCtrl.h"
#include "AIRyu.h"


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

	auto deth_itr = std::remove_if(objList->begin(), objList->end(), [](std::shared_ptr<Obj> obj) {return obj->CheckDeth(); });
	objList->erase(deth_itr, objList->end());

	sharedObj sObj[2];	// キャラクターのObj変数保存
	int charaCount = 0;	// 見つかったキャラクターの数
	OBJ_TYPE type[2];	// キャラクターのタイプ

	for (auto& data : *objList)
	{
		if (data->CheckObjType(OBJ_TYPE_CHARACTER))
		{
			sObj[charaCount] = data;
			type[charaCount] = OBJ_TYPE_CHARACTER;
			charaCount++;
		}
		else if (data->CheckObjType(OBJ_TYPE_AICHARACTER))
		{
			sObj[charaCount] = data;
			type[charaCount] = OBJ_TYPE_AICHARACTER;
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

			if (!(type[0] == type[1]))
			{
				if (type[0] == OBJ_TYPE_CHARACTER)
				{
					EnemyState state(sObj[0]->GetPos(), sObj[0]->GetAnim());
					sObj[1]->SetEnemyState(state);
				}
				else
				{
					EnemyState state(sObj[1]->GetPos(), sObj[1]->GetAnim());
					sObj[0]->SetEnemyState(state);
				}
			}
		}
	}

	//描画処理
	GameDraw();

	return std::move(own);
}

int GameScene::Init(void)
{
	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objListを全削除する

	//AddObjList()(objList, std::make_unique<Ryu>(VECTOR2(1440 - (290 / 2), 810), VECTOR2(-(290 / 2), -178), PAD_2, DIR_RIGHT));
	AddObjList()(objList, std::make_unique<Ryu>(VECTOR2((290 / 2), 810), VECTOR2(-(290 / 2), -178), PAD_1, DIR_RIGHT));
	AddObjList()(objList, std::make_unique<AIRyu>(VECTOR2(1440 - (290 / 2), 810), VECTOR2(-(290 / 2), -178), DIR_LEFT));

	//AddObjList()(objList, std::make_unique<Ryu>(lpSceneMng.GetDrawOffset(), popNo));
	return 0;
}

bool GameScene::GameDraw(void)
{
	//objListに登録されているｸﾗｽの描画処理を行う
	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	return true;
}
