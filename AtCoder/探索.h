#pragma once
#include "header.h"
// ■■■■■ 探索 ■■■■■


//【めぐる式二分探索】O(log|ok - ng|)
/*
* 条件 okQ() を満たす要素 ok と満たさない要素 ng との境界を二分探索する．
* 境界に隣り合うような条件を満たす要素（ok 側）の位置を返す．
*/
template <typename T> T meguru_search(T ok, T ng, function<bool(T)>& okQ) {
	// verify : https://algo-method.com/tasks/366

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
	// verify : https://algo-method.com/tasks/368

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
* Fibonacci_search(w) : O(log w)
*	最大で幅 w の開区間まで扱えるよう初期化する．
*
* search(left, right, f, up) : O(log(right - left))
*	関数 f(i) の開区間 (left, right) における最大[小]値を与える i を返す．
*	up = true なら f の階差の符号変化は + → 0 → - で，返すのは最大値となる．
*	up = false なら f の階差の符号変化は - → 0 → + で，返すのは最小値となる．
*/
struct Fibonacci_search {
	int n;
	vl fib;

	Fibonacci_search(ll w) : n(1), fib({ 1, 1 }) {
		// 利用する範囲のフィボナッチ数列を準備する．
		while (fib[n] < w) {
			fib.push_back(fib[n] + fib[n - 1]);
			n++;
		}
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
	const double phi = 1.61803398875;
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


//【並列二分探索】O(O(okQ) log max|ok[i] - ng[i]|)
/*
* i=[0..q) について，条件を満たす要素 ok[i] と満たさない要素 ng[i] の
* 境界を二分探索し，ok[i] を境界に接する条件を満たす要素に変更する．
* okQ は，okQ(mid, res) で呼び出すと mid[i] が条件を満たすかが res[i] に格納されるとする．
*/
template <typename T>
void parallel_binary_search(vector<T>& ok, vector<T>& ng,
	function<void(const vector<T>&, vb&)>& okQ)
{
	// 参考 : https://betrue12.hateblo.jp/entry/2019/08/14/152227
	// verify : https://atcoder.jp/contests/code-thanks-festival-2017-open/tasks/code_thanks_festival_2017_h

	int q = sz(ok); // クエリの数

	vector<T> mid(q);
	vb res(q);

	while (true) {
		bool update = false; // 更新が起こったか

		// それぞれのクエリの ok と ng の中央値を mid に格納する．
		rep(i, q) {
			if (abs(ok[i] - ng[i]) <= 1) continue;
			update = true;

			mid[i] = (ok[i] + ng[i]) / 2;
		}

		// 更新が起こらなかったなら探索終了
		if (!update) break;

		// mid に対して一括で ok か ng かを判定する．
		okQ(mid, res);

		////【テンプレ】
		//// mid の値ごとに処理するため，連想配列 mid → j を作る．
		//unordered_map<int, vi> mid_to_j;
		//rep(j, q) {
		//	mid_to_j[mid[j]].push_back(j);
		//}

		// 判定結果に応じて ok または ng を更新する．
		rep(i, q) {
			if (res[i]) ok[i] = mid[i];
			else ng[i] = mid[i];
		}
	}
}


