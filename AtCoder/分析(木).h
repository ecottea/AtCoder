#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "木DP.h"
#include "最短路.h"
// ■■■■■ 木の性質の分析 ■■■■■


//【木の中心】
/*
* 木 g において，最も遠い点までの距離を最小にする頂点を g の中心という．
* g の中心は g の直径の中点（辺上なら辺の両端点）になる．
* 
* verify : https://www.codechef.com/problems/MIN_UGLY
*/


//【木の直径】O(n)
/*
* 木 g の直径の長さ d を返し，直径の端点間を結ぶパスを path[0..d] に格納する．
* 直径の中点[辺上なら辺の両端点] は path[d/2], path[(d+1)/2] である．
*/
int tree_diameter(const Graph& g, vi* path = nullptr) {
	// verify : https://atcoder.jp/contests/abc221/tasks/abc221_f

	int n = sz(g);

	// 頂点 0 から幅優先探索を行う．
	vi dist(n, INF); // 頂点 0 からの最短距離
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
	int d = -INF, v1 = -1;
	rep(i, n) if (chmax(d, dist[i])) v1 = i;

	// 頂点 v1 から幅優先探索を行う．
	dist.assign(n, INF); // v1 からの最短距離
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
	d = -INF; int v2 = -1;
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


//【重み付き木の直径】O(n)
/*
* 重み付き木 g の直径の長さ d を返し，直径の端点間を結ぶパスを path[0..d] に格納する．
*/
ll tree_diameter(const WGraph& g, vi* path = nullptr) {
	// verify : https://judge.yosupo.jp/problem/tree_diameter

	int n = sz(g);

	// 頂点 0 から幅優先探索を行う．
	vl dist(n, INFL); // 頂点 0 からの最短距離
	dist[0] = 0;
	queue<int> q; // 次に探索する頂点を入れておくキュー
	q.push(0);

	while (!q.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INFL) continue;

			// 頂点 0 からの最短距離を確定する．
			dist[t] = dist[s] + t.cost;

			// 未探索の頂点として t を追加する．
			q.push(t);
		}
	}

	// 頂点 0 からの距離が最も遠い点 v1 を見つける．
	ll d = -INFL; int v1 = -1;
	rep(i, n) if (chmax(d, dist[i])) v1 = i;

	// 頂点 v1 から幅優先探索を行う．
	dist.assign(n, INFL); // v1 からの最短距離
	dist[v1] = 0;
	vi parent(n); // 直前に通ってきた頂点（経路復元用）
	parent[v1] = -1;
	q.push(v1); // 次に探索する頂点を入れておくキュー

	while (!q.empty()) {
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INFL) continue;

			// v からの最短距離を確定する．
			dist[t] = dist[s] + t.cost;
			parent[t] = s;

			q.push(t);
		}
	}

	// 頂点 v1 からの距離が最も遠い点 v2 を見つける．
	d = -INFL; int v2 = -1;
	rep(i, n) if (chmax(d, dist[i])) v2 = i;

	// v1 から v2 への経路を復元する．
	if (path != nullptr) {
		path->clear();

		int v = v2;
		path->push_back(v);
		while (v != v1) {
			v = parent[v];
			path->push_back(v);
		}
	}

	return d;
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
*	木 g に対して，各部分木から重心を取り除く操作を繰り返して得られる根付き木を構築する．
*	s の子が {t} ⇔ s を取り除いてできた {部分木} の重心が {t}
*
* int lca(int v1, int v2) : O(log n)
*	頂点 v1, v2 の最小共通祖先 LCA(v1, v2) を返す．
*	性質：LCA(v1, v2) は元の木の v1-v2 パス上に存在する．
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
			os << "cs:[" << v.cs << "]";
			return os;
		}
#endif
	};

	int n; // 頂点の数
	int rt; // 根
	vector<Node> v; // 頂点

	// 木 g で初期化する．
	Centroid_decomposition(const G& g) : n(sz(g)), rt(-1), v(n) {
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

	// v1 と v2 の最小共通祖先を返す．
	int lca(int v1, int v2) {
		// 木の高さが O(log n) なので，ダブリングを用いず愚直に上っていっても高速
		while (v[v1].dep < v[v2].dep) v2 = v[v2].p;
		while (v[v1].dep > v[v2].dep) v1 = v[v1].p;
		while (v1 != v2) { v1 = v[v1].p; v2 = v[v2].p; }

		return v1;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Centroid_decomposition& cd) {
		rep(i, sz(cd)) os << i << ": " << cd[i] << endl;
		return os;
	}
#endif
};


//【重心からの距離】O(n log n)
/*
* 木 g を重心分解して得られた根付き木 cd について，v の深さ i の先祖から v までの
* g における距離（先祖がいなければ -1）を dist[i][v] に格納して dist を返す．
*
* 利用：【木の重心分解】
*/
vvi distance_from_centroid(const Graph& g, const Centroid_decomposition<Graph>& cd) {
	// verify : https://www.spoj.com/problems/QTREE5/

	int n = sz(g);

	int i_max = 0;
	rep(v, n) chmax(i_max, cd[v].dep);

	vvi dist(i_max + 1, vi(n, -1));

	// cent : 部分木の重心
	rep(cent, n) {
		// c_dep : cent の深さ
		int c_dep = cd[cent].dep;

		function<void(int, int, int)> dfs = [&](int s, int p, int len) {
			dist[c_dep][s] = len;

			// 再帰処理
			repe(t, g[s]) {
				// cent と同じかそれより浅い頂点は無視する．
				if (cd[t].dep <= c_dep || t == p) continue;

				dfs(t, s, len + 1);
			}
		};
		dfs(cent, -1, 0);
	}

	return dist;
}


//【重心からの距離（重み付き木）】O(n log n)
/*
* 重み付き木 g を重心分解して得られた根付き木 cd について，v の深さ i の先祖から v までの
* g における距離（先祖がいなければ -1）を dist[i][v] に格納して dist を返す．
*
* 利用：【木の重心分解】
*/
vvl distance_from_centroid(const WGraph& g, const Centroid_decomposition<WGraph>& cd) {
	// verify : https://codeforces.com/gym/100633/problem/D

	int n = sz(g);

	int i_max = 0;
	rep(v, n) chmax(i_max, cd[v].dep);

	vvl dist(i_max + 1, vl(n, -1));

	// cent : 部分木の重心
	rep(cent, n) {
		// c_dep : cent の深さ
		int c_dep = cd[cent].dep;

		function<void(int, int, ll)> dfs = [&](int s, int p, ll len) {
			dist[c_dep][s] = len;

			// 再帰処理
			repe(t, g[s]) {
				// cent と同じかそれより浅い頂点は無視する．
				if (cd[t].dep <= c_dep || t == p) continue;

				dfs(t, s, len + t.cost);
			}
		};
		dfs(cent, -1, 0);
	}

	return dist;
}


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


//【重み付き木上のシュタイナー木】O(n)
/*
* 重み付き木 g の頂点集合 v を含む最小の木を st に構築し，その合計コストを返す．
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
	if (n == 1) return vi{ 0 };

	// 次数を求めておく．
	vi degree(n);
	rep(i, n) repe(t, g[i]) degree[t]++;

	// 次数が 1 の頂点から順に取り除いていく．
	queue<pii> q;
	rep(i, n) if (degree[i] == 1) q.push({ i, 0 });

	while (!q.empty()) {
		auto [s, d] = q.front(); q.pop();

		lv[s] = d;

		repe(t, g[s]) {
			// 頂点 s を取り除き，t の次数を更新する．
			degree[t]--;

			// 新たに次数 1 の頂点が生まれたらキューに追加する．
			if (degree[t] == 1) q.push({ t, d + 1 });
		}
	}

	return lv;
}


//【半径 d の木による最小被覆】O(n)
/*
* 木 g を g から誘導される半径 d の木で最小被覆するときの，各部分木の中心のリストを返す．
*/
vi subtree_covering_radius(const Graph& g, int d) {
	// verify : https://atcoder.jp/contests/arc116/tasks/arc116_e

	int n = sz(g);
	vi res;

	// 便宜上 0 を根とする部分木とみなす．
	// dp[s] : 部分木 s の被覆されていない頂点の最大深さ
	//         根から親の方の頂点を被覆できるなら負の値をとる．
	vi dp(n);

	function<void(int, int)> dfs = [&](int s, int p) {
		// 最も浅い中心を得る．
		int dp_min = INF;
		repe(t, g[s]) {
			if (t == p) continue;
			dfs(t, s);
			chmin(dp_min, dp[t]);
		}

		// s が葉の場合の例外処理
		if (dp_min == INF) {
			dp[s] = 0;
			return;
		}

		dp[s] = dp_min + 1;
		repe(t, g[s]) {
			if (t == p) continue;

			// 部分木 t 内で被覆されていない頂点であっても，
			// 別の部分木内にある中心から距離 d 以内にあるなら無視してよい．
			if (dp[t] + dp_min <= -3) continue;

			// さもなくばそれは部分木 s の被覆されていない頂点となる．
			chmax(dp[s], dp[t] + 1);
		}

		// s を中心として選ぶ場合
		if (dp[s] == d) {
			res.emplace_back(s);
			dp[s] = -(d + 1);
		}
	};
	dfs(0, -1);

	// 根 0 がまだ被覆されていない場合
	if (dp[0] >= 0) res.emplace_back(0);

	return res;
}


//【根付き木の頂点の深さ】O(n)
/*
* 各 s∈[0..n) について，r を根とする木 g の頂点 s の深さを格納したリストを返す．
* s の深さとは，根から s までの辺の本数のことである．
*
* 利用：【配る木 DP】
*/
using T_dot = int;
T_dot apply_dot(const T_dot& x, int s, int t) { return x + 1; }
T_dot root_dot(int r) { return 0; }
vector<T_dot> depth_of_tree(const Graph& g, int r) {
	// verify : https://algo-method.com/tasks/529

	return tree_giveDP<T_dot, apply_dot, root_dot>(g, r);
}


//【根付き木の重さ】O(n)
/*
* 各 s∈[0..n) について，r を根とする木 g の頂点 s の重さを格納したリストを返す．
* s の重さとは，部分木 s に含まれる辺の本数（s 自身を除く子孫の数）のことである．
*
* 利用：【貰う木 DP（頂点マージ）】
*/
using T_wot = int;
void merge_wot(T_wot& x, const T_wot& y, int s) { x += y; }
T_wot e_wot() { return 0; }
T_wot leaf_wot(int s) { return 0; }
T_wot apply_wot(const T_wot& x, int p, int s) { return x + 1; }
vector<T_wot> weight_of_tree(const Graph& g, int r) {
	// verify : https://algo-method.com/tasks/434

	return tree_getDP_vmerge<T_wot, merge_wot, e_wot, leaf_wot, apply_wot>(g, r);
}


//【根付き木の高さ】O(n)
/*
* 各 s∈[0..n) について，r を根とする木 g の頂点 s の高さを格納したリストを返す．
* s の高さとは，s から部分木 s の葉までの辺の本数の最大値のことである．
*
* 利用：【貰う木 DP（森経由）】
*/
using T_hot = int;
void merge_hot(T_hot& x, const T_hot& y) { chmax(x, y); }
T_hot e_hot() { return -INF; }
T_hot leaf_hot(int s) { return 0; }
void apply_hot(T_hot& x, int s) { x++; }
vector<T_hot> height_of_tree(const Graph& g, int r) {
	// verify : https://algo-method.com/tasks/528

	return tree_getDP_forest<T_hot, merge_hot, e_hot, leaf_hot, apply_hot>(g, r);
}


//【根付き木の高さ（重み付き）】O(n)
/*
* 各 s∈[0..n) について，r を根とする重み付き木 g の頂点 s の高さを格納したリストを返す．
* s の高さとは，s から部分木 s の葉までの辺のコストの和の最大値のことである．
*
* 利用：【貰う木 DP（頂点マージ，重み付き）】
*/
using T_hoct = ll;
void merge_hoct(T_hoct& x, const T_hoct& y, int s) { chmax(x, y); }
T_hoct e_hoct() { return 0; }
T_hoct leaf_hoct(int s) { return 0; }
T_hoct apply_hoct(const T_hoct& x, int s, int t, ll c) { return x + c; }
vector<T_hoct> height_of_weighted_tree(const WGraph& g, int r) {
	return tree_getDP_vmerge<T_hoct, merge_hoct, e_hoct, leaf_hoct, apply_hoct>(g, r);
}


//【木の高さ】O(n)
/*
* 与えられた重み付き木 g に対し，各 s∈[0..n) について
* 頂点 s を根にしたときの高さ（最も遠い葉までのコスト）を格納したリストを返す．
*
* 利用：【全方位木 DP】
*/
using T_hut = int;
T_hut merge_hut(T_hut x, T_hut y, int s) { return max(x, y); }
T_hut e_hut(int s) { return 0; }
T_hut leaf_hut(int s) { return 0; }
T_hut apply_hut(T_hut x, int s, int t) { return x + 1; }
vi height_of_undirected_tree(Graph& g) {
	return rerooting<T_hut, merge_hut, e_hut, leaf_hut, apply_hut>(g);
}


//【木の高さ（重み付き）】O(n)
/*
* 与えられた重み付き木 g に対し，各 s∈[0..n) について
* 頂点 s を根にしたときの高さ（最も遠い葉までのコスト）を格納したリストを返す．
*
* 利用：【全方位木 DP（重み付き）】
*/
using T_hutc = ll;
T_hutc merge_hutc(T_hutc x, T_hutc y, int s) { return max(x, y); }
T_hutc e_hutc(int s) { return 0; }
T_hutc leaf_hutc(int s) { return 0; }
T_hutc apply_hutc(T_hutc x, int p, int s, ll c) { return x + c; }
vl height_of_undirected_tree(const WGraph& g) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_B

	return rerooting<T_hutc, merge_hutc, e_hutc, leaf_hutc, apply_hutc>(g);
}


//【部分木の大きさ】O(n)
/*
* 与えられた木 g に対し，各 s∈[0..n) および s に隣接する各頂点 t について，
* s-t 間の辺を切断し t を根と見たときの部分木の頂点数を格納した二次元リストを返す．
*
* 利用：【全方位部分木 DP】
*/
using T_ss = int;
T_ss merge_ss(T_ss x, T_ss y, int s) { return x + y - 1; }
T_ss e_ss(int s) { return 1; }
T_ss leaf_ss(int s) { return 1; }
T_ss apply_ss(T_ss x, int p, int s) { return x + 1; }
vvi subtree_size(Graph& g) {
	// verify : https://atcoder.jp/contests/abc149/tasks/abc149_f

	vvi res;
	rerooting<T_ss, merge_ss, e_ss, leaf_ss, apply_ss>(g, &res);

	return res;
}


