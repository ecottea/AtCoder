#pragma once
#include "header.h"
// ■■■■■ 周期性 ■■■■■


//【周期数列の和】
/*
* Periodic_sequence_sum(function<T(T)> f, T a0) : O(nc + c)
*	a[i+1] = f(a[i]), a[0] = a0 なる数列で初期化する．
*
* int nc : O(1)
*	数列 a の非周期部分の長さ．
*
* int c : O(1)
*	数列 a の周期部分の長さ．
*
* T sum(ll n)
*	Σi=[0..n) a[i] を返す．
*
*（フロイドの循環検出法）
*/
template <class T>
struct Periodic_sequence_sum {
	// verify : https://atcoder.jp/contests/abc179/tasks/abc179_e

	// nc : 非周期部分の長さ，c : 周期部分の長さ
	int nc, c;

	// nc_acc[i] : Σa[0..i)，c_acc[i] : Σa[nc..nc+i)
	vector<T> nc_acc, c_acc;

	// a[i+1] = f(a[i]), a[0] = a0 なる数列で初期化する．
	Periodic_sequence_sum(const function<T(T)>& f, T a0) : nc(0), c(0), nc_acc({ 0 }), c_acc({ 0 }) {
		T x = a0, y = a0;
		do {
			x = f(x);
			y = f(f(y));
		} while (x != y);

		x = a0;
		while (x != y) {
			nc_acc.push_back(nc_acc[nc] + x);
			nc++;

			x = f(x);
			y = f(y);
		}

		do {
			c_acc.push_back(c_acc[c] + x);
			c++;

			x = f(x);
			y = f(f(y));
		} while (x != y);
	}

	// Σi = [0..n) a[i] を返す．
	T sum(ll n) {
		if (n <= nc) return nc_acc[n];

		T res = nc_acc[nc];
		n -= nc;
		res += c_acc[c] * (n / c);
		res += c_acc[n % c];

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Periodic_sequence_sum& pss) {
		os << "nc: " << pss.nc_acc << "(" << pss.nc << ")" << endl;
		os << " c: " << pss.c_acc << "(" << pss.c << ")" << endl;
		return os;
	}
#endif
};


//【フロイドの循環検出法】O(nc + c)
/*
* a[i+1] = f(a[i]), a[0] = a0 なる数列について，a[0] から始まる非周期列の長さ nc と
* a[nc] から始まる周期列の長さ c の組 {nc, c} を返す．
*/
template <class T>
pii floyds_cycle_finding(const function<T(T)>& f, T a0) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%95%E3%83%AD%E3%82%A4%E3%83%89%E3%81%AE%E5%BE%AA%E7%92%B0%E6%A4%9C%E5%87%BA%E6%B3%95
	// verify : https://atcoder.jp/contests/abc030/tasks/abc030_d

	T x = a0, y = a0;
	do {
		x = f(x);
		y = f(f(y));
	} while (x != y);

	x = a0;
	int nc = 0;
	while (x != y) {
		x = f(x);
		y = f(y);
		nc++;
	}

	int c = 0;
	do {
		x = f(x);
		y = f(f(y));
		c++;
	} while (x != y);

	return make_pair(nc, c);
}


//【周期境界条件の扱い】
/*
* 周期境界条件は扱いにくいので，次のようにして扱うとよい：
* 
* 列を 2 倍に伸ばし，任意の区間が連続的に並ぶようにする．
* verify : https://atcoder.jp/contests/arc024/tasks/arc024_2
* 
* 左端を決め打ちして計算し，右端で条件を満たすものだけを採用する．
* verify : https://atcoder.jp/contests/abc229/tasks/abc229_f
*/


//【列の最小周期】O(n)
/*
* 分析(文字列).h へ
*/

