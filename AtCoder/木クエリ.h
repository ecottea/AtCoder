#pragma once
#include "header.h"
#include "構造(木).h"
#include "フェニック木.h"
// ■■■■■ 木のクエリ処理 ■■■■■



//【根付き木のオイラーツアー】O(|V|)
/*
* 根付き木 rt のオイラーツアーを求める．
*
* in[s] : 最初に頂点 s を訪れた時刻（根なら 0）
* out[s] : 最後に頂点 s から離れた時刻（根なら 2 |V| - 1）
* pos[t] : 時刻 t に訪れた頂点の番号（長さ 2 |V| - 1）
*/
template <class TREE>
void euler_tour(TREE& rt, vi& in, vi& out, vi& pos) {
	int n = sz(rt);

	int time = 0;
	in = vi(n);
	out = vi(n);
	pos = vi(2 * n - 1);

	// 再帰用の関数
	function<void(int)> rf = [&](int s) {
		// s を最初に訪れた
		in[s] = time;
		pos[time++] = s;

		repe(t, rt[s].child) {
			rf(t);
			pos[time++] = s;
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(rt.r);
}


//【最小共通祖先】
/*
* 与えられた根付き木について，頂点対の最小共通祖先を求める．
*
* Lowest_common_ancestor(rt) : O(|V|)
*	根付き木 rt で初期化する．
*
* lca(u, v) : O(log |V|)
*	頂点 u, v の最小共通祖先を返す．
*
* dist(u, v) : O(log |V|)
*	頂点 u, v の距離を返す．
*
* 利用：【根付き木のオイラーツアー】
*/
pli op1(pli a, pli b) { return min(a, b); }
pli e1() { return { INFL, -1 }; }
template <class TREE> struct Lowest_common_ancestor {
	TREE rt;

	// オイラーツアーの結果の記録用
	// in[v] : v に最初に入った時刻
	// out[v] : v から最後に出た時刻
	// pos[t] : 時刻 t に居た頂点の番号
	vi in, out, pos;

	// 深さに関する区間最小クエリを処理するためのセグメント木
	// seg[t] : 時刻 t に居た頂点の (深さ, 番号)
	segtree<pli, op1, e1> seg;

	// コンストラクタ（根付き木で初期化）：O(|V|)
	Lowest_common_ancestor(TREE& rt_) : rt(rt_) {
		// オイラーツアーを求めておく．
		euler_tour(rt, in, out, pos);

		// 深さに関する区間最小クエリを処理するためのセグメント木を用意する．
		// 深さだけでなく頂点の番号も返したいのでそれらを対にしてもつ．
		int n = sz(rt.v);
		vector<pli> depth(2 * n - 1);
		rep(t, 2 * n - 1) {
			depth[t] = { rt[pos[t]].depth, pos[t] };
		}
		seg = segtree<pli, op1, e1>(depth);
	}

	// 頂点 u, v の最小共通祖先を返す．
	int lca(int u, int v) {
		// 初めて u または v に訪れたとき
		int left = min(in[u], in[v]);

		// 最後に u または v から離れたとき
		int right = max(out[u], out[v]);

		// その途中で訪れたことのある最も浅い頂点が最小共通祖先
		return seg.prod(left, right).second;
	}

	// 頂点 u, v の距離を返す．
	ll dist(int u, int v) {
		int r = lca(u, v);

		// 根からの距離の和を求め，ダブっている分を引く．
		return rt[u].dist + rt[v].dist - 2 * rt[r].dist;
	}
};


//【木のオイラーツアー】O(|V|)
/*
* 頂点 r を始点とする木 g のオイラーツアーを求める．
*
* in[v] : 最初に頂点 v を訪れた時刻（r なら 0）
* out[v] : 最後に頂点 v から離れた時刻（r なら 2 |V| - 1）
* pos[t] : 時刻 t に訪れた頂点の番号（長さ 2 |V| - 1）
*/
template <class G>
void euler_tour(G& g, int r, vi& in, vi& out, vi& pos) {
	// 参考 : https://qiita.com/recuraki/items/72e37eb9be9f71bc623a

	int n = sz(g);

	int time = 0;
	in = vi(n);
	out = vi(n);
	pos = vi(2 * n - 1);

	// 再帰用の関数
	function<void(int, int)> rf = [&](int s, int p) {
		// s を最初に訪れた
		in[s] = time;
		pos[time++] = s;

		for (auto t : g[s]) {
			if (t == p) continue;

			rf(t, s);
			pos[time++] = s;
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(r, -1);
}


//【辺加算／根からのパス総和クエリ】
/*
* Path_sum_query(g, r) : O(|V|)
*	木 g を根を r とみなして初期化する．
*
* add(v, val) : O(log|V|)
*	頂点 v を子とする辺に val を加算する．
*
* sum(v) : O(log|V|)
*	根 r から v までの辺の値の和を返す．
*
* 利用：【木のオイラーツアー】
*/
template <class T> struct Path_sum_query {
	// 参考：https://perogram.hateblo.jp/entry/2020/10/01/034136

	// オイラーツアーの結果の記録用
	// in[v] : v に最初に入った時刻
	// out[v] : v から最後に出た時刻
	vi in, out;

	// オイラーツアーに対する区間和クエリを処理するためのフェニック木
	// ft[t] : 時刻 t-1 から時刻 t に変わることで増加する値
	fenwick_tree<T> ft;

	// コンストラクタ（木と根で初期化）
	Path_sum_query(Graph& g, int r) {
		// オイラーツアーを求めておく．
		vi pos;
		euler_tour(g, r, in, out, pos);

		ft = fenwick_tree<T>(2 * sz(g));
	}

	// 頂点 v を子とする辺に val を加算する．
	void add(int v, T val) {
		// いもす法のように，v の部分木にいる間だけ val が累積和に寄与するようにする．
		ft.add(in[v], val);
		ft.add(out[v], -val);
	}

	// 根 r から v までの辺の値の和を返す．
	T sum(int v) { return ft.sum(0, in[v] + 1); }
};


//【根付き木の HL 分解】O(|V|)
/*
* 根付き木 rt の HL 分解を行う．
*
* in[s] : 最重頂点優先の行きがけ順で頂点 s を何番目になぞるか
* pos[i] : 最重頂点優先の行きがけ順で i 番目になぞる頂点
* top[s] : 頂点 s を含む連結成分の最も浅い頂点
*/
template <class TREE>
void heavy_light_decomposition(TREE& rt, vi& in, vi& pos, vi& top) {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n = sz(rt);

	int step = 0;
	in = vi(n);
	pos = vi(n);
	top = vi(n);

	// 再帰用の関数
	// s : 注目している頂点
	// p : s を含む連結成分の最も浅い頂点
	function<void(int, int)> rf = [&](int s, int p) {
		in[s] = step;
		pos[step++] = s;
		top[s] = p;

		// 重さ最大の頂点を得る．
		int w_max = -INF, v_max = -1;
		repe(t, rt[s].child) {
			if (chmax(w_max, rt[t].weight)) {
				v_max = t;
			}
		}

		// 重さ最大の頂点を優先的になぞる．
		if (v_max != -1) rf(v_max, p);

		// 残りの頂点をなぞる．
		repe(t, rt[s].child) {
			if (t == v_max) continue;
			rf(t, t);
		}

		return;
	};

	// 根から順に探索する．
	rf(rt.r, rt.r);
}


//【パス加算／パス総和クエリ】
/*
* Path_add_sum_query(rt) : O(|V|)
*	根付き木 rt で初期化する．
*
* add(v1, v2, val) : O((log |V|)^2)
*	頂点 v1 から v2 までの辺に val を加算する．
*
* sum(v1, v2) : O((log |V|)^2)
*	頂点 v1 から v2 までの辺の値の和を返す．
*
* 利用：
*	【根付き木の HL 分解】
*	【区間加算／区間総和クエリ】
*/
template <class T> struct Path_add_sum_query {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	// 根付き木
	RTree rt;

	// HL 分解の結果の記録用
	// in[v] : 頂点 v が最重頂点優先の行きがけ順で何番目になぞられるか
	// pos[i] : 最重頂点優先の行きがけ順で i 番目になぞる頂点
	// top[v] : 頂点 v を含む連結成分の最も浅い頂点
	vi in, pos, top;

	// in 順に並べた頂点の列 pos に対する区間加算／区間総和クエリを処理する．
	// rasq[i] : i 番目になぞる頂点に入る辺の値（rasq[0] は使わない）
	RASQ<T> rasq;

	// コンストラクタ（根付き木で初期化）
	Path_add_sum_query(RTree& rt_) : rt(rt_), rasq(sz(rt)) {
		// rt を HL 分解する．
		heavy_light_decomposition(rt, in, pos, top);
	}

	// 頂点 v1 から v2 までの辺に val を加算する．
	void add(int v1, int v2, T val) {
		// v1 と v2 が異なる連結成分に属している限りループを回す．
		while (top[v1] != top[v2]) {
			// v1 の方が浅い連結成分に属しているとする．
			if (in[top[v1]] > in[top[v2]]) {
				swap(v1, v2);
			}

			// v2 を含む連結成分は pos で並んで配置されているので，
			// 最も浅い頂点 top[v2] から v2 までの範囲に val を加算する．
			rasq.apply(in[top[v2]], in[v2] + 1, val);

			// 一つ浅い連結成分に移動する．
			v2 = rt.v[top[v2]].parent;
		}

		// ここまできたら v1 と v2 は同じ連結成分に属するので，
		// その間の辺のみに対して val を加算する．
		if (in[v1] > in[v2]) {
			swap(v1, v2);
		}
		rasq.apply(in[v1] + 1, in[v2] + 1, val);
	}

	// 頂点 v1 から v2 までの辺の値の和を返す．
	T sum(int v1, int v2) {
		T res = 0;

		// v1 と v2 が異なる連結成分に属している限りループを回す．
		while (top[v1] != top[v2]) {
			// v1 の方が浅い連結成分に属しているとする．
			if (in[top[v1]] > in[top[v2]]) {
				swap(v1, v2);
			}

			// v2 を含む連結成分は pos で並んで配置されているので，
			// 最も浅い頂点 top[v2] から v2 までの範囲の和を求める．
			res += rasq.prod(in[top[v2]], in[v2] + 1);

			// 一つ浅い連結成分に移動する．
			v2 = rt.v[top[v2]].parent;
		}

		// ここまできたら v1 と v2 は同じ連結成分に属するので，
		// その間の辺のみの和を res に加算する．
		if (in[v1] > in[v2]) {
			swap(v1, v2);
		}
		res += rasq.prod(in[v1] + 1, in[v2] + 1);

		return res;
	}
};


//【根付き木の HL 分解／オイラーツアー】O(|V|)
/*
* 根付き木 rt の HL 分解を行いつつオイラーツアーを得る．
*
* in[s] : 最重頂点優先で頂点 s に初めて入る時刻（根なら 0）
* out[s] : 最重頂点優先で頂点 s から最後にでる時刻（根なら 2 |V| - 1）
* pos[t] : 最重頂点優先で時刻 t で居る頂点（長さ 2 |V| - 1）
* top[s] : 頂点 s を含む連結成分の最も浅い頂点
*/
template <class TREE>
void hld_and_et(TREE& rt, vi& in, vi& out, vi& pos, vi& top) {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n = sz(rt);

	int time = 0;
	in = vi(n);
	out = vi(n);
	pos = vi(2LL * n - 1);
	top = vi(n);

	// 再帰用の関数
	// s : 注目している頂点
	// p : s を含む連結成分の最も浅い頂点
	function<void(int, int)> rf = [&](int s, int p) {
		in[s] = time;
		pos[time++] = s;
		top[s] = p;

		// 重さ最大の頂点を得る．
		int w_max = -INF, v_max = -1;
		repe(t, rt[s].child) {
			if (chmax(w_max, rt[t].weight)) {
				v_max = t;
			}
		}

		// 重さ最大の頂点を優先的になぞる．
		if (v_max != -1) {
			rf(v_max, p);
			pos[time++] = s;
		}

		// 残りの頂点をなぞる．
		repe(t, rt[s].child) {
			if (t == v_max) continue;

			rf(t, t);
			pos[time++] = s;
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(rt.r, rt.r);
}


//【部分木加算／パス総和クエリ】
/*
* Subtree_add_path_sum_query(rt) : O(|V|)
*	コスト付き根付き木 rt で初期化する．
*
* add(v, val) : O(log |V|)
*	頂点 v の部分木の辺に val を加算する．
*
* add(v1, v2, val) : O((log |V|)^2)
*	頂点 v1 から v2 までの辺に val を加算する．
*
* sum(v1, v2) : O((log |V|)^2)
*	頂点 v1 から v2 までの辺の値の和を返す．
*
* get(v) : O(log |V|)
*	頂点 v への v の親からの辺の値を返す．
*
* 利用：
*	【根付き木の HL 分解／オイラーツアー】
*	【区間加算／区間総和クエリ】
*/
template <class T> struct Subtree_add_path_sum_query {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	// 根付き木
	WRTree rt;

	// HL 分解とオイラーツアーの結果の記録用
	// in[s] : 最重頂点優先で頂点 s に初めて入る時刻（根なら 0）
	// out[s] : 最重頂点優先で頂点 s から最後にでる時刻（根なら 2 |V| -1）
	// pos[t] : 最重頂点優先で時刻 t で居る頂点（長さ 2 |V| -1）
	// top[s] : 頂点 s を含む連結成分の最も浅い頂点
	vi in, out, pos, top;

	// オイラーツアーで得られた列 pos に対する区間加算／区間総和クエリを処理する．
	// rasq[t] : 時刻 t で居る頂点に入る辺の値（rasq[0] は使わない）
	RASQ<T> rasq;

	// コンストラクタ（根付き木で初期化）
	Subtree_add_path_sum_query(WRTree& rt_) : rt(rt_) {
		// rt を HL 分解しつつオイラーツアーを得る．
		hld_and_et(rt, in, out, pos, top);

		vl val(2LL * rt.n - 1);
		rep(s, rt.n) {
			repe(e, rt[s].child) {
				val[in[e.to]] += e.cost;
			}
		}

		rasq = RASQ<T>(val);
	}

	// 頂点 v の部分木の辺に val を加算する．
	void add(int v, ll val) {
		rasq.apply(in[v] + 1, out[v], val);
	}

	// 頂点 v1 から v2 までの辺に val を加算する．
	void add(int v1, int v2, ll val) {
		// v1 と v2 が異なる連結成分に属している限りループを回す．
		while (top[v1] != top[v2]) {
			// v1 の方が浅い連結成分に属しているとする．
			if (in[top[v1]] > in[top[v2]]) {
				swap(v1, v2);
			}

			// v2 を含む連結成分は pos で並んで配置されているので，
			// 最も浅い頂点 top[v2] から v2 までの範囲に val を加算する．
			rasq.apply(in[top[v2]], in[v2] + 1, val);

			// 一つ浅い連結成分に移動する．
			v2 = rt[top[v2]].parent;
		}

		// ここまできたら v1 と v2 は同じ連結成分に属するので，
		// その間の辺のみに対して val を加算する．
		if (in[v1] > in[v2]) {
			swap(v1, v2);
		}
		rasq.apply(in[v1] + 1, in[v2] + 1, val);
	}

	// 頂点 v1 から v2 までの辺の値の和を返す．
	ll sum(int v1, int v2) {
		ll res = 0;

		// v1 と v2 が異なる連結成分に属している限りループを回す．
		while (top[v1] != top[v2]) {
			// v1 の方が浅い連結成分に属しているとする．
			if (in[top[v1]] > in[top[v2]]) {
				swap(v1, v2);
			}

			// v2 を含む連結成分は pos で並んで配置されているので，
			// 最も浅い頂点 top[v2] から v2 までの範囲の和を求める．
			res += rasq.prod(in[top[v2]], in[v2] + 1);

			// 一つ浅い連結成分に移動する．
			v2 = rt[top[v2]].parent;
		}

		// ここまできたら v1 と v2 は同じ連結成分に属するので，
		// その間の辺のみの和を res に加算する．
		if (in[v1] > in[v2]) {
			swap(v1, v2);
		}
		res += rasq.prod(in[v1] + 1, in[v2] + 1);

		return res;
	}

	// 頂点 v への v の親からの辺の値を返す．
	ll get(int v) { return sum(rt[v].parent, v); }
};


