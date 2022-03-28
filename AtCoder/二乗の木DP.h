#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 二乗の木 DP ■■■■■


//【部分木の数え上げ】O(n^2)
/*
* 木 g の頂点 r を含む大きさ i の部分木の個数を cnt[i] に格納する．
*
*（二乗の木 DP）
*/
void count_subtree(const Graph& g, int r, vm& cnt) {
	// 参考 : https://snuke.hatenablog.com/entry/2019/01/15/211812

	int n = sz(g);

	// 便宜上 g を r を根とする根付き木とみなす．
	// dp[v][i] : v を根とする大きさ i の部分木の個数
	vvm dp(n);
	
	// s : 注目頂点，p : s の親，戻り値 : 部分木 s の大きさ
	function<int(int, int)> dfs = [&](int s, int p) {
		int ws = 1; // 部分木 s の大きさ

		// ひとまず s を含む部分木を数え上げる．
		dp[s] = vm(ws + 1, 1);

		// s の子 t それぞれについて
		repe(t, g[s]) {
			if (t == p) continue;

			// wt : 部分木 t の大きさ
			int wt = dfs(t, s);

			// ndps[i] : 部分木 s に部分木 t をマージした後の大きさ i の部分木の個数
			// これは畳み込みなので，mod998244353 なら高速化できる．
			vm ndps(ws + wt + 1);
			repi(i, 1, ws) {
				repi(j, 0, wt) {
					ndps[i + j] += dp[s][i] * dp[t][j];
				}
			}

			dp[s] = ndps;
			ws += wt;
		}

		// s を含まない部分木は空の部分木のみである．
		dp[s][0] = 1;

		return ws;
	};

	dfs(r, -1);
	cnt = dp[r];
}


//【部分木の数え上げ】O(n k)
/*
* 木 g の頂点 r を含む大きさ k の部分木の個数を返す．
*
*（二乗の木 DP）
*/
mint count_subtree(const Graph& g, int r, int k) {
	// 参考 : https://snuke.hatenablog.com/entry/2019/01/15/211812

	int n = sz(g);

	// 便宜上 g を r を根とする根付き木とみなす．
	// dp[v][i] : v を根とする大きさ i の部分木の個数
	vvm dp(n);
	
	// s : 注目頂点，p : s の親，戻り値 : 部分木 s の大きさ
	function<int(int, int)> dfs = [&](int s, int p) {
		int ws = 1; // 部分木 s の大きさ
		dp[s] = vm(ws + 1, 1);

		// s の子 t それぞれについて
		repe(t, g[s]) {
			if (t == p) continue;

			// wt : 部分木 t の大きさ
			int wt = dfs(t, s);

			// ndps[i] : 部分木 s に部分木 t をマージした後の大きさ i の部分木の個数
			// これは畳み込みなので，mod998244353 なら高速化できる．
			vm ndps(min(ws + wt, k) + 1);
			repi(i, 1, min(ws, k)) {
				repi(j, 0, min(wt, k - i)) {
					ndps[i + j] += dp[s][i] * dp[t][j];
				}
			}

			dp[s] = ndps;
			ws += wt;
		}
		dp[s][0] = 1; // 空の部分木

		return ws;
	};

	dfs(r, -1);

	return dp[r][k];
}


//【部分木の最小コスト】O(n^2)
/*
* 頂点にコスト c[0..n) が与えられた木 g について，
* 頂点 r を含む大きさ i の部分木の最小コストを cost[i] に格納する．
*
*（二乗の木 DP）
*/
void minimum_cost_subtree(const Graph& g, const vl& c, int r, vl& cost) {
	// verify : https://atcoder.jp/contests/arc029/tasks/arc029_4

	int n = sz(g);

	// 便宜上 g を r を根とする根付き木とみなす．
	// dp[v][i] : v を根とする大きさ i の部分木の最小コスト
	vvl dp(n);

	// s : 注目頂点，p : s の親，戻り値 : 部分木 s の大きさ
	function<int(int, int)> dfs = [&](int s, int p) {
		int ws = 1; // 部分木 s の大きさ

		// ひとまず s を含む部分木を考える．
		dp[s] = vl({ 0, c[s] });

		// s の子 t それぞれについて
		repe(t, g[s]) {
			if (t == p) continue;

			// wt : 部分木 t の大きさ
			int wt = dfs(t, s);

			// ndps[i] : 部分木 s に部分木 t をマージした後の大きさ i の部分木の最小コスト
			vl ndps(ws + wt + 1, INFL);
			repi(i, 1, ws) {
				repi(j, 0, wt) {
					chmin(ndps[i + j], dp[s][i] + dp[t][j]);
				}
			}

			dp[s] = ndps;
			ws += wt;
		}

		// s を含まない部分木は空の部分木のみである．
		dp[s][0] = 0;

		return ws;
	};

	dfs(r, -1);
	cost = dp[r];
}


//【木の誘導部分グラフの数え上げ】O(n^2)
/*
* 木 g の k 本の辺からなる誘導部分グラフの個数を cnt[k] に格納する．
* 誘導部分グラフとは，その頂点対の辺の有無が g と一致する g の部分グラフをいう．
*
*（二乗の木状態 DP）
*/
void count_induced_subtree(const Graph& g, vm& cnt) {
	int n = sz(g);
	cnt.resize(n);

	// 便宜上 0 を根とした根付き木とみなす．
	// dp[s][k][type] : 部分木 s の，k 本の辺からなる誘導部分グラフで，
	//		s を type = 1:含む[0:含まない] ものの個数
	vvvm dp(n);

	// s : 注目頂点，p : s の親，戻り値 : 部分木 s の大きさ
	function<int(int, int)> dfs = [&](int s, int p) {
		// ws : 部分木 s の大きさ
		int ws = 1;

		// 根 s のみからなる木
		dp[s] = vvm(ws, vm(2, 1));

		// s の子 t を順に s にマージしていく．
		repe(t, g[s]) {
			if (t == p) continue;

			// wt : 部分木 t の大きさ
			int wt = dfs(t, s);

			// ndps[k][type] : 部分木 s に部分木 t をマージした後の個数
			vvm ndps(ws + wt, vm(2));
			rep(ks, ws) {
				rep(kt, wt) {
					ndps[ks + kt][0] += dp[s][ks][0] * dp[t][kt][0];
					ndps[ks + kt][0] += dp[s][ks][0] * dp[t][kt][1];
					ndps[ks + kt][1] += dp[s][ks][1] * dp[t][kt][0];
					ndps[ks + kt + 1][1] += dp[s][ks][1] * dp[t][kt][1];
				}
			}

			dp[s] = ndps;
			ws += wt;
		}

		return ws;
	};

	dfs(0, -1);

	rep(s, n) cnt[s] = dp[0][s][0] + dp[0][s][1];
}


//【互いに素なパスの数え上げ】O(n k)
/*
* 木 g 上の k 本の互いに素なパス（長さ 0 は不可）の組の個数を返す．
*
*（二乗の木状態 DP）
*/
mint count_coprime_path(Graph& g, int k) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_eel

	int n = sz(g);

	// 便宜上 g を 0 を根とする根付き木とみなす．
	// dp[s][c][i] : s を根とする部分木上の i 本の互いに素なパスの組で以下の条件
	//	c = 0 : s がパスに属さない
	//	c = 1 : s がパスの端点
	//	c = 2 : s がパスに属し端点以外
	// を満たすものの個数．
	vvvm dp(n);

	// s : 注目頂点，p : 親頂点，戻り値 : 部分木 s の大きさ
	function<int(int, int)> dfs = [&](int s, int p) {
		int ws = 1; // 部分木 s の大きさ
		dp[s] = vvm(3, vm(1));
		dp[s][0][0] = 1;

		repe(t, g[s]) {
			if (t == p) continue;

			// wt : 部分木 t の大きさ
			int wt = dfs(t, s);

			// ndps : 部分木 s に部分木 t をマージした後の部分木の個数
			vvm ndps(3, vm(min((ws + wt) / 2, k) + 1));

			repi(i, 0, min(ws / 2, k)) {
				repi(j, 0, min(wt / 2, k - i + 1)) {
					// 辺 (s, t) がパスに属さない場合
					mint sum = dp[t][0][j] + dp[t][1][j] + dp[t][2][j];
					rep(c, 3) {
						ndps[c][i + j] += dp[s][c][i] * sum;
					}

					// 辺 (s, t) がパスに属する場合
					if (i + j < sz(ndps[0]) - 1) {
						ndps[1][i + j + 1] += dp[s][0][i] * dp[t][0][j];
					}
					if (i + j < sz(ndps[0])) {
						ndps[1][i + j] += dp[s][0][i] * dp[t][1][j];
						ndps[2][i + j] += dp[s][1][i] * dp[t][0][j];
					}
					if (i + j > 0) {
						ndps[2][i + j - 1] += dp[s][1][i] * dp[t][1][j];
					}
				}
			}

			dp[s] = ndps;
			ws += wt;
		}

		return ws;
	};

	dfs(0, -1);

	mint res = 0;
	if (sz(dp[0][0]) > k) {
		res = dp[0][0][k] + dp[0][1][k] + dp[0][2][k];
	}

	return res;
}


//【先祖-子孫関係にない頂点集合の数え上げ】O(n^2)
/*
* r を根とする根付き木 g の大きさ i の頂点集合のうち，
* どの 2 頂点も先祖-子孫の関係にないものの個数を cnt[i] に格納する．
*
*（二乗の木 DP）
*/
void count_nonancestor_set(const Graph& g, int r, vm& cnt) {
	int n = sz(g);

	// dp[v][i] : v を根とする大きさ i の頂点集合の個数
	vvm dp(n);

	// s : 注目頂点，p : s の親，戻り値 : 部分木 s の大きさ
	function<int(int, int)> dfs = [&](int s, int p) {
		int ws = 1; // 部分木 s の大きさ

		// ひとまず s を選ばないものとして数え上げる．
		dp[s] = vm({ 1, 0 });

		// s の子 t それぞれについて
		repe(t, g[s]) {
			if (t == p) continue;

			// wt : 部分木 t の大きさ
			int wt = dfs(t, s);

			// ndps[i] : 部分木 s に部分木 t をマージした後の大きさ i の頂点集合の個数
			// これは畳み込みなので，mod998244353 なら高速化できる．
			vm ndps(ws + wt + 1);
			repi(i, 0, ws) {
				repi(j, 0, wt) {
					ndps[i + j] += dp[s][i] * dp[t][j];
				}
			}

			dp[s] = ndps;
			ws += wt;
		}

		// s を選べるのは根だけの一点集合のみである．
		dp[s][1]++;

		return ws;
	};

	dfs(r, -1);

	cnt = dp[r];
}


