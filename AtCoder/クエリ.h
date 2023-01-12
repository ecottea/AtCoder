#pragma once
#include "header.h"
#include "永続データ構造.h"
// ■■■■■ クエリ処理 ■■■■■


//【Mo's algorithm】O(n√q α + q log q)
/*
* a[0..n) の q 個の区間 a[l[j]..r[j]) クエリに対する解を res[j] に格納し res を返す．
* res00 は a[0..0) クエリに対する解とする．また区間に a[i] を追加[削除]する場合，
* 新たな解は insert[erase]（計算量 O(α)）で計算されるとする．
*
*（クエリ平方分割）
*/
template <class T, class S>
vector<S> mos_algorithm(const vector<T>& a, const vi& l, const vi& r, S res00) {
	// 参考 : https://ei1333.hateblo.jp/entry/2017/09/11/211011
	// verify : https://atcoder.jp/contests/abc174/tasks/abc174_f

	//【方法】
	// 区間 [0..n) を k 個のブロックに等分割する．ブロックの幅は n/k になる．
	// 左端の移動回数は，1 回のクエリで高々 n/k しか移動しないので q n/k + n 回．
	// 右端の移動回数は，1 ブロックごとに高々 n しか移動しないので k n / 2 回．
	// これらが一致するような k を求めると k = √(2q+1) + 1 となる．
	// ただ，前者は平均的には /2 くらい小さいはずなので，それに期待するなら k = √q がいい．

	int q = sz(l);
	int k = (int)(sqrt(q) + EPS);
	int w = max((sz(a) + k - 1) / k, 1);
	vector<S> res(q);

	// クエリを左端の位置するブロックについて昇順に，
	// 次いで右端を偶数番目のブロックは昇順，奇数番目のブロックは降順でソートする．
	vector<tuple<int, int, int>> lb_sr_j(q);
	rep(j, q) {
		int b = l[j] / w;
		lb_sr_j[j] = { b, (b % 2 == 0 ? 1 : -1) * r[j], j };
	}
	sort(all(lb_sr_j));

	// -------------- ここを実装する（auto の方が速い） ---------------

	// 区間に a[i] を追加する場合の解 sol を更新する．
	auto insert = [&](int i, S& sol) {
		sol = sol;
	};

	// 区間から a[i] を削除する場合の解 sol を更新する．
	auto erase = [&](int i, S& sol) {
		sol = sol;
	};
	// --------------------------------------------------------------

	// 初期化
	int lpt = 0, rpt = 0; S sol = res00;

	// クエリを順に処理していく
	rep(tmp, q) {
		// 区間を広げる
		while (lpt > l[j]) { insert(--lpt, sol); }
		while (rpt < r[j]) { insert(rpt++, sol); }

		// 区間を狭める
		while (lpt < l[j]) { erase(lpt++, sol); }
		while (rpt > r[j]) { erase(--rpt, sol); }

		res[j] = sol;
	}

	return res;
}


//【Mo's algorithm（区間縮小なし）】O((n + q)√q α)
/*
* a[0..n) の q 個の区間 a[l[j]..r[j]) クエリに対する解を res[j] に格納し res を返す．
* res_ep は空区間クエリに対する解とする．また区間の右に a[i] を追加する場合，
* 新たな解は insert（計算量 O(α)）で計算されるとする．
*
* 制約：左端を伸ばす操作は非破壊的
* 
*（クエリ平方分割）
*/
template <class T, class S>
vector<S> mos_algorithm_no_erase(const vector<T>& a, const vi& l, const vi& r, S res_ep) {
	// verify : https://codeforces.com/contest/620/problem/F

	//【方法】
	// 区間 [0..n) を k 個のブロックに等分割する．ブロックの幅は n/k になる．
	// 左端の移動回数は，1 回のクエリで高々 n/k しか移動しないので q n/k 回．
	// 右端の移動回数は，1 ブロックごとに高々 n しか移動しないので k n / 2 回．
	// これらが一致するような k を求めると k = √(2q) となる．
	// ただ，前者は平均的には /2 くらい小さいはずなので，それに期待するなら k = √q がいい．

	int q = sz(l);
	int k = (int)(sqrt(q) + EPS);
	int width = max((sz(a) + k - 1) / k, 1);
	vector<S> res(q);

	// クエリを左端の位置するブロックごとに分け，右端について昇順ソートする．
	vector<vector<pii>> lb_to_rj(k);
	vi l_max(k, -1); // ブロック内の左端位置の最大値
	rep(j, q) {
		lb_to_rj[min(l[j] / width, k - 1)].emplace_back(r[j], j);
		chmax(l_max[min(l[j] / width, k - 1)], l[j]);
	}
	rep(b, k) sort(all(lb_to_rj[b]));

	// -------------- ここを実装する（auto の方が速い） ---------------
	
	// 区間の右に a[i] を追加する場合の解 sol を更新する．
	auto insert = [&](int i, S& sol) {
		sol = sol;
	};
	// --------------------------------------------------------------

	// 初期化
	S sol = res_ep;

	// クエリを順に処理していく
	rep(b, k) {
		int rpt = l_max[b];

		repe(tmp, lb_to_rj[b]) {
			int j = tmp.second;

			// 右端がブロック内にある場合の例外処理
			if (r[j] <= l_max[b]) {
				S sol2 = res_ep;
				
				res[j] = sol2;

				continue;
			}

			// 右端を伸ばす（これは記録する）
			while (rpt < r[j]) { insert(rpt++, sol); }

			// 左端を伸ばす（これは記録しない）
			S sol2 = sol;
			repi(i, l[j], l_max[b] - 1) {
				sol2 = sol2;
			}

			res[j] = sol2;
		}

		sol = res_ep;
	}

	return res;
}


//【静的矩形和（アーベル群）】
/*
* Static_rectangle_sum(vl x, vl y, vS v) : O(n log n)
*	値 v[i] をもった n 個の点群 (x[i], y[i]) で初期化する．
*
* S sum(ll x1, ll y1, ll x2, ll y2) : O(log n)
*	[x1..x2)×[y1..y2) 内にある全ての点の値の和を返す．
*
* 利用：【永続セグメント木（モノイド）】,【座標圧縮】
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
class Static_rectangle_sum {
	// 参考 : https://qiita.com/hotman78/items/9c643feae1de087e6fc5

	// x[y] 座標の昇順列（x 座標は全て，y 座標はユニーク）
	vl xs, ys;

	// x 座標を時刻とみなした，圧縮後の y 座標に関する永続セグメント木
	Persistent_segtree<S, op, o> seg;

public:
	// 値 v[i] をもった n 個の点群 (x[i], y[i]) で初期化する．
	Static_rectangle_sum(const vl& x, const vl& y, const vector<S>& v) {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum
		
		int n = sz(x);
		xs.resize(n);

		// y 座標を座標圧縮しておく．
		vi y_cp;
		int m = coordinate_compression(y, y_cp, &ys);

		// 点群を x 座標昇順にソートする
		vector<pli> xi(n);
		rep(i, n) xi[i] = { x[i], i };
		sort(all(xi));

		// x 座標を時刻とみなして永続セグメント木に乗せる．
		seg = Persistent_segtree<S, op, o>(m);
		rep(t, n) {
			int i;
			tie(xs[t], i) = xi[t];

			S val = seg.get(y_cp[i], t);
			seg.set(y_cp[i], op(val, v[i]), t);
		}
	}

	// [x1..x2)×[y1..y2) 内にある全ての点の値の和を返す．
	S sum(ll x1, ll y1, ll x2, ll y2) const {
		// verify : https://judge.yosupo.jp/problem/rectangle_sum
		
		if (x1 >= x2 || y1 >= y2) return o();

		int t1 = lbpos(xs, x1);
		int t2 = lbpos(xs, x2);
		int j1 = lbpos(ys, y1);
		int j2 = lbpos(ys, y2);

		return op(seg.prod(j1, j2, t2), inv(seg.prod(j1, j2, t1)));
	}
};


//【狭義単調な点列】
/*
* Monotonous_points<T>(bool y_greater = true, T inf = max(T)/2) : O(1)
*	空で初期化する．x 座標は狭義単調増加で，y 座標は y_greater=true[false] なら狭義単調増加[減少]とする．
*
* void insert(T x, T y) : ならし O(log n)
*	点 (x, y) を挿入し，それにより単調性に違反する点は全て削除する．
*
* bool find_LL(T x, T y, bool strict = true) : O(log n)
*	x' < x かつ y' < y なる点 (x', y') が存在するかを返す（strict=false なら等号も許す）
*
* bool find_LG(T x, T y, bool strict = true) : O(log n)
*	x' < x かつ y' > y なる点 (x', y') が存在するかを返す（strict=false なら等号も許す）
*
* bool find_GL(T x, T y, bool strict = true) : O(log n)
*	x' > x かつ y' < y なる点 (x', y') が存在するかを返す（strict=false なら等号も許す）
*
* bool find_GG(T x, T y, bool strict = true) : O(log n)
*	x' > x かつ y' > y なる点 (x', y') が存在するかを返す（strict=false なら等号も許す）
*
* pTT lower_bound(T x) : O(log n)
*	x' >= x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
*
* pTT upper_bound(T x) : O(log n)
*	x' > x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
*
* pTT lower_bound_rev(T x) : O(log n)
*	x' <= x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
*
* pTT upper_bound_rev(T x) : O(log n)
*	x' < x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
*/
template <class T>
struct Monotonous_points {
	// 参考 : https://topcoder-g-hatena-ne-jp.jag-icpc.org/skyaozora/20141216.html

	bool y_greater; // y 座標について狭義単調増加か
	T inf; // 無限大

	// x 座標は狭義単調増加で，y 座標は y_greater=true[false] なら狭義単調増加[減少] な点列
	// ただし番兵として (-inf, -inf[inf]) と (inf, inf[-inf]) を含む．
	map<T, T> x_to_y;

	// 空で初期化する．x 座標は狭義単調増加で，y 座標は y_greater=true[false] なら狭義単調増加[減少]とする．
	Monotonous_points(bool y_greater = true, T inf_ = -1) : y_greater(y_greater) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		inf = (inf_ == -1 ? numeric_limits<T>::max() / 2 : inf_);

		// 番兵を挿入しておく．
		if (y_greater) { x_to_y[-inf] = -inf; x_to_y[inf] = inf; }
		else { x_to_y[-inf] = inf; x_to_y[inf] = -inf; }
	}

	// 点 (x, y) を挿入し，単調性に違反する点は全て削除する．
	void insert(T x, T y) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		// x <= x' なる最小の x' を指すイテレータを得る．
		auto it = x_to_y.lower_bound(x);

		// x' から昇順に，y' <= y[ y' >= y ] である限り要素を削除する．
		if (y_greater) {
			while (true) {
				if (it->second > y) break;
				it = x_to_y.erase(it);
			}
		}
		else {
			while (true) {
				if (it->second < y) break;
				it = x_to_y.erase(it);
			}
		}

		// x' から降順に，y' >= y[ y' <= y ] である限り要素を削除する．
		if (y_greater) {
			while (true) {
				it = prev(it);
				if (it->second < y) break;
				it = x_to_y.erase(it);
			}
		}
		else {
			while (true) {
				it = prev(it);
				if (it->second > y) break;
				it = x_to_y.erase(it);
			}
		}

		// 点 (x, y) を挿入する．
		x_to_y[x] = y;
	}

	// x' < x かつ y' < y なる点が存在するかを返す（strict=false なら等号も許す）
	bool find_LL(T x, T y, bool strict = true) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/1341

		if (strict) {
			T y2 = prev(x_to_y.lower_bound(x))->second;
			return y2 != -inf && y2 < y;
		}
		else {
			T y2 = prev(x_to_y.upper_bound(x))->second;
			return y2 != -inf && y2 <= y;
		}
	}

	// x' < x かつ y' > y なる点が存在するかを返す（strict=false なら等号も許す）
	bool find_LG(T x, T y, bool strict = true) {
		if (strict) {
			T y2 = prev(x_to_y.lower_bound(x))->second;
			return y2 != inf && y2 > y;
		}
		else {
			T y2 = prev(x_to_y.upper_bound(x))->second;
			return y2 != inf && y2 >= y;
		}
	}

	// x' > x かつ y' < y なる点が存在するかを返す（strict=false なら等号も許す）
	bool find_GL(T x, T y, bool strict = true) {
		if (strict) {
			T y2 = x_to_y.upper_bound(x)->second;
			return y2 != -inf && y2 < y;
		}
		else {
			T y2 = x_to_y.lower_bound(x)->second;
			return y2 != -inf && y2 <= y;
		}
	}

	// x' > x かつ y' > y なる点が存在するかを返す（strict=false なら等号も許す）
	bool find_GG(T x, T y, bool strict = true) {
		if (strict) {
			T y2 = x_to_y.upper_bound(x)->second;
			return y2 != inf && y2 > y;
		}
		else {
			T y2 = x_to_y.lower_bound(x)->second;
			return y2 != inf && y2 >= y;
		}
	}

	// x' >= x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
	pair<T, T> lower_bound(T x) {
		// verify : https://codeforces.com/gym/100633/problem/D

		return *x_to_y.lower_bound(x);
	}

	// x' > x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
	pair<T, T> upper_bound(T x) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		return *x_to_y.upper_bound(x);
	}

	// x' <= x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
	pair<T, T> lower_bound_rev(T x) {
		return *prev(x_to_y.upper_bound(x));
	}

	// x' < x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
	pair<T, T> upper_bound_rev(T x) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		return *prev(x_to_y.lower_bound(x));
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Monotonous_points& mp) {
		repe(p, mp.x_to_y) if (abs(p.first) != mp.inf) os << p << " ";
		return os;
	}
#endif
};


//【区間への一次式との最小値／一点参照クエリ】
/*
* Range_minimize1d_query(int n) : O(1)
*	要素数 n かつ初期値 INF で初期化する．
*
* Range_minimize1d_query(vl v) : O(n)
*	配列 v[0..n) で初期化する．
*
* set(int l, int r, ll a, ll b) : O(log n)
*	各 i∈[l, r) について v[i] を a i + b との最小値に変更する．
*
* ll get(int i) : O(m log n)（m : 一次の項の係数の種類）
*	v[i] を返す．
*/
ll op5(ll x, ll y) { return min(x, y); }
ll e5() { return INFL; }
ll mapping5(ll f, ll x) { return min(f, x); }
ll composition5(ll f, ll g) { return min(f, g); }
ll id5() { return INFL; }
ll op6(ll x, ll y) { return max(x, y); }
ll e6() { return -INFL; }
ll mapping6(ll f, ll x) { return max(f, x); }
ll composition6(ll f, ll g) { return max(f, g); }
ll id6() { return -INFL; }
struct Range_minimize1d_query {
	// verify : https://atcoder.jp/contests/abc216/tasks/abc216_g

	// 内部では値 v[i] を一次の項の係数 a で分けて
	//		min(a[1] i + b[1], a[2] i + b[2], ...)
	// の形で保持する．
	// a が同じであればその符号に応じて b の min や max に帰着できる．

	int n;
	using rmq = lazy_segtree<ll, op5, e5, ll, mapping5, composition5, id5>;
	using rMq = lazy_segtree<ll, op6, e6, ll, mapping6, composition6, id6>;
	unordered_map<ll, rmq> pos_segs;
	unordered_map<ll, rMq> neg_segs;

	Range_minimize1d_query(int n_) : n(n_) {}
	Range_minimize1d_query(const vl& v) : n(sz(v)) { pos_segs[0] = rmq(v); }

	void set(int l, int r, ll a, ll b) {
		// verify : https://atcoder.jp/contests/abc216/tasks/abc216_g

		if (a >= 0) {
			if (!pos_segs.count(a)) {
				pos_segs[a] = rmq(n);
			}
			pos_segs[a].apply(l, r, b);
		}
		else {
			if (!neg_segs.count(a)) {
				neg_segs[a] = rMq(n);
			}
			neg_segs[a].apply(l, r, b);
		}
	}

	ll get(int i) {
		// verify : https://atcoder.jp/contests/abc216/tasks/abc216_g

		ll res = INFL;
		repea(p, pos_segs) {
			chmin(res, p.first * i + p.second.get(i));
		}
		repea(p, neg_segs) {
			chmin(res, p.first * i + p.second.get(i));
		}
		return res;
	}
};


