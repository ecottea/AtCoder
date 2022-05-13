#pragma once
#include "header.h"
#include "構造(幾何).h"
#include "作図.h"
// ■■■■■ 計量 ■■■■■


//【点と直線の距離】O(1)
/*
* 点 p と直線 l との距離を返す．
*/
template <class T> inline double distance_P_L(const Point<T>& p, const Line<T>& l) {
	Point<double> d = (l.second - l.first).normalize();
	Point<double> n(-d.y, d.x);
	Point<double> p2 = p - l.first;
	return p2.dot(n);
}


//【偏角の比較】O(1)
/*
* 点 a と点 b の点 c からの (-π,π] 範囲の偏角を比較する．
* a の偏角より b の偏角が大きければ true，小さければ false を返す．
* 同じ偏角のときは，a への距離より b への距離が大きければ true，さもなくば false を返す．
* c 自身の偏角は未定義だが，便宜上 +∞ とする．
*/
template <class T>
bool compare_argument(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
	// verify : https://judge.yosupo.jp/problem/sort_points_by_argument

	// もし a = c なら，a の偏角(∞) を b の偏角(≦∞)が超えることはない．
	if (a == c) {
		return false;
	}

	// もし b = c なら，a の偏角(＜∞) より b の偏角(∞) が大きい． 
	if (b == c) {
		return true;
	}

	// 以降は a, b は c と異なるものとして考えて良い．

	// 偏角が正 (0, π] の範囲にあるか
	bool posQ_a = (a.y > 0 || (a.y == 0 && a.x < 0));
	bool posQ_b = (b.y > 0 || (b.y == 0 && b.x < 0));

	// 象限に注目するだけで判定可能なケースを判定する．
	if (!posQ_a && posQ_b) {
		return true;
	}
	if (posQ_a && !posQ_b) {
		return false;
	}

	// 以降は a, b の c からの位置ベクトルの成す角は π 未満と考えて良い．

	// 半平面内であれば c からの位置ベクトルで貼られる平行四辺形の符号付き面積の符号で判定．
	ll area = (a - c).cross(b - c);
	bool res;
	if (area != 0) {
		res = area > 0;
	}
	else {
		// 同じ偏角のときは距離で判定．
		res = (a - c).sqnorm() < (b - c).sqnorm();
	}
	return res;
}


//【偏角ソート】O(n log n)
/*
* n 点のリスト p を点 c からの (-π,π] 範囲の偏角昇順にソートする．
* 同じ偏角だった場合は c からの距離昇順とする．
* c 自身の偏角は未定義だが，便宜上 +∞ とする．
* 
* 利用：【偏角の比較】
*/
template <class T> void argument_sort(vector<Point<T>>& p, const Point<T>& c) {
	// verify : https://judge.yosupo.jp/problem/sort_points_by_argument

	auto compare = [&](Point<T> a, Point<T> b) {
		return compare_argument(a, b, c);
	};
	sort(all(p), compare);
}


//【多角形の面積】O(n)
/*
* n 角形 poly の符号付き面積を返す．
*
* n 角形は頂点を並べた列として表し，反時計回りのとき面積は正とする．
* （よって頂点の周る順の判定に用いることもできる．）
*/
template <class T> T area_polygon(const Polygon<T>& poly) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_3_A

	int n = sz(poly);
	T res = 0;
	rep(i, n) res += poly[i].cross(poly[(i + 1) % n]);

	return res / 2;
}


//【円と多角形の共通部分の面積】O(n)
/*
* 円 c と n 角形 poly との共通部分の符号付き面積を返す．
* 
* 利用：【円と直線の交点】
*/
double area_intersection_C_Poly(const Circle<ll>& c, const Polygon<ll>& poly) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_H
	
	int n = sz(poly);
	Point<double> o = Point<double>(c.first);
	double r_sq = (double)pow(c.second, 2);

	// poly に c との交点を追加した多角形 npoly を作る．
	// 同時に円の中心が原点にくるように平行移動しておく．
	Polygon<double> npoly;

	rep(i, n) {
		Point<ll> p1 = poly[i], p2 = poly[(i + 1) % n];
		Point<ll> dp1 = Point<double>(p1), dp2 = Point<double>(p2);

		npoly.push_back(dp1 - o);

		Point<double> dq1, dq2;
		int cnt = intersection_C_L(c, Line<ll>(p1, p2), dq1, dq2);

		if (cnt == 2) {
			double ratio1 = (dq1 - dp1).dot(dp2 - dp1) / (dp2 - dp1).sqnorm();
			double ratio2 = (dq2 - dp1).dot(dp2 - dp1) / (dp2 - dp1).sqnorm();
			if (0 < ratio1 && ratio1 < 1) {
				if (0 < ratio2 && ratio2 < 1) {
					if (ratio1 > ratio2) swap(dq1, dq2);
					npoly.push_back(dq1 - o);
					npoly.push_back(dq2 - o);
				}
				else {
					npoly.push_back(dq1 - o);
				}
			}
			else if (0 < ratio2 && ratio2 < 1) {
				npoly.push_back(dq2 - o);
			}
		}
	}
	int m = sz(npoly);

	double res = 0;

	// 扇形または三角形の符号付き面積を足し込んでいく．
	rep(i, m) {
		Point<double> p1 = npoly[i], p2 = npoly[(i + 1) % m];
		Point<double> mid = (p1 + p2) / 2;

		// 扇形の場合
		if (mid.sqnorm() > r_sq - EPS) {
			double th = atan2(p1.cross(p2), p1.dot(p2));
			res += r_sq * th / 2;
		}
		// 三角形の場合
		else {
			res += p1.cross(p2) / 2;
		}
	}

	return res;
}


//【円と円の共通部分の面積】O(1)
/*
* 2 円 c1, c2 の共通部分の面積を返す．
*/
template <class T>
double area_intersection_C_C(const Circle<T>& c1, const Circle<T>& c2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_I

	// 円 c1, c2 の中心と半径
	Point<T> o1 = c1.first, o2 = c2.first;
	T r1 = c1.second, r2 = c2.second;

	// o1 から o2 へのベクトル，半径の差
	Point<T> d = o2 - o1;
	T r_sum = r1 + r2, r_dif = abs(r1 - r2);

	// 中心間距離が円の半径の和以上の場合 → 面積 0
	if (d.sqnorm() >= r_sum * r_sum) {
		return 0;
	}

	// 中心間距離が円の半径の差以下の場合 → 小円の面積
	if (d.sqnorm() <= r_dif * r_dif) {
		T r_min = min(r1, r2);
		return PI * r_min * r_min;
	}

	// その他の場合 → 2 つの弓形の面積の和
	double x = (r1 * r1 - r2 * r2 + d.sqnorm()) / (2 * d.norm());
	double h = sqrt(r1 * r1 - x * x);
	Point<double> nd = Point<double>(d) * (x / d.norm());
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (h / d.norm());
	Point<double> p1 = Point<double>(o1) + nd + nn;
	Point<double> p2 = Point<double>(o1) + nd - nn;

	Point<double> u1 = p1 - Point<double>(o1);
	Point<double> u2 = p2 - Point<double>(o1);
	Point<double> v1 = p1 - Point<double>(o2);
	Point<double> v2 = p2 - Point<double>(o2);
	double seg1 = abs(u1.angle(u2)) * r1 * r1 / 2 - abs(u1.cross(u2)) / 2;
	double seg2 = abs(v1.angle(v2)) * r2 * r2 / 2 - abs(v1.cross(v2)) / 2;

	double res;
	if (r1 * r1 + d.sqnorm() < r2 * r2) {
		res = (PI * r1 * r1 - seg1) + seg2;
	}
	else if (r2 * r2 + d.sqnorm() < r1 * r1) {
		res = (PI * r2 * r2 - seg2) + seg1;
	}
	else {
		res = seg1 + seg2;
	}

	return res;
}


//【凸多角形の直径】O(n)
/*
* 凸 n 角形 poly の直径を求める．
*
* 凸 n 角形は頂点を反時計回りに並べた列として表す．
*
* 戻り値：poly の直径
* id = {i, j} : 直径の両端の頂点の番号が {i, j} であることを表す．
*/
template <class T> double caliper(const Polygon<T>& poly, pii& id) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_4_B

	int n = sz(poly);

	// x 座標が最小[最大]の頂点の番号 i0[j0] を得る．
	int i0 = 0, j0 = 0;
	repi(i, 1, n - 1) {
		if (poly[i].x < poly[i0].x) i0 = i;
		if (poly[i].x > poly[j0].x) j0 = i;
	}
	T sqres = (poly[i0] - poly[j0]).sqnorm();

	// 多角形の 2 頂点 poly[i], poly[j] を結ぶベクトルを返す．
	function<Point<T>(int, int)> v = [&](int i, int j) {
		return poly[j % n] - poly[i % n];
	};

	// 尺取り法で対心点対に限って距離を調べていく．
	int i = i0, j = j0;
	id.first = i0;
	id.second = j0;
	while (i <= i0 + n || j <= j0 + n) {
		// 対心点対を結ぶ線分の回転量が少ないよう更新する
		if (v(i, i + 1).cross(v(j, j + 1)) >= 0) j++;
		else i++;

		// 直径の候補を更新する．
		if (chmax(sqres, v(i, j).sqnorm())) {
			id.first = i % n;
			id.second = j % n;
		}
	}

	return sqrt(sqres);
}


//【格子点の個数】O(log|x2 - x1|)
/*
* x 軸以上かつ (x1, y1) と (x2, y2) を結ぶ線分以下にある格子点の個数を返す．
* 条件：x1 != x2, y1 >= 0, y2 >= 0
*/
ll count_lattice_point_le(ll x1, ll y1, ll x2, ll y2) {
	if (x1 > x2) {
		swap(x1, x2);
	}
	if (y1 > y2) {
		swap(y1, y2);
	}

	ll res = (x2 - x1 + 1) * (y2 - y1 + 1);
	res += gcd(x2 - x1, y2 - y1) + 1;
	res /= 2;
	res += (x2 - x1 + 1) * y1;
	return res;
}


//【格子点の個数】O(log|x2 - x1|)
/*
* x 軸以上かつ (x1, y1) と (x2, y2) を結ぶ線分より下にある格子点の個数を返す．
* 条件：x1 != x2, y1 >= 0, y2 >= 0
*/
ll count_lattice_point_l(ll x1, ll y1, ll x2, ll y2) {
	if (x1 > x2) {
		swap(x1, x2);
	}
	if (y1 > y2) {
		swap(y1, y2);
	}

	ll res = (x2 - x1 + 1) * (y2 - y1 + 1);
	res -= gcd(x2 - x1, y2 - y1) + 1;
	res /= 2;
	res += (x2 - x1 + 1) * y1;
	return res;
}


