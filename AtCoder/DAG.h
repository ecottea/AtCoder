#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "最短路.h"
#include "マッチング(二部).h"
#include "フロー.h"
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
* 全ての k に対して c(k) を求めるのが間に合わなくても，以下の形なら O(n) で値が求まる：
*	Σk c(k)		: パスの総数を求める DP
*	Σk c(k) a^k	: 上の DP において遷移の度に値を a 倍
* 
* verify : https://atcoder.jp/contests/diverta2019-2/tasks/diverta2019_2_e
*/


//【コスト最小パス】O(n + m)
/*
* DAG g の各頂点から gl までのコスト最小パスのコストを格納したリストを返す．
*/
vl lowest_cost_path(const WGraph& g, int gl) {
	int n = sz(g);

	// dist[s] : 頂点 s から gl までのコスト最小パスのコスト
	vl dist(n, INFL);
	vb seen(n);

	dist[gl] = 0;
	seen[gl] = true;

	// 貰う DP
	function<ll(int)> dfs = [&](int s) {
		if (seen[s]) return dist[s];
		seen[s] = true;

		// s → t と進む場合
		repe(t, g[s]) chmin(dist[s], dfs(t) + t.cost);

		return dist[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);

	return dist;
}


//【パスの個数】O(n + m)
/*
* DAG g の各頂点から gl までのパスの個数を格納したリストを返す．
*/
template <class T>
vector<T> count_path(const Graph& g, int gl) {
	// verify : https://atcoder.jp/contests/tenka1-2014-qualb/tasks/tenka1_2014_qualB_b

	int n = sz(g);

	// cnt[s] : 頂点 s からのパスの個数
	vector<T> cnt(n); vb seen(n);
	cnt[gl] = 1; seen[gl] = true;

	// 貰う DP
	function<T(int)> dfs = [&](int s) {
		if (seen[s]) return cnt[s];
		seen[s] = true;

		// s → t と進む場合
		repe(t, g[s]) cnt[s] += dfs(t);

		return cnt[s];
	};

	// 各頂点 s についての情報を計算する．
	rep(s, n) dfs(s);

	return cnt;
}


//【最長パス】O(n + m)
/*
* DAG g の各頂点からの最長パスの長さを格納したリストを返す．
*/
vi longest_path(const Graph& g) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_g

	int n = sz(g);

	// len[s] : 頂点 s からの最長パスの長さ
	vi len(n); vb seen(n);

	// 貰う DP
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

	return len;
}


//【スコア最大パス】O(n + m)
/*
* 重み付き DAG g の各頂点からのパスの最大スコアを格納したリストを返す．
*/
vl highest_score_path(const WGraph& g) {
	// verify : https://atcoder.jp/contests/code-festival-2017-qualb/tasks/code_festival_2017_qualb_d

	int n = sz(g);

	// sc[s] : 頂点 s からのパスの最大スコア
	vl sc(n); vb seen(n);

	// 貰う DP
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

	return sc;
}


//【スコア最大パス（頂点スコア）】O(n + m)
/*
* 頂点 i にスコア w[i] の与えられた DAG g のパス（長さ 0 も可）で，
* 各頂点からのパスの最大スコアを格納したリストを返す．
*/
vl highest_score_path(const Graph& g, const vl& w) {
	int n = sz(g);

	// sc[s] : 頂点 s からのパスの最大スコア
	vl sc(n, 0); vb seen(n);

	// 貰う DP
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

	return sc;
}


//【スコア最大パス（頂点スコア，復元）】O(n + m)
/*
* 頂点 i にスコア w[i] の与えられた DAG g の r からのパス（長さ 0 も可）の
* 最大スコアを返し，パスに属する頂点列を path に格納する．
*/
ll highest_score_path(const Graph& g, const vl& w, int r, vi* path = nullptr) {
	int n = sz(g);

	// dp[s] : 頂点 s からの最大スコア
	vl dp(n); vb seen(n); vi next(n, -1);

	// 貰う DP
	function<ll(int)> dfs = [&](int s) {
		if (seen[s]) return dp[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		repe(t, g[s]) if (chmax(dp[s], dfs(t))) next[s] = t;

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


//【スコア最大パスの組（頂点スコア）】O(n^3)
/*
* 頂点 i に非負スコア w[i] の与えられた DAG g（トポロジカルソート済）の頂点 0 からのパスの組で，
* いずれかのパスに属している頂点のスコアの和の最大値を返す．
*
*（DAG 上の二次元 DP）
*
* 利用：【幅優先探索】
*/
ll highest_score_twinpath(const Graph& g, const vl& w) {
	// 参考 : https://suikaba.hatenablog.com/entry/2017/08/26/172626
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph

	int n = sz(g);

	// downQ[s][t] : パス s → t が存在するか（いくつかの頂点を飛び越えて移動できるか）
	vvb downQ(n, vb(n));
	rep(s, n) {
		vi dist = breadth_first_search(g, s);

		rep(t, n) downQ[s][t] = (dist[t] < INF);
	}

	// dp[s1][s2] : 頂点 s1 < s2 からのパスの組の最大スコア
	vvl dp(n, vl(n)); vvb seen(n, vb(n));

	function<ll(int, int)> dfs = [&](int s1, int s2) {
		if (seen[s1][s2]) return dp[s1][s2];
		seen[s1][s2] = true;

		// ともに不動の場合
		dp[s1][s2] = w[s1] + w[s2];

		// s2 から行ける頂点 t2 の情報を元に (s1, s2) の情報を計算する．
		repe(t2, g[s2]) chmax(dp[s1][s2], dfs(s1, t2) + w[s2]);

		// s1 から行ける頂点 t1 の情報を元に (s1, s2) の情報を計算する．
		// ただし s1 からは s2 を飛び越えるような移動しか認めないこととして二重カウントを防ぐ．
		repi(t1, s2 + 1, n - 1) {
			if (downQ[s1][t1]) chmax(dp[s1][s2], dfs(s2, t1) + w[s1]);
		}

		return dp[s1][s2];
	};
	dfs(0, 0);
	dumpel(dp);

	ll res = 0;
	rep(s1, n) repi(s2, s1 + 1, n - 1) chmax(res, dp[s1][s2]);

	return res;
}


//【到達可能性（DAG）】O((n + m) q / 64)
/*
* 与えられた DAG g に対し，各 j∈[0..q) について，
* 頂点 u[j] から頂点 v[j] に到達可能かを格納したリストを返す．
*/
vb reachability_DAG(const Graph& g, const vi& u, const vi& v) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_bg

	//【方法】
	// bool 値をもつ素朴な配る DP を考え，それをクエリをまとめて扱うことで 64 倍高速化する．

	//【注意】
	// いっそのこと bitset で n 個まとめてやれば良さそうだがそれだと MLE の危険がある．

	int n = sz(g), q = sz(u);
	vb res(q);

	int j = 0;
	while (j < q) {
		// dp[i] :「頂点 i に到達可能か」を 64 個まとめたもの
		vector<ull> dp(n);

		// 初期化（初期位置 u[j] を 1 にする）
		int b = 0;
		for (; b < 64; ) {
			dp[u[j]] += 1ULL << (b++);
			if (++j == q) break;
		}

		// 配る DP をビット演算で 64 倍高速化する．
		rep(s, n) repe(t, g[s]) dp[t] |= dp[s];

		// 結果の格納
		rep(b2, b) {
			int j2 = j - (b - b2);
			res[j2] = get(dp[v[j2]], b2);
		}
	}

	return res;
}


//【到達可能な頂点数】
/*
* DAG g の各頂点から到達可能な頂点の個数を一括で求める問題を効率良く解く方法は知られていない．
* 
* 参考 : https://twitter.com/noshi91/status/1633031948938645505
*/


//【Dilworth の定理】
/*
*（推移的とは限らない）DAG g の最小パス被覆の大きさは，最大反鎖の大きさに一致する．
*	パス被覆：パスの集合で，g の任意の頂点がいずれかのパスに属するもの．
*	反鎖：頂点の集合で，任意の頂点間を結ぶパスが存在しないもの．
*
* verify : https://atcoder.jp/contests/abc237/tasks/abc237_h
*/


//【最小パス被覆（点素）】O( min(n^(2/3) (n + m), (n + m)^(3/2)) )（m : 辺の数）
/*
* DAG g の点素なパス被覆で最小のものの大きさを返す． 
*
* 利用：【二部グラフの最大マッチング】
*/
int minimum_disjoint_path_cover(const Graph& g, vvi* paths = nullptr) {
	// 参考 : https://kyopro.hateblo.jp/entry/2018/06/04/000659
	// verify : https://atcoder.jp/contests/abc237/tasks/abc237_h

	int n = sz(g);

	// S = V_out, T = V_in とする二部グラフ G=(S,T) を構築する．
	Bipartite_matching bm(n, n);
	rep(s, n) repe(t, g[s]) bm.add_edge(s, t);

	// G の最大マッチングの大きさは，g の点素なパス被覆で最小のものの大きさに等しい．
	int res = n - bm.solve();
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


//【最小パス被覆】O(n (n + m) log n) ?（m : 辺の数）
/*
* DAG g のパス被覆で最小のものの大きさを返す．
*
* 利用：【一般化最小費用流】
*/
int minimum_path_cover(const Graph& g) {
	// verify : https://atcoder.jp/contests/abc237/tasks/abc237_h

	int n = sz(g);

	// 頂点 v を (v_in=2v, v_out=2v+1) と倍化し，始点 ST を導入する．
	int ST = n * 2;
	Generalized_min_cost_flow mcf(ST + 1);

	rep(i, n) {
		// ST から各頂点 v_in にコスト 1 の辺を張る（パスを引く度にコスト 1）
		mcf.add_cost_edge(ST, 2 * i, INFL, 1);

		// 各頂点の v_in から v_out に最小流量 1 の辺を張る（全ての頂点を被覆しなければならない）
		mcf.add_cost_edge(2 * i, 2 * i + 1, 1, INFL, 0);

		// 各頂点から ST に辺を張る（流量は任意）
		mcf.add_cost_edge(2 * i + 1, ST, INFL, 0);
	}

	// g に辺 s→t が存在するときに限り辺 s_out → t_in を張る（パスがここを通ることを許す）
	rep(s, n) repe(t, g[s]) mcf.add_cost_edge(2 * s + 1, 2 * t, INFL, 0);

	return (int)mcf.flow().second;
}


//【推移的 DAG の最小パス被覆】
/*
* DAG g が推移的（辺 s→t, t→u がある ⇒ 辺 s→u がある）ならば，
* 点素な最小パス被覆の大きさと（点素とは限らない）最小パス被覆の大きさは等しい．
* 
* verify : https://atcoder.jp/contests/abc237/tasks/abc237_h
*/


//【独立集合の貪欲な選択 ⇔ コマ移動ゲーム】
/*
* DAG g からトポロジカルソートの逆順に貪欲に頂点を選んで構成した独立集合は，
* g 上のコマ移動ゲームにおける後手必勝局面の集合に等しい．
* 
* verify : https://atcoder.jp/contests/agc043/tasks/agc043_c
*/


//【格子 DAG の座標圧縮】
/*
* 座標圧縮.h へ
*/
