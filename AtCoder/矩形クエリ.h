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
* void add(ll x, ll y, T val) : O(1)
*	v[x][y] += val とする．
*
* void sum(ll x1, ll x2, ll y1, ll y2) : O(1)
*	クエリ Σv[x1..x2)[y1..y2) を追加する．
*
* vT solve() : O((n + q) log(n + q))
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


//【ウェーブレット行列（点群，アーベル群）】
/*
* Wavelet_matrix_points<S, T, op, o, inv>(vS x, vS y, vT v) : O(n log n)
*	大きさ n の重み付き点群 ((x[i], y[i]), v[i]) で初期化する．
*	値はアーベル群 (T, op, o, inv) の要素とする．
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
template <class S, class T, T(*op)(T, T), T(*o)(), T(*inv)(T)>
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
		T res = o();
		repir(j, m - 1, 0) {
			if (getb(v, j)) {
				res = op(res, op(acc[j][bs_acc[0][j][r]], inv(acc[j][bs_acc[0][j][l]])));
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
		// verify : https://yukicoder.me/problems/no/3026
		
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
		acc.assign(m + 1, vector<T>(n + 1, o()));

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
				acc[j + 1][i + 1] = op(acc[j + 1][i], ycp_v[i].second);
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
		rep(i, n) acc[0][i + 1] = op(acc[0][i], ycp_v[i].second);
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
		// verify : https://yukicoder.me/problems/no/3026
		
		int x0_cp = lbpos(x_sort, x0);
		int x1_cp = lbpos(x_sort, x1);
		int y0_cp = lbpos(y_uniq, y0);
		int y1_cp = lbpos(y_uniq, y1);
		if (x0_cp >= x1_cp || y0_cp >= y1_cp) return o();

		return op(sum_rsub(x0_cp, x1_cp, y1_cp), inv(sum_rsub(x0_cp, x1_cp, y0_cp)));
	}
};


//【1 点加算／矩形和（アーベル群）】
/*
* 動的に与えられる 1 点加算／矩形和 の混合クエリを処理したい場合は
* セグメント木.h の【二次元動的セグメント木】を利用すればよい．
* 
* verify : https://judge.yosupo.jp/problem/point_add_rectangle_sum
*/


//【1 点加算 → 上開放矩形和（可換モノイド，一括）】
/*
* Offline_rectangle_Uopen_sum<S, op, o>() : O(1)
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
class Offline_rectangle_Uopen_sum {
	int w, q;

	// (x 座標，イベントタイプ，クエリ番号，左位置，右位置, 加算値) の組
	vector<tuple<ll, int, int, int, int, S>> ev;
	static constexpr int DE = 0; // 長方形の下辺
	static constexpr int PT = 1; // 点

public:
	// v[0..h)[0..w) = o() で初期化する（h, w は自動で調整される）
	Offline_rectangle_Uopen_sum() : w(1), q(0) {
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


//【1 点加算 → 左上開放矩形和（一括）】
/*
* Offline_rectangle_ULopen_sum<T>() : O(1)
*	v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
*
* void add(ll x, int y, T val) : O(1)
*	v[x][y] += val とする．
*
* void sum(ll x2, int y2) : O(1)
*	クエリ Σv[0..x2)[0..y2) を追加する．
*
* vT solve() : O((n + q) log(n + q))
*	現在の v[0..h)[0..w) への各クエリに対する答えを格納したリストを返す．
*
*（平面走査）
*/
template <class T>
class Offline_rectangle_ULopen_sum {
	vl x_add, y_add; vector<T> w_add;
	vl x2_sum, y2_sum;

public:
	// v[0..h)[0..w) = 0 で初期化する（h, w は自動で調整される）
	Offline_rectangle_ULopen_sum() {
	}

	// v[x][y] += val とする．
	void add(ll x, ll y, T val) {
		x_add.emplace_back(x);
		y_add.emplace_back(y);
		w_add.emplace_back(val);
	}

	// クエリ Σv[0..x2)[0..y2) を追加する．
	void sum(ll x2, ll y2) {
		x2_sum.emplace_back(x2);
		y2_sum.emplace_back(y2);
	}

	// 各クエリに対する答えを格納したリストを返す．
	vector<T> solve() {
		// ys : y 座標のユニークな昇順列
		vl ys(y_add);
		uniq(ys);

		// (x 座標, イベントタイプ, クエリ番号) の組
		vector<tuple<ll, int, int>> ev;
		const int DE = 0; // 長方形の下辺
		const int PT = 2; // 点
		rep(i, sz(x_add)) {
			ev.emplace_back(x_add[i], PT, i);
		}
		int q = sz(x2_sum);
		rep(t, q) {
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
			// 総和クエリの下辺の場合
			else if (tp == DE) {
				int y2 = lbpos(ys, y2_sum[id]);
				res[id] += fen.sum(0, y2);
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


//【2 次元 K-D 木（M-可換モノイド）】
/*
* KDTree_2D<T, S, op, e, F, act, comp, id>(vT x, vT y, vS v) : O(n log n)
*	大きさ n の重み付き点群 ((x[i], y[i]), v[i]) で初期化する．
*	v[i] は左作用付きモノイド (S, op, e, F, act, comp, id) の元とする．
*
* get(int i) : O(√n)
*	v[i] を返す．
*
* set(int i, S val) : O(√n)
*	v[i] ← val とする．
*
* S sum(T x1, T x2, T y1, T y2) : O(√n)
*	[x1..x2)×[y1..y2) 内の点の重みの和を返す．
*
* apply(T x1, T x2, T y1, T y2, F f) : O(√n)
*	[x1..x2)×[y1..y2) 内の点全ての重みに f を作用させる．
*
* call_rect(T x1, T x2, T y1, T y2, function<void(int, S)> g) : O(√n + K)
*	[x1..x2)×[y1..y2) 内の点 K 個全てに対して g(i, v[i]) を呼ぶ．
*
* tiSl nearest(T x, T y) : O(log n)
*	(x, y) にユークリッド距離で最も近い点の (i, v[i], dist^2) を返す．
*	制約：点群の配置は一様ランダム
*
* K_nearest(T x, T y, function<void(int, S)> g) : O(K log n) ?
*	(x, y) にユークリッド距離で近い順で K 個の点全てに対して g(i, v[i], dist^2) を呼ぶ．
*	制約：点群の配置は一様ランダム
*
* call_circle(T x, T y, T D, function<void(int, S)> g) : O(K log n) ?
*	(x, y) からのユークリッド距離が D 以内の K 個の点全てに対して g(i, v[i], dist^2) を呼ぶ．
*	制約：点群の配置は一様ランダム
*/
template <class T, class S, S(*op)(S, S), S(*o)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class KDTree_2D {
	// 参考 : https://trap.jp/post/1489/

	struct Node {
		int idx;          // 点の番号
		T x1, x2, y1, y2; // 矩形 [x1..x2]×[y1..y2] に対応するノードであることを表す
		S val;            // ノードの値
		F lazy;           // 遅延させている作用
		Node* lc, * rc;   // 左右の子へのポインタ
		Node* p;          // 親へのポインタ

		Node() : idx(-1), x1(T(INFL)), x2(-T(INFL)), y1(T(INFL)), y2(-T(INFL)),
			val(o()), lazy(id()), lc(nullptr), rc(nullptr), p(nullptr) {}
	};

	int n;                // 頂点数
	Node* root;           // 根へのポインタ
	vector<Node*> leaves; // 葉へのポインタ

	void init(Node*& t, int iL, int iR, bool divx, vector<tuple<T, T, S, int>>& ps) {
		// 要素が一つだけなら葉として格納して帰る．
		if (iR - iL == 1) {
			auto [x, y, v, idx] = ps[iL];

			t = new Node();
			t->idx = idx;
			t->x1 = t->x2 = x;
			t->y1 = t->y2 = y;
			t->val = v;

			leaves[idx] = t;

			return;
		}

		int iM = (iL + iR) / 2;

		if (divx) {
			auto cmp = [](const tuple<T, T, S, int>& lhs, const tuple<T, T, S, int>& rhs) {
				return std::get<0>(lhs) < std::get<0>(rhs);
			};
			nth_element(ps.begin() + iL, ps.begin() + iM, ps.begin() + iR, cmp);
		}
		else {
			auto cmp = [](const tuple<T, T, S, int>& lhs, const tuple<T, T, S, int>& rhs) {
				return std::get<1>(lhs) < std::get<1>(rhs);
			};
			nth_element(ps.begin() + iL, ps.begin() + iM, ps.begin() + iR, cmp);
		}

		t = new Node();
		init(t->lc, iL, iM, !divx, ps);
		init(t->rc, iM, iR, !divx, ps);
		t->x1 = min(t->lc->x1, t->rc->x1);
		t->x2 = max(t->lc->x2, t->rc->x2);
		t->y1 = min(t->lc->y1, t->rc->y1);
		t->y2 = max(t->lc->y2, t->rc->y2);
		t->val = op(t->lc->val, t->rc->val);

		t->lc->p = t;
		t->rc->p = t;
	}

	// 子をもつノード t が不変条件を満たすよう子ノードの val から再計算を行う．
	// 呼び出す際には，子の lazy がいずれも id() でなくてはならない．
	void update(Node* t) {
		// 参考 : https://qiita.com/ngtkana/items/4d0b84d45210771aa074

		t->val = op(t->lc->val, t->rc->val);
	}

	// ノード t の不変条件を満たしたまま lazy を id() に書き換える．
	// 呼び出す際には，部分木 t 内の全てのノードで不変条件が満たされなければならない．
	void eval(Node* t) {
		// 参考 : https://qiita.com/ngtkana/items/4d0b84d45210771aa074

		// 遅延させていた作用がなければ何もしない．
		if (t->lazy == id()) return;

		// 葉ならすぐに作用させる．
		if (!t->lc) {
			t->val = act(t->lazy, t->val);
			t->lazy = id();
			return;
		}

		// 遅延作用を子に移す．
		t->lc->lazy = comp(t->lazy, t->lc->lazy);
		t->rc->lazy = comp(t->lazy, t->rc->lazy);

		// 自身の値に遅延させていた作用を適用する．
		t->val = act(t->lazy, t->val);
		t->lazy = id();
	}

	// 部分木 t 内の矩形 [x1..x2]×[y1..y2] との共通部分に属する要素の和を返す．
	S sum(Node* t, T x1, T x2, T y1, T y2) {
		eval(t);

		// 注目矩形とクエリ範囲が共通部分をもたない場合
		if (x2 < t->x1 || t->x2 < x1 || y2 < t->y1 || t->y2 < y1) return o();

		// 注目矩形がクエリ範囲に包まれている場合
		if (x1 <= t->x1 && t->x2 <= x2 && y1 <= t->y1 && t->y2 <= y2) return t->val;

		// 左右の子を見にいって値を求め，その和を返す．
		return op(sum(t->lc, x1, x2, y1, y2), sum(t->rc, x1, x2, y1, y2));
	}

	// 部分木 t 内の矩形 [x1..x2]×[y1..y2] との共通部分に属する要素に f を作用させる．
	void apply(Node* t, T x1, T x2, T y1, T y2, F f) {
		eval(t);

		// 注目矩形とクエリ範囲が共通部分をもたない場合
		if (x2 < t->x1 || t->x2 < x1 || y2 < t->y1 || t->y2 < y1) return;

		// 注目矩形がクエリ範囲に包まれている場合
		if (x1 <= t->x1 && t->x2 <= x2 && y1 <= t->y1 && t->y2 <= y2) {
			t->lazy = f;
			return;
		}

		// 左右の子に f を作用させる．
		apply(t->lc, x1, x2, y1, y2, f);
		apply(t->rc, x1, x2, y1, y2, f);

		eval(t->lc);
		eval(t->rc);
		update(t);
	}

	// 部分木 t 内の矩形 [x1..x2]×[y1..y2] との共通部分に属する点全てを res に格納する．
	template <class FUNC>
	void call_rect(Node* t, T x1, T x2, T y1, T y2, const FUNC& g) {
		eval(t);

		// 注目矩形とクエリ範囲が共通部分をもたない場合
		if (x2 < t->x1 || t->x2 < x1 || y2 < t->y1 || t->y2 < y1) return;

		// 葉であれば点に対して g を呼ぶ．
		if (t->idx != -1) {
			g(t->idx, t->val);
			return;
		}

		// 左右の子を見にいく．
		call_rect(t->lc, x1, x2, y1, y2, g);
		call_rect(t->rc, x1, x2, y1, y2, g);
	}

	// ノード t に対応する矩形と点 (x, y) との距離の 2 乗を返す．
	ll sq_dist(Node* t, T x, T y) {
		T dx = clamp(x, t->x1, t->x2) - x;
		T dy = clamp(y, t->y1, t->y2) - y;
		return (ll)dx * dx + (ll)dy * dy;
	}

	void nearest(Node* t, T x, T y, ll& d_min, int& idx, S& val) {
		// d : 注目矩形とクエリ点との距離の 2 乗
		ll d = sq_dist(t, x, y);

		// 暫定の最短距離以上であれば先を調べず帰る．
		if (d >= d_min) return;

		eval(t);

		// 葉なら暫定の最短距離を更新して帰る．
		if (t->idx != -1) {
			d_min = d;
			idx = t->idx;
			val = t->val;
			return;
		}

		// 左右の子を調べにいく．
		if (sq_dist(t->lc, x, y) < sq_dist(t->rc, x, y)) {
			nearest(t->lc, x, y, d_min, idx, val);
			nearest(t->rc, x, y, d_min, idx, val);
		}
		else {
			nearest(t->rc, x, y, d_min, idx, val);
			nearest(t->lc, x, y, d_min, idx, val);
		}
	}

	struct cmp_pq {
		bool operator()(const tuple<ll, int, S>& lhs, const tuple<ll, int, S>& rhs) const {
			return std::get<0>(lhs) < std::get<0>(rhs);
		}
	};
	using PQ = priority_queue<tuple<ll, int, S>, vector<tuple<ll, int, S>>, cmp_pq>;

	void K_nearest(Node* t, T x, T y, PQ& q) {
		// d : 注目矩形とクエリ点との距離の 2 乗
		ll d = sq_dist(t, x, y);

		// 暫定の最短距離以上であれば先を調べず帰る．
		if (d >= std::get<0>(q.top())) return;

		eval(t);

		// 葉なら暫定の最短距離を更新して帰る．
		if (t->idx != -1) {
			q.push({ d, t->idx, t->val });
			q.pop();
			return;
		}

		// 左右の子を調べにいく．
		if (sq_dist(t->lc, x, y) < sq_dist(t->rc, x, y)) {
			K_nearest(t->lc, x, y, q);
			K_nearest(t->rc, x, y, q);
		}
		else {
			K_nearest(t->rc, x, y, q);
			K_nearest(t->lc, x, y, q);
		}
	}

	template <class FUNC>
	void call_circle(Node* t, T x, T y, ll D_sq, const FUNC& g) {
		ll d = sq_dist(t, x, y);

		// 注目矩形とクエリ範囲が共通部分をもたない場合
		if (d > D_sq) return;

		eval(t);

		// 葉であれば点に対して g を呼ぶ．
		if (t->idx != -1) {
			g(t->idx, t->val, d);
			return;
		}

		// 左右の子を見にいく．
		call_circle(t->lc, x, y, D_sq, g);
		call_circle(t->rc, x, y, D_sq, g);
	}

#ifdef _MSC_VER
	void print(Node* t, ostream& os) {
		eval(t);

		// 葉なら出力する．
		if (!t->lc) {
			os << "(" << t->x1 << "," << t->y1 << "):" << t->val << " ";
			return;
		}

		print(t->lc, os);
		print(t->rc, os);
	}
#endif

public:
	// 大きさ n の重み付き点群 ((x[i], y[i]), v[i]) で初期化する．
	KDTree_2D(const vector<T>& x, const vector<T>& y, const vector<S>& v) : n(sz(x)), leaves(n) {
		// verify : https://judge.yosupo.jp/problem/dynamic_point_set_rectangle_affine_rectangle_sum

		Assert(n > 0);
		vector<tuple<T, T, S, int>> ps(n);
		rep(i, n) ps[i] = { x[i], y[i], v[i], i };

		init(root, 0, n, true, ps);
	}

	// v[i] ← val とする．
	void set(int i, S val) {
		// verify : https://judge.yosupo.jp/problem/dynamic_point_set_rectangle_affine_rectangle_sum

		Assert(0 <= i); Assert(i < n);

		auto t = leaves[i];

		vector<Node*> ts;
		ts.reserve(msb(n) + 2);
		while (t) {
			ts.push_back(t);
			t = t->p;
		}

		int K = sz(ts);

		repir(k, K - 1, 0) eval(ts[k]);

		ts[0]->val = val;

		repi(k, 1, K - 1) {
			eval(ts[k]->lc);
			eval(ts[k]->rc);
			update(ts[k]);
		}
	}

	// v[i] を返す．
	S get(int i) {
		// verify : https://atcoder.jp/contests/past202004-open/tasks/past202004_n

		Assert(0 <= i); Assert(i < n);

		auto t = leaves[i];

		vector<Node*> ts;
		ts.reserve(msb(n) + 2);
		while (t) {
			ts.push_back(t);
			t = t->p;
		}

		int K = sz(ts);

		repir(k, K - 1, 0) eval(ts[k]);

		return ts[0]->val;
	}

	// [x1..x2)×[y1..y2) 内の点の重みの和を返す．
	S sum(T x1, T x2, T y1, T y2) {
		// verify : https://judge.yosupo.jp/problem/dynamic_point_set_rectangle_affine_rectangle_sum

		if (x1 >= x2 || y1 >= y2) return o();
		return sum(root, x1, x2 - 1, y1, y2 - 1);
	}

	// [x1..x2)×[y1..y2) 内の点全てに f を作用させる．
	void apply(T x1, T x2, T y1, T y2, F f) {
		// verify : https://judge.yosupo.jp/problem/dynamic_point_set_rectangle_affine_rectangle_sum

		if (x1 >= x2 || y1 >= y2) return;
		apply(root, x1, x2 - 1, y1, y2 - 1, f);
	}

	// [x1..x2)×[y1..y2) 内の点全てに対して g(i, v[i]) を呼ぶ．
	template <class FUNC>
	void call_rect(T x1, T x2, T y1, T y2, const FUNC& g) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_C

		call_rect(root, x1, x2 - 1, y1, y2 - 1, g);
	}

	// (x, y) にユークリッド距離で最も近い点の (i, v[i], dist^2) を返す．
	tuple<int, S, ll> nearest(T x, T y) {
		// verify : https://yukicoder.me/problems/no/2909

		ll d_min = INFL; int idx = -1; S val = o();
		nearest(root, x, y, d_min, idx, val);
		return { idx, val, d_min };
	}

	// (x, y) にユークリッド距離で近い順で K 個の点全てに対して g(i, v[i]，dist^2) を呼ぶ．
	template <class FUNC>
	void K_nearest(T x, T y, int K, const FUNC& g) {
		// verify : https://judge.yosupo.jp/problem/closest_pair

		PQ q;
		rep(hoge, K) q.emplace(INFL, -1, o());
		K_nearest(root, x, y, q);

		while (!q.empty()) {
			auto [d, i, v] = q.top(); q.pop();
			g(i, v, d);
		}
	}

	// (x, y) からのユークリッド距離が D 以内の点全てに対して g(i, v[i]，dist^2) を呼ぶ．
	template <class FUNC>
	void call_circle(T x, T y, T D, const FUNC& g) {
		// verify : https://atcoder.jp/contests/abc234/tasks/abc234_h

		call_circle(root, x, y, (ll)D * D, g);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, KDTree_2D& kd) {
		kd.print(kd.root, os);
		return os;
	}
#endif

	/* FUNC の雛形
	using S = int;
	auto func = [&](int i, S x) {
		res.push_back(i);
	};
	KD.call(x1, x2, y1, y2, func);

	using S = int;
	auto func = [&](int i, S x, ll dist_sq) {
		res.push_back(i);
	};
	KD.call(x1, x2, y1, y2, func);
	*/
};


