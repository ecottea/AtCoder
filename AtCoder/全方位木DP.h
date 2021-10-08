#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "組合せ論.h"
// ■■■■■ 全方位木 DP ■■■■■



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


//【木の構築方法の数え上げ】O(|V|)
/*
* 連結性を保ったままの辺の追加を行う木 g を構築する方法の数を返す．
*
*（全方位木 DP）
* 
* 利用：【階乗と二項係数（mint利用）】
*/
mint continuous_tree_construction(Graph& g) {
	int n = sz(g);
	factorial_mint fm(n);

	// 辺 (p, s) を切断したときの s を含む部分木を部分木 (p, s) と呼ぶ．
	// dp[{p, s}] : s からの連結性を保ったまま部分木 (p, s) を構築する方法の数 
	//	頂点 s は最初から存在しているものとする．
	map<pii, mint> dp;
	map<pii, int> w; // w[{p, s}] : 部分木 (p, s) の大きさ

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[{p, s}] を計算する．
	function<mint(int, int)> dfs_to_leaf = [&](int p, int s) {
		dp[{p, s}] = 1;
		vi ws;

		// 子の情報を集めて自身の情報を計算する．
		for (auto t : g[s]) {
			if (t == p) continue;

			// それぞれの子の部分木を構築する方法の数
			dp[{p, s}] *= dfs_to_leaf(s, t);

			ws.push_back(w[{s, t}]);
		}

		// 各回でどの子の部分木の構築を進めるか
		dp[{p, s}] *= fm.multinomial(ws);

		w[{p, s}] = accumulate(all(ws), 1);
		return dp[{p, s}];
	};

	// 頂点 0 を根とし，根の方向に向かってのみの dp[{s, p}] を計算する．
	// また特に方向を持たない dp[{s, s}] も計算する．
	function<void(int, int)> dfs_to_root = [&](int p, int s) {
		// s から出ている辺の本数
		int m = sz(g[s]);

		// 部分木の大きさ
		vi ws(m);
		rep(i, m) {
			ws[i] += w[{s, g[s][i]}];
		}

		// 左右からの累積積を計算する．
		vector<mint> acc_l(m + 1), acc_r(m + 1);
		acc_l[0] = 1;
		rep(i, m) {
			int t = g[s][i];
			acc_l[i + 1] = acc_l[i] * dp[{s, t}];
		}
		acc_r[m] = 1;
		repir(i, m - 1, 0) {
			int t = g[s][i];
			acc_r[i] = acc_r[i + 1] * dp[{s, t}];
		}

		// 左右からの累積積を用いて 1 つ抜きの積を計算する．
		rep(i, m) {
			int t = g[s][i];
			dp[{t, s}] = acc_l[i] * acc_r[i + 1];

			int wi = ws[i];
			ws[i] = 0;
			dp[{t, s}] *= fm.multinomial(ws);
			w[{t, s}] = accumulate(all(ws), 1);
			ws[i] = wi;
		}

		// 総積も記録しておく．
		dp[{s, s}] = acc_l[m];
		dp[{s, s}] *= fm.multinomial(ws);

		// これで子から自身への情報が計算できたので，
		// 子に対して同様の計算を行っていく．
		for (auto t : g[s]) {
			if (t == p) continue;

			dfs_to_root(s, t);
		}
	};

	// 頂点 0 を根とし，葉の方向に向かってのみの dp[{p, s}] を計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_leaf(0, 0);

	// 頂点 0 を根とし，根の方向に向かってのみの dp[{p, s}] を計算する．
	// また特に方向を持たない dp[{s, s}] も計算する．
	// これならばシンプルな深さ優先探索なので O(|V|) で済む．
	dfs_to_root(0, 0);

	mint res = 0;

	// 深さ偶数の頂点から部分木の構築を始めた場合のみを加算する．
	function<void(int, int, int)> dfs = [&](int s, int p, int d) {
		if (d % 2 == 0) res += dp[{s, s}];

		repe(t, g[s]) {
			if (t == p) continue;

			dfs(t, s, d + 1);
		}
	};

	dfs(0, -1, 0);

	return res;
}


