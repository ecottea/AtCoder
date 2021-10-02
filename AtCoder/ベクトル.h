#pragma once
#include "header.h"
// ■■■■■ ベクトル ■■■■■


//【ベクトル】
/*
* STL の vector<T> を利用すればよい．
* 
* x + y : O(n)
*	n 次元ベクトル x, y の和を返す．+= も使用可．
*
* x - y : O(n)
*	n 次元ベクトル x, y の差を返す．-= も使用可．
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


