#include <DxLib.h>
#include <string>
#include "CollisionMng.h"



bool CollisionMng::ColLoad(std::string pasName, std::string animName)
{
	

	// �t�@�C���p�X�쐬
	std::string loadPass = pasName + "\\" + animName + ".map";		//�ǂݍ��ݐ�̃t�@�C���p�X���쐬
	
	// hitBoxNum��animNum��resize
	header.hitBoxNum.resize(header.animNum);
	
	//data.resize(header.animNum);

	HitBox oneTimeData;

	FILE* file;

	fopen_s(&file, loadPass.c_str(), "rb");

	if (!file)
	{
		return false;
	}

	

	fread(&header.animNum, sizeof(int), 1, file);

	fread(&header.hitBoxNum[0], sizeof(int)*header.hitBoxNum.size(), 1, file);




	for (int i = 0; i < data.size(); ++i)
	{
		data[i].hitBox.resize(header.hitBoxNum[i]);

		for (int a = 0; a < data[i].hitBox.size(); a++)
		{
		

			fread(&oneTimeData, sizeof(oneTimeData), 1, file);

			data[i].hitBox[a] = oneTimeData;
		}
	}


	

	fclose(file);

	loadFlag = false;


	return true;
}

void CollisionMng::Update(void)
{
	inputData.Update();

	auto mousePos = inputData.MousePos();

	if (inputFlag)
	{
		if (inputData.MouseStateDown(MOUSE_INPUT_LEFT))
		{
			colData.rect.startPos = inputData.MousePos();
		}


		if (inputData.MouseStateUp(MOUSE_INPUT_LEFT))
		{
			colData.rect.endPos = inputData.MousePos();
		}	

		
		

	/*	if (!(colData.rect.startPos.x == -1 && colData.rect.startPos.y == -1))
		{
			
			DrawBox(colData.rect.startPos.x, colData.rect.startPos.y, mousePos.x, mousePos.y, 0xffffff, false);
		}*/
	}
	
	if ((!loadFlag)&&(!saveFlag))
	{
		if (GetDragFileNum() > 0)
		{
			char filePath[MAX_PATH];

			GetDragFilePath(filePath);

			std::string f_name;

			frameNum = 0;



			bool findFlag = true;

			do
			{

				f_name = std::string(filePath) + "\\" + std::to_string(frameNum) + ".png";

				animFram[frameNum] = LoadGraph(f_name.c_str());		// tmpHandle��̧�ٖ�f_name�̉摜������ق���

				if (animFram[frameNum] == -1)
				{
					findFlag = false;		// ������Ȃ�����
				}
				else
				{

					frameNum++;							// num��i�߂Ď��̉摜��ǂݍ��ޏ���
				}

			} while (findFlag);		// findFlag��true�̎��͂܂��悪���邩������Ȃ�����J��Ԃ��Afalse�Ȃ炻�����͂Ȃ��͂������甲����

			// ��̃A�j���[�V�����̉摜����
			header.animNum = frameNum;
			frameNum = 0;
			data.resize(header.animNum);
			//texHandle = LoadGraph(f_name.c_str());

			// �ȉ��̏����Ńp�X����L�������ƃA�j���[�V�������𒊏o

			animName = filePath;

			auto findNum = animName.rfind("\\");

			animName = animName.substr(findNum + 1);

			//charName = charName.substr(findNum - 6, 6);

			// �����蔻��ǂݍ���
			//ColLoad(charName, animName);



			//auto i = imagePath.find("\\");
			//std::string replace = "/";
			//imagePath.replace(i, 1, replace);

			inputFlag = true;

			//DrawString(100, 100, charName.c_str(), 0xffffff);
		}
	}

	

	if (inputData.KeyStateDown(KEY_INPUT_RIGHT))
	{
		if (header.animNum - 1 > frameNum)
		{
			frameNum++;
			
		}
	}
	else if(inputData.KeyStateDown(KEY_INPUT_LEFT))
	{
		if (0 < frameNum)
		{
			frameNum--;
			
		}
	}
	else
	{
	}
	
	// Z�L�[�œ����蔻��������	
	if (inputData.KeyStateDown(KEY_INPUT_Z))
	{
		if (!(data[frameNum].hitBox.size() == 0))
		{
			data[frameNum].hitBox.pop_back();
		}
	}

	// �E�N���b�N�ōU���ƃv���C���[�̐؂�ւ�
	if (inputData.MouseStateDown(MOUSE_INPUT_RIGHT))
	{
		colTypeCnt++;
	}

	colTypeCnt = colTypeCnt % COLTYPE_MAX;

	if (colTypeCnt == COLTYPE_ATTACK)
	{
		colData.type = COLTYPE_ATTACK;
	}
	else if (colTypeCnt == COLTYPE_HIT)
	{
		colData.type = COLTYPE_HIT;
	}
	else if(colTypeCnt == COLTYPE_GUARD)
	{
		colData.type = COLTYPE_GUARD;
	}
	else
	{
	}


	// A�L�[�ň�̓����蔻��ۑ�
	if (inputData.KeyStateDown(KEY_INPUT_A))
	{
		colData.rect.endPos = mousePos;

		colData.rect.startPos -= VECTOR2(IMAGE_OFFSET_X+ (256 / 2), IMAGE_OFFSET_Y + 256);
		colData.rect.endPos -= VECTOR2(IMAGE_OFFSET_X + (256 / 2), IMAGE_OFFSET_Y + 256);

		data[frameNum].hitBox.push_back(colData);
	}

	if (inputData.KeyStateDown(KEY_INPUT_F6))
	{
		//ColSave();

		int tmp = MessageBox(
			NULL,								// �I�[�i�[�E�B���h�E�̃n���h��
			"���݂̓o�^�f�[�^��ۑ����܂����H",     // ���b�Z�[�W�{�b�N�X���̃e�L�X�g
			"�m�F�_�C�A���O",					// ���b�Z�[�W�{�b�N�X�̃^�C�g��
			MB_OKCANCEL						    // ���b�Z�[�W�{�b�N�X�̃X�^�C��
		);

		if (tmp == IDOK)
		{
			saveFlag = true;
		}
	}

	if (saveFlag)
	{
		char filePath[MAX_PATH];

		std::string f_name;

		if (GetDragFileNum() > 0)
		{
			GetDragFilePath(filePath);

			f_name = std::string(filePath);

			ColSave(f_name, animName);
		}
		else
		{
			DrawString(200, IMAGE_OFFSET_Y, "col�t�@�C�����h���b�O&�h���b�v���Ă�������", 0xffffff);

		}
	}

	if (inputData.KeyStateDown(KEY_INPUT_F5))
	{
		int tmp = MessageBox(
			NULL,								// �I�[�i�[�E�B���h�E�̃n���h��
			"�ۑ�����Ă���f�[�^�����[�h���܂����H",     // ���b�Z�[�W�{�b�N�X���̃e�L�X�g
			"�m�F�_�C�A���O",					// ���b�Z�[�W�{�b�N�X�̃^�C�g��
			MB_OKCANCEL						    // ���b�Z�[�W�{�b�N�X�̃X�^�C��
		);

		if (tmp == IDOK)
		{
			loadFlag = true;
		}
	}

	if (loadFlag)
	{
		char filePath[MAX_PATH];

		std::string f_name;


		if (GetDragFileNum() > 0)
		{
			GetDragFilePath(filePath);

			f_name = std::string(filePath);

			ColLoad(f_name, animName);
		}
		else
		{
			DrawString(200, IMAGE_OFFSET_Y, "col�t�@�C�����h���b�O&�h���b�v���Ă�������", 0xffffff);

		}
	}

}

void CollisionMng::Draw(void)
{
	VECTOR2 offset = VECTOR2(IMAGE_OFFSET_X + (256 / 2), IMAGE_OFFSET_Y + 256);

	int colColor = (colData.type == COLTYPE_ATTACK ? 0xff0000 : (colData.type == COLTYPE_HIT ? 0x87CEEB:0x00ff00));

	auto mousePos = inputData.MousePos();

	if (inputFlag)
	{
		if (animFram[frameNum] != -1)
		{
			DrawGraph(IMAGE_OFFSET_X, IMAGE_OFFSET_Y, animFram[frameNum], true);
		}
	}
	else
	{
		DrawString(200, IMAGE_OFFSET_Y, "�摜�t�@�C�����h���b�O&�h���b�v���Ă�������", 0xffffff);
	}
	
	// ���݂�
	if (!(colData.rect.startPos.x == -1 && colData.rect.startPos.y == -1))
	{
		DrawBox(colData.rect.startPos.x, colData.rect.startPos.y, mousePos.x, mousePos.y, colColor, false);
	}

	if (data.size())
	{
		for (auto col = data[frameNum].hitBox.begin(); col != data[frameNum].hitBox.end(); col++)
		{
			if (data[frameNum].hitBox.size() == 0)
			{
				break;
			}

			int tmpColor = ((*col).type == COLTYPE_ATTACK ? 0xff0000 : ((*col).type == COLTYPE_HIT ? 0x87CEEB : 0x00ff00));

			DrawBox((*col).rect.startPos.x + offset.x, (*col).rect.startPos.y + offset.y, (*col).rect.endPos.x + offset.x, (*col).rect.endPos.y + offset.y, tmpColor, false);
		}
	}
	

	
}

CollisionMng::CollisionMng()
{
	Init();
}


CollisionMng::~CollisionMng()
{
}

void CollisionMng::ColSave(std::string pathName, std::string animName)
{


	if (!data.size())
	{
		return;
	}

	DataHeader expData = {
		static_cast<int>(data.size()),
	};

	

	std::string SavePass = pathName + "\\" + animName + ".map";		//�ۑ���̃t�@�C���p�X���쐬

	HitBox oneTimeData;

	FILE* file;

	fopen_s(&file, SavePass.c_str(), "wb");		// ̧�ٵ���݁@�J���ď�������ŕ���

	expData.hitBoxNum.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		expData.hitBoxNum[i] = data[i].hitBox.size();
	}
	

	fwrite(&expData.animNum, sizeof(int), 1, file);

	fwrite(&expData.hitBoxNum[0], sizeof(int)*expData.hitBoxNum.size(), 1, file);

	for (int i = 0; i < data.size(); ++i)
	{
		for (int a = 0; a < data[i].hitBox.size(); a++)
		{
			oneTimeData = data[i].hitBox[a];

			fwrite(&oneTimeData, sizeof(oneTimeData),1, file);
		}
	}

	fclose(file);

	saveFlag = false;

}

void CollisionMng::Init(void)
{
	data.clear();
	colData.rect.startPos = { -1,-1 };
	colData.rect.endPos   = { -1,-1 };
	inputFlag = false;
	loadFlag = false;
	saveFlag = false;
	colTypeCnt = 0;

	for (int i = 0; i < 20; i++)
	{
		animFram[i] = 0;
	}
}