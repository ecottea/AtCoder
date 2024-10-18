#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 木（変換） ■■■■■


//【プリューファーコード】
/*
* 頂点 [0..n) をもつラベル付き木 T と，[0..n)^(n-2) の要素 C とは以下の方法で 1:1 に対応する．
*
* 木 T → プリューファーコード C：
*	T の葉のうちラベルが最小のものを v とする．
*	C の末尾に v の隣接頂点のラベルを追加し，T から v を削除する．
*	以上の操作を |T| = 2 になるまで繰り返すことで C[0..n-2) を得る．
*
* プリューファーコード C → 木 T：
*	v[0] = mex( C[0..n-2) ) とし，T の v[0] と C[0] を結ぶ．
*	v[1] = mex( v[0..1), C[1..n-2) ) とし，T の v[1] と C[1] を結ぶ．
*	...
*	v[n-3] = mex( v[0..n-3), C[n-3..n-2) ) とし，T の v[n-3] と C[n-3] を結ぶ．
*	v[0..n-2) に含まれない T の 2 頂点を結ぶ．
*	（指定された内点 C[i] に対し，余っている番号最小の頂点を葉として繋げることを繰り返している）
*
* 性質：
*	(T の頂点 s の次数) = (C に現れる s の個数) + 1
*
* 参考 : https://drken1215.hatenablog.com/entry/2020/10/25/132900
*/


//【木 → プリューファーコード】O(n log n)
/*
* n 頂点の木 g に対応するプリューファーコード c[0..n-2) を返す．
*/
vi prufer_code(const Graph& g) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%97%E3%83%AA%E3%83%A5%E3%83%BC%E3%83%95%E3%82%A1%E3%83%BC%E5%88%97

	int n = sz(g);

	if (n <= 2) return vi{};
	vi code(n - 2);

	// g2 : 辺の削除がしやすい木
	vector<unordered_set<int>> g2(n);
	rep(s, n) repe(t, g[s]) g2[s].insert(t);

	// 次数が 1 の頂点（葉）から順に取り除いていく．
	priority_queue_rev<int> q;
	rep(s, n) if (sz(g2[s]) == 1) q.push(s);

	rep(i, n - 2) {
		// s : 番号が最小の葉
		auto s = q.top(); q.pop();

		// s に隣接する頂点 t を順に記録する．
		int t = *g2[s].begin();
		code[i] = t;

		// 頂点 s を削除する．
		g2[t].erase(s);

		// 頂点 t が新たに葉になったならキューに追加する．
		if (sz(g2[t]) == 1) q.push(t);
	}

	return code;
}


//【プリューファーコード → 木】O(n log n)
/*
* プリューファーコード c[0..n-2) に対応する n 頂点の木 g を返す．
*/
Graph from_prufer_code(const vi& c) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%97%E3%83%AA%E3%83%A5%E3%83%BC%E3%83%95%E3%82%A1%E3%83%BC%E5%88%97
	// verify : https://yukicoder.me/problems/no/2917

	int n = sz(c) + 2;
	Graph g(n);

	// deg[s] : 頂点 s の残り次数
	vi deg(n, 1);
	rep(i, n - 2) deg[c[i]]++;

	// 残り次数が 1 の頂点に繋がる辺から順に決定していく．
	priority_queue_rev<int> q;
	rep(s, n) if (deg[s] == 1) q.push(s);

	rep(i, n - 2) {
		// s : 残り次数が 1 の頂点のうち番号が最小のもの
		auto s = q.top(); q.pop();

		// s と t = c[i] を辺で繋ぐ．
		int t = c[i];
		g[s].push_back(t);
		g[t].push_back(s);

		// 頂点 t の残り次数を 1 減らす．
		deg[t]--;

		// 頂点 t の残り次数が 1 になったならキューに追加する．
		if (deg[t] == 1) q.push(t);
	}

	// 残り次数が 1 となる残った 2 つの頂点を辺で繋ぐ．
	auto s = q.top(); q.pop();
	auto t = q.top();
	g[s].push_back(t);
	g[t].push_back(s);

	return g;
}


