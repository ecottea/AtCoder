#pragma once
#include "header.h"
// ■■■■■ グラフ上の数え上げ問題 ■■■■■


//【連結成分の数え上げ】O(deg(v)^k)
/*
* 無向グラフ g の大きさ k の連結成分の個数を返す．
* 
*（道ではなく頂点集合に注目したバックトラッキング）
*/
int count_connected_component(Graph& g, int k) {
	// verify : https://atcoder.jp/contests/abc211/tasks/abc211_e

	int n = sz(g);

	// seen[v] : 頂点 v の探索状態
	// 0：未探索，1：選択済，2：選択したことがあるので考慮不要
	vi seen(n);

	// 今選択している頂点の集合
	vi sel;

	// 見つけた連結成分の個数
	int res = 0;

	// バックトラッキング用の再帰関数
	// rmd : あと何個頂点を選べば良いか
	function<void(int)> dfs = [&](int rmd) {
		// もう頂点を選ぶ必要がなければ大きさ k の連結成分の発見．
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
		// 例えば先に続いて 0→3→2 のようなものを見落としてしまうので，
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


//【単純パスの数え上げ】O(|V|^2 2^|V|)
/*
* グラフ g について単純パス s → t の個数を cnt[s][t] に格納する．
*
*（bit DP）
*/
void count_simple_path(const Graph& g, vvl& cnt) {
	int n = sz(g);

	// dp[s][t][set] : 単純パス s → t で途中 set を通るものの個数
	//		s !∈ set, t ∈ set とする．
	vvvl dp(n, vvl(n, vl(1 << n)));
	vvvb seen(n, vvb(n, vb(1 << n)));
	rep(s, n) {
		dp[s][s][0] = 1;
		seen[s][s][0] = true;
	}

	// 単純パス s → t で途中 set を通るものの個数を返す．
	function<ll(int, int, int)> rf = [&](int s, int t, int set) {
		// もし確定済ならば DP テーブルの値をそのまま返す．
		if (seen[s][t][set]) return dp[s][t][set];
		seen[s][t][set] = true;

		// s から行ける各頂点 v について
		repe(v, g[s]) {
			// v が set に含まれていなければ何もしない．
			if (!(set & (1 << v))) continue;

			// s → v と進む単純パスの個数を加算する．
			dp[s][t][set] += rf(v, t, set - (1 << v));
		}

		return dp[s][t][set];
	};

	// 結果の格納
	cnt = vvl(n, vl(n));
	rep(s, n) {
		rep(t, n) {
			repb(set, n) {
				if ((set & (1 << s)) || !(set & (1 << t))) continue;
				cnt[s][t] += rf(s, t, set);
			}

			// 不動の場合もカウントする．
			if (s == t) cnt[s][t]++;
		}
	}
}


