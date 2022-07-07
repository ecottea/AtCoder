#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフのクエリ処理 ■■■■■


//【隣接変更作用／一点参照クエリ】
/*
* Neighbor_update_query(Graph g, vS v) : O(|V| + |E|)
*	グラフ g と初期値 v[0..n) で初期化する．
*
* set(int i, S x) : O(1)
*	v[i] = x とする．
*
* S get(int i) : O(√|E|)
*	v[i] を返す．
*
* update(int s, S x) : O(√|E|)
*	s の隣接頂点 t 全てについて v[t] = x とする．
*/
template <class S> class Neighbor_update_query {
	using vS = vector<S>;

	int n; // 頂点数
	Graph g; // グラフ
	int now; // 現在の時刻

	int th; // 頂点の次数が大きいかどうかの閾値
	Graph gl; // 隣接する次数が大きい頂点だけを記録したグラフ

	vS v; // 各頂点の値
	vi last_update; // 各頂点の値が最後に更新された時刻（次数の小さい頂点のみ）
	vS spread; // 各頂点から撒く値
	vi last_spread; // 各頂点から最後に値を撒いた時刻

public:
	// グラフ g と初期値 v[0..n) で初期化する．
	Neighbor_update_query(const Graph& g_, const vector<S>& v_)
		: n(sz(g_)), g(g_), now(0), gl(n), v(v_), last_update(n, -1), spread(n), last_spread(n, -1)
	{
		// verify : https://atcoder.jp/contests/abc219/tasks/abc219_g

		// m : g の辺の数
		int m = 0;
		rep(s, n) m += sz(g[s]);

		// 頂点の次数が大きいか小さいかの閾値 th を決定
		th = (int)(sqrt(m) + 0.001);

		// 隣接する大きい頂点だけを記録したグラフ gl を作成
		rep(s, n) repe(t, g[s]) if (sz(g[t]) > th) gl[s].push_back(t);
	}
	Neighbor_update_query() {}

	// v[i] = x とする．
	void set(int i, S x) {
		// verify : https://atcoder.jp/contests/abc219/tasks/abc219_g

		v[i] = x;
		last_update[i] = now++;
	}

	// v[i] を返す．
	S get(int i) {
		// verify : https://atcoder.jp/contests/abc219/tasks/abc219_g

		// 次数が小さい頂点の場合の更新
		if (sz(g[i]) <= th) {
			// 最後に v[i] が正しい値に更新された時刻を得る．
			int newest = last_update[i];

			// それ以降に周囲から撒かれた数のうち最新のものに変更する．
			repe(t, g[i]) if (chmax(newest, last_spread[t])) v[i] = spread[t];

			last_update[i] = now++;
		}

		// 次数が大きい頂点の場合は常に正しい値が入っている．
		return v[i];
	}

	// s の隣接頂点 t 全てについて v[t] = x とする．
	void update(int s, S x) {
		// verify : https://atcoder.jp/contests/abc219/tasks/abc219_g

		// 隣接する次数が大きい頂点の値の更新
		repe(t, gl[s]) v[t] = x;

		// 隣接頂点に数を撒いたことを記録
		spread[s] = x;
		last_spread[s] = now++;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Neighbor_update_query npq) {
		rep(i, npq.n) os << npq.get(i) << " ";
		return os;
	}
#endif
};


//【隣接作用／一点参照クエリ（アーベル群）】
/*
* Neighbor_add_set_query<S, op, o, inv>(Graph g) : O(|V| + |E|)
*	グラフ g と初期値 o() で初期化する．
*	要素はアーベル群 (S, op, o, inv) の元とする．
*
* Neighbor_add_set_query<S, op, o, inv>(Graph g, vS v) : O(|V| + |E|)
*	グラフ g と初期値 v[0..n) で初期化する．
*
* add(int i, S x) : O(1)
*	v[i] += x とする．
*
* set(int i, S x) : O(√|E|)
*	v[i] = x とする．
*
* S get(int i) : O(√|E|)
*	v[i] を返す．
*
* spread(int s, S x) : O(√|E|)
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


