#pragma once
#include "header.h"
// ■■■■■ 幾何学（構造） ■■■■■


//【平面上の点，二次元ベクトル】
/*
* 平面における点 ／ 二次元ベクトルを表す構造体
*
* Point() : O(1)
*	(0, 0) で初期化する．
*
* Point(x, y) : O(1)
*	(x, y) で初期化する．
*
* Point<double>(p) : O(1)
*	実数平面上の点にキャストする．
*
* p1 + p2 : O(1)
*	ベクトルとみなした加算を行う．+= も使用可．
*
* p1 - p2 : O(1)
*	ベクトルとみなした減算を行う．-= も使用可．
*
* c * p1 ／ p1 * c : O(1)
*	ベクトルとみなしたスカラー倍を行う．*= も使用可．
*
* p1 / c : O(1)
*	ベクトルとみなしたスカラー除算を行う．/= も使用可．
*
* sqnorm() : O(1)
*	自身の 2 乗ノルムを返す．
*
* dot(p) : O(1)
*	自身と p との内積を返す．
*
* cross(p) : O(1)
*	自身と p との外積を返す．
*/
template <typename T>
struct Point {
	// 点の x 座標と y 座標
	T x, y;

	// コンストラクタ
	Point() : x(0), y(0) {}
	Point(T tmp_x, T tmp_y) : x(tmp_x), y(tmp_y) {}
	Point(const Point& old) = default;

	// 代入
	Point& operator=(const Point& other) = default;

	// キャスト
	operator Point<double>() const { return Point<double>((double)x, (double)y); }
	operator Point<ll>() const { return Point<ll>((ll)x, (ll)y); }

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

	// 加減，スカラー倍，スカラー除算
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
	double norm() const { return sqrt(sqnorm()); }
	Point<double> normalize() const { return Point<double>(*this) / norm(); }

	// 内積，外積
	T dot(const Point& other) const { return x * other.x + y * other.y; }
	T cross(const Point& other) const { return x * other.y - y * other.x; }
};


//【平面内の直線】
/*
* {a, b} : 2 点 a, b を通る a → b 方向の有向直線を表す．
*
* その他，無向直線，有向線分，無向線分などを表すのにも用いる．
*/
template <typename T>
using Line = pair<Point<T>, Point<T>>;


//【平面内の多角形】
/*
* {pl[0], ..., pl[n - 1]} : 
*	これらの n 点を周る順に頂点にもつ n 角形を表す．
*/
template <typename T>
using Polygon = vector<Point<T>>;


//【平面内の円】
/*
* {p, r} : 点 p を中心とする半径 r の円を表す．
*/
template <typename T>
using Circle = pair<Point<T>, T>;

