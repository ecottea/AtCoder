#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "Union-Find.h"
// ■■■■■ グラフのクエリ処理 ■■■■■


//【隣接頂点クエリ（可換モノイド）】
/*
* Neighbor_vertex_query<S, op, o>(Graph g) : O(n + m)
*	グラフ g と初期値 v[0..n) = o() で初期化する．
*	要素は可換モノイド (S, op, o) の元とする．
*
* Neighbor_vertex_query<S, op, o>(Graph g, vS v) : O(n + m)
*	グラフ g と初期値 v[0..n) で初期化する．
*
* add(int i, S x) : O(1)
*	v[i] += x とする．
*
* S get(int i) : O(√m)
*	v[i] を返す．
*
* neighbor_add(int s, S x) : O(√m)
*	辺 s→t をもつ全ての頂点 t に対して v[t] += x とする．
*
* vS get_all() : O(n + m)
*	v[0..n) を返す．
*/
template <class S, S(*op)(S, S), S(*o)()>
class Neighbor_vertex_query {
	using vS = vector<S>;

	int n; // 頂点数

	int th; // 頂点の出次数が大きいかどうかの閾値
	Graph gl;     // 出次数が th 未満の頂点から出る辺のみからなるグラフ
	Graph gh_rev; // 出次数が th 以上の頂点から出る辺のみからなるグラフの逆グラフ

	vS v; // 各頂点の値
	vS lazy; // 各頂点から撒く値

public:
	// グラフ g と初期値 v[0..n) = o() で初期化する．
	Neighbor_vertex_query(const Graph& g) : n(sz(g)), gl(n), gh_rev(n), v(n, o()), lazy(n, o()) {
		// m : g の辺の数
		int m = 0;
		rep(s, n) m += sz(g[s]);

		// 頂点の出次数が大きいかどうかの閾値 th を決定
		th = (int)(sqrt(m) + 0.001);

		// 出次数が th 未満か以上かで g の辺を分割する．
		rep(s, n) repe(t, g[s]) {
			if (sz(g[s]) < th) gl[s].push_back(t);
			else gh_rev[t].push_back(s);
		}
	}

	// グラフ g と初期値 v[0..n) で初期化する．
	Neighbor_vertex_query(const Graph& g, const vS& v_) : Neighbor_vertex_query(g) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_ce

		v = v_;
	}
	Neighbor_vertex_query() {}

	// v[i] += x とする．
	void add(int i, S x) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_ce
		
		v[i] = op(v[i], x);
	}

	// v[i] を返す．
	S get(int i) const {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_ce
		
		// 自身に記録されている値
		S res = v[i];

		// 辺 s→i をもち出次数の大きい頂点 s から撒かれた値を加算する．
		repe(s, gh_rev[i]) res = op(res, lazy[s]);

		return res;
	}

	// 辺 s→t をもつ全ての頂点 t に対して v[t] += x とする．
	void neighbor_add(int s, S x) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_ce
		
		// s の出次数が大きい場合
		if (gl[s].empty()) {
			// 隣接頂点に数を撒いたことを記録する．
			lazy[s] = op(lazy[s], x);
		}
		// s の出次数が小さい場合
		else {
			// 実際に隣接頂点に値を加算する．
			repe(t, gl[s]) v[t] = op(v[t], x);
		}
	}

	// v[0..n) を返す．
	vS get_all() const {
		auto res(v);
		rep(t, n) repe(s, gh_rev[t]) res[t] = op(res[t], lazy[s]);
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Neighbor_vertex_query NAQ) {
		os << NAQ.get_all();
		return os;
	}
#endif
};


//【隣接更新クエリ】
/*
* t 回目のクエリのとき {t, val} として【隣接加算クエリ】で max モノイドを使えば良い．
* 
* verify : https://atcoder.jp/contests/typical90/tasks/typical90_ce
*/


//【隣接頂点クエリ（可換 M-集合）】
/*
* Neighbor_vertex_query<S, F, act, comp, id>(Graph g, vS v) : O(n + m)
*	グラフ g と初期値 v[0..n) = o() で初期化する．
*	要素は可換モノイド作用付き集合 (S, F, act, comp, id) の元とする．
*
* S get(int i) : O(√m)
*	v[i] を返す．
*
* apply(int i, F f) : O(1)
*	v[i] = f( v[i] ) とする．
*
* neighbor_apply(int s, F f) : O(√m)
*	辺 s→t をもつ全ての頂点 t に対して v[i] = f( v[i] ) とする．
*
* vS get_all() : O(n + m)
*	v[0..n) を返す．
*/
template <class S, class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Neighbor_vertex_query {
	using vS = vector<S>;
	using vF = vector<F>;

	int n; // 頂点数

	int th; // 頂点の出次数が大きいかどうかの閾値
	Graph gl;     // 出次数が th 未満の頂点から出る辺のみからなるグラフ
	Graph gh_rev; // 出次数が th 以上の頂点から出る辺のみからなるグラフの逆グラフ

	vS v; // 各頂点の値
	vF lazy; // 各頂点から撒く作用

public:
	// グラフ g と初期値 v[0..n) で初期化する．
	Neighbor_vertex_query(const Graph& g, const vS& v) : n(sz(g)), gl(n), gh_rev(n), v(v), lazy(n, id()) {
		// m : g の辺の数
		int m = 0;
		rep(s, n) m += sz(g[s]);

		// 頂点の出次数が大きいかどうかの閾値 th を決定
		th = (int)(sqrt(m) + 0.001);

		// 出次数が th 未満か以上かで g の辺を分割する．
		rep(s, n) repe(t, g[s]) {
			if (sz(g[s]) < th) gl[s].push_back(t);
			else gh_rev[t].push_back(s);
		}
	}
	Neighbor_vertex_query() {}

	// v[i] = f( v[i] ) とする．
	void apply(int i, F f) {
		v[i] = act(f, v[i]);
	}

	// v[i] を返す．
	S get(int i) const {
		// 自身に記録されている値
		S res = v[i];

		// 辺 s→i をもち出次数の大きい頂点 s からの作用を施す．
		repe(s, gh_rev[i]) res = act(lazy[s], res);

		return res;
	}

	// 辺 s→t をもつ全ての頂点 t に対して v[i] = f( v[i] ) とする．
	void neighbor_apply(int s, F f) {
		// s の出次数が大きい場合
		if (gl[s].empty()) {
			// 隣接頂点に数を撒いたことを記録する．
			lazy[s] = comp(f, lazy[s]);
		}
		// s の出次数が小さい場合
		else {
			// 実際に隣接頂点に値を加算する．
			repe(t, gl[s]) v[t] = act(f, v[t]);
		}
	}

	// v[0..n) を返す．
	vS get_all() const {
		auto res(v);
		rep(t, n) repe(s, gh_rev[t]) res[t] = act(lazy[s], res[t]);
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Neighbor_vertex_query NAQ) {
		os << NAQ.get_all();
		return os;
	}
#endif
};


//【offline dynamic connectivity】
/*
* Offline_dynamic_connectivity(int n) : O(1)
*	n 頂点の空の無向グラフで初期化する．
*
* add_edge(int u, int v) : O(1)
*	辺 u-v を追加する．多重辺も可．
*
* erase_edge(int u, int v) : O(1)
*	辺 u-v を削除する．
*
* add_query(int id) : O(1)
*	識別番号 id のクエリを追加する．
*
* solve(const function<void(int)>& f) : O(Q log Q log n)
*	各クエリに対する答えを一括計算する．
*	f(id) は識別番号 id のクエリが追加された時点でのグラフの状態に対する答えの計算を行う．
*/
class Offline_dynamic_connectivity {
	// 参考 : https://ei1333.github.io/luzhiled/snippets/other/offline-dynamic-connectivity.html
	// verify : https://atcoder.jp/contests/abc301/tasks/abc301_h

	int T = 1; // 現在時刻

	vector<pii> es; // 管理すべき無向辺 e = u-v のリスト（u < v）
	vector<unordered_map<int, int>> e_id; // 辺の ID

	vi e_cnt; // 各辺の本数
	vi appear; // 各辺が出現した時刻
	vector<tuple<int, int, int>> ex; // 各辺の存在していた時刻の半開区間と ID

	vvi qs; // 各時刻に処理すべきクエリのリスト

	vvi seg; // 辺の存在区間を管理するセグメント木

	// 時刻 [l..r) に辺 id が存在したことを記録する．
	void add_interval(int l, int r, int id) {
		l += T;
		r += T;

		while (l < r) {
			if (l & 1) {
				seg[l].emplace_back(id);
				l++;
			}
			if (r & 1) {
				seg[r - 1].emplace_back(id);
			}

			l >>= 1;
			r >>= 1;
		}
	}

	// セグ木を再帰的になぞる．
	void rf(int k, const function<void(int)>& f) {
		uf.snapshot();

		repe(id, seg[k]) {
			auto [u, v] = es[id];
			uf.merge(u, v);
		}

		if (k < T) {
			rf(2 * k, f);
			rf(2 * k + 1, f);
		}
		else {
			repe(qid, qs[k - T]) f(qid);
		}

		uf.rollback();
	}

public:
	int n; // 頂点数

	Rollback_Union_find uf;

	// n 頂点の空グラフで初期化する．
	Offline_dynamic_connectivity(int n) : e_id(n), qs(1), n(n), uf(n) {}
	Offline_dynamic_connectivity() : n(0) {}

	// 辺 u-v を追加する．
	void add_edge(int u, int v) {
		if (u > v) swap(u, v);

		auto it = e_id[u].find(v);
		if (it == e_id[u].end()) {
			e_id[u][v] = sz(es);
			e_cnt.emplace_back(1);
			appear.emplace_back(T);
			es.emplace_back(u, v);
		}
		else {
			int id = it->second;
			if (e_cnt[id] == 0) appear[id] = T;
			e_cnt[id]++;
		}

		qs.emplace_back(vi());
		T++;
	}

	// 辺 u-v を削除する．
	void erase_edge(int u, int v) {
		if (u > v) swap(u, v);

		Assert(e_id[u].count(v));
		int id = e_id[u][v];
		Assert(e_cnt[id] > 0);

		e_cnt[id]--;
		if (e_cnt[id] == 0) ex.emplace_back(appear[id], T, id);

		qs.emplace_back(vi());
		T++;
	}

	// 識別番号 id のクエリを追加する．
	void add_query(int id) {
		qs.back().emplace_back(id);
	}

	// 各クエリに対する答えを一括計算する．
	void solve(const function<void(int)>& f) {
		// 辺の存在区間を管理するセグメント木を構築する．
		seg.resize(T * 2);
		for (auto& [l, r, id] : ex) add_interval(l, r, id);
		rep(id, sz(e_cnt)) if (e_cnt[id] > 0) add_interval(appear[id], T, id);

		// セグ木を再帰的になぞりながら各クエリに対する答えを計算する．
		rf(1, f);
	}
};


