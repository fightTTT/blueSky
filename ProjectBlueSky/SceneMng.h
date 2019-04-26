#pragma once
#include <memory>
#include "VECTOR2.h"
#include "BaseScene.h"

class GameCtrl;

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng &GetInstance(void)			// このｸﾗｽのﾎﾟｲﾝﾀを取得する、ここでｼﾝｸﾞﾙﾄﾝの状態にしている
	{
		static SceneMng s_Instance;
		return s_Instance;
	}
	void Run(void);
	VECTOR2 GetScreenSize(void);						// ｽｸﾘｰﾝｻｲｽﾞを取得する

private:
	SceneMng();
	~SceneMng();

	std::shared_ptr<GameCtrl> gameCtl;		// GameControllerｸﾗｽのﾎﾟｲﾝﾀ格納用変数

	unique_Base activeScene;	// 各ｼｰﾝにｱｸｾｽするためのﾕﾆｰｸﾎﾟｲﾝﾀｰ

	bool SysInit(void);			// ｼｽﾃﾑ的な変数の初期化を行う
};

