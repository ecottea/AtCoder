#pragma once
#include "header.h"
#include "分析(グラフ).h"
// ■■■■■ 0-1 計画問題 ■■■■■


//【燃やす埋める問題】O(n^2 (n + m))
/*
* n 個のゴミについて，ゴミ i を燃やすと非負コスト x[i], 埋めると非負コスト y[i] がかかる．
* また m 個の p[j] = {a, b, c} は a を燃やし b を埋めるときの追加の非負コスト c を表す．
* この状況下で全てのゴミを燃やすか埋めるかするときの最小コストを返す．
*/
ll burn_bury_problem(const vl& x, const vl& y, const vector<tuple<int, int, ll>>& p) {
	// 参考 :  https://github.com/E869120/kyopro_educational_90/blob/main/editorial/040.jpg
	// verify : https://atcoder.jp/contests/abc225/tasks/abc225_g

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


//【フローへの帰着】
/*
* x[0..n) が論理変数のとき，x[i] と !x[i] のちょうど一方が真である．
* これを ST -> X[i], X[i] -> x[i], X[i] -> !x[i] というコスト 1 の 3 辺に対応させる．
* その他の条件もグラフで表されれば，「フローが n 流れる ⇔ 充足可能」となる．
* 
* verify : https://atcoder.jp/contests/abc241/tasks/abc241_g
*/


//【2-SAT】
/*
* Two_sat(int n) : O(n)
*	n 変数で初期化する．
*
* void add_clause(int i, bool bi, int j, bool bj)：O(1)
*	条件 (x[i] = bi) OR (x[j] = bj) を追加する．
*
* bool satisfiable() : O(n + m)（m は制約の数）
*	全ての条件を AND したものが充足可能かを返す．
*
* vb answer() : O(n)
*	真理値解を返す．
*
* 利用：【強連結成分分解】
*/
struct Two_sat {
	// 参考：https://tjkendev.github.io/procon-library/python/graph/2-sat.html
	// verify : https://judge.yosupo.jp/problem/two_sat

	int n; // 変数の数
	Graph g; // 頂点は g[2*i] : !x[i], g[2*i+1] : x[i] に対応
	vvi scc; // g の強連結成分分解結果
	vb sol; // 真理値解

	// n 変数で初期化する．
	Two_sat(int n_) : n(n_), g(2 * n), sol(n) {}

	// 条件 (x[i] = bi) OR (x[j] = bj) を追加する．
	void add_clause(int i, bool bi, int j, bool bj) {
		// a OR b <-> (!a => b AND !b => a) と考え辺を張る．
		g[2 * i + (int)(!bi)].push_back(2 * j + (int)(bj));
		g[2 * j + (int)(!bj)].push_back(2 * i + (int)(bi));
	}

	// 全ての条件を AND したものが充足可能かを返す．
	bool satisfiable() {
		// g を強連結成分分解する．
		strongly_connected_component(g, scc);

		// 変数 → 何番目の強連結成分に属するか
		vi x_to_c(n, -1);

		rep(i, sz(scc)) {
			repe(v, scc[i]) {
				int x = v / 2; bool b = (bool)(v % 2);

				// x ⇒ !x と !x ⇒ x が共にあれば充足不可能
				if (x_to_c[x] == i) return false;

				// x ⇒ !x があるなら x = false とする．
				if (x_to_c[x] == -1) {
					sol[x] = !b;
					x_to_c[x] = i;
				}
			}
		}

		return true;
	}

	// 真理値解を返す．
	vb answer() { return sol; }

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, Two_sat ts) {
		rep(s, ts.n) {
			os << "!" << s << " => ";
			repe(t, ts.g[2 * s]) {
				int x = t / 2; bool b = (bool)(t % 2);
				if (!b) os << "!";
				os << x << " ";
			}
			os << endl;

			os << s << " => ";
			repe(t, ts.g[2 * s + 1]) {
				int x = t / 2; bool b = (bool)(t % 2);
				if (!b) os << "!";
				os << x << " ";
			}
			os << endl;
		}
		return os;
	}
};


