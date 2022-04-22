#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 二部マッチング ■■■■■


//【二部グラフの最大マッチング，最小辺被覆，最小点被覆】
/*
* 二部グラフ (S, T) の最大マッチングなどを求める．
*
* Bipartite_matching(int n, int m) : O(|V|)
*	S, T の要素数を n, m で初期化する．
*
* add_edge(int s, int t) : O(1)
*	s∈S と t∈T の間に辺を張る．
*
* int flow() : O( min(|V|^(2/3) (|V| + |E|), (|V| + |E|)^(3/2)) )
*	フローを流し計算を行う．
*	戻り値：最大マッチングの大きさ
*
* maximum_matching(vector<pii>& es) : O(|E|)
*	最大マッチングの例を具体的に求め es に格納する．
*	flow() の後に呼び出すこと．
* 	es : 最大マッチングに含まれる辺 {s, t} ∈ S×T のリスト
*
* minimum_edge_covering(vector<pii>& es) : O(|V| + |E|)
*	最小辺被覆の例を具体的に求め es に格納する．
*	es が最小辺被覆であるとは，任意の頂点がある e∈es の端点として現れることをいう．
*	flow() の後に呼び出すこと．
* 	es : 最小辺被覆に含まれる辺 {s, t} ∈ S×T のリスト
*
* minimum_vertex_covering(vvi& vs) : O(|V| + |E|)
*	最小点被覆の例を具体的に求め，S の頂点を vs[0], T の頂点を vs[1] に格納する．
*	vs が最小点被覆であるとは，任意の辺がある v∈vs を端点にもつことをいう．
*	flow() の後に呼び出すこと．
*
*（最大フロー問題）
*/
struct Bipartite_matching {
	// 参考 : https://qiita.com/drken/items/e805e3f514acceb87602
	// 参考 : https://qiita.com/drken/items/7f98315b56c95a6181a4

	int n, m;
	mf_graph<int> g;
	int ST, GL;

	// |S|, |T| を渡して初期化する．
	Bipartite_matching(int n_, int m_) : n(n_), m(m_) {
		g = mf_graph<int>(n + m + 2);

		// スタートとゴールおよびそれらとの間の辺を先に作っておく．
		ST = n + m;
		GL = n + m + 1;
		rep(i, n) g.add_edge(ST, i, 1);
		rep(j, m) g.add_edge(j + n, GL, 1);
	}

	// s∈S と t∈T の間に辺を張る． 
	void add_edge(int s, int t) { g.add_edge(s, t + n, 1); }

	// 計算を実行し，最大マッチングの大きさを返す．
	int flow() { return g.flow(ST, GL); }

	// 最大マッチングの例を具体的に求める．
	void maximum_matching(vector<pii>& es) {
		// verify : https://judge.yosupo.jp/problem/bipartitematching

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
		es.clear();

		// マッチングに含まれない S, T の頂点の集合
		unordered_set<int> iso_s, iso_t;
		rep(i, n) {
			if (g.get_edge(i).flow == 0) iso_s.insert(i);
		}
		rep(j, m) {
			if (g.get_edge(j + n).flow == 0) iso_t.insert(j + n);
		}

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

	// 最小点被覆の例を具体的に求める．
	void minimum_vertex_covering(vvi& vs) {
		// verify : https://judge.yosupo.jp/problem/assignment

		vs = vvi(2);

		// ar[v] : g の残余グラフで ST から v に到達可能か
		vb ar = g.min_cut(ST);

		// 残余グラフで ST から到達不可能な S の頂点を選ぶ．
		rep(i, n) {
			if (!ar[i]) vs[0].push_back(i);
		}

		// 残余グラフで ST から到達可能な T の頂点を選ぶ．
		rep(j, m) {
			if (ar[n + j]) vs[1].push_back(j);
		}
	}
};


//【二部グラフの最小コスト最大マッチング】
/*
* コスト付き二部グラフ (S, T) の最小コスト最大マッチングを求める．
*
* Minimum_cost_bipartite_matching(int n, int m) : O(|V|)
*	S, T の要素数を n, m で初期化する．
*
* add_edge(int s, int t, ll c) : O(1)
*	s∈S と t∈T の間にコスト c の辺を張る．
*
* pil flow() : O(cost (|V| + |E|) log|V|)
*	フローを流し計算を行う．
*	戻り値 : 最大マッチングの大きさと，そのうちの最小コストの組
*
* minimul_cost_maximum_matching(vector<pii>& es) : O(|E|)
*	最小コスト最大マッチングの例を具体的に求め es に格納する．
*	flow() の後に呼び出すこと．
*	es : 最小コスト最大マッチングに含まれる辺 (s, t) ∈ S×T のリスト
*
*（最小費用流問題）
*/
struct Minimum_cost_bipartite_matching {
	int n, m;
	mcf_graph<int, ll> g;
	int ST, GL;

	// |S|, |T| を渡して初期化する．
	Minimum_cost_bipartite_matching(int n_, int m_) : n(n_), m(m_) {
		g = mcf_graph<int, ll>(n + m + 2);

		// スタートとゴールおよびそれらとの間の辺を先に作っておく．
		ST = n + m;
		GL = n + m + 1;
		rep(i, n) g.add_edge(ST, i, 1, 0);
		rep(i, m) g.add_edge(n + i, GL, 1, 0);
	}

	// s∈S と t∈T の間にコスト c の辺を張る． 
	void add_edge(int s, int t, ll c) { g.add_edge(s, n + t, 1, c); }

	// 最大マッチングの大きさと，そのうちの最小コストを返す．
	pil flow() { return g.flow(ST, GL); }

	// 実現例を具体的に求める．
	void minimul_cost_maximum_matching(vector<pii>& es) {
		es.clear();
		repe(e, g.edges()) {
			// フローが流れている S, T 間の辺がマッチングに対応する．
			if (e.flow == 1 && e.from != ST && e.to != GL) {
				es.push_back({ e.from, e.to - n });
			}
		}
	}
};


//【二部グラフの最大コスト最大マッチング】
/*
* 十分大きな定数を INF として，各コストを INF から引いたものに置き換えたグラフを作り，
* 【二部グラフの最小コスト最大マッチング】を利用する．
* 最後に (マッチングの大きさ) * INF から求まった最小コストを引けば良い．
* 
* verify : https://atcoder.jp/contests/abc247/tasks/abc247_g
*/


//【完全二部グラフの最小コスト完全マッチング／ハンガリアン法】O(|S|^3 ?)
/*
* S[i], T[j] 間のコストが c[i][j] で与えられるコスト付き完全二部グラフ (S, T) について，
* 完全マッチングのうち最小のコストをもつもののコストを返す．
* また S[i], T[j] がマッチングを成すことを p[i] = j として格納する．
*/
ll minimum_cost_perfect_matching(const vvl& c_, vi& p) {
	// 参考 : http://www.bunkyo.ac.jp/~nemoto/lecture/network/2010/matching1_2010.pdf
	// verify : https://judge.yosupo.jp/problem/assignment

	vvl c = c_;
	int n = sz(c);
	p.resize(n);

	while (true) {
		dumpel(c);

		// 各行について行の最小値を引く．
		vl y_min(n, INFL);
		rep(i, n) {
			ll x_min = *min_element(all(c[i]));
			rep(j, n) {
				c[i][j] -= x_min;
				chmin(y_min[j], c[i][j]);
			}
		}

		// 各列について列の最小値を引く．
		rep(i, n) rep(j, n) c[i][j] -= y_min[j];

		// 各行各列に存在する 0 部分の最大マッチング（大きさ k）を求める．
		Bipartite_matching bm(n, n);
		rep(i, n) rep(j, n) if (c[i][j] == 0) bm.add_edge(i, j);
		int k = bm.flow();

		// 完全マッチングが得られたら終了．
		if (k == n) {
			vector<pii> es;
			bm.maximum_matching(es);

			ll res = 0;
			repe(e, es) {
				p[e.first] = e.second;
				res += c_[e.first][e.second];
			}

			return res;
		}

		// 最小点被覆を得る（k 本の縦線または横線で全ての 0 を被覆することに対応する．）
		vvi vs;
		bm.minimum_vertex_covering(vs);

		// 直線が存在しない行および列を得る．
		vvi vs_cp(2);
		vi vs_all(n); iota(all(vs_all), 0);
		rep(t, 2) set_difference(all(vs_all), all(vs[t]), inserter(vs_cp[t], vs_cp[t].end()));

		// 直線で被覆されていない部分の最小値 v_min を求める．
		ll v_min = INFL;
		repe(x, vs_cp[0]) repe(y, vs_cp[1]) chmin(v_min, c[x][y]);

		// 直線で被覆されていない部分から v_min を引く．
		repe(x, vs_cp[0]) repe(y, vs_cp[1]) c[x][y] -= v_min;

		// 両方向の直線で被覆されている部分に v_min を加える．
		repe(x, vs[0]) repe(y, vs[1]) c[x][y] += v_min;
	}

	return -1;
}


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
	// verify : https://algo-method.com/tasks/316

	// n = |S|, m = |T|
	int n = sz(c), m = sz(c[0]);

	// dp[i][j] : S[0..i) と T[0..j) の弾性マッチングの最小コスト
	vvl dp(n + 1, vl(m + 1, INFL));
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
			dp[i + 1][j + 1]
				= min({ dp[i + 1][j], dp[i][j + 1], dp[i][j] }) + c[i][j];
		}
	}

	// DP 復元
	if (match != nullptr) {
		match->clear();
		int i = n - 1, j = m - 1;
		while (i >= 0 && j >= 0) {
			match->push_back({ i, j });

			auto dp_min = min({ dp[i + 1][j], dp[i][j + 1], dp[i][j] });

			if (dp_min == dp[i + 1][j]) {
				j--;
			}
			else if (dp_min == dp[i][j + 1]) {
				i--;
			}
			else {
				i--; j--;
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
	// verify : https://atcoder.jp/contests/dp/tasks/dp_o

	int n = sz(e);

	// dp[set] : set ⊂ T と，[0..|set|) ⊂ S との完全マッチングの個数
	vm dp(1LL << n);
	vb seen(1LL << n);

	// メモ化再帰用の関数
	function<mint(int)> rf = [&](int set) {
		// set が空集合の場合は完全マッチングは 1 通り．
		if (!set) return mint(1);

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


//【ホールの結婚定理】
/*
* 二部グラフ (S, T) の最大マッチング M ⊂ E の大きさを求めたい．
* 
* 各 s ∈ S について，f(s) ⊂ T を
*		f(s) = {t ∈ T | (s, t) ∈ E}
*		（s と辺で結ばれた T の頂点集合）
* と定める．さらに各 X ⊂ S について f(X) ⊂ T を
*		f(X) = ∪s∈X f(s)
*		（X の少なくとも 1 つの頂点と辺で結ばれた T の頂点集合）
* と定める．
*
* このとき
*		|S| - |M| = max(|X| - |f(X)| | X ⊂ S)
*		（マッチングに入れなかった S の元の個数の最小値は，
*		  X が f(X) からはみ出る大きさの最大値に等しい．）
* が成り立つ．
* 
* また各 Y ⊂ T について
*		g(Y) = (f(X) ⊂ Y となる X についての |X| の最大値)
* と定めれば，
*		|S| - |M| = max(g(Y) - |Y| | Y ⊂ T)
* が成り立つとも言い換えられる．
* 
* 無条件では
*		|2^S| = 2^|S|, |2^T| = 2^|T|
* なので計算量的に役に立たないが，|f(2^S)| が十分小さく，
* 各 Y ⊂ f(2^S) に対する g(Y) が効率よく計算できるならば役に立つ．
* 
* verify：https://atcoder.jp/contests/arc076/tasks/arc076_d
*/


