#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 木の数え上げ ■■■■■


//【森の連結成分の数】
/*
* 森 g の連結成分の個数は，(g の頂点数) - (g の辺数) に等しい．
*/


//【誘導部分グラフの連結成分の数】
/*
* 根付き木 g の誘導部分グラフ h の連結成分の個数は，
* 親が h に属さない（または g の根である）ような h の頂点の個数に等しい．
* 
* verify : https://atcoder.jp/contests/abc173/tasks/abc173_f
*/


//【独立集合の数え上げ】O(n)
/*
* 木 g の独立集合（辺を共有しない頂点の集合）の個数を返す．
*
*（木の状態 DP）
*/
mint count_independent_set(Graph& g) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	int n = sz(g);

	// 便宜上 g を 0 を根とした根付き木とみなす．
	// dp[s][b] : 部分木 s の独立集合で，
	//			  s が独立集合に b=1:含まれる[b=0:含まれない] ものの個数
	vvm dp(n, vm(2));
	vvb seen(n, vb(2));

	// p : s の親頂点
	function<mint(int, int, int)> dfs = [&](int s, int b, int p) {
		if (seen[s][b]) return dp[s][b];
		seen[s][b] = true;

		// 積についての単位元で初期化しておく．
		dp[s][b] = 1;

		// s のそれぞれの子 t について処理を行う．
		repe(t, g[s]) {
			if (t == p) continue;

			// t が独立集合に含まれないような部分木は候補になる．
			mint mul = dfs(t, 0, s);
			if (b == 0) {
				// s が独立集合に含まれないならば，
				// t が独立集合に含まれるような部分木も候補になる．
				mul += dfs(t, 1, s);
			}

			// s の部分木については独立なので，積の法則で数え上げる．
			dp[s][b] *= mul;
		}

		return dp[s][b];
	};

	return dfs(0, 0, -1) + dfs(0, 1, -1);
}


