#pragma once
#include "header.h"
// ■■■■■ 探索 ■■■■■


//【めぐる式二分探索】O(log|ok - ng|)
/*
* 条件 okQ() を満たす要素 ok と満たさない要素 ng との境界を二分探索する．
* 境界に隣り合うような条件を満たす要素（ok 側）の位置を返す．
*/
template <typename T> T meguru_search(T ok, T ng, function<bool(T)>& okQ) {
	// 境界が決定するまで
	while (abs(ok - ng) > 1) {
		// 区間の中間
		T mid = (ok + ng) / 2;

		// 中間が OK かどうかに応じて区間を縮小する．
		if (okQ(mid)) ok = mid;
		else ng = mid;
	}
	return ok;
}


//【二分探索（実数）】O(log(|ok - ng| / EPS))
/*
* 条件 okQ() を満たす要素 ok と満たさない要素 ng との境界を二分探索する．
*/
double binary_search(double ok, double ng, function<bool(double)>& okQ) {
	// 絶対誤差 EPS で境界が決定するまで
	while (abs(ok - ng) > EPS) {
		// 区間の中間
		double mid = (ok + ng) / 2;

		// 相対誤差が EPS 以下なら終了する．
		if (abs(ok - ng) <= mid * EPS) {
			break;
		}

		// 中間が OK かどうかに応じて区間を縮小する．
		if (okQ(mid)) {
			ok = mid;
		}
		else {
			ng = mid;
		}
	}
	return (ok + ng) / 2;
}


//【フィボナッチ探索】
/*
* fibonacci_search(w) : O(log w)
*	最大で幅 w の開区間まで扱えるよう初期化する．
*
* search(left, right, f, up) : O(log(right - left))
*	関数 f(i) の開区間 (left, right) における最大[小]値を与える i を返す．
*	up = true なら f の階差の符号変化は + → 0 → - で，返すのは最大値となる．
*	up = false なら f の階差の符号変化は - → 0 → + で，返すのは最小値となる．
*/
struct fibonacci_search {
	int n;
	vl fib;

	fibonacci_search(ll w) : n(1), fib({ 1, 1 }) {
		// 利用する範囲のフィボナッチ数列を準備する．
		while (fib[n] < w) {
			fib.push_back(fib[n] + fib[n - 1]);
			n++;
		}
		dump(fib);
	}

	ll search(ll left, ll right, const function<ll(ll)>& f_, bool up = true) const {
		function<ll(ll)> f = [&](ll x) {
			// 符号変化の条件を満たすよう範囲外の値を定めておく．
			ll val;
			if (x <= left) {
				val = -INFL - (left - x);
			}
			else if (x >= right) {
				// たぶん大丈夫だけどオーバーフローに注意
				val = -INFL - (x - right);
			}
			else {
				val = (up ? f_(x) : -f_(x));
			}

			return val;
		};
		repi(i, -2, 5) {
			dump(f(i));
		}

		// l, m1, m2, r の順で区間を φ: 1 :φ に内分する点を得る．
		int i = n;
		ll l = left;
		ll r = l + fib[i];
		ll m1 = l + fib[i - 2];
		ll m2 = l + fib[i - 1];
		i -= 3;

		// 内分点における関数値の計算
		ll v1 = f(m1);
		ll v2 = f(m2);

		// 候補が内分点のみになるまで
		while (i > 0) {
			// 左の内分点での値の方が大きければ，次の区間は左側をとる．
			if (v1 > v2) {
				// 右の内分点を新たに右端とする．
				r = m2;

				// 左の内分点を新たに右の内分点とする．
				m2 = m1;
				v2 = v1;

				// 左の内分点を新たに計算する．
				m1 = l + fib[i];
				v1 = f(m1);
			}
			// 右の内分点での値の方が大きければ，次の区間は右側をとる．
			else {
				// 左の内分点を新たに左端とする．
				l = m1;

				// 右の内分点を新たに左の内分点とする．
				m1 = m2;
				v1 = v2;

				// 右の内分点を新たに計算する．
				m2 = r - fib[i];
				v2 = f(m2);
			}
			i--;
		}

		// 最後の候補を比較し，大きかった方の番号を返す．
		return (v1 > v2) ? m1 : m2;
	}
};


//【黄金分割探索（実数）】O(log((right - left) / EPS))
/*
* 黄金分割探索を用いて，全域で狭義に上に凸な関数 f(x) の開区間 (left, right)
* における最大値を与える x を返す．
*/
double golden_search(double left, double right, function<double(double)>& f) {
	const double phi = 1.618033988749894848204586834365638;
	// l, m1, m2, r の順で区間を φ: 1 :φ に内分する点
	double l = left;
	double r = right;
	double m1 = (l * (1 + phi) + r * phi) / (2 * phi + 1);
	double m2 = (l * phi + r * (1 + phi)) / (2 * phi + 1);

	// 内分点における関数値の計算
	double v1 = f(m1);
	double v2 = f(m2);

	// 絶対誤差か相対誤差が EPS 以下になるまで
	while (r - l > EPS && r - l > EPS * (r + l) / 2) {
		// 左の内分点での値の方が大きければ，次の区間は左側をとる．
		if (v1 > v2) {
			// 右の内分点を新たに右端とする．
			r = m2;

			// 左の内分点を新たに右の内分点とする．
			m2 = m1;
			v2 = v1;

			// 左の内分点を新たに計算する．
			m1 = (l * (1 + phi) + r * phi) / (2 * phi + 1);
			v1 = f(m1);
		}
		// 右の内分点での値の方が大きければ，次の区間は右側をとる．
		else {
			// 左の内分点を新たに左端とする．
			l = m1;

			// 右の内分点を新たに左の内分点とする．
			m1 = m2;
			v1 = v2;

			// 右の内分点を新たに計算する．
			m2 = (l * phi + r * (1 + phi)) / (2 * phi + 1);
			v2 = f(m2);
		}
	}

	// 最後の候補を比較し，大きかった方の x を返す．
	return (v1 > v2) ? m1 : m2;
}


//【組の和の探索】
/*
* Sort_outer_sum(a, b) : O(n log n + m log m)
*	S = { a[i] + b[j] | i∈[0..n), j∈[0..m) } で初期化する．
*
* lower_bound(v) : O(n log m)
*	S の v 未満の要素の個数を返す．
*
* upper_bound(v) : O(n log m)
*	S の v 以下の要素の個数を返す．
*
* get(i) : O(n log m log(max(a+b) - min(a+b)))
*	S の i 番目の要素を返す．
*
* sum(i) : O(n log m log(max(a+b) - min(a+b)))
*	S の i 番目未満の要素の和を返す．
*
* 利用：【めぐる式二分探索】
*/
struct Sort_outer_sum {
	int n, m;
	vl a, b, acc_b;

	Sort_outer_sum() {}

	// S = { a[i] + b[j] | i∈[0..n), j∈[0..m) } で初期化する．
	Sort_outer_sum(const vl& a_, const vl& b_)
		: n(sz(a_)), m(sz(b_)), a(a_), b(b_) {
		sort(all(a));
		sort(all(b));

		acc_b = vl(m + 1);
		rep(j, m) {
			acc_b[j + 1] = acc_b[j] + b[j];
		}
	}

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
		ll cnt = 0;
		rep(i, n) {
			auto it = std::lower_bound(all(b), v - a[i]);
			cnt += (ll)distance(b.begin(), it);
		}
		return cnt;
	}

	// S の v 以下の要素の個数を返す．
	ll upper_bound(ll v) {
		ll cnt = 0;
		rep(i, n) {
			auto it = std::upper_bound(all(b), v - a[i]);
			cnt += (ll)distance(b.begin(), it);
		}
		return cnt;
	}

	// S の i 番目の要素を返す．
	ll get(ll i) {
		function<bool(ll)> okQ = [&](ll v) {
			return lower_bound(v) <= i;
		};
		return binary_search(a[0] + b[0] - 1, a[n - 1] + b[m - 1] + 1, okQ);
	}

	// S の i 番目未満の要素の和を返す．
	ll sum(ll i) {
		// v : i 番目の要素 
		ll v = get(i);

		// sum : v 未満の要素の和
		// cnt : v 未満の要素の個数
		ll sum = 0, cnt = 0;
		rep(i, n) {
			auto it = std::lower_bound(all(b), v - a[i]);
			int d = (int)distance(b.begin(), it);
			sum += a[i] * d + acc_b[d];
			cnt += d;
		}

		// 残り i - cnt 個の要素はちょうど v であるからその分を加算する．
		sum += v * (i - cnt);

		return sum;
	}
};


//【組の積の探索】
/*
* sort_outer_mul(a, b) : O(n log n + m log m)
*	S = { a[i] b[j] | i∈[0..n), j∈[0..m) } で初期化する．
*
* lower_bound(v) : O(n log m)
*	S の v 未満の要素の個数を返す．
*
* get(i) : O(n log m log(INFL))
*	S の i 番目の要素を返す．
*
* 利用：【めぐる式二分探索】
*/
struct sort_outer_mul {
	// n, m : a, b の要素数
	// np, mp : a, b の正の要素数
	// nz, mz : a, b の 0 の要素数
	// nn, mn : a, b の負の要素数
	int n, np, nz, nn, m, mp, mz, mn;

	// ap, bp : a, b の正の要素を昇順に格納したリスト
	// an, bn : a, b の負の要素の 絶対値 を昇順に格納したリスト
	vl ap, an, bp, bn;


	sort_outer_mul() {}

	// S = { a[i] b[j] | i∈[0..n), j∈[0..m) } で初期化する．
	sort_outer_mul(const vl& a, const vl& b) {
		np = nz = nn = 0;
		repe(x, a) {
			if (x > 0) {
				ap.push_back(x);
				np++;
			}
			else if (x < 0) {
				an.push_back(-x);
				nn++;
			}
			else {
				nz++;
			}
		}
		sort(all(ap));
		sort(all(an));
		n = np + nz + nn;

		mp = mz = mn = 0;
		repe(x, b) {
			if (x > 0) {
				bp.push_back(x);
				mp++;
			}
			else if (x < 0) {
				bn.push_back(-x);
				mn++;
			}
			else {
				mz++;
			}
		}
		sort(all(bp));
		sort(all(bn));
		m = mp + mz + mn;
	}

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
		ll cnt = 0;
		if (v > 0) {
			cnt += (ll)m * n - (ll)np * mp - (ll)nn * mn;
			repe(x, ap) {
				auto it = std::lower_bound(all(bp), (v + x - 1) / x);
				cnt += (ll)distance(bp.begin(), it);
			}
			repe(x, an) {
				auto it = std::lower_bound(all(bn), (v + x - 1) / x);
				cnt += (ll)distance(bn.begin(), it);
			}
		}
		else if (v < 0) {
			repe(x, ap) {
				auto it = std::upper_bound(all(bn), -v / x);
				cnt += (ll)distance(it, bn.end());
			}
			repe(x, an) {
				auto it = std::upper_bound(all(bp), -v / x);
				cnt += (ll)distance(it, bp.end());
			}
		}
		else {
			cnt += (ll)np * mn + (ll)nn * mp;
		}
		return cnt;
	}

	// S の i 番目の要素を返す．
	ll get(ll i) {
		function<bool(ll)> okQ = [&](ll v) {
			return lower_bound(v) <= i;
		};
		return binary_search(-INFL, INFL, okQ);
	}
};


