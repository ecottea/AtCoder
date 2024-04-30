#pragma once
#include "header.h"
#include "セグメント木.h"
#include "座標圧縮.h"
// ■■■■■ 矩形クエリ ■■■■■


//【1 点加算 → 矩形和（一括）】
/*
* Offline_rectangle_sum<T>() : O(1)
*	v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
*
* void point_add(ll x, int y, T val) : O(1)
*	v[x][y] += val とする．
*
* void add_query(ll x1, ll x2, int y1, int y2) : O(1)
*	クエリ Σv[x1..x2)[y1..y2) を追加する．
*
* vT sum() : O(w + (n + q) log w)
*	現在の v[0..h)[0..w) への各クエリに対する答えを格納したリストを返す．
*
*（平面走査）
*/
template <class T>
class Offline_rectangle_sum {
	int w, q;

	// (x 座標，イベントタイプ，クエリ番号，左位置，右位置, 加算値) の組
	vector<tuple<ll, int, int, int, int, T>> ev;
	const int DE = 0; // 長方形の下辺
	const int UE = 1; // 長方形の上辺
	const int PT = 2; // 点

public:
	// v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
	Offline_rectangle_sum() : w(1), q(0) {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum
	}

	// v[x][y] += val とする．
	void point_add(ll x, int y, T val) {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		ev.emplace_back(x, PT, -1, y, -1, val);

		chmax(w, y);
	}

	// クエリ Σv[x1..x2)[y1..y2) を追加する．
	void add_query(ll x1, ll x2, int y1, int y2) {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		ev.emplace_back(x1, UE, q, y1, y2, -1);
		ev.emplace_back(x2, DE, q, y1, y2, -1);

		chmax(w, y2);
		q++;
	}

	// 各クエリに対する答えを格納したリストを返す．
	vector<T> sum() {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		// イベントソート
		sort(all(ev));

		fenwick_tree<T> fen(w + 1);
		vector<T> res(q, 0);

		// 下方向に平面走査していく．
		for (auto& [x, tp, j, yl, yr, val] : ev) {
			if (tp == PT) {
				fen.add(yl, val);
			}
			else if (tp == UE) {
				res[j] -= fen.sum(yl, yr);
			}
			else if (tp == DE) {
				res[j] += fen.sum(yl, yr);
			}
		}

		return res;
	}
};


//【1 点加算 → 矩形和（一括，アーベル群）】
/*
* Offline_rectangle_sum<S, op, o, inv>() : O(1)
*	v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
*	値はアーベル群 (S, op, o, inv) の要素とする．
*
* void point_add(ll x, int y, S val) : O(1)
*	v[x][y] += val とする．
*
* void add_query(ll x1, ll x2, int y1, int y2) : O(1)
*	クエリ Σv[x1..x2)[y1..y2) を追加する．
*
* vS sum() : O(w + (n + q) log w)
*	現在の v[0..h)[0..w) への各クエリに対する答えを格納したリストを返す．
*
* 利用：【フェニック木（アーベル群）】
*
*（平面走査）
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
class Offline_rectangle_sum {
	int w, q;

	// (x 座標，イベントタイプ，クエリ番号，左位置，右位置, 加算値) の組
	using EV = tuple<ll, int, int, int, int, S>;
	vector<EV> ev;
	const int DE = 0; // 長方形の下辺
	const int UE = 1; // 長方形の上辺
	const int PT = 2; // 点

public:
	// v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
	Offline_rectangle_sum() : w(1), q(0) {
		// verify : https://mojacoder.app/users/shogo314/problems/rectangle_product
	}

	// v[x][y] += val とする．
	void point_add(ll x, int y, S val) {
		// verify : https://mojacoder.app/users/shogo314/problems/rectangle_product

		ev.emplace_back(x, PT, -1, y, -1, val);

		chmax(w, y);
	}

	// クエリ Σv[x1..x2)[y1..y2) を追加する．
	void add_query(ll x1, ll x2, int y1, int y2) {
		// verify : https://mojacoder.app/users/shogo314/problems/rectangle_product

		ev.emplace_back(x1, UE, q, y1, y2, S());
		ev.emplace_back(x2, DE, q, y1, y2, S());

		chmax(w, y2);
		q++;
	}

	// 各クエリに対する答えを格納したリストを返す．
	vector<S> sum() {
		// verify : https://mojacoder.app/users/shogo314/problems/rectangle_product

		// イベントソート
		sort(all(ev), [](const EV& l, const EV& r) {
			ll xl, xr; int tpl, tpr;
			tie(xl, tpl, ignore, ignore, ignore, ignore) = l;
			tie(xr, tpr, ignore, ignore, ignore, ignore) = r;
			if (xl != xr) return xl < xr;
			return tpl < tpr;
		});

		Fenwick_tree<S, op, o, inv> fen(w + 1);
		vector<S> res(q);

		// 下方向に平面走査していく．
		for (auto& [x, tp, j, yl, yr, val] : ev) {
			if (tp == PT) {
				fen.add(yl, val);
			}
			else if (tp == UE) {
				res[j] = inv(fen.sum(yl, yr));
			}
			else if (tp == DE) {
				res[j] = op(res[j], fen.sum(yl, yr));
			}
		}

		return res;
	}
};


//【1 点加算 → 矩形和（アーベル群）】
/*
* Static_rectangle_sum<S, op, o, inv>(vl x, vl y, vS v) : O(n log n)
*	値 v[i] をもった n 個の点群 (x[i], y[i]) で初期化する．
*	値はアーベル群 (S, op, o, inv) の要素とする．
*
* S sum(ll x1, ll x2, ll y1, ll y2) : O(log n)
*	[x1..x2)×[y1..y2) 内にある全ての点の値の和を返す．
*
* 利用：【永続セグメント木（モノイド）】,【座標圧縮】
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
class Static_rectangle_sum {
	// 参考 : https://qiita.com/hotman78/items/9c643feae1de087e6fc5

	// x[y] 座標の昇順列（x 座標は全て，y 座標はユニーク）
	vl xs, ys;

	// x 座標を時刻とみなした，圧縮後の y 座標に関する永続セグメント木
	Persistent_segtree<S, op, o> seg;

public:
	// 値 v[i] をもった n 個の点群 (x[i], y[i]) で初期化する．
	Static_rectangle_sum(const vl& x, const vl& y, const vector<S>& v) {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		int n = sz(x);
		xs.resize(n);

		// y 座標を座標圧縮しておく．
		vi y_cp;
		int m = coordinate_compression(y, y_cp, &ys);

		// 点群を x 座標昇順にソートする
		vector<pli> xi(n);
		rep(i, n) xi[i] = { x[i], i };
		sort(all(xi));

		// x 座標を時刻とみなして永続セグメント木に乗せる．
		seg = Persistent_segtree<S, op, o>(m);
		rep(t, n) {
			int i;
			tie(xs[t], i) = xi[t];

			S val = seg.get(y_cp[i], t);
			seg.set(y_cp[i], op(val, v[i]), t);
		}
	}

	// [x1..x2)×[y1..y2) 内にある全ての点の値の和を返す．
	S sum(ll x1, ll x2, ll y1, ll y2) const {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		if (x1 >= x2 || y1 >= y2) return o();

		int t1 = lbpos(xs, x1);
		int t2 = lbpos(xs, x2);
		int j1 = lbpos(ys, y1);
		int j2 = lbpos(ys, y2);

		return op(seg.prod(j1, j2, t2), inv(seg.prod(j1, j2, t1)));
	}
};


//【1 点加算／矩形和（アーベル群）】
/*
* 動的に与えられる 1 点加算／矩形和 の混合クエリを処理したい場合は
* セグメント木.h の【二次元動的セグメント木】を利用すればよい．
* 
* verify : https://judge.yosupo.jp/problem/point_add_rectangle_sum
*/


//【1 点加算 → 開放矩形和（可換モノイド，一括）】
/*
* Offline_rectangle_sum<S, op, o>() : O(1)
*	v[0..h)[0..w) = o() で初期化する（h, w は自動で調整される）
*	要素は可換モノイド <S, op, o> の元とする．
*
* void point_add(ll x, int y, S val) : O(1)
*	v[x][y] += val とする．
*
* void add_query(ll x2, int y1, int y2) : O(1)
*	クエリ Σv[0..x2)[y1..y2) を追加する．
*
* vS sum() : O((n + q) log w)
*	現在の v[0..h)[0..w) への各クエリに対する答えを格納したリストを返す．
*
*（平面走査）
*/
template <class S, S(*op)(S, S), S(*o)()>
class Offline_rectangle_open_sum {
	int w, q;

	// (x 座標，イベントタイプ，クエリ番号，左位置，右位置, 加算値) の組
	vector<tuple<ll, int, int, int, int, S>> ev;
	static constexpr int DE = 0; // 長方形の下辺
	static constexpr int PT = 1; // 点

public:
	// v[0..h)[0..w) = o() で初期化する（h, w は自動で調整される）
	Offline_rectangle_open_sum() : w(1), q(0) {
		// verify : https://www.codechef.com/problems/PIARQ
	}

	// v[x][y] += val とする．
	void point_add(ll x, int y, S val) {
		// verify : https://www.codechef.com/problems/PIARQ

		ev.emplace_back(x, PT, -1, y, -1, val);

		chmax(w, y);
	}

	// クエリ Σv[0..x2)[y1..y2) を追加する．
	void add_query(ll x2, int y1, int y2) {
		// verify : https://www.codechef.com/problems/PIARQ

		ev.emplace_back(x2, DE, q, y1, y2, -1);

		chmax(w, y2);
		q++;
	}

	// 各クエリに対する答えを格納したリストを返す．
	vector<S> sum() {
		// verify : https://www.codechef.com/problems/PIARQ

		// イベントソート
		sort(all(ev));

		Segtree<S, op, o> seg(w + 1);
		vector<S> res(q);

		// 下方向に平面走査していく．
		for (auto& [x, tp, j, yl, yr, val] : ev) {
			if (tp == PT) {
				seg.set(yl, op(seg.get(yl), val));
			}
			else if (tp == DE) {
				res[j] = seg.prod(yl, yr);
			}
		}

		return res;
	}
};


//【矩形加算 → 1 点参照（一括）】
/*
* Offline_rectangle_add<T>() : O(1)
*	v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
*
* void rectangle_add(ll x, int y, T val) : O(1)
*	v[x1..x2)[y1..y2) += val とする．
*
* void get(ll x, ll y) : O(1)
*	クエリ v[x][y] を追加する．
*
* vT solve() : O((n + q) log n)
*	現在の v[0..h)[0..w) への各クエリに対する答えを格納したリストを返す．
*
*（平面走査）
*/
template <class T>
class Offline_rectangle_add {
	vl x1_add, x2_add, y1_add, y2_add; vector<T> w_add;
	vl x_get, y_get;

public:
	// v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
	Offline_rectangle_add() {
		// verify : https://mojacoder.app/users/Tonegawac/problems/plane_add
	}

	// v[x1..x2)[y1..y2) += val とする．
	void rectangle_add(ll x1, ll x2, ll y1, ll y2, T val) {
		// verify : https://mojacoder.app/users/Tonegawac/problems/plane_add

		x1_add.emplace_back(x1);
		x2_add.emplace_back(x2);
		y1_add.emplace_back(y1);
		y2_add.emplace_back(y2);
		w_add.emplace_back(val);
	}

	// クエリ v[x][y] を追加する．
	void get(ll x, ll y) {
		// verify : https://mojacoder.app/users/Tonegawac/problems/plane_add

		x_get.emplace_back(x);
		y_get.emplace_back(y);
	}

	// 各クエリに対する答えを格納したリストを返す．
	vector<T> solve() {
		// verify : https://mojacoder.app/users/Tonegawac/problems/plane_add

		// ys : y 座標のユニークな昇順列
		vl ys;
		ys.reserve(sz(y1_add) + sz(y2_add));
		repe(y, y1_add) ys.emplace_back(y);
		repe(y, y2_add) ys.emplace_back(y);
		uniq(ys);

		// (x 座標, クエリ番号, y 座標, 加算時の符号) の組（加算は負のクエリ番号で表す）
		vector<tuple<ll, int, int, int>> ev;
		rep(i, sz(x1_add)) {
			ev.emplace_back(x1_add[i], -i - 1, lbpos(ys, y1_add[i]), 1);
			ev.emplace_back(x1_add[i], -i - 1, lbpos(ys, y2_add[i]), -1);
			ev.emplace_back(x2_add[i], -i - 1, lbpos(ys, y1_add[i]), -1);
			ev.emplace_back(x2_add[i], -i - 1, lbpos(ys, y2_add[i]), 1);
		}
		int q = sz(x_get);
		rep(t, q) {
			ev.emplace_back(x_get[t], t, ubpos(ys, y_get[t]), 1);
		}

		// イベントソート
		sort(all(ev));

		fenwick_tree<T> fen(sz(ys));
		vector<T> res(q);

		// 下方向に平面走査していく．
		for (auto& [x, id, y, sgn] : ev) {
			if (id < 0) {
				// 加算する場合
				fen.add(y, sgn * w_add[-id - 1]);
			}
			else {
				// 取得クエリの場合
				res[id] = fen.sum(0, y);
			}
		}

		return res;
	}
};


//【矩形加算／1 点参照（アーベル群）】
/*
* 動的に与えられる 矩形加算／1 点参照 の混合クエリを処理したい場合は
* セグメント木.h の【二次元動的セグメント木】と二次元いもす法を利用すればよい．
* 
* verify : https://judge.yosupo.jp/problem/rectangle_add_point_get
*/


//【矩形 chmax → 1 点参照（一括）】
/*
* セグメント木.h の【可換双対セグメント木（M<E>-集合）】に
*【要素の挿入&削除 作用付き 最大値 集合】を載せて平面走査すれば良い．
* 
* verify : https://atcoder.jp/contests/abc342/tasks/abc342_g
*/


//【矩形加算 → 矩形和（一括）】
/*
* Offline_rectangle_add_sum<T>() : O(1)
*	v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
*
* void rectangle_add(ll x1, ll x2, ll y1, ll y2, T val) : O(1)
*	v[x1..x2)[y1..y2) += val とする．
*
* void rectangle_sum(ll x1, ll x2, ll y1, ll y2) : O(1)
*	クエリ Σv[x1..x2)[y1..y2) を追加する．
*
* vT solve() : O((n + q) log n)
*	現在の v[0..h)[0..w) への各クエリに対する答えを格納したリストを返す．
*
*（平面走査）
*/
template <class T> tuple<T, T, T> oporas(tuple<T, T, T> x, tuple<T, T, T> y) {
	auto [sx, cx, wx] = x; // (上からの累積和, いまの総和, 横の長さ)
	auto [sy, cy, wy] = y;
	return { sx + sy, cx + cy, wx + wy };
}
template <class T> tuple<T, T, T> eoras() { return { 0, 0, 0 }; }
template <class T> tuple<T, T, T> actoras(tuple<T, T, T> f, tuple<T, T, T> x) {
	auto [h1, c1, r1] = f; // 行列 (1, h, r; 0, 1, c; 0, 0, 1)
	auto [s2, c2, w2] = x;
	return { c2 * h1 + s2 + r1 * w2, c2 + c1 * w2, w2 };
}
template <class T> tuple<T, T, T> comporas(tuple<T, T, T> f, tuple<T, T, T> g) {
	auto [h1, c1, r1] = f;
	auto [h2, c2, r2] = g;
	return { h1 + h2, c1 + c2, c2 * h1 + r1 + r2 };
}
template <class T> tuple<T, T, T> idoras() { return { 0, 0, 0 }; }
template <class T>
class Offline_rectangle_add_sum {
	vl x1_add, x2_add, y1_add, y2_add; vector<T> w_add;
	vl x1_sum, x2_sum, y1_sum, y2_sum;

public:
	// v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
	Offline_rectangle_add_sum() {
		// verify : https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum
	}

	// v[x1..x2)[y1..y2) += val とする．
	void rectangle_add(ll x1, ll x2, ll y1, ll y2, T val) {
		// verify : https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum

		x1_add.emplace_back(x1);
		x2_add.emplace_back(x2);
		y1_add.emplace_back(y1);
		y2_add.emplace_back(y2);
		w_add.emplace_back(val);
	}

	// クエリ Σv[x1..x2)[y1..y2) を追加する．
	void rectangle_sum(ll x1, ll x2, ll y1, ll y2) {
		// verify : https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum

		x1_sum.emplace_back(x1);
		x2_sum.emplace_back(x2);
		y1_sum.emplace_back(y1);
		y2_sum.emplace_back(y2);
	}

	// 各クエリに対する答えを格納したリストを返す．
	vector<T> solve() {
		// verify : https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum

		// ys : y 座標のユニークな昇順列
		vl ys;
		ys.reserve(sz(y1_add) + sz(y2_add) + sz(y1_sum) + sz(y2_sum));
		repe(y, y1_add) ys.emplace_back(y);
		repe(y, y2_add) ys.emplace_back(y);
		repe(y, y1_sum) ys.emplace_back(y);
		repe(y, y2_sum) ys.emplace_back(y);
		uniq(ys);

		// (x 座標，クエリ番号，左位置，右位置, 加算時の符号) の組（加算は負のクエリ番号で表す）
		vector<tuple<ll, int, int, int, int>> ev;
		rep(i, sz(x1_add)) {
			ev.emplace_back(x1_add[i], -i - 1, lbpos(ys, y1_add[i]), lbpos(ys, y2_add[i]), 1);
			ev.emplace_back(x2_add[i], -i - 1, lbpos(ys, y1_add[i]), lbpos(ys, y2_add[i]), -1);
		}
		int q = sz(x1_sum);
		rep(t, q) {
			ev.emplace_back(x1_sum[t], t, lbpos(ys, y1_sum[t]), lbpos(ys, y2_sum[t]), -1);
			ev.emplace_back(x2_sum[t], t, lbpos(ys, y1_sum[t]), lbpos(ys, y2_sum[t]), 1);
		}

		// イベントソート
		sort(all(ev));

		// seg : 区間の (s, c, w) = (上からの累積和, いまの総和, 横の長さ) を管理する遅延セグ木
		int m = sz(ys) - 1;
		vector<tuple<T, T, T>> ini(m);
		rep(j, m) ini[j] = { 0, 0, ys[j + 1] - ys[j] };
		lazy_segtree<tuple<T, T, T>, oporas<T>, eoras<T>, tuple<T, T, T>, actoras<T>, comporas<T>, idoras<T>> seg(ini);

		vector<T> res(q, 0);

		// 下方向に平面走査していく．
		ll x_prv = get<0>(ev[0]);
		for (auto [x, id, l, r, sgn] : ev) {
			// 縦方向に x - x_prv だけ伸ばす
			seg.apply(0, m, { x - x_prv, 0, 0 });

			if (id < 0) {
				// 加算する場合
				seg.apply(l, r, { 0, sgn * w_add[-id - 1], 0 });
			}
			else {
				// 総和クエリの場合
				res[id] += sgn * get<0>(seg.prod(l, r));
			}

			x_prv = x;
		}

		return res;
	}
};


