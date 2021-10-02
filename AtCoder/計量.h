#pragma once
#include "header.h"
#include "構造(幾何).h"
// ■■■■■ 計量 ■■■■■



//【点と直線の距離】O(1)
/*
* 点 p と直線 l との距離を返す．
*/
template <typename T>
inline double distance_P_L(const Point<T>& p, const Line<T>& l) {
	Point<double> d = (l.second - l.first).normalize();
	Point<double> n = { -d.y, d.x };
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
template <typename T>
bool compare_argument(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
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
template <typename T>
void argument_sort(vector<Point<T>>& p, const Point<T>& c) {
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
template <typename T>
T area_polygon(const Polygon<T>& poly) {
	int n = sz(poly);

	T res = 0;

	rep(i, n) {
		res += poly[i].cross(poly[(i + 1) % n]);
	}

	return res / 2;
}


//【凸多角形の直径】O(n)
/*
* キャリパー法で凸 n 角形 poly の直径を求める．
*
* 凸 n 角形は頂点を反時計回りに並べた列として表す．
*
* 戻り値：poly の直径
* id = {i, j} : 直径の両端の頂点の番号が {i, j} であることを表す．
*/
template <typename T>
double convex_diameter(const Polygon<T>& poly, pii& id) {
	int n = sz(poly);

	// x 座標が最小[最大]の頂点の番号 i0[j0] を得る．
	int i0 = 0, j0 = 0;
	repi(i, 1, n - 1) {
		if (poly[i].x < poly[i0].x) {
			i0 = i;
		}
		if (poly[i].x > poly[j0].x) {
			j0 = i;
		}
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
		if (v(i, i + 1).cross(v(j, j + 1)) >= 0) {
			j++;
		}
		else {
			i++;
		}

		// 直径の候補を更新する．
		if (chmax(sqres, v(i, j).sqnorm())) {
			id.first = i % n;
			id.second = j % n;
		}
	}

	return sqrt(sqres);
}


//【最短バイトニックツアー】O(n^2)
/*
* x 座標の互いに異なる n 個の点列 p について，
*	x 座標最小の点 →(x 座標昇順)→ x 座標最大の点 →(x 座標降順)→ x 座標最小の点
* の順に点を結ぶ経路の最短長を返す．
*/
template <class T>
double minimum_bitonic_tour(vector<Point<T>>& p) {
	int n = sz(p);

	sort(all(p));

	// dp[i][j] : i から j までの最短バイトニック経路長（i < j）
	vvd dp(n, vd(n, numeric_limits<double>::max()));
	dp[0][1] = (p[1] - p[0]).norm();
	repi(j, 1, n - 1) {
		repi(i, 0, j - 1) {
			// i, j が隣り合う頂点でない場合
			if (j - i >= 2) {
				// j からは j-1 にしか繋げない．
				dp[i][j] = dp[i][j - 1] + (p[j] - p[j - 1]).norm();
				continue;
			}

			// k : j をどこに繋ぐか
			repi(k, 0, i - 1) {
				chmin(dp[i][j], dp[k][i] + (p[k] - p[j]).norm());
			}
		}
	}

	double res = numeric_limits<double>::max();
	repi(i, 0, n - 2) {
		chmin(res, dp[i][n - 1] + (p[i] - p[n - 1]).norm());
	}

	return res;
}


//【格子点の個数】O(log|x2 - x1|)
/*
* x 軸以上かつ (x1, y1) と (x2, y2) を結ぶ線分以下にある格子点の個数を返す．
* 条件：x1 != x2, y1 >= 0, y2 >= 0
*/
ll count_lattice_point_ld(ll x1, ll y1, ll x2, ll y2) {
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


