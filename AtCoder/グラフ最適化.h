#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "最短路.h"
#include "マッチング.h"
#include "ビット全探索.h"
// ■■■■■ グラフ上の最適化問題 ■■■■■


//【最小全域森／クラスカル法】O(|E| log|V|)
/*
* クラスカル法でコスト付き無向グラフ g の最小全域森を求める．
* 最小全域森は msf に構成し，各最小全域木の代表元を mst に格納する．
* また戻り値として最小コストを返す．
*/
ll kruskal(const WGraph& g, WGraph& msf, vi& mst) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%82%AF%E3%83%A9%E3%82%B9%E3%82%AB%E3%83%AB%E6%B3%95
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_A

	int n = sz(g);
	msf = WGraph(n);

	// 辺を集めてコスト昇順にソートする．
	vector<tuple<ll, int, int>> edges;
	rep(i, n) {
		repe(v, g[i]) {
			edges.push_back({ v.cost, i, v.to });
		}
	}
	sort(all(edges));

	ll cost = 0; // 最小コスト
	dsu d(n + 1); // 連結判定用
	rep(i, sz(edges)) {
		// もし辺の両端が既に連結なら繋がない．
		int s = get<1>(edges[i]);
		int t = get<2>(edges[i]);
		if (d.same(s, t)) {
			continue;
		}

		// そうでないならコスト最小の辺なのでそれで繋ぐ．
		ll c = get<0>(edges[i]);
		msf[s].push_back({ t, c });
		cost += c;
		d.merge(s, t);
	}

	// 連結成分のそれぞれが最小全域木なので，その代表元を記録．
	mst = vi();
	repe(t, d.groups()) {
		mst.push_back(t[0]);
	}

	return cost;
}


//【最小全域木／プリム法】O(|E| log|V|)
/*
* コスト付きグラフ g の頂点 r を含む連結成分の最小全域木を mst に格納する．
* また戻り値として最小コストを返す．
*/
ll prim(const WGraph& g, int r, WGraph& mst) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%97%E3%83%AA%E3%83%A0%E6%B3%95
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_A

	int n = sz(g);
	mst = WGraph(n);
	ll res = 0;

	// selected[v] : 頂点 v を既に選んだかどうか
	vb selected(n);
	selected[r] = true;

	// 選んだ頂点から出ている辺をコスト昇順に記録しておくための優先度付きキュー．
	using E = tuple<ll, int, int>;
	priority_queue<E, vector<E>, greater<E>> q;
	repe(e, g[r]) {
		q.push({ e.cost, r, e.to });
	}

	while (!q.empty()) {
		ll c;
		int s, t;
		tie(c, s, t) = q.top();
		q.pop();

		// 既に選んだ頂点への辺なら何もしない．
		if (selected[t]) {
			continue;
		}

		// 最小全域木に辺を追加し，頂点を選んだことを記録しておく．
		mst[s].push_back({ t, c });
		res += c;
		selected[t] = true;

		// 調べるべき辺を追加する．
		repe(e, g[t]) {
			q.push({ e.cost, t, e.to });
		}
	}

	return res;
}


//【最大クリーク問題】O(2^√(2|E|) |V|)
/*
* 無向グラフ g の最大クリークの大きさを返す．
* 
* 利用：【部分集合の全探索】
*/
int maximum_clique(const Graph& g) {
	// 参考：https://www.slideshare.net/wata_orz/ss-12131479

	int n = sz(g);

	// 隣接行列 adj，各頂点の次数 deg，総次数 deg_sum，
	// 最小次数 deg_min，次数最小頂点の番号 i_min を得る．
	vvb adj(n, vb(n));
	vi deg(n);
	int deg_sum = 0, deg_min = INF, i_min = -1;
	rep(s, n) {
		repe(t, g[s]) {
			adj[s][t] = true;
			deg[s]++;
		}
		deg_sum += deg[s];

		if (chmin(deg_min, deg[s])) {
			i_min = s;
		}
	}

	// 考慮すべき頂点のリスト
	vi v(n);
	rep(i, n) {
		v[i] = i;
	}

	// 素朴な方法で最大クリークを求める O(2^n n)
	function<int()> naive = [&]() {
		int res = 0;

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
						goto LOOP_END;
					}
				}
			}

			// クリークが見つかったので大きさを更新する．
			chmax(res, popcount(set));

		LOOP_END:;
		}
		return res;
	};

	int res = 1;
	while (n > 0) {
		// 辺に対して頂点が十分少ないなら素朴な方法で構わない．
		if (deg_min * deg_min <= deg_sum) {
			return max(res, naive());
		}

		// 次数最小の頂点 v[i_min] の隣接点の番号の集合を得る．
		// 同時に v[i_min] に出入りする辺を削除したことにし，各頂点の次数 deg を更新する．
		int set = 0;
		rep(i, n) {
			if (adj[v[i_min]][v[i]]) {
				set += 1 << i;

				deg[v[i]]--;
			}
		}

		// まず v[i_min] を含む最大クリークの大きさ res を求める．
		// v[i_min] の隣接点の部分集合 sub すべてについて
		repbs(sub, set) {
			rep(i, n) {
				// sub に選んでいないなら無関係
				if (!(sub & (1 << i))) {
					continue;
				}

				repi(j, i + 1, n - 1) {
					// sub に選んでいないなら無関係
					if (!(sub & (1 << j))) {
						continue;
					}

					// sub がクリークでなければ何もしない．
					if (!adj[v[i]][v[j]]) {
						goto LOOP_END;
					}
				}
			}

			// sub がクリークなら v[i_min] と合わせてもクリークとなる．
			chmax(res, popcount(sub) + 1);

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
		if (v == t) {
			return f;
		}

		// 頂点 v に訪れたことを記録する．
		seen[v] = true;

		// v から出ている残余ネットワークの各辺 e について
		rep(j, sz(res[v])) {
			auto& e = res[v][j];

			// e の先の頂点に既に訪れていたり，e に空きが無いなら何もしない．
			if (seen[e.to] || e.cost == 0) {
				continue;
			}

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
		if (f == 0) {
			return mf;
		}

		// フローが流れたら流量を更新する．
		mf += f;
	}

	// ここにはこない．
	return 0LL;
}


//【巡回セールスマン問題】O(|V|^2 2^|V|)
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
		if (seen[s][set]) {
			return dp[s][set];
		}

		// s から出ている各辺 e について
		repe(e, g[s]) {
			auto t = e.to;
			auto c = e.cost;

			// e の行き先 t が set に含まれていなければ何もしない．
			if (!(set & (1 << t))) {
				continue;
			}

			// s → t と進む方がコストが小さければ更新する．
			chmin(dp[s][set], rf(t, set - (1 << t)) + c);
		}

		seen[s][set] = true;
		return dp[s][set];
	};

	// メモ化再帰を用いて bit DP を行う．
	auto res = rf(n - 1, (1 << n) - 1);
	return (res == INFL ? -1 : res);
}


//【最小コストハミルトンパス】O(|V|^2 2^|V|)
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
		if (seen[s][set]) {
			return dp[s][set];
		}
		seen[s][set] = true;

		// s から出ている各辺 e について
		repe(e, g[s]) {
			auto t = e.to;
			auto c = e.cost;

			// e の行き先 t が set に含まれていなければ何もしない．
			if (!(set & (1 << t))) {
				continue;
			}

			// s → t と進む方がコストが小さければ更新する．
			chmin(dp[s][set], rf(t, set - (1 << t)) + c);
		}

		return dp[s][set];
	};

	// メモ化再帰を用いて bit DP を行う．
	ll res = INFL;
	rep(s, n) {
		chmin(res, rf(s, (1 << n) - 1 - (1 << s)));
	}
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


