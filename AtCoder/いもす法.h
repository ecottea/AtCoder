#pragma once
#include "header.h"
#include "構造(木).h"
// ■■■■■ いもす法 ■■■■■


//【いもす法】
/*
* Imos<T>(int n) : O(n)
*	a[0..n) = 0 で初期化する．
*
* add(int l, int r, T val) : O(1)
*	a[l..r) += val とする準備を行う．
*
* void execute() : O(n)
*	実際の加算を行う．
*
* T [](int i) : O(1)
*	a[i] を返す．
*	制約 : 先に execute() を呼び出すこと．
*/
template <class T>
class Imos {
	// 参考：https://imoz.jp/algorithms/imos_method.html

	int n;
	vector<T> v;
	bool ex = false;

public:
	// a[0..n) = 0 で初期化する．
	Imos(int n) : n(n), v(n + 1) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_g
	}
	Imos() : n(0) {}

	// アクセス
	inline T const& operator[](int i) const { return v[i]; }
	inline T& operator[](int i) { return v[i]; }

	// a[l..r) += val とする準備を行う．
	void add(int l, int r, T val) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_g

		chmax(l, 0);
		chmin(r, n);
		if (l >= r) return;

		v[l] += val;
		v[r] -= val;
	}

	// 実際の加算を行う．
	void execute() {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_g

		rep(i, n) v[i + 1] += v[i];
		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos a) {
		if (!a.ex) a.execute();
		rep(i, a.n) os << a[i] << " ";
		return os;
	}
#endif
};


//【間引きいもす法】
/*
* Thinning_imos<T>(int n, int m) : O(n + m)
*	法を m とし，a[0..n) = 0 で初期化する．
*
* add(int l, int r, int k, T val) : O(1)
*	S = {i∈[l..r) | i=k (mod m)} とし a[S] += val とする準備を行う．
*
* void execute() : O(n)
*	実際の加算を行う．
*
* T [](int i) : O(1)
*	a[i] を返す．
*	制約 : 先に execute() を呼び出すこと．
*/
template <class T>
class Thinning_imos {
	int n, m;
	vector<T> v;
	bool ex = false;

public:
	// 法を m とし，a[0..n) = 0 で初期化する．
	Thinning_imos(int n, int m) : n(n), m(m), v(n + m) {
		// verify : https://yukicoder.me/problems/no/2359
	}
	Thinning_imos() : n(0), m(1) {}

	// アクセス
	inline T const& operator[](int i) const { return v[i]; }
	inline T& operator[](int i) { return v[i]; }

	// S = {i∈[l..r) | i=k (mod m)} とし a[S] += val とする準備を行う．
	void add(int l, int r, int k, T val) {
		// verify : https://yukicoder.me/problems/no/2359

		chmax(l, 0); chmin(r, n);
		if (l >= r) return;

		r += smod(k - r, m);
		l += smod(k - l, m);
		v[l] += val;
		v[r] -= val;
	}

	// 実際の加算を行う．
	void execute() {
		// verify : https://yukicoder.me/problems/no/2359

		rep(i, n) v[i + m] += v[i];
		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Thinning_imos a) {
		if (!a.ex) a.execute();
		rep(i, a.n) os << a[i] << " ";
		return os;
	}
#endif
};


//【二次元いもす法（長方形）】
/*
* Imos_2D<T>(int h, int w) : O(h w)
*	a[0..h)[0..w) = 0 で初期化する．
*
* add(int x1, int x2, int y1, int y2, T val) : O(1)
*	a[x1..x2)[y1..y2) += val とする準備を行う．
*
* execute() : O(h w)
*	実際に加算を行う．
*
* T [][](int i, int j) : O(1)
*	a[i][j] を返す．
*	制約 : 先に execute() を呼び出すこと．
*/
template <class T>
class Imos_2D {
	// 参考：https://imoz.jp/algorithms/imos_method.html

	int h, w;
	vector<vector<T>> v;
	bool ex = false;

public:
	// a[0..h)[0..w) = 0 で初期化する．
	Imos_2D(int h_, int w_) : h(h_), w(w_), v(h + 1, vector<T>(w + 1)) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_i
	}
	Imos_2D() : h(0), w(0) {}

	// アクセス
	inline vector<T> const& operator[](int i) const { return v[i]; }
	inline vector<T>& operator[](int i) { return v[i]; }

	// a[x1..x2)[y1..y2) += val とする準備を行う．
	void add(int x1, int x2, int y1, int y2, T val) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_i

		v[x1][y1] += val;
		v[x1][y2] -= val;
		v[x2][y1] -= val;
		v[x2][y2] += val;
	}

	// 実際の加算を行う．
	void execute() {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_i

		repi(i, 1, h) repi(j, 0, w) v[i][j] += v[i - 1][j];
		repi(i, 0, h) repi(j, 1, w) v[i][j] += v[i][j - 1];
		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos_2D a) {
		if (!a.ex) a.execute();
		rep(i, a.h) rep(j, a.w) os << a[i][j] << " \n"[j == a.w - 1];
		return os;
	}
#endif
};


//【二次元いもす法（長方形，三角形）】
/*
* Imos_2D_tri<T>(int h, int w) : O(h w)
*	a[0..h)[0..w) = 0 で初期化する．
*
* add_rect(int x1, int x2, int y1, int y2, T val) : O(1)
*	a[x1..x2)[y1..y2) += val とする準備を行う．
*
* add_tri(int x, int y, int d, T val) : O(1)
*	a[x..x+d][y..y+d] の対角線以下に val を加算する準備を行う．
*
* execute() : O(h w)
*	実際に加算を行う．
*
* T [][](int i, int j) : O(1)
*	a[i][j] を返す．
*	制約 : 先に execute() を呼び出すこと．
*
* pii size() : O(1)
*	(高さ, 幅) を返す．
*/
template <class T>
class Imos_2D_tri {
	// 参考 : https://imoz.jp/algorithms/imos_method.html

	using vT = vector<T>; using vvT = vector<vT>;

	int h, w;
	vvT v;
	bool ex = false;

public:
	// a[0..h)[0..w) = 0 で初期化する．
	Imos_2D_tri(int h_, int w_) : h(h_), w(w_), v(h + 2, vT(w + 2)) {}
	Imos_2D_tri() : h(0), w(0) {}

	// アクセス
	inline vT const& operator[](int i) const { return v[i]; }
	inline vT& operator[](int i) { return v[i]; }

	// (高さ, 幅) を返す．
	pii size() const { return make_pair(h, w); }

	// a[x1..x2)[y1..y2) += val とする準備を行う．
	void add_rect(int x1, int x2, int y1, int y2, T val) {
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

	// a[x..x+d][y..y+d] の対角線以下に val を加算する準備を行う．
	void add_tri(int x, int y, int d, T val) {
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
	void execute() {
		// verify : https://atcoder.jp/contests/joi2012ho/tasks/joi2012ho4

		// 下方向への累積和
		repi(i, 1, h) repi(j, 0, w) v[i][j] += v[i - 1][j];

		// 右方向への累積和
		repi(i, 0, h) repi(j, 1, w) v[i][j] += v[i][j - 1];

		// 右下がりの斜め方向への累積和
		repi(i, 1, h) repi(j, 1, w) v[i][j] += v[i - 1][j - 1];

		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos_2D_tri a) {
		if (!a.ex) a.execute();
		rep(i, a.h) rep(j, a.w) os << a[i][j] << " \n"[j == a.w - 1];
		return os;
	}
#endif
};


//【二次元間引きいもす法】
/*
* Thinning_imos_2D<T>(int h, int w, int mx, int my) : O((h + mx)(w + my))
*	法を (mx, my) とし，a[0..h)[0..w) = 0 で初期化する．
*
* add(int x1, int x2, int y1, int y2, int kx, int ky, T val) : O(1)
*	set = {(i,j)∈[x1..x2)×[y1..y2) | i=kx (mod mx), j=ky (mod my)} とし a[set] += val とする準備を行う．
*
* void execute() : O(n)
*	実際の加算を行う．
*
* T [](int x, int y) : O(1)
*	a[x][y] を返す．
*	制約 : 先に execute() を呼び出すこと．
*/
template <class T>
class Thinning_imos_2D {
	int h, w;
	int mx, my; // 法

	using vT = vector<T>;
	using vvT = vector<vT>;

	vvT v;
	bool ex = false;

public:
	// 法を (mx, my) とし，a[0..h)[0..w) = 0 で初期化する．
	Thinning_imos_2D(int h, int w, int mx, int my) : h(h), w(w), mx(mx), my(my), v(h + mx, vT(w + my)) {
		// verify : https://atcoder.jp/contests/arc178/tasks/arc178_d
	}
	Thinning_imos_2D() : h(0), w(0), mx(1), my(1) {}

	// アクセス
	inline vT const& operator[](int i) const { return v[i]; }
	inline vT& operator[](int i) { return v[i]; }

	// set = {(i,j)∈[x1..x2)×[y1..y2) | i=kx (mod mx), j=ky (mod my)} とし a[set] += val とする準備を行う．
	void add(int x1, int x2, int y1, int y2, int kx, int ky, T val) {
		// verify : https://atcoder.jp/contests/arc178/tasks/arc178_d

		chmax(x1, 0); chmin(x2, h); chmax(y1, 0); chmin(y2, w);
		if (x1 >= x2 || y1 >= y2) return;

		x1 += smod(kx - x1, mx);
		x2 += smod(kx - x2, mx);
		y1 += smod(ky - y1, my);
		y2 += smod(ky - y2, my);

		v[x1][y1] += val;
		v[x1][y2] -= val;
		v[x2][y1] -= val;
		v[x2][y2] += val;
	}

	// 実際の加算を行う．
	void execute() {
		// verify : https://atcoder.jp/contests/arc178/tasks/arc178_d

		rep(i, h) rep(j, w + my) v[i + mx][j] += v[i][j];
		rep(i, h + mx) rep(j, w) v[i][j + my] += v[i][j];
		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Thinning_imos_2D a) {
		if (!a.ex) a.execute();
		rep(i, a.n) os << a[i] << " ";
		return os;
	}
#endif
};


//【三次元いもす法（直方体）】
/*
* Imos_3D(int h, int w, int d) : O(h w d)
*	a[0..h)[0..w)[0..d) = 0 で初期化する．
*
* add(int x1, int x2, int y1, int y2, int z1, int z2, T val) : O(1)
*	a[x1..x2)[y1..y2)[z1..z2) += val とする準備を行う．
*
* execute() : O(h w d)
*	実際に加算を行う．
*
* T [][][](int i, int j, int k) : O(1)
*	a[i][j][k] を返す．
*	制約 : 先に execute() を呼び出すこと．
*/
template <class T>
class Imos_3D {
	// verify : https://atcoder.jp/contests/joi2013yo/tasks/joi2013yo_e

	using vT = vector<T>; using vvT = vector<vT>; using vvvT = vector<vvT>;

	int h, w, d;
	vvvT v;
	bool ex = false;

public:
	// a[0..h)[0..w)[0..d) = 0 で初期化する．
	Imos_3D(int h_, int w_, int d_) : h(h_), w(w_), d(d_), v(h + 1, vvT(w + 1, vT(d + 1))) {}
	Imos_3D() : h(0), w(0), d(0) {}

	// アクセス
	inline vvT const& operator[](int i) const { return v[i]; }
	inline vvT& operator[](int i) { return v[i]; }

	// a[x1..x2)[y1..y2)[z1..z2) += val とする準備を行う．
	void add(int x1, int x2, int y1, int y2, int z1, int z2, T val) {
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
	void execute() {
		repi(i, 1, h) repi(j, 0, w) repi(k, 0, d) v[i][j][k] += v[i - 1][j][k];
		repi(i, 0, h) repi(j, 1, w) repi(k, 0, d) v[i][j][k] += v[i][j - 1][k];
		repi(i, 0, h) repi(j, 0, w) repi(k, 1, d) v[i][j][k] += v[i][j][k - 1];
		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos_3D a) {
		if (!a.ex) a.execute();
		rep(i, a.h) {
			rep(j, a.w) {
				rep(k, a.d) {
					os << a[i][j][k] << " ";
				}
				os << endl;
			}
			os << endl;
		}
		return os;
	}
#endif
};


//【線形加重いもす法】
/*
* Linear_imos<T>(int n) : O(n)
*	v[0..n) = 0 で初期化する．
*
* add(int l, int r, T a, T b) : O(1)
*	v[l..r) に v[i] += a i + b とする準備を行う．
*
* add_right(ll l, ll r, T w0, T w1) : O(1)
*	v[l..r) に昇順に等差数列 w0, w1, ... を加算する準備を行う．
*
* add_left(ll r, ll l, T w0, T w1) : O(1)
*	v(l..r] に降順に等差数列 w0, w1, ... を加算する準備を行う．
*
* void execute() : O(n)
*	実際の加算を行う．
*
* T [](int i) : O(1)
*	v[i] を返す．
*	制約 : 先に execute() を呼び出すこと．
*/
template <class T>
class Linear_imos {
	int n;
	array<vector<T>, 2> v; // v[t] : 添字の t 次の係数
	bool ex = false;

public:
	// v[0..n) = 0 で初期化する．
	Linear_imos(int n) : n(n) {
		// verify : https://atcoder.jp/contests/abc268/tasks/abc268_e

		v[0].assign(n + 1, 0);
		v[1].assign(n + 1, 0);
	}
	Linear_imos() : n(0) {}

	// アクセス
	inline T const& operator[](int i) const { return v[0][i]; }
	inline T& operator[](int i) { return v[0][i]; }

	// v[l..r) に v[i] += a i + b とする準備を行う．
	void add(int l, int r, T a, T b) {
		// verify : https://judge.yosupo.jp/problem/range_linear_add_range_min

		chmax(l, 0); chmin(r, n);
		if (l >= r) return;

		v[0][l] += b;
		v[0][r] -= b;
		v[1][l] += a;
		v[1][r] -= a;
	}

	// v[l..r) に昇順に等差数列 w0, w1, ... を加算する準備を行う．
	void add_right(ll l, ll r, T w0, T w1) {
		// verify : https://atcoder.jp/contests/abc268/tasks/abc268_e

		// a l + b = w0, a(l+1) + b = w1 を解いて a, b を求める．
		ll a = w1 - w0;
		ll b = w0 - a * l;
		add((int)max<ll>(l, 0), (int)min<ll>(r, n), a, b);
	}

	// v(l..r] に降順に等差数列 w0, w1, ... を加算する準備を行う．
	void add_left(ll r, ll l, T w0, T w1) {
		// verify : https://atcoder.jp/contests/abc268/tasks/abc268_e

		// a r + b = w0, a(r-1) + b = w1 を解いて a, b を求める．
		ll a = w0 - w1;
		ll b = w0 - a * r;
		add((int)max<ll>(l + 1, 0), (int)min<ll>(r + 1, n), a, b);
	}

	// 実際の加算を行う．
	void execute() {
		// verify : https://atcoder.jp/contests/abc268/tasks/abc268_e

		// 正しい係数になるよう累積和をとる．
		rep(i, n) {
			v[0][i + 1] += v[0][i];
			v[1][i + 1] += v[1][i];
		}

		// 計算して項を 1 つにまとめる．
		rep(i, n) v[0][i] += v[1][i] * i;

		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Linear_imos a) {
		if (!a.ex) a.execute();
		rep(i, a.n) os << a[i] << " ";
		return os;
	}
#endif
};


//【指数加重いもす法】（未 verify）
/*
* Exponential_imos<T>(int n, T B) : O(n)
*	v[0..n) = 0 と底 B で初期化する．
*
* add_right(ll l, ll r, T a) : O(1)
*	v[l..r) に昇順に等比数列 a, aB, ... を加算する準備を行う．
*
* void execute() : O(n)
*	実際の加算を行う．
*
* T [](int i) : O(1)
*	v[i] を返す．
*	制約 : 先に execute() を呼び出すこと．
*/
template <class T>
class Exponential_imos {
	int n; T B;
	vector<T> v, powB;
	bool ex = false;

public:
	// a[0..n) = 0 と底 B で初期化する．
	Imos(int n, T B) : n(n), B(B), v(n + 1), powB(n + 1) {
		powB[0] = 1;
		repi(i, 1, n) powB[i] = powB[i - 1] * B;
	}
	Imos() : n(0), B(1) {}

	// アクセス
	inline T const& operator[](int i) const { return v[i]; }
	inline T& operator[](int i) { return v[i]; }

	// v[l..r) に昇順に等比数列 a, aB, ... を加算する準備を行う．
	void add_right(int l, int r, T a) {
		Assert(0 <= l && l <= r && r <= n);

		v[l] += a;
		v[r] -= a * powB[r - l];
	}

	// 実際の加算を行う．
	void execute() {
		rep(i, n) v[i + 1] += v[i] * B;
		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos a) {
		if (!a.ex) a.execute();
		rep(i, a.n) os << a[i] << " ";
		return os;
	}
#endif
};


//【木上いもす法（根からのパス，部分木）】
/*
* Imos_tree<T>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と頂点の値 v[0..n) = 0 で初期化する．
*
* add_path(int s, T val) : O(1)
*	根から頂点 s へのパス上の頂点全てに val を加算する準備を行う．
*
* add_subtree(int s, T val) : O(1)
*	部分木 s 内の頂点全てに val を加算する準備を行う．
*
* execute() : O(n)
*	実際の加算を行う．
*
* T [](int s) : O(1)
*	v[s] を返す．
*	制約 : 先に execute() を呼び出すこと．
*/
template <class T>
class Imos_tree {
	int n; Graph g; int rt;
	vector<T> v_anc, v_dsc;
	bool ex = false;

	// 再帰用の関数
	T execute_sub(int s, int p, T val) {
		v_dsc[s] += val;
		repe(t, g[s]) if (t != p) v_anc[s] += execute_sub(t, s, v_dsc[s]);
		return v_anc[s];
	};

public:
	// 根付き木 rt を 0 で初期化する．
	Imos_tree(const Graph& g, int rt) : n(sz(g)), g(g), rt(rt), v_anc(n), v_dsc(n) {
		// verify : https://atcoder.jp/contests/abc138/tasks/abc138_d
	}
	Imos_tree() : n(0), rt(-1) {}

	// アクセス
	inline T const& operator[](int i) const { return v_anc[i]; }
	inline T& operator[](int i) { return v_anc[i]; }

	// 根から頂点 s へのパス上の頂点全てに val を加算する準備を行う．
	void add_path(int s, T val) {
		Assert(0 <= s && s < n);

		v_anc[s] += val;
	}

	// 部分木 s 内の頂点全てに val を加算する準備を行う．
	void add_subtree(int s, T val) {
		// verify : https://atcoder.jp/contests/abc138/tasks/abc138_d

		Assert(0 <= s && s < n);

		v_dsc[s] += val;
	}

	// 実際の加算を行う．
	void execute() {
		// verify : https://atcoder.jp/contests/abc138/tasks/abc138_d

		execute_sub(rt, -1, 0);

		// 計算結果をまとめておく．
		rep(s, n) v_anc[s] += v_dsc[s];

		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos_tree a) {
		if (!a.ex) a.execute();
		rep(i, a.n) os << a[i] << " ";
		return os;
	}
#endif
};


//【木の辺上いもす法（根からのパス，部分木）】
/*
* Imos_tree_edge<T>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と辺値 v[0..n) = 0 で初期化する．
*	辺値 v[s] は頂点 s に入る辺の値を表す（v[rt] は無視する）
*
* add_path(int s, T val) : O(1)
*	根から頂点 s へのパス上の辺全てに val を加算する準備を行う．
*
* add_subtree(int s, T val) : O(1)
*	部分木 s 内の辺全てに val を加算する準備を行う．
*
* execute() : O(n)
*	実際の加算を行う．
*
* T [](int s) : O(1)
*	v[s] を返す．
*	制約 : 先に execute() を呼び出すこと．
*/
template <class T>
class Imos_tree_edge {
	int n; Graph g; int rt;
	vector<T> v_anc, v_dsc, v_add;
	bool ex = false;

	// 再帰用の関数
	T execute_sub(int s, int p, T val) {
		v_dsc[s] += val;
		repe(t, g[s]) if (t != p) v_anc[s] += execute_sub(t, s, v_dsc[s]);
		return v_anc[s];
	};

public:
	// rt を根とする根付き木 g と辺値 v[0..n) = 0 で初期化する．
	Imos_tree_edge(const Graph& g, int rt) : n(sz(g)), g(g), rt(rt), v_anc(n), v_dsc(n), v_add(n) {
		// verify : https://atcoder.jp/contests/abc329/tasks/abc329_g
	}
	Imos_tree_edge() : n(0), rt(-1) {}

	// アクセス
	inline T const& operator[](int i) const { return v_anc[i]; }
	inline T& operator[](int i) { return v_anc[i]; }

	// 根から頂点 s へのパス上の辺全てに val を加算する準備を行う．
	void add_path(int s, T val) {
		// verify : https://atcoder.jp/contests/abc329/tasks/abc329_g

		Assert(0 <= s && s < n);

		v_anc[s] += val;
	}

	// 部分木 s 内の辺全てに val を加算する準備を行う．
	void add_subtree(int s, T val) {
		Assert(0 <= s && s < n);

		v_dsc[s] += val;
		v_add[s] -= val;
	}

	// 実際の加算を行う．
	void execute() {
		// verify : https://atcoder.jp/contests/abc329/tasks/abc329_g

		execute_sub(rt, -1, 0);

		// 計算結果をまとめておく．
		rep(s, n) v_anc[s] += v_dsc[s] + v_add[s];

		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos_tree_edge a) {
		if (!a.ex) a.execute();
		rep(i, a.n) os << a[i] << " ";
		return os;
	}
#endif
};


//【木上いもす法（パス，部分木）】
/*
* Imos_tree_arbitrary_path<T>(Graph g, int rt) : O(n log n)
*	rt を根とする根付き木 g と頂点の値 v[0..n) = 0 で初期化する．
*
* add(int s, T val) : O(1)
*	頂点 s に val を加算する準備を行う．
*
* add_path(int s, T val) : O(1)
*	根から頂点 s へのパス上の頂点全てに val を加算する準備を行う．
*
* add_path(int s, int t, T val) : O(log n)
*	パス s-t 上の頂点全てに val を加算する準備を行う．
*
* add_subtree(int s, T val) : O(1)
*	部分木 s 内の頂点全てに val を加算する準備を行う．
*
* execute() : O(n)
*	実際の加算を行う．
*
* T [](int s) : O(1)
*	v[s] を返す．
*	制約 : 先に execute() を呼び出すこと．
*
* int lca(int s, int t) : O(log n)
*	頂点 s, t の最小共通祖先を返す．
*/
pii op_IT(pii a, pii b) { return min(a, b); }
pii e_IT() { return { INF, -1 }; }
template <class T>
class Imos_tree_arbitrary_path {
	int n;
	Graph g;
	int rt;
	bool ex = false;

public:
	// in[s]  : rt からの DFS で最初に頂点 s を訪れた時刻（根なら 0）
	// out[s] : rt からの DFS で最後に頂点 s から離れた時刻（根なら 2n-1）
	// pos[t] : rt からの DFS で時刻 t に居た頂点の番号（長さ 2n-1）
	// dep[s] : 頂点 s の深さ
	vi in, out, pos, dep;

private:
	// seg[t] : 時刻 t に居た頂点の (深さ, 番号)
	using SEG = segtree<pii, op_IT, e_IT>;
	SEG seg;

	vector<T> v_anc, v_dsc, v_add;

	// in, out, pos, dep を設定する．
	void dfs() {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int p) {
			// s を最初に訪れた
			in[s] = time;
			pos[time] = s;
			time++;

			repe(t, g[s]) {
				if (t == p) continue;

				dep[t] = dep[s] + 1;
				rf(t, s);
				pos[time] = s;
				time++;
			}

			// s から最後に離れる
			out[s] = time;
		};

		// 根から順に探索する．
		rf(rt, -1);
	}

	T execute_sub(int s, int p, T val) {
		v_dsc[s] += val;
		repe(t, g[s]) if (t != p) v_anc[s] += execute_sub(t, s, v_dsc[s]);
		return v_anc[s];
	};

public:
	// rt を根とする根付き木 g と頂点の値 v[0..n) = 0 で初期化する．
	Imos_tree_arbitrary_path(const Graph& g, int rt) : n(sz(g)), g(g), rt(rt),
		in(n), out(n), pos(2 * n - 1), dep(n), v_anc(n), v_dsc(n), v_add(n)
	{
		// verify : https://codeforces.com/contest/914/problem/E

		dfs();

		vector<pii> ini(2 * n - 1);
		rep(t, 2 * n - 1) ini[t] = { dep[pos[t]], pos[t] };
		seg = SEG(ini);
	}
	Imos_tree_arbitrary_path() : n(0), rt(-1) {}

	// アクセス
	inline T const& operator[](int i) const { return v_anc[i]; }
	inline T& operator[](int i) { return v_anc[i]; }

	// 根から頂点 s へのパス上の頂点全てに val を加算する準備を行う．
	void add_path(int s, T val) {
		Assert(0 <= s && s < n);

		v_anc[s] += val;
	}

	// 頂点 s, t の最小共通祖先を返す．
	int lca(int s, int t) const {
		// 初めて s または t に訪れたとき
		int l = min(in[s], in[t]);

		// 最後に s または t から離れたとき
		int r = max(out[s], out[t]);

		// その途中で訪れたことのある最も浅い頂点が最小共通祖先
		return seg.prod(l, r).second;
	}

	// 頂点 s に val を加算する準備を行う．
	void add(int s, T val) {
		// verify : https://codeforces.com/contest/914/problem/E

		Assert(0 <= s && s < n);

		v_add[s] += val;
	}

	// パス s-t 上の頂点全てに val を加算する準備を行う．
	void add_path(int s, int t, T val) {
		// verify : https://codeforces.com/contest/914/problem/E

		Assert(0 <= s && s < n && 0 <= t && t < n);

		int p = lca(s, t);

		v_anc[s] += val;
		v_anc[t] += val;
		v_anc[p] -= 2 * val;
		v_add[p] += val;
	}

	// 部分木 s 内の頂点全てに val を加算する準備を行う．
	void add_subtree(int s, T val) {
		Assert(0 <= s && s < n);

		v_dsc[s] += val;
	}

	// 実際の加算を行う．
	void execute() {
		// verify : https://codeforces.com/contest/914/problem/E

		execute_sub(rt, -1, 0);

		// 計算結果をまとめておく．
		rep(s, n) v_anc[s] += v_dsc[s] + v_add[s];

		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos_tree_arbitrary_path a) {
		if (!a.ex) a.execute();
		rep(i, a.n) os << a[i] << " ";
		return os;
	}
#endif
};


//【木の辺上いもす法（パス，部分木）】
/*
* Imos_tree_edge_arbitrary_path<T>(Graph g, int rt) : O(n log n)
*	rt を根とする根付き木 g と辺値 v[0..n) = 0 で初期化する．
*	辺値 v[s] は頂点 s に入る辺の値を表す（v[rt] は無視する）
*
* add_path(int s, T val) : O(1)
*	根から頂点 s へのパス上の辺全てに val を加算する準備を行う．
*
* add_path(int s, int t, T val) : O(log n)
*	パス s-t 上の辺全てに val を加算する準備を行う．
*
* add_subtree(int s, T val) : O(1)
*	部分木 s 内の辺全てに val を加算する準備を行う．
*
* execute() : O(n)
*	実際の加算を行う．
*
* T [](int s) : O(1)
*	v[s] を返す．
*	制約 : 先に execute() を呼び出すこと．
*
* int lca(int s, int t) : O(log n)
*	頂点 s, t の最小共通祖先を返す．
*/
pii op_ITE(pii a, pii b) { return min(a, b); }
pii e_ITE() { return { INF, -1 }; }
template <class T>
class Imos_tree_edge_arbitrary_path {
	int n;
	Graph g;
	int rt;
	bool ex = false;

public:
	// in[s]  : rt からの DFS で最初に頂点 s を訪れた時刻（根なら 0）
	// out[s] : rt からの DFS で最後に頂点 s から離れた時刻（根なら 2n-1）
	// pos[t] : rt からの DFS で時刻 t に居た頂点の番号（長さ 2n-1）
	// dep[s] : 頂点 s の深さ
	vi in, out, pos, dep;

private:
	// seg[t] : 時刻 t に居た頂点の (深さ, 番号)
	using SEG = segtree<pii, op_ITE, e_ITE>;
	SEG seg;

	vector<T> v_anc, v_dsc, v_add;

	// in, out, pos, dep を設定する．
	void dfs() {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int p) {
			// s を最初に訪れた
			in[s] = time;
			pos[time] = s;
			time++;

			repe(t, g[s]) {
				if (t == p) continue;

				dep[t] = dep[s] + 1;
				rf(t, s);
				pos[time] = s;
				time++;
			}

			// s から最後に離れる
			out[s] = time;
		};

		// 根から順に探索する．
		rf(rt, -1);
	}

	T execute_sub(int s, int p, T val) {
		v_dsc[s] += val;
		repe(t, g[s]) if (t != p) v_anc[s] += execute_sub(t, s, v_dsc[s]);
		return v_anc[s];
	};

public:
	// rt を根とする根付き木 g と辺値 v[0..n) = 0 で初期化する．
	Imos_tree_edge_arbitrary_path(const Graph& g, int rt) : n(sz(g)), g(g), rt(rt),
		in(n), out(n), pos(2 * n - 1), dep(n), v_anc(n), v_dsc(n), v_add(n)
	{
		// verify : https://atcoder.jp/contests/abc345/tasks/abc345_f

		dfs();

		vector<pii> ini(2 * n - 1);
		rep(t, 2 * n - 1) ini[t] = { dep[pos[t]], pos[t] };
		seg = SEG(ini);
	}
	Imos_tree_edge_arbitrary_path() : n(0), rt(-1) {}

	// アクセス
	inline T const& operator[](int i) const { return v_anc[i]; }
	inline T& operator[](int i) { return v_anc[i]; }

	// 根から頂点 s へのパス上の辺全てに val を加算する準備を行う．
	void add_path(int s, T val) {
		Assert(0 <= s && s < n);

		v_anc[s] += val;
	}

	// 頂点 s, t の最小共通祖先を返す．
	int lca(int s, int t) const {
		// 初めて s または t に訪れたとき
		int l = min(in[s], in[t]);

		// 最後に s または t から離れたとき
		int r = max(out[s], out[t]);

		// その途中で訪れたことのある最も浅い頂点が最小共通祖先
		return seg.prod(l, r).second;
	}

	// パス s-t 上の辺全てに val を加算する準備を行う．
	void add_path(int s, int t, T val) {
		// verify : https://atcoder.jp/contests/abc345/tasks/abc345_f

		Assert(0 <= s && s < n && 0 <= t && t < n);

		int p = lca(s, t);

		v_anc[s] += val;
		v_anc[t] += val;
		v_anc[p] -= 2 * val;
	}

	// 部分木 s 内の辺全てに val を加算する準備を行う．
	void add_subtree(int s, T val) {
		Assert(0 <= s && s < n);

		v_dsc[s] += val;
		v_add[s] -= val;
	}

	// 実際の加算を行う．
	void execute() {
		// verify : https://atcoder.jp/contests/abc345/tasks/abc345_f

		execute_sub(rt, -1, 0);

		// 計算結果をまとめておく．
		rep(s, n) v_anc[s] += v_dsc[s] + v_add[s];

		ex = true;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos_tree_edge_arbitrary_path a) {
		if (!a.ex) a.execute();
		rep(i, a.n) os << a[i] << " ";
		return os;
	}
#endif
};


//【木上いもす法（パス，部分木，HLD）】
/*
* Imos_tree_arbitrary_path<T>(Tree rt) : O(n)
*	根付き木 rt と初期値 0 で初期化する．
*
* add(int v, T val) : O(1)
*	頂点 v に val を加算する準備を行う．
*
* add_subtree(int v, T val) : O(1)
*	頂点 v の部分木の頂点に val を加算する準備を行う．
*
* add(int v1, int v2, T val) : O(log n)
*	頂点 v1 から v2 までの頂点（両端含む）に val を加算する準備を行う．
*
* sum() : O(n)
*	実際の加算を行う．
*
* T get(int v) : O(1)
*	加算後の頂点 v の値を得る．
*	制約 : sum() の後に呼び出さなければならない．
*
* 利用：【根付き木の HL 分解】
*/
template <class T>
struct Imos_tree_arbitrary_path {
	// 根付き木
	Rooted_tree rt;
	int n;

	// HL 分解の結果の記録用
	// in[s] : 最重頂点優先で頂点 s を何番目になぞるか（根なら 0）
	// out[s] : 最重頂点優先で頂点 s から出て次になぞる頂点が何番目か（根なら n）
	// pos[i] : 最重頂点優先で i 番目になぞる頂点（長さ n）
	// top[s] : 頂点 s を含む連結成分の最も浅い頂点
	vi in, out, pos, top;

	// imos[i] : in[v] = i であるような頂点の値
	vector<T> imos;

	// コンストラクタ（根付き木で初期化）
	Imos_tree_arbitrary_path(Rooted_tree& rt) : rt(rt), n(rt.n), imos(n + 1) {
		// verify : https://codeforces.com/contest/914/problem/E

		// rt を HL 分解する．
		heavy_light_decomposition(rt, in, out, pos, top);
	}

	// 頂点 v に val を加算する準備を行う．
	void add(int v, ll val) {
		// verify : https://codeforces.com/contest/914/problem/E

		imos[in[v]] += val;
		imos[in[v] + 1] -= val;
	}

	// 頂点 v の部分木の頂点に val を加算する準備を行う．
	void add_subtree(int v, ll val) {
		imos[in[v]] += val;
		imos[out[v]] -= val;
	}

	// 頂点 v1 から v2 までの頂点（両端含む）に val を加算する準備を行う．
	void add(int v1, int v2, ll val) {
		// verify : https://codeforces.com/contest/914/problem/E

		// v1 と v2 が異なる連結成分に属している限りループを回す．
		while (top[v1] != top[v2]) {
			// v1 の方が浅い連結成分に属しているとする．
			if (in[top[v1]] > in[top[v2]]) swap(v1, v2);

			// v2 を含む連結成分は pos で並んで配置されているので，
			// 最も浅い頂点 top[v2] から v2 までの範囲に val を加算する．
			imos[in[top[v2]]] += val;
			imos[in[v2] + 1] -= val;

			// 一つ浅い連結成分に移動する．
			v2 = rt[top[v2]].parent;
		}

		// ここまできたら v1 と v2 は同じ連結成分に属するので，
		// その間の頂点のみに対して val を加算する．
		if (in[v1] > in[v2]) swap(v1, v2);
		imos[in[v1]] += val;
		imos[in[v2] + 1] -= val;
	}

	// 実際の加算を行う．
	void sum() {
		// verify : https://codeforces.com/contest/914/problem/E

		rep(i, n) imos[i + 1] += imos[i];
	}

	// 頂点 v の値を返す．
	ll get(int v) {
		// verify : https://codeforces.com/contest/914/problem/E

		return imos[in[v]];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Imos_tree_arbitrary_path& q) {
		os << q.imos;
		return os;
	}
#endif
};


