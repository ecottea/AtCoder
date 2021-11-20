#pragma once
#include "header.h"
// ■■■■■ [遅延評価]フェニック木 ■■■■■


//【フェニック木】
/*
* Fenwick_tree(n) : O(n)
*	要素数 n かつ初期値 o で初期化する．
*	要素はアーベル群 T = (S, op, o, inv) の元とする．
*
* Fenwick_tree(a) : O(n)
*	配列 a で初期化する．
*
* set(i, x) : O(log n) // 遅いので add が使えるならそちらを使うべき
*	v[i] = x とする．
*
* get(i) : O(log n)
*	v[i] を返す．
*
* sum(l, r) : O(log n)
*	Σv[l..r) を返す．空なら o を返す．
*
* add(i, x) : O(log n)
*	v[i] += x とする．
*
* max_right<f>() : O(log n)
*	f( Σv[0..r) ) = true となる最大の r を返す．
*   f : S → bool で f(o) = true かつ単調とする．
*/
template <class T> struct Fenwick_tree {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数（要素数 + 1）
	int n;

	// v[i] : Σ[*..i] の値（i ： 1-indexed，v[0] は使わない）
	vector<T> v;

	// コンストラクタ（初期化なし）
	Fenwick_tree() {}

	// 要素数 n かつ初期値 o で初期化
	Fenwick_tree(int n_) : n(n_ + 1), v(n) {}

	// 配列 a で初期化
	Fenwick_tree(const vector<T>& v_) : n(sz(v_) + 1), v(n) {
		// 配列の値を仮登録する．
		rep(i, n - 1) v[i + 1] = v_[i];

		// 正しい値になるよう根に向かって累積和をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[i] += v[i - pow2];
			}
		}
	}

	// v[i] = x とする．（i : 0-indexed）
	void set(int i, T x) { add(i, x - get(i)); }

	// v[i] を返す．（i : 0-indexed）
	T get(int i) const { return sum(i, i + 1); }

	// Σv[l..r) を返す．空なら o を返す．（l, r : 0-indexed）
	T sum(int l, int r) const {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return sum_sub(r) - sum_sub(l);
	}

	// Σv[1..r] を返す．空なら o を返す．（r : 1-indexed）
	T sum_sub(int r) const {
		T res;

		// 子に向かって累積和をとっていく．
		while (r > 0) {
			res += v[r];

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// v[i] += x とする．（i : 0-indexed）
	void add(int i, T x) {
		// i を 1-indexed に直す．
		i++;

		// 根に向かって値を足していく．
		while (i < n) {
			v[i] += x;

			// i の最下位ビットに 1 を加算することで次の位置を得る．
			i += i & -i;
		}
	}

	// f( Σv[0, r) ) = true となる最大の r を返す．（r : 0-indexed）
	int max_right(const function<bool(T)>& f) const {
		T x;

		// 注目している閉区間は [l+1, r] で幅は len
		int l = 0;
		for (int len = 1 << msb(n - 1); len > 0; len >>= 1) {
			int r = l + len;

			if (r < n && f(x + v[r])) {
				x += v[r];
				l = r;
			}
		}
		return l;
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Fenwick_tree& ft) {
		rep(i, ft.n - 1) os << ft.get(i) << " ";
		return os;
	}
};


//【遅延評価フェニック木】
/*
* Lazy_fenwick_tree(n) : O(n)
*	要素数 n かつ初期値 o で初期化する．
*	要素は Z-加群 T = (S, op, e, inv, mul) の元とする．
*
* Lazy_fenwick_tree(a) : O(n)
*	配列 a で初期化する．
*
* set(i, x) : O(log n) // 遅いので add が使えるならそちらを使うべき
*	v[i] = x とする．
*
* get(i) : O(log n)
*	v[i] を返す．
*
* sum(l, r) : O(log n)
*	Σv[l..r) を返す．空なら o を返す．
*
* add(i, x) : O(log n)
*	v[i] += x とする．
*
* add(l, r, x) : O(log n)
*	v[l..r) += x とする．
*/
template <class T> struct Lazy_fenwick_tree {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数（要素数 + 1）
	int n;

	// Σ[1..i] を acc0[i] + i * acc1[i] と分解する．
	// さらに accd[i] = Σrawd[1..i] と表されるような rawd を導入する．（d = 0,1）
	// v[d] : Σrawd[*..i] の値（i ： 1-indexed，v[d][0] は使わない）
	vector<vector<T>> v;

	// コンストラクタ（初期化なし）
	Lazy_fenwick_tree() : n(0) {}

	// 要素数 n かつ初期値 o で初期化
	Lazy_fenwick_tree(int n_) : n(n_ + 1), v(2, vector<T>(n)) {}

	// 配列 a で初期化
	Lazy_fenwick_tree(const vector<T>& v_) : n(sz(v_) + 1), v(2, vector<T>(n)) {
		// 配列の値を仮登録する．
		rep(i, n - 1) v[0][i + 1] = v_[i];

		// 正しい値になるよう根に向かって累積和をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[0][i] += v[0][i - pow2];
			}
		}
	}

	// v[i] = x とする．（i : 0-indexed）
	void set(int i, T x) { add(i, x - get(i)); }

	// v[i] を返す．（i : 0-indexed）
	T get(int i) const { return sum(i, i + 1); }

	// Σv[l..r) を返す．空なら o を返す．（l, r : 0-indexed）
	T sum(int l, int r) const {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return sum_sub(r) - sum_sub(l);
	}

	// Σv[1..r] を返す．空なら o を返す．（r : 1-indexed）
	T sum_sub(int r) const { return sum_sub(r, 0) + r * sum_sub(r, 1); }

	// Σv[d][1..r] を返す．空なら o を返す．（r : 1-indexed）
	T sum_sub(int r, int d) const {
		T res;

		// 子に向かって累積和をとっていく．
		while (r > 0) {
			res += v[d][r];

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// v[i] += x とする．（i : 0-indexed）
	void add(int i, T x) {
		// i を 1-indexed に直す．
		i++;

		add_sub(i, x, 0);
	}

	// v[l..r) += x とする．（l, r : 0-indexed） 
	void add(int l, int r, T x) {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		l++;

		// 区間の端の値を調整する．
		add_sub(l, (l - 1) * (-x), 0);
		add_sub(r + 1, r * x, 0);
		add_sub(l, x, 1);
		add_sub(r + 1, -x, 1);
	}

	// v[d][i] += x とする．（i : 1-indexed）
	void add_sub(int i, T x, int d) {
		// 根に向かって値を op() していく．
		while (i < n) {
			v[d][i] += x;

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


//【二次元フェニック木】
/*
* Fenwick_tree_2d(n) : O(h w)
*	要素数 h * w かつ初期値 o で初期化する．
*	要素はアーベル群 T = (S, op, o, inv) の元とする．
*
* Fenwick_tree_2d(a) : O(h w)
*	二次元配列 a で初期化する．
*
* set(x, y, val) : O(log h log w) // 遅いので add が使えるならそちらを使うべき
*	v[x][y] = val とする．
*
* get(x, y) : O(log h log w)
*	v[x][y] を返す．
*
* sum(x1, y1, x2, y2) : O(log h log w)
*	ΣΣv[x1..x2)[y1..y2) を返す．空なら o を返す．
*
* add(x, y, val) : O(log h log w)
*	v[x][y] += val とする．
*/
template <class T> struct Fenwick_tree_2d {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数（要素数 + 1）
	int h, w;

	// v[x][y] : ΣΣ[*..x][*..y] の値（x, y ： 1-indexed，v[0][*], v[*][0] は使わない）
	vector<vector<T>> v;

	// コンストラクタ（初期化なし）
	Fenwick_tree_2d() {}

	// 要素数 n かつ初期値 e で初期化
	Fenwick_tree_2d(int h_, int w_) : h(h_ + 1), w(w_ + 1), v(h, vector<T>(w)) {}

	// 配列 a で初期化
	Fenwick_tree_2d(const vector<vector<T>>& v_) : h(sz(v_) + 1), w(sz(v_[0]) + 1),
		v(h, vector<T>(w)) {
		// 配列の値を仮登録する．
		rep(i, h - 1) {
			rep(j, w - 1) {
				v[i + 1][j + 1] = v_[i][j];
			}
		}

		// 正しい値になるよう根に向かって累積和をとっていく．
		// j 方向
		repi(i, 1, h - 1) {
			for (int pow2 = 1; 2 * pow2 < w; pow2 *= 2) {
				for (int j = 2 * pow2; j < w; j += 2 * pow2) {
					v[i][j] += v[i][j - pow2];
				}
			}
		}
		// i 方向
		repi(j, 1, w - 1) {
			for (int pow2 = 1; 2 * pow2 < h; pow2 *= 2) {
				for (int i = 2 * pow2; i < h; i += 2 * pow2) {
					v[i][j] += v[i - pow2][j];
				}
			}
		}
	}

	// v[x][y] = val とする．（x, y : 0-indexed）
	void set(int x, int y, T val) { add(x, y, val - get(x, y)); }

	// v[x][y] を返す．（ix, y : 0-indexed）
	T get(int x, int y) const { return sum(x, y, x + 1, y + 1); }

	// ΣΣv[x1..x2)[y1..y2) を返す．空なら e を返す．（x1, y1, x2, y2 : 0-indexed）
	T sum(int x1, int y1, int x2, int y2) const {
		// 0-indexed での半開長方形 [x1..x2) × [y1..y2) は，
		// 1-indexed での閉長方形 [x1+1..x2] × [y1+1..y2] に対応する．
		T res;
		res += sum_sub(x2, y2);
		res -= sum_sub(x2, y1);
		res -= sum_sub(x1, y2);
		res += sum_sub(x1, y1);
		return res;
	}

	// ΣΣv[1..x][1..y] を返す．空なら o を返す．（x, y : 1-indexed）
	T sum_sub(int x, int y) const {
		T res;

		// 子に向かって累積和をとっていく．
		// i, j の最下位ビットから 1 を減算することで次の位置を得る．
		for (int i = x; i > 0; i -= i & -i) {
			for (int j = y; j > 0; j -= j & -j) {
				res += v[i][j];
			}
		}

		return res;
	}

	// v[x][y] += val とする．（x, y : 0-indexed）
	void add(int x, int y, T val) {
		// x, y を 1-indexed に直す．
		x++; y++;

		// 根に向かって値を加算していく．
		// i, j の最下位ビットに 1 を加算することで次の位置を得る．
		for (int i = x; i < h; i += i & -i) {
			for (int j = y; j < w; j += j & -j) {
				v[i][j] += val;
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


