#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "行列.h"
#include "分析(グラフ).h"
#include "ヒープ.h"
// ■■■■■ 全域木 ■■■■■


//【最小全域森】O(m log n)
/*
* 重み付き無向グラフ g の最小全域森を求め，そのコストを返す．
* 最小全域森を msf に構成し，各最小全域木の代表元を rs に格納する．
*/
ll kruskal(const WGraph& g, WGraph* msf = nullptr, vi* rs = nullptr) {
	// verify : https://judge.yosupo.jp/problem/minimum_spanning_tree

	int n = sz(g);
	if (msf != nullptr)	*msf = WGraph(n);

	// 辺を集めてコスト昇順にソートする．
	priority_queue_rev<tuple<ll, int, int>> q;
	rep(s, n) repe(e, g[s]) q.push({ e.cost, s, e.to });

	ll cost = 0; // 最小コスト
	dsu d(n); // 連結判定用
	while (!q.empty()) {
		auto [c, s, t] = q.top(); q.pop();

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
	if (rs != nullptr) {
		rs->clear();
		repe(tmp, d.groups()) rs->push_back(tmp[0]);
	}

	return cost;
}


//【最小全域木】O(m log n)
/*
* 重み付き無向グラフ g の頂点 r を含む連結成分の最小全域木を mst に格納する．
* また戻り値として最小コストを返す．
*/
ll prim(const WGraph& g, int r, WGraph* mst = nullptr) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%97%E3%83%AA%E3%83%A0%E6%B3%95
	// verify : https://judge.yosupo.jp/problem/minimum_spanning_tree

	int n = sz(g);
	if (mst) *mst = WGraph(n);
	ll res = 0;

	// selected[v] : 頂点 v を既に選んだかどうか
	vb selected(n);
	selected[r] = true;

	// 選んだ頂点から出ている辺をコスト昇順に記録しておくための優先度付きキュー．
	using E = tuple<ll, int, int>;
	priority_queue_rev<E> q;
	repe(e, g[r]) q.push({ e.cost, r, e.to });

	while (!q.empty()) {
		auto [c, s, t] = q.top(); q.pop();

		// 既に選んだ頂点への辺なら何もしない．
		if (selected[t]) continue;

		// 最小全域木に辺を追加し，頂点を選んだことを記録しておく．
		if (mst) {
			(*mst)[s].push_back({ t, c });
			(*mst)[t].push_back({ s, c });
		}
		res += c;
		selected[t] = true;

		// 調べるべき辺を追加する．
		repe(e, g[t]) q.push({ e.cost, t, e.to });
	}

	return res;
}


//【全域森】O(n + m)
/*
* 無向グラフ g の全域森を返す．また各全域木の代表元を v に格納する．
*/
Graph spanning_forest(const Graph& g, vi* v = nullptr) {
	// verify : https://atcoder.jp/contests/abc233/tasks/abc233_f

	int n = sz(g);
	Graph sf(n);

	dsu d(n);
	rep(s, n) {
		repe(t, g[s]) {
			if (d.same(s, t)) continue;

			d.merge(s, t);
			sf[s].push_back(t);
			sf[t].push_back(s);
		}
	}

	// 連結成分のそれぞれが全域木なので，その代表元を記録．
	if (v != nullptr) {
		v->clear();
		repe(tmp, d.groups()) v->push_back(tmp[0]);
	}

	return sf;
}


//【最小全域森（圧縮）】O(n + m log n)
/*
* 重み付き無向グラフ g とその頂点集合 vs から，vs を頂点集合にもち，
* 辺 s-t のコストを g における s,t 間の距離と定めた重み付き無向グラフ g2 を構成する．
* g2 の最小全域森を msf に構成し，各最小全域木の代表元を rs に格納し，msf の総コストを返す．
*/
ll compressed_minimum_spanning_forest(const WGraph& g, const vi& vs, WGraph* msf = nullptr, vi* rs = nullptr) {
	// 参考 : https://tokoharuland.hateblo.jp/entry/2018/04/01/155743
	// verify : https://atcoder.jp/contests/abc250/tasks/abc250_h

	int n = sz(g), n2 = sz(vs);

	// nn[s] : g の頂点 s と最も近い vs の頂点が何番目の頂点か
	vi nn(n, -1);

	// dist[s] : g の頂点 s と最も近い vs の頂点との距離
	vl dist(n, INFL);

	// クラスカル法で考慮する g2 の辺の集合
	vector<tuple<ll, int, int>> es;

	// 複数始点ダイクストラ法で用いる優先度付きキュー
	priority_queue_rev<pli> q;
	rep(i, n2) {
		q.emplace(0, vs[i]);
		nn[vs[i]] = i;
		dist[vs[i]] = 0;
	}

	// 複数始点ダイクストラ
	while (!q.empty()) {
		auto [c, s] = q.top(); q.pop();

		if (dist[s] < c) continue;

		repe(e, g[s]) {
			// e.to に最も近い vs の頂点とを結ぶ辺だけを考慮すればいい．
			if (c + e.cost >= dist[e.to]) {
				es.emplace_back(dist[e.to] + c + e.cost, nn[s], nn[e.to]);
				continue;
			}

			dist[e.to] = c + e.cost;
			nn[e.to] = nn[s];

			q.push({ dist[e.to], e.to });
		}
	}

	// クラスカル法で考慮する g2 の辺の集合をコスト昇順にソートする．
	sort(all(es));

	ll cost = 0; // 最小コスト
	dsu d(n2); // 連結判定用
	if (msf != nullptr)	*msf = WGraph(n2);

	for (auto [c, s, t] : es) {
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
	if (rs != nullptr) {
		rs->clear();
		repe(tmp, d.groups()) rs->push_back(tmp[0]);
	}

	return cost;
}


//【全域森に必ず使われる辺】
/*
* 無向グラフ g の全域森に必ず使われる辺は，g の橋の全てである．
*/


//【最小全域森に必ず使われる辺】O(n + m log n)
/*
* 頂点 [0..n) をもち，各 j∈[0..m) についてコスト c[j] の辺 u[j]-v[j] をもつグラフを G とする．
* G の最小全域森全てに共通する辺の番号を昇順に並べたリストを返す．
*
* 利用：【lowlink】
*/
vi required_edge_for_MST(int n, const vi& u, const vi& v, const vl& c) {
	// verify : https://algo-method.com/tasks/1014

	int m = sz(u);

	// 辺をコスト（昇順）ごとに分類する．
	map<ll, vi> edges;
	rep(j, m) edges[c[j]].push_back(j);

	vi res;

	// 今までに追加した辺で連結になる頂点を同一視する．
	dsu d(n);

	for (auto& [c, js] : edges) {
		// 旧頂点番号から新頂点番号への対応
		unordered_map<int, int> i_to_i2; int id_i = 0;

		// 新辺番号から旧辺番号への対応
		vi j2_to_j; int id_j = 0;

		// コスト c の辺のみからなる縮約されたグラフ g2 を作成する．
		IGraph g2;
		repe(j, js) {
			int lu = d.leader(u[j]);
			int lv = d.leader(v[j]);

			if (!i_to_i2.count(lu)) {
				i_to_i2[lu] = id_i++;
				g2.push_back(vector<IEdge>());
			}
			if (!i_to_i2.count(lv)) {
				i_to_i2[lv] = id_i++;
				g2.push_back(vector<IEdge>());
			}
			j2_to_j.push_back(j);

			g2[i_to_i2[lu]].push_back({ i_to_i2[lu], i_to_i2[lv], id_j });
			g2[i_to_i2[lv]].push_back({ i_to_i2[lv], i_to_i2[lu], id_j });
			id_j++;
		}

		// g2 の橋を列挙する．
		Lowlink LL(g2);
		vi bs = LL.get_bridges();
		repe(j2, bs) res.push_back(j2_to_j[j2]);

		// 頂点の同一視を進める
		repe(j, js) d.merge(u[j], v[j]);
	}

	return res;
}


//【全域木の数え上げ】O(n^3)
/*
* 無向グラフ g（多重辺可）の全域木の個数を返す．
*
* 利用：【行列】,【行列式】
*/
mint matrix_tree_theorem(const Graph& g) {
	// 参考 : https://mizuwater0.hatenablog.com/entry/2018/11/25/233547
	// verify : https://atcoder.jp/contests/jsc2021/tasks/jsc2021_g

	int n = sz(g);
	if (n <= 1) return 1;

	// mat : g のラプラシアン行列から最終行と最終列を除いたもの
	//	mat[s][s] : 頂点 s の次数（自己ループは除く）
	//	mat[s][t] : -(頂点 s, t を結ぶ辺の数)
	Matrix<mint> mat(n - 1, n - 1);

	rep(s, n - 1) {
		repe(t, g[s]) {
			if (t == s) continue; // 自己ループは無視

			if (t < n - 1) mat[s][t]--;
			mat[s][s]++;
		}
	}

	// ラプラシアン行列の任意の余因子が全域木の個数を与える．
	return determinant(mat);
}


//【全域森の数え上げ】O(n^3)
/*
* 自己ループのない無向グラフ g（多重辺は可）の全域森の個数を返す．
*
*（行列木定理）
*
* 利用：【行列】,【行列式】,【連結成分分解】
*/
mint count_spanning_forest(const Graph& g) {
	int n = sz(g);
	if (n <= 1) return 1;

	// g を連結成分分解する．
	vvi ccs = connected_component(g);

	// mat : g のラプラシアン行列
	//	mat[s][s] : 頂点 s の次数
	//	mat[s][t] : -(頂点 s, t を結ぶ辺の数)
	Matrix<mint> mat(n, n);
	rep(s, n) {
		mat[s][s] = sz(g[s]);
		repe(t, g[s]) {
			Assert(s != t); // 自己ループは許さない

			mat[s][t]--;
		}
	}

	// 各連結成分に属する代表頂点に対応する行と列を除去する．
	unordered_set<int> avoid;
	repe(cc, ccs) avoid.insert(cc[0]);
	rep(i, n) {
		rep(j, n) {
			if (avoid.count(i) || avoid.count(j)) {
				// クロネッカーのデルタへの書き換えで除去したことにする．
				mat[i][j] = (int)(i == j);
			}
		}
	}

	// ラプラシアン行列の任意の余因子行列の行列式が全域木の個数を与える．
	return determinant(mat);
}


//【最小全域森の数え上げ】O(n^3)
/*
* 自己ループのない重み付き無向グラフ g の最小全域森のコストと個数を返す．
*
* 利用：【全域森の数え上げ】
*/
pair<ll, mint> count_minimum_spanning_forest(const WGraph& g) {
	// verify : https://atcoder.jp/contests/arc018/tasks/arc018_4

	int n = sz(g);

	// 辺をコスト（昇順）ごとに分類する．
	map<ll, vector<pii>> edges;
	rep(s, n) repe(e, g[s]) edges[e.cost].push_back({ s, e.to });

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
			auto [s, t] = st;

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
		//	計算量は O(n^3) から真に改善する．
		cnt *= count_spanning_forest(g2);

		// 頂点の同一視を進める
		repe(st, tmp.second) {
			auto [s, t] = st;

			if (!d.same(s, t)) {
				cost += c;
				d.merge(s, t);
			}
		}
	}

	return make_pair(cost, cnt);
}


//【有向全域木の数え上げ】O(n^3)
/*
* 有向グラフ g（多重辺可）の r を根とする有向全域木の個数を返す．
*
* 利用：【行列】,【行列式】
*/
mint directed_matrix_tree_theorem(const Graph& g, int r) {
	// 参考 : https://atcoder.jp/contests/abc336/editorial/9060
	
	int n = sz(g);
	if (n <= 1) return 1;

	// mat : g の有向ラプラシアン行列から第 r 行と第 r 列を除いたもの
	//	mat[s][s] : 頂点 s の入次数（自己ループは除く）
	//	mat[s][t] : -(辺 t→s の数)
	Matrix<mint> mat(n - 1, n - 1);

	rep(s, n) {
		int s2 = s < r ? s : s - 1;

		repe(t, g[s]) {
			if (t == s || t == r) continue; // 自己ループは無視
			int t2 = t < r ? t : t - 1;

			if (s != r) mat[t2][s2]--;
			mat[t2][t2]++;
		}
	}

	// 有向ラプラシアン行列の (r,r)-余因子が全域木の個数を与える．
	return determinant(mat);
}


//【有向全域木の数え上げ（重み付き）】O(n^3)
/*
* 重み付き有向グラフ g の r を根とする有向全域木の個数を返す（辺の重みは辺の本数と解釈する．）
*
* 利用：【行列】,【行列式】
*/
mint directed_matrix_tree_theorem(const WGraph& g, int r) {
	// 参考 : https://atcoder.jp/contests/abc336/editorial/9060
	// verify : https://atcoder.jp/contests/abc336/tasks/abc336_g

	int n = sz(g);
	if (n <= 1) return 1;

	// mat : g の有向ラプラシアン行列から第 r 行と第 r 列を除いたもの
	//	mat[s][s] : 頂点 s の入次数
	//	mat[s][t] : -(辺 t→s の数)
	Matrix<mint> mat(n - 1, n - 1);

	rep(s, n) {
		int s2 = s < r ? s : s - 1;

		repe(t, g[s]) {
			if (t == s || t == r) continue; // 自己ループは無視
			int t2 = t < r ? t : t - 1;

			if (s != r) mat[t2][s2] -= t.cost;
			mat[t2][t2] += t.cost;
		}
	}

	// 有向ラプラシアン行列の (r,r)-余因子が全域木の個数を与える．
	return determinant(mat);
}


//【最小全域森の結合森】O(m log n)
/*
* クラスカル法で最小全域森を構築する様子を表した根付き森を fst に構築し，
* その根のリストを rs に格納する．
* 葉に近い頂点から順に統合されていく（コストが同じなら同時に統合される）
*/
void mst_tree(const WGraph& g, Graph& fst, vi& rs) {
	int n = sz(g);

	// コストごと（昇順）に辺を集めておく．
	map<ll, vector<pii>> c_to_ab;
	rep(s, n) {
		repe(e, g[s]) {
			c_to_ab[e.cost].push_back({ s, e.to });
		}
	}

	// p[s] : 頂点 s の親
	vi p(n, -1);

	// r[s] : 頂点 r の属する部分木の根
	vi r(n); iota(all(r), 0);

	// 頂点 a の根を返しつつ途中を短絡する．
	function<int(int)> find_root = [&](int a) {
		int ra = r[a];
		if (ra == a) return a;

		ra = find_root(ra);
		r[a] = ra;
		return ra;
	};

	repe(tmp, c_to_ab) {
		// rs : 後で親を設定しないといけない頂点の集合
		unordered_set<int> roots;
		repe(ab, tmp.second) {
			int a, b;
			tie(a, b) = ab;

			int ra = find_root(a), rb = find_root(b);
			if (ra == rb) continue;

			roots.insert(ra);
			roots.insert(rb);

			// 仮の根を設定しておく．
			r[rb] = ra;
		}

		int nn = n;
		repe(s, roots) {
			// rs : s の仮の根または真の根
			int rs = find_root(s);

			// rs が仮の根であった場合
			if (rs < n) {
				// 真の根を作り，rs の根とする．
				r.push_back(nn);
				p.push_back(-1);
				r[rs] = nn++;
				rs = r[rs];
			}

			// 親を設定する．
			p[s] = rs;
		}
		n = nn;
	}

	fst = Graph(n);
	rs.clear();
	rep(i, n) {
		if (p[i] != -1) {
			fst[i].push_back(p[i]);
			fst[p[i]].push_back(i);
		}
		else {
			rs.push_back(i);
		}
	}
}


//【全域森と基本閉路】
/*
* 無向グラフ G=(V,E) の全域森 T を 1 つ固定し，T^c=E-T とおく．
* 各 e∈T^c に対し，E∪{e} にはただ 1 つの基本閉路 C(e) が存在する．
* 
* どの 2 つも辺を共有しない G の閉路の和集合全体を C(G) と表すと，
* 対称差 △ を演算として {C(e) | e∈T^c} は F2 上のベクトル空間 C(G) の基底となる．
*/


//【有向最小全域木】O(m log n)
/*
* 重み付き有向グラフ g の r を根とする有向最小全域木のコストを返す（なければ -1）
*
* 利用：【併合可能遅延ヒープ（モノイド作用付き全順序集合）】
*/
bool leq_dmst(pli a, pli b) { return a >= b; }
pli inf_dmst() { return { -INFL, -1 }; }
pli act_dmst(ll f, pli x) { return { f + x.first, x.second }; }
ll comp_dmst(ll f, ll g) { return f + g; }
ll id_dmst() { return 0; }
ll directed_minimum_spanning_tree(const WGraph& g, int r, WGraph* mst = nullptr) {
	// 参考 : https://joisino.hatenablog.com/entry/2017/01/11/230141
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_B

	int n = sz(g);

	// seen[v] : 頂点 v の走査状態
	const int ROOT = 2, PATH = 1, FREE = 0;
	vi seen(n, FREE);
	seen[r] = ROOT;

	// 閉路の縮約用
	dsu uf(n);

	// qs[t] : t に入ってくる辺の (縮約後のコスト, 始点) をコスト昇順に記録したキュー
	vector<Lazy_skew_heap<pli, leq_dmst, inf_dmst, ll, act_dmst, comp_dmst, id_dmst>> qs(n);
	rep(s, n) repe(e, g[s]) qs[e.to].push({ e.cost, s });

	// 有向最小全域木に使われる辺の (縮約後のコスト，始点) のリスト
	vector<pli> mst_rev(n);

	// 走査中のパス
	vi path;

	// 最小コスト
	ll res = 0;

	function<bool(int)> rf = [&](int t) {
		// 頂点 t を縮約された成分の代表の頂点に置き換える．
		t = uf.leader(t);

		// 根 r と繋がったら終了．
		if (seen[t] == ROOT) {
			repe(v, path) seen[v] = ROOT;
			path.clear();
			return true;
		}
		seen[t] = PATH;
		path.push_back(t);

		// 自己ループを無視する．
		while (!qs[t].empty() && uf.same(qs[t].top().second, t)) qs[t].pop();

		// t に入ってくる辺が無ければ失敗．
		if (qs[t].empty()) return false;

		// t に入ってくるコスト c が最小の辺 s→t を得る．
		ll c; int s;
		tie(c, s) = qs[t].top(); qs[t].pop();

		// s→t を暫定的に使う辺に追加する．
		res += c;
		mst_rev[t] = { c, s };

		// s→t を含めてもサイクルが出来なかったなら s の処理へ．
		if (seen[s] != PATH) return rf(s);

		// s→t を含むサイクルが出来た場合
		int v = t;
		do {
			// サイクル内で v に入ってくる辺 e を使うのをやめ v に外から入ってくる辺 e' を使うなら，
			// 辺 e' のコストが辺 e のコストの分だけ小さくなったものとみなせる．
			qs[v].apply(-mst_rev[v].first);

			// サイクルを縮約していく．
			uf.merge(v, t);
			qs[t].merge(qs[v]);

			// サイクル内の 1 つ前の頂点へ．
			v = uf.leader(mst_rev[v].second);
		} while (!uf.same(v, t));

		// t の属する縮約成分の代表 lt の元に情報を集約する．
		int lt = uf.leader(t);
		qs[lt].merge(qs[t]); // O(1)

		// 改めて t の処理を行う．
		return rf(t);
	};

	// 全ての頂点が根に繋がるまで処理を行う．
	rep(t, n) rf(t);

	return res;
}


//【全域木の列挙】
/*
* 列挙(グラフ).h の【有向木の列挙】で k=n とすればよい．
*/
