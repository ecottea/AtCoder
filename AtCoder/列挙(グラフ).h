#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ上の列挙問題 ■■■■■


//【完全グラフの完全マッチングの列挙】O((2n)!! n)
/*
* 頂点 [0..2n) をもつ完全グラフの完全マッチングを mcs に列挙する．
* 完全マッチングは n 個の頂点対のリストとして表す．
*/
void enumerate_perfect_matching(int n, vector<vector<pii>>& mcs) {
	// verify : https://atcoder.jp/contests/abc236/tasks/abc236_d

	mcs.clear();

	// a[i] : 頂点 i が何番目のマッチングに属しているか（未使用なら -1）
	vi a(2 * n, -1);

	// k : 次に定めるのが何番目のマッチングか
	int k = 0;

	// mc : 作成途中のマッチング
	vector<pii> mc(n);

	// 頂点 i 以降のマッチングを見つける
	function<void(int)> rf = [&](int i) {
		// 全ての頂点をマッチし終えたら結果を格納する．
		if (i == 2 * n) {
			mcs.push_back(mc);
			return;
		}

		// 頂点 i が使用済みだった場合は次の頂点へ進む．
		if (a[i] != -1) {
			rf(i + 1);
			return;
		}

		// 頂点 i を k 番目のマッチングの片方に選ぶ．
		a[i] = k;
		mc[k].first = i;

		// j : 頂点 i とマッチさせる頂点
		repi(j, i + 1, 2 * n - 1) {
			// 頂点 j が使用済みだった場合は選べない．
			if (a[j] != -1) continue;

			// 頂点 j を頂点 i とマッチさせる．
			a[j] = k;
			mc[k].second = j;
			k++;

			// 次の頂点に進む．
			rf(i + 1);

			// 頂点 j を未使用に戻しておく．
			k--;
			a[j] = -1;
		}

		// 頂点 i を未使用に戻しておく．
		a[i] = -1;

		return;
	};

	rf(0);
}


//【連結成分の列挙】O(deg(v)^k |V| k)
/*
* 無向グラフ g の大きさ k の連結成分を ccs に列挙する．
* 連結成分は頂点番号のリストとして表す．
*
*（バックトラッキング）
*/
void enumerate_connected_component(const Graph& g, int k, vvi& ccs) {
	// verify : https://atcoder.jp/contests/abc211/tasks/abc211_e

	int n = sz(g);

	// seen[v] : 頂点 v の探索を済ませたか
	vb seen(n);

	// 今選択している頂点の集合
	vi sel;

	// バックトラッキング用の再帰関数
	// rmd : あと何個頂点を選べば良いか
	function<void(int)> dfs = [&](int rmd) {
		// もう頂点を選ぶ必要がなければ大きさ k の連結成分の発見．
		if (rmd == 0) {
			ccs.push_back(sel);
			return;
		}

		// 探索すべき頂点を記憶しておくためのリスト
		vi nxt;

		// 選択済みの各頂点 s について
		repe(s, sel) {
			// s と隣接する各頂点 t について
			repe(t, g[s]) {
				// もし t が探索済なら何もしない．
				if (seen[t]) continue;

				// t が未探索なら探索すべき頂点として記録する．
				nxt.push_back(t);
			}
		}

		// 探索すべき各頂点 v について
		repe(v, nxt) {
			// もし v が探索済なら何もしない．
			if (seen[v]) continue;

			// v を選択して再帰を回し，その後 v を削除する．
			// ただし 0→1→2 の後で 0→2→1 を見に行ったりするのを防ぐため，
			// v の状態だけは探索済のままにしておく．
			seen[v] = true;
			sel.push_back(v);
			dfs(rmd - 1);
			sel.pop_back();
		}

		// 探索済の状態にしたままだと，
		// 例えば先に続いて 0→3→2 のようなものを見落としてしまうので，
		// ちゃんと状態を未探索に戻しておく．
		repe(v, nxt) seen[v] = false;
	};

	// グラフ g の各頂点 v について
	rep(v, n) {
		// v を選択して再帰を回し，その後 v を削除する．
		// ただし 0→1→2 の後で 0→2→1 を見に行ったりするのを防ぐため，
		// v の状態は探索済のままにしておく．
		seen[v] = true;
		sel.push_back(v);
		dfs(k - 1);
		sel.pop_back();
	}
}


//【有向木の列挙】O(?)
/*
* 参照付きグラフ g の r を根とする大きさ k の有向木の辺集合を dts に列挙する．
* 辺集合は辺に付けられた参照番号を並べたリストで表す．
*
*（バックトラッキング）
*/
void enumerate_tree(IGraph& g, int r, int k, vvi& dts) {
	// verify : https://atcoder.jp/contests/arc009/tasks/arc009_4

	int n = sz(g);

	// seen_v[i] : 頂点 i の探索を済ませたか
	vb seen_v(n);

	// seen_e[i][j] : 頂点 i から頂点 j への辺の探索を済ませたか
	vvb seen_e(n, vb(n));

	// 今選択している頂点の集合，辺番号の集合
	vi sel, ids;

	// バックトラッキング用の再帰関数
	// rmd : あと何個頂点を選べば良いか
	function<void(int)> dfs = [&](int rmd) {
		// もう頂点を選ぶ必要がなければ大きさ k の有向木の発見．
		if (rmd == 0) {
			dts.push_back(ids);
			return;
		}

		// 探索すべき辺を記憶しておくためのリスト
		vector<tuple<int, int, int>> stids;

		// 選択済みの各頂点 s について
		repe(s, sel) {
			// s から出る各辺 e について
			repe(e, g[s]) {
				// もし e や e の先が探索済なら何もしない．
				if (seen_e[s][e.to] || seen_v[e.to]) continue;

				// e と e の先が未探索なら探索すべき辺として記録する．
				stids.push_back({ s, e.to, e.id });
			}
		}

		// 探索すべき各辺 e : s -> t について
		repe(stid, stids) {
			int s, t, id;
			tie(s, t, id) = stid;

			// もし e や e の先が探索済なら何もしない．
			if (seen_e[s][t] || seen_v[t]) continue;

			// e と e の先を選択済みの状態にして再帰を回し，その後状態を元に戻す．
			// ただし 2←0→1 の後で 1←0→2 を見に行ったりするのを防ぐため，
			// e の状態だけは探索済のままにしておく．
			seen_e[s][t] = true;
			seen_e[t][s] = true;
			seen_v[t] = true;
			sel.push_back(t);
			ids.push_back(id);
			dfs(rmd - 1);
			ids.pop_back();
			sel.pop_back();
			seen_v[t] = false;
		}

		// e を探索済の状態にしたままだと，
		// 例えば先に続いて 0→2→1 のようなものを見落としてしまうので，
		// ちゃんと状態を未探索に戻しておく．
		repe(stid, stids) {
			int s, t, id;
			tie(s, t, id) = stid;

			seen_e[s][t] = false;
			seen_e[t][s] = false;
		}
	};

	sel.push_back(r);
	seen_v[r] = true;
	dfs(k - 1);
}


//【クリークの列挙】O(2^(1.4√|E|) |V|)
/*
* 無向グラフ g の i 番目に見つけたクリークを cs[i] に頂点の集合として列挙する．
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


