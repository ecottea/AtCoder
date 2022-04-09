#pragma once
#include "header.h"
// ■■■■■ 疎行列 ■■■■■


//【疎行列】
/*
* n 行の疎行列を表す構造体
*
* K 個の要素を除き値は 0 である．
* 行ごとに (列, 値) のリストをもつ．
*
* SMatrix(int n) : O(n)
*	n 行の零行列で初期化する．
*
* set(int i, int j, T v) : O(1)
*	A[i][j] = v とする．
*
* c * A ／ A * c : O(K)
*	疎行列 A とスカラー c のスカラー積を返す．*= も使用可．
*
* A * v : O(K)
*	疎行列 A と列ベクトル v の積を返す．
*/
template <class T> struct SMatrix {
	int n; // 行列のサイズ（n 行）
	vector<vector<pair<int, T>>> v; // 行列の成分

	// コンストラクタ（初期化なし，n 行の零行列，リスト）
	SMatrix() {}
	SMatrix(const int& n_) : n(n_), v(n) {}

	// 代入
	SMatrix(const SMatrix& old) = default;
	SMatrix& operator=(const SMatrix& other) = default;

	// 要素の設定
	void set(int i, int j, T val) { v[i].push_back({ j, val }); }

	// スカラー倍
	SMatrix& operator*=(const T& sc) {
		rep(i, n) {
			repe(p, v[i]) p.second *= sc;
		}
		return *this;
	}
	SMatrix operator*(const T& sc) const { return SMatrix(*this) *= sc; }
	friend SMatrix operator*(const T& sc, const SMatrix& mat) { return mat * sc; }

	// 行列ベクトル積
	vector<T> operator*(const vector<T>& vec) const {
		vector<T> res(n);
		rep(i, n) {
			repe(p, v[i]) res[i] += p.second * vec[p.first];
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const SMatrix& a) {
		int m = 0;
		rep(i, a.n) repe(p, a.v[i]) chmax(m, p.first + 1);

		vector<vector<T>> tmp(a.n, vector<T>(m));
		rep(i, a.n) repe(p, a.v[i]) tmp[i][p.first] = p.second;

		rep(i, a.n) {
			rep(j, m) os << tmp[i][j] << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【疎行列同士の積】
/*
* 疎行列同士の積は疎行列とは限らない．
* 例えば，全て 1 の列ベクトル * 全て 1 の行ベクトル で全て 1 の行列になる．
*/


