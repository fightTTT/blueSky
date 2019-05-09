#pragma once

class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);
	~VECTOR2();

	int x;
	int y;

	// ‘ã“ü‰‰ŽZŽq
	VECTOR2& operator = (const VECTOR2& vec);

	// “Y‚¦Žš‰‰ŽZŽq
	int& operator[](int i);

	// ”äŠr‰‰ŽZŽq
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;
	bool operator<(int k) const;

	// ’P€‰‰ŽZŽq
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (int k);
	VECTOR2& operator /= (int k);
	VECTOR2  operator+() const;
	VECTOR2  operator-() const;
};

// ÍÞ¸ÄÙ‚Ì‰‰ŽZ
// VECTOR2 + int
VECTOR2 operator+(const VECTOR2 &u, int v);

// VECTOR2 - int
VECTOR2 operator-(const VECTOR2 &u, int v);

// VECTOR2 + VECTOR2
VECTOR2 operator+(const VECTOR2 &u, const VECTOR2 &v);

// VECTOR2 - VECTOR2
VECTOR2 operator-(const VECTOR2 &u, const VECTOR2 &v);

// VECTOR2 * int
VECTOR2 operator*(const VECTOR2 &u, int v);

// int * VECTOR2
VECTOR2 operator*(int u, const VECTOR2 &v);

// VECTOR2 / int
VECTOR2 operator/(const VECTOR2 &u, int v);

// VECTOR2 % VECTOR2
VECTOR2 operator/(const VECTOR2 &u, const VECTOR2 &v);

// VECTOR2 % int
VECTOR2 operator%(const VECTOR2 &u, int v);

// VECTOR2 % VECTOR2
VECTOR2 operator%(const VECTOR2 &u, const VECTOR2 &v);

struct Rect
{
	Rect() : startPos({0, 0}), endPos({0, 0}) {}
	Rect(VECTOR2 start, VECTOR2 end) : startPos(start), endPos(end) {}

	VECTOR2 startPos;
	VECTOR2 endPos;
};

