#pragma once
#include "header.h"
#include "構造(木).h"
// ■■■■■ いもす法 ■■■■■


//【いもす法】
/*
* Imos<T>(int n) : O(n)
*	半開区間 [0..n) を 0 で初期化する．
*
* set(int l, int r, T val) : O(1)
*	半開区間 [l..r) に val を加算する準備を行う．
*
* void sum() : O(n)
*	実際の加算を行う．
*
* T [](int i) : O(1)
*	加算後の位置 i の値を得る．
*	制約 : 先に sum() を呼び出すこと．
*/
template <class T>
class Imos {
	// 参考：https://imoz.jp/algorithms/imos_method.html

	int n;
	vector<T> v;

public:
	// [0, n) 上の a を 0 で初期化する．
	Imos(int n) : n(n), v(n + 1) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_g
	}
	Imos() : n(0) {}

	// アクセス
	inline T const& operator[](int i) const { return v[i]; }
	inline T& operator[](int i) { return v[i]; }

	// 半開区間 [l..r) に val を加算する準備を行う．
	void set(int l, int r, T val) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_g

		chmax(l, 0);
		chmin(r, n);
		if (l >= r) return;

		v[l] += val;
		v[r] -= val;
	}

	// 実際の加算を行う．
	void sum() {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_g

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


//【間引きいもす法】
/*
* Thinning_imos<T>(int n, int m) : O(n + m)
*	法を m とし，a[0..n) = 0 で初期化する．
*
* set(int l, int r, int k, T val) : O(1)
*	S = {i∈[l..r) | i=k (mod m)} とし a[S] += val とする準備を行う．
*
* void sum() : O(n)
*	実際の加算を行う．
*
* T [](int i) : O(1)
*	加算後の a[i] を返す．
*	制約 : sum() の後に呼び出さなければならない．
*/
template <class T>
class Thinning_imos {
	int n, m;
	vector<T> v;

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
	void set(int l, int r, int k, T val) {
		// verify : https://yukicoder.me/problems/no/2359

		chmax(l, 0); chmin(r, n);
		if (l >= r) return;

		r += smod(k - r, m);
		l += smod(k - l, m);
		v[l] += val;
		v[r] -= val;
	}

	// 実際の加算を行う．
	void sum() {
		// verify : https://yukicoder.me/problems/no/2359

		rep(i, n) v[i + m] += v[i];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Thinning_imos& imos) {
		rep(i, imos.n) os << imos[i] << " ";
		return os;
	}
#endif
};


//【二次元いもす法（長方形）】
/*
* Imos_2D(int h, int w) : O(h w)
*	[0..h)×[0..w) を 0 で初期化する．
*
* set(int x1, int x2, int y1, int y2, T val) : O(1)
*	[x1..x2)×[y1..y2) に val を加算する準備を行う．
*
* sum() : O(h w)
*	実際に加算を行う．
*
* T [][](int i, int j) : O(1)
*	加算後の位置 (i, j) の値を得る．
*	制約 : 先に sum() を呼び出すこと．
*/
template <class T>
struct Imos_2D {
	// 参考：https://imoz.jp/algorithms/imos_method.html

	int h, w;
	vector<vector<T>> v;

	// [0, h)×[0, w) を 0 で初期化する．
	Imos_2D(int h_, int w_) : h(h_), w(w_), v(h + 1, vector<T>(w + 1)) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_i
	}
	Imos_2D() : h(0), w(0) {}

	// アクセス
	inline vector<T> const& operator[](int i) const { return v[i]; }
	inline vector<T>& operator[](int i) { return v[i]; }

	// [x1, x2)×[y1, y2) に val を加算する準備を行う．O(1)
	void set(int x1, int x2, int y1, int y2, T val) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_i

		v[x1][y1] += val;
		v[x1][y2] -= val;
		v[x2][y1] -= val;
		v[x2][y2] += val;
	}

	// 実際の加算を行う．O(h w)
	void sum() {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_i

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
* Imos_2D_tri<T>(int h, int w) : O(h w)
*	[0..h)×[0..w) を 0 で初期化する．
*
* set_rect(int x1, int x2, int y1, int y2, T val) : O(1)
*	[x1..x2]×[y1..y2] に val を加算する準備を行う．
*
* set_tri(int x, int y, int d, T val) : O(1)
*	[x..x+d]×[y..y+d] の対角線以下に val を加算する準備を行う．
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

	// [0, h)×[0, w) を 0 で初期化する．
	Imos_2D_tri(int h_, int w_) : h(h_), w(w_), v(h + 2, vT(w + 2)) {}
	Imos_2D_tri() : h(0), w(0) {}

	// アクセス
	inline vT const& operator[](int i) const { return v[i]; }
	inline vT& operator[](int i) { return v[i]; }

	// (高さ, 幅) を返す．
	pii size() const { return make_pair(h, w); }

	// 長方形 [x1, x2]×[y1, y2] に val を加算する準備を行う．O(1)
	void set_rect(int x1, int x2, int y1, int y2, T val) {
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

	// 正方形 [x, y]×[x + d, y + d] の対角線以下に val を加算する準備を行う．O(1)
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
* Imos_3D(int h, int w, int d) : O(h w d)
*	[0..h)×[0..w)×[0..d) を 0 で初期化する．
*
* set(int x1, int x2, int y1, int y2, int z1, int z2, T val) : O(1)
*	[x1..x2)×[y1..y2)×[z1..z2) に val を加算する準備を行う．
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

	// [0..h)×[0..w)×[0..d) を 0 で初期化する．
	Imos_3D(int h_, int w_, int d_) : h(h_), w(w_), d(d_),
		v(h + 1, vector<vector<T>>(w + 1, vector<T>(d + 1))) {}
	Imos_3D() : h(0), w(0), d(0) {}

	// アクセス
	inline vector<vector<T>> const& operator[](int i) const { return v[i]; }
	inline vector<vector<T>>& operator[](int i) { return v[i]; }

	// [x1..x2)×[y1..y2)×[z1..z2) に val を加算する準備を行う．O(1)
	void set(int x1, int x2, int y1, int y2, int z1, int z2, T val) {
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
*	半開区間 [0..n) を 0 で初期化する．
*
* set(int l, int r, T a, T b) : O(1)
*	i∈[l..r) に a i + b を一括加算する準備を行う．
*
* set_right(int l, int r, T w0, T w1) : O(1)
*	[l..r) に昇順に等差数列 v0, v1, ... を一括加算する準備を行う．
*
* set_left(int r, int l, T w0, T w1) : O(1)
*	(l..r] に降順に等差数列 v0, v1, ... を一括加算する準備を行う．
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
	Linear_imos(int n_) : n(n_), v(2, vector<T>(n + 1)) {
		// verify : https://atcoder.jp/contests/abc268/tasks/abc268_e
	}
	Linear_imos() : n(0) {}

	// アクセス
	inline T const& operator[](int i) const { return v[0][i]; }
	inline T& operator[](int i) { return v[0][i]; }

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
		rep(i, imos.n) os << imos[i] << " ";
		return os;
	}
#endif
};


//【木上いもす法（根からのパス，部分木）】
/*
* Imos_tree(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g を 0 で初期化する．
*
* set_path(int s, T val) : O(1)
*	根から頂点 s へのパス上の頂点全てに val を加算する準備を行う．
*
* set_subtree(int s, T val) : O(1)
*	部分木 s 内の頂点全てに val を加算する準備を行う．
*
* sum() : O(n)
*	実際の加算を行う．
*
* T get(int s) : O(1)
*	加算後の頂点 s の値を得る．
*	制約 : sum() の後に呼び出さなければならない．
*/
template <class T>
class Imos_tree {
	int n; Graph g; int rt;
	vector<T> v_anc, v_dsc;

	// 再帰用の関数
	T sum_sub(int s, int p, T val) {
		v_dsc[s] += val;
		repe(t, g[s]) if (t != p) v_anc[s] += sum_sub(t, s, v_dsc[s]);
		return v_anc[s];
	};

public:
	// 根付き木 rt を 0 で初期化する．
	Imos_tree(const Graph& g, int rt) : n(sz(g)), g(g), rt(rt), v_anc(n), v_dsc(n) {
		// verify : https://atcoder.jp/contests/abc138/tasks/abc138_d
	}
	Imos_tree() : n(0), rt(-1) {}

	// 根から頂点 s へのパス上の頂点全てに val を加算する準備を行う．
	void set_path(int s, T val) {
		Assert(0 <= s && s < n);

		v_anc[s] += val;
	}

	// 部分木 s 内の頂点全てに val を加算する準備を行う．
	void set_subtree(int s, T val) {
		// verify : https://atcoder.jp/contests/abc138/tasks/abc138_d

		Assert(0 <= s && s < n);

		v_dsc[s] += val;
	}

	// 実際の加算を行う．
	void sum() {
		// verify : https://atcoder.jp/contests/abc138/tasks/abc138_d

		sum_sub(rt, -1, 0);
	}

	// 加算後の頂点 s の値を得る．
	T get(int s) const {
		// verify : https://atcoder.jp/contests/abc138/tasks/abc138_d

		Assert(0 <= s && s < n);

		return v_anc[s] + v_dsc[s];
	}
};


//【木の辺上いもす法（根からのパス，部分木）】
/*
* Imos_tree_edge(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と辺値 v[0..n) = 0 で初期化する．
*	辺値 v[s] は頂点 s に入る辺の値を表す（v[rt] は無視）
*
* set_path(int s, T val) : O(1)
*	根から頂点 s へのパス上の辺全てに val を加算する準備を行う．
*
* set_subtree(int s, T val) : O(1)
*	部分木 s 内の辺全てに val を加算する準備を行う．
*
* sum() : O(n)
*	実際の加算を行う．
*
* T get(int s) : O(1)
*	加算後の頂点 s に入る辺の値を得る．
*	制約 : sum() の後に呼び出さなければならない．
*/
template <class T>
class Imos_tree_edge {
	int n; Graph g; int rt;
	vector<T> v_anc, v_dsc, v_add;

	// 再帰用の関数
	T sum_sub(int s, int p, T val) {
		v_dsc[s] += val;
		repe(t, g[s]) if (t != p) v_anc[s] += sum_sub(t, s, v_dsc[s]);
		return v_anc[s];
	};

public:
	// rt を根とする根付き木 g と辺値 v[0..n) = 0 で初期化する．
	Imos_tree_edge(const Graph& g, int rt) : n(sz(g)), g(g), rt(rt), v_anc(n), v_dsc(n), v_add(n) {
		// verify : https://atcoder.jp/contests/abc329/tasks/abc329_g
	}
	Imos_tree_edge() : n(0), rt(-1) {}

	// 根から頂点 s へのパス上の辺全てに val を加算する準備を行う．
	void set_path(int s, T val) {
		// verify : https://atcoder.jp/contests/abc329/tasks/abc329_g

		Assert(0 <= s && s < n);

		v_anc[s] += val;
	}

	// 部分木 s 内の辺全てに val を加算する準備を行う．
	void set_subtree(int s, T val) {
		Assert(0 <= s && s < n);

		v_dsc[s] += val;
		v_add[s] -= val;
	}

	// 実際の加算を行う．
	void sum() {
		// verify : https://atcoder.jp/contests/abc329/tasks/abc329_g

		sum_sub(rt, -1, 0);
	}

	// 加算後の頂点 s に入る辺の値を得る．
	T get(int s) const {
		// verify : https://atcoder.jp/contests/abc329/tasks/abc329_g

		Assert(0 <= s && s < n);

		return v_anc[s] + v_dsc[s] + v_add[s];
	}
};


//【木上いもす法（パス，部分木）】
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
*（いもす法）
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


//【木の辺上いもす法（パス，部分木）】（未 verify）
/*
* Imos_tree_edge(Graph g, int rt) : O(n log n)
*	rt を根とする根付き木 g と辺値 v[0..n) = 0 で初期化する．
*	辺値 v[s] は頂点 s に入る辺の値を表す（v[rt] は無視）
*
* set_path(int s, T val) : O(1)
*	根から頂点 s へのパス上の辺全てに val を加算する準備を行う．
*
* set_path(int s, int t, T val) : O(log n)
*	パス s-t 上の辺全てに val を加算する準備を行う．
*
* set_subtree(int s, T val) : O(1)
*	部分木 s 内の辺全てに val を加算する準備を行う．
*
* sum() : O(n)
*	実際の加算を行う．
*
* T get(int s) : O(1)
*	加算後の頂点 s に入る辺の値を得る．
*	制約 : sum() の後に呼び出さなければならない．
*
* int lca(int s, int t) : O(log n)
*	頂点 s, t の最小共通祖先を返す．
*
* int get_in(int s) : O(1)
*	rt からの DFS で最初に頂点 s を訪れた時刻（根なら 0）を返す．
*
* int get_out(int s) : O(1)
*	rt からの DFS で最後に頂点 s から離れた時刻（根なら 2n-1）を返す．
*
* int get_pos(int t) : O(1)
*	rt からの DFS で時刻 t（∈[0..2n-1)）に居た頂点の番号を返す．
*
* int get_dep(int s) : O(1)
*	頂点 s の深さを返す．
*/
pii op_ITE(pii a, pii b) { return min(a, b); }
pii e_ITE() { return { INF, -1 }; }
template <class T>
class Imos_tree_edge_arbitrary_path {
	int n;
	Graph g;
	int rt;

	// in[s]  : rt からの DFS で最初に頂点 s を訪れた時刻（根なら 0）
	// out[s] : rt からの DFS で最後に頂点 s から離れた時刻（根なら 2n-1）
	// pos[t] : rt からの DFS で時刻 t に居た頂点の番号（長さ 2n-1）
	// dep[s] : 頂点 s の深さ
	vi in, out, pos, dep;

	// seg[t] : 時刻 t に居た頂点の (深さ, 番号)
	using SEG = segtree<pii, op_ITE, e_ITE>;
	SEG seg;

	vector<T> v_anc, v_dsc, v_add;

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

	T sum_sub(int s, int p, T val) {
		v_dsc[s] += val;
		repe(t, g[s]) if (t != p) v_anc[s] += sum_sub(t, s, v_dsc[s]);
		return v_anc[s];
	};

public:
	// rt を根とする根付き木 g と辺値 v[0..n) = 0 で初期化する．
	Imos_tree_edge_arbitrary_path(const Graph& g, int rt) : n(sz(g)), g(g), rt(rt),
		in(n), out(n), pos(2 * n - 1), dep(n), v_anc(n), v_dsc(n), v_add(n)
	{
		dfs();

		vector<pii> ini(2 * n - 1);
		rep(t, 2 * n - 1) ini[t] = { dep[pos[t]], pos[t] };
		seg = SEG(ini);
	}
	Imos_tree_edge_arbitrary_path() : n(0), rt(-1) {}

	// 根から頂点 s へのパス上の辺全てに val を加算する準備を行う．
	void set_path(int s, T val) {
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
	void set_path(int s, int t, T val) {
		Assert(0 <= s && s < n && 0 <= t && t < n);

		int p = lca(s, t);

		v_anc[s] += val;
		v_anc[t] += val;
		v_anc[p] -= 2 * val;
	}

	// 部分木 s 内の辺全てに val を加算する準備を行う．
	void set_subtree(int s, T val) {
		Assert(0 <= s && s < n);

		v_dsc[s] += val;
		v_add[s] -= val;
	}

	// 実際の加算を行う．
	void sum() {
		sum_sub(rt, -1, 0);
	}

	// 加算後の頂点 s に入る辺の値を得る．
	T get(int s) const {
		Assert(0 <= s && s < n);

		return v_anc[s] + v_dsc[s] + v_add[s];
	}

	inline int get_in(int s) const {
		return in[s];
	}

	inline int get_out(int s) const {
		return out[s];
	}

	inline int get_pos(int t) const {
		return pos[t];
	}

	inline int get_dep(int s) const {
		return dep[s];
	}
};


