#include <stdlib.h>
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
#include "ResultScene.h"
#include "HitEffect.h"

#define STICK_HUMAN_IMAGE_SIZE_X (256)
#define STICK_HUMAN_IMAGE_SIZE_Y (256)

#define BG_IMAGE_SIZE_X (1920)
#define BG_IMAGE_SIZE_Y (720)

#define DEF_BG_POS_X (-(BG_IMAGE_SIZE_X / 6))
#define DEF_BG_POS_Y (-64)

#define DEF_CENTER_POS_X (640)

#define PI2 (3.141592654f*6)	// ���ق̉�]�p

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
	if (CheckGameEnd())
	{
		if (koDrawCount == 0)
		{
			WaitTimer(650);
			charaObj[0].charaObj->SetAnimStopFlag(true);
			charaObj[1].charaObj->SetAnimStopFlag(true);

			if (drawflag)
			{
				charaObj[0].winCount++;
				charaObj[1].winCount++;
			}
			else
			{
				charaObj[winCharacter].winCount++;
			}
		}

		koDrawCount++;
		bool leftWinFlag;
		if (charaObj[0].winCount >= 2)
		{
			leftWinFlag = true;
		}
		else
		{
			leftWinFlag = false;
		}

		if (koDrawCount >= 120)
		{
			if (gameEndFlag)
			{
				return std::make_unique<ResultScene>(leftWinFlag);
			}
			else
			{
				Init();
			}
		}
	}
	else
	{

		for (int i = 0; i < 2; i++)
		{
			beforPos[i] = charaObj[i].charaObj->GetPos();
		}

		// ���X�V
		for (auto& data : *objList)
		{
			data->UpDate(controller, objList);
		}

		auto deth_itr = std::remove_if(objList->begin(), objList->end(), [](std::shared_ptr<Obj> obj) {return obj->CheckDeth(); });
		objList->erase(deth_itr, objList->end());

		for (int i = 0; i < 2; i++)
		{
			afterPos[i] = charaObj[i].charaObj->GetPos();
		}

		ExtrusionUpdata();

		std::vector<sharedObj> shotObj;
		std::vector<sharedObj> shotObj2;
		EnemyState eState;

		for (auto& data : *objList)
		{
			if (data->CheckObjType(OBJ_TYPE_SHOT))
			{
				shotObj.push_back(data);
				shotObj2.push_back(data);
				ShotData shot(data->GetPos(), data->GetPadID());
				eState.pushBackShotData(shot);
			}
		}

		// �L�����N�^�[���m������������
		if (charaObj[0].charaObj->GetPos().x > charaObj[1].charaObj->GetPos().x)
		{
			charaObj[0].charaObj->SetDir(DIR_LEFT);
			charaObj[1].charaObj->SetDir(DIR_RIGHT);
		}
		else
		{
			charaObj[0].charaObj->SetDir(DIR_RIGHT);
			charaObj[1].charaObj->SetDir(DIR_LEFT);
		}

		// ��l���������ł�����l���U���n�̃A�j���[�V�����̎��ɁA�����������Ă�������K�[�h��Ԃɂ���
		if ((charaObj[0].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SMALL)
		 || (charaObj[0].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_BIG)
		 || (charaObj[0].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SP))
		{
			if (abs(afterPos[0].x - afterPos[1].x) <= (STICK_HUMAN_IMAGE_SIZE_X * 2))
			{
				if (charaObj[1].charaObj->GetAnim() == "���ړ�")
				{
					charaObj[1].charaObj->SetAnim("�K�[�h_����");
				}

				if (charaObj[1].charaObj->GetAnim() == "���Ⴊ��_���")
				{
					charaObj[1].charaObj->SetAnim("�K�[�h_���Ⴊ��");
				}
			}
		}
		if ((charaObj[1].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SMALL)
		 || (charaObj[1].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_BIG)
		 || (charaObj[1].charaObj->GetAnimAttribute(1) == ANIM_ATTRIBUTE_ATTACK_SP))
		{
			if (abs(afterPos[0].x - afterPos[1].x) <= (STICK_HUMAN_IMAGE_SIZE_X * 2))
			{
				if (charaObj[0].charaObj->GetAnim() == "���ړ�")
				{
					charaObj[0].charaObj->SetAnim("�K�[�h_����");
				}

				if (charaObj[0].charaObj->GetAnim() == "���Ⴊ��_���")
				{
					charaObj[0].charaObj->SetAnim("�K�[�h_���Ⴊ��");
				}
			}
		}

		// �L�����N�^�[�̏�Ԃ𑊎�ɓn��
		eState.enemyPos = charaObj[0].charaObj->GetPos();
		eState.enemyAnimAttribute[0] = charaObj[0].charaObj->GetAnimAttribute(0);
		eState.enemyAnimAttribute[1] = charaObj[0].charaObj->GetAnimAttribute(1);
		eState.enemyAnimAttribute[2] = charaObj[0].charaObj->GetAnimAttribute(2);
		eState.enemyAnim = charaObj[0].charaObj->GetAnim();
		charaObj[1].charaObj->SetEnemyState(eState);

		eState.enemyPos = charaObj[1].charaObj->GetPos();
		eState.enemyAnimAttribute[0] = charaObj[1].charaObj->GetAnimAttribute(0);
		eState.enemyAnimAttribute[1] = charaObj[1].charaObj->GetAnimAttribute(1);
		eState.enemyAnimAttribute[2] = charaObj[1].charaObj->GetAnimAttribute(2);
		eState.enemyAnim = charaObj[1].charaObj->GetAnim();
		charaObj[0].charaObj->SetEnemyState(eState);

		std::string animName[2];

		for (int i = 0; i < 2; i++)
		{
			animName[i] = charaObj[i].charaObj->GetAnim();

			charaObj[i].AttackHitOld = charaObj[i].charaObj->GetHitFlag();
			// �����蔻���false�ɂ���
			charaObj[i].charaObj->SetHitData(false, COLTYPE_NON);
			damageFlag[i] = false;
		}

		// �����蔻�菈��
		{
			

			if (lpColMng.GetColFlag(charaObj[0].charaObj->GetAnim())
				&& lpColMng.GetColFlag(charaObj[1].charaObj->GetAnim()))
			{
				// �����蔻��̏����擾
				for (int i = 0; i < 2; i++)
				{
					id[i] = charaObj[i].charaObj->GetFrame();
					if (id[i] < charaObj[i].charaObj->GetAnimFrame(charaObj[i].charaObj->GetAnim()))
					{
						colData[i] = lpColMng.GetCollisionData("�_�l��", charaObj[i].charaObj->GetAnim(), id[i]);
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

						colData[i].hitBox[a].rect.startPos.x += charaObj[i].charaObj->GetAnimOffSet(animName[i]).x;
						colData[i].hitBox[a].rect.endPos.x += charaObj[i].charaObj->GetAnimOffSet(animName[i]).x;
						colData[i].hitBox[a].rect.startPos.y += charaObj[i].charaObj->GetAnimOffSet(animName[i]).y;
						colData[i].hitBox[a].rect.endPos.y += charaObj[i].charaObj->GetAnimOffSet(animName[i]).y;

						int oneTimePos;

						colData[i].hitBox[a].rect.startPos.x *= static_cast<int>(charaObj[i].charaObj->GetDir()) * -2 + 1;
						colData[i].hitBox[a].rect.endPos.x *= static_cast<int>(charaObj[i].charaObj->GetDir()) * -2 + 1;

						// startPos��endPos�����傫�������ꍇ�A���W����������
						if (colData[i].hitBox[a].rect.startPos.x > colData[i].hitBox[a].rect.endPos.x)
						{
							oneTimePos = colData[i].hitBox[a].rect.endPos.x;
							colData[i].hitBox[a].rect.endPos.x = colData[i].hitBox[a].rect.startPos.x;
							colData[i].hitBox[a].rect.startPos.x = oneTimePos;
						}
						if (colData[i].hitBox[a].rect.startPos.y > colData[i].hitBox[a].rect.endPos.y)
						{
							oneTimePos = colData[i].hitBox[a].rect.endPos.y;
							colData[i].hitBox[a].rect.endPos.y = colData[i].hitBox[a].rect.startPos.y;
							colData[i].hitBox[a].rect.startPos.y = oneTimePos;
						}

						// ���݂̃v���C���[��pos
						colData[i].hitBox[a].rect.startPos.x = (charaObj[i].charaObj->GetPos().x + colData[i].hitBox[a].rect.startPos.x) + (charaObj[i].charaObj->GetDivSize().x / 2);
						colData[i].hitBox[a].rect.endPos.x = (charaObj[i].charaObj->GetPos().x + colData[i].hitBox[a].rect.endPos.x) + (charaObj[i].charaObj->GetDivSize().x / 2);
						colData[i].hitBox[a].rect.startPos.y = (charaObj[i].charaObj->GetPos().y + colData[i].hitBox[a].rect.startPos.y) + (charaObj[i].charaObj->GetDivSize().y);
						colData[i].hitBox[a].rect.endPos.y = (charaObj[i].charaObj->GetPos().y + colData[i].hitBox[a].rect.endPos.y) + (charaObj[i].charaObj->GetDivSize().y);

					}
				}

				VECTOR2 hitRectPos;

				// �U�����̓����蔻��
				for (int i = 0; i < 2; i++)
				{
					for (int a = 0; a < colData[i].hitBox.size(); a++)
					{
						
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
										if (charaObj[i].charaObj->GetHitBoxType() != COLTYPE_HIT)
										{
											charaObj[i].charaObj->SetHitData(true, colData[i].hitBox[a].type);
										}
										charaObj[(i + 1) % 2].charaObj->SetHitData(true, colData[(i + 1) % 2].hitBox[b].type);

										if (colData[(i + 1) % 2].hitBox[b].type == COLTYPE_GUARD)
										{
											break;
										}

										hitRectPos = colData[1 - i].hitBox[b].rect.startPos;

										//break;
									}
								}
							}
						}
					}
					if ((charaObj[(i + 1) % 2].charaObj->GetHitFlag()) && (charaObj[(i + 1) % 2].charaObj->GetHitBoxType() == COLTYPE_HIT) && !damageFlag[(i + 1) % 2])
					{
						charaObj[(i + 1) % 2].charaObj->CheckDamage(charaObj[i].charaObj->GetAnimAttribute(1));
						damageFlag[(i + 1) % 2] = true;
					}

					if (!hitRectPos && !charaObj[i].AttackHitOld && charaObj[i].charaObj->GetAnimAttribute(2) != ANIM_ATTRIBUTE_INVINCIBLE)
					{
						// �q�b�g�G�t�F�N�g�\��
						AddObjList()(objList, std::make_shared<HitEffect>(hitRectPos, VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64)));
					}
				}
		
				for (int shot1 = 0; shot1 < shotObj.size(); shot1++)
				{
					if (shotObj[shot1]->GetHitFlag())
					{
						break;
					}
					for (int shot2 = 0; shot2 < shotObj2.size(); shot2++)
					{

						if (shotObj[shot2]->GetHitFlag())
						{
							break;
						}
						shotObj[shot1]->SetHitData(false, COLTYPE_NON);
						shotObj[shot2]->SetHitData(false, COLTYPE_NON);

						if (shotObj[shot1] != shotObj2[shot2])
						{
							if (shotObj[shot1]->GetPos().x + (shotObj[shot1]->GetDivSize().x / 2) + 50 >= shotObj[shot2]->GetPos().x + (shotObj[shot2]->GetDivSize().x / 2) - 50
								&& shotObj[shot1]->GetPos().x + (shotObj[shot1]->GetDivSize().x / 2) - 50 <= shotObj[shot2]->GetPos().x + (shotObj[shot2]->GetDivSize().x / 2) + 50)
							{
								shotObj[shot1]->SetHitData(true, COLTYPE_NON);
								shotObj2[shot2]->SetHitData(true, COLTYPE_NON);
							}
						}
					}
				}

				for (int i = 0; i < 2; i++)
				{
					for (int a = 0; a < colData[i].hitBox.size(); a++)
					{
						// �g�����̓����蔻��
						for (auto& data : *objList)
						{
							if (data->CheckObjType(OBJ_TYPE_SHOT))
							{
								if (data->GetHitFlag())
								{
									break;
								}
								VECTOR2 startPos = { 0,0 };
								VECTOR2 endPos = { 0,0 };

								if (colData[i].hitBox[a].type != COLTYPE_ATTACK)
								{
									startPos = { data->GetPos().x + (data->GetDivSize().x / 2) - 50,data->GetPos().y + (data->GetDivSize().y / 2) - 50 };
									endPos = { data->GetPos().x + (data->GetDivSize().x / 2) + 50,data->GetPos().y + (data->GetDivSize().y / 2) + 50 };

									if (colData[i].hitBox[a].rect.endPos.x >= startPos.x
										&& colData[i].hitBox[a].rect.startPos.x <= endPos.x
										&& colData[i].hitBox[a].rect.endPos.y >= startPos.y
										&& colData[i].hitBox[a].rect.startPos.y <= endPos.y)
									{
										charaObj[i].charaObj->SetHitData(true, colData[i].hitBox[a].type);
										data->SetHitData(true, colData[i].hitBox[a].type);

										break;
									}
								}
							}
						}
					}
					if ((charaObj[i].charaObj->GetHitFlag()) && (charaObj[i].charaObj->GetHitBoxType() == COLTYPE_HIT) && !damageFlag[i])
					{
						charaObj[i].charaObj->CheckDamage(ANIM_ATTRIBUTE_SHOT);
						damageFlag[i] = true;
					}
				}

			}
		}

		for (auto& data : *objList)
		{
			data->CheckHitFlag();
		}

		deth_itr = std::remove_if(objList->begin(), objList->end(), [](std::shared_ptr<Obj> obj) {return obj->CheckDeth(); });
		objList->erase(deth_itr, objList->end());

		// �w�i�̈ʒu���X�V
		BgPosUpDate();

		for (auto& data : *objList)
		{
			if (data->CheckObjType(OBJ_TYPE_SHOT))
			{
				data->AddPos(bgPos - bgPosOld);
			}
		}
	}
	
	maskCnt++;

	// �`�揈��
	GameDraw();

	return std::move(own);
}

int GameScene::Init(void)
{
	MODE mode = lpSceneMng.GetMode();
	ssize = lpSceneMng.GetScreenSize();
	bgPos = VECTOR2(DEF_BG_POS_X, DEF_BG_POS_Y);
	hitStopFlag = false;
	gameEndFlag = false;
	loseCharacter = -1;
	winCharacter = -1;
	flashCnt = 0;
	drawflag = false;
	koDrawCount = 0;
	charaObj[0].AttackHitOld = false;
	charaObj[1].AttackHitOld = false;

	if (!objList)
	{
		objList = std::make_shared<sharedObjList>();
	}
	objList->clear();		// objList��S�폜����
	for (int a = 0; a < 2; a++)
	{
		id[a] = 0;
	}

	charaObj[0].charaObj = *AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2((ssize.x / 4), ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_1, DIR_LEFT));
	charaObj[0].charaType = OBJ_TYPE_CHARACTER;

	if (mode == MODE_1PLAYER)
	{
		charaObj[1].charaObj = *AddObjList()(objList, std::make_unique<AIStickHuman>(VECTOR2(ssize.x * 3 / 4, ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), DIR_RIGHT));
		charaObj[1].charaType = OBJ_TYPE_AICHARACTER;
	}
	else if (mode == MODE_2PLAYER)
	{
		charaObj[1].charaObj = *AddObjList()(objList, std::make_unique<StickHuman>(VECTOR2(ssize.x * 3 / 4, ssize.y), VECTOR2(-(STICK_HUMAN_IMAGE_SIZE_X / 2), -STICK_HUMAN_IMAGE_SIZE_Y - 64), PAD_2, DIR_RIGHT));
		charaObj[1].charaType = OBJ_TYPE_CHARACTER;
	}
	else
	{
		// MODE_MAX�����Ă���(�G���[)
		AST();
	}
	
	maskCnt = 0;
	smallStarMask = LoadMask("image/�Q�[���V�[���p/winStar_mask1.png");

	return 0;
}

void GameScene::BgPosUpDate(void)
{
	// ��׸���̈ʒu���i�[
	VECTOR2 characterPos[2] = { charaObj[0].charaObj->GetPos(), charaObj[1].charaObj->GetPos() };

	// 1�ڰёO�̔w�i�̈ʒu���W�̊i�[
	bgPosOld = bgPos;

	// �����̏���
	bgPos.x += (DEF_CENTER_POS_X - ((characterPos[0].x + characterPos[1].x) / 2));
	if (bgPos.x > 0)
	{
		bgPos.x = 0;
	}
	if (bgPos.x < (ssize.x - BG_IMAGE_SIZE_X))
	{
		bgPos.x = (ssize.x - BG_IMAGE_SIZE_X);
	}

	int leftCharacter;		// ���ɂ������characterPos��index
	int rightCharacter;		// �E�ɂ������characterPos��index

	if (characterPos[0].x < characterPos[1].x)
	{
		leftCharacter = 0;
		rightCharacter = 1;
	}
	else
	{
		leftCharacter = 1;
		rightCharacter = 0;
	}

	int characterDistanceHalf = ((characterPos[rightCharacter].x - characterPos[leftCharacter].x) / 2);

	if ((bgPos.x != 0) && (bgPos.x != (ssize.x - BG_IMAGE_SIZE_X)))
	{
		characterPos[leftCharacter].x = DEF_CENTER_POS_X - characterDistanceHalf;
		characterPos[rightCharacter].x = DEF_CENTER_POS_X + characterDistanceHalf;
	}

	

	// �����̏���
	int highCharacter;		// ��ɂ������characterPos��index
	int lowCharacter;		// ���ɂ������characterPos��index
	if (characterPos[0].y < characterPos[1].y)
	{
		highCharacter = 0;
		lowCharacter = 1;
	}
	else
	{
		highCharacter = 1;
		lowCharacter = 0;
	}
	
	bgPos.y = (DEF_BG_POS_Y + ((ssize.y - characterPos[highCharacter].y) / 4));
	if (bgPos.y > 0)
	{
		bgPos.y = 0;
	}
	if (bgPos.y < DEF_BG_POS_Y)
	{
		bgPos.y = DEF_BG_POS_Y;
	}

	if (bgPosOld.y != bgPos.y)
	{
		characterPos[highCharacter].y += (bgPos.y - bgPosOld.y);
		characterPos[lowCharacter].y += (bgPos.y - bgPosOld.y);
	}

	// �͈͊O�̏���
	int edgePosLeft;		// �ړ������̍���
	int edgePosRight;		// �ړ������̉E��

	edgePosLeft = (ssize.x / 5) + bgPos.x;
	if (edgePosLeft < (STICK_HUMAN_IMAGE_SIZE_X / 2))
	{
		edgePosLeft = (STICK_HUMAN_IMAGE_SIZE_X / 2);
	}
	if (edgePosLeft > (ssize.x / 5))
	{
		edgePosLeft = (ssize.x / 5);
	}

	edgePosRight = (((ssize.x * 4) / 5) + bgPos.x - (ssize.x - BG_IMAGE_SIZE_X));
	if (edgePosRight < ((ssize.x * 4) / 5))
	{
		edgePosRight = ((ssize.x * 4) / 5);
	}
	if (edgePosRight > (ssize.x - (STICK_HUMAN_IMAGE_SIZE_X / 2)))
	{
		edgePosRight = (ssize.x - (STICK_HUMAN_IMAGE_SIZE_X / 2));
	}

	int edgePosDown;		// �ړ������̉���

	edgePosDown = ssize.y + (bgPos.y - DEF_BG_POS_Y);
	if (edgePosDown < ssize.y)
	{
		edgePosDown = ssize.y;
	}
	if (edgePosDown > (ssize.y - DEF_BG_POS_Y))
	{
		edgePosDown = (ssize.y - DEF_BG_POS_Y);
	}

	if (characterPos[leftCharacter].x < edgePosLeft)
	{
		if ((characterPos[leftCharacter].y > edgePosDown) || (charaObj[leftCharacter].charaObj->GetAnimAttribute(0) != ANIM_ATTRIBUTE_AIR))
		{
			charaObj[leftCharacter].charaObj->SetPos(VECTOR2(edgePosLeft, edgePosDown));
		}
		else
		{
			charaObj[leftCharacter].charaObj->SetPos(VECTOR2(edgePosLeft, characterPos[leftCharacter].y));
		}
	}
	else
	{
		if ((characterPos[leftCharacter].y > edgePosDown) || (charaObj[leftCharacter].charaObj->GetAnimAttribute(0) != ANIM_ATTRIBUTE_AIR))
		{
			charaObj[leftCharacter].charaObj->SetPos(VECTOR2(characterPos[leftCharacter].x, edgePosDown));
		}
		else
		{
			charaObj[leftCharacter].charaObj->SetPos(VECTOR2(characterPos[leftCharacter].x, characterPos[leftCharacter].y));
		}
	}

	if (characterPos[rightCharacter].x > edgePosRight)
	{
		if ((characterPos[rightCharacter].y > edgePosDown) || (charaObj[rightCharacter].charaObj->GetAnimAttribute(0) != ANIM_ATTRIBUTE_AIR))
		{
			charaObj[rightCharacter].charaObj->SetPos(VECTOR2(edgePosRight, edgePosDown));
		}
		else
		{
			charaObj[rightCharacter].charaObj->SetPos(VECTOR2(edgePosRight, characterPos[rightCharacter].y));
		}
	}
	else
	{
		if ((characterPos[rightCharacter].y > edgePosDown) || (charaObj[rightCharacter].charaObj->GetAnimAttribute(0) != ANIM_ATTRIBUTE_AIR))
		{
			charaObj[rightCharacter].charaObj->SetPos(VECTOR2(characterPos[rightCharacter].x, edgePosDown));
		}
		else
		{
			charaObj[rightCharacter].charaObj->SetPos(VECTOR2(characterPos[rightCharacter].x, characterPos[rightCharacter].y));
		}
	}
}

void GameScene::ExtrusionUpdata()
{
	int leftCharacter;		// ���ɂ������characterPos��index
	int rightCharacter;		// �E�ɂ������characterPos��index

	if (afterPos[0].x < afterPos[1].x)
	{
		leftCharacter = 0;
		rightCharacter = 1;
	}
	else
	{
		leftCharacter = 1;
		rightCharacter = 0;
	}

	if ((afterPos[leftCharacter].x + (STICK_HUMAN_IMAGE_SIZE_X / 8)) > (afterPos[rightCharacter].x - (STICK_HUMAN_IMAGE_SIZE_X / 8)))
	{
		int highCharacter;		// ��ɂ������characterPos��index
		int lowCharacter;		// ���ɂ������characterPos��index

		if (afterPos[0].y < afterPos[1].y)
		{
			highCharacter = 0;
			lowCharacter = 1;
		}
		else
		{
			highCharacter = 1;
			lowCharacter = 0;
		}

		if (afterPos[highCharacter].y > (afterPos[lowCharacter].y - (STICK_HUMAN_IMAGE_SIZE_Y * 3 / 5)))
		{
			// �����܂ŗ����牟���o��
			int moveSum = (afterPos[leftCharacter].x - beforPos[leftCharacter].x) + (afterPos[rightCharacter].x - beforPos[rightCharacter].x);
			int addPosX = (afterPos[leftCharacter].x + (STICK_HUMAN_IMAGE_SIZE_X / 8)) - (afterPos[rightCharacter].x - (STICK_HUMAN_IMAGE_SIZE_X / 8));

			if (moveSum > 0)
			{
				charaObj[rightCharacter].charaObj->AddPos({ addPosX, 0 });

				if (charaObj[rightCharacter].charaObj->GetPos().x > (ssize.x * 4 / 5))
				{
					charaObj[rightCharacter].charaObj->SetPos({ (ssize.x * 4 / 5), afterPos[rightCharacter].y });
					charaObj[leftCharacter].charaObj->SetPos({ (ssize.x * 4 / 5) - (STICK_HUMAN_IMAGE_SIZE_X / 4), afterPos[leftCharacter].y });
				}
			}
			else if (moveSum < 0)
			{
				charaObj[leftCharacter].charaObj->AddPos({ -addPosX, 0 });

				if (charaObj[leftCharacter].charaObj->GetPos().x < (ssize.x / 5))
				{
					charaObj[leftCharacter].charaObj->SetPos({ (ssize.x / 5), afterPos[leftCharacter].y });
					charaObj[rightCharacter].charaObj->SetPos({ (ssize.x / 5) + (STICK_HUMAN_IMAGE_SIZE_X / 4), afterPos[rightCharacter].y });
				}
			}
			else
			{
				addPosX /= 2;

				charaObj[rightCharacter].charaObj->AddPos({ addPosX, 0 });
				charaObj[leftCharacter].charaObj->AddPos({ -addPosX, 0 });
			}
		}
	}
}

bool GameScene::CheckGameEnd()
{
	if (charaObj[0].winCount >= 2 || charaObj[1].winCount >= 2)
	{
		gameEndFlag = true;
		return true;
	}
	else
	{
		if (hitStopFlag)
		{
			if (drawflag)
			{
				if (charaObj[0].charaObj->GetAnim() == "�_���[�W_�_�E��" && charaObj[0].charaObj->GetAnimEndFlag())
				{
					return true;
				}
			}
			else
			{
				if (charaObj[loseCharacter].charaObj->GetAnim() == "�_���[�W_�_�E��" && charaObj[loseCharacter].charaObj->GetAnimEndFlag())
				{
					return true;
				}
			}
			
			WaitTimer(50);
		}
		else
		{
			for (int i = 0; i < 2; ++i)
			{
				if (charaObj[i].charaObj->GetPlayerHP() == 0)
				{
					if (winCharacter == i)
					{
						// ��������
						drawflag = true;
					}
					else
					{
						loseCharacter = i;
						winCharacter = (1 - i);
					}

					hitStopFlag = true;
				}
			}
		}
	}

	return false;
}

bool GameScene::GameDraw(void)
{
	DrawGraph(bgPos.x, bgPos.y, IMAGE_ID("image/�Q�[���V�[���p/bluesky_�w�i.png")[0], true);

	DrawGraph((ssize.x/2)-175, 0, IMAGE_ID("image/�Q�[���V�[���p/centerBer.png")[0], true);

	//objList�ɓo�^����Ă���׽�̕`�揈�����s��
	for (auto &data : (*objList))
	{
		(*data).Draw();
	}
	
	std::string animName[2];

	for (int charNum = 0; charNum < 2; charNum++)
	{
		animName[charNum] = charaObj[charNum].charaObj->GetAnim();
		if (lpColMng.GetColFlag(charaObj[charNum].charaObj->GetAnim()))
		{
			int colColor;
			for (int i = 0; i < colData[charNum].hitBox.size(); i++)
			{

				colColor = (colData[charNum].hitBox[i].type == COLTYPE_ATTACK ? 0xff0000 : (colData[charNum].hitBox[i].type == COLTYPE_HIT ? 0x0000ff : 0x00ff00));

				DrawBox(charaObj[charNum].charaObj->GetDrawOffSet().x + colData[charNum].hitBox[i].rect.startPos.x, charaObj[charNum].charaObj->GetDrawOffSet().y + colData[charNum].hitBox[i].rect.startPos.y,
						charaObj[charNum].charaObj->GetDrawOffSet().x + colData[charNum].hitBox[i].rect.endPos.x, charaObj[charNum].charaObj->GetDrawOffSet().y + colData[charNum].hitBox[i].rect.endPos.y, colColor, false);
			}
		}
	}

	// KO�̕����̕`��
	if (koDrawCount)
	{
		// �ݸ����ڲ��
		if (lpSceneMng.GetMode() == MODE_1PLAYER && winCharacter == 0)
		{
			DrawGraph((ssize.x / 2) - 100, 200, IMAGE_DIV_ID("image/�Q�[���V�[���p/singleResult.png", VECTOR2(200, 65), VECTOR2(PLAYER_CNT_MAX, 1))[0], true);		// �����摜��`��
		}
		if (lpSceneMng.GetMode() == MODE_1PLAYER && winCharacter != 0)
		{
			DrawGraph((ssize.x / 2) - 100, 200, IMAGE_DIV_ID("image/�Q�[���V�[���p/singleResult.png", VECTOR2(200, 65), VECTOR2(PLAYER_CNT_MAX, 1))[1], true);		// �����摜��`��
		}
		// �����ڲ��
		if (lpSceneMng.GetMode() == MODE_2PLAYER && winCharacter == 0)
		{
			DrawGraph((ssize.x / 2) - 100, 200, IMAGE_DIV_ID("image/�Q�[���V�[���p/multiResult.png", VECTOR2(200, 65), VECTOR2(PLAYER_CNT_MAX, 1))[0], true);		// �����摜��`��
		}
		if (lpSceneMng.GetMode() == MODE_2PLAYER && winCharacter != 0)
		{
			DrawGraph((ssize.x / 2) - 100, 200, IMAGE_DIV_ID("image/�Q�[���V�[���p/multiResult.png", VECTOR2(200, 65), VECTOR2(PLAYER_CNT_MAX, 1))[1], true);		// �����摜��`��
		}

	}

	/*DrawFormatString(0, 300, 0xff0000, "chara 0 : winCount %d\nchara 1 : winCount %d\n", charaObj[0].winCount, charaObj[1].winCount);*/
	flashCnt++;
	if (charaObj[0].winCount == 1)
	{
		CreateMaskScreen();     // Ͻ��J�n
		DrawMask( ((ssize.x / 2) - 175 + 70 - 15), (69 - 15), smallStarMask, DX_MASKTRANS_BLACK); // ���F�̏ꏊ�����`��
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);               // ���Z�u�����h�ɐݒ�
		DrawRotaGraph2( ((ssize.x / 2) - 175 + 70 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * maskCnt, IMAGE_ID("image/�Q�[���V�[���p/starBack1.png")[0], true);			
		DrawRotaGraph2( ((ssize.x / 2) - 175 + 70 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 120), IMAGE_ID("image/�Q�[���V�[���p/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 70 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 60), IMAGE_ID("image/�Q�[���V�[���p/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 70 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 180), IMAGE_ID("image/�Q�[���V�[���p/starBack1.png")[0], true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);     // �����ނ�ؾ��
		DeleteMaskScreen();		// Ͻ��I��

		DrawGraph((ssize.x / 2) - 175 +70, 69, IMAGE_ID("image/�Q�[���V�[���p/winStar1.png")[0], true);
	}
	if (charaObj[0].winCount == 2)
	{
		if (((flashCnt / 4) % 2) == 0)
		{
			DrawGraph(((ssize.x / 2) - 175 + 70 - 15), (69 - 15), IMAGE_ID("image/�Q�[���V�[���p/winStar_flash1.png")[0], true);		// �����_�ł̕`��
			DrawGraph(((ssize.x / 2) - 175 + 20 - 20), (10 - 20), IMAGE_ID("image/�Q�[���V�[���p/winStar_flash2.png")[0], true);
		}
		DrawGraph((ssize.x / 2) - 175 +70, 69, IMAGE_ID("image/�Q�[���V�[���p/winStar1.png")[0], true);
		DrawGraph((ssize.x / 2) - 175 +20, 10, IMAGE_ID("image/�Q�[���V�[���p/winStar2.png")[0], true);
	}

	if (charaObj[1].winCount == 1)
	{
		CreateMaskScreen();     // Ͻ��J�n
		DrawMask(((ssize.x / 2) - 175 + 232 - 15), (69 - 15), smallStarMask, DX_MASKTRANS_BLACK); // ���F�̏ꏊ�����`��
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);               // ���Z�u�����h�ɐݒ�
		DrawRotaGraph2(((ssize.x / 2) - 175 + 232 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * maskCnt, IMAGE_ID("image/�Q�[���V�[���p/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 232 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 120), IMAGE_ID("image/�Q�[���V�[���p/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 232 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 60), IMAGE_ID("image/�Q�[���V�[���p/starBack1.png")[0], true);
		DrawRotaGraph2(((ssize.x / 2) - 175 + 232 + 25), (69 + 25), 0, 9, 1.0, PI2 / 240 * (maskCnt - 180), IMAGE_ID("image/�Q�[���V�[���p/starBack1.png")[0], true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);     // �����ނ�ؾ��
		DeleteMaskScreen();		// Ͻ��I��

		DrawGraph((ssize.x / 2) - 175 + 232, 69, IMAGE_ID("image/�Q�[���V�[���p/winStar1.png")[0], true);
	}
	if (charaObj[1].winCount == 2)
	{
		if (((flashCnt / 4) % 2) == 0)
		{
			DrawGraph(((ssize.x / 2) - 175 + 232 - 15), (69 - 15), IMAGE_ID("image/�Q�[���V�[���p/winStar_flash1.png")[0], true);		// �����_�ł̕`��
			DrawGraph(((ssize.x / 2) - 175 + 262 - 20), (10 - 20), IMAGE_ID("image/�Q�[���V�[���p/winStar_flash2.png")[0], true);
		}
		DrawGraph((ssize.x / 2) - 175 + 232, 69, IMAGE_ID("image/�Q�[���V�[���p/winStar1.png")[0], true);
		DrawGraph((ssize.x / 2) - 175 + 262, 10, IMAGE_ID("image/�Q�[���V�[���p/winStar2.png")[0], true);
	}



	return true;
}
