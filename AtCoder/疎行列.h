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
template <class T>
struct SMatrix {
	int n; // 行列のサイズ（m 行）
	vector<vector<pair<int, T>>> v; // 行列の成分


	// コンストラクタ（初期化なし）
	SMatrix() {}

	// コンストラクタ（m 行の零行列で初期化）
	SMatrix(const int& n_) : n(n_), v(n) {}


	// コピーコンストラクタ
	SMatrix(const SMatrix& old) = default;

	// 代入
	SMatrix& operator=(const SMatrix& other) = default;

	// 右からのスカラー倍
	SMatrix operator*(const T& sc) const {
		SMatrix res(n);
		rep(i, n) {
			repe(p, v[i]) {
				res.v[i].push_back({ p.first, p.second * sc });
			}
		}
		return res;
	}
	SMatrix& operator*=(const T& sc) {
		rep(i, n) {
			for (auto& p : v[i]) {
				p.second *= sc;
			}
		}
		return *this;
	}

	// 行列ベクトル積 : O(K)
	vector<T> operator*(const vector<T>& vec) const {
		vector<T> res(n);
		rep(i, n) {
			repe(p, v[i]) {
				res[i] += p.second * vec[p.first];
			}
		}
		return res;
	}
};

// 左からのスカラー倍 : O(K)
template <class T>
SMatrix<T> operator*(const T& sc, SMatrix<T>& mat) {
	return mat * sc;
}

