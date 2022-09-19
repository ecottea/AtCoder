#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "変換(グラフ).h"
#include "最短路.h"
#include "ビット全探索.h"
#include "数え上げ(グラフ).h"
#include "マッチング(一般).h"
// ■■■■■ グラフ上の最適化問題 ■■■■■


//【最大独立集合問題】O(2^(|V|/2) |V|)
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
	rep(s, n1) {
		repe(t, g[s]) {
			if (t >= n1) continue;

			int set = (1 << s) + (1 << t);
			is_ind1[set] = false;
		}
	}

	// 独立集合でない集合を部分集合にもつ集合は独立集合ではない．
	repb(set1, n1) {
		rep(i, n1) {
			if (set1 & (1 << i)) {
				int sub1 = set1 - (1 << i);
				is_ind1[set1] = is_ind1[set1] && is_ind1[sub1];
			}
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
	repb(set1, n1) {
		rep(i, n1) {
			if (set1 & (1LL << i)) {
				int sub1 = set1 - (1LL << i);
				no_edge[set1] = no_edge[1LL << i] & no_edge[sub1];
				break;
			}
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
	rep(i, n2) {
		repe(t, g[n1 + i]) {
			if (t < n1) continue;

			int set2 = (1 << i) + (1 << (t - n1));
			max_ind2[set2] = 1 << i;
		}
	}

	// 3 点以上の集合 set2 ⊂ V2 については，それが独立集合ならそれ自身，
	// さもなくば 1 つ抜き集合の最大独立集合のうちの最大のものを選択する．
	repb(set2, n2) {
		int pc = popcount(set2);
		if (pc <= 2) {
			// どの辺の両端ともならない 2 点集合 {s, t} ⊂ V2 は独立集合である．
			if (pc == 2 && max_ind2[set2] == 0) {
				max_ind2[set2] = set2;
			}
			continue;
		}

		bool ind_flag = true;
		rep(i, n2) {
			if (set2 & (1 << i)) {
				int sub2 = set2 - (1 << i);
				if (popcount(max_ind2[set2]) < popcount(max_ind2[sub2])) {
					max_ind2[set2] = max_ind2[sub2];
				}
				if (max_ind2[sub2] != sub2) {
					ind_flag = false;
				}
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
		rep(i, n1) {
			if (set1_max & (1 << i)) {
				vs->push_back(i);
			}
		}
		rep(i, n2) {
			if (set2_max & (1 << i)) {
				vs->push_back(n1 + i);
			}
		}
	}

	return pc_max;
}


//【最大クリーク問題】O(2^(|V|/2) |V|)
/*
* 無向グラフ g の最大クリークの大きさを返す．
* S ⊂ V がクリークであるとは，S の任意の 2 点を結ぶ辺が E に属することをいう．
*
* 利用：【補グラフ】,【最大独立集合問題】
*/
int maximum_clique(const Graph& g, vi* vs = nullptr) {
	// verify : https://atcoder.jp/contests/abc002/tasks/abc002_4

	//【方法】
	// 最大クリーク問題は，補グラフについての最大独立集合問題と等価である．

	Graph gc;
	complement_graph(g, gc);

	return maximum_independent_set(gc, vs);
}


//【彩色数】O(2^|V| |V|)
/*
* 無向グラフ g の彩色数を返す．彩色数とは，独立集合への分割の最小個数である．
*
* 利用：【独立集合の数え上げ】
*/
int chromatic_number(const Graph& g) {
	// 参考 : https://drken1215.hatenablog.com/entry/2019/01/16/030000
	// verify : https://judge.yosupo.jp/problem/chromatic_number

	//【方法】
	// 判定問題
	//		(P): k 個の独立集合へ分割可能か
	// を考え，(P) を満たす最小の k を線形探索する．
	//
	// (P) の代わりに
	//		(P2): 長さ k の独立集合の列で被覆可能か
	// を考えても，可能性は変わらない．
	//
	// (P2) の代わりに
	//		(P3): 長さ k の独立集合の列で被覆する方法は何通りか
	// を考え，この答えが 0 か否かを見ることにする．そこで
	//		g[set] : set を長さ k の独立集合の列で被覆する方法の数
	// とおく．これは彩色多項式とは関係ないので注意．
	//
	// g の下位集合でのゼータ変換を
	//		f[set] = Σ_(sub ⊂ set) g[sub]
	// とおく．f は
	//		f[set] = set から 長さ k の独立集合の列を選ぶ方法の数
	// と解釈できるので，
	//		f[set] = (set の独立集合の数)^k
	// と表される．
	//
	// 以上を逆に辿ると，以下のアルゴリズムが得られる：
	//	k について昇順に
	//		各 set ⊂ V について set の独立集合の数を求める．
	//		→ f[set] = (set の独立集合の数)^k を求める．
	//		→ f[set] に包除原理を適用し g[V] を求める．
	//	を行い，始めて g[V] != 0 となった k を返せば良い．

	int n = sz(g);
	if (n == 0) return 0;

	// ind[set] : set の部分集合のうち，独立集合をなすものの個数
	vm ind;
	count_independent_set(g, ind);

	// pow_k[set] : (-1)^#(V - set) ind[set]^k
	vm pow(1LL << n);
	repb(set, n) pow[set] = (n - popcount(set)) % 2 ? -1 : 1;

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


//【最小クリーク被覆】O(2^|V| |V|)
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

	Graph gc;
	complement_graph(g, gc);

	return chromatic_number(gc);
}


//【巡回セールスマン問題】O(2^|V| |V|^2)
/*
* コスト付き有向グラフ g の最小コストハミルトン閉路のコストを返す（存在しなければ -1）
*
*（bit DP）
*/
ll traveling_salesman_problem(const WGraph& g) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_2_A

	int n = sz(g);

	// dp[i][set] : 頂点 i から set を通り頂点 n - 1 までのハミルトンパスの最小コスト
	//	i !∈ set だが，n - 1 ∈ set なので注意．
	vvl dp(n, vl(1LL << n, INFL));
	vvb seen(n, vb(1LL << n));
	dp[n - 1][0] = 0;
	seen[n - 1][0] = true;

	// メモ化再帰用の関数の定義
	function<ll(int, int)> rf = [&](int s, int set) {
		// もし確定済ならば DP テーブルの値をそのまま返す．
		if (seen[s][set]) return dp[s][set];
		seen[s][set] = true;

		// s から出ている各辺 e について
		repe(e, g[s]) {
			int t = e.to; ll c = e.cost;

			// e の行き先 t が set に含まれていなければ何もしない．
			if (!(set & (1 << t))) continue;

			// s → t と進む方がコストが小さければ更新する．
			chmin(dp[s][set], rf(t, set - (1 << t)) + c);
		}

		return dp[s][set];
	};

	// メモ化再帰を用いて bit DP を行う．
	auto res = rf(n - 1, (1 << n) - 1);
	return (res == INFL ? -1 : res);
}


//【最小コストハミルトンパス】O(2^|V| |V|^2)
/*
* コスト付き有向グラフ g の最小コストハミルトンパスのコストを返す（存在しなければ -1）
*
*（bit DP）
*/
ll shortest_hamiltonian_path(const WGraph& g) {
	int n = sz(g);

	// dp[s][set] : 頂点 s から出発し set を通るハミルトンパスの最小コスト
	//	s !∈ set とする．
	vvl dp(n, vl(1LL << n, INFL));
	vvb seen(n, vb(1LL << n));
	rep(s, n) {
		dp[s][0] = 0;
		seen[s][0] = true;
	}

	// メモ化再帰用の関数の定義
	function<ll(int, int)> rf = [&](int s, int set) {
		// もし確定済ならば DP テーブルの値をそのまま返す．
		if (seen[s][set]) return dp[s][set];
		seen[s][set] = true;

		// s から出ている各辺 e について
		repe(e, g[s]) {
			int t = e.to; ll c = e.cost;

			// e の行き先 t が set に含まれていなければ何もしない．
			if (!(set & (1 << t))) continue;

			// s → t と進む方がコストが小さければ更新する．
			chmin(dp[s][set], rf(t, set - (1 << t)) + c);
		}

		return dp[s][set];
	};

	// メモ化再帰を用いて bit DP を行う．
	ll res = INFL;
	rep(s, n) chmin(res, rf(s, (1 << n) - 1 - (1 << s)));

	return (res == INFL ? -1 : res);
}


//【中国人郵便配達問題】O(2^|V| |V|)
/*
* コスト付き連結無向グラフの全辺を通る閉路の最小コストを返す．
*
* 利用：【最小コスト完全マッチング】
*/
ll chinese_postman_problem(const WGraph& g) {
	// 参考：https://perogram.hateblo.jp/entry/2020/09/30/101602
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_2_B

	int n = sz(g);

	// 全頂点対の最短経路長を求めておく．
	vvl cost;
	warshall_floyd(g, cost);

	// 次数が奇数の頂点を求める．
	vi vodd;
	rep(i, n) {
		if (sz(g[i]) % 2 == 1) {
			vodd.push_back(i);
		}
	}
	int nodd = sz(vodd);

	// 次数が奇数の頂点だけを集めたコスト付きグラフの隣接行列を作る．
	// 辺のコストは元のグラフ g での最短経路長とする．
	vvl adj(nodd, vl(nodd, INFL));
	rep(i, nodd) {
		rep(j, nodd) {
			adj[i][j] = cost[vodd[i]][vodd[j]];
		}
	}

	// g の全ての辺のコストの総和を求める．
	ll res = 0;
	rep(s, n) {
		repe(e, g[s]) {
			res += e.cost;
		}
	}
	res /= 2;

	// オイラー閉路を作るために必要な最小のコスト分を加算する．
	res += minimum_cost_matching(adj);

	return res;
}


//【最大流問題】O(|E| maxflow)
/*
* コスト付き有向グラフ g の始点 s から終点 t までの最大フローの大きさを返す．
*/
ll ford_fullkerson(const WGraph& g, int s, int t) {
	// 参考：https://algo-logic.info/ford-fullkerson/
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_6_A

	int n = sz(g);

	// 残余ネットワークを作り初期化する．
	// また更新のために逆向きの辺の番号を記録しておく．
	WGraph res(n);
	vvi rev(n);
	rep(i, n) {
		repe(e, g[i]) {
			// 順方向は g と同じ，逆方向は 0 で初期化．
			res[i].push_back(e);
			res[e.to].push_back({ i, 0 });

			// e = res[i][j] の逆向きの辺は res[e.to][rev[i][j]]
			rev[i].push_back(sz(res[e.to]) - 1);
			rev[e.to].push_back(sz(res[i]) - 1);
		}
	}

	vb seen(n);
	function<ll(int, ll)> dfs = [&](int v, ll f) {
		// 終点 t まで流せたら流量を返す．
		if (v == t) return f;

		// 頂点 v に訪れたことを記録する．
		seen[v] = true;

		// v から出ている残余ネットワークの各辺 e について
		rep(j, sz(res[v])) {
			auto& e = res[v][j];

			// e の先の頂点に既に訪れていたり，e に空きが無いなら何もしない．
			if (seen[e.to] || e.cost == 0) continue;

			// e にフローを流す．
			ll f2 = dfs(e.to, min(f, e.cost));

			// もしフローが流れたら残余ネットワークを更新し流量を返す．
			if (f2 > 0) {
				e.cost -= f2;
				res[e.to][rev[v][j]].cost += f2;
				return f2;
			}
		}

		// ここまでくるのはフローを流せなかったとき
		return 0LL;
	};

	ll mf = 0;
	while (true) {
		// どの頂点にも訪れていない状態にする．
		seen.assign(n, false);

		// 始点 s からフローを流す．
		ll f = dfs(s, INFL);

		// フローが流れなかったら最大まで流し切ったので結果を返す．
		if (f == 0) return mf;

		// フローが流れたら流量を更新する．
		mf += f;
	}

	// ここにはこない．
	return 0LL;
}


