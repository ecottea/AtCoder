#pragma once
#include "header.h"
// ■■■■■ クエリ処理 ■■■■■


//【Mo's algorithm】O(n√q α)
/*
* a[0..n) の q 個の区間 a[l[j]..r[j]) クエリに対する解を res[j] に格納する．
* res00 は a[0..0) クエリに対する解とする．また区間に a[i] を追加[削除]する場合，
* 新たな解は insert[erase]（計算量 O(α)）で計算されるとする．
*
*（平方分割）
*/
template <class T, class S>
void mos_algorithm(const vector<T>& a, const vi& l, const vi& r, S res00, vector<S>& res) {
	// 参考 : https://ei1333.hateblo.jp/entry/2017/09/11/211011
	// verify : https://atcoder.jp/contests/abc242/tasks/abc242_g

	int q = sz(l);
	int sqrt_q = (int)(sqrt(q) + EPS);
	res.resize(q);

	// a[0..n) を幅 √q のブロックに分割する．
	// クエリを左端の位置するブロックについて昇順に，
	// 次いで右端を偶数番目のブロックは昇順，奇数番目のブロックは降順でソートする．
	vector<tuple<int, int, int>> lb_sr_j(q);
	rep(j, q) {
		int b = l[j] / sqrt_q; 
		lb_sr_j[j] = { b, (b % 2 == 0 ? 1 : -1) * r[j], j };
	}
	sort(all(lb_sr_j));

	// ----------------------- ここを実装する -----------------------

	// 区間に a[i] を追加する場合の解 sol を更新する．
	function<void(int, S&)> insert = [&](int i, S& sol) {
		sol = sol;
	};

	// 区間から a[i] を削除する場合の解 sol を更新する．
	function<void(int, S&)> erase = [&](int i, S& sol) {
		sol = sol;
	};
	// --------------------------------------------------------------

	// 初期化
	int lpt = 0, rpt = 0; S sol = res00;

	// クエリを順に処理していく
	rep(k, q) {
		int j = get<2>(lb_sr_j[k]);

		// 区間を広げる
		while (lpt > l[j]) { insert(--lpt, sol); }
		while (rpt < r[j]) { insert(rpt++, sol); }

		// 区間を狭める
		while (lpt < l[j]) { erase(lpt++, sol); }
		while (rpt > r[j]) { erase(--rpt, sol); }

		res[j] = sol;
	}
}


//【法を m とした和クエリ】
/*
* Mod_sum_query(a) : O(n)
*	配列 a で初期化する．
*
* sum_mod(m) : O(max(a) log(n) / m)
*	a[0..n) mod m の和を返す．
* 
* sum_lack(m) : O(max(a) log(n) / m)
*	a[0..n) を m で割った不足の和を返す．
*/
struct Mod_sum_query {
	vi a;    // ★ a でなくバケツで累積和を持てば O(log n) を落とせる．
	int n;
	ll asum; // a[0..n) の和

	// コンストラクタ（何もしない）
	Mod_sum_query() : n(0), asum(0) {}

	// 配列 a で初期化
	Mod_sum_query(const vi& a_) : a(a_), n(sz(a)), asum(0) {
		sort(all(a));
		rep(i, n) asum += a[i];
	}

	// a[0..n) mod m の和を返す．
	ll sum_mod(int m) {
		ll res = asum;

		for (ll v = m; v <= a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			res -= (ll)m * distance(lower_bound(all(a), v), a.end());
		}

		return res;
	}

	// a[0..n) を m で割った不足の和を返す．
	ll sum_lack(int m) {
		// sum : 1-indexed での a[0..n) mod m の和
		ll sum = asum;

		for (ll v = m; v < a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			sum -= (ll)m * distance(lower_bound(all(a), v + 1), a.end());
		}

		// 不足分を返す．
		return (ll)m * n - sum;
	}
};


//【Convex-Hull Trick（挿入単調／クエリ単調）】
/*
* insert(l) : n 回で O(n)
*	l = {a, b} が表す直線 y = a x + b を追加する．
*	呼び出す際の挿入する直線の傾き a は降順でなくてはならない．
*
* min(x) : q 回で O(q)
*	a x + b の最小値を返す．
*	呼び出す際の x 座標は昇順でなくてはならない．
*/
struct Convex_hull_trick {
	// 参考 : https://satanic0258.hatenablog.com/entry/2016/08/16/181331
	// verify : https://atcoder.jp/contests/dp/tasks/dp_z

	int n; // 記録している直線の本数
	deque<pll> lines; // 直線を傾き狭義降順に記録したデック

	Convex_hull_trick() : n(0) {}

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


//【slope trick】
/*
* Slope_trick() : O(1)
*	f(x) = 0 で初期化する．
*
* min(lr) : O(1)
*	min f(x) を返し，必要ならそれを与える x の範囲 [l, r] を lr に格納する．
*
* add_const(x0) : O(1)
*	f(x) += y0 とする．
*
* add_right(x0) : O(1)
*	f(x) += min(x - x0, 0) とする．（＿／ の形を加算する．）
*
* add_left(x0) : O(1)
*	f(x) += min(x0 - x, 0) とする．（＼＿ の形を加算する．）
*
* add_abs(x0) : O(1)
*	f(x) += |x - x0| とする．（＼／ の形を加算する．）
*
* acc_min_left() : O(1)
*	f(x) を左から累積最小値をとったものに置き換える．（＼＿ の形にする．）
*
* acc_min_right() : O(1)
*	f(x) を右から累積最小値をとったものに置き換える．（＿／ の形にする．）
*
* shift(x0) : O(1)
*	f(x) を x0 だけ平行移動する．（f(x) ← f(x - x0)）
*
* sliding_window_min(x0, x1) : O(1)
*	f(x) を min f([x+x0, x+x1]) に置き換える．（＼＿＿／ の形にする．）
*/
struct Slope_trick {
	// 参考 : https://maspypy.com/slope-trick-1-%E8%A7%A3%E8%AA%AC%E7%B7%A8

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


