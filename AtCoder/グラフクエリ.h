#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフのクエリ処理 ■■■■■


//【隣接作用／一点参照クエリ（可換モノイド）】
/*
* Neighbor_add_query<S, op, o>(Graph g) : O(n + m)
*	グラフ g と初期値 o() で初期化する．
*	要素は可換モノイド (S, op, o) の元とする．
*
* Neighbor_add_query<S, op, o>(Graph g, vS v) : O(n + m)
*	グラフ g と初期値 v[0..n) で初期化する．
*
* add(int i, S x) : O(1)
*	v[i] += x とする．
*
* S get(int i) : O(√m)
*	v[i] を返す．
*
* spread(int s, S x) : O(√m)
*	s の隣接頂点 t 全てに対して v[t] += x とする．
*/
template <class S, S(*op)(S, S), S(*o)()>
class Neighbor_add_query {
	using vS = vector<S>;

	int n; // 頂点数
	Graph g; // グラフ

	int th; // 頂点の次数が大きいかどうかの閾値
	Graph gl; // 隣接する次数が大きい頂点だけを記録したグラフ

	vS v; // 各頂点の値
	vS lazy; // 各頂点から撒く値

public:
	// グラフ g と単位元 o() で初期化
	Neighbor_add_query(const Graph& g_) : n(sz(g_)), g(g_), gl(n), v(n, o()), lazy(n, o()) {
		// m : g の辺の数
		int m = 0;
		rep(s, n) m += sz(g[s]);

		// 頂点の次数が大きいか小さいかの閾値 th を決定
		th = (int)(sqrt(m) + 0.001);

		// 隣接する大きい頂点だけを記録したグラフ gl を作成
		rep(s, n) repe(t, g[s]) if (sz(g[t]) > th) gl[s].push_back(t);
	}

	// グラフ g と配列 a[0..n) で初期化
	Neighbor_add_query(const Graph& g, const vector<S>& v_) : Neighbor_add_query(g) { v = v_; }
	Neighbor_add_query() {}

	// v[i] += x とする．
	void add(int i, S x) {
		v[i] = op(v[i], x);
	}

	// v[i] を返す．
	S get(int i) {
		// i の次数が大きい場合は v[i] に正しい値が入っている．
		if (sz(g[i]) > th) return v[i];

		// i の次数が小さい場合は周りから撒かれた値の和を計算する．
		S res = o();
		repe(j, g[i]) res = op(lazy[j], res);

		return op(v[i], res);
	}

	// s の隣接頂点 t 全てに対して v[t] += x とする．
	void spread(int s, S x) {
		// 隣接する次数が大きい頂点の値の更新
		repe(t, gl[s]) v[t] = op(x, v[t]);

		// 隣接頂点に数を撒いたことを記録
		lazy[s] = op(x, lazy[s]);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Neighbor_add_query npq) {
		rep(i, npq.n) os << npq.get(i) << " ";
		return os;
	}
#endif
};


//【隣接作用／一点参照クエリ（アーベル群）】
/*
* Neighbor_add_set_query<S, op, o, inv>(Graph g) : O(n + m)
*	グラフ g と初期値 o() で初期化する．
*	要素はアーベル群 (S, op, o, inv) の元とする．
*
* Neighbor_add_set_query<S, op, o, inv>(Graph g, vS v) : O(n + m)
*	グラフ g と初期値 v[0..n) で初期化する．
*
* add(int i, S x) : O(1)
*	v[i] += x とする．
*
* set(int i, S x) : O(√m)
*	v[i] = x とする．
*
* S get(int i) : O(√m)
*	v[i] を返す．
*
* spread(int s, S x) : O(√m)
*	s の隣接頂点 t 全てに対して v[t] += x とする．
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
class Neighbor_add_set_query {
	using vS = vector<S>;

	int n; // 頂点数
	Graph g; // グラフ

	int th; // 頂点の次数が大きいかどうかの閾値
	Graph gl; // 隣接する次数が大きい頂点だけを記録したグラフ

	vS v; // 各頂点の値
	vS lazy; // 各頂点から撒く値

	// i に隣接する頂点から撒かれた値の総和を返す．（i の次数が大きければ o() を返す．）
	S gather(int i) {
		if (sz(g[i]) > th) return o();

		S res = o();
		repe(j, g[i]) res = op(lazy[j], res);
		return res;
	}

public:
	// グラフ g と単位元 o() で初期化
	Neighbor_add_set_query(const Graph& g_) : n(sz(g_)), g(g_), gl(n), v(n, o()), lazy(n, o()) {
		// m : g の辺の数
		int m = 0;
		rep(s, n) m += sz(g[s]);

		// 頂点の次数が大きいか小さいかの閾値 th を決定
		th = (int)(sqrt(m) + 0.001);

		// 隣接する大きい頂点だけを記録したグラフ gl を作成
		rep(s, n) repe(t, g[s]) if (sz(g[t]) > th) gl[s].push_back(t);
	}

	// グラフ g と配列 a[0..n) で初期化
	Neighbor_add_set_query(const Graph& g, const vector<S>& v_) : Neighbor_add_set_query(g) { v = v_; }
	Neighbor_add_set_query() {}

	// v[i] += x とする．
	void add(int i, S x) {
		v[i] = op(v[i], x);
	}

	// v[i] = x とする．
	void set(int i, S x) {
		v[i] = op(x, inv(gather(i)));
	}

	// v[i] を返す．
	S get(int i) {
		return op(v[i], gather(i));
	}

	// s の隣接頂点 t 全てに対して v[t] += x とする．
	void spread(int s, S x) {
		// 隣接する次数が大きい頂点の値の更新
		repe(t, gl[s]) v[t] = op(x, v[t]);

		// 隣接頂点に数を撒いたことを記録
		lazy[s] = op(x, lazy[s]);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Neighbor_add_set_query npq) {
		rep(i, npq.n) os << npq.get(i) << " ";
		return os;
	}
#endif
};


