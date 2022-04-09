#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフのクエリ処理 ■■■■■


//【隣接作用／一点参照クエリ】
/*
* Neighbor_prod_query(g) : O(n + m)
*	グラフ g と初期値 e で初期化する．
*	要素はモノイド (S, op, e) の元とする．
*
* Neighbor_prod_query(g, v) : O(n + m)
*	グラフ g と初期値 v で初期化する．
*
* set(i, x) : O(1)
*	v[i] = x とする．
*
* get(i) : O(√m log n)  // 特別な演算（可換，最左優先等）なら log n は落とせる
*	v[i] を返す．
*
* apply(s, x) : O(√m)
*	s の隣接頂点 t 全てに対して v[t] = op(x, v[t]) とする．
*/
template <class S, S(*op)(S, S), S(*e)()>
struct Neighbor_prod_query {
	Graph g; // グラフ
	int n; // 頂点数

	int th; // 頂点の次数が大きいかどうかの閾値
	Graph gl; // 隣接する大きい頂点だけを記録したグラフ

	vi v; // v[i] : 頂点 i に書かれている数
	vi spread; // spread[i] : 頂点 i から撒く数
	vi last; // last[i] : 頂点 i から最後に撒いたクエリ
	int q; // 何個のクエリを処理したか

	// コンストラクタ（初期化なし）
	Neighbor_prod_query() {}

	// グラフ g と単位元 e で初期化
	Neighbor_prod_query(const Graph& g_)
		: g(g_), n(sz(g)), gl(n), v(n, e()), spread(n, e()), last(n, -1), q(0) {
		// m : g の辺の数
		int m = 0;
		rep(s, n) m += sz(g[s]);
		
		// 頂点の次数が大きいか小さいかの閾値 th を決定
		th = (int)sqrt(m);

		// 隣接する大きい頂点だけを記録したグラフ gl を作成
		rep(s, n) {
			repe(t, g[s]) {
				if (sz(g[t]) > th) gl[s].push_back(t);
			}
		}
	}

	// グラフ g と配列 a で初期化
	Neighbor_prod_query(const Graph& g, const vector<S>& v_) : Neighbor_prod_query(g) {
		rep(i, n) v[i] = v_[i];
	}

	// v[i] = x とする．
	void set(int i, S x) { v[i] = x; }

	// v[i] を返す．
	S get(int i) {
		// 次数が大きい頂点であれば v[i] に正しい値が記録されている．
		S res = v[i];

		// 次数が小さい頂点の場合の更新
		if (sz(g[i]) <= th) {
			// 最後に v[i] が正しい値に更新されたクエリ q0 を得る．
			int q0 = last[i];
			res = v[i];

			// ----- この部分に O(n log n) がかかってしまう．-----
			// q0 以降のクエリにより周囲から撒かれた数をクエリ昇順に集める．
			std::set<pair<int, S>> fs;
			repe(t, g[i]) {
				if (last[t] > q0) {
					fs.insert({ last[t], spread[t] });
				}
			}
			// 撒かれた数を v[i] にクエリ昇順に作用させ正しい値を得る．
			repe(p, fs) res = op(p.second, res);
			// ---------------------------------------------------

			//// update など，最左優先で良い場合はこちらにすると O(n) で済む．
			//repe(t, g[i]) {
			//	if (chmax(q0, last[t])) {
			//		res = op(spread[t], res);
			//	}
			//}

			// v[i] を正しい値に更新しておく．
			v[i] = res;
		}

		return res;
	}

	// s の隣接頂点 t 全てに対して v[t] = op(x, v[t]) とする．
	void apply(int s, S x) {
		// 隣接する次数が大きい頂点の値の更新
		repe(t, gl[s]) v[t] = x;
		
		// 隣接頂点に数を撒いたことを記録
		spread[s] = x;
		last[s] = q++;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Neighbor_prod_query npq) {
		rep(i, npq.n) os << npq.get(i) << " ";
		return os;
	}
#endif
};


