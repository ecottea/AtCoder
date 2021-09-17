#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ上の数え上げ問題 ■■■■■



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
		vm acc_l(m + 1), acc_r(m + 1);
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

	cnt = vm(n);
	rep(s, n) {
		cnt[s] = dp[{s, s}];
	}
}


//【独立集合の数え上げ】O(|V|)
/*
* 木 g の独立集合の個数を返す．
* 
*（木の状態 DP）
*/
mint count_independent_set(Graph& g) {
	int n = sz(g);

	// 便宜上 0 を根とした根付き木とみなし，黒で塗った頂点を独立集合として選ぶことにする．
	// dp[i][j] : 頂点 i が色 j(1:黒, 0:白) のとき，その部分木に含まれる独立集合の個数
	vvm dp(n, vm(2));
	vvb seen(n, vb(2));

	// メモ化再帰用の関数
	// s : 注目頂点，c : s の色，p : 親頂点
	function<mint(int, int, int)> dfs = [&](int s, int c, int p) {
		// 既に計算済ならその値を返す．
		if (seen[s][c]) {
			return dp[s][c];
		}
		seen[s][c] = true;

		// 積についての単位元で初期化しておく．
		dp[s][c] = 1;

		// s のそれぞれの子 t について処理を行う．
		for (auto t : g[s]) {
			// 親には戻らない
			if (t == p) {
				continue;
			}

			// t が白になるような部分木は候補になる．
			mint mul = dfs(t, 0, s);
			if (!c) {
				// s の色が白ならば，t が黒になるような部分木も候補になる．
				mul += dfs(t, 1, s);
			}

			// s の部分木については独立なので，積の法則で数え上げる．
			dp[s][c] *= mul;
		}

		return dp[s][c];
	};

	return dfs(0, 0, 0) + dfs(0, 1, 0);
}


//【連結成分の数え上げ】O(deg(v)^k)
/*
* グラフ g の大きさ k の連結成分の個数を返す．
* 
*（道ではなく頂点集合に注目したバックトラッキング）
*/
int count_connected_component(Graph& g, int k) {
	int n = sz(g);

	// seen[v] : 頂点 v の探索状態
	// 0：未探索，1：選択済，2：選択したことがあるので考慮不要
	vi seen(n);

	// 今選択している頂点の集合
	vi sel;

	// 見つけた連結成分の個数
	int res = 0;

	// バックトラッキング用の再帰関数
	// rmd : 後何個頂点を選べば良いか
	function<void(int)> dfs = [&](int rmd) {
		// もう頂点を選ぶ必要がなければ連結成分の発見．
		if (rmd == 0) {
			res++;
			return;
		}

		// 探索すべき頂点を記憶しておくためのリスト
		set<int> nxt;

		// 選択済みの各頂点 s について
		repe(s, sel) {
			// s と隣接する各頂点 t について
			repe(t, g[s]) {
				// もし t が未探索の状態でないなら何もしない．
				if (seen[t] != 0) {
					continue;
				}

				// t が未探索なら探索すべき頂点として記録する．
				nxt.insert(t);
			}
		}

		// 探索すべき各頂点 v について
		repe(v, nxt) {
			// もし v が未探索の状態でないなら何もしない．
			if (seen[v] != 0) {
				continue;
			}

			// v を選択済みの状態にして再帰を回し，その後状態を元に戻す．
			// ただし 0→1→2 の後で 0→2→1 を見に行ったりするのを防ぐため，
			// v の状態は「選択したことがあるので考慮不要」にしておく．
			seen[v] = 1;
			sel.push_back(v);
			dfs(rmd - 1);
			sel.pop_back();
			seen[v] = 2;
		}

		// 「選択したことがあるので考慮不要」の状態にしたままだと，
		// 例えば先の続いて 0→3→2 のようなものを見落としてしまうので，
		// ちゃんと状態を「未探索」に戻しておく．
		repe(v, nxt) {
			seen[v] = 0;
		}
	};

	// グラフ g の各頂点 v について
	rep(v, n) {
		// v を選択済みの状態にして再帰を回し，その後状態を元に戻す．
		// ただし 0→1→2 の後で 1→0→2 を見に行ったりするのを防ぐため，
		// v の状態は「選択したことがあるので考慮不要」にしておく．
		seen[v] = 1;
		sel.push_back(v);
		dfs(k - 1);
		sel.pop_back();
		seen[v] = 2;
	}

	return res;
}


