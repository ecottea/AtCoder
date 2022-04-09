#pragma once
#include "header.h"
#include "構造(幾何).h"
// ■■■■■ 作図 ■■■■■


//【凸包】O(n log n)
/*
* 点群 p[0..n) の凸包の頂点を反時計回りに ch に格納する．
* p[0] は x 座標最小（同じものがあれば y 座標最小）の点とする．
*/
template <typename T> void convex_hull(vector<Point<T>> p, Polygon<T>& ch) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_4_A

	int n = sz(p);

	// x 座標を優先して昇順ソート（x 座標が同じなら y 座標昇順）
	sort(all(p));

	// 凸包を成す頂点
	ch.clear();

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


//【上からの凸包】O(n log n)
/*
* n 個の点 (x[i], y[i]) の上からの凸包の x 座標について昇順で j 番目の頂点の座標を p[j] に格納する．
* また p の大きさを返す．strict = false とすると広義凸包，upper = false とすると下からの凸包を格納する．
*/
template <class T>
int upper_convex_hull(const vector<T>& x, const vector<T>& y, vector<pair<T, T>>& p,
	bool strict = false, bool upper = true)
{
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_4_A

	int n = sz(x);
	p.clear();

	// x 座標について昇順に並べる．
	map<T, T> x_to_y;
	rep(i, n) {
		if (x_to_y.count(x[i])) {
			if (upper) chmax(x_to_y[x[i]], y[i]);
			else chmin(x_to_y[x[i]], y[i]);
		}
		else x_to_y[x[i]] = y[i];
	}

	// 3 点 P, Q, R のの部分が上に凸かを返す．
	//		P, Q, R 部分が上に凸
	//		⇔ 直線 PQ の傾き > 直線 PR の傾き
	//		⇔ (Q[y] - P[y]) / (Q[x] - P[x]) > (R[y] - P[y]) / (R[x] - P[x])
	//		⇔ (Q[y] - P[y]) (R[x] - P[x]) > (R[y] - P[y]) (Q[x] - P[x])
	using PT = pair<T, T>;
	function<bool(const PT&, const PT&, const PT&)> convexQ = [&](const PT& P, const PT& Q, const PT& R) {
		T left = (Q.second - P.second) * (R.first - P.first);
		T right = (R.second - P.second) * (Q.first - P.first);

		if (!strict && left == right) return true;

		return upper ? left > right : left < right;
	};

	int pt = 0;
	repe(xy, x_to_y) {
		// 凸包の直前の 2 点を P, Q，今見ている点を R とし，
		// P, Q, R 部分が上に凸でない限り Q を除去することを繰り返す．
		while (pt >= 2 && !convexQ(p[pt - 2], p[pt - 1], xy)) {
			p.pop_back();
			pt--;
		}

		// 今見ている点を暫定的に凸包に加える．
		p.push_back(xy);
		pt++;
	}

	return pt;
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


