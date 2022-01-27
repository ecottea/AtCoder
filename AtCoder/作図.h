#pragma once
#include "header.h"
#include "構造(幾何).h"
// ■■■■■ 作図 ■■■■■


//【回転】O(1)
/*
* 点 p を点 c を中心に th[rad] だけ回転した点を返す．
*/
inline Point<double> rotate(const Point<double>& p, const Point<double>& c, double th) {
	Point<double> q;

	q.x = cos(th) * (p.x - c.x) - sin(th) * (p.y - c.y) + c.x;
	q.y = sin(th) * (p.x - c.x) + cos(th) * (p.y - c.y) + c.y;

	return q;
}


//【90°回転】O(1)
/*
* 点 p を点 c を中心に 90°× i だけ回転した点を返す．
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
template <typename T>
inline Line<double> corner_bisector(const Point<T>& a, const Point<T>& o, const Point<T>& b) {
	Point<double> p1 = o;
	Point<double> p2 = p1 + (a - o).normalize() + (b - o).normalize();
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
* 2 直線 l1, l2 の交点を返す．
*/
template <typename T>
inline Point<double> intersection_L_L(const Line<T>& l1, const Line<T>& l2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_2_C

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
	double x_dnm = x3 * y1 - x4 * y1 - x3 * y2 + x4 * y2
		- x1 * y3 + x2 * y3 + x1 * y4 - x2 * y4;
	double y_num = x2 * y1 * y3 - x4 * y1 * y3 - x1 * y2 * y3 + x4 * y2 * y3
		- x2 * y1 * y4 + x3 * y1 * y4 + x1 * y2 * y4 - x3 * y2 * y4;
	double y_dnm = x3 * y1 - x4 * y1 - x3 * y2 + x4 * y2
		- x1 * y3 + x2 * y3 + x1 * y4 - x2 * y4;

	return { x_num / x_dnm, y_num / y_dnm };
}


//【内接円】O(1)
/*
* 三角形 a b c の内接円を返す．
*/
template <typename T>
inline Circle<double> incircle(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
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


//【凸包】O(n log n)
/*
* n 個の点の集合 p の凸包の頂点を反時計回りに ch に格納する．
*/
template <typename T>
void convex_hull(const Polygon<T>& p, Polygon<T>& ch) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_4_A
	
	int n = (int)p.size();

	// x 座標を優先して昇順ソート（x 座標が同じなら y 座標昇順）
	sort(all(p), [](Point<T> a, Point<T> b) {
		return a.x == b.x ? a.y < b.y : a.x < b.x;
		});
	dump_array(p);

	// 凸包を成す頂点
	ch = vector<Point<T>>();

	// まず x 座標昇順に見ていき，凸包の y 座標の小さい側を得る．
	int pt = 0;
	rep(i, n) {
		// 凸でない限り直前の点を除去することを繰り返す．
		// 凸かどうかは外積を用いて判定できる．
		while (pt >= 2 && (ch[pt - 1] - ch[pt - 2]).cross(p[i] - ch[pt - 2]) < 0) {
			ch.pop_back();
			pt--;
		}

		// 今見ている点を暫定的に凸包に加える．
		ch.push_back(p[i]);
		pt++;
	}
	dump_array(ch);

	// 次に x 座標降順に見ていき，凸包の y 座標の大きい側を得る．
	repir(i, n - 2, 0) {
		// 凸でない限り直前の点を除去することを繰り返す．
		// 凸かどうかは外積を用いて判定できる．
		while (pt >= 2 && (ch[pt - 1] - ch[pt - 2]).cross(p[i] - ch[pt - 2]) < 0) {
			ch.pop_back();
			pt--;
		}

		// 今見ている点を暫定的に凸包に加える．
		ch.push_back(p[i]);
		pt++;
	}

	// p[0] が重複してしまっているので取り除く．
	ch.pop_back();
}


//【凸多角形の切断】O(n)
/*
* 凸 n 角形 poly を有向直線 l で切断した左側の凸多角形を lpoly に返す．
* 
* 利用：【2 直線の交点】
*/
template <typename T>
void convex_cut(const Polygon<T>& poly, const Line<T>& l, Polygon<double>& lpoly) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_4_C

	int n = sz(poly);

	lpoly.clear();
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
			lpoly.push_back(intersection_L_L(l, e));
		}
	}
}


