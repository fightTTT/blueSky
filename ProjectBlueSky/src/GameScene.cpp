#include "Dxlib.h"
#include "ObjList.h"
#include "Obj.h"
#include "StickHuman.h"
#include "AIStickHuman.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "GameScene.h"
#include "GameCtrl.h"
#include "CollisionMng.h"

#define STICK_HUMAN_IMAGE_SIZE_X (256)
#define STICK_HUMAN_IMAGE_SIZE_Y (256)

#define BG_IMAGE_SIZE_X (1920)
#define BG_IMAGE_SIZE_Y (720)

// ���ޯ�ү���ޗp��`
#ifdef _DEBUG		// ���s���̏���
#define AST() {\
	CHAR ast_mes[256];\
	wsprintf(ast_mes, "%s %d�s��\0", __FILE__, __LINE__);\
	MessageBox(0, ast_mes, "���ĕ\��", MB_OK);\
	}
#else				// �������̏���
#define AST()
#endif		// _DEBUG

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

	sharedObj sObj[2];	// �L�����N�^�[��Obj�ϐ��ۑ�
	sharedObj shotObj;
	int charaCount = 0;	// ���������L�����N�^�[�̐�
	OBJ_TYPE type[2];	// �L�����N�^�[�̃^�C�v
	EnemyState eState;

	auto deth_itr = std::remove_if(objList->begin(), objList->end(), [](std::shared_ptr<Obj> obj) {return obj->CheckDeth(); });
	objList->erase(deth_itr, objList->end());

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
			if (data->GetPadID() == PAD_1)
			{
				shotObj = data;
			}
			else if (data->GetPadID() == PAD_2 || data->GetPadID() == PAD_AI)
			{
				shotObj = data;
			}
			else
			{
				AST();
			}
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

			// ��l���������ł�����l���U���n�̃A�j���[�V�����̎��ɁA�����������Ă�������K�[�h��Ԃɂ���
			if (sObj[0]->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK)
			{
				if (sObj[1]->GetAnim() == "���ړ�")
				{
					sObj[1]->SetAnim("�K�[�h_����");
				}

				if (sObj[1]->GetAnim() == "���Ⴊ��_���")
				{
					sObj[1]->SetAnim("�K�[�h_���Ⴊ��");
				}
			}
			if (sObj[1]->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK)
			{
				if (sObj[0]->GetAnim() == "���ړ�")
				{
					sObj[0]->SetAnim("�K�[�h_����");
				}

				if (sObj[0]->GetAnim() == "���Ⴊ��_���")
				{
					sObj[0]->SetAnim("�K�[�h_���Ⴊ��");
				}
			}

			// �L�����N�^�[�̏�Ԃ𑊎�ɓn��
			if (!(type[0] == type[1]))
			{
				if (type[0] == OBJ_TYPE_CHARACTER)
				{
					eState.enemyPos = sObj[0]->GetPos();
					eState.enemyAnimAttribute[0] = sObj[0]->GetAnimAttribute(0);
					eState.enemyAnimAttribute[1] = sObj[0]->GetAnimAttribute(1);
					sObj[1]->SetEnemyState(eState);
				}
				else
				{
					eState.enemyPos = sObj[1]->GetPos();
					eState.enemyAnimAttribute[0] = sObj[1]->GetAnimAttribute(0);
					eState.enemyAnimAttribute[1] = sObj[1]->GetAnimAttribute(1);
					sObj[0]->SetEnemyState(eState);
				}
			}
		}
	}

	std::string animName[2];

	for (int i = 0; i < 2; i++)
	{
		animName[i] = sObj[i]->GetAnim();

		// �����蔻���false�ɂ���
		sObj[i]->SetHitData(false, COL_TYPE_NON);
	}

	// �����蔻�菈��
	{
		ColInfo colData[2];			// �����蔻��̏����i�[����ϐ�(2�L������)

		if (lpColMng.GetColFlag(sObj[0]->GetAnim())
		   && lpColMng.GetColFlag(sObj[1]->GetAnim()))
		{
			

			// �����蔻��̏����擾
			for (int i = 0; i < 2; i++)
			{
				id[i] = sObj[i]->GetCount(sObj[i]->GetAnim());
				if (id[i] < sObj[i]->GetAnimFrame(sObj[i]->GetAnim()))
				{
					colData[i] = lpColMng.GetCollisionData("�_�l��", sObj[i]->GetAnim(), id[i]);
				}
				else
				{
					id[i] = 0;
				}
			}

			// �����蔻���POS�̔��]����
			for (int i = 0; i < 2; i++)
			{
				for (int a = 0; a < colData[i].hitBox.size(); a++)
				{

					colData[i].hitBox[a].rect.startPos.x += sObj[i]->GetAnimOffSet(animName[i]).x;
					colData[i].hitBox[a].rect.endPos.x += sObj[i]->GetAnimOffSet(animName[i]).x;
					colData[i].hitBox[a].rect.startPos.y += sObj[i]->GetAnimOffSet(animName[i]).y;
					colData[i].hitBox[a].rect.endPos.y += sObj[i]->GetAnimOffSet(animName[i]).y;

					int b;

					colData[i].hitBox[a].rect.startPos.x *= static_cast<int>(sObj[i]->GetDir()) * -2 + 1;
					colData[i].hitBox[a].rect.endPos.x *= static_cast<int>(sObj[i]->GetDir()) * -2 + 1;

					// startPos��endPos�����傫�������ꍇ�A���W����������
					if (colData[i].hitBox[a].rect.startPos.x > colData[i].hitBox[a].rect.endPos.x)
					{
						b = colData[i].hitBox[a].rect.endPos.x;
						colData[i].hitBox[a].rect.endPos.x = colData[i].hitBox[a].rect.startPos.x;
						colData[i].hitBox[a].rect.startPos.x = b;
					}
					if (colData[i].hitBox[a].rect.startPos.y > colData[i].hitBox[a].rect.endPos.y)
					{
						b = colData[i].hitBox[a].rect.endPos.y;
						colData[i].hitBox[a].rect.endPos.y = colData[i].hitBox[a].rect.startPos.y;
						colData[i].hitBox[a].rect.startPos.y = b;
					}

					// ���݂̃v���C���[��pos
					colData[i].hitBox[a].rect.startPos.x = (sObj[i]->GetPos().x + colData[i].hitBox[a].rect.startPos.x) + (sObj[i]->GetDivSize().x/2);
					colData[i].hitBox[a].rect.endPos.x = (sObj[i]->GetPos().x + colData[i].hitBox[a].rect.endPos.x) + (sObj[i]->GetDivSize().x / 2);
					colData[i].hitBox[a].rect.startPos.y =(sObj[i]->GetPos().y + colData[i].hitBox[a].rect.startPos.y) + (sObj[i]->GetDivSize().y);
					colData[i].hitBox[a].rect.endPos.y = (sObj[i]->GetPos().y + colData[i].hitBox[a].rect.endPos.y) + (sObj[i]->GetDivSize().y);

				}
			}


			// �U�����̓����蔻��
			for (int i = 0; i < 2; i++)
			{
				for (int a = 0; a < colData[i].hitBox.size(); a++)
				{
					if (a >= 1)
					{
						if (colData[i].hitBox[a - 1].type == COLTYPE_ATTACK)
						{
							break;
						}
					}

				
					
					for (int b = 0; b < colData[(i + 1) % 2].hitBox.size(); b++)
					{
						if (colData[i].hitBox[a].type == COLTYPE_ATTACK)
						{
							if (colData[(i + 1) % 2].hitBox[b].type != COLTYPE_ATTACK)
							{
								if (colData[i].hitBox[a].rect.endPos.x >= colData[(i + 1) % 2].hitBox[b].rect.startPos.x
									&& colData[i].hitBox[a].rect.startPos.x <= colData[(i + 1) % 2].hitBox[b].rect.endPos.x
									&& colData[i].hitBox[a].rect.endPos.y >= colData[(i + 1) % 2].hitBox[b].rect.startPos.y
									&& colData[i].hitBox[a].rect.startPos.y <= colData[(i + 1) % 2].hitBox[b].rect.endPos.y)
								{
									sObj[(i + 1) % 2]->SetHitData(true, colData[(i + 1) % 2].hitBox[b].type);
									
									if (colData[(i + 1) % 2].hitBox[b].type == COLTYPE_GUARD)
									{
										break;
									}
									break;
								}
								else
								{
									sObj[(i + 1) % 2]->SetHitData(false, colData[(i + 1) % 2].hitBox[b].type);
								}
							}
						}
						else
						{
							sObj[(i + 1) % 2]->SetHitData(false, colData[(i + 1) % 2].hitBox[b].type);
						}
					}
				}
			}

			for (int i = 0; i < 2; i++)
			{
				for (int a = 0; a < colData[i].hitBox.size(); a++)
				{
					// �g�����̓����蔻��
					if (eState.shotData.size())
					{
						VECTOR2 startPos = { 0,0 };
						VECTOR2 endPos = { 0,0 };

						if (colData[i].hitBox[a].type != COLTYPE_ATTACK)
						{
							for (int s = 0; s < eState.shotData.size(); s++)
							{
								startPos = { eState.shotData[s].pos.x + (shotObj->GetDivSize().x / 2) - 50,eState.shotData[s].pos.y + (shotObj->GetDivSize().y / 2) - 50 };
								endPos = { eState.shotData[s].pos.x + (shotObj->GetDivSize().x / 2) + 50,eState.shotData[s].pos.y + (shotObj->GetDivSize().y / 2) + 50 };

								if (colData[i].hitBox[a].rect.endPos.x >= startPos.x
									&& colData[i].hitBox[a].rect.startPos.x <= endPos.x
									&& colData[i].hitBox[a].rect.endPos.y >= startPos.y
									&& colData[i].hitBox[a].rect.startPos.y <= endPos.y)
								{

									sObj[i]->SetHitData(true, colData[i].hitBox[a].type);
									shotObj->SetHitData(true, colData[i].hitBox[a].type);
									break;
								}
								else
								{
									sObj[i]->SetHitData(false, colData[i].hitBox[a].type);
								}
							}
						}
					}
				}
			}
		}
	}

	for (auto& data : *objList)
	{
		data->CheckHitFlag();
	}

	

	//�`�揈��
	GameDraw();

	return std::move(own);
}

int GameScene::Init(void)
{
	MODE mode = lpSceneMng.GetMode();
	VECTOR2 ssize = lpSceneMng.GetScreenSize();

	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objList��S�폜����
	for (int a = 0; a < 2; a++)
	{
		id[a] = 0;
	}

	AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2((STICK_HUMAN_IMAGE_SIZE_X / 2), ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_1, DIR_LEFT));

	if (mode == MODE_1PLAYER)
	{
		AddObjList()(objList, std::make_unique<AIStickHuman>(VECTOR2(ssize.x - (STICK_HUMAN_IMAGE_SIZE_X / 2), ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), DIR_RIGHT));
	}
	else if (mode == MODE_2PLAYER)
	{
		AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2(ssize.x - (STICK_HUMAN_IMAGE_SIZE_X / 2), ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_2, DIR_RIGHT));
	}
	else
	{
		// MODE_MAX�����Ă���(�G���[)
		AST();
	}
	
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
