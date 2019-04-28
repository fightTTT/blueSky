#pragma once
#include <list>
#include <map>
#include "Gamecontroller.h"
#include "BaseScene.h"
#include "ColMng.h"

#define P_CHAR_MAX	(3)		// 登場ｷｬﾗの総数
#define ANIM_MAX	(21)	// ｱﾆﾒｰｼｮﾝﾊﾟﾀｰﾝの総数

class CollisionEditScene :
	public BaseScene
{
public:
	CollisionEditScene();
	~CollisionEditScene();
	unique_Base Update(unique_Base own, const GameController &controller);
	void Draw(void);

private:
	bool ColSave(void);				// ｺﾗｲﾀﾞｰ情報のｾｰﾌﾞ

	bool ColSave(std::string CharName,std::string AnimName);// ｺﾗｲﾀﾞｰ情報のｾｰﾌﾞ(増田システム＿テスト実装)

	void ChangePicture(void);								//コライダーを設定する画像の変更(増田システム_テスト実装)

	int Init(void);					// 初期化関係
	void nowIdListInit(int id);		// nowIdListの初期化	idが変わるたびに呼ぶ
	void ColInfoInit(void);			// ｺﾗｲﾀﾞｰ情報の初期化

	void AddCol(void);				// ｺﾗｲﾀﾞｰ情報の追加

	void DrawGrid(void);			// 罫線の描画
	void DrawInfo(int colColor);	// ｽﾃｰﾀｽ情報の描画　pos値など

	std::vector<ColInf> colVector;	// ｺﾗｲﾀﾞｰを保存するVector
	std::list<ColInf> colList;		// ｺﾗｲﾀﾞｰを保存するList
	std::list<ColInf> nowIdList;		// 現在のidだけが入っているList

	std::string NowCharName;		//現在設定中のキャラクター名　(増田システム＿テスト実装)
	std::string NowAnimName;		//現在設定中のアニメーション名(増田システム＿テスト実装)

	std::string NowUseDataName;		//現在使用中の連番データ名(増田システム＿テスト実装)

	int id;							// 表示画像のID

	VECTOR2 nowPos;

	ColInf data; 
	
	std::array<std::string, P_CHAR_MAX>	charNameTbl;	// ｷｬﾗ名を格納
	std::array<std::string, ANIM_MAX>	animNameTbl;	// ｱﾆﾒｰｼｮﾝ名を格納
	std::array<std::string, ANIM_MAX>	dataNameTbl;	// ｱﾆﾒｰｼｮﾝﾌｧｲﾙ名を格納
	std::array<int, 3> maxTblPtn;						// 各NameTblの最大要素数を格納
	int tblCnt;						// NameTblの要素にｱｸｾｽするためのｶｳﾝﾄ
	bool changePictureFlag;			// ｴﾃﾞｨｯﾄ画像変更状態管理ﾌﾗｸﾞ	true:変更
	int changeCnt;					// 変更するものを表すｶｳﾝﾄ	0:ｷｬﾗ名	1:ｱﾆﾒｰｼｮﾝ名	その他:変更終了

	bool newEditFlag;				// true:ｺﾘｼﾞｮﾝの新規追加	false:既存のｺﾘｼﾞｮﾝの修正
	std::list<ColInf>::iterator			editItr;		// 現在編集中のｺﾘｼﾞｮﾝを指す
	int editCnt;					// 現在編集中のｺﾗｲﾀﾞｰを点滅表示させるためのｶｳﾝﾀｰ
};
