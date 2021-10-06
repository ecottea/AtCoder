#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "グラフ最適化.h"
// ■■■■■ 木 ■■■■■



//【木のオイラーツアー】O(|V|)
/*
* 頂点 r を始点とする木 g のオイラーツアーを求める．
*
* in[v] : 最初に頂点 v を訪れた時刻（r なら 0）
* out[v] : 最後に頂点 v から離れた時刻（r なら 2 |V| - 1）
* pos[t] : 時刻 t に訪れた頂点の番号（長さ 2 |V| - 1）
*/
template <class G>
void euler_tour(G& g, int r, vi& in, vi& out, vi& pos) {
	// 参考 : https://qiita.com/recuraki/items/72e37eb9be9f71bc623a

	int n = sz(g);

	int time = 0;
	in = vi(n);
	out = vi(n);
	pos = vi(2 * n - 1);

	// 再帰用の関数
	function<void(int, int)> rf = [&](int s, int p) {
		// s を最初に訪れた
		in[s] = time;
		pos[time++] = s;

		for (auto t : g[s]) {
			// 親には戻らない．
			if (t == p) {
				continue;
			}

			rf(t, s);
			pos[time++] = s;
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(r, -1);
}


//【木の直径】O(|E| log|V|)
/*
* コスト付き木の直径を返す．
* 直径の両端となる頂点の組を p に格納する．
*
* 利用：【単一始点最短路／ダイクストラ法】
*/
ll tree_diameter(const WGraph& g, pii& p) {
	int n = sz(g);

	// 適当な頂点を始点にして最遠の点 s を求める．
	vl dist;
	dijkstra(g, 0, dist);

	ll max_dist = -1;
	int s;
	rep(i, n) {
		if (chmax(max_dist, dist[i])) {
			s = i;
		}
	}

	// s を始点にして最遠の点 t を求めれば，|s t| が木の直径である．
	max_dist = -1;
	int t;
	dijkstra(g, s, dist);
	rep(i, n) {
		if (chmax(max_dist, dist[i])) {
			t = i;
		}
	}

	p = { s, t };
	return max_dist;
}


//【直径とその中点】O(n)
/*
* 木の直径の両端点を ep = {s, t} に，経路 s → t の中点を ctr に格納する．
* 中点が頂点 v のときは ctr = {v, v}，辺 es → et 上のときは ctr = {es, et} とする．
* また直径の長さを返す．
*
* 利用：【幅優先探索】
*/
int tree_diameter_and_midpoint(const Graph& g, pii& ep, pii& ctr) {
	int n = sz(g);

	// 頂点 0 から幅優先探索を行う．
	vi dist;
	breadth_first_search(g, 0, dist);

	// 頂点 0 からの距離が最も遠い点 ep0 を見つける．
	int d = 0, ep0;
	rep(i, n) {
		if (chmax(d, dist[i])) ep0 = i;
	}

	dist = vi(n, -1); // スタートからの最短距離を保持するテーブル
	dist[ep0] = 0;
	vi parent(n); // 直前に通ってきた頂点（経路復元用）
	parent[ep0] = -1;
	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(ep0);

	// 頂点 ep0 から幅優先探索を行う．
	while (!que.empty()) {
		auto s = que.front();
		que.pop();

		for (auto t : g[s]) {
			if (dist[t] != -1) continue;

			dist[t] = dist[s] + 1;
			parent[t] = s;

			que.push(t);
		}
	}

	// 頂点 ep0 からの距離が最も遠い点 ep1 を見つける．
	d = 0; int ep1;
	rep(i, n) {
		if (chmax(d, dist[i])) ep1 = i;
	}

	// 直径の中点を得る．
	int c0, c1 = ep1;
	rep(i, d / 2) c1 = parent[c1];
	c0 = (d % 2 == 0 ? c1 : parent[c1]);

	// 結果の格納
	ep = { ep0, ep1 };
	ctr = { c0, c1 };

	return d;
}


//【独立集合の数え上げ】O(|V|)
/*
* 木 g の独立集合（辺を共有しない頂点の集合）の個数を返す．
*
*（木の状態 DP）
*/
mint count_independent_set(Graph& g) {
	int n = sz(g);

	// s : 注目頂点，c : s が独立集合に含まれるか，p : 親頂点
	// 戻り値 : 部分木 s に含まれる独立集合の個数（便宜上 0 を根とした根付き木とみなす）
	function<mint(int, int, int)> dfs = [&](int s, int c, int p) {
		// 積についての単位元で初期化しておく．
		mint res = 1;

		// s のそれぞれの子 t について処理を行う．
		for (auto t : g[s]) {
			// 親には戻らない
			if (t == p) continue;
			
			// t が独立集合に含まれないような部分木は候補になる．
			mint mul = dfs(t, 0, s);
			if (!c) {
				// s が独立集合に含まれないならば，
				// t が独立集合に含まれるような部分木も候補になる．
				mul += dfs(t, 1, s);
			}

			// s の部分木については独立なので，積の法則で数え上げる．
			res *= mul;
		}

		return res;
	};

	return dfs(0, 0, -1) + dfs(0, 1, -1);
}


//【部分木の数え上げ】O(|V|)
/*
* 木 g の部分木のうち頂点 i を含むものの個数を cnt[i] に格納する．
*
*（全方位木 DP）
*/
void count_subtree(Graph& g, vm& cnt) {
	int n = sz(g);

	// 辺 (p, s) を切断したときの s を含む部分木を部分木 (p, s) と呼ぶ．
	// dp[{p, s}] : 部分木 (p, s) の s を含む部分木の個数
	map<pii, mint> dp;

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[{p, s}] を計算する．
	function<mint(int, int)> dfs_to_leaf = [&](int p, int s) {
		dp[{p, s}] = 1;

		// 子の情報を集めて積をとり，自身の情報を計算する．
		for (auto t : g[s]) {
			if (t != p) {
				dp[{p, s}] *= dfs_to_leaf(s, t) + 1;
			}
		}

		return dp[{p, s}];
	};

	// 頂点 0 を根とし，根の方向に向かってのみの dp[{s, p}] を計算する．
	// また特に方向を持たない dp[{s, s}] も計算する．
	function<void(int, int)> dfs_to_root = [&](int p, int s) {
		// s から出ている辺の本数
		int m = sz(g[s]);

		// 左右からの累積積を計算する．
		vector<mint> acc_l(m + 1), acc_r(m + 1);
		acc_l[0] = 1;
		for (int i = 0; i < m; i++) {
			int t = g[s][i];
			acc_l[i + 1] = acc_l[i] * (dp[{s, t}] + 1);
		}
		acc_r[m] = 1;
		for (int i = m - 1; i >= 0; i--) {
			int t = g[s][i];
			acc_r[i] = acc_r[i + 1] * (dp[{s, t}] + 1);
		}

		// 左右からの累積積を用いて 1 つ抜きの積を計算する．
		rep(i, m) {
			int t = g[s][i];
			dp[{t, s}] = acc_l[i] * acc_r[i + 1];
		}

		// 総積も記録しておく．
		dp[{s, s}] = acc_l[m];

		// これで子から自身への情報が計算できたので，
		// 子に対して同様の計算を行っていく．
		for (auto t : g[s]) {
			if (t != p) {
				dfs_to_root(s, t);
			}
		}
	};

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[{p, s}] を計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_leaf(0, 0);

	// 頂点 0 を根とし，根の方向に向かってのみの dp[{p, s}] を計算する．
	// また特に方向を持たない dp[{s, s}] も計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_root(0, 0);

	cnt = vector<mint>(n);
	rep(s, n) {
		cnt[s] = dp[{s, s}];
	}
}


//【木の高さ】O(|V|)
/*
* 木 g の頂点 i を根にしたときの高さを h[i] に格納する．
*
*（全方位木 DP）
*/
void height_of_undirected_tree(Graph& g, vl& h) {
	int n = sz(g);

	// 辺 (p, s) を切断したときの s を根とする部分木を部分木 (p, s) と呼ぶ．
	// dp[{p, s}] : 部分木 (p, s) の高さ
	map<pii, ll> dp;

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[{p, s}] を計算する．
	function<ll(int, int)> dfs_to_leaf = [&](int p, int s) {
		// 子の情報を集めてその最大値をとり，自身の情報を計算する．
		for (auto t : g[s]) {
			if (t != p) {
				chmax(dp[{p, s}], dfs_to_leaf(s, t) + 1);
			}
		}

		return dp[{p, s}];
	};

	// 頂点 0 を根とし，根の方向に向かってのみの dp[{s, p}] を計算する．
	// また特に方向を持たない dp[{s, s}] も計算する．
	function<void(int, int)> dfs_to_root = [&](int p, int s) {
		// s から出ている辺の本数
		int m = sz(g[s]);

		// 左右からの累積最大値を計算する．
		vl acc_l(m + 1), acc_r(m + 1);
		for (int i = 0; i < m; i++) {
			auto t = g[s][i];
			acc_l[i + 1] = max(acc_l[i], dp[{s, t}] + 1);
		}
		for (int i = m - 1; i >= 0; i--) {
			auto t = g[s][i];
			acc_r[i] = max(acc_r[i + 1], dp[{s, t}] + 1);
		}

		// 左右からの累積最大値を用いて 1 つ抜きの最大値を計算する．
		rep(i, m) {
			int t = g[s][i];
			dp[{t, s}] = max(acc_l[i], acc_r[i + 1]);
		}

		// 総最大値も記録しておく．
		dp[{s, s}] = acc_l[m];

		// これで子から自身への情報が計算できたので，
		// 子に対して同様の計算を行っていく．
		for (auto t : g[s]) {
			if (t != p) {
				dfs_to_root(s, t);
			}
		}
	};

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[{p, s}] を計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_leaf(0, 0);

	// 頂点 0 を根とし，根の方向に向かってのみの dp[{p, s}] を計算する．
	// また特に方向を持たない dp[{s, s}] も計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_root(0, 0);

	h = vl(n);
	rep(s, n) {
		h[s] = dp[{s, s}];
	}
}


//【コスト付き木の高さ】O(|V|)
/*
* コスト付き木 g の頂点 i を根にしたときの高さを h[i] に格納する．
*
*（全方位木 DP）
*/
void height_of_undirected_tree(WGraph& g, vl& h) {
	int n = sz(g);

	// 辺 (p, s) を切断したときの s を根とする部分木を部分木 (p, s) と呼ぶ．
	// dp[{p, s}] : 部分木 (p, s) の高さ
	map<pii, ll> dp;

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[{p, s}] を計算する．
	function<ll(int, int)> dfs_to_leaf = [&](int p, int s) {
		// 子の情報を集めてその最大値をとり，自身の情報を計算する．
		for (auto e : g[s]) {
			if (e.to != p) {
				chmax(dp[{p, s}], dfs_to_leaf(s, e.to) + e.cost);
			}
		}

		return dp[{p, s}];
	};

	// 頂点 0 を根とし，根の方向に向かってのみの dp[{s, p}] を計算する．
	// また特に方向を持たない dp[{s, s}] も計算する．
	function<void(int, int)> dfs_to_root = [&](int p, int s) {
		// s から出ている辺の本数
		int m = sz(g[s]);

		// 左右からの累積最大値を計算する．
		vl acc_l(m + 1), acc_r(m + 1);
		for (int i = 0; i < m; i++) {
			auto e = g[s][i];
			acc_l[i + 1] = max(acc_l[i], dp[{s, e.to}] + e.cost);
		}
		for (int i = m - 1; i >= 0; i--) {
			auto e = g[s][i];
			acc_r[i] = max(acc_r[i + 1], dp[{s, e.to}] + e.cost);
		}

		// 左右からの累積最大値を用いて 1 つ抜きの最大値を計算する．
		rep(i, m) {
			int t = g[s][i].to;
			dp[{t, s}] = max(acc_l[i], acc_r[i + 1]);
		}

		// 総最大値も記録しておく．
		dp[{s, s}] = acc_l[m];

		// これで子から自身への情報が計算できたので，
		// 子に対して同様の計算を行っていく．
		for (auto e : g[s]) {
			if (e.to != p) {
				dfs_to_root(s, e.to);
			}
		}
	};

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[{p, s}] を計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_leaf(0, 0);

	// 頂点 0 を根とし，根の方向に向かってのみの dp[{p, s}] を計算する．
	// また特に方向を持たない dp[{s, s}] も計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_root(0, 0);

	h = vl(n);
	rep(s, n) {
		h[s] = dp[{s, s}];
	}
}


