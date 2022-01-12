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


//【クリークの列挙】O(2^(1.4√|E|) |V|)
/*
* 無向グラフ g の i 番目に見つけたクリークを cs[i] に頂点の列として列挙する．
* S ⊂ V がクリークであるとは，S の任意の 2 点を結ぶ辺が E に属することをいう．
*/
void enumerate_clique(const Graph& g, vvi& cs) {
	// 参考：https://www.slideshare.net/wata_orz/ss-12131479
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/2306

	int n = sz(g);
	cs.clear();

	// 隣接行列 adj，各頂点の次数 deg，総次数 deg_sum，
	// 最小次数 deg_min，次数最小頂点の番号 i_min を得る．
	vvb adj(n, vb(n));
	vi deg(n);
	int deg_sum = 0, deg_min = INF, i_min = -1;
	rep(s, n) {
		deg[s] = sz(g[s]);
		deg_sum += deg[s];
		repe(t, g[s]) adj[s][t] = true;

		if (chmin(deg_min, deg[s])) i_min = s;
	}

	// 考慮すべき頂点のリスト
	vi v(n);
	iota(all(v), 0);

	// 素朴な方法で最大クリークを求める O(2^n n)
	function<void()> naive = [&]() {
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
						goto NEXT_LOOP;
					}
				}
			}

			// クリークが見つかったので記録する．
			cs.push_back(vi());
			rep(i, n) {
				if (set & (1 << i)) {
					cs.rbegin()->push_back(v[i]);
				}
			}

		NEXT_LOOP:;
		}
	};

	int res = 1;
	while (n > 0) {
		// 辺に対して頂点が十分少ないなら素朴な方法で構わない．
		if (deg_min * deg_min >= deg_sum) {
			naive();
			return;
		}

		// 次数最小の頂点 v[i_min] の隣接点の番号の集合を得る．
		// 同時に v[i_min] に出入りする辺を削除したことにし，各頂点の次数 deg を更新する．
		vi ia;
		rep(i, n) {
			if (adj[v[i_min]][v[i]]) {
				ia.push_back(i);

				deg[v[i]]--;
			}
		}
		int d = sz(ia);

		// まず v[i_min] を含む最大クリークの大きさ res を求める．
		// v[i_min] の隣接点の部分集合 sub すべてについて
		repb(sub, d) {
			rep(i, d) {
				// sub に選んでいないなら無関係
				if (!(sub & (1 << i))) {
					continue;
				}

				repi(j, i + 1, d - 1) {
					// sub に選んでいないなら無関係
					if (!(sub & (1 << j))) {
						continue;
					}

					// sub がクリークでなければ何もしない．
					if (!adj[v[ia[i]]][v[ia[j]]]) {
						goto LOOP_END;
					}
				}
			}

			// sub がクリークなら v[i_min] と合わせてもクリークとなるので記録する．
			cs.push_back(vi({ v[i_min] }));
			rep(i, d) {
				if (sub & (1 << i)) {
					cs.rbegin()->push_back(v[ia[i]]);
				}
			}

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
}


