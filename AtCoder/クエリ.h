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


//【Convex-Hull Trick（挿入単調，クエリ単調）】
/*
* Convex_hull_trick_monotonous() : O(1)
*	空で初期化する．
* 
* insert(pll l) : ならし O(1)
*	l = {a, b} が表す直線 y = a x + b を追加する．
*	制約：呼び出す際の挿入する直線の傾き a は狭義降順でなくてはならない．
*
* ll min(ll x) : ならし O(1)
*	a x + b の最小値を返す．
*	制約：呼び出す際の x 座標は狭義昇順でなくてはならない．
*/
struct Convex_hull_trick_monotonous {
	// 参考 : https://satanic0258.hatenablog.com/entry/2016/08/16/181331
	// verify : https://atcoder.jp/contests/dp/tasks/dp_z

	int n; // 記録している直線の本数
	deque<pll> lines; // 直線を傾き狭義降順に記録したデック

	Convex_hull_trick_monotonous() : n(0) {}

	void insert(pll l) {
		ll a3, b3;
		tie(a3, b3) = l;

		while (n >= 2) {
			ll a1, b1, a2, b2;
			tie(a1, b1) = lines[n - 2];
			tie(a2, b2) = lines[n - 1];

			// y = a2 x + b2 が必要な直線なら消去をここまでにする．
			// 積をとってしまっているのでオーバーフローに注意．
			if ((a1 - a2) * (b3 - b2) > (a2 - a3) * (b2 - b1)) {
				break;
			}
			lines.pop_back();
			n--;
		}
		lines.push_back(l);
		n++;
	}

	ll min(ll x) {
		while (n >= 2) {
			ll a1, b1, a2, b2;
			tie(a1, b1) = lines[0];
			tie(a2, b2) = lines[1];

			if (a1 * x + b1 < a2 * x + b2) {
				break;
			}
			lines.pop_front();
			n--;
		}

		ll a, b;
		tie(a, b) = lines[0];
		return a * x + b;
	}
};


//【Convex-Hull Trick】
/*
* Convex_hull_trick<T>(bool min_flag = true) : O(1)
*	空で初期化する．min_flag = true[false] なら最小値[最大値] クエリに対応する．
*
* insert(T a, T b) : ならし O(log n)
*	直線 y = a x + b を追加する．
*
* T get(T x) : O(log n)
*	a x + b の最小値[最大値] を返す．
*	制約：直線集合は空でない
*/
template <class T> class Convex_hull_trick {
	// 参考(理屈)：https://satanic0258.hatenablog.com/entry/2016/08/16/181331
	// 参考(実装)：https://github.com/satanic0258/Cpp_snippet/blob/master/src/technique/ConvexHullTrick.cpp

	// 1 本の直線を表す構造体
	struct Line {
		// 直線の式が y = a x + b であることを表す．
		T a, b;

		// 直線であるか（さもなくば最小値クエリ）
		bool is_line;

		// 次の直線へのポインタを返す関数 (クエリとの比較で)
		mutable function<const Line* ()> getSuc;

		Line(T a_, T b_, bool is_line = true) : a(a_), b(b_), is_line(is_line) {}

		bool operator<(const Line& rhs) const {
			// set は lower_bound のように任意の比較関数を引数にとることはできないので，
			// 比較演算子内で取得クエリか否かで場合分けすることにより無理やり二分探索を実現する．
			//（set を使わず自前で平衡二分探索木を書くなら，左右の子を参照して下っていくだけでいい）

			// 直線と最小値クエリの比較
			if (!rhs.is_line) {
				const Line* suc = getSuc();
				if (suc == nullptr) return false;

				const T& x = rhs.a;
				return (suc->a - a) * x + (suc->b - b) < T(0);
			}

			// 最小値クエリと直線の比較
			if (!is_line) {
				const Line* suc = rhs.getSuc();
				if (suc == nullptr) return true;

				const T& x = a;
				return (suc->a - rhs.a) * x + (suc->b - rhs.b) > T(0);
			}

			// 直線と直線の比較
			return a > rhs.a;
		}

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Line& l) {
			os << "y=";

			if (l.a == 1) os << "x";
			else if (l.a == 0);
			else if (l.a == -1) os << "-x";
			else os << l.a << "x";

			if (l.a == 0 || l.b < 0) os << l.b;
			else if (l.b > 0) os << "+" << l.b;

			return os;
		}
#endif
	};

	set<Line> lines; // 直線を傾き狭義降順に記録した集合

	// 最小値クエリに対応する場合は true，最大値クエリに対応する場合は false
	bool min_flag;

public:
	// 空で初期化する．
	Convex_hull_trick(bool min_flag = true) : min_flag(min_flag) {}

	// 直線 l : y = a x + b を追加する．
	void insert(T a, T b) {
		// verify : https://judge.yosupo.jp/problem/line_add_get_min

		// 最大値クエリに対応する場合は -1 倍して上下反転し，最小値クエリとして扱う．
		if (!min_flag) {
			a = -a;
			b = -b;
		}

		// nit : l の次に傾きが小さい直線（無いなら lines.end()）
		auto nit = lines.lower_bound({ a, b });

		// pit : l の次に傾きが大きい直線（無いなら lines.end()）
		auto pit = (nit != lines.begin() ? prev(nit) : lines.end());

		// pit と l の傾きが等しい場合
		if (pit != lines.end() && pit->a == a) {
			// pit の方が低い位置にあるなら l は不要
			if (pit->b <= b) return;

			// l の方が低い位置にあるなら pit は不要
			lines.erase(pit);
		}
		// l と nit の傾きが等しい場合
		else if (nit != lines.end() && a == nit->a) {
			// nit の方が低い位置にあるなら l は不要
			if (nit->b <= b) return;

			// l の方が低い位置にあるなら nit は不要
			lines.erase(nit);
		}
		// pit, l, nit の傾きが全て異なる場合
		else if (pit != lines.end() && nit != lines.end()) {
			// x1 = x1_num / x1_dnm : pit と l の交点の x 座標
			//（y = pit->a x + pit->b と y = a x + b を連立する）
			T x1_num = b - pit->b, x1_dnm = pit->a - a;

			// x2 = x2_num / x2_dnm : l と nit の交点の x 座標
			//（y = a x + b と y = nit->a x + nit->b を連立する）
			T x2_num = nit->b - b, x2_dnm = a - nit->a;

			// x1 >= x2 となっているなら l は不要な直線なので追加せず終わる．
			if (x1_num * x2_dnm >= x2_num * x1_dnm) return;
		}

		// 直線 l を追加する．
		auto it = lines.insert({ a, b }).first;
		it->getSuc = [=] { return (next(it) == lines.end() ? nullptr : &*next(it)); };

		// l より傾きが大きい直線のうち，l のせいで不必要になったものを削除する．
		if (it != lines.begin()) {
			auto pit = prev(it);
			while (pit != lines.begin()) {
				// pit : l の次に傾きが大きい直線
				// ppit : l の次の次に傾きが大きい直線
				auto ppit = prev(pit);

				// x1 = x1_num / x1_dnm : ppit と pit の交点の x 座標
				//（y = ppit->a x + ppit->b と y = pit->a x + pit->b を連立する）
				T x1_num = pit->b - ppit->b, x1_dnm = ppit->a - pit->a;

				// x2 = x2_num / x2_dnm : pit と l の交点の x 座標
				//（y = pit->a x + pit->b と y = a x + b を連立する）
				T x2_num = b - pit->b, x2_dnm = pit->a - a;

				// x1 <= x2 となっているなら pit は必要な直線なので削除せず終わる．
				if (x1_num * x2_dnm <= x2_num * x1_dnm) break;

				// さもなくば pit は不必要な直線なので削除する．
				pit = prev(lines.erase(pit));
			}
		}

		// l より傾きが小さい直線のうち，l のせいで不必要になったものを削除する．
		if (next(it) != lines.end()) {
			auto nit = next(it);
			while (next(nit) != lines.end()) {
				// nit : l の次に傾きが小さい直線
				// nnit : l の次の次に傾きが小さい直線
				auto nnit = next(nit);

				// x1 = x1_num / x1_dnm : l と nit の交点の x 座標
				//（y = a x + b と y = nit->a x + nit->b を連立する）
				T x1_num = nit->b - b, x1_dnm = a - nit->a;

				// x2 = x2_num / x2_dnm : nit と nnit の交点の x 座標
				//（y = nit->a x + nit->b と y = nnit->a x + nnit->b を連立する）
				T x2_num = nnit->b - nit->b, x2_dnm = nit->a - nnit->a;

				// x1 <= x2 となっているなら nit は必要な直線なので削除せず終わる．
				if (x1_num * x2_dnm <= x2_num * x1_dnm) break;

				// さもなくば nit は不必要な直線なので削除する．
				nit = lines.erase(nit);
			}
		}
	}

	// a x + b の最小値[最大値] を返す．
	T get(T x) {
		// verify : https://judge.yosupo.jp/problem/line_add_get_min

		Assert(!lines.empty());

		auto it = lines.lower_bound(Line{ x, x, false });

		if (min_flag) return it->a * x + it->b;
		else return -(it->a * x + it->b); // 最大値クエリの場合は -1 倍していたので元に戻す．
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Convex_hull_trick& cht) {
		for (auto it = cht.lines.begin(); it != cht.lines.end(); it++) {
			os << *it << (next(it) != cht.lines.end() ? "," : "");
		}
		return os;
	}
#endif
};


//【Slope Trick】
/*
* Slope_trick() : O(1)
*	f(x) = 0 で初期化する．
*
* ll min(pll* lr) : O(1)
*	min f(x) を返し，必要ならそれを与える x の範囲 [l, r] を lr に格納する．
*
* add_const(ll x0) : O(1)
*	f(x) += y0 とする．
*
* add_right(ll x0) : O(1)
*	f(x) += min(x - x0, 0) とする．（＿／ の形を加算する．）
*
* add_left(ll x0) : O(1)
*	f(x) += min(x0 - x, 0) とする．（＼＿ の形を加算する．）
*
* add_abs(ll x0) : O(1)
*	f(x) += |x - x0| とする．（＼／ の形を加算する．）
*
* acc_min_left() : O(1)
*	f(x) を左から累積最小値をとったものに置き換える．（＼＿ の形にする．）
*
* acc_min_right() : O(1)
*	f(x) を右から累積最小値をとったものに置き換える．（＿／ の形にする．）
*
* shift(ll x0) : O(1)
*	f(x) を x0 だけ平行移動する．（f(x) ← f(x - x0)）
*
* sliding_window_min(ll x0, ll x1) : O(1)
*	f(x) を min f([x+x0, x+x1]) に置き換える．（＼＿＿／ の形にする．）
*/
struct Slope_trick {
	// 参考 : https://maspypy.com/slope-trick-1-%E8%A7%A3%E8%AA%AC%E7%B7%A8
	// verify : https://atcoder.jp/contests/abc127/tasks/abc127_f

	ll y_min; // 最小値
	priority_queue<ll> l; // 最小値より左の折れ点の x 座標を降順に取り出せるキュー
	priority_queue_rev<ll> r; // 最小値より右の折れ点の x 座標を昇順に取り出せるキュー
	ll add_l; // 最小値より左側の平行移動量
	ll add_r; // 最小値より右側の平行移動量

	// f(x) = 0 で初期化する．
	Slope_trick() : y_min(0), add_l(0), add_r(0) {
		l.push(-INFL);
		r.push(INFL);
	};

	// min f(x) を返し，必要ならそれを与える x の範囲 [l, r] を lr に格納する．
	ll min(pll* lr = nullptr) {
		if (lr != nullptr) {
			*lr = { l.top() + add_l, r.top() + add_r };
		}
		return y_min;
	}

	// f(x) += y0 とする．
	void add_const(ll y0) {
		y_min += y0;
	}

	// f(x) += min(x - x0, 0) とする．（＿／ の形を加算する．）
	void add_right(ll x0) {
		y_min += max(0LL, (l.top() + add_l) - x0);
		l.push(x0 - add_l);
		r.push((l.top() + add_l) - add_r);
		l.pop();
	}

	// f(x) += min(x0 - x, 0) とする．（＼＿ の形を加算する．）
	void add_left(ll x0) {
		y_min += max(0LL, x0 - (r.top() + add_r));
		r.push(x0 - add_r);
		l.push((r.top() + add_r) - add_l);
		r.pop();
	}

	// f(x) += |x - x0| とする．（＼／ の形を加算する．）
	void add_abs(ll x0) {
		add_right(x0);
		add_left(x0);
	}

	// f(x) を左から累積最小値をとったものに置き換える．（＼＿ の形にする．）
	void acc_min_left() {
		r = priority_queue_rev<ll>();
		r.push(INFL);
	}

	// f(x) を右から累積最小値をとったものに置き換える．（＿／ の形にする．）
	void acc_min_right() {
		l = priority_queue<ll>();
		l.push(-INFL);
	}

	// f(x) を x0 だけ平行移動する．（f(x) ← f(x - x0)）
	void shift(ll x0) {
		add_l += x0;
		add_r += x0;
	}

	// f(x) を min f([x+x0, x+x1]) に置き換える．（＼＿＿／ の形にする．）
	void sliding_window_min(ll x0, ll x1) {
		add_l += x0;
		add_r += x1;
	}
};


//【区間への一次式との最小値 & 一点取得クエリ】
/*
* Range_minimize1d_query(n) : O(1)
*	要素数 n かつ初期値 INF で初期化する．
*
* Range_minimize1d_query(v) : O(n)
*	配列 v で初期化する．
*
* set(l, r, a, b) : O(log n)
*	半開区間 [l, r) の要素 v[i] を a i + b との最小値に変更する．
*
* get(i) : O(m log n)（m : 一次の項の係数の種類）
*	v[i] 番目の要素を返す．
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


