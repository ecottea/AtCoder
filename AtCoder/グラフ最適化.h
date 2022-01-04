#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "変形(グラフ).h"
#include "最短路.h"
#include "ビット全探索.h"
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
	vb is_ind1(1 << n1, true);

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
				is_ind1[set1] = is_ind1[set1] & is_ind1[sub1];
			}
		}
	}


	// no_edge[set1] : set1 ⊂ V1 との間に辺をもたない V2 の頂点集合
	vi no_edge(1 << n1);

	// 空集合 ⊂ V1 の相手は明らかに全体集合 V2
	no_edge[0] = (1 << n2) - 1;

	// 1 点集合 {s} ⊂ V1 の相手は s と辺で結ばれていない V2 の点集合
	rep(s, n1) {
		no_edge[1 << s] = (1 << n2) - 1;
		repe(t, g[s]) {
			if (t < n1) continue;

			no_edge[1 << s] -= (1 << (t - n1));
		}
	}

	// 2 点以上の集合 set1 ⊂ V1 については 1 点集合の相手との共通部分を考える．
	repb(set1, n1) {
		rep(i, n1) {
			if (set1 & (1 << i)) {
				int sub1 = set1 - (1 << i);
				no_edge[set1] = no_edge[1 << i] & no_edge[sub1];
				break;
			}
		}
	}


	// max_ind2[set2] : set2 ⊂ V2 の最大独立集合の 1 つ
	vi max_ind2(1 << n2);

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
* 利用：【補グラフ】，【最大独立集合問題】
*/
int maximum_clique(const Graph& g, vi* vs = nullptr) {
	// verify : https://atcoder.jp/contests/abc002/tasks/abc002_4

	//【方法】
	// 最大クリーク問題は，補グラフについての最大独立集合問題と等価である．

	Graph gc;
	complement_graph(g, gc);

	return maximum_independent_set(gc, vs);
}


//【クリークの列挙】O(2^(1.4√|E|) |V|)
/*
* 無向グラフ g の i 番目に見つけたクリークを cs[i] に頂点の列として列挙する．
* S ⊂ V がクリークであるとは，S の任意の 2 点を結ぶ辺が E に属することをいう．
*/
void enumerate_clique(const Graph& g, vvi& cs) {
	// 参考：https://www.slideshare.net/wata_orz/ss-12131479
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/2306

	int n = sz(g);
	cs.clear();

	// 隣接行列 adj，各頂点の次数 deg，総次数 deg_sum，
	// 最小次数 deg_min，次数最小頂点の番号 i_min を得る．
	vvb adj(n, vb(n));
	vi deg(n);
	int deg_sum = 0, deg_min = INF, i_min = -1;
	rep(s, n) {
		deg[s] = sz(g[s]);
		deg_sum += deg[s];
		repe(t, g[s]) adj[s][t] = true;

		if (chmin(deg_min, deg[s])) i_min = s;
	}

	// 考慮すべき頂点のリスト
	vi v(n);
	iota(all(v), 0);

	// 素朴な方法で最大クリークを求める O(2^n n)
	function<void()> naive = [&]() {
		// 全ての部分集合 set について
		repb(set, n) {
			bool sum = 0;

			// n 頂点それぞれについて
			rep(i, n) {
				// set に選んでいないなら無関係
				if (!(set & (1 << i))) {
					continue;
				}

				// i 番目以降の頂点について
				repi(j, i + 1, n - 1) {
					// set に選んでいないなら無関係 
					if (!(set & (1 << j))) {
						continue;
					}

					// 辺 (v[i], v[j]) がなければクリークでない．
					if (!adj[v[i]][v[j]]) {
						goto NEXT_LOOP;
					}
				}
			}

			// クリークが見つかったので記録する．
			cs.push_back(vi());
			rep(i, n) {
				if (set & (1 << i)) {
					cs.rbegin()->push_back(v[i]);
				}
			}

		NEXT_LOOP:;
		}
	};

	int res = 1;
	while (n > 0) {
		// 辺に対して頂点が十分少ないなら素朴な方法で構わない．
		if (deg_min * deg_min >= deg_sum) {
			naive();
			return;
		}

		// 次数最小の頂点 v[i_min] の隣接点の番号の集合を得る．
		// 同時に v[i_min] に出入りする辺を削除したことにし，各頂点の次数 deg を更新する．
		vi ia;
		rep(i, n) {
			if (adj[v[i_min]][v[i]]) {
				ia.push_back(i);

				deg[v[i]]--;
			}
		}
		int d = sz(ia);

		// まず v[i_min] を含む最大クリークの大きさ res を求める．
		// v[i_min] の隣接点の部分集合 sub すべてについて
		repb(sub, d) {
			rep(i, d) {
				// sub に選んでいないなら無関係
				if (!(sub & (1 << i))) {
					continue;
				}

				repi(j, i + 1, d - 1) {
					// sub に選んでいないなら無関係
					if (!(sub & (1 << j))) {
						continue;
					}

					// sub がクリークでなければ何もしない．
					if (!adj[v[ia[i]]][v[ia[j]]]) {
						goto LOOP_END;
					}
				}
			}

			// sub がクリークなら v[i_min] と合わせてもクリークとなるので記録する．
			cs.push_back(vi({ v[i_min] }));
			rep(i, d) {
				if (sub & (1 << i)) {
					cs.rbegin()->push_back(v[ia[i]]);
				}
			}

		LOOP_END:;
		}

		// v[i_min] を含む最大クリークの大きさは求まったので，
		// 以降は v[i_min] を含まないクリークだけを考えれば良い．
		// 頂点 v[i_min] と v[n-1] を交換して n を減らすことで v[i_min] を除去する．
		swap(v[i_min], v[n - 1]);
		n--;

		// 総次数 deg_sum，最小次数 deg_min，次数最小頂点の番号 i_min を得る．
		deg_sum = 0;
		deg_min = INF;
		rep(i, n) {
			if (chmin(deg_min, deg[v[i]])) {
				i_min = i;
			}
			deg_sum += deg[v[i]];
		}
	}
}


//【巡回セールスマン問題】O(2^|V| |V|^2)
/*
* コスト付き有向グラフ g の最小コストハミルトン閉路のコストを返す．
* ハミルトン閉路が存在しない場合は -1 を返す．
*
*（bit DP）
*/
ll traveling_salesman_problem(const WGraph& g) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_2_A

	int n = sz(g);

	// dp[i][set] : 頂点 i から set を通り頂点 n - 1 までのハミルトンパスの最小コスト
	//	i !∈ set だが，n - 1 ∈ set なので注意．
	vvl dp(n, vl(1 << n, INFL));
	vvb seen(n, vb(1 << n));
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
* コスト付き有向グラフ g の最小コストハミルトンパスのコストを返す．
* ハミルトンパスが存在しない場合は -1 を返す．
*
*（bit DP）
*/
ll shortest_hamiltonian_path(const WGraph& g) {
	int n = sz(g);

	// dp[s][set] : 頂点 s から出発し set を通るハミルトンパスの最小コスト
	//	s !∈ set とする．
	vvl dp(n, vl(1 << n, INFL));
	vvb seen(n, vb(1 << n));
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


//【最大流問題／フォード・ファルカーソンのアルゴリズム】O(|E| maxflow)
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


