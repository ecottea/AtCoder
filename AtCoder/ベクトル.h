#pragma once
#include "header.h"
// ■■■■■ ベクトル ■■■■■


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
* dot(x, y) : O(n)
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
* dot(x, y) : O(n)
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


