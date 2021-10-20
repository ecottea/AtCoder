#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ ■■■■■



//【深さ優先探索】O(|E|)
/*
* グラフ g に対し，始点を s として深さ優先探索を行い，
* 行きがけ順に pre(i)，帰りがけ順に post(i) を実行する．
*/
template <class G>
void depth_first_search(G& g, int s, function<void(int)>& pre, function<void(int)>& post) {
	int n = sz(g);

	// 頂点を訪れたことを記録しておくテーブル．
	vb seen(n);

	// 再帰用の関数
	function<void(int)> dfs = [&](int s) {
		// 頂点を訪れたことを記録
		seen[s] = true;

		// 行きがけ順の処理
		pre(s);

		for (auto t : g[s]) {
			// 未探索の頂点を探索しにいく．
			if (!seen[t]) {
				dfs(t);
			}
		}

		// 帰りがけ順の処理
		post(s);

		//visited[s] = false; // これをつけるとバックトラッキングになる．
	};

	// s を始点として再帰関数を呼び出す．
	dfs(s);
}


