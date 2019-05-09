#include "Dxlib.h"
#include "ObjList.h"
#include "Obj.h"
#include "Ryu.h"
#include "StickHuman.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "GameCtrl.h"
#include "AIRyu.h"

#define RYU_IMAGE_SIZE_X (290)
#define RYU_IMAGE_SIZE_Y (178)


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
					EnemyState state(sObj[0]->GetPos(), sObj[0]->GetLongAttackFlag(), sObj[0]->GetAnim());
					sObj[1]->SetEnemyState(state);
				}
				else
				{
					EnemyState state(sObj[1]->GetPos(), sObj[1]->GetLongAttackFlag(), sObj[1]->GetAnim());
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
	auto ssize = lpSceneMng.GetScreenSize();
	
	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objListを全削除する

	//AddObjList()(objList, std::make_unique<Ryu>(VECTOR2(1440 - (290 / 2), ssize.y), VECTOR2(-(290 / 2), -178), PAD_2, DIR_RIGHT));
	AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2(1440 - (290 / 2), ssize.y), VECTOR2(-(290 / 2), -178), PAD_1, DIR_RIGHT));
	AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2((RYU_IMAGE_SIZE_X / 2), ssize.y), VECTOR2(-(290 / 2), -178), PAD_2, DIR_LEFT));
	//AddObjList()(objList, std::make_unique<AIRyu>(VECTOR2(ssize.x - (RYU_IMAGE_SIZE_X / 2), ssize.y), VECTOR2(-(290 / 2), -178), DIR_LEFT));

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
