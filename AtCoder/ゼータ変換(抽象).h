#pragma once
#include "header.h"
// ■■■■■ 集合のゼータ変換・メビウス変換など ■■■■■


//【ゼータ変換（上位集合，可換モノイド）】O(2^n n)
/*
* 可換モノイド (S, op, e) の元を要素とする a[0..2^n) を
*       A[set] = Πsup⊃set a[sup]
* なる A[0..2^n) に上書きする（上位集合の値全てを自身に加える）
*/
template <class S, S(*op)(S, S), S(*e)()>
void superset_zeta(vector<S>& f) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(f));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) f[set] = op(f[set], f[set + (1 << i)]);
	}
}


//【メビウス変換（上位集合，アーベル群）】O(2^n n)
/*
* アーベル群 (S, op, e, inv) の元を要素とする A[0..2^n) を
*       A[set] = Πsup⊃set a[sup]
* なる a[0..2^n) に上書きする（上位集合からの自身の値への寄与を取り除く）
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
void superset_mobius(vector<S>& f) {
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(f));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) f[set] = op(f[set], inv(f[set + (1 << i)]));
	}
}


//【積集合畳込み（環）】O(2^n n)
/*
* 環 (S, add, o, mi, mul, e) の元を要素とする a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sup1∩sup2 = set) a[sup1] b[sup2]
* なる c[0..2^n) を返す．
*
* 利用：【ゼータ変換（上位集合，可換モノイド）】,【メビウス変換（上位集合，アーベル群）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
vector<S> and_convolution(vector<S> a, vector<S> b) {
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(a));

	superset_zeta<S, add, o>(a);
	superset_zeta<S, add, o>(b);

	repb(set, n) a[set] = mul(a[set], b[set]);

	superset_mobius<S, add, o, mi>(a);

	return a;
}


//【上位積集合畳込み（半環）】O(2^n n)
/*
* 半環 (S, add, o, mul, e) の元を要素とする a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sup1∩sup2 ⊃ set) a[sup1] b[sup2]
* なる c[0..2^n) を返す．
*
* 利用：【ゼータ変換（上位集合，可換モノイド）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
vector<S> superset_and_convolution(vector<S> a, vector<S> b) {
	int n = msb(sz(a));

	superset_zeta<S, add, o>(a);
	superset_zeta<S, add, o>(b);

	repb(set, n) a[set] = mul(a[set], b[set]);

	return a;
}


//【ゼータ変換（下位集合，可換モノイド）】O(2^n n)
/*
* 可換モノイド (S, op, e) の元を要素とする a[0..2^n) を
*       A[set] = Σsub⊂set a[sub]
* なる A[0..2^n) に上書きする（下位集合の値全てを自身に加える）
*/
template <class S, S(*op)(S, S), S(*e)()>
void subset_zeta(vector<S>& f) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(f));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) f[set + (1 << i)] = op(f[set + (1 << i)], f[set]);
	}
}


//【メビウス変換（下位集合，アーベル群）】O(2^n n)
/*
* アーベル群 (S, op, e, inv) の元を要素とする A[0..2^n) を
*       A[set] = Σsub⊂set a[sub]
* なる a[0..2^n) に上書きする（下位集合からの自身の値への寄与を取り除く）
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
void subset_mobius(vector<S>& f) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(f));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) f[set + (1 << i)] = op(f[set + (1 << i)], inv(f[set]));
	}
}


//【和集合畳込み（環）】O(2^n n)
/*
* 環 (S, add, o, mi, mul, e) の元を要素とする a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sub1∪sub2 = set) a[sub1] b[sub2]
* なる c[0..2^n) を返す．
*
* 利用：【ゼータ変換（下位集合，可換モノイド）】,【メビウス変換（下位集合，アーベル群）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
vector<S> or_convolution(vector<S> a, vector<S> b) {
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

	int n = msb(sz(a));

	subset_zeta<S, add, o>(a);
	subset_zeta<S, add, o>(b);

	repb(set, n) a[set] = mul(a[set], b[set]);

	subset_mobius<S, add, o, mi>(a);

	return a;
}


//【下位和集合畳込み（半環）】O(2^n n)
/*
* 半環 (S, add, o, mul, e) の元を要素とする a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sub1∪sub2 ⊂ set) a[sub1] b[sub2]
* なる c[0..2^n) を返す．
*
* 利用：【ゼータ変換（下位集合，可換モノイド）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
vector<S> subset_or_convolution(vector<S> a, vector<S> b) {
	int n = msb(sz(a));

	subset_zeta<S, add, o>(a);
	subset_zeta<S, add, o>(b);
	dump(a);

	repb(set, n) a[set] = mul(a[set], b[set]);

	return a;
}


//【複数の二項式の下位和集合畳込み】O(2^n n^2)
/*
* 半環 (S, add, o, mul, e) の元を要素とする 2^n 個の二項式 (e {} + a[set] set)
* を全て下位和集合畳込みした結果を返す．
*
* 利用：【下位和集合畳込み（半環）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
vector<S> multi_subset_or_convoluion(const vector<S>& a) {
	// verify : https://atcoder.jp/contests/abc215/tasks/abc215_h

	int N = sz(a);

	vector<vector<S>> f(N);
	rep(i, N) f[i] = { a[i] };

	// 2 冪個ずつまとめていく（分割統治法）
	for (int k = 1; k < N; k *= 2) {
		for (int i = 0; i + k < N; i += 2 * k) {
			auto c = subset_or_convolution<S, add, o, mul, e>(f[i], f[i + k]);
			rep(set, k) c[set] = add(c[set], f[i + k][set]);
			f[i].insert(f[i].end(), all(c));
		}
	}

	return f[0];
}


