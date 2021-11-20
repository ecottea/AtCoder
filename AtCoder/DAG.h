#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "最短路.h"
// ■■■■■ DAG（有向非巡回グラフ） ■■■■■


//【トポロジカルソート】O(|V| + |E|)
/*
* DAG g をトポロジカルソートした結果を seq に返す．
* g が DAG でない場合は失敗し，seq.size() < g.size() となる．
*
*（葉からの幅優先探索）
*/
void topological_sort(const Graph& g, vi& seq) {
	int n = sz(g);

	// 入次数を求めておく．
	vi in_degree(n);
	rep(i, n) {
		repe(t, g[i]) {
			in_degree[t]++;
		}
	}

	// 入次数が 0 の頂点から順に取り除いていく．
	queue<int> q;
	rep(i, n) {
		if (in_degree[i] == 0) {
			q.push(i);
		}
	}

	seq.clear();
	while (!q.empty()) {
		auto s = q.front();
		q.pop();

		// 入次数が 0 の頂点を見つけ結果に格納する．
		seq.push_back(s);

		repe(t, g[s]) {
			// 頂点 s を取り除き，t の入次数を更新する．
			in_degree[t]--;

			// 新たに入次数 0 の頂点が生まれたらキューに追加する．
			if (in_degree[t] == 0) {
				q.push(t);
			}
		}
	}
}


//【強連結成分分解】O(|V| + |E|)
/*
* 有向グラフ g を強連結成分分解し，トポロジカルソートされた結果を scc に返す．
* scc[i] は i 番目の強連結成分の頂点からなるリストである．
*/
void strongly_connected_component(const Graph& g, vvi& scc) {
	// 参考 : https://hkawabata.github.io/technical-note/note/Algorithm/graph/scc.html

	int n = sz(g);

	// 辺の向きを逆にしたグラフを作成
	Graph g_rev(n);
	rep(s, n) {
		repe(t, g[s]) {
			g_rev[t].push_back(s);
		}
	}

	// 各頂点の状態（0:未探索，1:順探索済かつ未逆探索，2:逆探索済）
	vi status(n, 0);


	// step1: まず順探索（深さ優先）を行い，結果をスタックに格納する．

	// 深さ優先の順探索で見つかった順に頂点を記録するスタック
	stack<int> stk;

	// 順探索用の再帰関数
	function<void(int)> trace = [&](int s) {
		// 状態を順探索済かつ未逆探索（1）にする．
		status[s] = 1;

		repe(t, g[s]) {
			// 未探索の頂点を探索しにいく．
			if (status[t] == 0) {
				trace(t);
			}
		}

		// 先の探索が済んだら自身を記録する（深さ優先探索）
		stk.push(s);
	};

	rep(i, n) {
		// 未探索の頂点を見つけたら探索する．
		if (status[i] == 0) {
			trace(i);
		}
	}


	// step2: 次に逆探索を行い，強連結成分を確定する．

	// 逆探索用の再帰関数
	function<void(int)> trace_rev = [&](int s) {
		// 状態を逆探索済（2）にする．
		status[s] = 2;

		repe(t, g_rev[s]) {
			// 未逆探索の頂点を探索しにいく．
			if (status[t] == 1) {
				trace_rev(t);
			}
		}

		// 先の探索が済んだら自身を強連結成分の一員として記録する．
		scc.rbegin()->push_back(s);
	};

	while (!stk.empty()) {
		auto v = stk.top();
		stk.pop();

		// 新しい強連結成分を見つけたらそれをなぞりに行く．
		if (status[v] == 1) {
			scc.push_back(vi());
			trace_rev(v);
		}
	}
}


//【パスの個数】O(|V| + |E|)
/*
* DAG g の頂点 s からのパス（不動も可）の個数を cnt[s] に格納する．
*
*（DAG 上の DP）
*/
void count_path(const Graph& g, vm& cnt) {
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
	rep(s, n) {
		chmax(res, dfs(s));
	}

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
			downQ[s][t] = (dist[t] >= 0);
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


