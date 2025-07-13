#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "変換(グラフ).h"
#include "最短路.h"
#include "bit全探索.h"
#include "数え上げ(グラフ).h"
#include "マッチング(一般).h"
// ■■■■■ グラフ上の最適化問題 ■■■■■


//【最大独立集合問題】O(2^(n/2) n)
/*
* 無向グラフ g の最大独立集合の 1 つを vs に格納し，その大きさを返す．
* S ⊂ V が独立集合であるとは，S の任意の 2 点を結ぶ辺が E に属さないことをいう．
*/
int maximum_independent_set(const Graph& g, vi* vs = nullptr) {
	// verify : https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_g

	int n = sz(g);

	// 前半 V1 と後半 V2 の頂点数
	int n1 = n / 2, n2 = n - n1;


	// is_ind1[set1] : set1 ⊂ V1 が独立集合か
	vb is_ind1(1LL << n1, true);

	// 辺の両端からなる 2 点集合 {s, t} ⊂ V1 は独立集合ではない．
	rep(s, n1) repe(t, g[s]) {
		if (t >= n1) continue;

		int set = (1 << s) + (1 << t);
		is_ind1[set] = false;
	}

	// 独立集合でない集合を部分集合にもつ集合は独立集合ではない．
	repb(set1, n1) rep(i, n1) {
		if (set1 & (1 << i)) {
			int sub1 = set1 - (1 << i);
			is_ind1[set1] = is_ind1[set1] && is_ind1[sub1];
		}
	}


	// no_edge[set1] : set1 ⊂ V1 との間に辺をもたない V2 の頂点集合
	vi no_edge(1LL << n1);

	// 空集合 ⊂ V1 の相手は明らかに全体集合 V2
	no_edge[0] = (1 << n2) - 1;

	// 1 点集合 {s} ⊂ V1 の相手は s と辺で結ばれていない V2 の点集合
	rep(s, n1) {
		no_edge[1LL << s] = (1 << n2) - 1;
		repe(t, g[s]) {
			if (t < n1) continue;

			no_edge[1LL << s] -= (1 << (t - n1));
		}
	}

	// 2 点以上の集合 set1 ⊂ V1 については 1 点集合の相手との共通部分を考える．
	repb(set1, n1) rep(i, n1) {
		if (set1 & (1LL << i)) {
			int sub1 = set1 - (1LL << i);
			no_edge[set1] = no_edge[1LL << i] & no_edge[sub1];
			break;
		}
	}


	// max_ind2[set2] : set2 ⊂ V2 の最大独立集合の 1 つ
	vi max_ind2(1LL << n2);

	// 1 点集合 {s} ⊂ V2 の最大独立集合は {s} ⊂ V2
	rep(i, n2) {
		int set2 = 1 << i;
		max_ind2[set2] = set2;
	}

	// 辺の両端からなる 2 点集合 {s, t} ⊂ V2 は独立集合ではない．
	rep(i, n2) repe(t, g[n1 + i]) {
		if (t < n1) continue;

		int set2 = (1 << i) + (1 << (t - n1));
		max_ind2[set2] = 1 << i;
	}

	// 3 点以上の集合 set2 ⊂ V2 については，それが独立集合ならそれ自身，
	// さもなくば 1 つ抜き集合の最大独立集合のうちの最大のものを選択する．
	repb(set2, n2) {
		int pc = popcount(set2);
		if (pc <= 2) {
			// どの辺の両端ともならない 2 点集合 {s, t} ⊂ V2 は独立集合である．
			if (pc == 2 && max_ind2[set2] == 0) max_ind2[set2] = set2;

			continue;
		}

		// ind_flag : set2 自身が独立集合か
		bool ind_flag = true;

		rep(i, n2) {
			if (set2 & (1 << i)) {
				int sub2 = set2 - (1 << i);
				if (popcount(max_ind2[set2]) < popcount(max_ind2[sub2])) {
					max_ind2[set2] = max_ind2[sub2];
				}
				if (max_ind2[sub2] != sub2) ind_flag = false;
			}
		}

		if (ind_flag) max_ind2[set2] = set2;
	}


	// 最大独立集合の大きさ
	int pc_max = 0;

	// 最大独立集合の前半部分と後半部分
	int set1_max = 0, set2_max = 0;

	repb(set1, n1) {
		// set1 ⊂ V1 が独立集合でないなら不適
		if (!is_ind1[set1]) continue;

		// set1 ⊂ V1 と辺で結ばれていない頂点集合 set2 ⊂ V2 を得る
		int set2 = no_edge[set1];

		// set2 ⊂ V2 をその最大独立集合に置き換える．
		set2 = max_ind2[set2];

		int pc = popcount(set1) + popcount(set2);
		if (chmax(pc_max, pc)) {
			set1_max = set1;
			set2_max = set2;
		}
	}

	// 最大独立集合の構成
	if (vs != nullptr) {
		vs->clear();
		rep(i, n1) if (set1_max & (1 << i)) vs->push_back(i);
		rep(i, n2) if (set2_max & (1 << i)) vs->push_back(n1 + i);
	}

	return pc_max;
}


//【最大クリーク問題】O(2^(n/2) n)
/*
* 無向グラフ g の最大クリークの 1 つを vs に格納し，その大きさを返す．
* S ⊂ V がクリークであるとは，S の任意の 2 点を結ぶ辺が E に属することをいう．
*
* 利用：【補グラフ】,【最大独立集合問題】
*/
int maximum_clique(const Graph& g, vi* vs = nullptr) {
	// verify : https://atcoder.jp/contests/abc002/tasks/abc002_4

	//【方法】
	// 最大クリーク問題は，補グラフについての最大独立集合問題と等価である．

	Graph gc = complement_graph(g);

	return maximum_independent_set(gc, vs);
}


//【彩色数】O(2^n n)
/*
* 無向グラフ g の彩色数を返す．彩色数とは，独立集合への分割の最小個数である．
*/
int chromatic_number(const Graph& g) {
	// 参考 : https://drken1215.hatenablog.com/entry/2019/01/16/030000
	// verify : https://judge.yosupo.jp/problem/chromatic_number

	//【方法】
	// 判定問題
	//		(P): k 個の独立集合へ分割可能か ⇔ k-彩色可能か
	// を考え，(P) を満たす最小の k を線形探索する．
	//
	// (P) の代わりに
	//		(P2): k 個の独立集合で被覆可能か
	// を考えても，可能性は変わらない．
	//
	// (P2) の代わりに
	//		(P3): k 個の独立集合で被覆する方法は何通りか
	// を考え，この答えが 0 か否かを見ることにする．そこで
	//		g[set] : set を k 個の独立集合で被覆する方法の数
	// とおく．（分割 → 被覆としたのでこれは k-彩色の場合の数ではないことに注意．）
	//
	// g の下位集合でのゼータ変換を
	//		f[set] = Σ_(sub ⊂ set) g[sub]
	// とおく．f は
	//		f[set] = set から k 個の独立集合を選ぶ方法の数
	// と解釈できるので，
	//		f[set] = (set の独立集合の数)^k
	// と表される．
	//
	// 以上を逆に辿ると，以下のアルゴリズムが得られる：
	//	k について昇順に
	//		各 set ⊂ V について set の独立集合の数を求める．
	//		→ f[set] = (set の独立集合の数)^k を求める．
	//		→ f[set] に下位集合メビウス変換(最大元) を行い g[V] を求める．
	//	を行い，初めて g[V] != 0 となった k を返せば良い．
	//
	// 実際には独立集合の数ではなく，頂点にランダムな重みを割り当て，
	// 独立集合のスコアを重みの積と定めてのスコア和を考えれば失敗する確率は十分低くなる．

	int n = sz(g);
	if (n == 0) return 0;

	mt19937_64 mt((int)time(NULL));
	uniform_int_distribution<int> rnd(0, 998244352);

	// w[s] : 頂点 s に割り当てるランダムな重み
	vm w(n);
	rep(s, n) w[s] = rnd(mt);

	// ind[set] : set の部分集合のうち，独立集合をなすもののスコア和
	vm ind(1LL << n);
	ind[0] = 1;

	repi(set, 1, (1 << n) - 1) {
		// s : set に含まれる頂点
		int s = msb(set);

		// s を含まない独立集合のスコアを加算する．
		int sub = set - (1 << s);
		ind[set] = ind[sub];

		// s を含む独立集合のスコアを加算する．
		repe(t, g[s]) {
			// s と辺で結ばれた頂点は選ぶことができない．
			sub &= ~(1 << t);
		}
		ind[set] += w[s] * ind[sub];
	}

	// pow_k[set] : (-1)^|V - set| ind[set]^k
	vm pow(1LL << n);
	repb(set, n) pow[set] = (n - popcount(set)) & 1 ? -1 : 1;

	repi(k, 1, n) {
		mint sum = 0;

		repb(set, n) {
			pow[set] *= ind[set];
			sum += pow[set];
		}

		if (sum != 0) return k;
	}
	return n;
}


//【最小クリーク被覆】O(2^n n)
/*
* 無向グラフ g の最小クリーク被覆の大きさを返す．
* S ⊂ V がクリークであるとは，S の任意の 2 点を結ぶ辺が E に属することをいう．
*
* 利用：【補グラフ】,【彩色数】
*/
int minimum_clique_cover(const Graph& g) {
	// verify : https://atcoder.jp/contests/abc187/tasks/abc187_f

	//【方法】
	// 最小クリーク被覆の大きさは，補グラフについての彩色数に等しい．

	Graph gc = complement_graph(g);

	return chromatic_number(gc);
}


//【巡回セールスマン問題】O(2^n n^2)
/*
* 重み付き有向グラフ g の最小コストハミルトン閉路のコストを返す（存在しなければ INFL）
*
*（bit DP）
*/
ll traveling_salesman_problem(const WGraph& g) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_cv

	int n = sz(g);
	if (n == 0) return INFL;
	if (n == 1) return 0;

	// dp[s][set] : 頂点 s から n-1 までの set を通る単純パスの最小コスト
	vvl dp(n, vl(1LL << n, INFL));
	dp[n - 1][1LL << (n - 1)] = 0;

	// 貰う DP
	repb(set, n) rep(s, n) {
		// s から出発するので s を通らないことはありえない．
		if (!getb(set, s)) continue;

		// s から出ている各辺 e について
		repe(e, g[s]) {
			int t = e.to; ll c = e.cost;

			// e の行き先 t が set に含まれていなければ e は通れない．
			if (!getb(set, t)) continue;

			// e を通って s → t と進む方がコストが小さければ更新する．
			chmin(dp[s][set], c + dp[t][set - (1 << s)]);
		}
	}

	ll res = INFL;

	// ハミルトン路 t→n-1 に辺 n-1→t を追加してハミルトン閉路を作る．
	repe(t, g[n - 1]) chmin(res, dp[t][(1LL << n) - 1] + t.cost);

	return res;
}


//【巡回セールスマン問題（隣接行列）】O(2^n n^2)
/*
* 重み付き隣接行列 c[0..n)[0..n) が表す有向グラフの最小コストハミルトン閉路のコストを返す（存在しなければ INFL）
*
*（bit DP）
*/
ll traveling_salesman_problem(const vvl& c) {
	// verify : https://atcoder.jp/contests/abc180/tasks/abc180_e

	int n = sz(c);
	if (n == 0) return INFL;
	if (n == 1) return 0;

	// dp[s][set] : 頂点 s から n-1 までの set を通る単純パスの最小コスト
	vvl dp(n, vl(1LL << n, INFL));
	dp[n - 1][1LL << (n - 1)] = 0;

	// 貰う DP
	repb(set, n) {
		// s, t ∈ set なる辺 s→t をチェックする．
		repis(s, set) repis(t, set - (1 << s)) {
			chmin(dp[s][set], c[s][t] + dp[t][set - (1 << s)]);
		}
	}

	ll res = INFL;

	// ハミルトン路 t→n-1 に辺 n-1→t を追加してハミルトン閉路を作る．
	rep(t, n) chmin(res, dp[t][(1 << n) - 1] + c[n - 1][t]);

	return res;
}


//【最小コストハミルトン閉路】O(2^n n^2)
/*
* 重み付き隣接行列 c[0..n)[0..n) が表す有向グラフ g に対し，各頂点集合 set について，
* 誘導部分グラフ g[set] の最小コストハミルトン閉路のコストを並べたリストを返す．
*
*（bit DP）
*/
vl shortest_hamiltonian_cycle(const vvl& c) {
	int n = sz(c);
	if (n == 0) return vl{ INFL };
	if (n == 1) return vl{ INFL, 0 };

	vl res(1LL << n, INFL);

	// v : set に含まれる番号最大の頂点
	rep(v, n) {
		// dp[s][set] : 頂点 s から頂点 v までの set⊂[0..v] を通る単純パスの最小コスト
		vvl dp(v + 1, vl(1LL << (v + 1), INFL));
		dp[v][1LL << v] = 0;

		// 貰う DP
		repi(set, 1 << v, (1 << (v + 1)) - 1) {
			// s, t ∈ set なる辺 s→t をチェックする．
			repis(s, set) repis(t, set - (1 << s)) {
				chmin(dp[s][set], c[s][t] + dp[t][set - (1 << s)]);
			}
		}

		// g[set] のハミルトン路 s→v に辺 v→s を追加して g[set] のハミルトン閉路を得る．
		repi(set, 1 << v, (1 << (v + 1)) - 1) repis(s, set) {
			chmin(res[set], dp[s][set] + c[v][s]);
		}
	}

	return res;
}


//【最小コストハミルトンパス】O(2^n n^2)
/*
* 重み付き有向グラフ g の最小コストハミルトンパスのコストを返す（存在しなければ INFL）
* また頂点 s から出発する g[set] のハミルトンパスの最小コストを dp[s][set] に格納する．
*
*（bit DP）
*/
ll shortest_hamiltonian_path(const WGraph& g, vvl& dp) {
	// verify : https://atcoder.jp/contests/jag2013autumn/tasks/icpc2013autumn_c

	int n = sz(g);

	// dp[s][set] : 頂点 s から出発し set を通る単純パスの最小コスト
	dp = vvl(n, vl(1LL << n, INFL));

	repb(set, n) rep(s, n) {
		// s から出発するので s を通らないことはありえない．
		if (!getb(set, s)) continue;

		// set = {s} の場合は不動でいいのでコストは 0 である．
		if (set == (1 << s)) {
			dp[s][set] = 0;
			continue;
		}

		// s から出ている各辺 e について
		repe(e, g[s]) {
			int t = e.to; ll c = e.cost;

			// e の行き先 t が set に含まれていなければ e は通れない．
			if (!getb(set, t)) continue;

			// e を通って s → t と進む方がコストが小さければ更新する．
			chmin(dp[s][set], c + dp[t][set - (1 << s)]);
		}
	}

	ll res = INFL;
	rep(s, n) chmin(res, dp[s][(1 << n) - 1]);

	return res;
}


//【最小コストハミルトンパス】O(2^n n^3)
/*
* 与えられた重み付き有向グラフ g に対し，g[set] の s→t ハミルトンパス
* の最小コストを dp[s][t][set] に格納し dp を返す．
*
*（bit DP）
*/
vvvl shortest_hamiltonian_path(const WGraph& g) {
	// verify : https://mojacoder.app/users/milkcoffee/contests/mr-contest-002/tasks/3

	int n = sz(g);

	// dp[s][t][set] : set を通る s→t 単純パスの最小コスト
	vvvl dp(n, vvl(n, vl(1LL << n, INFL)));

	// 貰う DP
	repb(set, n) rep(s, n) rep(t, n) {
		// s→t パスなので s, t を通らないことはありえない．
		if (!getb(set, s) || !getb(set, t)) continue;

		// set = {s} = {t} の場合は不動でいいのでコストは 0 である．
		if (s == t && set == (1 << s)) {
			dp[s][t][set] = 0;
			continue;
		}

		// s から出ている各辺 e について
		repe(e, g[s]) {
			int v = e.to; ll c = e.cost;

			// e の行き先 v が set に含まれていなければ e は通れない．
			if (!getb(set, v)) continue;

			// v を通って s → t と進む方がコストが小さければ更新する．
			chmin(dp[s][t][set], c + dp[v][t][set - (1 << s)]);
		}
	}

	return dp;
}


//【中国人郵便配達問題】O(2^n n)
/*
* 重み付き連結無向グラフの全辺を通る閉路の最小コストを返す．
*
* 利用：【全頂点対最短路】,【最小コスト完全マッチング】
*/
ll chinese_postman_problem(const WGraph& g) {
	// 参考：https://perogram.hateblo.jp/entry/2020/09/30/101602
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_2_B

	int n = sz(g);

	// 全頂点対の最短経路長を求めておく．
	vvl cost = warshall_floyd(g);

	// 次数が奇数の頂点を求める（握手補題により必ず偶数個）
	vi v_odd;
	rep(i, n) if (sz(g[i]) & 1) v_odd.push_back(i);
	int n_odd = sz(v_odd);

	// 次数が奇数の頂点だけを集めた重み付き完全グラフ g2 を作る．
	// 辺のコストは元のグラフ g での最短経路長とする．
	WGraph g2(n_odd);
	rep(i, n_odd) rep(j, n_odd) g2[i].push_back({ j, cost[v_odd[i]][v_odd[j]] });

	// g の全ての辺のコストの総和を求める．
	ll res = 0;
	rep(s, n) repe(e, g[s]) res += e.cost;
	res /= 2;

	// オイラー閉路を作るために必要な最小のコスト分を加算する．
	vl cost_odd = minimum_cost_matching(g2);
	res += cost_odd.back();

	return res;
}


//【最小コストシュタイナー木】O(n 3^K + (n + m) 2^K log n)（K = |vs|）
/*
* 与えられた重み付き無向グラフ g と頂点集合 tm について，
* g の辺からなる木で tm を全て含むもののコストの最小値を返す．
* 必要ならば es に選んだ辺の両端点の組からなるリストを格納する．
*/
ll minimum_cost_steiner_tree(const WGraph& g, const vi& tm, vector<pii>* es = nullptr) {
	// 参考 : https://kopricky.github.io/code/Academic/steiner_tree.html
	// verify : https://judge.yosupo.jp/problem/minimum_steiner_tree

	int n = sz(g), K = sz(tm);

	// dp[set][v] : 頂点集合 set∪{v} を連結にする最小コスト
	vvl dp(1LL << K, vl(n, INFL));
	rep(s, n) dp[0][s] = 0;
	rep(k, K) dp[1LL << k][tm[k]] = 0;

	using P = tuple<int, int, int>;
	vector<vector<P>> prv(1LL << K, vector<P>(n, { -1, -1, -1 }));

	repb(set, K) {
		if (set == 0) continue;

		// set = sub凵(set-sub) と分け，それぞれが v と連結になるパターン → SoS-bit 全探索
		rep(v, n) {
			for (int sub = set; sub > 0; sub = (sub - 1) & set) {
				if (chmin(dp[set][v], dp[sub][v] + dp[set ^ sub][v])) {
					prv[set][v] = { sub, set ^ sub, v };
				}
			}
		}

		// set と u が連結になり，u と v を最短パスで結ぶパターン → 多始点ダイクストラ
		priority_queue_rev<pli> q;
		rep(v, n) q.push({ dp[set][v], v });

		while (!q.empty()) {
			auto [dpv, v] = q.top(); q.pop();
			if (dp[set][v] < dpv) continue;

			repe(t, g[v]) {
				if (chmin(dp[set][t], dp[set][v] + t.cost)) {
					prv[set][t] = { set, -1, v };
					q.push({ dp[set][t], t });
				}
			}
		}
	}

	if (es) {
		es->clear();

		queue<pii> q;
		q.emplace((1 << K) - 1, tm[0]);

		while (!q.empty()) {
			auto [set, v] = q.front(); q.pop();

			auto [pset1, pset2, pv] = prv[set][v];
			if (pset1 == -1) continue;

			if (pset2 != -1) {
				q.emplace(pset1, pv);
				q.emplace(pset2, pv);
			}
			else {
				es->emplace_back(pv, v);
				q.emplace(pset1, pv);
			}
		}
	}

	return dp[(1 << K) - 1][tm[0]];
}


//【Turan の定理】
/*
* n 頂点の単純無向グラフのうち，大きさ r+1 のクリークを含まない条件の下で辺数を最大にするものは
* 頂点をほぼ r 等分した完全 r 部グラフであり，そのときの辺数は
*	n(n-1)/2 - ( (n mod r)bin(ceil(n/r), 2) + (r - (n mod r))bin(floor(n/r), 2) )
* である．
* 
* verify : https://projecteuler.net/problem=713
*/


//【頂点除去による DAG 化 → 無理】
/*
* 与えられた有向グラフ g に対し，それを DAG にするために取り除かなければならない
* 最小の頂点集合を求める問題は「フィードバック頂点集合問題」と呼ばれており，NP 困難である．
*/


