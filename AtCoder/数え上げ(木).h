#pragma once
#include "header.h"
#include "分析(木).h"
#include "二項係数.h"
#include "木DP.h"
// ■■■■■ 木（数え上げ） ■■■■■


//【根付き木の独立集合の数え上げ】O(n)
/*
* 木 g の独立集合（辺を共有しない頂点の集合）の個数を返す．
*
* 利用：【貰う木 DP（頂点マージ）】
*/
using T_cind = pair<mint, mint>; // (根を選択，根を非選択)
void merge_cind(T_cind& x, const T_cind& y, int s) { x.first *= y.first; x.second *= y.second; }
T_cind e_cind() { return { 1, 1 }; }
T_cind leaf_cind(int s) { return { 1, 1 }; }
T_cind apply_cind(const T_cind& x, int s, int t) { return { x.second, x.first + x.second }; }
mint count_independent_set(const Graph& g) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	auto dp = tree_getDP_vmerge<T_cind, merge_cind, e_cind, leaf_cind, apply_cind>(g, 0);
	return dp[0].first + dp[0].second;
}


//【根付き木の部分木の数え上げ（大きさ毎）】O(n^2)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..|s|]（|s| は部分木 s の大きさ）について，
* 部分木 s の大きさ i の部分木の個数を格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP（森経由）】
*/
using T_cs = vm;
void merge_cs(T_cs& x, const T_cs& y) {
	// ns[nt] : 部分木 x[ y ] の大きさ + 1
	int ns = sz(x), nt = sz(y);

	// これは畳込みなので mod 998244353 なら O(n log n) まで高速化できそうだが，
	// 毛虫グラフに近いとき長さ O(1) と O(n) の畳込みを O(n) 回やるのでだめ．
	T_cs nx(ns + nt - 1);
	rep(i, ns) rep(j, nt) nx[i + j] += x[i] * y[j];
	x = move(nx);
}
T_cs e_cs() { return T_cs{ 1 }; }
T_cs leaf_cs(int s) {
	// 空の部分木も認める．
	return T_cs{ 1, 1 };
}
void apply_cs(T_cs& x, int s) {
	// ns : 部分木 x の大きさ + 1
	int ns = sz(x);

	x.resize(ns + 1);
	repir(i, ns, 1) x[i] = x[i - 1];
	x[0] = 1; // 空の部分木も認める．
}
vector<T_cs> count_subtree(const Graph& g, int r) {
	// 参考 : https://snuke.hatenablog.com/entry/2019/01/15/211812

	return tree_getDP_forest<T_cs, merge_cs, e_cs, leaf_cs, apply_cs>(g, r);
}


//【根付き木の部分木の数え上げ（大きさ毎，大きさ k 以下）】O(n k)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..min(|s|,k)]（|s| は部分木 s の大きさ）について，
* 部分木 s の大きさ i の部分木の個数を格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP（森経由）】
*/
int k_csk; // 部分木の大きさの最大値
using T_csk = vm;
void merge_csk(T_csk& x, const T_csk& y) {
	int ns = sz(x), nt = sz(y);
	int n = min(ns + nt - 1, k_csk + 1);

	T_csk nx(n);
	rep(i, ns) repi(j, 0, min(nt - 1, k_csk - i)) nx[i + j] += x[i] * y[j];
	x = move(nx);
}
T_csk e_csk() { return T_csk{ 1 }; }
T_csk leaf_csk(int s) { return T_csk{ 1, 1 }; }
void apply_csk(T_csk& x, int s) {
	int ns = sz(x);
	int n = min(ns + 1, k_csk + 1);

	x.resize(n);
	repir(i, n - 1, 1) x[i] = x[i - 1];
	x[0] = 1;
}
vector<T_csk> count_subtree(const Graph& g, int r, int k) {
	// 参考 : https://snuke.hatenablog.com/entry/2019/01/15/211812
	
	//【補足】
	// 定数倍を少し犠牲にするなら，O(n^2) の二乗の木 DP において merge や apply の直後に
	//		x.resize(min(sz(x), k + 1));
	// を付けるだけでいい．
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_eel

	k_csk = k;
	return tree_getDP_forest<T_csk, merge_csk, e_csk, leaf_csk, apply_csk>(g, r);
}


//【根付き木の部分木の数え上げ（大きさ毎，mod 998244353）】O(n (log n)^2)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 i∈[0..n] について大きさ i の部分木の個数を格納したリストを返す．
*
* 利用：【貰う木 DP（森経由，多項式，mod 998244353）】
*/
MFPS leaf_csM(int s) { return MFPS(vm{ 1,1 }); }
pair<MFPS, MFPS> apply_csM(int s) { return { MFPS(vm{ 0,1 }), MFPS(1) }; }
vm count_subtree_MFPS(const Graph& g, int r) {
	auto res = tree_getDP_forest_MFPS<leaf_csM, apply_csM>(g, r);
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
* 利用：【貰う木 DP（森経由）】
*/
using T_csl = vm;
void merge_csl(T_csl& x, const T_csl& y) {
	int ns = sz(x), nt = sz(y);

	T_csl nx(ns + nt - 1);
	rep(i, ns) rep(j, nt) nx[i + j] += x[i] * y[j];
	x = move(nx);
}
T_csl e_csl() { return T_csl{ 1 }; }
T_csl leaf_csl(int s) { return T_csl{ 1, 1 }; }
void apply_csl(T_csl& x, int s) {
	x[1]++; // s が葉である部分木
}
vector<T_csl> count_subtree_leaf(const Graph& g, int r) {
	return tree_getDP_forest<T_csl, merge_csl, e_csl, leaf_csl, apply_csl>(g, r);
}


//【根付き木の部分木の数え上げ（葉の数毎，mod 998244353）】O(n (log n)^2)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 i∈[0..n] について葉を i 個もつ部分木の個数を格納したリストを返す．
*
* 利用：【貰う木 DP（森経由，多項式，mod 998244353）】
*/
MFPS leaf_cslM(int s) { return MFPS(vm{ 1,1 }); }
pair<MFPS, MFPS> apply_cslM(int s) { return { MFPS(1), MFPS(vm{ 0,1 }) }; }
vm count_subtree_leaf_MFPS(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/abc269/tasks/abc269_h

	auto res = tree_getDP_forest_MFPS<leaf_cslM, apply_cslM>(g, r);
	res.resize(sz(g) + 1);
	return res.c;
}


//【根付き木の誘導部分グラフの数え上げ（辺の数毎）】O(n^2)
/*
* 与えられた r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..|s|)（|s| は部分木 s の大きさ）について，
* 部分木 s の i 本の辺からなる誘導部分グラフの個数を格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP（一括）】
*/
using T_cis = vector<pair<mint, mint>>; // (根を含まない, 根を含む)
void merge_cis(T_cis& x, int s, const T_cis& y, int t) {
	int ns = sz(x), nt = sz(y);

	T_cis nx(ns + nt);
	rep(i, ns) rep(j, nt) {
		nx[i + j].first += x[i].first * y[j].first;
		nx[i + j].first += x[i].first * y[j].second;
		nx[i + j].second += x[i].second * y[j].first;
		nx[i + j + 1].second += x[i].second * y[j].second;
	}
	x = move(nx);
}
T_cis leaf_cis(int s) { return T_cis{ {1, 1} }; }
vvm count_induced_subtree(const Graph& g, int r) {
	int n = sz(g);
	
	auto dp = tree_getDP_once<T_cis, merge_cis, leaf_cis>(g, r);
	
	vvm res(n);
	rep(i, n) {
		res[i].resize(sz(dp[i]));
		rep(j, sz(dp[i])) res[i][j] = dp[i][j].first + dp[i][j].second;
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
* 利用：【貰う木 DP（一括）】
*/
using T_ccp = vector<tuple<mint, mint, mint>>; // (根がパスに属さない, 根がパスの端点, 根がパスの端点以外)
void merge_ccp(T_ccp& x, int s, const T_ccp& y, int t) {
	int ns = sz(x), nt = sz(y);
	int n = ns + nt;

	T_ccp nx(n);
	rep(i, ns) {
		auto [cs0, cs1, cs2] = x[i];

		rep(j, nt) {
			auto [ct0, ct1, ct2] = y[j];

			// 辺 s→t がパスに属さない場合
			mint sum = ct0 + ct1 + ct2;
			get<0>(nx[i + j]) += cs0 * sum;
			get<1>(nx[i + j]) += cs1 * sum;
			get<2>(nx[i + j]) += cs2 * sum;

			// 辺 s→t がパスに属する場合
			if (i + j + 1 < n) get<1>(nx[i + j + 1]) += cs0 * ct0;
			get<1>(nx[i + j]) += cs0 * ct1;
			get<2>(nx[i + j]) += cs1 * ct0;
			if (i + j - 1 >= 0) get<2>(nx[i + j - 1]) += cs1 * ct1;
		}
	}
	x = move(nx);
}
T_ccp leaf_ccp(int s) { return T_ccp{ {1, 0, 0} }; }
vvm count_coprime_path(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_eel
	
	int n = sz(g);

	auto dp = tree_getDP_once<T_ccp, merge_ccp, leaf_ccp>(g, r);

	vvm res(n);
	rep(i, n) {
		res[i].resize(sz(dp[i]));
		rep(j, sz(dp[i])) {
			auto [c0, c1, c2] = dp[i][j];
			res[i][j] = c0 + c1 + c2;
		}
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
T_st merge_st(T_st x, T_st y, int s) { return x * y; }
T_st e_st() { return 1; }
T_st leaf_st(int s) { return 1; }
T_st apply_st(T_st x, int p, int s) { return x + 1; }
vm count_subtree(Graph& g) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_v

	return rerooting<T_st, merge_st, e_st, leaf_st, apply_st>(g);
}


//【木の構築方法の数え上げ】O(n)
/*
* 与えられた木 g に対し，各 s∈[0..n) について，
* 頂点 s から始めて連結性を保ったまま辺の追加を行い木 g を構築する方法の数を格納したリストを返す．
*
* 利用：【全方位木 DP】,【階乗など（法が大きな素数）】
*/
using T_cctc = pair<mint, int>; // (構築方法の数, 辺の数)
Factorial_mint fm_cctc;
T_cctc merge_cctc(T_cctc x, T_cctc y, int s) {
	mint cnt = x.first * y.first * fm_cctc.bin(x.second + y.second, x.second);
	return { cnt, x.second + y.second };
}
T_cctc e_cctc() { return { 1, 0 }; }
T_cctc leaf_cctc(int s) { return { 1, 0 }; }
T_cctc apply_cctc(T_cctc x, int p, int s) { return { x.first, x.second + 1 }; }
vm count_continuous_tree_construction(Graph& g) {
	// verify : https://atcoder.jp/contests/abc160/tasks/abc160_f

	int n = sz(g);
	fm_cctc = Factorial_mint(n);

	auto dp = rerooting<T_cctc, merge_cctc, e_cctc, leaf_cctc, apply_cctc>(g);

	vm res(n);
	rep(i, n) res[i] = dp[i].first;

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

	if (k <= 0) return 0;

	int n = sz(g);

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
* 利用：【木の重心分解】
*/
vl tree_distance_frequency(const Graph& g) {
	// verify : https://judge.yosupo.jp/problem/frequency_table_of_tree_distance

	int n = sz(g);
	Centroid_decomposition<Graph> cd(g);

	vl res(n);

	// cent : 部分木の重心
	rep(cent, n) {
		// nc : cent に隣接する頂点の個数，c_dep : cent の深さ
		int nc = sz(g[cent]), c_dep = cd[cent].dep;

		// cnt[i][j] : cent を始点にもち，cent の i 番目の隣接頂点方向へ向かう長さ j のパスの本数
		vvl cnt(nc, vl(1, 1));

		// cnt_all[j] : cent を始点にもつ長さ j のパスの本数
		vl cnt_all(1, 1);

		// cent で分割された各部分木の cent に隣接する頂点を根として dfs する．
		rep(i, nc) {
			// cent と同じかそれより浅い頂点は無視する．
			if (cd[g[cent][i]].dep <= c_dep) continue;

			function<void(int, int, int)> dfs = [&](int s, int p, int len) {
				// 長さ len のパスの存在を記録する
				if (sz(cnt[i]) == len) cnt[i].resize(len + 1);
				cnt[i][len]++;

				if (sz(cnt_all) == len) cnt_all.resize(len + 1);
				cnt_all[len]++;

				// 再帰処理
				repe(t, g[s]) {
					// cent と同じかそれより浅い頂点は無視する．
					if (cd[t].dep <= c_dep || t == p) continue;

					dfs(t, s, len + 1);
				}
			};
			dfs(g[cent][i], -1, 1);
		}

		// cent を含むパスを数える．
		auto conv = convolution_ll(cnt_all, cnt_all);
		rep(j, 2 * sz(cnt_all) - 1) res[j] += conv[j];

		rep(i, nc) {
			// cent で折り返すパスを数えすぎているので引く．
			cnt[i][0] = 0;
			auto conv = convolution_ll(cnt[i], cnt[i]);
			rep(j, 2 * sz(cnt[i]) - 1) res[j] -= conv[j];
		}
	}

	rep(j1, n) res[j1] /= 2;

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
* 証明：【ラベル付き根付き木の数え上げ】の結果を，どれを根としているかの n で割り引けばよい．
*/


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


