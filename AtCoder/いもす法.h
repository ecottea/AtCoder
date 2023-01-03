#pragma once
#include "header.h"
#include "構造(木).h"
// ■■■■■ いもす法 ■■■■■


//【いもす法】
/*
* Imos<T>(int n) : O(n)
*	半開区間 [0, n) を 0 で初期化する．
*
* set(int l, int r, T val) : O(1)
*	半開区間 [l, r) に val を加算する準備を行う．
*
* void sum() : O(n)
*	実際の加算を行う．
*
* T [](int i) : O(1)
*	加算後の位置 i の値を得る．
*	制約 : sum() の後に呼び出さなければならない．
*/
template <class T>
class Imos {
	// 参考：https://imoz.jp/algorithms/imos_method.html

	int n;
	vector<T> v;

public:
	// [0, n) 上の a を 0 で初期化する．
	Imos(int n) : n(n), v(n + 1) {}

	// アクセス
	T const& operator[](int i) const { return v[i]; }
	T& operator[](int i) { return v[i]; }

	// 半開区間 [l, r) に val を加算する準備を行う．
	void set(int l, int r, T val) {
		// verify : https://atcoder.jp/contests/abc188/tasks/abc188_d

		chmax(l, 0);
		chmin(r, n);
		if (l >= r) return;

		v[l] += val;
		v[r] -= val;
	}

	// 実際の加算を行う．
	void sum() {
		// verify : https://atcoder.jp/contests/abc188/tasks/abc188_d

		rep(i, n) v[i + 1] += v[i];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Imos& imos) {
		rep(i, imos.n) os << imos[i] << " ";
		os << endl;
		return os;
	}
#endif
};


//【二次元いもす法（長方形）】
/*
* [0, h) * [0, w) 内の長方形領域に一定の値を加算する．
*
* Imos_2D(int h, int w) : O(h w)
*	[0, h) * [0, w) を 0 で初期化する．
*
* set(int x1, int y1, int x2, int y2, T val) : O(1)
*	[x1, x2) * [y1, y2) に val を加算する準備を行う．
*
* sum() : O(h w)
*	実際に加算を行う．
*
* T [][](int i, int j) : O(1)
*	加算後の位置 (i, j) の値を得る．
*	制約 : sum() の後に呼び出さなければならない．
*/
template <class T>
struct Imos_2D {
	// 参考：https://imoz.jp/algorithms/imos_method.html

	int h, w;
	vector<vector<T>> v;

	// [0, h) * [0, w) を 0 で初期化する．
	Imos_2D(int h_, int w_) : h(h_), w(w_), v(h + 1, vector<T>(w + 1)) {}

	// アクセス
	vector<T> const& operator[](int i) const { return v[i]; }
	vector<T>& operator[](int i) { return v[i]; }

	// [x1, x2) * [y1, y2) に val を加算する準備を行う．O(1)
	void set(int x1, int y1, int x2, int y2, T val) {
		// verify : https://atcoder.jp/contests/abc276/tasks/abc276_h

		v[x1][y1] += val;
		v[x1][y2] -= val;
		v[x2][y1] -= val;
		v[x2][y2] += val;
	}

	// 実際の加算を行う．O(h w)
	void sum() {
		// verify : https://atcoder.jp/contests/abc276/tasks/abc276_h

		repi(i, 1, h) repi(j, 0, w) v[i][j] += v[i - 1][j];
		repi(i, 0, h) repi(j, 1, w) v[i][j] += v[i][j - 1];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Imos_2D& imos) {
		rep(i, imos.h) {
			rep(j, imos.w) os << imos[i][j] << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【二次元いもす法（長方形，三角形）】
/*
* [0, h) * [0, w) 内の長方形または三角形領域に一定の値を加算する．
*
* Imos_2D_tri<T>(int h, int w) : O(h w)
*	[0, h) * [0, w) を 0 で初期化する．
*
* set_rect(int x1, int y1, int x2, int y2, T val) : O(1)
*	[x1, x2] * [y1, y2] に val を加算する準備を行う．
*
* set_tri(int x, int y, int d, T val) : O(1)
*	[x, x+d] * [y, y+d] の対角線以下に val を加算する準備を行う．
*
* sum() : O(h w)
*	実際に加算を行う．
*
* T [][](int i, int j) : O(1)
*	加算後の位置 (i, j) の値を得る．
*	制約 : sum() の後に呼び出さなければならない．
*
* pii size() : O(1)
*	(高さ, 幅) を返す．
*/
template <class T>
struct Imos_2D_tri {
	// 参考：https://imoz.jp/algorithms/imos_method.html

	using vT = vector<T>; using vvT = vector<vT>;

	int h, w;
	vvT v;

	// [0, h) * [0, w) を 0 で初期化する．
	Imos_2D_tri(int h_, int w_) : h(h_), w(w_), v(h + 2, vT(w + 2)) {}

	// アクセス
	vT const& operator[](int i) const { return v[i]; }
	vT& operator[](int i) { return v[i]; }

	// (高さ, 幅) を返す．
	pii size() const { return make_pair(h, w); }

	// 長方形 [x1, x2] * [y1, y2] に val を加算する準備を行う．O(1)
	void set_rect(int x1, int y1, int x2, int y2, T val) {
		// 左上の角
		v[x1][y1] += val;
		v[x1 + 1][y1 + 1] -= val;

		// 左下の角
		v[x2 + 2][y1 + 1] += val;
		v[x2 + 1][y1] -= val;

		// 右上の角
		v[x1 + 1][y2 + 2] += val;
		v[x1][y2 + 1] -= val;

		// 右下の角
		v[x2 + 1][y2 + 1] += val;
		v[x2 + 2][y2 + 2] -= val;
	}

	// 正方形 [x, y] * [x + d, y + d] の対角線以下に val を加算する準備を行う．O(1)
	void set_tri(int x, int y, int d, T val) {
		// verify : https://atcoder.jp/contests/joi2012ho/tasks/joi2012ho4

		// 左上の角
		v[x][y] += val;
		v[x][y + 1] -= val;

		// 左下の角
		v[x + d + 2][y + 1] += val;
		v[x + d + 1][y] -= val;

		// 右下の角
		v[x + d + 1][y + d + 2] += val;
		v[x + d + 2][y + d + 2] -= val;
	}

	// 実際の加算を行う．O(h w)
	void sum() {
		// verify : https://atcoder.jp/contests/joi2012ho/tasks/joi2012ho4

		// 下方向への累積和
		repi(i, 1, h) repi(j, 0, w) v[i][j] += v[i - 1][j];

		// 右方向への累積和
		repi(i, 0, h) repi(j, 1, w) v[i][j] += v[i][j - 1];

		// 右下がり方向への累積和
		repi(i, 1, h) repi(j, 1, w) v[i][j] += v[i - 1][j - 1];

		// 不要な部分の削除
		v.resize(h);
		rep(i, h) v[i].resize(w);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Imos_2D_tri imos) {
		rep(i, imos.size().first) {
			rep(j, imos.size().second) os << imos[i][j] << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【三次元いもす法（直方体）】
/*
* [0, h) * [0, w) * [0, d) 内の直方体領域に一定の値を加算する．
*
* Imos_3D(int h, int w, int d) : O(h w d)
*	[0, h) * [0, w) * [0, d) を 0 で初期化する．
*
* set(int x1, int y1, int z1, int x2, int y2, int z2, T val) : O(1)
*	[x1, x2) * [y1, y2) * [z1, z2) に val を加算する準備を行う．
*
* sum() : O(h w d)
*	実際に加算を行う．
*
* T [][][](int i, int j, int k) : O(1)
*	加算後の位置 (i, j, k) の値を得る．
*	制約 : sum() の後に呼び出さなければならない．
*/
template <class T>
struct Imos_3D {
	// verify : https://atcoder.jp/contests/joi2013yo/tasks/joi2013yo_e
	
	int h, w, d;
	vector<vector<vector<T>>> v;

	// [0, h) * [0, w) * [0, d) を 0 で初期化する．
	Imos_3D(int h_, int w_, int d_) : h(h_), w(w_), d(d_),
		v(h + 1, vector<vector<T>>(w + 1, vector<T>(d + 1))) {}

	// アクセス
	vector<vector<T>> const& operator[](int i) const { return v[i]; }
	vector<vector<T>>& operator[](int i) { return v[i]; }

	// [x1, x2) * [y1, y2) * [z1, z2) に val を加算する準備を行う．O(1)
	void set(int x1, int y1, int z1, int x2, int y2, int z2, T val) {
		v[x1][y1][z1] += val;
		v[x1][y1][z2] -= val;
		v[x1][y2][z1] -= val;
		v[x2][y1][z1] -= val;
		v[x1][y2][z2] += val;
		v[x2][y1][z2] += val;
		v[x2][y2][z1] += val;
		v[x2][y2][z2] -= val;
	}

	// 実際の加算を行う．O(h w d)
	void sum() {
		repi(i, 1, h) {
			repi(j, 0, w) {
				repi(k, 0, d) {
					v[i][j][k] += v[i - 1][j][k];
				}
			}
		}
		repi(i, 0, h) {
			repi(j, 1, w) {
				repi(k, 0, d) {
					v[i][j][k] += v[i][j - 1][k];
				}
			}
		}
		repi(i, 0, h) {
			repi(j, 0, w) {
				repi(k, 1, d) {
					v[i][j][k] += v[i][j][k - 1];
				}
			}
		}
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Imos_3D& imos) {
		rep(i, imos.h) {
			rep(j, imos.w) {
				rep(k, imos.d) {
					os << imos[i][j] << " ";
				}
				os << endl;
			}
			os << endl;
		}
		return os;
	}
#endif
}; 


//【線形いもす法】
/*
* Linear_imos<T>(int n) : O(n)
*	半開区間 [0, n) を 0 で初期化する．
*
* set(int l, int r, T a, T b) : O(1)
*	i∈[l, r) に a i + b を一括加算する準備を行う．
*
* set_right(int l, int r, T w0, T w1) : O(1)
*	[l, r) に昇順に等差数列 v0, v1, ... を一括加算する準備を行う．
*
* set_left(int l, int r, T w0, T w1) : O(1)
*	(l, r] に降順に等差数列 v0, v1, ... を一括加算する準備を行う．
*
* void sum() : O(n)
*	実際の加算を行う．
*
* T [](int i) : O(1)
*	加算後の位置 i の値を得る．
*	制約 : sum() の後に呼び出さなければならない．
*/
template <class T>
class Linear_imos {
	int n;
	vector<vector<T>> v; // v[t] : 添字の t 次の係数

public:
	// [0, n) 上の a を 0 で初期化する．
	Linear_imos(int n_) : n(n_), v(2, vector<T>(n + 1)) {}

	// アクセス
	T const& operator[](int i) const { return v[0][i]; }
	T& operator[](int i) { return v[0][i]; }

	// i∈[l, r) に a i + b を一括加算する準備を行う．
	void set(int l, int r, T a, T b) {
		chmax(l, 0);  chmin(r, n);
		if (l >= r) return;

		v[0][l] += b;
		v[0][r] -= b;
		v[1][l] += a;
		v[1][r] -= a;
	}

	// [l, r) に昇順に等差数列 v0, v1, ... を一括加算する準備を行う．
	void set_right(int l, int r, T w0, T w1) {
		// verify : https://atcoder.jp/contests/abc268/tasks/abc268_e

		// a l + b = w0, a(l+1) + b = w1 を解いて a, b を求める．
		ll a = w1 - w0;
		ll b = w0 - a * l;
		set(l, r, a, b);
	}

	// (l, r] に降順に等差数列 v0, v1, ... を一括加算する準備を行う．
	void set_left(int r, int l, T w0, T w1) {
		// verify : https://atcoder.jp/contests/abc268/tasks/abc268_e

		// a r + b = w0, a(r-1) + b = w1 を解いて a, b を求める．
		ll a = w0 - w1;
		ll b = w0 - a * r;
		set(l + 1, r + 1, a, b);
	}

	// 実際の加算を行う．
	void sum() {
		// verify : https://atcoder.jp/contests/abc268/tasks/abc268_e

		// 正しい係数になるよう累積和をとる．
		rep(i, n) {
			v[0][i + 1] += v[0][i];
			v[1][i + 1] += v[1][i];
		}

		// 計算して項を 1 つにまとめる．
		rep(i, n) v[0][i] += v[1][i] * i;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Linear_imos imos) {
		imos.sum();
		rep(i, imos.n) os << imos[i] << " ";
		return os;
	}
#endif
};


//【木上のいもす法】
/*
* 木クエリ.h へ
*/

