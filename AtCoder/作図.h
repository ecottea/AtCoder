#pragma once
#include "header.h"
#include "構造(幾何).h"
// ■■■■■ 作図 ■■■■■


//【回転】O(1)
/*
* 点 p を点 c を中心に th[rad] だけ回転した点を返す．
*/
inline Point<double> rotate(const Point<double>& p, const Point<double>& c, double th) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_5_A

	Point<double> q;
	q.x = cos(th) * (p.x - c.x) - sin(th) * (p.y - c.y) + c.x;
	q.y = sin(th) * (p.x - c.x) + cos(th) * (p.y - c.y) + c.y;
	return q;
}


//【90° 回転】O(1)
/*
* 点 p を点 c を中心に 90° × i だけ回転した点を返す．
*/
template <class T>
inline Point<T> rotate90(const Point<T>& p, const Point<T>& c, int i) {
	Point<T> q;
	switch (smod(i, 4)) {
	case 0:
		q.x = 1 * (p.x - c.x) - 0 * (p.y - c.y) + c.x;
		q.y = 0 * (p.x - c.x) + 1 * (p.y - c.y) + c.y;
		break;
	case 1:
		q.x = 0 * (p.x - c.x) - 1 * (p.y - c.y) + c.x;
		q.y = 1 * (p.x - c.x) + 0 * (p.y - c.y) + c.y;
		break;
	case 2:
		q.x = -1 * (p.x - c.x) - 0 * (p.y - c.y) + c.x;
		q.y = 0 * (p.x - c.x) + (-1) * (p.y - c.y) + c.y;
		break;
	case 3:
		q.x = 0 * (p.x - c.x) - (-1) * (p.y - c.y) + c.x;
		q.y = -1 * (p.x - c.x) + 0 * (p.y - c.y) + c.y;
		break;
	default:;
	}
	return q;
}


//【角の二等分線】O(1)
/*
* ∠ a o b の二等分線を返す．
*/
template <class T>
inline Line<double> corner_bisector(const Point<T>& a, const Point<T>& o, const Point<T>& b) {
	Point<double> p1 = o;
	Point<double> p2 = p1 + (a - o).normalize() + (b - o).normalize();
	return { p1, p2 };
}


//【垂直二等分線】O(1)
/*
* 線分 l の垂直二等分線を返す．
*/
template <class T>
inline Line<double> vertical_bisector(const Line<T>& l) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_C

	Point<double> p1 = Point<double>(l.first + l.second) / 2.;
	Point<double> d = l.second - l.first;
	Point<double> n(-d.y, d.x);
	Point<double> p2 = p1 + n;
	return { p1, p2 };
}


//【垂線の足】O(1)
/*
* 点 p から直線 l へ降ろした垂線の足を返す．
*/
inline Point<double> foot_of_perpendicular(const Point<double>& p, const Line<double>& l) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_1_A

	auto d = (l.second - l.first).normalize();
	return l.first + (p - l.first).dot(d) * d;
}


//【対称移動】O(1)
/*
* 直線 l に関して点 p を対称移動した点を返す．
*/
inline Point<double> symmetrical_move(const Point<double>& p, const Line<double>& l) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_1_B

	auto d = (l.second - l.first).normalize();
	auto v = p - l.first;
	return l.first - v + v.dot(d) * d * 2.0;
}


//【2 直線の交点】O(1)
/*
* 2 直線 l1, l2 の交点を (x/d, y/d) とし，3 つ組 {x, y, d} を返す．
*
* 制約：8×(座標)^3 がオーバーフローしない
*/
template <class T>
tuple<T, T, T> intersection_L_L(const Line<T>& l1, const Line<T>& l2) {
	// verify : https://atcoder.jp/contests/abc356/tasks/abc356_g

	T x1 = l1.first.x;
	T y1 = l1.first.y;
	T x2 = l1.second.x;
	T y2 = l1.second.y;
	T x3 = l2.first.x;
	T y3 = l2.first.y;
	T x4 = l2.second.x;
	T y4 = l2.second.y;

	T x_num = x2 * x3 * y1 - x2 * x4 * y1 - x1 * x3 * y2 + x1 * x4 * y2
		- x1 * x4 * y3 + x2 * x4 * y3 + x1 * x3 * y4 - x2 * x3 * y4;
	T y_num = x2 * y1 * y3 - x4 * y1 * y3 - x1 * y2 * y3 + x4 * y2 * y3
		- x2 * y1 * y4 + x3 * y1 * y4 + x1 * y2 * y4 - x3 * y2 * y4;
	T dnm = x3 * y1 - x4 * y1 - x3 * y2 + x4 * y2
		- x1 * y3 + x2 * y3 + x1 * y4 - x2 * y4;

	return { x_num, y_num, dnm };
}


//【2 直線の交点（実数）】O(1)
/*
* 2 直線 l1, l2 の交点を返す．
*/
template <class T>
Point<double> intersection_L_L_double(const Line<T>& l1, const Line<T>& l2) {
	// verify : https://codeforces.com/contest/598/problem/F

	double x1 = (double)l1.first.x;
	double y1 = (double)l1.first.y;
	double x2 = (double)l1.second.x;
	double y2 = (double)l1.second.y;
	double x3 = (double)l2.first.x;
	double y3 = (double)l2.first.y;
	double x4 = (double)l2.second.x;
	double y4 = (double)l2.second.y;

	double x_num = x2 * x3 * y1 - x2 * x4 * y1 - x1 * x3 * y2 + x1 * x4 * y2
		- x1 * x4 * y3 + x2 * x4 * y3 + x1 * x3 * y4 - x2 * x3 * y4;
	double y_num = x2 * y1 * y3 - x4 * y1 * y3 - x1 * y2 * y3 + x4 * y2 * y3
		- x2 * y1 * y4 + x3 * y1 * y4 + x1 * y2 * y4 - x3 * y2 * y4;
	double dnm = x3 * y1 - x4 * y1 - x3 * y2 + x4 * y2
		- x1 * y3 + x2 * y3 + x1 * y4 - x2 * y4;

	return { x_num / dnm, y_num / dnm };
}


//【円と直線の交点】O(1)
/*
* 円 c と直線 l の共有点の個数を返す．また共有点があればその座標を p1, p2 に格納する．
*/
int intersection_C_L(const Circle<ll>& c, const Line<ll>& l, Point<double>& p1, Point<double>& p2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_D
	
	// 円 c の中心が原点にくるよう平行移動
	Point<ll> o = c.first;
	Point<ll> a = l.first - o;
	Point<ll> b = l.second - o;

	// 直線 l の方向ベクトル
	Point<ll> d = b - a;

	// (0, 0) と l との符号付き距離の分子，分母の二乗，円 c の半径
	ll num = a.cross(b);
	ll dnm_sq = d.sqnorm();
	ll r = c.second;

	// (0, 0) と l との距離が円の半径より大きい場合 → 共有点 0 個
	if (dnm_sq != 0 && (num * num - 1) / dnm_sq >= r * r) return 0;

	// (0, 0) と l との符号付き距離
	double dist = num / sqrt(dnm_sq);

	// 円 c の中心から弦の中点までのベクトル
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (-dist / d.norm());

	// (0, 0) と l との距離が円の半径に等しい場合 → 接点 1 個
	if (dnm_sq != 0 && num * num / dnm_sq >= r * r) {
		p1 = p2 = Point<double>(o) + nn;
		return 1;
	}

	// 弦の中点から一方の交点までのベクトル
	Point<double> nd = Point<double>(d) * (sqrt(r * r - dist * dist) / d.norm());

	// (0, 0) と l との距離が円の半径より小さい場合 → 交点 2 個
	p1 = Point<double>(o) + nn + nd;
	p2 = Point<double>(o) + nn - nd;
	return 2;
}


//【円と直線の交点（実数）】O(1)
/*
* 円 c と直線 l の共有点の個数を返す．また共有点があればその座標を p1, p2 に格納する．
*/
int intersection_C_L(const Circle<double>& c, const Line<double>& l, Point<double>& p1, Point<double>& p2) {
	// verify : https://atcoder.jp/contests/abc263/tasks/abc263_h

	// 円 c の中心が原点にくるよう平行移動
	Point<double> o = c.first;
	Point<double> a = l.first - o;
	Point<double> b = l.second - o;

	// 直線 l の方向ベクトル
	Point<double> d = b - a;

	// (0, 0) と l との符号付き距離の分子，分母の二乗，円 c の半径
	double num = a.cross(b);
	double dnm_sq = d.sqnorm();
	double r = c.second;

	// (0, 0) と l との距離が円の半径より大きい場合 → 共有点 0 個
	if (num * num >= r * r * dnm_sq) return 0;

	// (0, 0) と l との符号付き距離
	double dist = num / sqrt(dnm_sq);

	// 円 c の中心から弦の中点までのベクトル
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (-dist / d.norm());

	// 弦の中点から一方の交点までのベクトル
	Point<double> nd = Point<double>(d) * (sqrt(r * r - dist * dist) / d.norm());

	// (0, 0) と l との距離が円の半径より小さい場合 → 交点 2 個
	p1 = Point<double>(o) + nn + nd;
	p2 = Point<double>(o) + nn - nd;
	return 2;
}


//【2 円の交点】O(1)
/*
* 2 円 c1, c2 の共有点の個数を返す．また共有点があればその座標を p1, p2 に格納する．
*/
int intersection_C_C(const Circle<ll>& c1, const Circle<ll>& c2, Point<double>& p1, Point<double>& p2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_E
	
	// 円 c1, c2 の中心と半径
	Point<ll> o1 = c1.first, o2 = c2.first;
	ll r1 = c1.second, r2 = c2.second;

	// o1 から o2 へのベクトル，半径の和と差
	Point<ll> d = o2 - o1;
	ll r_sum = r1 + r2, r_dif = abs(r1 - r2);

	// 中心間距離が円の半径の和より大きい場合 → 共有点 0 個
	if (d.sqnorm() > r_sum * r_sum) {
		return 0;
	}

	// 中心間距離が円の半径の和に等しい場合 → 外接点 1 個
	if (d.sqnorm() == r_sum * r_sum) {
		p1 = p2 = Point<double>(o1) + Point<double>(d) * (r1 / d.norm());
		return 1;
	}

	// 中心間距離が円の半径の差より小さい場合 → 共有点 0 個
	if (d.sqnorm() < r_dif * r_dif) {
		return 0;
	}

	// 中心間距離が円の半径の差に等しい場合 → 内接点 1 個
	if (d.sqnorm() == r_dif * r_dif) {
		if (r1 > r2) {
			p1 = p2 = Point<double>(o1) + Point<double>(d) * (r1 / d.norm());
		}
		else {
			p1 = p2 = Point<double>(o1) - Point<double>(d) * (r1 / d.norm());
		}
		return 1;
	}

	// その他の場合 → 交点 2 個
	double x = (r1 * r1 - r2 * r2 + d.sqnorm()) / (2 * d.norm());
	double h = sqrt(r1 * r1 - x * x);
	Point<double> nd = Point<double>(d) * (x / d.norm());
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (h / d.norm());
	p1 = Point<double>(o1) + nd + nn;
	p2 = Point<double>(o1) + nd - nn;
	
	if (isnan(p1.x)) return 0;

	return 2;
}


//【2 円の交点（中心と 1 点指定）】O(1)
/*
* 点 o1 を中心とし点 a1 を通る円と点 o2 を中心とし点 a2 を通る円との共有点のリストを返す．
*
* 制約 : 4 乗がオーバーフローしない
*/
template <class T>
vector<Point<double>> intersection_C_C(const Point<T>& o1, const Point<T>& a1, const Point<T>& o2, const Point<T>& a2) {
	T x1 = o1.x;
	T y1 = o1.y;
	T x2 = o2.x;
	T y2 = o2.y;

	T r1_sq = (a1 - o1).sqnorm();
	T r2_sq = (a2 - o2).sqnorm();
	T dist_sq = (o1 - o2).sqnorm();

	// D : 判別式
	T D = 2 * dist_sq * (r1_sq + r2_sq) - (r1_sq - r2_sq) * (r1_sq - r2_sq) - dist_sq * dist_sq;

	vector<Point<double>> res;

	if (D < 0) return res;

	T x_num = (r2_sq - r1_sq) * (x1 - x2) + (x1 + x2) * dist_sq;
	T y_num = (r2_sq - r1_sq) * (y1 - y2) + (y1 + y2) * dist_sq;

	if (D == 0) {
		double res_x = (double)x_num / (2 * dist_sq);
		double res_y = (double)y_num / (2 * dist_sq);
		res.emplace_back(res_x, res_y);
	}
	else {
		// ここで実数同士の異符号の和が発生しているので桁落ちする．
		double res_x = (x_num + (y2 - y1) * sqrt(D)) / (2 * dist_sq);
		double res_y = (y_num - (x2 - x1) * sqrt(D)) / (2 * dist_sq);
		res.emplace_back(res_x, res_y);

		res_x = (x_num - (y2 - y1) * sqrt(D)) / (2 * dist_sq);
		res_y = (y_num + (x2 - x1) * sqrt(D)) / (2 * dist_sq);
		res.emplace_back(res_x, res_y);
	}

	return res;
}


//【2 円の交点（実数）】O(1)（誤差注意！）
/*
* 2 円 c1, c2 の共有点のリストを返す．
*/
template <class D>
vector<Point<D>> intersection_C_C(const Circle<D>& c1, const Circle<D>& c2) {
	// verify : https://atcoder.jp/contests/abc157/tasks/abc157_f

	// 円 c1, c2 の中心と半径
	Point<D> o1 = c1.first, o2 = c2.first;
	D r1 = c1.second, r2 = c2.second;

	// o1 から o2 へのベクトル，半径の和と差
	Point<D> d = o2 - o1;
	D r_sum = r1 + r2, r_dif = abs(r1 - r2);

	vector<Point<D>> res;

	// 中心間距離が円の半径の和より大きい場合 → 共有点 0 個
	if (d.sqnorm() > r_sum * r_sum) return res;

	// 中心間距離が円の半径の差より小さい場合 → 共有点 0 個
	if (d.sqnorm() < r_dif * r_dif) return res;

	// その他の場合 → 交点 2 個
	D x = (r1 * r1 - r2 * r2 + d.sqnorm()) / (2 * d.norm());
	D h = sqrt(r1 * r1 - x * x);
	Point<D> nd = Point<D>(d) * (x / d.norm());
	Point<D> nn = Point<D>(-(D)d.y, (D)d.x) * (h / d.norm());
	res.push_back(Point<D>(o1) + nd - nn);
	res.push_back(Point<D>(o1) + nd + nn);

	// 2 個と思ってたら 0 個だった場合はごめんなさいする．
	if (isnan(res[0].x)) return vector<Point<D>>();

	return res;
}


//【円の接線】O(1)
/*
* 点 p を通る円 c の接線の本数を返す．また接点があればその座標を t1, t2 に格納する．
*/
int tangent_to_circle(const Point<ll>& p, const Circle<ll>& c, Point<double>& t1, Point<double>& t2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_F
	
	// 円 c の中心と半径
	Point<ll> o = c.first; ll r = c.second;

	// o から p へのベクトル，半径の和と差
	Point<ll> d = p - o;

	// p と o の距離が円の半径より小さい場合 → 接線 0 本
	if (d.sqnorm() < r * r) return 0;

	// p と o の距離が円の半径に等しい場合 → 接線 1 本
	if (d.sqnorm() == r * r) {
		t1 = t2 = p;
		return 1;
	}

	// p と o の距離が円の半径より大きい場合 → 接線 2 本
	double x = r * r / d.norm();
	double y = r * sqrt(1 - (double)r * r / d.sqnorm());
	Point<double> nd = Point<double>(d) * (x / d.norm());
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (y / d.norm());
	t1 = Point<double>(o) + nd + nn;
	t2 = Point<double>(o) + nd - nn;
	return 2;
}


//【2 円の共通接線】O(1)
/*
* 2 円 c1, c2 の共通接線の本数を返す．また c1 との接点があればその座標を ts に格納する．
* c1 = c2 のときは -1 を返す．
*/
int common_tangent(const Circle<ll>& c1, const Circle<ll>& c2, vector<Point<double>>& ts) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_G
	
	ts.clear();

	if (c1 == c2) return -1;

	// 円 c1, c2 の中心と半径
	Point<ll> o1 = c1.first; ll r1 = c1.second;
	Point<ll> o2 = c2.first; ll r2 = c2.second;

	// o1 から o2 へのベクトル，半径の和と差
	Point<ll> d = o2 - o1;
	ll r_sum = r1 + r2, r_dif = abs(r1 - r2);

	// 中心間距離が円の半径の差より小さい場合 → 共通接線 0 本
	if (d.sqnorm() < r_dif * r_dif) return 0;

	// 中心間距離が円の半径の差に等しい場合 → 共通接線 1 本
	if (d.sqnorm() == r_dif * r_dif) {
		ts.push_back(Point<double>(o1) + (r1 > r2 ? 1 : -1) * Point<double>(d) * (r1 / d.norm()));
		return 1;
	}

	// 共通外接線を追加
	double x = r1 * r_dif / d.norm();
	double y = r1 * sqrt(1 - (double)r_dif * r_dif / d.sqnorm());
	Point<double> nd = Point<double>(d) * (x / d.norm());
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (y / d.norm());
	ts.push_back(Point<double>(o1) + (r1 > r2 ? 1 : -1) * nd + nn);
	ts.push_back(Point<double>(o1) + (r1 > r2 ? 1 : -1) * nd - nn);

	// 中心間距離が円の半径の和より大きい場合 → 共通接線 4 本
	if (d.sqnorm() > r_sum * r_sum) {
		// 共通内接線を追加
		double x = r1 * r_sum / d.norm();
		double y = r1 * sqrt(1 - (double)r_sum * r_sum / d.sqnorm());
		Point<double> nd = Point<double>(d) * (x / d.norm());
		Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (y / d.norm());
		ts.push_back(Point<double>(o1) + nd + nn);
		ts.push_back(Point<double>(o1) + nd - nn);

		return 4;
	}

	// 中心間距離が円の半径の和に等しい場合 → 共通接線 3 本
	if (d.sqnorm() == r_sum * r_sum) {
		ts.push_back(Point<double>(o1) + Point<double>(d) * (r1 / d.norm()));
		return 3;
	}

	// その他の場合 → 共通接線 2 本
	return 2;
}


//【内接円】O(1)
/*
* 三角形 a b c の内接円を返す．
*/
template <class T>
Circle<double> incircle(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_B

	auto len_a = (b - c).norm();
	auto len_b = (c - a).norm();
	auto len_c = (a - b).norm();
	Point<double> da = a;
	Point<double> db = b;
	Point<double> dc = c;
	auto i = (len_a * da + len_b * db + len_c * dc) / (len_a + len_b + len_c);

	Point<double> d = (b - a).normalize();
	Point<double> n = { -d.y, d.x };
	Point<double> p2 = i - a;
	auto r = abs(p2.dot(n));

	return { i, r };
}


//【外接円】O(1)
/*
* 三角形 a b c の外接円を返す．
*/
template <class T>
Circle<double> circircle(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_C

	//【方法】
	// Mathematica で
	//	Solve[(x1-x)^2 + (y1-y)^2 == (x2-x)^2 + (y2-y)^2 == (x3-x)^2 + (y3-y)^2, {x, y}] // FullSimplify
	// を実行し，結果を CForm[] して整形した．

	double x1 = a.x, y1 = a.y;
	double x2 = b.x, y2 = b.y;
	double x3 = c.x, y3 = c.y;

	double dnm = 2 * (x3 * (-y1 + y2) + x2 * (y1 - y3) + x1 * (-y2 + y3));

	Point<double> o;
	o.x = (x3 * x3 * (y2 - y1) + x2 * x2 * (y1 - y3)
		+ (x1 * x1 + (y1 - y2) * (y1 - y3)) * (y3 - y2)) / dnm;
	o.y = (y3 * y3 * (x1 - x2) + (y2 * y2) * (x3 - x1)
		+ ((y1 * y1) + (x1 - x2) * (x1 - x3)) * (x2 - x3)) / dnm;
	auto r = (Point<double>(a) - o).norm();
	return { o, r };
}


//【最小包含円】O(n)
/*
* 点群 p[0..n) の最小包含円を求め，円上の点の 3 つ組 {i1, i2, i3} を返す．
*	i1 = i2 = i3 = -1	: 空
*	i2 = i3 = -1		: 1 点 p[i1]
*	i3 = -1				: 2 点 p[i1], p[i2] を直径の両端とする円
*	other				: 3 点 p[i1], p[i2], p[i3] を通る円
*
* 利用：【点の内外判定（円，直径指定）】,【点の内外判定（円，3 点指定）】
*/
template <class T>
tuple<int, int, int> minimum_enclosing_circle(const vector<Point<T>>& p) {
	// verify : https://judge.yosupo.jp/problem/minimum_enclosing_circle

	int n = sz(p);

	vi idx(n);
	iota(all(idx), 0);

	mt19937_64 mt((int)time(NULL));
	shuffle(all(idx), mt);

	int i1 = -1, i2 = -1, i3 = -1;

	rep(t, n) {
		int i = idx[t];

		if (i3 != -1 && inner_circle_by_3points(p[i1], p[i2], p[i3], p[i]) >= 0) continue;
		if (i3 == -1 && i2 != -1 && inner_circle_by_diameter(p[i1], p[i2], p[i]) >= 0) continue;
		i1 = i;
		i2 = -1;
		i3 = -1;

		rep(s, t) {
			int j = idx[s];

			if (i3 != -1 && inner_circle_by_3points(p[i1], p[i2], p[i3], p[j]) >= 0) continue;
			if (i3 == -1 && i2 != -1 && inner_circle_by_diameter(p[i1], p[i2], p[j]) >= 0) continue;
			i2 = j;
			i3 = -1;

			rep(u, s) {
				int k = idx[u];

				if (i3 != -1 && inner_circle_by_3points(p[i1], p[i2], p[i3], p[k]) >= 0) continue;
				if (i3 == -1 && i2 != -1 && inner_circle_by_diameter(p[i1], p[i2], p[k]) >= 0) continue;
				i3 = k;
			}
		}
	}

	return { i1, i2, i3 };
}


//【凸多角形の切断】O(n)
/*
* 凸 n 角形 poly を有向直線 l で切断した左側の凸多角形を返す．
*
* 利用：【2 直線の交点（実数）】
*/
template <class T>
Polygon<double> convex_cut(const Polygon<T>& poly, const Line<T>& l) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_4_C

	int n = sz(poly);

	Polygon<double> lpoly;

	rep(i, n) {
		// 多角形の点 poly[i] が有向直線 l の左側（l 上も含む）である場合
		T op1 = (l.second - l.first).cross(poly[i] - l.first);
		if (op1 >= 0) {
			Point<double> dp = { (double)poly[i].x, (double)poly[i].y };
			lpoly.push_back(dp);
		}

		T op2 = (l.second - l.first).cross(poly[(i + 1) % n] - l.first);
		if ((op1 > 0 && op2 < 0) || (op1 < 0 && op2 > 0)) {
			Line<T> e{ poly[i], poly[(i + 1) % n] };
			lpoly.push_back(intersection_L_L_double(l, e));
		}
	}

	return lpoly;
}


