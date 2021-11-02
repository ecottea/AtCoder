#pragma once
#include "header.h"
// ■■■■■ 0-1 計画問題 ■■■■■



//【燃やす埋める問題】O(n^2 (n + m))
/*
* n 個のゴミについて，ゴミ i を燃やすと非負コスト x[i], 埋めると非負コスト y[i] がかかる．
* また m 個の p[j] = {a, b, c} は a を燃やし b を埋めるときの追加の非負コスト c を表す．
* この状況下で全てのゴミを燃やすか埋めるかするときの最小コストを返す．
*/
ll burn_bury_problem(const vl& x, const vl& y, const vector<tuple<int, int, ll>>& p) {
	// 参考 :  https://github.com/E869120/kyopro_educational_90/blob/main/editorial/040.jpg

	//【方法】
	// 頂点 S, T, [0..n) をもつ以下のグラフ G 上の最小カット問題に帰着させる：
	//		∀i=[0..n), S → i ：容量 y[i]
	//		∀i=[0..n), i → T ：容量 x[i]
	//		∀{a, b, c} ∈ p, a → b ：容量 c
	//
	// 辺がカットされたか否かとゴミを燃やしたか埋めたかの対応は以下の通り：
	//	  S と i が繋がっている			   T と i が繋がっている
	// ⇔ T と i がカットされている		⇔ S と i がカットされている
	// ⇔ コスト y[i] を支払った		⇔ コスト x[i] を支払った
	// ⇔ ゴミ i を燃やした				⇔ ゴミ i を埋めた

	int n = sz(x);

	const int S = n, T = n + 1;
	mf_graph<ll> g(n + 2);

	rep(i, n) {
		g.add_edge(S, i, y[i]);
		g.add_edge(i, T, x[i]);
	}

	repe(tmp, p) {
		int a, b; ll c;
		tie(a, b, c) = tmp;

		g.add_edge(a, b, c);
	}

	return g.flow(S, T);
}


//【project selection problem】O((n + m)^2 (n + m + c))
/*
* n 個の計画があり，計画 i を実行すると非負の利益 x[i] を得る．
* また m 個の機械があり，機械 j を購入すると非負のコスト y[i] がかかる．
* c 個の p[k] = {a, b} は計画 a の実行には機械 b が必要であることを表す．
* この状況下で得られる最大利益を返す．
*/
ll project_selection_problem(const vl& x, const vl& y, const vector<pii>& p) {
	// 参考 : https://kmyk.github.io/algorithm-encyclopedia-staging/project-selection-problem#noredirect

	//【方法】
	// 頂点 S, T, a[0..n), b[0..m) をもつ以下のグラフ G 上の最小カット問題に帰着させる：
	//		∀i = [0..n), S → i ：容量 x[i]
	//		∀j = [0..m), j → T ：容量 y[i]
	//		∀{a, b} ∈ p, a → b ：容量 +∞
	//
	// あらかじめ総利益 Σx[i] を得たことにして，
	// 計画 i を実行しなかったときコスト x[i] を払うものとする．
	// 
	// 辺がカットされたか否かとゴミを燃やしたか埋めたかの対応は以下の通り：
	//	  S と i がカットされている		   T と j がカットされている
	// ⇔ コスト x[i] を支払った		⇔ コスト y[i] を支払った
	// ⇔ 計画 i を実行しなかった		⇔ 機械 j を購入した
	//
	//    S と i が繋がっている			   T と j が繋がっている
	// ⇔ 計画 i を実行した				⇔ 機械 j を購入しなかった

	int n = sz(x), m = sz(y);

	const int S = n + m, T = n + m + 1;
	mf_graph<ll> g(n + m + 2);

	rep(i, n) g.add_edge(S, i, x[i]);
	rep(j, m) g.add_edge(j, T, y[j]);

	repe(tmp, p) {
		int a, b;
		tie(a, b) = tmp;

		g.add_edge(a, b, INFL);
	}

	return accumulate(all(x), 0LL) - g.flow(S, T);
}


