#pragma once
#include "header.h"
#include "構造(幾何).h"
#include "分析(文字列).h"
#include "順列.h"
#include "探索.h"
#include "作図.h"
#include "計算.h"
#include "括弧列.h"
#include "座標圧縮.h"
#include "累積演算.h"
// ■■■■■ 点群，線分群，直線群など ■■■■■


//【点群の合同判定】O(|s| log|s|)　
/*
* 点の集合 s, t が反転なしで合同かどうかを返す．
*
* 利用：【偏角ソート】,【部分文字列判定】
*/
template <typename T>
bool congruenceQ(vector<Point<T>> s, vector<Point<T>> t) {
	// verify : https://atcoder.jp/contests/abc207/tasks/abc207_d

	int n = sz(s);

	// 点の数が違うならもちろん合同ではない．
	if (sz(t) != n) return false;

	// 原点中心に n 倍拡大した上で重心を求める．
	Point<T> gs, gt;
	rep(i, n) {
		gs += s[i];
		s[i] *= n;

		gt += t[i];
		t[i] *= n;
	}

	// 重心が原点にくるよう平行移動を行う．
	rep(i, n) {
		s[i] -= gs;
		t[i] -= gt;
	}

	// 原点の周りで偏角ソートを行う．
	Point<T> o;
	argument_sort(s);
	argument_sort(t);

	// 原点に点が存在した場合の例外処理
	// o は便宜上偏角最大としていたので，存在するなら配列の末尾にある．
	if (*s.rbegin() == o) {
		if (*t.rbegin() == o) {
			// 両者が原点を含んでいたなら，それらを取り除く．
			s.pop_back();
			t.pop_back();
			n--;
		}
		else {
			// s のみが原点を含んでいたなら一致しない．
			return false;
		}
	}
	else {
		// t のみが原点を含んでいたなら一致しない．
		if (*t.rbegin() == o) return false;
	}

	// 1 点のみだった場合の例外処理（もちろん合同）
	if (s.empty()) return true;

	// 原点回りの三角形の辺の長さと夾角の大きさの情報を格納したリスト
	// 実際には長さの代わりに 2 乗ノルム，夾角の大きさの代わりに余弦と正弦，
	// さらにその代わりに内積と外積を用いる．
	// （内積だけだと，例えば対称移動させた直角三角形で WA してしまう．）
	vector<T> tri_s, tri_t;
	s.push_back(s[0]);
	t.push_back(t[0]);
	rep(i, n) {
		tri_s.push_back(s[i].sqnorm());
		tri_s.push_back(s[i].dot(s[i + 1]));
		tri_s.push_back(s[i].cross(s[i + 1]));

		tri_t.push_back(t[i].sqnorm());
		tri_t.push_back(t[i].dot(t[i + 1]));
		tri_t.push_back(t[i].cross(t[i + 1]));
	}

	// tri_t をコピーし 2 倍に延長することで周期境界を扱いやすくする．
	rep(i, 3 * n) tri_t.push_back(tri_t[i]);

	// tri_s を 2 * tri_t が連続部分列として含んでいるかをチェックする．
	vi pos = knuth_morris_pratt(tri_t, tri_s);

	return !pos.empty();
}


//【点群の相似判定】O(|s| log|s|)
/*
* 点の集合 s, t が反転なしで相似であれば，s から t への倍率を返す（相似でなければ -1 を返す）
*
* 利用：【偏角ソート】,【部分文字列判定】
*/
template <typename T>
double similarityQ(vector<Point<T>> s, vector<Point<T>> t) {
	// verify : https://atcoder.jp/contests/abc022/tasks/abc022_d

	int n = sz(s);

	// 点の数が違うならもちろん相似ではない．
	if (sz(t) != n) return -1;

	// 原点中心に n 倍拡大した上で重心を求める．
	Point<T> gs, gt;
	rep(i, n) {
		gs += s[i];
		s[i] *= n;

		gt += t[i];
		t[i] *= n;
	}

	// 重心が原点にくるよう平行移動を行う．
	rep(i, n) {
		s[i] -= gs;
		t[i] -= gt;
	}

	// 原点の周りで偏角ソートを行う．
	Point<T> o;
	argument_sort(s);
	argument_sort(t);

	// 原点に点が存在した場合の例外処理
	// o は便宜上偏角最大としていたので，存在するなら配列の末尾にある．
	if (*s.rbegin() == o) {
		if (*t.rbegin() == o) {
			// 両者が原点を含んでいたなら，それらを取り除く．
			s.pop_back();
			t.pop_back();
			n--;
		}
		else {
			// s のみが原点を含んでいたなら相似ではない．
			return -1;
		}
	}
	else {
		// t のみが原点を含んでいたなら相似ではない．
		if (*t.rbegin() == o) return -1;
	}

	// 1 点のみだった場合の例外処理（もちろん相似）
	if (s.empty()) return 1;

	// 原点回りの三角形の二辺比と夾角の大きさの情報を格納したリスト
	// 実際には二辺比の代わりに 2 乗ノルムの比，夾角の大きさの代わりに余弦と正弦，
	// さらにその代わりに内積と外積の比を用いる．
	vector<T> tri_s, tri_t;
	s.push_back(s[0]);
	t.push_back(t[0]);
	rep(i, n) {
		T sqnorm_s = s[i].sqnorm();
		T sqnorm_s2 = s[i + 1].sqnorm();
		T g_s = gcd(sqnorm_s, sqnorm_s2);
		tri_s.push_back(sqnorm_s / g_s);
		tri_s.push_back(sqnorm_s2 / g_s);

		T in_s = s[i].dot(s[i + 1]);
		T out_s = s[i].cross(s[i + 1]);
		g_s = gcd(abs(in_s), abs(out_s));
		tri_s.push_back(in_s / g_s);
		tri_s.push_back(out_s / g_s);

		T sqnorm_t = t[i].sqnorm();
		T sqnorm_t2 = t[i + 1].sqnorm();
		T g_t = gcd(sqnorm_t, sqnorm_t2);
		tri_t.push_back(sqnorm_t / g_t);
		tri_t.push_back(sqnorm_t2 / g_t);

		T in_t = t[i].dot(t[i + 1]);
		T out_t = t[i].cross(t[i + 1]);
		g_t = gcd(abs(in_t), abs(out_t));
		tri_t.push_back(in_t / g_t);
		tri_t.push_back(out_t / g_t);
	}

	// tri_t をコピーし 2 倍に延長することで周期境界を扱いやすくする．
	rep(i, 4 * n) tri_t.push_back(tri_t[i]);

	// 2 * tri_t が tri_s を連続部分列として含んでいるかをチェックする．
	vi pos = knuth_morris_pratt(tri_t, tri_s);

	double res = -1;
	if (!pos.empty()) {
		int i = pos[0] / 4;
		res = t[i].norm() / s[0].norm();
	}

	return res;
}


//【点群の作る三角形の数え上げ】O(n^2 log n)　
/*
* 点群 p[0..n) から 3 点を選んで作れる鋭角[直角, 鈍角]三角形の個数を cp[c0, cn] とし，{cp,c0,cn} を返す．
*
* 制約：どの 3 点も共線でない．
*
* 利用：【偏角の比較（同偏角は同一視）】
*/
template <class T>
tuple<ll, ll, ll> count_triangles(const vector<Point<T>>& p) {
	// verify : https://atcoder.jp/contests/abc033/tasks/abc033_d

	int n = sz(p);

	// 原点からみた偏角で点を比較する関数．
	auto cmp = [&](const Point<ll>& a, const Point<ll>& b) {
		return compare_argument_weakly(a, b, { 1, 0 }, { 0, 0 });
	};

	// c0 : 直角三角形の個数，cn : 鈍角三角形の個数
	ll c0 = 0, cn = 0;

	// 点 p[i] 周りの角度を調べる．
	rep(i, n) {
		// v[0..n-1) : p[i] 以外の点全てについて，点 p[i] からの位置ベクトルを偏角順にソートしたもの．
		vector<Point<ll>> v;
		rep(j, n) if (j != i) v.emplace_back(p[j] - p[i]);
		sort(all(v), cmp);

		// 辺 v[j] を含むものを調べる．
		rep(j, n - 1) {
			Point<ll> e{ -v[j].y, v[j].x };

			// v[j] を 90° 回転した方向 e を調査する．
			auto it1 = lower_bound(all(v), e, cmp);
			int j1 = (int)distance(v.begin(), it1) % (n - 1);
			if (v[j].dot(v[j1]) == 0 && v[j].cross(v[j1]) > 0) {
				c0++;
				j1 = (j1 + 1) % n;
			}

			// v[j] を -90° 回転した方向 -e を調査する．
			auto it2 = lower_bound(all(v), -e, cmp);
			int j2 = (int)distance(v.begin(), it2) % (n - 1);
			if (v[j].dot(v[j2]) == 0 && v[j].cross(v[j2]) < 0) {
				c0++;
			}

			// 偏角が e から -e までの点を 3 点目に選ぶと鈍角三角形ができる．
			cn += smod(j2 - j1, n - 1);
		}
	}

	// 角を挟む辺それぞれに対して数えてしまっているので 2 で割る．
	c0 /= 2; cn /= 2;

	// cp : 鋭角三角形の個数
	ll cp = (ll)n * (n - 1) * (n - 2) / 6 - (c0 + cn);

	return { cp, c0, cn };
}


//【凸包】O(n log n)
/*
* 点群 p[0..n) の凸包の頂点を反時計回りに ch に格納する．
* p[0] は x 座標最小（同じものがあれば y 座標最小）の点とする．
*/
template <typename T>
void convex_hull(vector<Point<T>> p, Polygon<T>& ch) {
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
	bool strict = true, bool upper = true)
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


//【線分群の交点（軸平行）】O(n log n)
/*
* 2 点 (x1[i], y1[i]), (x2[i], y2[i]) を結ぶ n 本の閉線分の共有点の数を返す．
*
* 制約：線分は軸平行，互いに平行な線分同士は共有点をもたない．
*/
ll count_intersection(const vl& x1, const vl& y1, const vl& x2, const vl& y2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_6_A

	int n = sz(x1);

	// 各線分が x1 <= x2, y1 <= y2 を満たすようにしつつ，x1 ごとに昇順に記録する．
	// 線分が x 軸平行のとき type = 0，y 軸平行のとき type = 1 とし，
	// 同じ x1 をもつ線分同士については type 0 が先，type 1 が後に並ぶようにする．
	map<ll, set<tuple<int, ll, ll>>> seg;
	rep(i, n) {
		if (x1[i] != x2[i]) {
			seg[min(x1[i], x2[i])].insert({ 0, max(x1[i], x2[i]), y2[i] });
		}
		else {
			seg[x1[i]].insert({ 1, min(y1[i], y2[i]), max(y1[i], y2[i]) });
		}
	}

	ll res = 0;

	// 走査中の x 座標と交点をもつような x 軸平行な線分の y 座標を記録するテーブル．
	// 二分探索を利用するため vector でなく set を用いている．
	set<ll> sx;

	// x 軸平行な線分の終端まで走査したらそれをテーブルから取り除くために，
	// 終端の x 座標について昇順に取り出すことのできる優先度付きキュー．
	priority_queue_rev<pll> q;

	// x 座標について昇順に走査していく．
	// 実際に幅の分だけ走査するのではなく，x1 の値となっているものだけを走査する．
	repe(p, seg) {
		// x 軸平行な線分のうち x2 が走査位置の手前であるものをテーブルから取り除く．
		while (!q.empty() && (q.top().first < p.first)) {
			sx.erase(q.top().second);
			q.pop();
		}

		// x1 が同じものを順に見ていく．
		// まずは type 0 が処理され，その後 type 1 が処理される．
		repe(s, p.second) {
			int type; ll x2_or_y1, y2;
			tie(type, x2_or_y1, y2) = s;

			// x 軸平行な線分の場合
			if (type == 0) {
				// 走査中の x 軸平行な線分のテーブルに追加する．
				sx.insert(y2);

				// 終端まで走査したときにテーブルから削除するためのキューに登録する．
				q.push({ x2_or_y1, y2 });
			}
			// y 軸平行な線分の場合
			else {
				// 走査中の x 軸平行な線分のテーブルから，
				// y 座標が y1 以上 y2 以下のものを数える．
				auto it1 = sx.lower_bound(x2_or_y1);
				auto it2 = sx.upper_bound(y2);
				res += (ll)distance(it1, it2);
			}
		}
	}

	return res;
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


//【直線群の交点数（左半平面）】
/*
* Line_intersections_lhplane(vl a, vl b, vl c, ll inf) : O(n log n)
*	n 本の直線 a[i] x + b[i] y = c[i] で初期化する．
*	全ての交点は (-inf, inf)×(-inf, inf) に収まっているものとする．
*
* ll count(int|ll r) : O(n log n)
*	x 座標が r より小さい交点の数を返す．
*
* ll count(Frac r) : O(n log n)
*	x 座標が r より小さい交点の数を返す（オーバーフローに注意）
*
* ll count(double r) : O(n log n)
*	x 座標が r より小さい交点の数を返す（誤差に注意）
*
* 利用：【転倒数】，【有理数】
*/
class Line_intersections_lhplane {
	// y 軸平行でない直線数
	int n;

	// 全ての交点は (-inf, inf)×(-inf, inf) に収まっている
	ll inf;

	// y 軸と非平行な直線のみを x = -inf における y 座標について昇順に並べ替えたときの
	// i 番目の直線が a[i] x + b[i] y = c[i] (b[i] != 0) であることを表す．
	vl a, b, c;

	// y 軸と平行な直線の x 座標の昇順列
	vector<Frac> xs;
	vd xs_d;

public:
	// 直線群 a[i] x + b[i] y = c[i] で初期化する．
	Line_intersections_lhplane(const vl& a_, const vl& b_, const vl& c_, ll inf_) : n(0), inf(inf_) {
		// verify : https://atcoder.jp/contests/tenka1-2017/tasks/tenka1_2017_e

		// x = -inf における点 i の y 座標を，{y, i} の形で昇順に並べたもの（y 軸非平行な直線のみ）
		vector<pair<Frac, int>> yi;

		rep(i, sz(a_)) {
			// 直線 i が y 軸平行でない場合
			if (b_[i] != 0) {
				yi.emplace_back(Frac{ c_[i] - a_[i] * (-inf), b_[i] }, i);
				n++;
			}
			// 直線 i が y 軸平行である場合
			else {
				xs.emplace_back(c_[i], a_[i]);
			}
		}

		sort(all(yi));
		sort(all(xs));
		repe(x, xs) xs_d.emplace_back(x);

		repe(tmp, yi) {
			int i = tmp.second;
			a.emplace_back(a_[i]);
			b.emplace_back(b_[i]);
			c.emplace_back(c_[i]);
		}
	}

	// x 座標が r より小さい交点の数を返す．
	ll count(const Frac& r) {
		// y[i] : i 番目の直線の x = r における y 座標
		vector<Frac> y(n);
		rep(i, n) y[i] = (c[i] - a[i] * r) / b[i];

		// y 座標の転倒数が交点の個数となる．
		ll res = inversion_number(y);

		// y 軸平行な直線との交点の個数を追加する．
		res += lbpos(xs, r) * (ll)n;

		return res;
	}
	ll count(ll r) { return count(Frac(r)); }
	ll count(int r) { return count(Frac(r)); }

	// x 座標が r より小さい交点の数を返す（誤差注意）
	ll count(double r) {
		// verify : https://atcoder.jp/contests/tenka1-2017/tasks/tenka1_2017_e

		// y[i] : i 番目の直線の x = r における y 座標
		vd y(n);
		rep(i, n) y[i] = (c[i] - a[i] * r) / b[i];

		// y 座標の転倒数が交点の個数となる．
		ll res = inversion_number(y);

		// y 軸平行な直線との交点の個数を追加する．
		res += lbpos(xs_d, r) * (ll)n;

		return res;
	}
};


//【直線群の交点数（円盤）】
/*
* Line_intersections_disk(vl a, vl b, vl c) : O(n log n)
*	n 本の直線 a[i] x + b[i] y = c[i] で初期化する．
*
* ll count(double r) : O(n log n)
*	原点からの距離が r より小さい交点の数を返す（誤差に注意）
*
* 利用：【円と直線の交点（実数）】,【色付き括弧列の違反対の数え上げ】
*/
class Line_intersections_disk {
	// 直線の数
	int n;

	// ls[i] : i 番目の直線
	vector<Line<double>> ls;

public:
	// 直線群 a[i] x + b[i] y = c[i] で初期化する．
	Line_intersections_disk(const vl& a, const vl& b, const vl& c) : n(sz(a)), ls(n) {
		// verify : https://atcoder.jp/contests/abc263/tasks/abc263_h

		rep(i, n) {
			if (abs(b[i]) > abs(a[i])) {
				ls[i] = { { -1, (double)(c[i] + a[i]) / b[i] }, { 1, (double)(c[i] - a[i]) / b[i] } };
			}
			else {
				ls[i] = { { (double)(c[i] + b[i]) / a[i], -1 }, { (double)(c[i] - b[i]) / a[i], 1 } };
			}
		}
	}

	// 原点からの距離が r より小さい交点の数を返す（誤差注意）
	ll count(double r) {
		// verify : https://atcoder.jp/contests/abc263/tasks/abc263_h

		vector<pair<double, int>> thi; int id = 0;
		Circle<double> c({ 0, 0 }, r);

		rep(i, n) {
			Point<double> p1, p2;
			int cnt = intersection_C_L(c, ls[i], p1, p2);

			if (cnt == 0) continue;

			thi.emplace_back(atan2(p1.y, p1.x), id);
			thi.emplace_back(atan2(p2.y, p2.x), id);
			id++;
		}

		sort(all(thi));

		vi p;
		repe(tmp, thi) p.emplace_back(tmp.second);

		return count_illegal_parenthesis_pair(p);
	}
};


//【一定距離以内の点対の列挙】O(?)
/*
* n 個の点 (x[i], y[i]) からなる対で距離 d 以内にあるものを res に格納する．
* 点対は添字の対 (i, j)（i < j）として表し，辞書順に並べるものとする．
* 全探索できる点対の個数の上限を lim に与える．
* 
* 利用：【回転】
*/
void enumerate_point_pair(const vl& x, const vl& y, ll d, vector<pii>& res, ll lim = (ll)1e8) {
	// 参考 : https://wata-orz.hatenadiary.org/entry/20111218/1324226179
	// verify : https://atcoder.jp/contests/abc234/tasks/abc234_h
	
	int n = sz(x);

	// 乱数生成器
	static mt19937_64 mt64((int)time(NULL));
	uniform_real_distribution<double> rnd(0., 2 * PI);

	while (true) {
		// 点群をランダムな角度で回転させ，x 座標昇順にソートする．
		vector<pair<Point<double>, int>> pi(n);

		const Point<double> o(0, 0);
		double th = rnd(mt64);
		rep(i, n) {
			Point<double> p((double)x[i], (double)y[i]);
			pi[i] = { rotate(p, o, th), i };
		}
		sort(all(pi));

		int l, r;

		// sum : x 座標の差が d 以内である点対の個数
		ll sum = 0;

		// 尺取法で x 座標の差が d 以内である点対を数える．
		l = 0, r = 0;
		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d + EPS) {
				sum += r - l;
				r++;
			}
			else {
				l++;
			}
		}

		// 調べるべき点対が多すぎるなら回転角をランダムに決め直す．
		if (sum > lim) continue;

		res.clear();

		// 尺取法で x 座標の差が d 以内である点対を列挙し，精密に距離を求める．
		l = 0; r = 0;
		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d + EPS) {
				repi(i, l, r - 1) {
					int jr = pi[r].second;
					int ji = pi[i].second;
					if (ji > jr) swap(ji, jr);

					Point<ll> e(x[ji] - x[jr], y[ji] - y[jr]);
					if (e.sqnorm() <= d * d) {
						res.push_back({ ji, jr });
					}
				}

				r++;
			}
			else {
				l++;
			}
		}
		sort(all(res));

		return;
	}
}


//【最近点対】O(?)（TODO : 嘘解法なのでやり直す）
/*
* n 個の点 (x[i], y[i]) について，最も近い 2 点の距離を返す．
* またその点対が i 番目と j 番目であることを ps = {i, j} として格納する．
* 全探索できる点対の個数の上限を lim に与える．
*
* 利用：【回転】
*/
template <class T>
double recent_point_pair(const vector<T>& x, const vector<T>& y, pii* ps = nullptr, ll lim = (ll)1e8) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_5_A
	
	int n = sz(x);

	// 乱数生成器
	static mt19937_64 mt64((int)time(NULL));
	uniform_real_distribution<double> rnd(0., 2 * PI);

	// x 座標の差が d 以内の点対に絞って精密に距離を求める．
	double d = 1;

	while (true) {
		// 点群をランダムな角度で回転させ，x 座標昇順にソートする．
		vector<pair<Point<double>, int>> pi(n);

		const Point<double> o(0, 0);
		double th = rnd(mt64);
		rep(i, n) {
			Point<double> p(x[i], y[i]);
			pi[i] = { rotate(p, o, th), i };
		}
		sort(all(pi));

		int l, r;

		// sum : x 座標の差が d 以内である点対の個数
		ll sum = 0;

		// 尺取法で x 座標の差が d 以内である点対を数える．
		l = 0, r = 0;
		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d) {
				sum += r - l;
				r++;
			}
			else {
				l++;
			}
		}

		// 該当する点対が存在しないなら，d を大きくし，回転角をランダムに決め直す．
		if (sum == 0) {
			d *= 2;
			continue;
		}

		// 調べるべき点対が多すぎるなら，d を小さくし，回転角をランダムに決め直す．
		if (sum > lim) {
			d /= 2;
			continue;
		}

		// 尺取法で x 座標の差が d 以内である点対を列挙し，精密に距離を求める．
		l = 0; r = 0;
		double res = (double)INFL;

		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d) {
				repi(i, l, r - 1) {
					int jr = pi[r].second;
					int ji = pi[i].second;
					if (ji > jr) swap(ji, jr);

					Point<double> e(x[ji] - x[jr], y[ji] - y[jr]);
					if (chmin(res, e.norm())) {
						if (ps != nullptr) {
							*ps = { ji, jr };
						}
					}
				}

				r++;
			}
			else {
				l++;
			}
		}

		// 最短距離が d を超えていたなら，d を大きくし，回転角をランダムに決め直す．
		if (res > d) {
			d *= 2;
			continue;
		}

		return res;
	}
}


//【2 点間マンハッタン距離の最大値】O(n)
/*
* n 個の点 (x[i], y[i]) から選んだ 2 点のマンハッタン距離の最大値を返す．
*/
ll maximize_manhattan_distance(const vl& x, const vl& y) {
	// verify : https://atcoder.jp/contests/abc178/tasks/abc178_e

	//【方法】
	// 点 (x1, y1) より点 (x2, y2) が右下にある場合，マンハッタン距離は
	//		(x2 - x1) + (y2 - y1) = (x2 + y2) - (x1 + y1)
	// とも表される．点 (x1, y1) より点 (x2, y2) が左下にある場合，マンハッタン距離は
	//		(x2 - x1) - (y2 - y1) = (x2 - y2) - (x1 - y1)
	// とも表される．2 点の位置関係はこれらのうちいずれかなので，
	//		max(x[i] + y[i]) - min(x[i] + y[i])
	//		max(x[i] - y[i]) - min(x[i] - y[i])
	// のうち大きい方が答えである．

	int n = sz(x);

	ll s_max = -INFL, s_min = INFL, d_max = -INFL, d_min = INFL;
	rep(i, n) {
		chmax(s_max, x[i] + y[i]);
		chmin(s_min, x[i] + y[i]);
		chmax(d_max, x[i] - y[i]);
		chmin(d_min, x[i] - y[i]);
	}

	ll res = max(s_max - s_min, d_max - d_min);

	return res;
}


//【2 点間距離の最大値と円被覆】
/*
* 距離がユークリッド距離，マンハッタン距離，チェス盤距離のいずれかであるとする．
* 点群 S 内の任意の 2 点の距離が 2r 以下のとき，S は半径 r の閉円盤で被覆される．  
* verify : https://atcoder.jp/contests/joi2010ho/tasks/joi2010ho_d
* 
* ハニカム格子上の 6 近傍距離ではそのような被覆は存在するとは限らない．
* verify : https://atcoder.jp/contests/abc280/tasks/abc280_g
*/


//【直径 d 以下の部分集合の数え上げ（2 次元，マンハッタン距離）】O(n^2)
/*
* i 番目の点が (x[i], y[i]) である大きさ n の点群の部分集合のうち，
* マンハッタン距離で測った直径が d 以下であるものの個数を返す（空集合含む）
*
* 利用：【座標圧縮】，【二次元累積和（長方形）】
*/
template <class T>
mint count_manhattan_distance_clique_2D(const vector<T>& x, const vector<T>& y, T d) {
	int n = sz(x);

	// 45° 回転させ，代わりにチェス盤距離で測ることにする．
	vector<T> u(n), v(n);
	rep(i, n) {
		u[i] = x[i] + y[i];
		v[i] = x[i] - y[i];
	}

	vi u_cp, v_cp; vector<T> xs, ys;
	int h = coordinate_compression(u, u_cp, &xs);
	int w = coordinate_compression(v, v_cp, &ys);
	
	vvi cnt(h, vi(w));
	rep(i, n) cnt[u_cp[i]][v_cp[i]]++;
	Cumulative_sum_2D<int> acc(cnt);
	
	vm pow2(n + 1);
	pow2[0] = 1;
	rep(i, n) pow2[i + 1] = pow2[i] * 2;

	mint res = 1; // 空集合を先に数えておく．

	// BB の右下隅（内部）の座標 (i, j) で場合分けしながら二次元尺取法で数え上げていく．
	// (i0, j0) : 左上隅として許せる限界（内部）
	int i0 = 0;
	rep(i, h) {
		while (xs[i] - xs[i0] > d) i0++;

		int j0 = 0;
		rep(j, w) {
			while (ys[j] - ys[j0] > d) j0++;

			// (i, j) の要素を選ぶ場合
			mint add1 = pow2[cnt[i][j]] - 1;
			add1 *= pow2[acc.sum(i0, j0, i + 1, j + 1) - cnt[i][j]];
			res += add1;

			// (i, j) の要素を選ばない場合
			mint add0 = pow2[acc.sum(i, j0, i + 1, j)] - 1;
			add0 *= pow2[acc.sum(i0, j, i, j + 1)] - 1;
			add0 *= pow2[acc.sum(i0, j0, i, j)];
			res += add0;
		}
	}

	return res;
}


//【直径 D 以下の部分集合の数え上げ（3 次元，チェビシェフ距離）】O(n^3)
/*
* i 番目の点が (x[i], y[i], z[i]) である大きさ n の点群の部分集合のうち，
* チェビシェフ距離で測った直径が D 以下であるものの個数を返す（空集合含む）
*
* 利用：【座標圧縮】，【三次元累積和（直方体）】
*/
template <class T>
mint count_chebyshev_distance_clique_3D(const vector<T>& x, const vector<T>& y, const vector<T>& z, T D) {
	// verify : https://atcoder.jp/contests/abc280/tasks/abc280_g

	int n = sz(x);

	vi x_cp, y_cp, z_cp; vector<T> xs, ys, zs;
	int h = coordinate_compression(x, x_cp, &xs);
	int w = coordinate_compression(y, y_cp, &ys);
	int d = coordinate_compression(z, z_cp, &zs);
	dump(xs); dump(ys); dump(zs);

	vvvi cnt(h, vvi(w, vi(d)));
	rep(i, n) cnt[x_cp[i]][y_cp[i]][z_cp[i]]++;
	dumpel(cnt);

	Cumulative_sum_3D<int> acc(cnt);

	vm pow2(n + 1);
	pow2[0] = 1;
	rep(i, n) pow2[i + 1] = pow2[i] * 2;

	mint res = 1; // 空集合を先に数えておく．

	// BB の最大隅（内部）の座標 (i, j, k) で場合分けしながら数え上げていく．
	// (i0, j0, k0) : 最小隅として許せる限界（内部）
	int i0 = 0;
	rep(i, h) {
		while (xs[i] - xs[i0] > D) i0++;

		int j0 = 0;
		rep(j, w) {
			while (ys[j] - ys[j0] > D) j0++;

			int k0 = 0;
			rep(k, d) {
				while (zs[k] - zs[k0] > D) k0++;

				dump("------", i0, i, j0, j, k0, k, "-------");

				// 無条件の場合
				res += pow2[acc.sum(i0, j0, k0, i + 1, j + 1, k + 1)];
				dump(res);
				// BB の最大隅条件を，少なくとも 1 個の座標が満たしていない場合
				res -= pow2[acc.sum(i0, j0, k0, i, j + 1, k + 1)];
				res -= pow2[acc.sum(i0, j0, k0, i + 1, j, k + 1)];
				res -= pow2[acc.sum(i0, j0, k0, i + 1, j + 1, k)];
				dump(res);
				// BB の最大隅条件を，少なくとも 2 個の座標が満たしていない場合
				res += pow2[acc.sum(i0, j0, k0, i, j, k + 1)];
				res += pow2[acc.sum(i0, j0, k0, i + 1, j, k)];
				res += pow2[acc.sum(i0, j0, k0, i, j + 1, k)];
				dump(res);
				// BB の最大隅条件を，少なくとも 3 個の座標が満たしていない場合
				res -= pow2[acc.sum(i0, j0, k0, i, j, k)];

				dump(res);
			}
		}
	}

	return res;
}


//【狭義単調な点列】
/*
* Monotonous_points<T>(bool y_greater = true, T inf = max(T)/2) : O(1)
*	空で初期化する．x 座標は狭義単調増加で，y 座標は y_greater=true[false] なら狭義単調増加[減少]とする．
*
* void insert(T x, T y) : ならし O(log n)
*	点 (x, y) を挿入し，それにより単調性に違反する点は全て削除する．
*
* bool find_LL(T x, T y, bool strict = true) : O(log n)
*	x' < x かつ y' < y なる点 (x', y') が存在するかを返す（strict=false なら等号も許す）
*
* bool find_LG(T x, T y, bool strict = true) : O(log n)
*	x' < x かつ y' > y なる点 (x', y') が存在するかを返す（strict=false なら等号も許す）
*
* bool find_GL(T x, T y, bool strict = true) : O(log n)
*	x' > x かつ y' < y なる点 (x', y') が存在するかを返す（strict=false なら等号も許す）
*
* bool find_GG(T x, T y, bool strict = true) : O(log n)
*	x' > x かつ y' > y なる点 (x', y') が存在するかを返す（strict=false なら等号も許す）
*
* pTT lower_bound(T x) : O(log n)
*	x' >= x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
*
* pTT upper_bound(T x) : O(log n)
*	x' > x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
*
* pTT lower_bound_rev(T x) : O(log n)
*	x' <= x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
*
* pTT upper_bound_rev(T x) : O(log n)
*	x' < x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
*/
template <class T>
struct Monotonous_points {
	// 参考 : https://topcoder-g-hatena-ne-jp.jag-icpc.org/skyaozora/20141216.html

	bool y_greater; // y 座標について狭義単調増加か
	T inf; // 無限大

	// x 座標は狭義単調増加で，y 座標は y_greater=true[false] なら狭義単調増加[減少] な点列
	map<T, T> x_to_y;

	// 空で初期化する．x 座標は狭義単調増加で，y 座標は y_greater=true[false] なら狭義単調増加[減少]とする．
	Monotonous_points(bool y_greater = true, T inf_ = -1) : y_greater(y_greater) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		inf = (inf_ == -1 ? numeric_limits<T>::max() / 2 : inf_);

		// 番兵を挿入しておく．
		if (y_greater) { x_to_y[-inf] = -inf; x_to_y[inf] = inf; }
		else { x_to_y[-inf] = inf; x_to_y[inf] = -inf; }
	}

	// 点 (x, y) を挿入し，単調性に違反する点は全て削除する．
	void insert(T x, T y) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		// x <= x' なる最小の x' を指すイテレータを得る．
		auto it = x_to_y.lower_bound(x);

		// x' から昇順に，y' <= y[ y' >= y ] である限り要素を削除する．
		if (y_greater) {
			while (true) {
				if (it->second > y) break;
				it = x_to_y.erase(it);
			}
		}
		else {
			while (true) {
				if (it->second < y) break;
				it = x_to_y.erase(it);
			}
		}

		// x' から降順に，y' >= y[ y' <= y ] である限り要素を削除する．
		if (y_greater) {
			while (true) {
				it = prev(it);
				if (it->second < y) break;
				it = x_to_y.erase(it);
			}
		}
		else {
			while (true) {
				it = prev(it);
				if (it->second > y) break;
				it = x_to_y.erase(it);
			}
		}

		// 点 (x, y) を挿入する．
		x_to_y[x] = y;
	}

	// x' < x かつ y' < y なる点が存在するかを返す（strict=false なら等号も許す）
	bool find_LL(T x, T y, bool strict = true) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/1341

		if (strict) {
			T y2 = prev(x_to_y.lower_bound(x))->second;
			return y2 != -inf && y2 < y;
		}
		else {
			T y2 = prev(x_to_y.upper_bound(x))->second;
			return y2 != -inf && y2 <= y;
		}
	}

	// x' < x かつ y' > y なる点が存在するかを返す（strict=false なら等号も許す）
	bool find_LG(T x, T y, bool strict = true) {
		if (strict) {
			T y2 = prev(x_to_y.lower_bound(x))->second;
			return y2 != inf && y2 > y;
		}
		else {
			T y2 = prev(x_to_y.upper_bound(x))->second;
			return y2 != inf && y2 >= y;
		}
	}

	// x' > x かつ y' < y なる点が存在するかを返す（strict=false なら等号も許す）
	bool find_GL(T x, T y, bool strict = true) {
		if (strict) {
			T y2 = x_to_y.upper_bound(x)->second;
			return y2 != -inf && y2 < y;
		}
		else {
			T y2 = x_to_y.lower_bound(x)->second;
			return y2 != -inf && y2 <= y;
		}
	}

	// x' > x かつ y' > y なる点が存在するかを返す（strict=false なら等号も許す）
	bool find_GG(T x, T y, bool strict = true) {
		if (strict) {
			T y2 = x_to_y.upper_bound(x)->second;
			return y2 != inf && y2 > y;
		}
		else {
			T y2 = x_to_y.lower_bound(x)->second;
			return y2 != inf && y2 >= y;
		}
	}

	// x' >= x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
	pair<T, T> lower_bound(T x) {
		// verify : https://codeforces.com/gym/100633/problem/D

		return *x_to_y.lower_bound(x);
	}

	// x' > x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
	pair<T, T> upper_bound(T x) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		return *x_to_y.upper_bound(x);
	}

	// x' <= x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
	pair<T, T> lower_bound_rev(T x) {
		return *prev(x_to_y.upper_bound(x));
	}

	// x' < x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
	pair<T, T> upper_bound_rev(T x) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		return *prev(x_to_y.lower_bound(x));
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Monotonous_points& mp) {
		repe(p, mp.x_to_y) if (abs(p.first) != mp.inf) os << p << " ";
		return os;
	}
#endif
};


