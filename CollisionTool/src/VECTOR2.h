#pragma once

class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);
	~VECTOR2();

	int x;
	int y;

	// ë„ì¸ââéZéq
	VECTOR2& operator = (const VECTOR2& vec);

	// ìYÇ¶éöââéZéq
	int& operator[](int i);

	// î‰ärââéZéq
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;

	// íPçÄââéZéq
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (int k);
	VECTOR2& operator /= (int k);
	VECTOR2  operator+() const;
	VECTOR2  operator-() const;
};

// Õﬁ∏ƒŸÇÃââéZ
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

struct Circle
{
	Circle() : center({0, 0}), r(0) {}
	Circle(VECTOR2 in_center, int in_r) : center(in_center), r(in_r) {}

	VECTOR2 center;
	int r;
};

