#pragma once
#include "header.h"
// ■■■■■ 疎行列 ■■■■■


//【疎行列】
/*
* n 行の疎行列を表す構造体
*
* K 個の要素を除き値は 0 である．
* 行ごとに (列, 値) を連想配列で持つ．
*
* SMatrix(n) : O(n)
*	n 行の零行列で初期化する．
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

	// コンストラクタ（初期化なし，n 行の零行列，連想配列）
	SMatrix() {}
	SMatrix(const int& n_) : n(n_), v(n) {}
	SMatrix(const vector<vector<pair<int, T>>>& v_) : n(sz(v_)), v(v_) {}

	// 代入
	SMatrix(const SMatrix& old) = default;
	SMatrix& operator=(const SMatrix& other) = default;

	// スカラー倍
	SMatrix& operator*=(const T& sc) {
		rep(i, n) {
			repe(p, v[i]) p.second *= sc;
		}
		return *this;
	}
	SMatrix operator*(const T& sc) const { return SMatrix(*this) *= sc; }
	friend SMatrix operator*(const T& sc, const SMatrix& mat) { return mat * sc; }

	// 行列ベクトル積 : O(K)
	vector<T> operator*(const vector<T>& vec) const {
		vector<T> res(n);
		rep(i, n) {
			repe(p, v[i]) res[i] += p.second * vec[p.first];
		}
		return res;
	}
};


