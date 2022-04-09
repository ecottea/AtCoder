#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "二項係数.h"
// ■■■■■ 全方位木 DP ■■■■■


//【木の高さ】O(n)
/*
* 木 g の頂点 i を根にしたときの高さ（最も遠い葉までの距離）を h[i] に格納する．
*
*（全方位木 DP）
*/
void height_of_undirected_tree(Graph& g, vi& h) {
	ll n = sz(g); // unordered_map<ll, int> を使うので ll にキャストしておく．

	// 辺 (p, s) を切断したときの s を根とする部分木を部分木 (p, s) と呼ぶ．
	// dp[p * n + s] : 部分木 (p, s) の高さ
	unordered_map<ll, int> dp;

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[p * n + s] を計算する．
	function<int(int, int)> dfs_to_leaf = [&](int p, int s) {
		// 子の情報を集めてその最大値をとり，自身の情報を計算する．
		for (auto t : g[s]) {
			if (t != p) {
				chmax(dp[p * n + s], dfs_to_leaf(s, t) + 1);
			}
		}

		return dp[p * n + s];
	};

	// 頂点 0 を根とし，根の方向に向かってのみの dp[s * n + p] を計算する．
	// また特に方向を持たない dp[s * n + s] も計算する．
	function<void(int, int)> dfs_to_root = [&](int p, int s) {
		// s から出ている辺の本数
		int m = sz(g[s]);

		// 左右からの累積最大値を計算する．
		vi acc_l(m + 1), acc_r(m + 1);
		for (int i = 0; i < m; i++) {
			auto t = g[s][i];
			acc_l[i + 1] = max(acc_l[i], dp[s * n + t] + 1);
		}
		for (int i = m - 1; i >= 0; i--) {
			auto t = g[s][i];
			acc_r[i] = max(acc_r[i + 1], dp[s * n + t] + 1);
		}

		// 左右からの累積最大値を用いて 1 つ抜きの最大値を計算する．
		rep(i, m) {
			int t = g[s][i];
			dp[t * n + s] = max(acc_l[i], acc_r[i + 1]);
		}

		// 総最大値も記録しておく．
		dp[s * n + s] = acc_l[m];

		// これで子から自身への情報が計算できたので，
		// 子に対して同様の計算を行っていく．
		for (auto t : g[s]) {
			if (t != p) dfs_to_root(s, t);
		}
	};

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[p * n + s] を計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_leaf(0, 0);

	// 頂点 0 を根とし，根の方向に向かってのみの dp[p * n + s] を計算する．
	// また特に方向を持たない dp[s * n + s] も計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_root(0, 0);

	h.resize(n);
	rep(s, n) h[s] = dp[s * n + s];
}


//【コスト付き木の高さ】O(n)
/*
* コスト付き木 g の頂点 i を根にしたときの高さ（最も遠い葉までのコスト）を h[i] に格納する．
* 
*（全方位木 DP）
*/
void height_of_undirected_tree(WGraph& g, vl& h) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_B

	ll n = sz(g);

	// 辺 (p, s) を切断したときの s を根とする部分木を部分木 (p, s) と呼ぶ．
	// dp[p * n + s] : 部分木 (p, s) の高さ
	unordered_map<ll, ll> dp;

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[p * n + s] を計算する．
	function<ll(int, int)> dfs_to_leaf = [&](int p, int s) {
		// 子の情報を集めてその最大値をとり，自身の情報を計算する．
		for (auto e : g[s]) {
			if (e.to != p) {
				chmax(dp[p * n + s], dfs_to_leaf(s, e.to) + e.cost);
			}
		}

		return dp[p * n + s];
	};

	// 頂点 0 を根とし，根の方向に向かってのみの dp[s * n + p] を計算する．
	// また特に方向を持たない dp[s * n + s] も計算する．
	function<void(int, int)> dfs_to_root = [&](int p, int s) {
		// s から出ている辺の本数
		int m = sz(g[s]);

		// 左右からの累積最大値を計算する．
		vl acc_l(m + 1), acc_r(m + 1);
		for (int i = 0; i < m; i++) {
			auto e = g[s][i];
			acc_l[i + 1] = max(acc_l[i], dp[s * n + e.to] + e.cost);
		}
		for (int i = m - 1; i >= 0; i--) {
			auto e = g[s][i];
			acc_r[i] = max(acc_r[i + 1], dp[s * n + e.to] + e.cost);
		}

		// 左右からの累積最大値を用いて 1 つ抜きの最大値を計算する．
		rep(i, m) {
			int t = g[s][i].to;
			dp[t * n + s] = max(acc_l[i], acc_r[i + 1]);
		}

		// 総最大値も記録しておく．
		dp[s * n + s] = acc_l[m];

		// これで子から自身への情報が計算できたので，
		// 子に対して同様の計算を行っていく．
		for (auto e : g[s]) {
			if (e.to != p) {
				dfs_to_root(s, e.to);
			}
		}
	};

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[p * n + s] を計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_leaf(0, 0);

	// 頂点 0 を根とし，根の方向に向かってのみの dp[p * n + s] を計算する．
	// また特に方向を持たない dp[s * n + s] も計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_root(0, 0);

	h = vl(n);
	rep(s, n) {
		h[s] = dp[s * n + s];
	}
}


//【部分木の数え上げ】O(n)
/*
* 木 g の部分木のうち頂点 i を含むものの個数を cnt[i] に格納する．
*
*（全方位木 DP）
*/
void count_subtree(Graph& g, vm& cnt) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_v

	ll n = sz(g);

	// 辺 (p, s) を切断したときの s を含む部分木を部分木 (p, s) と呼ぶ．
	// dp[p * n + s] : 部分木 (p, s) の s を含む部分木の個数
	unordered_map<ll, mint> dp;

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[p * n + s] を計算する．
	function<mint(int, int)> dfs_to_leaf = [&](int p, int s) {
		dp[p * n + s] = 1;

		// 子の情報を集めて積をとり，自身の情報を計算する．
		for (auto t : g[s]) {
			if (t != p) {
				dp[p * n + s] *= dfs_to_leaf(s, t) + 1;
			}
		}

		return dp[p * n + s];
	};

	// 頂点 0 を根とし，根の方向に向かってのみの dp[s * n + p] を計算する．
	// また特に方向を持たない dp[s * n + s] も計算する．
	function<void(int, int)> dfs_to_root = [&](int p, int s) {
		// s から出ている辺の本数
		int m = sz(g[s]);

		// 左右からの累積積を計算する．
		vector<mint> acc_l(m + 1), acc_r(m + 1);
		acc_l[0] = 1;
		for (int i = 0; i < m; i++) {
			int t = g[s][i];
			acc_l[i + 1] = acc_l[i] * (dp[s * n + t] + 1);
		}
		acc_r[m] = 1;
		for (int i = m - 1; i >= 0; i--) {
			int t = g[s][i];
			acc_r[i] = acc_r[i + 1] * (dp[s * n + t] + 1);
		}

		// 左右からの累積積を用いて 1 つ抜きの積を計算する．
		rep(i, m) {
			int t = g[s][i];
			dp[t * n + s] = acc_l[i] * acc_r[i + 1];
		}

		// 総積も記録しておく．
		dp[s * n + s] = acc_l[m];

		// これで子から自身への情報が計算できたので，
		// 子に対して同様の計算を行っていく．
		for (auto t : g[s]) {
			if (t != p) {
				dfs_to_root(s, t);
			}
		}
	};

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[p * n + s] を計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_leaf(0, 0);

	// 頂点 0 を根とし，根の方向に向かってのみの dp[p * n + s] を計算する．
	// また特に方向を持たない dp[s * n + s] も計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_root(0, 0);

	cnt = vector<mint>(n);
	rep(s, n) {
		cnt[s] = dp[s * n + s];
	}
}


//【木の構築方法の数え上げ】O(n)
/*
* 頂点 s から始めて連結性を保ったまま辺の追加を行い木 g を構築する方法の数を cnt[s] に格納する．
*
*（全方位木 DP）
*
* 利用：【階乗と二項係数（法が大きな素数，mint利用）】
*/
void count_continuous_tree_construction(const Graph& g, vm& cnt) {
	// verify : https://atcoder.jp/contests/abc160/tasks/abc160_f

	ll n = sz(g);
	cnt.resize((int)n);
	Factorial_mint fm((int)n);

	// 辺 (p, s) を切断したときの s を含む部分木を部分木 (p, s) と呼ぶ．
	// dp[p * n + s] : s からの連結性を保ったまま部分木 (p, s) を構築する方法の数 
	//				   頂点 s は最初から存在しているものとする．
	unordered_map<ll, mint> dp;
	unordered_map<ll, int> w; // w[p * n + s] : 部分木 (p, s) の大きさ（頂点数）

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[p * n + s] を計算する．
	function<mint(int, int)> dfs_to_leaf = [&](int p, int s) {
		dp[p * n + s] = 1;
		vi ws;

		// 子の情報を集めて自身の情報を計算する．
		repe(t, g[s]) {
			if (t == p) continue;

			// それぞれの子の部分木を構築する方法の数
			dp[p * n + s] *= dfs_to_leaf(s, t);

			ws.push_back(w[s * n + t]);
		}

		// 各回でどの子の部分木の構築を進めるか
		dp[p * n + s] *= fm.multinomial(ws);

		w[p * n + s] = accumulate(all(ws), 1);
		return dp[p * n + s];
	};

	// 頂点 0 を根とし，根の方向に向かってのみの dp[s * n + p] を計算する．
	// また特に方向を持たない dp[s * n + s] も計算する．
	function<void(int, int)> dfs_to_root = [&](int p, int s) {
		// s から出ている辺の本数
		int m = sz(g[s]);

		// 部分木の大きさ
		vi ws(m);
		rep(i, m) ws[i] = w[s * n + g[s][i]];
		int w_sum = accumulate(all(ws), 0);

		// 左右からの累積積を計算する．
		vector<mint> acc_l(m + 1), acc_r(m + 1);
		acc_l[0] = 1;
		rep(i, m) {
			int t = g[s][i];
			acc_l[i + 1] = acc_l[i] * dp[s * n + t];
		}
		acc_r[m] = 1;
		repir(i, m - 1, 0) {
			int t = g[s][i];
			acc_r[i] = acc_r[i + 1] * dp[s * n + t];
		}

		// 各回でどの子の部分木の構築を進めるかに対応する多項係数
		mint mul = fm.multinomial(ws);

		// 左右からの累積積を用いて 1 つ抜きの積を計算する．
		rep(i, m) {
			int t = g[s][i];
			dp[t * n + s] = acc_l[i] * acc_r[i + 1];

			// 各回でどの子の部分木の構築を進めるか
			// 多項係数の計算は毎回行うのではなく差分更新する．
			mint nmul = mul;
			nmul *= fm.factorial(ws[i]) * fm.factorial_inv(w_sum) * fm.factorial(w_sum - ws[i]);
			dp[t * n + s] *= nmul;
			w[t * n + s] = w_sum - ws[i] + 1;
		}

		// 総積も記録しておく．
		dp[s * n + s] = acc_l[m];
		dp[s * n + s] *= mul;

		// これで子から自身への情報が計算できたので，
		// 子に対して同様の計算を行っていく．
		for (auto t : g[s]) {
			if (t == p) continue;

			dfs_to_root(s, t);
		}
	};

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[p * n + s] を計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_leaf(0, 0);

	// 頂点 0 を根とし，根の方向に向かってのみの dp[p * n + s] を計算する．
	// また特に方向を持たない dp[s * n + s] も計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_root(0, 0);

	rep(s, n) cnt[s] = dp[s * n + s];
}


