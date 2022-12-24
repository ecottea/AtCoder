#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "最短路.h"
// ■■■■■ 木の性質の分析 ■■■■■


//【木の直径】O(n)
/*
* 木 g の直径の長さ d を返し，直径の端点間を結ぶパスを path[0..d] に格納する．
* 直径の端点は path[0], path[d] である．
* 直径の中点[辺上なら辺の両端点] は path[d/2], path[(d+1)/2] である．
*/
int tree_diameter(const Graph& g, vi* path = nullptr) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_c
	// verify : https://atcoder.jp/contests/abc221/tasks/abc221_f

	int n = sz(g);

	// 頂点 0 から幅優先探索を行う．
	vi dist(n, INF); // 頂点 0 からの最短距離：O(n)
	dist[0] = 0;
	queue<int> q; // 次に探索する頂点を入れておくキュー
	q.push(0);

	while (!q.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INF) continue;

			// 頂点 0 からの最短距離を確定する．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			q.push(t);
		}
	}

	// 頂点 0 からの距離が最も遠い点 v1 を見つける．
	int d = -INF, v1 = 0;
	rep(i, n) if (chmax(d, dist[i])) v1 = i;

	// 頂点 v1 から幅優先探索を行う．
	dist.assign(n, INF); // v1 からの最短距離：O(n)
	dist[v1] = 0;
	vi parent(n); // 直前に通ってきた頂点（経路復元用）
	parent[v1] = -1;
	q.push(v1); // 次に探索する頂点を入れておくキュー

	while (!q.empty()) {
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INF) continue;

			// v からの最短距離を確定する．
			dist[t] = dist[s] + 1;
			parent[t] = s;

			q.push(t);
		}
	}

	// 頂点 v1 からの距離が最も遠い点 v2 を見つける．
	d = -INF; int v2 = v1;
	rep(i, n) if (chmax(d, dist[i])) v2 = i;

	// v1 から v2 への経路を復元する．
	if (path != nullptr) {
		path->resize(d + 1);

		int v = v2;
		(*path)[d] = v;
		repir(i, d - 1, 0) (*path)[i] = parent[(*path)[i + 1]];
	}

	return d;
}


//【コスト付き木の直径】O(n log n)
/*
* コスト付き木の直径の長さを返す．また直径の両端となる頂点の組を p に格納する．
*
* 利用：【単一始点最短路／ダイクストラ法】
*/
ll tree_diameter(const WGraph& g, pii& p) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_A

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

	// s を始点にして最遠の点 t を求めれば，s と t の距離が木の直径である．
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


//【木の重心】O(n)
/*
* 木 g の重心を返し，もう 1 つ重心がある場合はそれを c2 に格納する（なければ -1）
* 木 g の重心とは，その頂点を取り除いてできる部分木の大きさが全て |g|/2 以下になる点である．
*/
template <class G>
int tree_centroid(const G& g, int* c2 = nullptr) {
	// 参考 : https://qiita.com/drken/items/4b4c3f1824339b090202
	// verify : https://atcoder.jp/contests/agc018/tasks/agc018_d

	int n = sz(g), hn = n / 2;

	int centroid = -1;
	if (c2 != nullptr) *c2 = -1;

	// 0 を根とする部分木 s に含まれる頂点の個数を返す．（p : s の親）
	function<int(int, int)> dfs = [&](int s, int p) {
		// s_cnt : 部分木 s の大きさ
		int s_cnt = 1;

		// ok : 頂点 s が重心か
		bool ok = true;

		// s の子 t を調べる．
		repe(t, g[s]) {
			if (t == p) continue;

			// t_cnt : 部分木 t の大きさ
			int t_cnt = dfs(t, s);

			// 大きさが |g|/2 を超える部分木があれば s は重心ではない．
			if (t_cnt > hn) ok = false;

			// 部分木 t の大きさを加える．
			s_cnt += t_cnt;
		}

		// p を含む部分木の大きさが |g|/2 を超えていれば s は重心ではない．
		if (n - s_cnt > hn) ok = false;

		// s は重心なのでそれを記録する．
		if (ok) {
			if (centroid == -1) centroid = s;
			else if (c2 != nullptr) *c2 = s;
		}

		return s_cnt;
	};

	dfs(0, -1);

	return centroid;
}


//【木の重心分解】
/*
* Centroid_decomposition<G>(G g) : O(n log n)
*	木 g に対して，各部分木から重心を取り除く操作を繰り返して得られる木構造を構築する．
*	s の子が {t} ⇔ s を取り除いてできた {部分木} の重心が {t}
*
* 性質：∀u,v∈V について，LCA(u,v) は u-v パス上に存在する．
*/
template <class G>
struct Centroid_decomposition {
	// 参考 : https://ferin-tech.hatenablog.com/entry/2020/03/06/162311

	struct Node {
		int size = -1; // この頂点を重心とする部分木の大きさ
		int dep = -1; // この頂点が何回目の操作で取り除かれたか
		int p = -1; // 親（なければ -1）
		vi cs; // 子のリスト

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "size:" << v.size << ", ";
			os << "dep:" << v.dep << ", ";
			os << "p:" << v.p << ", ";
			os << "cs:" << v.cs;
			return os;
		}
#endif
	};

	int n; // 頂点の数
	int rt; // 根
	vector<Node> v; // 頂点

	// 木 g で初期化する．
	Centroid_decomposition(const G& g) : n(sz(g)), v(n) {
		// verify : https://codeforces.com/contest/342/problem/E

		// cnt[v] : 部分木 v の大きさ（使いまわす．根はその都度直前に取り除かれた重心に変わる）
		vi cnt(n);

		// bc を根としたときの部分木 s の大きさ cnt[s] を更新し，既に重心を発見しているかを返す．
		//	p : bc を根としたときの s の親
		//	r_size : s を含む部分木全体の大きさ
		//	dep : 何回目の処理か
		//	bc : 直前に取り除かれた重心
		function<bool(int, int, int, int, int)> dfs = [&](int s, int p, int r_size, int dep, int bc) {
			// 部分木 s の大きさ
			cnt[s] = 1;

			// ok : 頂点 s が重心か
			bool ok = true;

			// s の子 t を調べる．
			repe(t, g[s]) {
				// 親や既に取り除かれた頂点には進まない．
				if (t == p || v[t].dep != -1) continue;

				// 部分木 t の大きさを求める（既に重心を発見していればすぐに帰る）
				if (dfs(t, s, r_size, dep, bc)) return true;

				// 大きさが元の半分を超える部分木があれば s は重心ではない．
				if (cnt[t] > r_size / 2) ok = false;

				// 部分木 t の大きさを加える．
				cnt[s] += cnt[t];
			}

			// p を含む部分木の大きさが元の半分を超えていれば s は重心ではない．
			if (r_size - cnt[s] > r_size / 2) ok = false;

			// s は重心なのでそれを記録し取り除く
			if (ok) {
				// s の情報を決定する．
				v[s].size = r_size;
				v[s].dep = dep;
				v[s].p = bc;
				if (bc != -1) v[bc].cs.push_back(s);
				else rt = s;

				// s の親 p を含む部分木を重心分解する．
				if (p != -1) dfs(p, -1, r_size - cnt[s], dep + 1, s);

				// s の各子 t を含む部分木を重心分解する．
				repe(t, g[s]) {
					if (t == p || v[t].dep != -1) continue;

					dfs(t, -1, cnt[t], dep + 1, s);
				}

				return true;
			}

			return false;
		};

		dfs(0, -1, sz(g), 0, -1);
	}
	Centroid_decomposition() : n(0), rt(-1) {}

	// アクセス
	Node const& operator[](int i) const { return v[i]; }
	Node& operator[](int i) { return v[i]; }

	// 大きさ
	int size() const { return n; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Centroid_decomposition& cd) {
		rep(i, sz(cd)) os << i << ": " << cd[i] << endl;
		return os;
	}
#endif
};


//【木上のシュタイナー木】O(n)
/*
* 木 g の頂点集合 v を含む最小の木を st に構築し，その大きさを返す．
* また st の頂点 i が g のどの頂点と対応するかを id[i] に格納する．
*/
int steiner_tree(const Graph& g, const vi& v, Graph& st, vi& id) {
	// verify : https://atcoder.jp/contests/arc030/tasks/arc030_2

	if (v.empty()) {
		st.clear();
		id.clear();
		return 0;
	}

	int n = sz(g);

	vb sel(n); // v に属するか
	repe(s, v) sel[s] = true;

	vi deg(n); // 次数
	queue<int> q_deg1; // 次数 1 の頂点を入れておくキュー

	rep(s, n) {
		deg[s] = sz(g[s]);
		if (!sel[s] && deg[s] == 1) q_deg1.push(s);
	}

	// 次数 1 で v に属さない頂点を次々に除去していく．
	int m = n;
	while (!q_deg1.empty()) {
		int s = q_deg1.front();
		q_deg1.pop();

		deg[s] = -1; // st から除外の意味とする
		m--;

		repe(t, g[s]) {
			if (deg[t] > 0) deg[t]--;
			if (!sel[t] && deg[t] == 1) q_deg1.push(t);
		}
	}

	st.resize(m);
	id.resize(m);
	vi id_inv(n);
	queue<int> q_st;

	q_st.push(v[0]);
	id[0] = v[0];
	id_inv[v[0]] = 0;
	int i = 1;

	// 幅優先探索で st の頂点をなぞりつつ新たな木を構築する．
	while (!q_st.empty()) {
		int s = q_st.front();
		q_st.pop();

		repe(t, g[s]) {
			if (deg[t] >= 0) {
				id[i] = t;
				id_inv[t] = i;

				st[id_inv[s]].push_back(i);
				st[i].push_back(id_inv[s]);

				q_st.push(t);
				i++;
			}
		}

		deg[s] = -2; // 探索終了の意味とする
	}

	return m;
}


//【コスト付き木上のシュタイナー木】O(n)
/*
* コスト付き木 g の頂点集合 v を含む最小の木を st に構築し，その合計コストを返す．
* また st の頂点 i が g のどの頂点と対応するかを id[i] に格納する．
*/
ll steiner_tree(const WGraph& g, const vi& v, WGraph& st, vi& id) {
	int n = sz(g);

	vb sel(n); // v に属するか
	repe(s, v) sel[s] = true;

	vi deg(n); // 次数
	queue<int> q_deg1; // 次数 1 の頂点を入れておくキュー

	rep(s, n) {
		deg[s] = sz(g[s]);
		if (!sel[s] && deg[s] == 1) q_deg1.push(s);
	}

	// 次数 1 で v に属さない頂点を次々に除去していく．
	int m = n;
	while (!q_deg1.empty()) {
		int s = q_deg1.front();
		q_deg1.pop();

		deg[s] = -1; // st から除外の意味とする
		m--;

		repe(t, g[s]) {
			if (deg[t] > 0) deg[t]--;
			if (!sel[t] && deg[t] == 1) q_deg1.push(t);
		}
	}

	st.resize(m);
	id.resize(m);
	vi id_inv(n);
	queue<int> q_st;

	q_st.push(v[0]);
	id[0] = v[0];
	id_inv[v[0]] = 0;
	int i = 1;

	ll cost = 0;

	// 幅優先探索で st の頂点をなぞりつつ新たな木を構築する．
	while (!q_st.empty()) {
		int s = q_st.front();
		q_st.pop();

		repe(t, g[s]) {
			if (deg[t] >= 0) {
				id[i] = t;
				id_inv[t] = i;

				st[id_inv[s]].push_back({ i, t.cost });
				st[i].push_back({ id_inv[s], t.cost });
				cost += t.cost;

				q_st.push(t);
				i++;
			}
		}

		deg[s] = -2; // 探索終了の意味とする
	}

	return cost;
}


//【葉の削除回数】O(n)
/*
* 木 g に対し葉の削除を繰り返したとき何回目に頂点 i が削除されるかを lv[i] に格納し lv を返す．
*
*（葉からの幅優先探索）
*/
vi leaf_remove_level(const Graph& g) {
	int n = sz(g);
	vi lv(n);

	// 木が 1 頂点のみで次数 1 の頂点が存在しない場合の例外処理
	if (n == 1) {
		lv[0] = 0;
		return;
	}

	// 次数を求めておく．
	vi degree(n);
	rep(i, n) {
		repe(t, g[i]) {
			degree[t]++;
		}
	}

	// 次数が 1 の頂点から順に取り除いていく．
	queue<pii> q;
	rep(i, n) {
		if (degree[i] == 1) {
			q.push({ i, 0 });
		}
	}

	while (!q.empty()) {
		int s, d;
		tie(s, d) = q.front();
		q.pop();

		lv[s] = d;

		repe(t, g[s]) {
			// 頂点 s を取り除き，t の次数を更新する．
			degree[t]--;

			// 新たに次数 1 の頂点が生まれたらキューに追加する．
			if (degree[t] == 1) {
				q.push({ t, d + 1 });
			}
		}
	}

	return lv;
}


