#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ マッチング ■■■■■



//【二部グラフの最大マッチング】
/*
* 二部グラフ (S, T) の最大マッチングを求める．
*
* Bipartite_matching(n, m) : O(n + m)
*	S, T の要素数を n, m で初期化する．
*
* add_edge(s, t) : O(1)
*	s∈S と t∈T の間に辺を張る．
*
* flow() : O((n + m) |E|)
*	フローを流し計算を行う．
*	戻り値：最大マッチングの大きさ
*
* maximum_matching(es) : O((n + m) |E|)
*	最大マッチングの例を具体的に求め es に格納する．
*	flow() の後に呼び出すこと．
* 	es : 最大マッチングに含まれる辺 {s, t} ∈ S×T のリスト
*
* minimum_edge_covering : O((n + m) |E|)
*	最小辺被覆の例を具体的に求め es に格納する．
*	flow() の後に呼び出すこと．
* 	es : 最小辺被覆に含まれる辺 {s, t} ∈ S×T のリスト
*/
struct Bipartite_matching {
	int n, m;
	mf_graph<int> g;
	int ST, GL;

	// |S|, |T| を渡して初期化する．
	Bipartite_matching(int n_, int m_) : n(n_), m(m_) {
		g = mf_graph<int>(n + m + 2);

		// スタートとゴールおよびそれらとの間の辺を先に作っておく．
		ST = n + m;
		GL = n + m + 1;
		rep(i, n) {
			g.add_edge(ST, i, 1);
		}
		rep(j, m) {
			g.add_edge(j + n, GL, 1);
		}
	}

	// s∈S と t∈T の間に辺を張る． 
	void add_edge(int s, int t) {
		g.add_edge(s, t + n, 1);
	}

	// 計算を実行し，最大マッチングの大きさを返す．
	int flow() {
		return g.flow(ST, GL);
	}

	// 最大マッチングの例を具体的に求める．
	void maximum_matching(vector<pii>& es) {
		es.clear();
		repe(e, g.edges()) {
			// フローが流れている S, T 間の辺がマッチングに対応する．
			if (e.flow == 1 && e.from != ST && e.to != GL) {
				es.push_back({ e.from, e.to - n });
			}
		}
	}

	// 最小辺被覆の例を具体的に求める．
	void minimum_edge_covering(vector<pii>& es) {
		// マッチングに含まれない S, T の頂点
		unordered_set<int> iso_s, iso_t;
		rep(i, n) {
			if (g.get_edge(i).flow == 0) {
				iso_s.insert(i);
			}
		}
		rep(j, m) {
			if (g.get_edge(j + n).flow == 0) {
				iso_t.insert(j + n);
			}
		}

		es.clear();
		repe(e, g.edges()) {
			// マッチングに含まれる S, T の頂点はそのまま結ぶ．
			if (e.flow == 1 && e.from != ST && e.to != GL) {
				es.push_back({ e.from, e.to - n });
			}
			// マッチングに含まれない S の頂点は，適当な T の頂点と結んでおく．
			else if (iso_s.count(e.from)) {
				es.push_back({ e.from, e.to - n });
				iso_s.erase(e.from);
			}
			// マッチングに含まれない T の頂点は，適当な S の頂点と結んでおく．
			else if (iso_t.count(e.to)) {
				es.push_back({ e.from, e.to - n });
				iso_t.erase(e.to);
			}
		}
	}
};


//【二部グラフの最小コスト最大マッチング】
/*
* 二部グラフ (S, T) の最小コスト最大マッチングを求める．
*
* minimum_cost_bipartite_matching(m, n) : O(m + n)
*	S, T の要素数を m, n で初期化する．
*
* add_edge(s, t, c) : O(1)
*	s∈S と t∈T の間にコスト c の辺を張る．
*
* flow() : O(cost (|V| + |E|) log|V|)
*	フローを流し計算を行う．
*	戻り値 : 最大マッチングの大きさと，そのうちの最小コストの組
*
* minimul_cost_maximum_matching(es) : O((m + n) |E|)
*	最大マッチングの例を具体的に求め es に格納する．
*	flow() の後に呼び出すこと．
*	es : 最大マッチングに含まれる辺 (s, t) ∈ S×T のリスト
*
*（最小費用流問題）
*/
struct minimum_cost_bipartite_matching {
	int m, n;
	mcf_graph<int, ll> g;
	int ST, GL;
	
	// |S|, |T| を渡して初期化する．
	minimum_cost_bipartite_matching(int m_, int n_) : m(m_), n(n_) {
		g = mcf_graph<int, ll>(m + n + 2);

		// スタートとゴールおよびそれらとの間の辺を先に作っておく．
		ST = m + n;
		GL = m + n + 1;
		rep(i, m) {
			g.add_edge(ST, i, 1, 0);
		}
		rep(i, n) {
			g.add_edge(m + i, GL, 1, 0);
		}
	}

	// s∈S と t∈T の間にコスト c の辺を張る． 
	void add_edge(int s, int t, ll c) {
		g.add_edge(s, m + t, 1, c);
	}

	// 最大マッチングの大きさと，そのうちの最小コストを返す．
	pair<int, ll> flow() {
		return g.flow(ST, GL);
	}

	// 実現例を具体的に求める．O(|E|)
	vector<pii>& minimul_cost_maximum_matching(vector<pii>& es) {
		es.clear();
		repe(e, g.edges()) {
			// フローが流れている S, T 間の辺がマッチングに対応する．
			if (e.flow == 1 && e.from != ST && e.to != GL) {
				es.push_back({ e.from, e.to - m });
			}
		}
	}
};


//【二部グラフのコスト最小弾性マッチング】O(|S| |T|)
/*
* コスト付き完全二部グラフ (S, T) のコスト最小弾性マッチングのコストを返す．
* またそのようなマッチングを昇順に match に格納する．
*
* c[i][j] : S[i] と T[j] の間にある辺のコスト．
* (i, j) ∈ res : S[i] と T[j] がマッチングしていることを表す．
*
*（二次元 DP）
*/
ll minimum_cost_elastic_matching(vvl& c, vector<pii>* match = nullptr) {
	// n = |S|, m = |T|
	int n = sz(c);
	int m = sz(c[0]);

	// dp[i][j] : S[0..i) と T[0..j) の弾性マッチングの最小コスト
	vvl dp(n + 1LL, vl(m + 1LL, INFL));
	dp[0][0] = 0;

	// 貰う DP
	rep(i, n) {
		rep(j, m) {
			// S[i] と T[j] は繋ぐしかないのでその分のコストは絶対必要になる．
			// その他に
			//		S[i] に T[j - 1] が繋がる
			//		T[j] に S[i - 1] が繋がる
			//		S[i], T[j] 共に他に繋がる頂点はなし
			// の 3 通りの場合が考えられるので，そのうち最小のものを選ぶ．
			dp[i + 1LL][j + 1LL]
				= min({ dp[i + 1LL][j], dp[i][j + 1LL], dp[i][j] }) + c[i][j];
		}
	}

	// DP 復元
	if (match != nullptr) {
		match->clear();
		int i = n - 1, j = m - 1;
		while (i >= 0 && j >= 0) {
			match->push_back({ i, j });

			auto dp_min = min({ dp[i + 1LL][j], dp[i][j + 1LL], dp[i][j] });

			if (dp_min == dp[i + 1LL][j]) {
				j--;
			}
			else if (dp_min == dp[i][j + 1LL]) {
				i--;
			}
			else {
				i--;
				j--;
			}
		}

		reverse(all(*match));
	}

	return dp[n][m];
}


//【二部グラフの完全マッチングの数え上げ】O(2^|S| |S|)
/*
* |S| = |T| なる二部グラフ (S, T) の完全マッチングの個数を返す．
*
* e[i][j] : S[i] と T[j] の間に辺が存在するなら true, なければ false．
*
*（bit DP）
*/
mint count_perfect_matching(vvb& e) {
	// 参考 : https://kyopro-friends.hatenablog.com/entry/2019/01/12/231035

	int n = sz(e);

	// dp[set] : set ⊂ T と，[0..|set|) ⊂ S との完全マッチングの個数
	vm dp(1LL << n);
	vb seen(1LL << n);

	// メモ化再帰用の関数
	function<mint(int)> rf = [&](int set) {
		// set が空集合の場合は完全マッチングは 1 通り．
		if (!set) {
			return mint(1);
		}

		// dp[set] が計算済ならその値を返す．
		if (seen[set]) return dp[set];
		seen[set] = true;

		// S の |set| 番目の頂点 i を得る．
		int i = popcount(set) - 1;

		// 頂点 i が j ∈ set とマッチしている場合についてループ．
		rep(j, n) {
			// (i, j) に辺がなかったり，j が set に属していなければ何もしない．
			if (!e[i][j] || !(set & (1 << j))) continue;
			
			// i と j がマッチしている場合の数を加算する．
			dp[set] += rf(set - (1 << j));
		}

		return dp[set];
	};

	// set = T として再帰関数に投げる．
	return rf((1 << n) - 1);
}


//【最小コスト完全マッチング】O(2^|V| |V|)
/*
* コスト付きグラフ g の隣接行列 adj を元に，g の最小コスト完全マッチングのコストを返す．
*
*（bit DP）
*/
ll minimum_cost_matching(const vvl& adj) {
	int n = sz(adj);
	if (n % 2 == 1) {
		return -INFL;
	}

	// dp[set] : set に含まれる頂点で作れる完全マッチングの最小コスト
	vl dp(int(1 << n), INF);
	vb seen(int(1 << n));
	dp[0] = 0;
	seen[0] = true;

	// set : 考慮すべき頂点の集合
	function<ll(int)> rf = [&](int set) {
		// 計算済ならその値を返す．
		if (seen[set]) {
			return dp[set];
		}
		seen[set] = true;

		// s : set で最も番号の小さい頂点
		int s = lsb(set);

		// t : s とペアになる set の頂点
		repi(t, s + 1, n - 1) {
			if (set & (1 << t)) {
				chmin(dp[set], rf(set - (1 << s) - (1 << t)) + adj[s][t]);
			}
		}

		return dp[set];
	};

	// 全頂点に対して最小コストを計算する．
	return rf((1 << n) - 1);
}


