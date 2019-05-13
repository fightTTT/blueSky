#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "GameCtrl.h"
#include "VECTOR2.h"
#include "DxLib.h"
#include "CharSelCursor.h"
#include "ImageMng.h"

#define BOX_SIZE_X (100)
#define BOX_SIZE_Y (100)

SelectScene::SelectScene()
{
	Init();
}

SelectScene::~SelectScene()
{
}

unique_Base SelectScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	for (auto& data : *objList)
	{
		data->UpDate(controller, objList);
	}

	/* �ްѼ�݂ֈړ� */
	if (controller.GetPadDataTrg(PAD_1, BUTTON_START) || controller.GetPadDataTrg(PAD_2, BUTTON_START))
	{
		return std::make_unique<GameScene>();
	}

	SelectDraw();

	return std::move(own);
}

int SelectScene::Init()
{
	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objList��S�폜����

	AddObjList()(objList, std::make_unique<CharSelCursor>());		// ؽĂ�Cursor�׽��ǉ�

	scSize = lpSceneMng.GetScreenSize();

	charNameTbl = { "�V��",
					"�N��",
					"�A�J",
					"�A�I",
					"�~�h��",
					"�L�C��",
					"�����T�L",
					"�`��" };

	return 0;
}

void SelectScene::SelectDraw()
{
	DrawGraph((scSize.x / 2) - (BOX_SIZE_X * 2), (scSize.y * 3 / 5), IMAGE_ID("image/charSel.png")[0], true);		// ��׈ꗗ��`��
			
	// ���O
	DrawFormatString(10, (scSize.y /2), 0xffffff, "%s", charNameTbl[lpSceneMng.GetCharID()]);

	for (auto &data : (*objList))
	{
		(*data).Draw();
	}

	//DrawString(1200, 200, "SelectScene", 0xffffff);

	DrawString(1000, 10, "START�{�^�� or SpaceKey�őJ��", 0xffffff);

}
