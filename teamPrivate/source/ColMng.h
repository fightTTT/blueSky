#pragma once
#include <vector>
#include <map>
#include <list>
#include "VECTOR2.h"

struct DataHeader
{
	int colCnt;						// 保存するｺﾗｲﾀﾞｰの数
};

enum COL_TYPE {
	COL_TYPE_PLAYER,
	COL_TYPE_WEAPON,
	COL_TYPE_NON
};

typedef struct
{
	bool	 boxFlag;		// true:矩形　false:円形
	COL_TYPE weponFlag;		// 武器のｺﾗｲﾀﾞｰ　ﾌﾟﾚｲﾔのｺﾗｲﾀﾞｰ

	VECTOR2	 startPos;		// 矩形開始位置
	VECTOR2  endPos;		// 矩形終了位置

	VECTOR2 offset;			// 画像ｵﾌｾｯﾄ値	↑↓→←で調整するやつ

	int		 r;				// 円形半径

	int		 id;			// 何ﾌﾚｰﾑ目か

	int		 attackPoint;	// 攻撃力
	int		 angryPoint;	// 怒りｹﾞｰｼﾞﾎﾟｲﾝﾄ
	int		 specialPoint;	// 必殺ｹﾞｰｼﾞﾎﾟｲﾝﾄ
}ColInf;

#define lpColMng  ColMng::GetInstance()

class ColMng
{
public:
	static ColMng &GetInstance(void)
	{
		static ColMng s_Instance;
		return (s_Instance);
	}

	//ゲッター　キャラ名とアニメーション名を指定して、そのアニメーションのVectorを返す(増田システム＿テスト実装)
    std::vector<ColInf> GetCollider(std::string CharName, std::string AnimName);
	std::vector<ColInf> GetCollider(std::string CharName, std::string AnimName,int animFrame);

	//新ローダー　キャラ名とアニメーション名を指定してMapに格納する(増田システム＿テスト実装)
	void ColLoad(std::string CharName, std::string AnimName);
private:
	ColMng();
	~ColMng();

	// ｷｬﾗ名→ｱﾆﾒｰｼｮﾝ名→ｱﾆﾒｰｼｮﾝ番号
	std::map<std::string, std::map<std::string, std::vector<ColInf>> >	colMap;			//ｺﾗｲﾀﾞｰ情報を格納
};

