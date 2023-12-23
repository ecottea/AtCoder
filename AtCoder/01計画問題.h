#pragma once
#include "header.h"
#include "分析(グラフ).h"
#include "フロー.h"
// ■■■■■ 0-1 計画問題 ■■■■■


//【二次 0-1 計画問題】
/*
* Binary_programming_BB(int n) : O(1)
*	n 個の論理変数 X[0..n) で初期化する．
*
* add_cost(ll c) : O(1)
*	固定コスト c がかかるようにする（c < 0 なら利益と解釈する）
*
* add_cost0(int i, ll c) : O(1)
*	X[i] = 0 のときコスト c がかかるようにする（c < 0 なら利益と解釈する）
*
* add_cost1(int i, ll c) : O(1)
*	X[i] = 1 のときコスト c がかかるようにする（c < 0 なら利益と解釈する）
*
* add_cost01(int i, int j, ll c) : O(1)
*	X[i] = 0 かつ X[j] = 1 のとき非負コスト c がかかるようにする．
*
* add_profit00(int i, int j, ll p) : O(1)
*	X[i] = 0 かつ X[j] = 0 のとき非負利益 p を得られるようにする．
*
* add_profit11(int i, int j, ll p) : O(1)
*	X[i] = 1 かつ X[j] = 1 のとき非負利益 p を得られるようにする．
*
* ll solve() : O(n^2 m)（m : 条件の数）
*	適切に X[0..n) を定めた場合の最小コストを返す．
*/
struct Binary_programming_BB {
	// 参考 : https://kanpurin.hatenablog.com/entry/moyasu-umeru

	// n : 論理変数の数
	int n;

	// pre_cost : 前払いしているコスト
	ll pre_cost = 0;

	// cost0[i] : X[i] = 0 のときにかかる非負コスト
	// cost1[i] : X[i] = 1 のときにかかる非負コスト
	vl cost0, cost1;

	// cost01[i][j] : X[i] = 0 かつ X[j] = 1 のときにかかる非負コスト
	vvl cost01;

	// n 変数で初期化
	Binary_programming_BB(int n_) : n(n_), cost0(n), cost1(n), cost01(n, vl(n)) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_an
	}

	// 固定コスト c がかかるようにする（c < 0 なら利益と解釈する）
	void add_cost(ll c) {
		// verify : https://mojacoder.app/users/_kanpurin_/problems/project_selection_problem003

		pre_cost += c;
	}

	// X[i] = 0 のときコスト c がかかるようにする（c < 0 なら利益と解釈する）
	void add_cost0(int i, ll c) {
		// verify : https://mojacoder.app/users/_kanpurin_/problems/project_selection_problem003

		Assert(0 <= i && i < n);

		// コスト |c| がかかる場合
		if (c >= 0) {
			cost0[i] += c;
		}
		// 利益 |c| が得られる場合
		else {
			// 利益 |c| を前借りしておき，X[i] = 1 のときコスト |c| がかかると言い換えればよい．
			pre_cost += c;
			cost1[i] -= c;
		}
	}

	// X[i] = 1 のときコスト c がかかるようにする（c < 0 なら利益と解釈する）
	void add_cost1(int i, ll c) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_an

		Assert(0 <= i && i < n);

		// コスト |c| がかかる場合
		if (c >= 0) {
			cost1[i] += c;
		}
		// 利益 |c| が得られる場合
		else {
			// 利益 |c| を前借りしておき，X[i] = 0 のときコスト |c| がかかると言い換えればよい．
			pre_cost += c;
			cost0[i] -= c;
		}
	}

	// X[i] = 0 かつ X[j] = 1 のとき非負コスト c がかかるようにする．
	void add_cost01(int i, int j, ll c) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_an

		Assert(0 <= i && i < n);
		Assert(0 <= j && j < n);
		Assert(c >= 0);

		cost01[i][j] += c;
	}

	// X[i] = 0 かつ X[j] = 0 のとき非負利益 p を得られるようにする．
	void add_profit00(int i, int j, ll p) {
		// verify : https://yukicoder.me/problems/9212

		Assert(0 <= i && i < n);
		Assert(0 <= j && j < n);
		Assert(p >= 0);

		// 利益 p を前借りしておき，
		//		X[i] = 1 のときコスト p がかかる
		//		X[i] = 0 かつ X[j] = 1 のときコスト p がかかる
		// と言い換えればよい．
		pre_cost -= p;
		cost1[i] += p;
		cost01[i][j] += p;
	}

	// X[i] = 1 かつ X[j] = 1 のとき非負利益 p を得られるようにする．
	void add_profit11(int i, int j, ll p) {
		// verify : https://yukicoder.me/problems/9212

		Assert(0 <= i && i < n);
		Assert(0 <= j && j < n);
		Assert(p >= 0);

		// 利益 p を前借りしておき，
		//		X[j] = 0 のときコスト p がかかる
		//		X[i] = 0 かつ X[j] = 1 のときコスト p がかかる
		// と言い換えればよい．
		pre_cost -= p;
		cost0[j] += p;
		cost01[i][j] += p;
	}

	// 適切に X[0..n) を定めた場合の最小コストを返す．
	ll solve() {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_an

		// ST : 始点（恒等的に 0），GL : 終点（恒等的に 1）
		// g の残余ネットワークで ST から到達可能な頂点は 0，それ以外は 1 にする．
		int ST = n, GL = n + 1;
		mf_graph<ll> g(n + 2);

		rep(i, n) {
			// X[i] = 0 にコスト c0， X[i] = 1 にコスト c1 がかかるとき
			//	c = min(c0, c1) として確定でコスト c がかかるとし，
			//		c0 > c1 なら X[i] = 0 にコスト c0 - c がかかる
			//		c0 < c1 なら X[i] = 1 にコスト c1 - c がかかる
			// としてよい．
			ll c = min(cost0[i], cost1[i]);
			pre_cost += c;

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

		return pre_cost + g.flow(ST, GL);
	}
};


//【高次の利益】
/*
* X[i] = 0[1] かつ X[j] = 0[1] かつ X[k] = 0[1] のとき非負利益 p が得られることは，
* 利益 p を前借りした上で変数 Y を追加し，
*	Y = 1[0] のときコスト p
*	Y = 0[1] かつ X[i] = 1[0] のときコスト ∞
*	Y = 0[1] かつ X[j] = 1[0] のときコスト ∞
*	Y = 0[1] かつ X[k] = 1[0] のときコスト ∞
* とすることで二次の項のみで表現できる．
* 
* verify : https://atcoder.jp/contests/abc326/tasks/abc326_g
*/


//【二部グラフでの add_cost11】
/*
* 二部グラフ (S, T) の頂点 s∈S, t∈T に対して add_cost11(s, t, c) をしたい場合，
* S 側の 0-1 の役割を反転させることができれば add_cost01(s, t, c) で代用できる．
* 
* verify : https://mojacoder.app/users/_kanpurin_/problems/project_selection_problem002
*/


//【多値変数】
/*
* X[i] ∈ [0..K) の場合，新たに論理変数を Y[i][k] = Boole[X[i] ≦ k] と定めれば，
* cost11(s, t) に Monge 性があれば上位要素メビウス変換を用いて【二次 0-1 計画問題】に帰着できる．
* 
* 参考 : https://noshi91.hatenablog.com/entry/2021/06/29/044225
* verify : https://mojacoder.app/users/_kanpurin_/problems/project_selection_problem003
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


//【集合被覆問題】O(2^n m / 64)
/*
* 与えられた [0..m) の部分集合族 s[0..n) について，
* s[set] が [0..m) を被覆するような |set| の最小値を返す（不可能なら INF）
*
* 制約：m ≦ M
*
*（bit 全探索）
*/
template <int M>
int set_covering_problem(const vector<bitset<M>>& s, int m) {
	int n = sz(s);

	bitset<M> base;
	rep(j, m) base[j] = 1;

	int res = INF;

	repb(set, n) {
		bitset<M> b;
		rep(i, n) if (get(set, i)) b |= s[i];

		if (b == base) chmin(res, popcount(set));
	}

	return res;
}


//【集合被覆問題】O(2^m n / 64)
/*
* 与えられた [0..m) の部分集合族 s[0..n) について，
* s[set] が [0..m) を被覆するような |set| の最小値を返す（不可能なら INF）
*
*（インライン bit DP）
*/
int set_covering_problem(const vi& s, int m) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_w

	int n = sz(s);

	vi dp(1LL << m, INF);
	dp[0] = 0;

	// インライン配る bit DP
	rep(i, n) repir(set, (1 << m) - 1, 0) {
		chmin(dp[set | s[i]], dp[set] + 1);
	}

	return dp[(1 << m) - 1];
}


