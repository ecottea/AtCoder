#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 木 DP ■■■■■



//【独立集合の数え上げ】O(|V|)
/*
* 木 g の独立集合（辺を共有しない頂点の集合）の個数を返す．
*
*（木の状態 DP）
*/
mint count_independent_set(Graph& g) {
	int n = sz(g);

	// s : 注目頂点，c : s が独立集合に含まれるか，p : 親頂点
	// 戻り値 : 部分木 s に含まれる独立集合の個数（便宜上 0 を根とした根付き木とみなす）
	function<mint(int, int, int)> dfs = [&](int s, int c, int p) {
		// 積についての単位元で初期化しておく．
		mint res = 1;

		// s のそれぞれの子 t について処理を行う．
		for (auto t : g[s]) {
			// 親には戻らない
			if (t == p) continue;

			// t が独立集合に含まれないような部分木は候補になる．
			mint mul = dfs(t, 0, s);
			if (!c) {
				// s が独立集合に含まれないならば，
				// t が独立集合に含まれるような部分木も候補になる．
				mul += dfs(t, 1, s);
			}

			// s の部分木については独立なので，積の法則で数え上げる．
			res *= mul;
		}

		return res;
	};

	return dfs(0, 0, -1) + dfs(0, 1, -1);
}


