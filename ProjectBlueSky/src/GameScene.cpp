#include "Dxlib.h"
#include "ObjList.h"
#include "Obj.h"
#include "StickHuman.h"
#include "AIStickHuman.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "GameScene.h"
#include "GameCtrl.h"

#define RYU_IMAGE_SIZE_X (256)
#define RYU_IMAGE_SIZE_Y (256)

#define BG_IMAGE_SIZE_X (1920)
#define BG_IMAGE_SIZE_Y (720)


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

	sharedObj sObj[2];	// �L�����N�^�[��Obj�ϐ��ۑ�
	int charaCount = 0;	// ���������L�����N�^�[�̐�
	OBJ_TYPE type[2];	// �L�����N�^�[�̃^�C�v
	EnemyState eState;

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

		// shot�̏����Z�b�g
		if (data->CheckObjType(OBJ_TYPE_SHOT))
		{
			ShotData shot(data->GetPos(), data->GetPadID());
			eState.pushBackShotData(shot);
		}

		// �L�����N�^�[��2�l���������ꍇ
		if (charaCount == 2)
		{
			// �L�����N�^�[���m������������
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

			// �L�����N�^�[�̏�Ԃ𑊎�ɓn��
			if (!(type[0] == type[1]))
			{
				if (type[0] == OBJ_TYPE_CHARACTER)
				{
					eState.enemyPos = sObj[0]->GetPos();
					eState.enemyAnimName = sObj[0]->GetAnim();
					sObj[1]->SetEnemyState(eState);
				}
				else
				{
					eState.enemyPos = sObj[1]->GetPos();
					eState.enemyAnimName = sObj[1]->GetAnim();
					sObj[0]->SetEnemyState(eState);
				}
			}
		}
	}

	//�`�揈��
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
	objList->clear();		// objList��S�폜����

	AddObjList()(objList, std::make_unique<AIStickHuman>(VECTOR2(1440 - (290 / 2), ssize.y), VECTOR2(-(290 / 2), -178), DIR_RIGHT));
	AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2((RYU_IMAGE_SIZE_X / 2), ssize.y), VECTOR2(-(290 / 2), -178), PAD_1, DIR_LEFT));
	//AddObjList()(objList, std::make_unique<AIRyu>(VECTOR2(ssize.x - (RYU_IMAGE_SIZE_X / 2), ssize.y), VECTOR2(-(290 / 2), -178), DIR_LEFT));

	return 0;
}

bool GameScene::GameDraw(void)
{
	DrawGraph(-(BG_IMAGE_SIZE_X / 6), 0, IMAGE_ID("image/bluesky_�w�i.png")[0], true);

	//objList�ɓo�^����Ă���׽�̕`�揈�����s��
	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	return true;
}
