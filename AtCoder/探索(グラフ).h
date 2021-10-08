#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ ■■■■■



//【幅優先探索】O(|E|)
/*
* グラフ g に対し，始点を start として幅優先探索を行い，
* start から各頂点 i への最短経路長を dist[i] に格納する．
* i が start から到達不能な頂点の場合は dist[i] = -1 となる．
*/
void breadth_first_search(const Graph& g, int start, vi& dist) {
	int n = sz(g);
	dist = vi(n, -1); // スタートからの最短距離を保持するテーブル
	dist[start] = 0;
	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(start);

	while (!que.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = que.front();
		que.pop();

		for (auto t : g[s]) {
			if (dist[t] != -1) {
				// 発見済みの頂点なので何もしない．
				continue;
			}

			// スタートからの最短距離を確定する．
			// 幅優先探索なので，最短だという保証がある．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}
}


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


