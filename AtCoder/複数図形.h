#pragma once
#include "header.h"
#include "構造(幾何).h"
#include "分析(文字列).h"
#include "順列.h"
#include "探索.h"
#include "作図.h"
#include "有理数.h"
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
template <class T>
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
template <class T>
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
* 点群 p[0..n) から 3 点を選んで作れる鋭角[直角,鈍角]三角形の個数を cp[c0,cn] とし，{cp,c0,cn} を返す．
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
* 点群 p[0..n) の頂点数最小の凸包の頂点を反時計回りに格納したリスト ch を返す．
* ch[0] は x 座標最小（同じものがあれば y 座標最小）の点とする．
*/
template <class T>
Polygon<T> convex_hull(vector<Point<T>> p) {
	// verify : https://judge.yosupo.jp/problem/static_convex_hull

	int n = sz(p);
	if (n == 0) return Polygon<T>();

	// 重複を除去し，x 座標を優先して昇順ソート（x 座標が同じなら y 座標昇順）
	uniq(p);
	n = sz(p);

	// 下側（および右側）凸包を成す頂点
	Polygon<T> ch_l;

	// まず x 座標昇順に見ていく．
	int pt = 0;
	rep(i, n) {
		// 凸でない限り直前の点を除去することを繰り返す．凸かどうかは外積を用いて判定できる．
		while (pt >= 2 && (ch_l[pt - 1] - ch_l[pt - 2]).cross(p[i] - ch_l[pt - 2]) <= 0) {
			ch_l.pop_back();
			pt--;
		}

		// 今見ている点を暫定的に凸包に加える．
		ch_l.push_back(p[i]);
		pt++;
	}

	// 上側（および左側）凸包を成す頂点
	Polygon<T> ch_h;

	// 次に x 座標降順に見ていく．
	pt = 0;
	repir(i, n - 1, 0) {
		// 凸でない限り直前の点を除去することを繰り返す．凸かどうかは外積を用いて判定できる．
		while (pt >= 2 && (ch_h[pt - 1] - ch_h[pt - 2]).cross(p[i] - ch_h[pt - 2]) <= 0) {
			ch_h.pop_back();
			pt--;
		}

		// 今見ている点を暫定的に凸包に加える．
		ch_h.push_back(p[i]);
		pt++;
	}

	// 重複を取り除きつつまとめる．
	repi(i, 1, sz(ch_h) - 1) ch_l.push_back(ch_h[i]);
	if (sz(ch_l) > 1) ch_l.pop_back();

	return ch_l;
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
	using pTT = pair<T, T>;
	auto convexQ = [&](const pTT& P, const pTT& Q, const pTT& R) {
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


//【線分群の交点（軸平行）】O(n log w + w)
/*
* 2 点 (x1[i], y1[i]), (x2[i], y2[i]) を結ぶ n 本の閉線分について，
* (L 字型の共有点の個数, T 字型の共有点の個数, 十字型の共有点の個数) の 3 つ組を返す．
*
* 制約：y1[i]≧0，線分は軸平行，互いに平行な線分同士は共有点をもたない．
*/
tuple<ll, ll, ll> count_intersections_of_segments(const vi& x1, const vi& y1, const vi& x2, const vi& y2) {
	// verify : https://yukicoder.me/problems/no/1920

	int n = sz(x1);

	// (x 座標，イベントタイプ，左位置，右位置) の組
	vector<tuple<int, int, int, int>> ev;
	const int EPU = 0; // 縦方向の線分の上の端点
	const int VSG = 1; // 縦方向の線分の胴体
	const int EPD = 2; // 縦方向の線分の下の端点
	const int ERS = 3; // 縦方向の線分の消滅
	const int HSG = 4; // 横方向の線分

	int w = 0;
	rep(i, n) {
		// 縦方向の線分の場合
		if (y1[i] == y2[i]) {
			int xu = x1[i], xd = x2[i], y = y1[i];
			if (xu > xd) swap(xu, xd);

			ev.emplace_back(xu, EPU, y, y);
			ev.emplace_back(xu + 1, VSG, y, y);
			ev.emplace_back(xd, EPD, y, y);
			ev.emplace_back(xd + 1, ERS, y, y);

			chmax(w, y);
		}
		// 横方向の線分の場合
		else if (x1[i] == x2[i]) {
			int yl = y1[i], yr = y2[i], x = x1[i];
			if (yl > yr) swap(yl, yr);

			ev.emplace_back(x, HSG, yl, yr);

			chmax(w, yr);
		}
		else Assert(!"illegal segment!!");
	}

	// イベント混合ソート
	sort(all(ev));

	fenwick_tree<int> ep(w + 1), seg(w + 1);
	ll cntL = 0, cntT = 0, cntX = 0;

	// 下方向に平面走査していく．
	for (auto& [x, tp, yl, yr] : ev) {
		if (tp == EPU) ep.add(yl, 1);
		else if (tp == VSG) ep.add(yl, -1), seg.add(yl, 1);
		else if (tp == EPD) seg.add(yl, -1), ep.add(yl, 1);
		else if (tp == ERS) ep.add(yl, -1);
		else if (tp == HSG) {
			cntL += ep.sum(yl, yl + 1) + ep.sum(yr, yr + 1);
			cntT += seg.sum(yl, yl + 1) + seg.sum(yr, yr + 1);
			if (yl + 1 <= yr) {
				cntT += ep.sum(yl + 1, yr);
				cntX += seg.sum(yl + 1, yr);
			}
		}
	}

	return { cntL, cntT, cntX };
}


//【線分群の連結性（軸平行）】O(n log w + w)
/*
* 2 点 (x1[i], y1[i]), (x2[i], y2[i]) を結ぶ n 本の閉線分について，連結関係を表す Union-Find を返す．
*
* 制約：y1[i]≧0，(線分の長さ)>0，線分は軸平行，互いに平行な線分同士は共有点をもたない．
*/
int opccc(int a, int b) { return min(a, b); }
int eccc() { return INF; }
int opccc2(int a, int b) { return max(a, b); }
int eccc2() { return -1; }
dsu connectivity_of_segments(const vi& x1, const vi& y1, const vi& x2, const vi& y2) {
	// verify : https://mojacoder.app/users/Tonegawac/problems/segment-connectivity

	int n = sz(x1);

	// (x 座標，イベントタイプ，左位置，右位置, 辺番号) の組
	vector<tuple<int, int, int, int, int>> ev;
	const int EPU = 0; // 縦方向の線分の上の端点
	const int ERS = 1; // 縦方向の線分の消滅
	const int HSG = 2; // 横方向の線分

	int w = 0;
	rep(i, n) {
		// 縦方向の線分の場合
		if (y1[i] == y2[i]) {
			int xu = x1[i], xd = x2[i], y = y1[i];
			if (xu > xd) swap(xu, xd);

			ev.emplace_back(xu, EPU, y, y, i);
			ev.emplace_back(xd + 1, ERS, y, y, i);

			chmax(w, y);
		}
		// 横方向の線分の場合
		else if (x1[i] == x2[i]) {
			int yl = y1[i], yr = y2[i], x = x1[i];
			if (yl > yr) swap(yl, yr);

			ev.emplace_back(x, HSG, yl, yr, i);

			chmax(w, yr);
		}
		else Assert(!"illegal segment!!");
	}

	// イベントソート
	sort(all(ev));

	// vid[y] : 位置 y にある縦方向の線分の番号（なければ -1）
	segtree<int, opccc2, eccc2> vid(w + 1);

	// 次に見るべき右の座標
	lazy_segtree<int, opccc, eccc, int, opccc, opccc, eccc> rgt(w + 1);

	dsu d(n);

	// 下方向に平面走査していく．
	for (auto& [x, tp, yl, yr, id] : ev) {
		if (tp == EPU) {
			// 縦方向の線分を追加する．
			vid.set(yl, id);

			// より左からのジャンプをここで止める．
			rgt.apply(0, yl, yl);
		}
		else if (tp == ERS) {
			// 縦方向の線分を削除する．
			vid.set(yl, -1);

			// 右方向の最も近い線分まではジャンプできる．
			int ny = vid.max_right(yl, [](int t) { return t == -1; });
			rgt.set(yl, ny);
		}
		else if (tp == HSG) {
			// 途中で見た縦方向の線分の位置
			vi seen;

			// yl から yr まで可能ならジャンプしながら線分を拾っていく．
			int y = yl;
			while (y <= yr) {
				int id2 = vid.get(y);
				if (id2 != -1) {
					d.merge(id, id2);
					seen.push_back(y);
				}
				y = rgt.get(y);
			}

			// 連結成分からは一気に右端までジャンプできる．
			repe(y2, seen) rgt.set(y2, y);
		}
	}

	return d;
}


//【長方形の和集合の面積】O(n log n)
/*
* n 個の長方形 [x1[i]..x2[i])×[y1[i]..y2[i]) の和集合の面積を返す．
*
*（平面走査）
*/
pil opRUA(pil x, pil y) {
	auto [vx, cx] = x;
	auto [vy, cy] = y;
	if (vx < vy) return x;
	if (vx > vy) return y;
	return { vx, cx + cy };
}
pil eRUA() { return { INF, 0 }; } // (min, cnt)
pil actRUA(int f, pil x) { auto [vx, cx] = x; return { vx + f, cx }; }
int compRUA(int f, int g) { return f + g; }
int idRUA() { return 0; } // add
ll rectangle_union_area(const vl& x1, const vl& x2, const vl& y1, const vl& y2) {
	// 参考 : https://www.mathenachia.blog/library-report-2022/#toc10
	// verify : https://judge.yosupo.jp/problem/area_of_union_of_rectangles

	int n = sz(x1);

	// ys : y 座標のユニークな昇順列
	vl ys(2 * n);
	rep(i, n) {
		ys[2 * i] = y1[i];
		ys[2 * i + 1] = y2[i];
	}
	uniq(ys);

	// (x 座標，クエリタイプ，左位置，右位置) の組
	constexpr int UP = 1, DN = -1;
	vector<tuple<ll, int, int, int>> ev;
	rep(i, n) {
		int y1_cp = lbpos(ys, y1[i]);
		int y2_cp = lbpos(ys, y2[i]);
		ev.push_back({ x1[i], UP, y1_cp, y2_cp });
		ev.push_back({ x2[i], DN, y1_cp, y2_cp });
	}

	// イベントソート
	sort(all(ev));

	// seg : 区間の (m, c) = (最小値, 最小値をとる元の個数) を管理する遅延セグ木
	int m = sz(ys) - 1;
	vector<pil> ini(m);
	rep(j, m) ini[j] = { 0, ys[j + 1] - ys[j] };
	lazy_segtree<pil, opRUA, eRUA, int, actRUA, compRUA, idRUA> seg(ini);

	// 下方向に平面走査していく．
	ll res = 0; ll x_prv = get<0>(ev[0]);
	for (auto [x, add, l, r] : ev) {
		ll h = x - x_prv; // 高さ
		ll w = ys[m] - ys[0]; // 幅

		// 長方形の重なりが 0 である分だけ幅を小さくする．
		auto [v_min, cnt] = seg.all_prod();
		if (v_min == 0) w -= cnt;

		// 長方形の面積を一括加算する．
		res += h * w;

		x_prv = x;

		seg.apply(l, r, add);
	}

	return res;
}


//【最短バイトニックツアー】O(n^2)
/*
* x 座標の互いに異なる n 個の点群 p について，
*	x 座標最小の点 →(x 座標昇順)→ x 座標最大の点 →(x 座標降順)→ x 座標最小の点
* の順に点を結ぶ経路の最短長を返す．
*/
template <class T>
double minimum_bitonic_tour(vector<Point<T>>& p) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_2_C

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
	vector<Frac<ll>> xs;
	vd xs_d;

public:
	// 直線群 a[i] x + b[i] y = c[i] で初期化する．
	Line_intersections_lhplane(const vl& a_, const vl& b_, const vl& c_, ll inf_) : n(0), inf(inf_) {
		// verify : https://atcoder.jp/contests/tenka1-2017/tasks/tenka1_2017_e

		// x = -inf における点 i の y 座標を，{y, i} の形で昇順に並べたもの（y 軸非平行な直線のみ）
		vector<pair<Frac<ll>, int>> yi;

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
	ll count(const Frac<ll>& r) {
		// y[i] : i 番目の直線の x = r における y 座標
		vector<Frac<ll>> y(n);
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

		return count_illegal_colored_parenthesis_pair(p);
	}
};


//【最遠点対（ユークリッド距離）】O(n log n)
/*
* n 個の点 (x[i], y[i]) について，最も遠い 2 点の距離の 2 乗を返す．
* またその点対が i 番目と j 番目であることを ps = {i, j} として格納する．
*
* 利用：【凸包】,【凸多角形の直径】
*/
template <class T = ll>
T farthest_point_pair(const vector<T>& x, const vector<T>& y, pii* ps = nullptr) {
	// verify : https://judge.yosupo.jp/problem/furthest_pair

	int n = sz(x);

	ll sqd_max = -INFL; int id1 = -1, id2 = -1;

	unordered_map<ll, int> xy_to_id; constexpr ll W = 2000100131;
	rep(i, n) {
		ll h = (x[i] + W / 2) * W + (y[i] + W / 2);
		if (xy_to_id.count(h) && id1 == -1) {
			id1 = xy_to_id[h];
			id2 = i;
			sqd_max = 0;
		}
		xy_to_id[h] = i;
	}

	vector<Point<T>> p(n);
	rep(i, n) p[i] = Point<T>(x[i], y[i]);

	auto ch = convex_hull(p);

	pii k12;
	if (chmax(sqd_max, caliper(ch, k12))) {
		auto [k1, k2] = k12;

		ll h = (ch[k1].x + W / 2) * W + (ch[k1].y + W / 2);
		id1 = xy_to_id[h];

		h = (ch[k2].x + W / 2) * W + (ch[k2].y + W / 2);
		id2 = xy_to_id[h];
	}

	if (ps) *ps = { id1, id2 };

	return sqd_max;
}


//【一定距離以内の点対の列挙（ユークリッド距離）】O(?)
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
			if (pi[r].first.x - pi[l].first.x <= d + 1e-12) {
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
			if (pi[r].first.x - pi[l].first.x <= d + 1e-12) {
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


//【最近点対（ユークリッド距離）】O(n√n) ?
/*
* n 個の点 (x[i], y[i]) について，最も近い 2 点の距離の 2 乗を返す．
* またその点対が i 番目と j 番目であることを ps = {i, j} として格納する．
* 全探索できる点対の個数の上限を lim に与える（デフォルト値は 3/2 n√n）
*
* 利用：【回転】
*/
template <class T = ll>
T nearest_point_pair(const vector<T>& x, const vector<T>& y, pii* ps = nullptr, ll lim = -1) {
	// verify : https://judge.yosupo.jp/problem/closest_pair

	int n = sz(x);

	// 同一座標に 2 点以上存在する場合への対処
	unordered_map<ll, int> xy_to_id; constexpr ll W = 2000100131;
	rep(i, n) {
		ll h = (x[i] + W / 2) * W + (y[i] + W / 2);
		if (xy_to_id.count(h)) {
			if (ps != nullptr) *ps = { xy_to_id[h], i };
			return 0;
		}
		xy_to_id[h] = i;
	}

	// グリッド状のときを考えると少なくともこれくらいは必要になる．
	if (lim == -1) lim = max((ll)(1.5 * pow(n, 1.5)), 100LL);

	// 乱数生成器
	static mt19937_64 mt64((int)time(NULL));
	uniform_real_distribution<double> rnd(0., 2 * PI);

	// x 座標の差が d 以内の点対に絞って精密に距離を求める．
	double d = pow(10, 4.5);

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

		// cnt : x 座標の差が d 以内である点対の個数
		ll cnt = 0;

		// 尺取法で x 座標の差が d 以内である点対を数える．
		int l = 0, r = 0;
		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d) {
				cnt += r - l;
				r++;
			}
			else {
				l++;
			}
		}

		// 該当する点対が存在しないなら，d を大きくし，回転角をランダムに決め直す．
		if (cnt == 0) {
			d *= 2;
			continue;
		}

		// 調べるべき点対が多すぎるなら，d を小さくし，回転角をランダムに決め直す．
		if (cnt > lim) {
			d /= 2;
			continue;
		}

		// 尺取法で x 座標の差が d 以内である点対を列挙し，精密に距離を求める．
		l = 0; r = 0;
		ll sqres = (ll)9e18; // INFL では (10^9 - (-10^9))^2 + (10^9 - (-10^9))^2 より小さい

		while (r < n) {
			if (pi[r].first.x - pi[l].first.x <= d) {
				repi(i, l, r - 1) {
					int jr = pi[r].second;
					int ji = pi[i].second;
					if (ji > jr) swap(ji, jr);

					Point<ll> e((ll)(x[ji] - x[jr]), (ll)(y[ji] - y[jr]));
					if (chmin(sqres, e.sqnorm())) {
						if (ps != nullptr) *ps = { ji, jr };
					}
				}

				r++;
			}
			else {
				l++;
			}
		}

		// 最短距離が d を超えていたなら，d を大きくし，回転角をランダムに決め直す．
		if (sqres > d * d) {
			d *= 2;
			continue;
		}

		return sqres;
	}
}


//【最遠点（マンハッタン距離）】O(n)
/*
* 与えられた n 個の点 (x[i], y[i]) について，各点とのマンハッタン距離が最大の点のリストを返す．
*/
template <class T>
vi maximum_manhattan_distance(const vector<T>& x, const vector<T>& y) {
	// verify : https://yukicoder.me/problems/no/2436

	//【方法】
	// 45° 回転する変数変換
	//		u = x + y, v = x - y
	// を施すと，2 点 p[i], p[j] 間のマンハッタン距離は
	//		dist(p[i], p[j]) = max(u[i] - u[j], u[j] - u[i], v[i] - v[j], v[j] - v[i])
	// と表される．よって max のネストを整理して
	//		max_j dist(p[i], p[j])
	//		= max(u[i] - min(u), max(u) - u[i], v[i] - min(v), max(v) - v[i])
	// となる．

	int n = sz(x);

	vector<T> u(n), v(n);
	rep(i, n) {
		u[i] = x[i] + y[i];
		v[i] = x[i] - y[i];
	}

	pair<T, int> u_max = { -(T)INFL, -1 }, u_min = { (T)INFL, -1 };
	pair<T, int> v_max = { -(T)INFL, -1 }, v_min = { (T)INFL, -1 };

	rep(i, n) {
		chmax(u_max, { u[i], i });
		chmin(u_min, { u[i], i });
		chmax(v_max, { v[i], i });
		chmin(v_min, { v[i], i });
	}

	vi res(n);
	rep(i, n) {
		T dist = -(T)INFL;
		if (chmax(dist, u[i] - u_min.first)) res[i] = u_min.second;
		if (chmax(dist, u_max.first - u[i])) res[i] = u_max.second;
		if (chmax(dist, v[i] - v_min.first)) res[i] = v_min.second;
		if (chmax(dist, v_max.first - v[i])) res[i] = v_max.second;
	}

	return res;
}


//【最近点（マンハッタン距離）】O(n log n)
/*
* 与えられた n 個の点 (x[i], y[i]) について，各点とのマンハッタン距離が（自身を除き）最小の点のリストを返す．
*
* 利用：【座標圧縮】
*/
template <class T> pair<T, int> opmmd(pair<T, int> a, pair<T, int> b) { return min(a, b); }
template <class T> pair<T, int> emmd() { return { (T)INFL, -1 }; }
template <class T>
vi minimum_manhattan_distance(const vector<T>& x, const vector<T>& y) {
	// verify : https://yukicoder.me/problems/no/2436

	//【方法】
	// 2 点 p[i], p[j] 間のマンハッタン距離
	//		|x[i] - x[j]| + |y[i] - y[j]|
	// は，座標の大小関係により
	//		(x[i] + y[i]) - (x[j] + y[j])  (x[i] ≧ x[j] かつ y[i] ≧ y[j] のとき)
	//		(x[i] - y[i]) - (x[j] - y[j])  (x[i] ≧ x[j] かつ y[i] ≦ y[j] のとき)
	//		-(x[i] - y[i]) + (x[j] - y[j])  (x[i] ≦ x[j] かつ y[i] ≧ y[j] のとき)
	//		-(x[i] + y[i]) + (x[j] + y[j])  (x[i] ≦ x[j] かつ y[i] ≦ y[j] のとき)
	// と場合分けして表される．よって
	//		min_j≠i (|x[i] - x[j]| + |y[i] - y[j]|)
	//		= min( (x[i] + y[i]) - max_j≠i (x[j] + y[j]) | x[i] ≧ x[j] かつ y[i] ≧ y[j],
	//			   (x[i] - y[i]) - max_j≠i (x[j] - y[j]) | x[i] ≧ x[j] かつ y[i] ≦ y[j],
	//			  -(x[i] - y[i]) + min_j≠i (x[j] - y[j]) | x[i] ≦ x[j] かつ y[i] ≧ y[j],
	//			  -(x[i] + y[i]) + min_j≠i (x[j] + y[j]) | x[i] ≦ x[j] かつ y[i] ≦ y[j] )
	// となる．

	int n = sz(x);

	vi y_cp;
	int w = coordinate_compression(y, y_cp);

	vector<pair<T, int>> xi(n);
	rep(i, n) xi[i] = { x[i], i };
	sort(all(xi));

	vi res(n, -1); vector<T> dist(n, (T)INFL);

	segtree<pair<T, int>, opmmd<T>, emmd<T>> nu_min(w), nv_min(w);
	rep(t, n) {
		int i = xi[t].second;

		auto [nu, j1] = nu_min.prod(0, y_cp[i]);
		if (chmin(dist[i], (x[i] + y[i]) + nu)) res[i] = j1;

		auto [nv, j2] = nv_min.prod(y_cp[i], w);
		if (chmin(dist[i], (x[i] - y[i]) + nv)) res[i] = j2;

		nu_min.set(y_cp[i], min(nu_min.get(y_cp[i]), { -(x[i] + y[i]), i }));
		nv_min.set(y_cp[i], min(nv_min.get(y_cp[i]), { -(x[i] - y[i]), i }));
	}

	segtree<pair<T, int>, opmmd<T>, emmd<T>> u_min(w), v_min(w);
	repir(t, n - 1, 0) {
		int i = xi[t].second;

		auto [v, j1] = v_min.prod(0, y_cp[i]);
		if (chmin(dist[i], v - (x[i] - y[i]))) res[i] = j1;

		auto [u, j2] = u_min.prod(y_cp[i], w);
		if (chmin(dist[i], u - (x[i] + y[i]))) res[i] = j2;

		u_min.set(y_cp[i], min(u_min.get(y_cp[i]), { x[i] + y[i], i }));
		v_min.set(y_cp[i], min(v_min.get(y_cp[i]), { x[i] - y[i], i }));
	}

	return res;
}


//【マンハッタン最小全域木】O(n log n)
/*
* 点群 (x[0..n), y[0..n)) のマンハッタン距離に基づく最小全域木を g に格納し，総コストを返す．
*/
template <class T>
ll manhattan_MST(const vector<T>& x0, const vector<T>& y0, Graph& g) {
	// 参考 : https://www.topcoder.com/community/competitive-programming/tutorials/line-sweep-algorithms/
	// verify : https://judge.yosupo.jp/problem/manhattanmst

	int n = sz(x0);

	vector<tuple<T, T, int>> xyi(n);
	rep(i, n) xyi[i] = { x0[i], y0[i], i };

	vector<pair<T, int>> sk(n), tmp(n);

	priority_queue_rev<tuple<ll, int, int>> q;

	auto execute = [&] {
		// sk : y-x について昇順にマージソートしていく配列（k : x 座標昇順で何番目か）
		rep(k, n) sk[k] = { get<1>(xyi[k]) - get<0>(xyi[k]), k };

		// x 座標昇順で [l..r) 番目の点についての処理を行う．
		function<void(int, int)> rf = [&](int l, int r) {
			if (r - l <= 1) return;

			// x[m] : 上半平面と下半平面の境界となる x 座標
			int m = (l + r) / 2;

			// 上下の半平面それぞれについて再帰的に処理を行う．
			rf(l, m);
			rf(m, r);

			// 点群を y-x について昇順，次いで x について昇順にソートする．
			merge(sk.begin() + l, sk.begin() + m, sk.begin() + m, sk.begin() + r, tmp.begin() + l);
			repi(k, l, r - 1) sk[k] = tmp[k];

			// s_max : 上半平面における x+y の最大値
			T s_max = -T(INFL); int i_max = -1; T x_max = -1, y_max = -1;

			repi(t, l, r - 1) {
				int k = sk[t].second;
				auto [x, y, i] = xyi[k];

				// 注目点が上半平面に属している場合
				if (k < m) {
					// x+y が最大の点を記録しておく．
					if (chmax(s_max, x + y)) {
						i_max = i;
						x_max = x;
						y_max = y;
					}
				}
				// 注目点が下半平面に属している場合
				else {
					// 上半平面の x+y が最大の点との間の辺が MST の候補となる．
					if (i_max != -1) {
						ll c = abs(x - x_max) + abs(y - y_max);
						q.emplace(c, i, i_max);
					}
				}
			}
		};
		rf(0, n);
	};

	// 8 通りの向きについて MST の辺の候補を洗い出す．
	sort(all(xyi));
	execute();
	rep(k, n) get<1>(xyi[k]) *= -1;
	execute();
	rep(k, n) { get<0>(xyi[k]) *= -1; } reverse(all(xyi));
	execute();
	rep(k, n) get<1>(xyi[k]) *= -1;
	execute();
	rep(k, n) { swap(get<0>(xyi[k]), get<1>(xyi[k])); } sort(all(xyi));
	execute();
	rep(k, n) get<1>(xyi[k]) *= -1;
	execute();
	rep(k, n) { get<0>(xyi[k]) *= -1; } reverse(all(xyi));
	execute();
	rep(k, n) get<1>(xyi[k]) *= -1;
	execute();

	// クラスカル法で MST を求める．
	g = Graph(n); ll cost = 0; dsu d(n); int cnt = 0;
	while (cnt < n - 1) {
		auto [c, s, t] = q.top(); q.pop();

		// もし辺の両端が既に連結なら繋がない．
		if (d.same(s, t)) continue;

		// そうでないならコスト最小の辺なのでそれで繋ぐ．
		cost += c;
		d.merge(s, t);
		cnt++;

		g[s].push_back(t);
		g[t].push_back(s);
	}

	return cost;
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
	// 右下隅に点があるとは限らないので，その候補は点のある行と列の交点 O(n^2) 個になる．

	// (i0, j0) : 左上隅として許せる限界（内部）
	int i0 = 0;
	rep(i, h) {
		while (xs[i] - xs[i0] > d) i0++;

		int j0 = 0;
		rep(j, w) {
			while (ys[j] - ys[j0] > d) j0++;

			// (i, j) の要素を選ぶ場合
			// BB の右下隅が (i, j) になることは確定するので，残りは自由に選べる．
			mint add1 = pow2[cnt[i][j]] - 1;
			add1 *= pow2[acc.sum(i0, j0, i + 1, j + 1) - cnt[i][j]];
			res += add1;

			// (i, j) の要素を選ばない場合
			// BB の右端と下端を確定させるため，それぞれ少なくとも 1 つ点を選ぶ必要がある．
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


