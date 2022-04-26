#pragma once
#include "header.h"
// ■■■■■ 幾何学（構造） ■■■■■


//【平面上の点，二次元ベクトル】
/*
* 平面における点／二次元ベクトルを表す構造体
*
* Point<T>() : O(1)
*	(0, 0) で初期化する．
*
* Point<T>(T x, T y) : O(1)
*	(x, y) で初期化する．
*
* p1 == p2, p1 != p2, p1 < p2, p1 > p2, p1 <= p2, p1 >= p2 : O(1)
*	x 座標優先，次いで y 座標の大小比較を行う．
* 
* p1 + p2, p1 - p2, c * p, p * c, p / c : O(1)
*	ベクトルとみなした加算，減算，スカラー倍，スカラー除算を行う．複合代入演算子も使用可．
*
* T sqnorm() : O(1)
*	自身の 2 乗ノルムを返す．
*
* double norm() : O(1)
*	自身のノルムを返す．
*
* Point<double> normalize() : O(1)
*	自身を正規化したベクトルを返す．
*
* T dot(Point<T> p) : O(1)
*	自身と p との内積を返す．
*
* T cross(Point<T> p) : O(1)
*	自身と p との外積を返す．
* 
* T angle(Point<T> p) : O(1)
*	自身から p までの成す角度を返す．
*/
template <class T> struct Point {
	// 点の x 座標，y 座標
	T x, y;

	// コンストラクタ
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}

	// 代入
	Point(const Point& old) = default;
	Point& operator=(const Point& other) = default;

	// キャスト
	operator Point<ll>() const { return Point<ll>((ll)x, (ll)y); }
	operator Point<double>() const { return Point<double>((double)x, (double)y); }

	// 入出力
	friend istream& operator>>(istream& is, Point& p) { is >> p.x >> p.y; return is; }
	friend ostream& operator<<(ostream& os, const Point& p) { os << '(' << p.x << ',' << p.y << ')'; return os; }

	// 比較（x 座標優先）
	bool operator==(const Point& p) const { return x == p.x && y == p.y; }
	bool operator!=(const Point& p) const { return !(*this == p); }
	bool operator<(const Point& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator>=(const Point& p) const { return !(*this < p); }
	bool operator>(const Point& p) const { return x == p.x ? y > p.y : x > p.x; }
	bool operator<=(const Point& p) const { return !(*this > p); }

	// 加算，減算，スカラー倍，スカラー除算
	Point& operator+=(const Point& p) { x += p.x; y += p.y;	return *this; }
	Point operator+(const Point& p) const { Point q(*this); return q += p; }
	Point& operator-=(const Point& p) { x -= p.x; y -= p.y;	return *this; }
	Point operator-(const Point& p) const { Point q(*this); return q -= p; }
	Point& operator*=(const T& c) { x *= c; y *= c;	return *this; }
	Point operator*(const T& c) const { Point q(*this); return q *= c; }
	Point& operator/=(const T& c) { x /= c; y /= c;	return *this; }
	Point operator/(const T& c) const { Point q(*this); return q /= c; }
	friend Point operator*(const T& sc, const Point& p) { return p * sc; }

	// 二乗ノルム，ノルム，正規化
	T sqnorm() const { return x * x + y * y; }
	double norm() const { return sqrt((double)x * x + (double)y * y); }
	Point<double> normalize() const { return Point<double>(*this) / norm(); }

	// 内積，外積，成す角度
	T dot(const Point& other) const { return x * other.x + y * other.y; }
	T cross(const Point& other) const { return x * other.y - y * other.x; }
	double angle(const Point& other) const { 
		return atan2(this->cross(other), this->dot(other));
	}
};


//【平面内の直線，線分】
/*
* {a, b} : 2 点 a, b を通る a → b 方向の有向直線を表す．
*
* その他，無向直線，有向線分，無向線分などを表すのにも用いる．
*/
template <class T> using Line = pair<Point<T>, Point<T>>;


//【平面内の円】
/*
* {p, r} : 点 p を中心とする半径 r の円を表す．
*/
template <class T> using Circle = pair<Point<T>, T>;


//【平面内の多角形】
/*
* Polygon(p[0..n]) : これらの点を周る順に頂点にもつ n 角形を表す．
*/
template <class T> using Polygon = vector<Point<T>>;


