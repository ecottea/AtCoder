#pragma once
#include "header.h"
// ■■■■■ フェニック木 ■■■■■



//【フェニック木】
/*
* Fenwick_tree(n) : O(n)
*	要素数 n かつ初期値 e で初期化する．
*	要素はアーベル群 (S, op, e, inv) の元とする．
*
* Fenwick_tree(a) : O(n)
*	配列 a で初期化する．
*
* set(i, x) : O(log n) // 遅いので apply が使えるならそちらを使うべき
*	v[i] = x とする．
*
* get(i) : O(log n)
*	v[i] を返す．
*
* prod(l, r) : O(log n)
*	op( v[l..r) ) を返す．空なら e() を返す．
*
* apply(i, x) : O(log n)
*	v[i] = op(v[i], x) とする．
*
* max_right<f>() : O(log n)
*	f( op( v[0..r) ) ) = true となる最大の r を返す．
*   f : S → bool で f(e()) = true かつ単調とする．
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
struct Fenwick_tree {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数（要素数 + 1）
	int n;

	// v[i] : op( [*..i] ) の値（i ： 1-indexed，v[0] は使わない）
	vector<S> v;


	// コンストラクタ（初期化なし）
	Fenwick_tree() {}

	// 要素数 n かつ初期値 e で初期化
	Fenwick_tree(int n_) : n(n_ + 1), v(n, e()) {}

	// 配列 a で初期化
	Fenwick_tree(const vector<S>& v_) : n(sz(v_) + 1), v(n) {
		// 配列の値を仮登録する．
		rep(i, n - 1) {
			v[i + 1] = v_[i];
		}

		// 正しい値になるよう根に向かって累積 op() をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[i] = op(v[i], v[i - pow2]);
			}
		}
	}


	// v[i] = x とする．（i : 0-indexed）
	void set(int i, S x) {
		// 差分を求める．
		S d = op(x, inv(get(i)));

		apply(i, d);
	}

	// v[i] を返す．（i : 0-indexed）
	S get(int i) const {
		return prod(i, i + 1);
	}

	// op( v[l..r) ) を返す．空なら e を返す．（l, r : 0-indexed）
	S prod(int l, int r) const {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return prod_sub(r) - prod_sub(l);
	}

	// op( v[1..r] ) を返す．空なら e を返す．（r : 1-indexed）
	S prod_sub(int r) const {
		S res = e();

		// 子に向かって累積 op() をとっていく．
		while (r > 0) {
			res = op(res, v[r]);

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// v[i] = op(v[i], x) とする．（i : 0-indexed）
	void apply(int i, S x) {
		// i を 1-indexed に直す．
		i++;

		// 根に向かって値を op() していく．
		while (i < n) {
			v[i] = op(v[i], x);

			// i の最下位ビットに 1 を加算することで次の位置を得る．
			i += i & -i;
		}
	}

	// f( op( v[0, r) ) ) = true となる最大の r を返す．（r : 0-indexed）
	int max_right(const function<bool(S)>& f) const {
		S x = e();

		// 注目している閉区間は [l+1, r] で幅は len
		int l = 0;
		for (int len = 1 << msb(n - 1); len > 0; len = len >> 1) {
			int r = l + len;

			if (r < n && f(op(x, v[r]))) {
				x = op(x, v[r]);
				l = r;
			}
		}
		return l;
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Fenwick_tree& ft) {
		rep(i, ft.n - 1) {
			os << ft.get(i) << " ";
		}
		return os;
	}
};


//【遅延評価フェニック木】
/*
* Lazy_fenwick_tree(n) : O(n)
*	要素数 n かつ初期値 e で初期化する．
*	要素は Z 加群 (S, op, e, inv) の元とする．
*	x ∈ S を k 個 op() したものを mul(x, k) とする．
*
* Lazy_fenwick_tree(a) : O(n)
*	配列 a で初期化する．
*
* set(i, x) : O(log n) // 遅いので apply が使えるならそちらを使うべき
*	v[i] = x とする．
*
* get(i) : O(log n)
*	v[i] を返す．
*
* prod(l, r) : O(log n)
*	op( v[l..r) ) を返す．空なら e() を返す．
*
* apply(i, x) : O(log n)
*	v[i] = op(v[i], x) とする．
*
* apply(l, r, x) : O(log n)
*	v[l..r) = op(v[l..r), x) とする．
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S), S(*mul)(S, int)>
struct Lazy_fenwick_tree {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数（要素数 + 1）
	int n;

	// op( [1..i] ) を acc0[i] op mul(acc1[i], i) と分解する．
	// さらに accd[i] = op( rawd[1..i] ) と表されるような rawd を導入する．
	// v[d] : op( rawd[*..i] ) の値（i ： 1-indexed，v[d][0] は使わない）
	vector<vector<S>> v;

	// コンストラクタ（初期化なし）
	Lazy_fenwick_tree() : n(0) {}

	// 要素数 n かつ初期値 e で初期化
	Lazy_fenwick_tree(int n_) : n(n_ + 1), v(2, vector<S>(n, e())) {}

	// 配列 a で初期化
	Lazy_fenwick_tree(const vector<S>& v_) : n(sz(v_) + 1), v(2, vector<S>(n, e())) {
		// 配列の値を仮登録する．
		rep(i, n - 1) v[0][i + 1LL] = v_[i];

		// 正しい値になるよう根に向かって累積 op() をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[0][i] = op(v[0][i], v[0][(ll)i - pow2]);
			}
		}
	}

	// v[i] = x とする．（i : 0-indexed）
	void set(int i, S x) {
		// 差分を求める．
		S d = op(x, inv(get(i)));

		apply(i, d);
	}

	// v[i] を返す．（i : 0-indexed）
	S get(int i) const {
		return prod(i, i + 1);
	}

	// op( v[l..r) ) を返す．空なら e を返す．（l, r : 0-indexed）
	S prod(int l, int r) const {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return prod_sub(r) - prod_sub(l);
	}

	// op( v[1..r] ) を返す．空なら e を返す．（r : 1-indexed）
	S prod_sub(int r) const {
		return prod_sub(r, 0) + mul(prod_sub(r, 1), r);
	}

	// op( v[d][1..r] ) を返す．空なら e を返す．（r : 1-indexed）
	S prod_sub(int r, int d) const {
		S res = e();

		// 子に向かって累積 op() をとっていく．
		while (r > 0) {
			res = op(res, v[d][r]);

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// v[i] = op(v[i], x) とする．（i : 0-indexed）
	void apply(int i, S x) {
		// i を 1-indexed に直す．
		i++;

		apply_sub(i, x, 0);
	}

	// v[l..r) = op(v[l..r), x) とする．（l, r : 0-indexed） 
	void apply(int l, int r, S x) {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		l++;

		// 区間の端の値を調整する．
		apply_sub(l, mul(inv(x), l - 1), 0);
		apply_sub(r + 1, mul(x, r), 0);
		apply_sub(l, x, 1);
		apply_sub(r + 1, inv(x), 1);
	}

	// v[d][i] = op(v[d][i], x) とする．（i : 1-indexed）
	void apply_sub(int i, S x, int d) {
		// 根に向かって値を op() していく．
		while (i < n) {
			v[d][i] = op(v[d][i], x);

			// i の最下位ビットに 1 を加算することで次の位置を得る．
			i += i & -i;
		}
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Lazy_fenwick_tree& ft) {
		rep(i, ft.n - 1) os << ft.get(i) << " ";
		return os;
	}
};


//【区間加算／区間総和クエリ】
/*
* 利用：【遅延評価フェニック木】
*/
template <class T> T op8(T x, T y) { return x + y; }
template <class T> T e8() { return T(0); }
template <class T> T inv8(T x) { return -x; }
template <class T> T mul8(T f, int i) { return f * i; }
template <class T> using RASQ = Lazy_fenwick_tree<T, op8<T>, e8<T>, inv8<T>, mul8<T>>;


//【二次元フェニック木】
/*
* Fenwick_tree_2d(n) : O(h w)
*	要素数 h * w かつ初期値 e で初期化する．
*	要素はアーベル群 (S, op, e, inv) の元とする．
*
* Fenwick_tree_2d(a) : O(h w)
*	二次元配列 a で初期化する．
*
* set(x, y, val) : O(log h log w) // 遅いので apply が使えるならそちらを使うべき
*	v[x][y] = val とする．
*
* get(x, y) : O(log h log w)
*	v[x][y] を返す．
*
* prod(x1, y1, x2, y2) : O(log h log w)
*	op( v[x1..x2)[y1..y2) ) を返す．空なら e() を返す．
*
* apply(x, y, val) : O(log h log w)
*	v[x][y] = op(v[x][y], val) とする．
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
struct Fenwick_tree_2d {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数（要素数 + 1）
	int h, w;

	// v[x][y] : op( [*..x][*..y] ) の値（x, y ： 1-indexed，v[0][*], v[*][0] は使わない）
	vector<vector<S>> v;


	// コンストラクタ（初期化なし）
	Fenwick_tree_2d() {}

	// 要素数 n かつ初期値 e で初期化
	Fenwick_tree_2d(int h_, int w_) : h(h_ + 1), w(w_ + 1), v(h, vector<S>(w, e())) {}

	// 配列 a で初期化
	Fenwick_tree_2d(const vector<vector<S>>& v_) : h(sz(v_) + 1), w(sz(v_[0]) + 1),
		v(h, vector<S>(w, e())) {
		// 配列の値を仮登録する．
		rep(i, h - 1) {
			rep(j, w - 1) {
				v[i + 1][j + 1] = v_[i][j];
			}
		}

		// 正しい値になるよう根に向かって累積 op() をとっていく．
		// j 方向
		repi(i, 1, h - 1) {
			for (int pow2 = 1; 2 * pow2 < w; pow2 *= 2) {
				for (int j = 2 * pow2; j < w; j += 2 * pow2) {
					v[i][j] = op(v[i][j], v[i][j - pow2]);
				}
			}
		}
		// i 方向
		repi(j, 1, w - 1) {
			for (int pow2 = 1; 2 * pow2 < h; pow2 *= 2) {
				for (int i = 2 * pow2; i < h; i += 2 * pow2) {
					v[i][j] = op(v[i][j], v[i - pow2][j]);
				}
			}
		}
	}

	// v[x][y] = val とする．（x, y : 0-indexed）
	void set(int x, int y, S val) {
		// 差分を求める．
		S d = op(val, inv(get(x, y)));

		apply(x, y, d);
	}

	// v[x][y] を返す．（ix, y : 0-indexed）
	S get(int x, int y) const {
		return prod(x, y, x + 1, y + 1);
	}

	// op( v[x1..x2)[y1..y2) ) を返す．空なら e を返す．（x1, y1, x2, y2 : 0-indexed）
	S prod(int x1, int y1, int x2, int y2) const {
		// 0-indexed での半開長方形 [x1..x2) × [y1..y2) は，
		// 1-indexed での閉長方形 [x1+1..x2] × [y1+1..y2] に対応する．
		S res = e();
		res = op(res, prod_sub(x2, y2));
		res = op(res, inv(prod_sub(x2, y1)));
		res = op(res, inv(prod_sub(x1, y2)));
		res = op(res, prod_sub(x1, y1));
		return res;
	}

	// op( v[1..x][1..y] ) を返す．空なら e を返す．（x, y : 1-indexed）
	S prod_sub(int x, int y) const {
		S res = e();

		// 子に向かって累積 op() をとっていく．
		// i, j の最下位ビットから 1 を減算することで次の位置を得る．
		for (int i = x; i > 0; i -= i & -i) {
			for (int j = y; j > 0; j -= j & -j) {
				res = op(res, v[i][j]);
			}
		}

		return res;
	}

	// v[x][y] = op(v[x][y], val) とする．（x, y : 0-indexed）
	void apply(int x, int y, S val) {
		// x, y を 1-indexed に直す．
		x++; y++;

		// 根に向かって値を op() していく．
		// i, j の最下位ビットに 1 を加算することで次の位置を得る．
		for (int i = x; i < h; i += i & -i) {
			for (int j = y; j < w; j += j & -j) {
				v[i][j] = op(v[i][j], val);
			}
		}
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Fenwick_tree_2d& ft) {
		rep(x, ft.h - 1) {
			rep(y, ft.w - 1) {
				os << ft.get(x, y) << " ";
			}
			cout << "\n";
		}
		return os;
	}
};


