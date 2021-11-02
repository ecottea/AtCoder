#pragma once
#include "header.h"
#include "最短路.h"
// ■■■■■ 線形計画問題 ■■■■■



//【牛ゲー】
/*
* Ushige(n) : O(1)
*	n 変数で初期化する．
*
* set_ub(a, b, d) : O(1)
*	v[b] - v[a] <= d という制約を追加する．
*
* set_lb(a, b, d) : O(1)
*	v[b] - v[a] >= d という制約を追加する．
*
* maximize_diff(a, diff) : O(n m)（m : 制約の数）
*	v[b] - v[a] の最大値（無いなら INFL）を diff[b] に格納する．
*	制約を満たすことが不可能なら false を返す．
*
* 利用：【単一始点最短路／ベルマン・フォード法】
*/
struct Ushige {
	int n;
	WGraph g;

	Ushige() : n(0) {}

	Ushige(int n_) : n(n_), g(n_) {}

	void set_ub(int a, int b, ll d) {
		// 差の上限に対応する重みを持つ辺を張る．
		g[a].push_back({ b, d });
	}

	void set_lb(int a, int b, ll d) {
		// 差の下限に対応する重みを持つ辺を張る．
		g[b].push_back({ a, -d });
	}

	bool maximize_diff(int a, vl& diff) {
		// a を始点とする最短経路問題を解く．
		// b までの最短経路長がそのまま v[b] - v[a] の最大値になる．
		// ただし負の閉路を持っていた場合は制約を満たせない．
		return bellman_ford(g, a, diff);
	}
};



