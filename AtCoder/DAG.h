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


//【DAG 上 DP とスコア和】
/*
* DAG g の ST から GL までの長さ k のパスの数を c(k) とおく．
* 全ての k に対して c(k) を求めるのが間に合わなくても，以下の形なら O(|V|) で値が求まる：
*	Σk c(k)		: パスの総数を求める DP
*	Σk c(k) a^k	: 上の DP において遷移の度に値を a 倍
* 
* verify : https://atcoder.jp/contests/diverta2019-2/tasks/diverta2019_2_e
*/


//【ディルワースの定理】
/*
* DAG g の最小パス被覆の大きさは，最大反鎖の大きさに一致する．
*	パス被覆：パスの集合で，g の任意の頂点がいずれかのパスに属するもの．
*	反鎖：頂点の集合で，任意の頂点間を結ぶパスが存在しないもの．
* 
* verify : https://atcoder.jp/contests/abc134/tasks/abc134_e
*/


//【コスト最小パス】O(|V| + |E|)
/*
* DAG g の頂点 s から gl までのコスト最小パスのコストを dist[s] に格納する．
*/
void lowest_cost_path(const WGraph& g, int gl, vl& dist) {
	int n = sz(g);

	// dist[s] : 頂点 s から gl までのコスト最小パスのコスト
	dist.resize(n, INFL);
	vb seen(n);

	dist[gl] = 0;
	seen[gl] = true;

	function<ll(int)> dfs = [&](int s) {
		if (seen[s]) return dist[s];
		seen[s] = true;

		// s → t と進む場合
		repe(t, g[s]) chmin(dist[s], dfs(t) + t.cost);

		return dist[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);
}


//【パスの個数】O(|V| + |E|)
/*
* DAG g の頂点 s から gl までのパスの個数を cnt[s] に格納する．
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

		// s → t と進む場合
		repe(t, g[s]) cnt[s] += dfs(t);

		return cnt[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);
}


//【最長パス】O(|V| + |E|)
/*
* DAG g の頂点 s からの最長パスの長さを len[s] に格納する．
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
		
		// s → t と進む場合
		repe(t, g[s]) chmax(len[s], dfs(t) + 1);

		return len[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) if (!seen[s]) dfs(s);
}


//【スコア最大パス】O(|V| + |E|)
/*
* コスト付き DAG g の頂点 s からのスコア最大パスのスコアを sc[s] に格納する．
*/
void highest_score_path(const WGraph& g, vl& sc) {
	// verify : https://atcoder.jp/contests/code-festival-2017-qualb/tasks/code_festival_2017_qualb_d

	int n = sz(g);

	// sc[s] : 頂点 s からのスコア最大パスのスコア
	sc.resize(n);
	vb seen(n);

	function<ll(int)> dfs = [&](int s) {
		if (seen[s]) return sc[s];
		seen[s] = true;
		sc[s] = 0;

		// s → t と進む場合
		repe(t, g[s]) chmax(sc[s], dfs(t) + t.cost);

		return sc[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) if (!seen[s]) dfs(s);
}


//【スコア最大パス（頂点スコア）】O(|V| + |E|)
/*
* 頂点 i に非負スコア w[i] の与えられた DAG g のパス（長さ 0 も可）で，
* 頂点 s からのパスの最大スコアを sc[s] に格納する．
*/
void highest_score_path(const Graph& g, const vl& w, vl& sc) {
	int n = sz(g);

	// sc[s] : 頂点 s からのパスの最大スコア
	sc = vl(n, 0);
	vb seen(n);

	function<ll(int)> dfs = [&](int s) {
		if (seen[s]) return sc[s];
		seen[s] = true;
		
		// s → t と進む場合
		repe(t, g[s]) chmax(sc[s], dfs(t));

		sc[s] += w[s];

		return sc[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);
}


//【スコア最大パス（頂点スコア，復元）】O(|V| + |E|)
/*
* 頂点 i に非負スコア w[i] の与えられた DAG g の r からのパス（長さ 0 も可）で，
* パスに属する頂点のスコアの和を最大とするパスの頂点列を path に格納する．
* またそのパスのスコアを返す．
*/
ll highest_score_path(const Graph& g, const vl& w, int r, vi* path = nullptr) {
	int n = sz(g);

	// dp[s] : 頂点 s からの最大スコア
	vl dp(n);
	vb seen(n);
	vi next(n, -1);

	function<ll(int)> dfs = [&](int s) {
		if (seen[s]) return dp[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		dp[s] = 0;
		repe(t, g[s]) {
			if (chmax(dp[s], dfs(t))) next[s] = t;
		}
		dp[s] += w[s];

		return dp[s];
	};

	// r から探索
	ll res = dfs(r);

	// DP 復元
	if (path != nullptr) {
		path->clear();

		for (int s = r; s != -1; s = next[s]) path->push_back(s);
	}

	return res;
}


//【スコア最大パスの組（頂点スコア，始点任意）】O(|V|^3)
/*
* 頂点 i に非負スコア w[i] の与えられた DAG g（トポロジカルソート済）のパスの組で，
* いずれかのパスに属している頂点のスコアの和の最大値を返す．
*
*（DAG 上の二次元 DP）
*
* 利用：【幅優先探索】
*/
ll highest_score_twinpath(const Graph& g_, const vl& w_) {
	// 参考 : https://suikaba.hatenablog.com/entry/2017/08/26/172626
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph

	int n = sz(g_);

	// 全頂点への有向辺をもちスコアが 0 の頂点 0 を追加する．
	Graph g(n + 1);
	repi(t, 1, n) g[0].push_back(t);
	rep(s, n) repe(t, g_[s]) g[s + 1].push_back(t + 1);

	vl w(n + 1);
	w[0] = 0;
	rep(s, n) w[s + 1] = w_[s];

	n++;

	// downQ[s][t] : パス s → t が存在するか（いくつかの頂点を飛び越えて移動できるか）
	vvb downQ(n, vb(n));
	rep(s, n) {
		vi dist;
		breadth_first_search(g, s, dist);

		rep(t, n) downQ[s][t] = (dist[t] < INF);
	}

	// dp[s1][s2] : 頂点 s1 < s2 からのパスの組の最大スコア
	vvl dp(n, vl(n));
	vvb seen(n, vb(n));

	function<ll(int, int)> dfs = [&](int s1, int s2) {
		if (seen[s1][s2]) return dp[s1][s2];
		seen[s1][s2] = true;

		dp[s1][s2] = w[s1] + w[s2];

		// s2 から行ける頂点 t2 の情報を元に (s1, s2) の情報を計算する．
		repe(t2, g[s2]) chmax(dp[s1][s2], dfs(s1, t2) + w[s2]);

		// s1 から行ける頂点 t1 の情報を元に (s1, s2) の情報を計算する．
		// ただし s1 からは s2 を飛び越えるような移動しか認めないこととする．
		repi(t1, s2 + 1, n - 1) {
			if (downQ[s1][t1]) chmax(dp[s1][s2], dfs(s2, t1) + w[s1]);
		}

		return dp[s1][s2];
	};

	dfs(0, 0);

	ll res = 0;
	rep(s2, n) rep(s1, s2) chmax(res, dp[s1][s2]);

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
	// 参考 : https://kyopro.hateblo.jp/entry/2018/06/04/000659

	int n = sz(g);

	Bipartite_matching bm(n, n);
	rep(s, n) repe(t, g[s]) bm.add_edge(s, t);

	int res = n - bm.flow();

	if (paths == nullptr) return res;

	vector<pii> es = bm.maximum_matching();

	dsu uf(n);
	repe(e, es) {
		int s, t;
		tie(s, t) = e;

		uf.merge(s, t);
	}

	*paths = uf.groups();

	return res;
}


