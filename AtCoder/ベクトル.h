#pragma once
#include "header.h"
// ■■■■■ ベクトル ■■■■■


//【一次独立なベクトル集合の数え上げ】
/*
* 体 F_p 上のベクトル空間 (F_p)^n において，一次独立な k 本のベクトルの選び方は
*		(1/k!) Πi∈[0..k) (p^n - p^i) [通り]
* ある．
* 
*（証明）順序による区別を付けて数え上げ，最後に k! で割れば良い．
* 1 本目 v1 の選び方は <0> の元を除く p^n - p^0 通りある．
* 2 本目 v2 の選び方は <v1> の元を除く p^n - p^1 通りある（v1 の選び方に依らない）
* 3 本目 v3 の選び方は <v1,v2> の元を除く p^n - p^2 通りある（v1,v2 の選び方に依らない）
* 以下同様で，一般に k 本目の選び方は p^n - p^k 通りあるので，積の法則から冒頭の式が従う．
*/


//【部分空間の数え上げ】
/*
* 体 F_p 上のベクトル空間 (F_p)^n において，次元が d の部分空間は
*		Πi∈[0..d) (p^n - p^i)/(p^d - p^i) [個]
* ある．これは q-二項係数 q-bin(n, d, p) に等しい．
* 
*（証明）求める場合の数を a[d] とおく．
* (F_p)^n から一次独立な d 本のベクトルを選ぶ方法は，上で見たように
*		(1/d!) Πi∈[0..d) (p^n - p^i) [通り]
* ある．一方，まず (F_p)^n の d 次元部分空間 V を選び，その後 V の基底を選ぶ方法は
*		a[d] * (1/d!) Πi∈[0..d) (p^d - p^i) [通り]
* ある．両者が等しいことから冒頭の式が従う．
*/


//【ベクトル】
/*
* STL の vector<T> を利用すればよい．
* 
* x + y, x - y, x * y : O(n)
*	n 次元ベクトル x, y の和，差，成分積を返す．+=, -=, *= も使用可．
*
* c * x ／ x * c : O(n)
*	n 次元ベクトル x とスカラー c の積を返す．*= も使用可．
*
* T dot(x, y) : O(n)
*	n 次元ベクトル x, y の内積を返す．
*/

// ベクトルの加算 : O(n)
template <class T> inline vector<T>& operator+=(vector<T>& x, const vector<T>& y) {
	rep(i, sz(x)) x[i] += y[i];
	return x;
}
template <class T> inline vector<T> operator+(const vector<T>& x, const vector<T>& y) {
	vector<T> res = x; return res += y;
}

// ベクトルの減算 : O(n)
template <class T> inline vector<T>& operator-=(vector<T>& x, const vector<T>& y) {
	rep(i, sz(x)) x[i] -= y[i];
	return x;
}
template <class T> inline vector<T> operator-(const vector<T>& x, const vector<T>& y) {
	vector<T> res = x; return res -= y;
}

// ベクトルの成分積 : O(n)
template <class T> inline vector<T>& operator*=(vector<T>& x, const vector<T>& y) {
	rep(i, sz(x)) x[i] *= y[i];
	return x;
}
template <class T> inline vector<T> operator*(const vector<T>& x, const vector<T>& y) {
	vector<T> res = x; return res *= y;
}

// スカラー倍 : O(n)
template <class T> inline vector<T>& operator*=(vector<T>& x, const T& c) {
	rep(i, sz(x)) x[i] *= c;
	return x;
}
template <class T> inline vector<T> operator*(const vector<T>& x, const T& c) {
	vector<T> res = x; return res *= c;
}
template <class T> inline vector<T> operator*(const T& c, const vector<T>& x) {
	return x * c;
}

// ベクトルの内積 : O(n)
template <class T> inline T dot(const vector<T>& x, const vector<T>& y) {
	T res = 0;
	rep(i, sz(x)) res += x[i] * y[i];
	return res;
}


//【ベクトル（固定サイズ）】
/*
* STL の array<T, N> を利用すればよい．
*
* x + y, x - y, x * y : O(N)
*	N 次元ベクトル x, y の和，差，成分積を返す．+=, -=, *= も使用可．
*
* c * x ／ x * c : O(N)
*	N 次元ベクトル x とスカラー c の積を返す．*= も使用可．
*
* T dot(x, y) : O(N)
*	N 次元ベクトル x, y の内積を返す．
*/

// ベクトルの加算 : O(N)
template <class T, size_t N> inline array<T, N>& operator+=(array<T, N>& x, const  array<T, N>& y) {
	rep(i, N) x[i] += y[i];
	return x;
}
template <class T, size_t N> inline array<T, N> operator+(const array<T, N>& x, const array<T, N>& y) {
	array<T, N> res = x; return res += y;
}

// ベクトルの減算 : O(n)
template <class T, size_t N> inline array<T, N>& operator-=(array<T, N>& x, const array<T, N>& y) {
	// verify : https://judge.yosupo.jp/problem/sum_of_multiplicative_function
	rep(i, N) x[i] -= y[i];
	return x;
}
template <class T, size_t N> inline array<T, N> operator-(const array<T, N>& x, const array<T, N>& y) {
	// verify : https://judge.yosupo.jp/problem/sum_of_multiplicative_function
	array<T, N> res = x; return res -= y;
}

// ベクトルの成分積 : O(n)
template <class T, size_t N> inline array<T, N>& operator*=(array<T, N>& x, const array<T, N>& y) {
	rep(i, N) x[i] *= y[i];
	return x;
}
template <class T, size_t N> inline array<T, N> operator*(const array<T, N>& x, const array<T, N>& y) {
	// verify : https://judge.yosupo.jp/problem/sum_of_multiplicative_function
	array<T, N> res = x; return res *= y;
}

// スカラー倍 : O(n)
template <class T, size_t N> inline array<T, N>& operator*=(array<T, N>& x, const T& c) {
	rep(i, N) x[i] *= c;
	return x;
}
template <class T, size_t N> inline array<T, N> operator*(const array<T, N>& x, const T& c) {
	array<T, N> res = x; return res *= c;
}
template <class T, size_t N> inline array<T, N> operator*(const T& c, const array<T, N>& x) {
	return x * c;
}

// ベクトルの内積 : O(n)
template <class T, size_t N> inline T dot(const array<T, N>& x, const array<T, N>& y) {
	// verify : https://judge.yosupo.jp/problem/sum_of_multiplicative_function
	T res = 0;
	rep(i, N) res += x[i] * y[i];
	return res;
}


//【ベクトル（半環）】
/*
* 成分は半環 <S, add, o, mul, e> の元とする．
*
* x + y : O(n)
*	n 次元ベクトル x, y の和を返す．+= も使用可．
*
* x * y : O(n)
*	n 次元ベクトル x, y の成分積を返す．
*
* c * x ／ x * c : O(n)
*	n 次元ベクトル x とスカラー c の積を返す．
*
* S dot(x, y) : O(n)
*	n 次元ベクトル x, y の内積を返す．
*/

// ベクトルの加算 : O(n)
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
inline vector<S>& operator+=(vector<S>& x, const vector<S>& y) {
	rep(i, sz(x)) x[i] = add(x[i], y[i]);
	return x;
}
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
inline vector<S> operator+(const vector<S>& x, const vector<S>& y) {
	vector<S> res = x; return res += y;
}

// ベクトルの成分積 : O(n)
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
inline vector<S> operator*(const vector<S>& x, const vector<S>& y) {
	int n = sz(x);
	vector<S> res(n);
	rep(i, n) res[i] = mul(x[i], y[i]);
	return res;
}

// スカラー倍 : O(n)
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
inline vector<S> operator*(const vector<S>& x, const S& c) {
	int n = sz(x);
	vector<S> res(n);
	rep(i, n) res[i] = mul(x[i], c);
	return res;
}
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
inline vector<S> operator*(const S& c, const vector<S>& x) {
	int n = sz(x);
	vector<S> res(n);
	rep(i, n) res[i] = mul(c, x[i]);
	return res;
}

// ベクトルの内積 : O(n)
/* verify : https://codeforces.com/contest/1681/problem/E  */
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
inline S dot(const vector<S>& x, const vector<S>& y) {
	S res = o();
	rep(i, sz(x)) res = add(res, mul(x[i], y[i]));
	return res;
}


