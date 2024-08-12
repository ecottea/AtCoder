#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ上の列挙問題 ■■■■■


//【単純パスの列挙（始点指定）】O(?)
/*
* グラフ g の st を始点とする単純パス全てを格納した二次元リストを返す．
*/
template <class G>
vvi enumerate_simple_path(const G& g, int st) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_bt

	int n = sz(g);
	vvi paths;
	vi seq; // 訪れた頂点の列

	// 頂点を訪れたことを記録しておくテーブル．
	vb seen(n);

	// 再帰用の関数
	function<void(int)> dfs = [&](int s) {
		// s を訪れたことを記録
		seen[s] = true;
		seq.push_back(s);

		// 単純パスを記録
		paths.push_back(seq);

		repe(t, g[s]) {
			// 探索済なら何もしない．
			if (seen[t]) continue;

			// 未探索の頂点を探索しにいく．
			dfs(t);
		}

		// s を訪れた記録を削除
		seen[s] = false;
		seq.pop_back();
	};
	dfs(st);

	return paths;
}


//【単純パスの列挙（始点，終点指定）】O(?)
/*
* グラフ g の ST から GL への単純パス全てを格納した二次元リストを返す．
*/
template <class G>
vvi enumerate_simple_path(const G& g, int ST, int GL) {
	// verify : https://mojacoder.app/users/RedSpica/contests/RedSpica-Regular-Selection/tasks/7

	int n = sz(g);
	vvi paths;
	vi seq; // 訪れた頂点の列

	// 頂点を訪れたことを記録しておくテーブル．
	vb seen(n);

	// 再帰用の関数
	function<void(int)> dfs = [&](int s) {
		// s を訪れたことを記録
		seen[s] = true;
		seq.push_back(s);

		// GL にたどり着いたら単純パスを記録
		if (s == GL) {
			paths.push_back(seq);
		}
		// まだ GL にたどり着いていないなら先を探索
		else {
			repe(t, g[s]) {
				// 探索済なら何もしない．
				if (seen[t]) continue;

				// 未探索の頂点を探索しにいく．
				dfs(t);
			}
		}

		// s を訪れた記録を削除
		seen[s] = false;
		seq.pop_back();
	};
	dfs(ST);

	return paths;
}


//【連結成分の列挙】O(?)
/*
* 無向グラフ g の大きさ k の連結成分全てのリストを返す．
* 連結成分は頂点番号のリストとして表す．
*/
vvi enumerate_connected_component(const Graph& g, int k) {
	// verify : https://atcoder.jp/contests/abc211/tasks/abc211_e

	int n = sz(g);
	vvi ccs;

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

	return ccs;
}


//【有向木の列挙】O(?)
/*
* 有向グラフ g（多重辺不可）の r を根とする大きさ k の有向木全てのリストを返す．
*/
vector<Graph> enumerate_tree(Graph& g, int r, int k) {
	int n = sz(g);

	vector<Graph> res;
	Graph g2(n);

	// seen_v[i] : 頂点 i の探索を済ませたか
	vb seen_v(n);

	// seen_e[i][j] : 頂点 i から頂点 j への辺の探索を済ませたか
	vvb seen_e(n, vb(n));

	// 今選択している頂点の集合
	vi sel;

	// バックトラッキング用の再帰関数
	// rmd : あと何個頂点を選べば良いか
	function<void(int)> dfs = [&](int rmd) {
		// もう頂点を選ぶ必要がなければ大きさ k の有向木の発見．
		if (rmd == 0) {
			res.push_back(g2);
			return;
		}

		// 探索すべき辺を記憶しておくためのリスト
		vector<pii> sts;

		// 選択済みの各頂点 s について
		repe(s, sel) {
			// s から出る各辺 s→t について
			repe(t, g[s]) {
				// もし s→t や t が探索済なら何もしない．
				if (seen_e[s][t] || seen_v[t]) continue;

				// s→t と t が未探索なら探索すべき辺として記録する．
				sts.push_back({ s, t });
			}
		}

		// 探索すべき各辺 s→t について
		for (auto [s, t] : sts) {
			// もし s→t や t が探索済なら何もしない．
			if (seen_e[s][t] || seen_v[t]) continue;

			// s→t と t を選択済みの状態にして再帰を回し，その後状態を元に戻す．
			// ただし 2←0→1 の後で 1←0→2 を見に行ったりするのを防ぐため，
			// s→t の状態だけは探索済のままにしておく．
			seen_e[s][t] = true;
			seen_v[t] = true;
			sel.push_back(t);
			g2[s].push_back(t);
			dfs(rmd - 1);
			g2[s].pop_back();
			sel.pop_back();
			seen_v[t] = false;
		}

		// s→t を探索済の状態にしたままだと，
		// 例えば先に続いて 0→2→1 のようなものを見落としてしまうので，
		// ちゃんと状態を未探索に戻しておく．
		for (auto [s, t] : sts) seen_e[s][t] = false;
	};

	sel.push_back(r);
	seen_v[r] = true;
	dfs(k - 1);

	return res;
}


//【有向木の列挙（参照付き）】O(?)
/*
* 参照付き有向グラフ g の r を根とする大きさ k の有向木の辺集合のリストを返す．
* 辺集合は辺に付けられた参照番号を並べたリストで表す．
*/
vvi enumerate_tree(IGraph& g, int r, int k) {
	// verify : https://atcoder.jp/contests/arc009/tasks/arc009_4

	int n = sz(g);

	int m = 0;
	rep(s, n) repe(e, g[s]) chmax(m, e.id);
	m++;

	vvi res;

	// seen_v[i] : 頂点 i の探索を済ませたか
	vb seen_v(n);

	// seen_e[id] : 辺番号 id の辺の探索を済ませたか
	vb seen_e(m);

	// 今選択している頂点の集合，辺番号の集合
	vi sel, ids;

	// バックトラッキング用の再帰関数
	// rmd : あと何個頂点を選べば良いか
	function<void(int)> dfs = [&](int rmd) {
		// もう頂点を選ぶ必要がなければ大きさ k の有向木の発見．
		if (rmd == 0) {
			res.push_back(ids);
			return;
		}

		// 探索すべき辺を記憶しておくためのリスト
		vector<tuple<int, int, int>> stids;

		// 選択済みの各頂点 s について
		repe(s, sel) {
			// s から出る各辺 e について
			repe(e, g[s]) {
				// もし e や e の先が探索済なら何もしない．
				if (seen_e[e.id] || seen_v[e.to]) continue;

				// e と e の先が未探索なら探索すべき辺として記録する．
				stids.push_back({ s, e.to, e.id });
			}
		}

		// 探索すべき各辺 e : s→t について
		for (auto [s, t, id] : stids) {
			// もし e や e の先が探索済なら何もしない．
			if (seen_e[id] || seen_v[t]) continue;

			// e と e の先を選択済みの状態にして再帰を回し，その後状態を元に戻す．
			// ただし 2←0→1 の後で 1←0→2 を見に行ったりするのを防ぐため，
			// e の状態だけは探索済のままにしておく．
			seen_e[id] = true;
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
		for (auto [s, t, id] : stids) seen_e[id] = false;
	};

	sel.push_back(r);
	seen_v[r] = true;
	dfs(k - 1);

	return res;
}


//【クリークの列挙】O(2^(1.4√m) n)
/*
* 無向グラフ g のクリークを成す頂点集合のリストを返す（空グラフを含む）
* S ⊂ V がクリークであるとは，S の任意の 2 点を結ぶ辺が E に属することをいう．
*/
vvi enumerate_clique(const Graph& g) {
	// 参考：https://www.slideshare.net/wata_orz/ss-12131479
	// verify : https://judge.yosupo.jp/problem/enumerate_cliques

	int n = sz(g);
	vvi cs;

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
			// n 頂点それぞれについて
			rep(i, n) {
				// set に選んでいないなら無関係
				if (!(set & (1 << i))) continue;

				// i 番目以降の頂点について
				repi(j, i + 1, n - 1) {
					// set に選んでいないなら無関係 
					if (!(set & (1 << j))) continue;

					// 辺 (v[i], v[j]) がなければクリークでない．
					if (!adj[v[i]][v[j]]) goto NEXT_LOOP;
				}
			}

			// クリークが見つかったので記録する．
			cs.push_back(vi());
			rep(i, n) if (set & (1 << i)) cs.rbegin()->push_back(v[i]);

		NEXT_LOOP:;
		}
	};

	while (n > 0) {
		// 辺に対して頂点が十分少ないなら素朴な方法で構わない．
		if (deg_min * deg_min >= deg_sum) {
			naive();
			return cs;
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
				if (!(sub & (1 << i))) continue;

				repi(j, i + 1, d - 1) {
					// sub に選んでいないなら無関係
					if (!(sub & (1 << j))) continue;

					// sub がクリークでなければ何もしない．
					if (!adj[v[ia[i]]][v[ia[j]]]) goto LOOP_END;
				}
			}

			// sub がクリークなら v[i_min] と合わせてもクリークとなるので記録する．
			cs.push_back(vi({ v[i_min] }));
			rep(i, d) if (sub & (1 << i)) cs.rbegin()->push_back(v[ia[i]]);

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
			if (chmin(deg_min, deg[v[i]])) 	i_min = i;
			deg_sum += deg[v[i]];
		}
	}

	return cs;
}


//【三角形の列挙】O(m √m log n) ?
/*
* 無向グラフ g の三角形 {v1, v2, v3} のリストを返す．（v1 < v2 < v3）
*/
vector<tuple<int, int, int>> enumerate_triangles(const Graph& g) {
	// verify : https://judge.yosupo.jp/problem/enumerate_triangles

	int n = sz(g);

	// gD : g の s → t (s < t) なる辺のみからなる DAG
	Graph gD(n);
	rep(s, n) repe(t, g[s]) if (s < t) gD[s].push_back(t);

	// d[s] : gD の頂点 s の出次数
	vi d(n);
	rep(s, n) {
		d[s] = sz(gD[s]);
		sort(all(gD[s])); // 辺は行き先について昇順ソートしておく
	}

	vector<tuple<int, int, int>> res;

	rep(s, n) repe(t, gD[s]) {
		if (d[s] < d[t]) {
			// 辺 s→u をもつ各 u について，辺 t→u が存在するかを二分探索で調べる．
			repir(j, sz(gD[s]) - 1, 0) {
				int u = gD[s][j];
				if (u <= t) break;

				auto it = lower_bound(all(gD[t]), u);
				if (it != gD[t].end() && *it == u) {
					res.emplace_back(s, t, u);
				}
			}
		}
		else {
			// 辺 t→u をもつ各 u について，辺 s→u が存在するかを二分探索で調べる．
			repe(u, gD[t]) {
				auto it = lower_bound(all(gD[s]), u);
				if (it != gD[s].end() && *it == u) {
					res.emplace_back(s, t, u);
				}
			}
		}
	}

	return res;
}


//【完全グラフの完全マッチングの列挙】
/*
* マッチング(一般).h へ
*/
