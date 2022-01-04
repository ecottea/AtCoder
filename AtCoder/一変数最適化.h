#pragma once
#include "header.h"
// ■■■■■ 一変数関数の最小化[最大化] ■■■■■


//【差の総和の最小化】O(n log n)
/*
* a[0..n) について min_x Σi |a[i] - x| を返す．
*/
ll minimize_difference_sum(vl a) {
	int n = sz(a);

	//【方法】
	// f(x) = Σi |a[i] - x| のグラフは下に凸の折れ線状になり，
	// x が a の中央値のところで最小値をとる．
	// n が偶数のときは底が平らになるので，厳密な中央値でなくても構わない．

	sort(all(a));

	// x : a の中央値
	ll x = a[n / 2];

	ll res = 0;

	// 中央値までと中央値以降で符号を場合分けする．
	rep(i, n / 2) res -= a[i] - x;
	repi(i, n / 2, n - 1) res += a[i] - x;

	return res;
}


//【二乗の総和の最小化】O(n)
/*
* a[0..n) について min_x Σi (a[i] - x)^2 を返す．
*/
template <class T> double minimize_squared_sum(const vector<T>& a) {
	int n = sz(a);

	//【方法】
	// f(x) = Σi (a[i] - x)^2 のグラフは下に凸の放物線になり，
	// 軸の位置は x = Σa[0..n) / n = mean(x) である．

	// a_mean : a[0..n) の平均
	double a_mean = accumulate(all(a), 0.) / n;

	double res = 0;
	rep(i, n) res += pow(a[i] - a_mean, 2.);

	return res;
}


//【XOR の総和の最小化】O(n)
/*
* a[0..n) について min_x Σi (a[i] XOR x) を返す．
*/
ll minimize_xor_sum(const vl& a) {
	int n = sz(a);
	int m = msbll(*max_element(all(a))) + 1;

	//【方法】
	// 2 進数表記したときの桁 j ごとに独立に考えることができる．
	// a[0..n) の j 桁目の 1 の個数が n/2 以上[以下] なら
	// x の j 桁目は 1[0] にするのがよい．

	vi cnt(m);
	rep(i, n) {
		rep(j, m) {
			if (a[i] & (1LL << j)) cnt[j]++;
		}
	}
	dump(cnt);

	ll res = 0;
	rep(j, m) {
		res += (ll)min(cnt[j], n - cnt[j]) << j;
	}

	return res;
}


//【XOR の総和の最大化】O(n)
/*
* a[0..n) について max_(x<=k) Σi (a[i] XOR x) を返す．
*/
ll maximize_xor_sum(const vl& a, ll k) {
	// verify : https://atcoder.jp/contests/abc117/tasks/abc117_d

	int n = sz(a);
	int m = msbll(max(*max_element(all(a)), k)) + 1;

	//【方法】
	// 2 進数表記したときの上の桁から貪欲に考える．
	// a[0..n) の j 桁目の 1 の個数が n/2 以上[以下] なら
	// x の j 桁目は可能なら 0[1] にするのがよい．

	vi cnt(m);
	rep(i, n) {
		rep(j, m) {
			if (a[i] & (1LL << j)) cnt[j]++;
		}
	}

	bool smaller = false;

	ll res = 0;
	repir(j, m - 1, 0) {
		ll c1 = cnt[j], c0 = n - c1;

		if (smaller) {
			res += max(c0, c1) << j;
		}
		else {
			if (k & (1LL << j)) {
				res += max(c0, c1) << j;
				if (c0 <= c1) smaller = true;
			}
			else {
				res += c1 << j;
			}
		}
	}

	return res;
}


