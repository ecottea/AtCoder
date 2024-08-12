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
* void add(ll x, int y, T val) : O(1)
*	v[x][y] += val とする．
*
* void sum(ll x1, ll x2, int y1, int y2) : O(1)
*	クエリ Σv[x1..x2)[y1..y2) を追加する．
*
* vT solve() : O(w + (n + q) (log(n + q) + log w))
*	現在の v[0..h)[0..w) への各クエリに対する答えを格納したリストを返す．
*
*（平面走査）
*/
template <class T>
class Offline_rectangle_sum {
	vl x_add, y_add; vector<T> w_add;
	vl x1_sum, x2_sum, y1_sum, y2_sum;

public:
	// v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
	Offline_rectangle_sum() {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum
	}

	// v[x][y] += val とする．
	void add(ll x, ll y, T val) {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		x_add.emplace_back(x);
		y_add.emplace_back(y);
		w_add.emplace_back(val);
	}

	// クエリ Σv[x1..x2)[y1..y2) を追加する．
	void sum(ll x1, ll x2, ll y1, ll y2) {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		x1_sum.emplace_back(x1);
		x2_sum.emplace_back(x2);
		y1_sum.emplace_back(y1);
		y2_sum.emplace_back(y2);
	}

	// 各クエリに対する答えを格納したリストを返す．
	vector<T> solve() {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		// ys : y 座標のユニークな昇順列
		vl ys(y_add);
		uniq(ys);

		// (x 座標, イベントタイプ, クエリ番号) の組
		vector<tuple<ll, int, int>> ev;
		const int DE = 0; // 長方形の下辺
		const int UE = 1; // 長方形の上辺
		const int PT = 2; // 点
		rep(i, sz(x_add)) {
			ev.emplace_back(x_add[i], PT, i);
		}
		int q = sz(x1_sum);
		rep(t, q) {
			ev.emplace_back(x1_sum[t], UE, t);
			ev.emplace_back(x2_sum[t], DE, t);
		}

		// イベントソート
		sort(all(ev));

		fenwick_tree<T> fen(sz(ys));
		vector<T> res(q, 0);

		// 下方向に平面走査していく．
		for (auto& [x, tp, id] : ev) {
			// 点への加算の場合
			if (tp == PT) {
				int y = lbpos(ys, y_add[id]);
				fen.add(y, w_add[id]);
			}
			// 総和クエリの上辺の場合
			else if (tp == UE) {
				int y1 = lbpos(ys, y1_sum[id]);
				int y2 = lbpos(ys, y2_sum[id]);
				res[id] -= fen.sum(y1, y2);
			}
			// 総和クエリの下辺の場合
			else if (tp == DE) {
				int y1 = lbpos(ys, y1_sum[id]);
				int y2 = lbpos(ys, y2_sum[id]);
				res[id] += fen.sum(y1, y2);
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


//【ウェーブレット行列（点群）】
/*
* Wavelet_matrix_points<S, T>(vS x, vS y, vT v) : O(n log n)
*	大きさ n の重み付き点群 ((x[i], y[i]), v[i]) で初期化する．
*
* S get(S x0, S x1, int i) : O(log n)
*	x∈[x0..x1) なる点のうち，y 座標昇順で i 番目の点の y 座標を返す（なければ INFL）
*
* int count(S x0, S x1, S y0, S y1) : O(log n)
*	[x0..x1)×[y0..y1) 内の点の個数を返す．
*
* T sum(S x0, S x1, S y0, S y1) : O(log n)
*	[x0..x1)×[y0..y1) 内の点の重みの和を返す．
*/
template <class S, class T>
class Wavelet_matrix_points {
	// 参考 : https://miti-7.hatenablog.com/entry/2018/04/28/152259

	int n; // 要素数
	int m; // msb 以下の桁数
	vi bs; // bs[i][j] : 第 j+1 ビットについての安定ソート後の y[i] の第 j ビット
	array<vvi, 2> bs_acc; // bs_acc[b] : bs[*][b] のビット b=0,1 それぞれの個数の累積和
	vi num_zeros; // num_zeros[j] : bs[j] の 0 の個数
	vector<vector<T>> acc; // acc[j] : 第 j ビットについての安定ソート後の w の累積和
	vector<S> x_sort; // x 座標の昇順列
	vector<S> y_uniq; // y 座標のユニークな昇順列

	// a[l..r) の中で [0..v) に値をもつ要素の個数を返す．
	int count_rsub(int l, int r, int v) {
		int cnt = 0;
		repir(j, m - 1, 0) {
			if (getb(v, j)) {
				cnt += bs_acc[0][j][r] - bs_acc[0][j][l];
				r = num_zeros[j] + bs_acc[1][j][r];
				l = num_zeros[j] + bs_acc[1][j][l];
			}
			else {
				r = bs_acc[0][j][r];
				l = bs_acc[0][j][l];
			}
		}

		return cnt;
	}

	// a[l..r) の中で [0..v) に値をもつ要素の和を返す．
	T sum_rsub(int l, int r, int v) {
		T res = 0;
		repir(j, m - 1, 0) {
			if (getb(v, j)) {
				res += acc[j][bs_acc[0][j][r]] - acc[j][bs_acc[0][j][l]];
				r = num_zeros[j] + bs_acc[1][j][r];
				l = num_zeros[j] + bs_acc[1][j][l];
			}
			else {
				r = bs_acc[0][j][r];
				l = bs_acc[0][j][l];
			}
		}

		return res;
	}

public:
	// 大きさ n の重み付き点群 ((x[i], y[i]), v[i]) で初期化する．
	Wavelet_matrix_points(const vector<S>& x, const vector<S>& y, const vector<T>& v) : n(sz(x)) {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		// 点群を x 座標昇順にソートする．
		vector<pair<S, int>> xi(n);
		rep(i, n) xi[i] = { x[i], i };
		sort(all(xi));

		// y_uniq : y 座標のユニークな昇順列
		y_uniq = y;
		uniq(y_uniq);
		y_uniq.emplace_back((S)INFL + 1); // 番兵

		// x_sort : x 座標の昇順列
		x_sort.resize(n);

		// ycp_v : 座圧後の y 座標と重みの組の列
		vector<pair<int, T>> ycp_v(n);

		rep(i, n) {
			int id;
			tie(x_sort[i], id) = xi[i];
			ycp_v[i] = { lbpos(y_uniq, y[id]), v[id] };
		}

		// メモリ確保
		m = msb(sz(y_uniq)) + 1;
		bs.resize(n);
		bs_acc[0] = bs_acc[1] = vvi(m, vi(n + 1));
		num_zeros.resize(m);
		acc.assign(m + 1, vector<T>(n + 1));

		// j : 注目ビット位置（上位ビットから順に見ていく）
		repir(j, m - 1, 0) {
			rep(i, n) {
				// bs[i][j] : 注目ビットが 1 か
				bs[i] |= ycp_v[i].first & (1 << j);

				// ビット 0, 1 それぞれの個数の累積和を求める．
				rep(b, 2) bs_acc[b][j][i + 1] = bs_acc[b][j][i];
				int b = getb(ycp_v[i].first, j);
				bs_acc[b][j][i + 1]++;
				num_zeros[j] += 1 - b;

				// 重みの累積和を求める．
				acc[j + 1][i + 1] = acc[j + 1][i] + ycp_v[i].second;
			}

			// 注目ビットが 0 のものを左，1 のものを右に寄せる安定ソートを行う．
			vector<pair<int, T>> nycp_w0, nycp_w1;
			nycp_w0.reserve(num_zeros[j]);
			nycp_w1.reserve(n - num_zeros[j]);
			rep(i, n) {
				if (getb(ycp_v[i].first, j)) nycp_w1.push_back(ycp_v[i]);
				else nycp_w0.push_back(ycp_v[i]);
			}
			ycp_v.clear();
			repe(tmp, nycp_w0) ycp_v.push_back(tmp);
			repe(tmp, nycp_w1) ycp_v.push_back(tmp);
		}

		// 重みの累積和を求める．
		rep(i, n) acc[0][i + 1] = acc[0][i] + ycp_v[i].second;
	}
	Wavelet_matrix_points() : n(0), m(0) {}

	// x∈[x0..x1) なる点のうち，y 座標昇順で i 番目の点の y 座標を返す（なければ INFL）
	S get(S x0, S x1, int i) {
		int x0_cp = lbpos(x_sort, x0);
		int x1_cp = lbpos(x_sort, x1);
		if (x0_cp >= x1_cp) return S(INFL);
		int y_cp = 0;

		repir(j, m - 1, 0) {
			y_cp <<= 1;

			int cnt0 = bs_acc[0][j][x1_cp] - bs_acc[0][j][x0_cp];
			if (i >= cnt0) {
				y_cp++;
				x0_cp = num_zeros[j] + bs_acc[1][j][x0_cp];
				x1_cp = num_zeros[j] + bs_acc[1][j][x1_cp];
				i -= cnt0;
			}
			else {
				x0_cp = bs_acc[0][j][x0_cp];
				x1_cp = bs_acc[0][j][x1_cp];
			}
		}

		return y_cp < sz(y_uniq) ? y_uniq[y_cp] : S(INFL);
	}

	// [x0..x1)×[y0..y1) 内の点の個数を返す．
	int count(S x0, S x1, S y0, S y1) {
		int x0_cp = lbpos(x_sort, x0);
		int x1_cp = lbpos(x_sort, x1);
		int y0_cp = lbpos(y_uniq, y0);
		int y1_cp = lbpos(y_uniq, y1);
		if (x0_cp >= x1_cp || y0_cp >= y1_cp) return 0;

		return count_rsub(x0_cp, x1_cp, y1_cp) - count_rsub(x0_cp, x1_cp, y0_cp);
	}

	// [x0..x1)×[y0..y1) 内の点の重みの和を返す．
	T sum(S x0, S x1, S y0, S y1) {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum

		int x0_cp = lbpos(x_sort, x0);
		int x1_cp = lbpos(x_sort, x1);
		int y0_cp = lbpos(y_uniq, y0);
		int y1_cp = lbpos(y_uniq, y1);
		if (x0_cp >= x1_cp || y0_cp >= y1_cp) return 0;

		return sum_rsub(x0_cp, x1_cp, y1_cp) - sum_rsub(x0_cp, x1_cp, y0_cp);
	}
};


//【1 点加算 → 矩形和（アーベル群）】（遅い）
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
* void rectangle_add(ll x1, ll x2, ll y1, ll y2, T val) : O(1)
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


