#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ ゲーム ■■■■■


// 【点取りゲーム】O(|V| + |E|)　
/*
* ゲームのルール：
* コスト付き有向非巡回グラフ g のある頂点 v にコマが置かれている．
* 先手と後手は交互にコマを辺で繋がれた頂点のいずれかへ動かし，辺のコスト分の得点を得る．
* コマが移動不可能になったとき，自分がより高い点数を得ていることを目指す．
*
* res[v] = {f, l} : v にコマがある状態から最善を尽くしたときの f:先手[l:後手] の点数
*/
void score_game(const WGraph& g, vector<pll>& res) {
	int n = sz(g);

	vb seen(n);
	res = vector<pll>(n);

	function<void(int)> dfs = [&](int s) {
		// s の情報を計算済だったら何もしない．
		if (seen[s]) return;
		seen[s] = true;

		// コマが移動不能になったら終了．
		if (g[s].empty()) {
			res[s] = { 0, 0 };
			return;
		}

		// s から出ている辺 e の情報を元に s の情報を得る．
		res[s] = { -INFL, INFL };
		repe(e, g[s]) {
			// e を辿った場合の結果を求める．
			dfs(e.to);

			// e を辿る方が利得が大きいなら更新する．
			if (res[s].first < res[e.to].second + e.cost) {
				res[s].first = res[e.to].second + e.cost;
				res[s].second = res[e.to].first;
			}
		}
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);
}

