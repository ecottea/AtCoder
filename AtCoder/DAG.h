#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "最短路.h"
#include "マッチング(二部).h"
// ■■■■■ DAG（有向非巡回グラフ） ■■■■■


//【DAG と DP】
/*
* いかなる DP も，その状態を頂点，状態遷移を有向辺とすると DAG になる．
* もし DAG でないと，メモ化再帰で実装したときに無限ループを生じてしまう．
* 参考 : https://tayama-2.hatenadiary.org/entry/20111210/1323502092
* 
* TLE 以前に DP の遷移式すら作れずに困った場合，
*		(a) 持つ状態を増やして状態遷移グラフを DAG にする．
*		(b) DAG でなくても効率的に求まる最短路問題に帰着する．
* などの対処が考えられる．
* 
* (a) の例：
* 非 DAG の長さ k のパスの数え上げにおいては，「どの頂点に居るか」だけでなく，
* 「何歩で来たか」も状態にもつことで状態遷移グラフを DAG にしている．
* verify : https://atcoder.jp/contests/abc244/tasks/abc244_e
* 
* (b) の例：
* 操作回数の最小値を求めよとか，コストを最小化せよとかがあれば疑うべき．
* verify : https://atcoder.jp/contests/abc244/tasks/abc244_f
*/


//【ディルワースの定理】
/*
* DAG g の最小パス被覆の大きさは，最大反鎖の大きさに一致する．
*	パス被覆：パスの集合で，g の任意の頂点がいずれかのパスに属するもの．
*	反鎖：頂点の集合で，任意の頂点間を結ぶパスが存在しないもの．
* 
* verify : https://atcoder.jp/contests/abc134/tasks/abc134_e
*/


//【パスの個数】O(|V| + |E|)
/*
* DAG g の頂点 s から gl までのパスの個数を cnt[s] に格納する．
*
*（DAG 上の DP）
*/
void count_path(const Graph& g, int gl, vm& cnt) {
	// verify : https://atcoder.jp/contests/tenka1-2014-qualb/tasks/tenka1_2014_qualB_b

	int n = sz(g);

	// cnt[s] : 頂点 s からのパスの個数
	cnt.resize(n);
	vb seen(n);

	cnt[gl] = 1;
	seen[gl] = true;

	function<mint(int)> dfs = [&](int s) {
		if (seen[s]) return cnt[s];
		seen[s] = true;

		// s → t と進む場合
		repe(t, g[s]) cnt[s] += dfs(t);

		return cnt[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);
}


//【パスの個数】O(|V| + |E|)
/*
* DAG g の頂点 s からのパス（不動も可）の個数を cnt[s] に格納する．
*
*（DAG 上の DP）
*/
void count_all_path(const Graph& g, vm& cnt) {
	int n = sz(g);

	// cnt[s] : 頂点 s からのパスの個数
	cnt.resize(n);
	vb seen(n);

	function<mint(int)> dfs = [&](int s) {
		if (seen[s]) return cnt[s];
		seen[s] = true;

		cnt[s] = 1; // 不動の場合に対応

		repe(t, g[s]) {
			// s → t と進む場合
			cnt[s] += dfs(t);
		}

		return cnt[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);
}


//【最長パス】O(|V| + |E|)
/*
* DAG g の頂点 s からの最長パスの長さを len[s] に格納する．
*
*（DAG 上の DP）
*/
void longest_path(const Graph& g, vi& len) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_g

	int n = sz(g);

	// len[s] : 頂点 s からの最長パスの長さ
	len.resize(n);
	vb seen(n);

	function<int(int)> dfs = [&](int s) {
		if (seen[s]) return len[s];
		seen[s] = true;
		len[s] = 0;
		
		repe(t, g[s]) {
			// s → t と進む場合
			chmax(len[s], dfs(t) + 1);
		}

		return len[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) {
		if (!seen[s]) dfs(s);
	}
}


//【コスト最大パス（頂点コスト，始点任意）】O(|V| + |E|)
/*
* 頂点コスト w の与えられた DAG g のパス（長さ 0 も可）で，
* パスに属する頂点のコストの和の最大値を返す．
*
*（DAG 上の DP）
*/
ll highest_cost_path(const Graph& g, const vl& w) {
	int n = sz(g);

	// dp[s] : 頂点 s からの最大コスト
	vl dp(n);
	vb seen(n);

	function<ll(int)> dfs = [&](int s) {
		if (seen[s]) return dp[s];
		seen[s] = true;
		dp[s] = 0;
		
		repe(t, g[s]) {
			// s → t と進む場合
			chmax(dp[s], dfs(t));
		}
		dp[s] += w[s];

		return dp[s];
	};

	// 各頂点 s についての情報を計算する．
	ll res = 0;
	rep(s, n) chmax(res, dfs(s));

	return res;
}


//【コスト最大パス（頂点コスト，始点固定）】O(|V| + |E|)
/*
* 頂点コスト w の与えられた DAG g の r からのパス（長さ 0 も可）で，
* パスに属する頂点のコストの和を最大とするパスの頂点列を path に格納する．
* またそのパスのコストを返す．
*
*（DAG 上の DP）
*/
ll highest_cost_path(const Graph& g, const vl& w, int r, vi* path = nullptr) {
	int n = sz(g);

	// dp[s] : 頂点 s からの最大コスト
	vl dp(n);
	vb seen(n);
	vi next(n, -1);

	function<ll(int)> dfs = [&](int s) {
		if (seen[s]) return dp[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		dp[s] = 0;
		repe(t, g[s]) {
			if (chmax(dp[s], dfs(t))) {
				next[s] = t;
			}
		}
		dp[s] += w[s];

		return dp[s];
	};

	// r から探索
	ll res = dfs(r);

	// DP 復元
	if (path != nullptr) {
		path->clear();

		for (int s = r; s != -1; s = next[s]) {
			path->push_back(s);
		}
	}

	return res;
}


//【コスト最大パスの組（頂点コスト，始点任意）】O(|V|^3)
/*
* 頂点コスト w の与えられた DAG g（トポロジカルソート済）のパスの組で，
* いずれかのパスに属している頂点のコストの和の最大値を返す．
*
*（DAG 上の二次元 DP）
*
* 利用：【幅優先探索】
*/
ll highest_cost_twinpath(const Graph& g_, const vl& w_) {
	// 参考 : https://suikaba.hatenablog.com/entry/2017/08/26/172626
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph

	int n = sz(g_);

	// 全頂点への有向辺をもちコストが 0 の頂点 0 を追加する．
	Graph g(n + 1);
	repi(t, 1, n) {
		g[0].push_back(t);
	}
	rep(s, n) {
		repe(t, g_[s]) {
			g[s + 1].push_back(t + 1);
		}
	}

	vl w(n + 1);
	w[0] = 0;
	rep(s, n) {
		w[s + 1] = w_[s];
	}

	n++;

	// downQ[s][t] : パス s → t が存在するか（いくつかの頂点を飛び越えて移動できるか）
	vvb downQ(n, vb(n));
	rep(s, n) {
		vi dist;
		breadth_first_search(g, s, dist);

		rep(t, n) {
			downQ[s][t] = (dist[t] < INF);
		}
	}

	// dp[s1][s2] : 頂点 s1 < s2 からのパスの組の最大コスト
	vvl dp(n, vl(n));
	vvb seen(n, vb(n));

	function<ll(int, int)> dfs = [&](int s1, int s2) {
		if (seen[s1][s2]) return dp[s1][s2];
		seen[s1][s2] = true;

		dp[s1][s2] = w[s1] + w[s2];

		// s2 から行ける頂点 t2 の情報を元に (s1, s2) の情報を計算する．
		repe(t2, g[s2]) {
			chmax(dp[s1][s2], dfs(s1, t2) + w[s2]);
		}

		// s1 から行ける頂点 t1 の情報を元に (s1, s2) の情報を計算する．
		// ただし s1 からは s2 を飛び越えるような移動しか認めないこととする．
		repi(t1, s2 + 1, n - 1) {
			if (downQ[s1][t1]) {
				chmax(dp[s1][s2], dfs(s2, t1) + w[s1]);
			}
		}

		return dp[s1][s2];
	};

	dfs(0, 0);

	ll res = 0;
	rep(s2, n) {
		rep(s1, s2) {
			chmax(res, dp[s1][s2]);
		}
	}
	return res;
}


//【最小パス被覆】O( min(|V|^(2/3) (|V| + |E|), (|V| + |E|)^(3/2)) )
/*
* DAG g の最小パス被覆の大きさを返す．
* 
* 戻り値は反鎖（互いに行き来できない頂点集合）の大きさの最大値とも解釈できる．
*
* 利用：【二部グラフの最大マッチング】
*/
int minimum_path_cover(const Graph& g, vvi* paths = nullptr) {
	// 参考：https://kyopro.hateblo.jp/entry/2018/06/04/000659

	int n = sz(g);

	Bipartite_matching bm(n, n);
	rep(s, n) {
		repe(t, g[s]) {
			bm.add_edge(s, t);
		}
	}

	int res = n - bm.flow();

	if (paths == nullptr) return res;

	vector<pii> es;
	bm.maximum_matching(es);

	dsu uf(n);
	repe(e, es) {
		int s, t;
		tie(s, t) = e;

		uf.merge(s, t);
	}

	*paths = uf.groups();

	return res;
}


