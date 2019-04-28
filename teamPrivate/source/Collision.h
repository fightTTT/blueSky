#pragma once
#include "VECTOR2.h"

class Collision
{
public:
	Collision();
	~Collision();

};

// 当たり判定

// 矩形同士の当たり判定
// 引数(    始点, 終点 )
bool Square(VECTOR2 startPos1, VECTOR2 endPos1, VECTOR2 startPos2, VECTOR2 endPos2);

// 円同士の当たり判定 
// 引数(中心座標,半径)
bool Circle(VECTOR2 centerPos1, int r1, VECTOR2 centerPos2, int r2);

// 円と矩形の当たり判定
// 引数(中心座標,半径,矩形の始点,終点)
// 参考ｻｲﾄ https://ja.stackoverflow.com/questions/28275/%E9%95%B7%E6%96%B9%E5%BD%A2%E3%81%A8%E5%86%86%E3%81%AE%E8%A1%9D%E7%AA%81%E5%88%A4%E5%AE%9A%E3%82%92%E8%A1%8C%E3%81%86%E3%82%B3%E3%83%BC%E3%83%89%E3%82%92%E6%95%99%E3%81%88%E3%81%A6%E4%B8%8B%E3%81%95%E3%81%84
bool CircleToSpuare(VECTOR2 pos, int r, VECTOR2 startPos, VECTOR2 endPos);