#pragma once
#include "header.h"
// ■■■■■ [遅延評価]フェニック木（抽象代数上） ■■■■■


//【フェニック木（アーベル群）】
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
	// verify : https://judge.yosupo.jp/problem/point_add_range_sum

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


//【遅延評価フェニック木（Z-加群）】
/*
* Lazy_fenwick_tree<S, op, e, inv, mul>(int n) : O(n)
*	要素数 n かつ初期値 e で初期化する．
*	要素は Z 加群 (S, op, e, inv, mul) の元とする．
*
* Lazy_fenwick_tree<S, op, e, inv, mul>(vS a) : O(n)
*	配列 a で初期化する．
*
* set(int i, S x) : O(log n)
*	v[i] = x とする．
*
* S get(int i) : O(log n)
*	v[i] を返す．
*
* S prod(int l, int r) : O(log n)
*	op( v[l..r) ) を返す．空なら e() を返す．
*
* apply(int i, S x) : O(log n)
*	v[i] = op(v[i], x) とする．
*
* apply(int l, int r, S x) : O(log n)
*	v[l..r) = op(v[l..r), x) とする．
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S), S(*mul)(ll, S)>
struct Lazy_fenwick_tree {
	// 参考：https://algo-logic.info/binary-indexed-tree/
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_G

	// ノードの個数（要素数 + 1）
	int n;

	// op( [1..i] ) を acc0[i] + i acc1[i] と分解する．
	// さらに acc?[i] = Σraw?[1..i] と表されるような raw? を導入する．
	// v[?][i] : Σraw?[*..i] の値（i ： 1-indexed，v[?][0] は使わない）
	vector<vector<S>> v;

	// コンストラクタ（初期化なし）
	Lazy_fenwick_tree() : n(0) {}

	// 要素数 n かつ初期値 e で初期化
	Lazy_fenwick_tree(int n_) : n(n_ + 1), v(2, vector<S>(n, e())) {}

	// 配列 a で初期化
	Lazy_fenwick_tree(const vector<S>& v_) : n(sz(v_) + 1), v(2, vector<S>(n, e())) {
		// 配列の値を仮登録する．
		rep(i, n - 1) v[0][i + 1] = v_[i];

		// 正しい値になるよう根に向かって累積 op() をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[0][i] = op(v[0][i], v[0][i - pow2]);
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
		return op(prod_sub(r), inv(prod_sub(l)));
	}

	// op( v[1..r] ) を返す．空なら e を返す．（r : 1-indexed）
	S prod_sub(int r) const {
		return op(prod_sub(r, 0), mul(r, prod_sub(r, 1)));
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
		apply_sub(l, mul(l - 1, inv(x)), 0);
		apply_sub(r + 1, mul(r, x), 0);

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


//【二次元フェニック木（アーベル群）】
/*
* Fenwick_tree_2D<S, op, e, inv>(int h, int w) : O(h w)
*	要素数 h * w かつ初期値 e で初期化する．
*	要素はアーベル群 (S, op, e, inv) の元とする．
*
* Fenwick_tree_2D<S, op, e, inv>(vvS a) : O(h w)
*	二次元配列 a で初期化する．
*
* apply(int x, int y, S val) : O(log h log w)
*	v[x][y] = op(v[x][y], val) とする．
*
* set(int x, int y, S val) : O(log h log w)
*	v[x][y] = val とする．
*
* S get(int x, int y) : O(log h log w)
*	v[x][y] を返す．
*
* S prod(int x1, int y1, int x2, int y2) : O(log h log w)
*	op( v[x1..x2)[y1..y2) ) を返す．空なら e() を返す．
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
struct Fenwick_tree_2D {
	// 参考：https://algo-logic.info/binary-indexed-tree/
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/2842

	// ノードの個数（要素数 + 1）
	int h, w;

	// v[x][y] : op( [*..x][*..y] ) の値（x, y ： 1-indexed，v[0][*], v[*][0] は使わない）
	vector<vector<S>> v;

	// コンストラクタ（初期化なし）
	Fenwick_tree_2D() {}

	// 要素数 h * w かつ初期値 e で初期化
	Fenwick_tree_2D(int h_, int w_) : h(h_ + 1), w(w_ + 1), v(h, vector<S>(w, e())) {}

	// 配列 a で初期化
	Fenwick_tree_2D(const vector<vector<S>>& v_) : h(sz(v_) + 1), w(sz(v_[0]) + 1),
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

	// v[x][y] を返す．（x, y : 0-indexed）
	S get(int x, int y) const {
		return prod(x, y, x + 1, y + 1);
	}

	// op( v[x1..x2)[y1..y2) ) を返す．空なら e を返す．（x1, y1, x2, y2 : 0-indexed）
	S prod(int x1, int y1, int x2, int y2) const {
		// 0-indexed での半開長方形 [x1..x2) * [y1..y2) は，
		// 1-indexed での閉長方形 [x1+1..x2] * [y1+1..y2] に対応する．
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
	friend ostream& operator<<(ostream& os, const Fenwick_tree_2D& ft) {
		rep(x, ft.h - 1) {
			rep(y, ft.w - 1) {
				os << ft.get(x, y) << " ";
			}
			cout << "\n";
		}
		return os;
	}
};


//【二次元遅延評価フェニック木（Z 加群）】
/*
* Lazy_fenwick_tree_2D<S, op, e, inv, mul>(int h, int w) : O(h w)
*	要素数 h * w かつ初期値 e で初期化する．
*	要素は Z 加群 (S, op, e, inv, mul) の元とする．
*
* Lazy_fenwick_tree_2D<S, op, e, inv, mul>(vvS a) : O(h w)
*	二次元配列 a で初期化する．
*
* apply(int x, int y, S val) : O(log h log w)
*	v[x][y] = op(v[x][y], val) とする．
*
* apply(int x1, int y1, int x2, int y2, S val) : O(log h log w)
*	v[x1..x2)[y1..y2) = op(v[x1..x2)[y1..y2), val) とする．
*
* set(int x, int y, S val) : O(log h log w)
*	v[x][y] = val とする．
*
* S get(int x, int y) : O(log h log w)
*	v[x][y] を返す．
*
* S prod(int x1, int y1, int x2, int y2) : O(log h log w)
*	op( v[x1..x2)[y1..y2) ) を返す．空なら e() を返す．
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S), S(*mul)(ll, S)>
struct Lazy_fenwick_tree_2D {
	// verify : https://codeforces.com/contest/869/problem/E

	// ノードの個数（要素数 + 1）
	int h, w;

	// op( [1..i][1..j] ) を acc0[i][j] + i acc1[i][j] + j acc2[i][j] + i j acc3[i][j] と分解する．
	// さらに acc?[i][j] = Σraw?[1..i][1..j] と表されるような raw? を導入する．
	// v[?][i][j] : Σraw?[*..i][*..j] の値（i, j ： 1-indexed，v[*][0], v[0][*] は使わない）
	using vS = vector<S>; using vvS = vector<vS>; using vvvS = vector<vvS>;
	vvvS v;

	// 要素数 h * w かつ初期値 e で初期化
	Lazy_fenwick_tree_2D(int h_, int w_) : h(h_ + 1), w(w_ + 1), v(4, vvS(h, vS(w, e()))) {}

	// 配列 a で初期化
	Lazy_fenwick_tree_2D(const vector<vector<S>>& v_) : h(sz(v_) + 1), w(sz(v_[0]) + 1), v(4, vvS(h, vS(w, e()))) {
		// 配列の値を仮登録する．
		rep(i, h - 1) {
			rep(j, w - 1) {
				v[0][i + 1][j + 1] = v_[i][j];
			}
		}

		// 正しい値になるよう根に向かって累積 op() をとっていく．
		// j 方向
		repi(i, 1, h - 1) {
			for (int pow2 = 1; 2 * pow2 < w; pow2 *= 2) {
				for (int j = 2 * pow2; j < w; j += 2 * pow2) {
					v[0][i][j] = op(v[0][i][j], v[0][i][j - pow2]);
				}
			}
		}
		// i 方向
		repi(j, 1, w - 1) {
			for (int pow2 = 1; 2 * pow2 < h; pow2 *= 2) {
				for (int i = 2 * pow2; i < h; i += 2 * pow2) {
					v[0][i][j] = op(v[0][i][j], v[0][i - pow2][j]);
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

	// v[x][y] を返す．（x, y : 0-indexed）
	S get(int x, int y) const {
		return prod(x, y, x + 1, y + 1);
	}

	// op( v[x1..x2)[y1..y2) ) を返す．空なら e を返す．（x1, y1, x2, y2 : 0-indexed）
	S prod(int x1, int y1, int x2, int y2) const {
		// 0-indexed での半開長方形 [x1..x2) * [y1..y2) は，
		// 1-indexed での閉長方形 [x1+1..x2] * [y1+1..y2] に対応する．
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
		res = op(res, prod_sub(x, y, 0));
		res = op(res, mul(x, prod_sub(x, y, 1)));
		res = op(res, mul(y, prod_sub(x, y, 2)));
		res = op(res, mul((ll)x * y, prod_sub(x, y, 3)));
		return res;
	}

	// op( v[d][1..x][1..y] ) を返す．空なら e を返す．（x, y : 1-indexed）
	S prod_sub(int x, int y, int d) const {
		S res = e();

		// 子に向かって累積 op() をとっていく．
		// i, j の最下位ビットから 1 を減算することで次の位置を得る．
		for (int i = x; i > 0; i -= i & -i) {
			for (int j = y; j > 0; j -= j & -j) {
				res = op(res, v[d][i][j]);
			}
		}

		return res;
	}

	// v[x][y] = op(v[x][y], val) とする．（x, y : 0-indexed）
	void apply(int x, int y, S val) {
		// x, y を 1-indexed に直す．
		x++; y++;

		apply_sub(x, y, val, 0);
	}

	// v[x1..x2)[y1..y2) = op(v[x1..x2)[y1..y2), val) とする．（x, y : 0-indexed）
	void apply(int x1, int y1, int x2, int y2, S val) {
		// 0-indexed での半開長方形 [x1..x2) * [y1..y2) は，
		// 1-indexed での閉長方形 [x1+1..x2] * [y1+1..y2] に対応する．
		x1++; y1++;

		// 区間の端の値を調整する．
		apply_sub(x1, y1, mul((ll)(x1 - 1) * (y1 - 1), val), 0);
		apply_sub(x1, y2 + 1, mul((ll)(x1 - 1) * y2, inv(val)), 0);
		apply_sub(x2 + 1, y1, mul((ll)x2 * (y1 - 1), inv(val)), 0);
		apply_sub(x2 + 1, y2 + 1, mul((ll)x2 * y2, val), 0);

		apply_sub(x1, y1, mul(y1 - 1, inv(val)), 1);
		apply_sub(x1, y2 + 1, mul(y2, val), 1);
		apply_sub(x2 + 1, y1, mul(y1 - 1, val), 1);
		apply_sub(x2 + 1, y2 + 1, mul(y2, inv(val)), 1);

		apply_sub(x1, y1, mul(x1 - 1, inv(val)), 2);
		apply_sub(x1, y2 + 1, mul(x1 - 1, val), 2);
		apply_sub(x2 + 1, y1, mul(x2, val), 2);
		apply_sub(x2 + 1, y2 + 1, mul(x2, inv(val)), 2);

		apply_sub(x1, y1, val, 3);
		apply_sub(x1, y2 + 1, inv(val), 3);
		apply_sub(x2 + 1, y1, inv(val), 3);
		apply_sub(x2 + 1, y2 + 1, val, 3);
	}

	// v[d][x][y] = op(v[d][x][y], val) とする．（x, y : 1-indexed）
	void apply_sub(int x, int y, S val, int d) {
		// 根に向かって値を op() していく．
		// i, j の最下位ビットに 1 を加算することで次の位置を得る．
		for (int i = x; i < h; i += i & -i) {
			for (int j = y; j < w; j += j & -j) {
				v[d][i][j] = op(v[d][i][j], val);
			}
		}
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Lazy_fenwick_tree_2D& ft) {
		rep(x, ft.h - 1) {
			rep(y, ft.w - 1) {
				os << ft.get(x, y) << " ";
			}
			cout << "\n";
		}
		return os;
	}
};


//【フェニック木（ACL 仕様）】
/*
* fenwick_tree(n) : O(n)
*	要素数 n かつ初期値 0 で初期化する．
*
* add(p, val) : O(log n)
*	v[p] に val を加算する．
*
* sum(l, r) : O(log n)
*	半開区間 [l, r) の要素の総和を返す．
*/
template <class T> struct fenwick_tree {
	int n;
	vector<T> v;

	fenwick_tree() {}
	fenwick_tree(int n_) : n(n_ + 1), v(n) {}
	void add(int p, T val) {
		p++;
		while (p < n) {
			v[p] += val;
			p += p & -p;
		}
	}
	T sum_sub(int r) {
		T res = 0;
		while (r > 0) {
			res += v[r];
			r -= r & -r;
		}
		return res;
	}
	T sum(int l, int r) {
		return sum_sub(r) - sum_sub(l);
	}
};


