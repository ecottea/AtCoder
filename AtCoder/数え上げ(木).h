#pragma once
#include "header.h"
#include "分析(木).h"
#include "二項係数.h"
#include "木DP.h"
#include "数論変換.h"
// ■■■■■ 木（数え上げ） ■■■■■


//【根付き木の独立集合の数え上げ】O(n)
/*
* 木 g の独立集合（辺を共有しない頂点の集合）の個数を返す．
*
* 利用：【貰う木 DP】
*/
struct T_cind { mint c1, c0; }; // (根を選択，根を非選択)
T_cind leaf_cind(int s) {
	return T_cind{ 1, 1 };
}
T_cind add_edge_cind(const T_cind& x, int p, int s) {
	return T_cind{ x.c0, x.c0 + x.c1 };
}
void merge_cind(T_cind& x, const T_cind& y, int s) {
	x.c0 *= y.c0;
	x.c1 *= y.c1;
}
void add_vertex_cind(T_cind& x, int s) {
	;
}
mint count_independent_set(const Graph& g) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	auto dp = tree_getDP<T_cind, leaf_cind, add_edge_cind, merge_cind, add_vertex_cind>(g, 0);
	return dp[0].c0 + dp[0].c1;
}


//【根付き木の部分木の数え上げ（大きさ毎）】O(n^2)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..|s|]（|s| は部分木 s の大きさ）について，
* 部分木 s の大きさ i の部分木の個数を格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP】
*/
using T_cs = vm;
T_cs leaf_cs(int s) {
	// 空の部分木も認める．
	return T_cs{ 1, 1 };
}
T_cs add_edge_cs(const T_cs& x, int p, int s) {
	return x;
}
void merge_cs(T_cs& x, const T_cs& y, int s) {
	// ns[nt] : 部分木 x[ y ] の大きさ + 1
	int ns = sz(x), nt = sz(y);

	// これは畳込みなので mod 998244353 なら O(n log n) まで高速化できそうな気になるが，
	// 毛虫グラフに近いとき長さ O(1) と O(n) の畳込みを O(n) 回やるのでだめ．
	T_cs nx(ns + nt - 1);
	rep(i, ns) rep(j, nt) nx[i + j] += x[i] * y[j];
	x = move(nx);
}
void add_vertex_cs(T_cs& x, int s) {
	// ns : 部分木 x の大きさ + 1
	int ns = sz(x);

	x.resize(ns + 1);
	repir(i, ns, 1) x[i] = x[i - 1];
	x[0] = 1; // 空の部分木も認める．
}
vector<T_cs> count_subtree(const Graph& g, int r) {
	// 参考 : https://snuke.hatenablog.com/entry/2019/01/15/211812

	return tree_getDP<T_cs, leaf_cs, add_edge_cs, merge_cs, add_vertex_cs>(g, r);
}


//【根付き木の部分木の数え上げ（大きさ毎，大きさ K 以下）】O(n K)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..min(|s|,K)]（|s| は部分木 s の大きさ）について，
* 部分木 s の大きさ i の部分木の個数を格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP】
*/
int K_csk; // 部分木の大きさの最大値
using T_csk = vm;
T_csk leaf_csk(int s) {
	return T_csk{ 1, 1 };
}
T_csk add_edge_csk(const T_csk& x, int p, int s) {
	return x;
}
void merge_csk(T_csk& x, const T_csk& y, int s) {
	int ns = sz(x), nt = sz(y);
	int n = min(ns + nt - 1, K_csk + 1);

	T_csk nx(n);
	rep(i, ns) repi(j, 0, min(nt - 1, K_csk - i)) nx[i + j] += x[i] * y[j];
	x = move(nx);
}
void add_vertex_csk(T_csk& x, int s) {
	int ns = sz(x);
	int n = min(ns + 1, K_csk + 1);

	x.resize(n);
	repir(i, n - 1, 1) x[i] = x[i - 1];
	x[0] = 1;
}
vector<T_csk> count_subtree(const Graph& g, int r, int K) {
	// 参考 : https://snuke.hatenablog.com/entry/2019/01/15/211812

	K_csk = K;
	return tree_getDP<T_csk, leaf_csk, add_edge_csk, merge_csk, add_vertex_csk>(g, r);
}


//【根付き木の部分木の数え上げ（大きさ毎，mod 998244353）】O(n (log n)^3)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 i∈[0..n] について大きさ i の部分木の個数を格納したリストを返す．
*
* 利用：【貰う木 DP（多項式，mod 998244353）】
*/
MFPS leaf_csM(int s) { return MFPS(vm{ 1,1 }); }
pair<MFPS, MFPS> apply_csM(int s) { return { MFPS(vm{ 0,1 }), MFPS(1) }; }
vm count_subtree_MFPS(const Graph& g, int r) {
	auto res = tree_getDP_MFPS<leaf_csM, apply_csM>(g, r);
	res.resize(sz(g) + 1);
	return res.c;
}


//【根付き木の部分木の数え上げ（葉の数毎）】O(n^2)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..l(s)]（l(s) は部分木 s の葉の数）について，
* 部分木 s の葉を i 個もつ部分木の個数を格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP】
*/
using T_csl = vm;
T_csl leaf_csl(int s) {
	return T_csl{ 1, 1 };
}
T_csl add_edge_csl(const T_csl& x, int p, int s) {
	return x;
}
void merge_csl(T_csl& x, const T_csl& y, int s) {
	int nx = sz(x), ny = sz(y);

	T_csl z(nx + ny - 1);
	rep(i, nx) rep(j, ny) z[i + j] += x[i] * y[j];
	x = move(z);
}
void add_vertex_csl(T_csl& x, int s) {
	x[1]++; // s が葉である部分木
}
vector<T_csl> count_subtree_by_leaf(const Graph& g, int r) {
	return tree_getDP<T_csl, leaf_csl, add_edge_csl, merge_csl, add_vertex_csl>(g, r);
}


//【根付き木の部分木の数え上げ（葉の数毎，mod 998244353）】O(n (log n)^3)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 i∈[0..n] について葉を i 個もつ部分木の個数を格納したリストを返す．
*
* 利用：【貰う木 DP（多項式，mod 998244353）】
*/
MFPS leaf_cslM(int s) { return MFPS(vm{ 1,1 }); }
pair<MFPS, MFPS> apply_cslM(int s) { return { MFPS(1), MFPS(vm{ 0,1 }) }; }
vm count_subtree_by_leaf_MFPS(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/abc269/tasks/abc269_h

	auto res = tree_getDP_MFPS<leaf_cslM, apply_cslM>(g, r);
	res.resize(sz(g) + 1); // 葉の数以降の次数の係数は 0 で埋まっている．
	return res.c;
}


//【根付き木の連結誘導部分グラフの数え上げ（葉の色指定）】O(n)
/*
* 与えられた r を根とする根付き木 g とその頂点の色 c[0..n)∈{0,1}^n に対し，各 s∈[0..n) について
* 部分木 s の誘導部分グラフで，頂点数 2 以上で連結かつ葉の色が全て 1 であるものの個数を格納したリストを返す．
*
* 利用：【貰う木 DP】
*/
vi c_cislc; // 頂点の色 ∈ {0,1}
struct T_cislc {
	// 辺数 1 以上の連結誘導部分グラフの個数を管理する．
	mint c0; // 根の次数が 0
	mint c1; // 根の次数が 1，ただし根だけは色 0 で葉となることを許す
	mint c2; // 根の次数が 2 以上
#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const T_cislc& x) {
		os << '(' << x.c0 << ',' << x.c1 << ',' << x.c2 << ',' << ')';
		return os;
	}
#endif
};
T_cislc leaf_cislc(int s) {
	return { 0, 0, 0 };
}
T_cislc add_edge_cislc(const T_cislc& x, int p, int s) {
	T_cislc z;

	z.c0 = x.c0 + x.c2 + (c_cislc[s] ? x.c1 : 0); // deg(s)=1 かつ c[s]=0 なら放置できない
	z.c1 = x.c1 + x.c2 + (c_cislc[s] ? 1 : 0); // c[s]=1 なら辺 s-p を選択できる
	z.c2 = 0;

	return z;
}
void merge_cislc(T_cislc& x, const T_cislc& y, int s) {
	T_cislc z;

	z.c0 = x.c0 + y.c0;
	z.c1 = x.c1 + y.c1;
	z.c2 = x.c2 + y.c2 + (x.c1 + x.c2) * (y.c1 + y.c2); // 結合するパターンがある

	x = move(z);
}
void add_vertex_cislc(T_cislc& x, int s) {
	;
}
vm count_connected_induced_subtree_by_leaf_color(const Graph& g, int r, vi col) {
	// verify : https://atcoder.jp/contests/abc340/tasks/abc340_g

	int n = sz(g);
	c_cislc = move(col);
	auto dp = tree_getDP<T_cislc, leaf_cislc, add_edge_cislc, merge_cislc, add_vertex_cislc>(g, r);
	dump(dp);

	vm res(n);
	rep(s, n) res[s] = dp[s].c0 + dp[s].c2 + (c_cislc[s] ? dp[s].c1 : 0);
	return res;
}


//【根付き木の誘導部分グラフの数え上げ（辺の数毎）】O(n^2)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..|s|)（|s| は部分木 s の大きさ）について，
* 部分木 s の i 本の辺からなる誘導部分グラフの個数を格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP】
*/
struct T_cis { vm c0, c1; }; // (根を含まない, 根を含む)
T_cis leaf_cis(int s) {
	return T_cis{ {1}, {1} };
}
T_cis add_edge_cis(const T_cis& x, int p, int s) {
	int n = sz(x.c0);

	T_cis z;
	z.c0.resize(n + 1);
	z.c1.resize(n + 1);

	rep(i, n) {
		z.c0[i] += x.c0[i];
		z.c0[i] += x.c1[i];
		z.c1[i] += x.c0[i];
		z.c1[i + 1] += x.c1[i]; // 隣接する頂点をともに選ぶなら間の辺も選ばれる
	}
	return z;
}
void merge_cis(T_cis& x, const T_cis& y, int s) {
	int nx = sz(x.c0), ny = sz(y.c0);

	T_cis z;
	z.c0.resize(nx + ny);
	z.c1.resize(nx + ny);

	rep(i, nx) rep(j, ny) {
		z.c0[i + j] += x.c0[i] * y.c0[j];
		z.c1[i + j] += x.c1[i] * y.c1[j];
	}

	x = move(z);
}
void add_vertex_cis(T_cis& x, int s) {
	;
}
vvm count_induced_subtree(const Graph& g, int r) {
	int n = sz(g);

	auto dp = tree_getDP<T_cis, leaf_cis, add_edge_cis, merge_cis, add_vertex_cis>(g, r);

	vvm res(n);
	rep(i, n) {
		int L = sz(dp[i].c0);
		res[i].resize(L);
		rep(j, L) res[i][j] = dp[i].c0[j] + dp[i].c1[j];
	}

	return res;
}


//【根付き木の点素なパスの数え上げ（辺の数毎）】O(n^2)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..|s|)（|s| は部分木 s の大きさ）について，
* 部分木 s の i 本の点素な長さ 1 以上のパスの組の個数を格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP】
*/
struct T_ccp {
	vm n, e, o; // (根がパスに属さない, 根がパスの端点, 根が端点以外のパス上)
};
T_ccp leaf_ccp(int s) {
	return T_ccp{ {1}, {0}, {0} };
}
T_ccp add_edge_ccp(const T_ccp& x, int p, int s) {
	int n = sz(x.n);

	T_ccp z;
	z.n.resize(n + 1);
	z.e.resize(n + 1);
	z.o.resize(n + 1);

	rep(i, n) {
		z.n[i] += x.n[i] + x.e[i] + x.o[i];
		z.e[i] += x.e[i];
		z.e[i + 1] += x.n[i]; // 辺 p-s を新たにパスとする．
	}

	return z;
}
void merge_ccp(T_ccp& x, const T_ccp& y, int s) {
	int nx = sz(x.n), ny = sz(y.n);

	T_ccp z;
	z.n.resize(nx + ny);
	z.e.resize(nx + ny);
	z.o.resize(nx + ny);

	rep(i, nx) rep(j, ny) {
		z.n[i + j] += x.n[i] * y.n[j];
		z.e[i + j] += x.n[i] * y.e[j];
		z.o[i + j] += x.n[i] * y.o[j];
		z.e[i + j] += x.e[i] * y.n[j];
		if (i + j - 1 >= 0) z.o[i + j - 1] += x.e[i] * y.e[j];
		z.o[i + j] += x.o[i] * y.n[j];
	}
	x = move(z);
}
void add_vertex_ccp(T_ccp& x, int s) {
	;
}
vvm count_coprime_path(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_eel

	int n = sz(g);

	auto dp = tree_getDP<T_ccp, leaf_ccp, add_edge_ccp, merge_ccp, add_vertex_ccp>(g, r);

	vvm res(n);
	rep(i, n) {
		int L = sz(dp[i].n);
		res[i].resize(L);
		rep(j, L) res[i][j] = dp[i].n[j] + dp[i].e[j] + dp[i].o[j];
	}

	return res;
}


//【部分木の数え上げ】O(n)
/*
* 与えられた木 g に対し，各 s∈[0..n) について，
* 頂点 s を含む g の部分木の個数を格納したリストを返す．
*
* 利用：【全方位木 DP】
*/
using T_st = mint;
T_st leaf_st(int s) {
	return 1;
}
T_st add_edge_st(const T_st& x, int p, int s) {
	return x + 1; // 部分木 s の頂点を 1 つも選択しないことも可能になる．
}
T_st merge_st(const T_st& x, const T_st& y, int s) {
	return x * y;
}
T_st add_vertex_st(const T_st& x, int s) {
	return x; // 1×1×...×1 が根のみの選択に対応するので，ここでの +1 は不要
}
vm count_subtree(const Graph& g) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_v

	return rerooting<T_st, leaf_st, add_edge_st, merge_st, add_vertex_st>(g);
}


//【ヒープの数え上げ】O(n)
/*
* 与えられた木 g の頂点に対する数 [0..n) の割り当て方のうち，
* 各 s∈[0..n) を根とみなしたときヒープをなすものの数を格納したリストを返す．
*
* 制約：fm は n! まで計算可能
*
* 利用：【全方位木 DP】
*/
struct T_ch {
	mint v; // ヒープの数
	int c; // 辺の数
};
Factorial_mint const* fm_ch;
T_ch leaf_ch(int s) {
	return { 1, 0 };
}
T_ch add_edge_ch(const T_ch& x, int p, int s) {
	return { x.v, x.c + 1 };
}
T_ch merge_ch(const T_ch& x, const T_ch& y, int s) {
	// 左右それぞれで大小順が変わらない限り，独立に数の再割り当てができる．
	return { x.v * y.v * fm_ch->bin(x.c + y.c, x.c), x.c + y.c };
}
T_ch add_vertex_ch(const T_ch& x, int s) {
	// 根に割り当てられる数は 0 しかなく，他については全体に 1 加算される．
	return x;
}
vm count_heap(const Graph& g, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc160/tasks/abc160_f

	int n = sz(g);
	fm_ch = &fm;

	auto dp = rerooting<T_ch, leaf_ch, add_edge_ch, merge_ch, add_vertex_ch>(g);

	vm res(n);
	rep(i, n) res[i] = dp[i].v;

	return res;
}


//【パスの数え上げ（長さ指定）】O(n log n)
/*
* 木 g の異なる 2 頂点の組で，距離が k であるものの個数を返す．
*
* 利用：【木の重心分解】
*/
ll count_tree_distance(const Graph& g, int k) {
	// verify : https://csacademy.com/contest/round-58/task/path-inversions/

	int n = sz(g);
	if (k <= 0 || k >= n) return 0;

	// 木 g を重心分解する．
	Centroid_decomposition<Graph> cd(g);
	
	ll res = 0;

	// cnt_all[l] : 長さ l のパスの本数（使い回す）
	vl cnt_all(n);

	// cent : 部分木の重心
	rep(cent, n) {
		// nc : cent に隣接する頂点の個数，c_dep : cent の深さ
		int nc = sz(g[cent]), c_dep = cd[cent].dep;

		// 部分木ごとの長さのリスト
		vvi lens(nc);

		// cent で分割された各部分木の cent に隣接する頂点を根として dfs する．
		rep(i, nc) {
			// cent と同じかそれより浅い頂点は無視する．
			if (cd[g[cent][i]].dep <= c_dep) continue;

			function<void(int, int, int)> dfs = [&](int s, int p, int l) {
				// 長さ l のパスの存在を記録する．
				lens[i].emplace_back(l);
				cnt_all[l]++;

				repe(t, g[s]) {
					// cent と同じかそれより浅い頂点は無視する．
					if (cd[t].dep <= c_dep || t == p) continue;

					dfs(t, s, l + 1);
				}
			};
			dfs(g[cent][i], -1, 1);
		}
		
		// 2 回目の dfs
		rep(i, nc) {
			// i 番目の部分木の分をロールバック
			repe(l, lens[i]) cnt_all[l]--;

			// cent と同じかそれより浅い頂点は無視する．
			if (cd[g[cent][i]].dep <= c_dep) continue;

			function<void(int, int, int)> dfs = [&](int s, int p, int l) {
				// cent を端点にもつ長さ k のパスを数える（後で 2 で割るので 2 本として数える）
				if (l == k) res += 2;

				// 他の部分木内の長さ 1 以上のパスと繋げて長さ k になるパスを数える．
				if (k - l >= 1) res += cnt_all[k - l];

				repe(t, g[s]) {
					// cent と同じかそれより浅い頂点は無視する．
					if (cd[t].dep <= c_dep || t == p) continue;

					dfs(t, s, l + 1);
				}
			};
			dfs(g[cent][i], -1, 1);

			// i 番目の部分木の分のロールバックを元に戻す
			repe(l, lens[i]) cnt_all[l]++;
		}

		// cnt_all の初期化（assign() より速い）
		rep(i, nc) repe(l, lens[i]) cnt_all[l]--;
	}

	// 同じパスを 2 通りの向きで数えてしまっているので 2 で割る．
	return res / 2;
}


//【パスの数え上げ（長さごと）】O(n (log n)^2)
/*
* 各 d∈[0..n) について，木 g の異なる 2 点の組で距離が d であるものの個数のリストを返す．
*
* 利用：【木の 1/3 重心分解】
*/
vl tree_distance_frequency(const Graph& g) {
	int n = sz(g);

	vl res(n);

	auto f = [&](const Graph& gl, const vi& idl, const Graph& gr, const vi& idr) {
		// 左側の部分木（根を除く）についての根からの距離の分布を求める．
		int nl = sz(gl);
		vl cntl(nl);

		function<void(int, int, int)> dfsl = [&](int s, int p, int d) {
			cntl[d]++;
			repe(t, gl[s]) {
				if (t == p) continue;
				dfsl(t, s, d + 1);
			}
		};
		dfsl(0, -1, 0);
		cntl[0]--;

		// 右側の部分木（根を除く）についての根からの距離の分布を求める．
		int nr = sz(gr);
		vl cntr(nr);

		function<void(int, int, int)> dfsr = [&](int s, int p, int d) {
			cntr[d]++;
			repe(t, gr[s]) {
				if (t == p) continue;
				dfsr(t, s, d + 1);
			}
		};
		dfsr(0, -1, 0);
		cntr[0]--;

		// 左右の部分木を跨ぐパスの長さの分布を求める．
		auto cnt = convolution_ll(cntl, cntr);
		rep(i, sz(cnt)) res[i] += cnt[i];
	};
	one_third_centroid_decomposition(g, f);

	// 大きさ 2 の部分木（g の辺）に対する例外処理
	if (n >= 2) res[1] += n - 1;

	return res;
}


//【森の連結成分の数】
/*
* 森 g の連結成分の個数は，(g の頂点数) - (g の辺数) に等しい．
* 
* verify : https://atcoder.jp/contests/agc015/tasks/agc015_c
*/


//【誘導部分グラフの連結成分の数】
/*
* 根付き木 g の誘導部分グラフ h の連結成分の個数は，
* 親が h に属さない（または g の根である）ような h の頂点の個数に等しい．
* 
* verify : https://atcoder.jp/contests/abc173/tasks/abc173_f
*/


//【二分木の数え上げ】
/*
* n 頂点の二分木の個数は Catalan(n) = bin(2n,n)/(n+1) である．
* 
* 注意：wikipedia には，2n+1 頂点の "全" 二分木の個数が Catalan(n) であることが載っている．
* 
* 証明：n 頂点の二分木（根が空も許す）の個数を a[n] とおき，その母関数
*		f(z) = Σn∈[0..∞) a[n] z^n
* を導入する．
* 
* 根が 2 つ（空も許す）の n 頂点の二分森の個数 b[n] は，一方の頂点数で場合分けすれば
*		b[n] = Σi∈[0..n] a[i] a[n-i]
* と畳込みで表すことができ，対応する母関数は
*		f(z)^2
* である．根が空でない n 頂点の二分木の個数 c[n] は，n-1 頂点の二分森に根を追加すれば得られるので，
*		c[n] = b[n-1]  (n ≧ 1)
*		c[0] = 0
* と表すことができ，対応する母関数は
*		z f(z)^2
* である．これと根が空の二分木を合わせることで n 頂点の二分木全てが得られるので，
*		a[n] = c[n]  (n ≧ 1)
*		a[0] = 1
* なる関係式が得られ，対応する母関数の関数方程式は
*		z f(z)^2 + 1 = f(z)
* となる．
* 
* 関数方程式を変形すると
*		z = (f(z) - 1) / f(z)^2
* となる．ラグランジュの反転公式を利用するため，
*		F(z) = f(z) - 1
* とおくと，[z^0] F[z] = 0, [z^1] F[z] ≠ 0 を満たす．F(z) を用いて関数方程式を書き直すと
*		z = F(z) / (1 + F(z))^2
* となるので，F(z) の逆関数 G(z) は
*		G(z) = z / (1 + z)^2
* であり，[z^0] G[z] = 0, [z^1] G[z] ≠ 0 を満たす．よって，ラグランジュの反転公式より，
*		[z^n] F(z) 
*		= (1/n) [z^(n-1)]((z / G(z))^n)
*		= (1/n) [z^(n-1)]((1 + z)^2n)
*		= (1/n) bin(2n, n-1)
*		= bin(2n,n)/(n+1)
* を得る．
* 
* 参考 : https://atcoder.jp/contests/abc222/editorial/2742
*/


//【ラベル付き木の数え上げ】
/*
* n 頂点のラベル付き木の個数は n^(n-2) である．（Cayley の定理）
* 
* 証明：n^(n-2) 通りのプリューファーコードと 1:1 対応があることから明らか．
* あるいは【ラベル付き根付き木の数え上げ】の結果を，どれを根としているかの n で割り引けばよい．
*/


//【ラベル付き木の数え上げ（次数制約付き）】
/*
* n 頂点のラベル付き木で頂点 i の次数が d[i] であるものの個数は以下の多項係数で与えられる：
*	mul(n-2, d-1) = (n-2)! / (Πi (d[i]-1)!)
*
* 参考 : https://drken1215.hatenablog.com/entry/2020/10/25/132900
* verify : https://atcoder.jp/contests/NYC2015/tasks/nyc2015_5
*/


//【根付き木の数え上げ（子の順序の区別あり）】
/*
* 子の順序を区別する n+1 頂点の根付き木の個数は Catalan(n) = bin(2n,n)/(n+1) である．
* 
* 証明：根から始めて左優先で DFS を行い，辺を降りるときに '('，昇るときに ')' を
* 出力して得られる文字列は，長さ 2n の正しい括弧列を一意に生成する．
*/


//【根付き木の数え上げ（子の順序の区別なし，mod 998244353）】O(n (log n)^2)
/*
* 各 i∈[0..n] について，子の順序の区別をしない i 頂点の根付き木の個数を格納したリストを返す．
*
* 利用：【オンライン畳込み（mod 998244353）】
*/
vm count_unique_rooted_tree(int n) {
	// 参考 : https://oeis.org/A000081
	// 参考 : https://mathworld.wolfram.com/EulerTransform.html

	//【方法】
	// n 頂点の根付き木（根が空は許さない）の個数を a[n] とおく．
	// 根にぶらさがる部分木をどう選ぶかは個数制限なし部分和問題と同様に考えられるので，
	//		a[n+1] = [z^n] Πi∈[0..n] 1/(1 - z^i)^a[i]
	// なる漸化式を得る．
	//
	// a の母関数を A(z) = Σi∈[0..∞) a[i] z^i とおくと，右辺について
	//		log( Πi∈[0..n] 1/(1 - z^i)^a[i] )
	//		= Σi∈[0..n] -a[i] log(1 - z^i)
	//		= Σi∈[0..n] a[i] Σk∈[1..∞) 1/k (z^i)^k
	//		= Σk∈[1..∞) 1/k Σi∈[0..n] a[i] (z^k)^i
	//		= Σk∈[1..∞) 1/k A(z^k)
	// と計算できることから，母関数についての関数方程式
	//		A(z)/z = exp( Σk∈[1..∞) 1/k A(z^k) )
	// を得る．オンラインで指数関数を計算することにより a[i] が順次得られる．

	vm a(n + 1), c(n + 1);
	a[1] = 1;

	Online_convolution O(n + 1);

	repi(i, 1, n - 1) {
		for (int j = i; j <= n; j += i) c[j] += i * a[i];
		O.set(a[i], c[i]);
		a[i + 1] = O[i - 1] / i;
	}

	return a;
}


//【ラベル付き根付き木の数え上げ】
/*
* n 頂点のラベル付き根付き木の個数は n^(n-1) である．
*
* 証明：空の木を許さないものとし，指数型母関数を f(z) とおく．
* 根を m 個もつ根付き森の指数型母関数は，根を並べる順序を割り引いて f(z)^m / m! となる．
* 根付き森の指数型母関数は m∈[0..∞) にわたってこれを加え合わせればよいので
*		1 + f(z) + f(z)^2 / 2! + f(z)^3 / 3! + ... = exp(f(z))
* となる．共通の根を追加することは指数型母関数でも z 倍に対応するので，
*		z exp(f(z)) = f(z)
*		⇔ z = f(z) / exp(f(z))
* なる関数方程式を得る．
* 
* ラグランジュの反転公式より，
*		g(z) = z / exp(z)
* とおくと，
*		[z^n] f(z)
*		= (1/n) [z^(n-1)]((z / g(z))^n)
*		= (1/n) [z^(n-1)] exp(nz)
*		= (1/n) n^(n-1) / (n-1)!
*		= n^(n-1) / n!
* を得る．
* 
* 参考 : https://joisino.hatenablog.com/entry/2017/08/20/200000
*/


