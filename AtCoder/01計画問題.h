#pragma once
#include "header.h"
#include "分析(グラフ).h"
#include "フロー.h"
// ■■■■■ 0-1 計画問題 ■■■■■


//【二次 0-1 計画問題（燃やす埋める問題）】
/*
* Binary_programming_BB(int n) : O(1)
*	n 変数 X[0..n) で初期化する．
*
* add_cost0(int i, ll c) : O(1)
*	X[i] = 0 のとき非負コスト c がかかるようにする．
*
* add_cost1(int i, ll c) : O(1)
*	X[i] = 1 のとき非負コスト c がかかるようにする．
*
* add_profit0(int i, ll p) : O(1)
*	X[i] = 0 のとき非負利益 p が得られるようにする．
*
* add_profit1(int i, ll p) : O(1)
*	X[i] = 1 のとき非負利益 p が得られるようにする．
*
* add_cost01(int i, int j, ll c) : O(1)
*	X[i] = 0 かつ X[j] = 1 のとき非負コスト c がかかるようにする．
*
* add_profit00(int i, int j, ll p) : O(1)
*	X[i] = 0 かつ X[j] = 0 のとき非負利益 p が得られるようにする．
*
* add_profit11(int i, int j, ll p) : O(1)
*	X[i] = 1 かつ X[j] = 1 のとき非負利益 p が得られるようにする．
*
* ll solve() : O(n^2 m)（m : 条件の数）
*	得られる最大利益を返す．
*/
struct Binary_programming_BB {
	// 参考 : https://yosupo.hatenablog.com/entry/2015/03/31/134336

	// n : 01-変数の数
	int n;
	
	// profit : 前借りしている非負利益
	ll profit = 0;

	// cost0[i] : X[i] = 0 のときにかかる非負コスト
	// cost1[i] : X[i] = 1 のときにかかる非負コスト
	vl cost0, cost1;

	// cost01[i][j] : X[i] = 0 かつ X[j] = 1 のときにかかる非負コスト
	vvl cost01;

	// n 変数で初期化
	Binary_programming_BB(int n_) : n(n_), cost0(n), cost1(n), cost01(n, vl(n)) {}

	// X[i] = 0 のとき非負コスト c がかかるようにする．
	void add_cost0(int i, ll c) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_an

		Assert(c >= 0);

		cost0[i] += c;
	}

	// X[i] = 1 のとき非負コスト c がかかるようにする．
	void add_cost1(int i, ll c) {
		// verify : https://atcoder.jp/contests/abc225/tasks/abc225_g

		Assert(c >= 0);

		cost1[i] += c;
	}

	// X[i] = 0 のとき非負利益 p が得られるようにする．
	void add_profit0(int i, ll p) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_an

		Assert(p >= 0);

		// 利益 p を前借りしておき，X[i] = 1 のときコスト p がかかると言い換えればよい．
		profit += p;
		cost1[i] += p;
	}

	// X[i] = 1 のとき非負利益 p が得られるようにする．
	void add_profit1(int i, ll p) {
		// verify : https://atcoder.jp/contests/abc225/tasks/abc225_g

		Assert(p >= 0);

		// 利益 p を前借りしておき，X[i] = 0 のときコスト p がかかると言い換えればよい．
		profit += p;
		cost0[i] += p;
	}

	// X[i] = 0 かつ X[j] = 1 のとき非負コスト c がかかるようにする．
	void add_cost01(int i, int j, ll c) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_an

		Assert(c >= 0);

		cost01[i][j] += c;
	}

	// X[i] = 0 かつ X[j] = 0 のとき非負利益 p が得られるようにする．
	void add_profit00(int i, int j, ll p) {
		Assert(p >= 0);

		// 利益 p を前借りしておき，
		//		X[i] = 1 のときコスト p がかかる
		//		X[i] = 0 かつ X[j] = 1 のときコスト p がかかる
		// と言い換えればよい．
		profit += p;
		cost1[i] += p;
		cost01[i][j] += p;
	}

	// X[i] = 1 かつ X[j] = 1 のとき非負利益 p が得られるようにする．
	void add_profit11(int i, int j, ll p) {
		// verify : https://atcoder.jp/contests/abc225/tasks/abc225_g

		Assert(p >= 0);

		// 利益 p を前借りしておき，
		//		X[j] = 0 のときコスト p がかかる
		//		X[i] = 0 かつ X[j] = 1 のときコスト p がかかる
		// と言い換えればよい．
		profit += p;
		cost0[j] += p;
		cost01[i][j] += p;
	}

	// 最大利益を返す．
	ll solve() {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_an

		// ST : 始点（恒等的に 0），GL : 終点（恒等的に 1）
		// g の残余ネットワークで ST から到達可能な頂点は 0，それ以外は 1 とにする．
		int ST = n, GL = n + 1;
		mf_graph<ll> g(n + 2);
		
		rep(i, n) {
			// X[i] = 0 にコスト c0， X[i] = 1 にコスト c1 がかかるとき
			//	c = min(c0, c1) として確定でコスト c がかかるとし，
			//		c0 > c1 なら X[i] = 0 にコスト c0 - c がかかる
			//		c0 < c1 なら X[i] = 1 にコスト c1 - c がかかる
			// としてよい．
			ll c = min(cost0[i], cost1[i]);
			profit -= c;

			if (cost0[i] > cost1[i]) {
				// X[i] = 0 にコスト c0 - c がかかるとき
				//	X[i] = 0 かつ X[GL] = 1 だとコスト c0 - c がかかると言い換えられる．
				//	よって辺 i → GL をカットすることにコスト c0 - c を課せば良い．
				g.add_edge(i, GL, cost0[i] - c);
			}
			else if (cost0[i] < cost1[i]) {
				// X[i] = 1 にコスト c1 - c がかかるとき
				//	X[ST] = 0 かつ X[i] = 1 だとコスト c1 - c がかかると言い換えられる．
				//	よって辺 ST → i をカットすることにコスト c1 - c を課せば良い．
				g.add_edge(ST, i, cost1[i] - c);
			}
		}

		rep(i, n) rep(j, n) {
			// X[i] = 0 かつ X[j] = 1 にコスト c がかかるとき
			//	そのまま辺 i → j をカットすることにコスト c を課せば良い．
			if (cost01[i][j] > 0) g.add_edge(i, j, cost01[i][j]);
		}

		return profit - g.flow(ST, GL);
	}
};


//【三次の利益】
/*
* X[i] = 0 かつ X[j] = 0 かつ X[k] = 0 のとき非負利益 p が得られることは，
* 利益 p を前借りした上で変数 Y を追加し，
*	Y = 1 のときコスト p
*	Y = 0 かつ X[i] = 1 のときコスト ∞
*	Y = 0 かつ X[j] = 1 のときコスト ∞
*	Y = 0 かつ X[k] = 1 のときコスト ∞
* とすることで二次の項のみで表現できる．
*/


//【project selection problem】O((n + m)^2 (n + m + c))
/*
* n 個の計画があり，計画 i を実行すると非負の利益 x[i] を得る．
* また m 個の機械があり，機械 j を購入すると非負のコスト y[j] がかかる．
* c 個の p[k] = {i, j} は計画 i の実行には機械 j が必要であることを表す．
* この状況下で得られる最大利益を返す．
* また実行した計画を x_ids に，購入した機械を y_ids にそれぞれ格納する．
*/
ll project_selection_problem(const vl& x, const vl& y, const vector<pii>& p,
	vi* x_ids = nullptr, vi* y_ids = nullptr)
{
	// verify : https://atcoder.jp/contests/arc031/tasks/arc031_4

	//【方法】
	// あらかじめ総利益 Σx[i] を得たことにして，
	// 計画 i を実行しなかったときコスト x[i] を払うものとする．
	// これによりコストを最小化する問題だとみなすことができる．
	// 
	// 頂点 S, a[0..n), b[0..m), T をもつ以下のグラフ G 上の最小カット問題に帰着させる：
	//		∀i = [0..n), S → a[i] ：容量 x[i]
	//		∀j = [0..m), b[j] → T ：容量 y[j]
	//		∀{i, j} ∈ p, a[i] → b[j] ：容量 +∞
	//
	// 辺がカットされたか否かと計画を実行したか[機械を購入したか] の対応は以下の通り：
	//	  S と i がカットされている	   T と j がカットされている
	// ⇔ コスト x[i] を支払った		⇔ コスト y[j] を支払った
	// ⇔ 計画 i を実行しなかった		⇔ 機械 j を購入した
	//
	//    S と i が繋がっている		   T と j が繋がっている
	// ⇔ 計画 i を実行した			⇔ 機械 j を購入しなかった

	int n = sz(x), m = sz(y);

	mf_graph<ll> g(n + m + 2);
	int S = n + m, T = S + 1;

	rep(i, n) g.add_edge(S, i, x[i]);
	rep(j, m) g.add_edge(n + j, T, y[j]);

	repe(tmp, p) {
		int a, b;
		tie(a, b) = tmp;

		g.add_edge(a, n + b, INFL);
	}

	ll res = accumulate(all(x), 0LL) - g.flow(S, T);

	if (x_ids != nullptr) {
		x_ids->clear();
		y_ids->clear();

		vb mc = g.min_cut(S);

		rep(i, n) if (mc[i]) x_ids->emplace_back(i);
		rep(j, m) if (mc[n + j]) y_ids->emplace_back(j);
	}

	return res;
}


//【XOR の最大化】
/*
* 集合 a[0..n) について max_(set ⊂ a[0..n)) (XOR set) を求めるには，
* 右辺を 11..11 にして【連立一次方程式（優先度付き）】を利用すればよい．
* 
* verify : https://atcoder.jp/contests/abc141/tasks/abc141_f
*/


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
		scc = strongly_connected_component(g);

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

#ifdef _MSC_VER
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
#endif
};


//【2-SAT の解の数え上げ】
/*
* 2-SAT の解の数え上げを多項式時間で行えるアルゴリズムは見つかっていない．
*/


//【割り当て問題】O(n^3 k log(n))
/*
* a[0..n)[0..n) の各行各列から k 個以下要素を選択したときの和の最大値を返す．
* また要素が選ばれたかどうかを sel[0..n)[0..n) に格納する．
*
* 利用：【最小費用流（負コスト可，DAG）】
*/
ll allocation_problem(const vvl& a, int k, vvb* sel = nullptr) {
	// verify : https://atcoder.jp/contests/practice2/tasks/practice2_e

	int n = sz(a);

	int L = n * n, R = L + n, ST = R + n, GL = ST + 1;
	Negative_mcf_graph_DAG g(GL + 1);

	// 始点から各行 i へ，容量 k，コスト 0 の辺を張る．
	rep(i, n) g.add_edge(ST, L + i, k, 0);

	rep(i, n) rep(j, n) {
		// 各行 i から各要素 (i,j) へ，容量 1，コスト -a[i][j] の辺を張る．
		g.add_edge(L + i, i * n + j, 1, -a[i][j]);

		// 各要素 (i,j) から各列 j へ，容量 1，コスト 0 の辺を張る．
		g.add_edge(i * n + j, R + j, 1, 0);
	}

	// 各列 j から終点へ，容量 k，コスト 0 の辺を張る．
	rep(j, n) g.add_edge(R + j, GL, k, 0);

	// 始点から終点へ，容量 ∞，コスト 0 の辺を張り，要素を選択しないことを許す．
	g.add_edge(ST, GL, INFL, 0);

	auto cost = g.flow(ST, GL, n * k).second;

	if (sel != nullptr) {
		*sel = vvb(n, vb(n));
		repe(e, g.g_pos.edges()) {
			// 行 i から要素 (i,j) への辺にフローが流れていれば (i,j) を選択する．
			if (L <= e.from && e.from < R && e.flow == 1) {
				(*sel)[e.to / n][e.to % n] = true;
			}
		}
	}

	return -cost;
}


