#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 二乗の木 DP ■■■■■



//【部分木の数え上げ】O(|V|^2)
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
		dp[s] = vm(ws + 1LL, 1);

		// s の子 t それぞれについて
		repe(t, g[s]) {
			if (t == p) continue;

			// wt : 部分木 t の大きさ
			int wt = dfs(t, s);

			// ndps[i] : 部分木 s に部分木 t をマージした後の大きさ i の部分木の個数
			vm ndps(ws + wt + 1);
			repi(i, 1, ws) {
				repi(j, 0, wt) {
					ndps[(ll)i + j] += dp[s][i] * dp[t][j];
				}
			}

			dp[s] = ndps;
			ws += wt;
		}
		dp[s][0] = 1; // 空の部分木

		return ws;
	};

	dfs(r, -1);
	cnt = dp[r];
}


//【部分木の数え上げ】O(|V| k)
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
		dp[s] = vm(ws + 1LL, 1);

		// s の子 t それぞれについて
		repe(t, g[s]) {
			if (t == p) continue;

			// wt : 部分木 t の大きさ
			int wt = dfs(t, s);

			// ndps[i] : 部分木 s に部分木 t をマージした後の大きさ i の部分木の個数
			vm ndps(min(ws + wt, k) + 1);
			repi(i, 1, min(ws, k)) {
				repi(j, 0, min(wt, k - i)) {
					ndps[(ll)i + j] += dp[s][i] * dp[t][j];
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


//【互いに素なパスの数え上げ】O(|V| k)
/*
* 木 g 上の k 本の互いに素なパスの組の個数を返す．
*
*（二乗の木 DP）
*/
mint count_coprime_path(Graph& g, int k) {
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
			vvm ndps(3, vm(min((ws + wt) / 2, k) + 1LL));

			repi(i, 0, min(ws / 2, k)) {
				repi(j, 0, min(wt / 2, k - i + 1)) {
					// 辺 (s, t) がパスに属さない場合
					mint sum = dp[t][0][j] + dp[t][1][j] + dp[t][2][j];
					rep(c, 3) {
						ndps[c][(ll)i + j] += dp[s][c][i] * sum;
					}

					// 辺 (s, t) がパスに属する場合
					if (i + j < sz(ndps[0]) - 1) {
						ndps[1][(ll)i + j + 1] += dp[s][0][i] * dp[t][0][j];
					}
					if (i + j < sz(ndps[0])) {
						ndps[1][(ll)i + j] += dp[s][0][i] * dp[t][1][j];
						ndps[2][(ll)i + j] += dp[s][1][i] * dp[t][0][j];
					}
					if (i + j > 0) {
						ndps[2][(ll)i + j - 1] += dp[s][1][i] * dp[t][1][j];
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


