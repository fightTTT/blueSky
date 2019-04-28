#pragma once
#include "DxLib.h"
#include "VECTOR2.h"
#include <array>

#define GET_KEY_INTERVAL	  30		// InputIntervalHold内で使われる、Trueを返す間隔の初期値
#define GET_KEY_INTERVAL_MIN  5			// InputIntervalHold内で使われる、Trueを返す間隔の最小値



typedef std::array<char, 256> HitKeyArray;

class GameController
{
public:
	GameController();
	~GameController();
	const HitKeyArray& GetNowInput()const;		// 現フレームのHitKeyArrayのゲッター
	const HitKeyArray& GetOldInput()const;		// 1フレーム前のHitKeyArrayのゲッター

	const int& GetNowMouseInputFlag()const;		// 現フレームのMouseInputFlagのゲッター
	const int& GetOldMouseInputFlag()const;		// 1フレーム前のMouseInputFlagのゲッター
	const VECTOR2& GetMousePos()const;			// カーソル座標のゲッター

	void Update();

	// KeyCodeを指定すればそのKeyCodeのフラグを返してくれる
	const bool GetNowInputKey(int InputKeyCode)const;		// 現フレームのInputKeyCodeに渡されたKeyCodeのフラグを返す
	const bool GetOldInputKey(int InputKeyCode)const;		// 前フレームのInputKeyCodeに渡されたKeyCodeのフラグを返す

	//----------------------------------------------------------------------------------//
	// 引数にDxLibで設定されたKeyCodeを送るだけで後はTrueかFalseが帰ってくる			//
	// Trueが帰ってきているときが条件に合っているときで、Falseの時は条件に合っていない時//
	//----------------------------------------------------------------------------------//

	bool GetInputDown			(int InputKeyCode)const;	// 1フレームだけTrueを返す、長押ししてもTrueを返さない
	bool GetInputHold			(int InputKeyCode)const;	// 前フレームから継続して押されている時にTrueを返す
	bool GetInputUp				(int InputKeyCode)const;	// ボタンが押された状態から離されたときに１フレームだけTrueを返す

	bool GetMouseInputDown		(int InputKeyCode)const;	// 1フレームだけTrueを返す、長押ししてもTrueを返さない
	bool GetMouseInputHold		(int InputKeyCode)const;	// 前フレームから継続して押されている時にTrueを返す
	bool GetMouseInputUp		(int InputKeyCode)const;	// ボタンが押された状態から離されたときに１フレームだけTrueを返す

	bool GetInputDown		(int InputPadCode,int PadNum)const;	// 1フレームだけTrueを返す、長押ししてもTrueを返さない
	bool GetInputHold		(int InputPadCode,int PadNum)const;	// 前フレームから継続して押されている時にTrueを返す
	bool GetInputUp			(int InputPadCode,int PadNum)const;	// ボタンが押された状態から離されたときに１フレームだけTrueを返す

protected:
	HitKeyArray HitKeyNowArray;								// 現フレームの入力されているキー情報が格納されている
	HitKeyArray HitKeyOldArray;								// 1フレーム前に入力されているキー情報が格納されている

	int NowMouseInputFlag;									// 現フレームのマウスのキー入力が格納される
	int OldMouseInputFlag;									// 1フレームのマウスのキー入力が格納される

	VECTOR2 MousePos;										//マウスのポジション

	int HitPad0_NowArray;									// 現フレームの入力されているパッド０情報が格納されている
	int HitPad0_OldArray;									// 1フレーム前に入力されているパッド０情報が格納されている
	int HitPad1_NowArray;									// 現フレームの入力されているパッド１情報が格納されている
	int HitPad1_OldArray;									// 1フレーム前に入力されているパッド１情報が格納されている
	int HitPad2_NowArray;									// 現フレームの入力されているパッド２情報が格納されている
	int HitPad2_OldArray;									// 1フレーム前に入力されているパッド２情報が格納されている
	int HitPad3_NowArray;									// 現フレームの入力されているパッド３情報が格納されている
	int HitPad3_OldArray;									// 1フレーム前に入力されているパッド３情報が格納されている

	std::array<int*[2], 4> PadArray;							// GetInput関数の第2引数から、Pad情報が格納されている配列へアクセスする
private:
	void RefreshInput(void);								// HitKeyArrayのリフレッシュ
};

