#pragma once
#include "header.h"
// ■■■■■ トロピカル畳込み ■■■■■


//【max-plus 畳込み】O(n m)
/*
* 数列 a[0..n) と b[0..m) を max-plus 代数にて畳み込んだ数列 c[0..n+m-1) を返す．
* すなわち c[i] = MAX j∈[0..i] (a[j] + b[i-j])  (∀i∈[0..n+m-1)) である．
*/
template <class T>
vector<T> naive_max_plus_convolution(const vector<T>& a, const vector<T>& b) {
	int n = sz(a), m = sz(b);
	if (min(n, m) == 0) return vector<T>();

	T T_MIN = numeric_limits<T>::lowest();

	// c[i] = MAX j∈[0..i] (a[j] + b[i-j])  (∀i∈[0..n+m-1))
	vector<T> c(n + m - 1, T_MIN);
	rep(i, n + m - 1) {
		repi(j, max(i - (m - 1), 0), min(i, n - 1)) {
			chmax(c[i], a[j] + b[i - j]);
		}
	}

	return c;
}


//【max-plus 畳込み（上に凸）】O(n + m)
/*
* 上に凸な数列 a[0..n) と b[0..m) を max-plus 代数にて畳み込んだ数列 c[0..n+m-1) を返す．
* 数列が上に凸であるとは，階差数列が広義単調減少であることをいう．
*/
template <class T>
vector<T> concave_max_plus_convolution(const vector<T>& a, const vector<T>& b) {
	// 参考 : https://twitter.com/maspy_stars/status/1396750434824450051
	// verify : https://atcoder.jp/contests/abc218/tasks/abc218_h

	//【方法】
	// 添字が小さい順に c を決定していく．c[0], c[1] は
	//		c[0] = a[0] + b[0]
	//		c[1] = max(a[0] + b[1], a[1] + b[0])
	// となる．
	// 
	// c[1] = a[0] + b[1] の場合を例として考える．c[2] は定義通りだと
	//		c[2] = max(a[0] + b[2], a[1] + b[1], a[2] + b[0])
	// であるが，
	//		a[2] + b[0]
	//		= (a[1] + b[0]) + (a[2] - a[1])
	//		≦ (a[0] + b[1]) + (a[2] - a[1]) （c[1] = a[0] + b[1] より）
	//		≦ (a[0] + b[1]) + (a[1] - a[0]) （a が上に凸より）
	//		= a[1] + b[1]
	// なので，候補を 2 つに絞り
	//		c[2] = max(a[0] + b[2], a[1] + b[1])
	// としてしまって良い．
	//
	// c[3] 以降も同様に考え候補を 2 つに絞ることができる．

	int n = sz(a), m = sz(b);

	// 一方が空数列だった場合は空数列を返す．
	if (min(n, m) == 0) return vector<T>();

	vector<T> c(n + m - 1);
	c[0] = a[0] + b[0];

	int i = 0, j = 0;
	while (i + j < n + m - 2) {
		if (i == n - 1 || (j != m - 1 && a[i + 1] - a[i] < b[j + 1] - b[j])) {
			c[i + j + 1] = c[i + j] + (b[j + 1] - b[j]);
			j++;
		}
		else {
			c[i + j + 1] = c[i + j] + (a[i + 1] - a[i]);
			i++;
		}
	}

	return c;
}


