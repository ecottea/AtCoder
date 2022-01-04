#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "行列.h"
#include "分析(グラフ).h"
// ■■■■■ 全域木 ■■■■■


//【最小全域森／クラスカル法】O(|E| log|V|)
/*
* クラスカル法でコスト付き無向グラフ g の最小全域森を求める．
* 最小全域森は msf に構成し，各最小全域木の代表元を mst に格納する．
* また戻り値として最小コストを返す．
*/
ll kruskal(const WGraph& g, WGraph* msf = nullptr, vi* mst = nullptr) {
	int n = sz(g);
	if (msf != nullptr)	*msf = WGraph(n);

	// 辺を集めてコスト昇順にソートする．
	priority_queue_rev<tuple<ll, int, int>> q;
	rep(s, n) {
		repe(e, g[s]) {
			q.push({ e.cost, s, e.to });
		}
	}

	ll cost = 0; // 最小コスト
	dsu d(n); // 連結判定用
	while (!q.empty()) {
		int s, t; ll c;
		tie(c, s, t) = q.top(); q.pop();

		// もし辺の両端が既に連結なら繋がない．
		if (d.same(s, t)) continue;

		// そうでないならコスト最小の辺なのでそれで繋ぐ．
		cost += c;
		d.merge(s, t);

		if (msf != nullptr) {
			(*msf)[s].push_back({ t, c });
			(*msf)[t].push_back({ s, c });
		}
	}

	// 連結成分のそれぞれが最小全域木なので，その代表元を記録．
	if (mst != nullptr) {
		mst->clear();
		repe(tmp, d.groups()) mst->push_back(tmp[0]);
	}

	return cost;
}


//【最小全域木／プリム法】O(|E| log|V|)
/*
* コスト付きグラフ g の頂点 r を含む連結成分の最小全域木を mst に格納する．
* また戻り値として最小コストを返す．
*/
ll prim(const WGraph& g, int r, WGraph& mst) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%97%E3%83%AA%E3%83%A0%E6%B3%95
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_A

	int n = sz(g);
	mst = WGraph(n);
	ll res = 0;

	// selected[v] : 頂点 v を既に選んだかどうか
	vb selected(n);
	selected[r] = true;

	// 選んだ頂点から出ている辺をコスト昇順に記録しておくための優先度付きキュー．
	using E = tuple<ll, int, int>;
	priority_queue<E, vector<E>, greater<E>> q;
	repe(e, g[r]) {
		q.push({ e.cost, r, e.to });
	}

	while (!q.empty()) {
		ll c;
		int s, t;
		tie(c, s, t) = q.top();
		q.pop();

		// 既に選んだ頂点への辺なら何もしない．
		if (selected[t]) {
			continue;
		}

		// 最小全域木に辺を追加し，頂点を選んだことを記録しておく．
		mst[s].push_back({ t, c });
		res += c;
		selected[t] = true;

		// 調べるべき辺を追加する．
		repe(e, g[t]) {
			q.push({ e.cost, t, e.to });
		}
	}

	return res;
}


//【全域森】O(|V| + |E|)
/*
* 無向グラフ g の全域森を sf に構成し，各全域木の代表元を v に格納する．
*/
void spanning_forest(const Graph& g, Graph& sf, vi* v = nullptr) {
	// verify : https://atcoder.jp/contests/abc233/tasks/abc233_f

	int n = sz(g);
	sf = Graph(n);

	dsu d(n);
	rep(s, n) {
		repe(t, g[s]) {
			if (d.same(s, t)) continue;

			d.merge(s, t);
			sf[s].push_back(t);
			sf[t].push_back(s);
		}
	}

	// 連結成分のそれぞれが最小全域木なので，その代表元を記録．
	if (v != nullptr) {
		v->clear();
		repe(tmp, d.groups()) v->push_back(tmp[0]);
	}
}


//【全域木の数え上げ】O(|V|^3)
/*
* 自己ループのない無向グラフ g（多重辺は可）の全域木の個数を返す．
*
*（行列木定理）
*
* 利用：【行列】，【行列式】
*/
mint count_spanning_tree(const Graph& g) {
	// verify : https://atcoder.jp/contests/jsc2021/tasks/jsc2021_g

	int n = sz(g);
	if (n <= 1) return 1;

	// mat : g のラプラシアン行列から最終行と最終列を除いたもの
	//	mat[s][s] : 頂点 s の次数
	//	mat[s][t] : -(頂点 s, t を結ぶ辺の数)
	Matrix<mint> mat(n - 1, n - 1);

	rep(s, n - 1) {
		mat[s][s] = sz(g[s]);
		repe(t, g[s]) {
			assert(s != t); // 自己ループは許さない

			if (t < n - 1) {
				mat[s][t]--;
			}
		}
	}

	// ラプラシアン行列の任意の余因子行列の行列式が全域木の個数を与える．
	return determinant(mat);
}


//【全域森の数え上げ】O(|V|^3)
/*
* 自己ループのない無向グラフ g（多重辺は可）の全域森の個数を返す．
*
*（行列木定理）
*
* 利用：【行列】，【行列式】，【連結成分分解】
*/
mint count_spanning_forest(const Graph& g) {
	int n = sz(g);
	if (n <= 1) return 1;

	// g を連結成分分解する．
	vvi ccs;
	connected_component(g, ccs);

	// mat : g のラプラシアン行列
	//	mat[s][s] : 頂点 s の次数
	//	mat[s][t] : -(頂点 s, t を結ぶ辺の数)
	Matrix<mint> mat(n, n);
	rep(s, n) {
		mat[s][s] = sz(g[s]);
		repe(t, g[s]) {
			assert(s != t); // 自己ループは許さない

			mat[s][t]--;
		}
	}

	// 各連結成分に属する代表頂点に対応する行と列を除去する．
	unordered_set<int> avoid;
	repe(cc, ccs) avoid.insert(cc[0]);
	rep(i, n) {
		rep(j, n) {
			if (avoid.count(i) || avoid.count(j)) {
				mat[i][j] = (i == j);
			}
		}
	}

	// ラプラシアン行列の任意の余因子行列の行列式が全域木の個数を与える．
	return determinant(mat);
}


//【最小全域森の数え上げ】O(|V|^3)
/*
* 自己ループのないコスト付き無向グラフ g の最小全域森のコストと個数を返す．
*
* 利用：【全域森の数え上げ】
*/
pair<ll, mint> count_minimum_spanning_forest(const WGraph& g) {
	// verify : https://atcoder.jp/contests/arc018/tasks/arc018_4

	int n = sz(g);

	// 辺をコスト（昇順）ごとに分類する．
	map<ll, vector<pii>> edges;
	rep(s, n) {
		repe(e, g[s]) {
			edges[e.cost].push_back({ s, e.to });
		}
	}

	ll cost = 0; mint cnt = 1;

	// 今までに追加した辺で連結になる頂点を同一視する．
	dsu d(n);

	repe(tmp, edges) {
		// c : 辺のコスト
		ll c = tmp.first;

		// 旧頂点番号から新頂点番号への対応
		unordered_map<int, int> i_to_i2;
		int i = 0;

		// コスト c の辺のみからなる縮約されたグラフを作成する．
		Graph g2;
		repe(st, tmp.second) {
			int s, t;
			tie(s, t) = st;

			int rs = d.leader(s);
			int rt = d.leader(t);

			// 自己ループは閉路を作ってしまうのでだめ
			if (rs == rt) continue;

			if (!i_to_i2.count(rs)) {
				i_to_i2[rs] = i++;
				g2.push_back(vi());
			}
			if (!i_to_i2.count(rt)) {
				i_to_i2[rt] = i++;
				g2.push_back(vi());
			}

			g2[i_to_i2[rs]].push_back(i_to_i2[rt]);
		}

		// 全域木を数え上げる．
		//	これがボトルネックになるので，1 つのコストに対応する辺が少なければ
		//	計算量は O(|V|^3) から真に改善する．
		cnt *= count_spanning_forest(g2);

		// 頂点の同一視を進める
		repe(st, tmp.second) {
			int s, t;
			tie(s, t) = st;

			if (!d.same(s, t)) {
				cost += c;
				d.merge(s, t);
			}
		}
	}

	return make_pair(cost, cnt);
}


