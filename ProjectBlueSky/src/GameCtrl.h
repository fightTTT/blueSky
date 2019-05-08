#pragma once
#include <array>

enum BUTTON_TYPE
{
	// 左スティック
	THUMB_L_UP,
	THUMB_L_DOWN,
	THUMB_L_RIGHT,
	THUMB_L_LEFT,

	// 右スティック
	THUMB_R_UP,
	THUMB_R_DOWN,
	THUMB_R_RIGHT,
	THUMB_R_LEFT,

	// 十字キー
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_RIGHT,
	BUTTON_LEFT,

	BUTTON_X,
	BUTTON_Y,
	BUTTON_A,
	BUTTON_B,

	// スタートボタン
	BUTTON_START,

	BUTTON_TYPE_MAX
};

enum PAD_ID
{
	PAD_1,
	PAD_2,
	PAD_MAX
};

using KEY_ARRAY = std::array<char, 256>;
using PAD_ARRAY = std::array<bool, BUTTON_TYPE_MAX>;

enum KEY_TYPE
{
	KEY_TYPE_NOW,
	KEY_TYPE_OLD,
	KEY_TYPE_MAX
};

class GameCtrl
{
public:
	GameCtrl();
	~GameCtrl();

	const KEY_ARRAY &GetCtrl(KEY_TYPE type) const;		// 引数に対応したﾀｲﾐﾝｸﾞのｷｰ入力情報を取得する
	bool GetPadData(PAD_ID padNum, BUTTON_TYPE button) const;
	bool GetPadDataTrg(PAD_ID padNum, BUTTON_TYPE button) const;

	bool UpDate(void);		// ｷｰの入力情報の更新をする

private:
	KEY_ARRAY data;			// 現在のﾌﾚｰﾑのｷｰ入力情報を格納する
	KEY_ARRAY dataOld;		// 1ﾌﾚｰﾑ前のﾌﾚｰﾑのｷｰ入力情報を格納する

	std::array<PAD_ARRAY, PAD_MAX> padData;
	std::array<PAD_ARRAY, PAD_MAX> padDataOld;
};
