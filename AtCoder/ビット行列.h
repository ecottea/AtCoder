#pragma once
#include "header.h"
// ■■■■■ ビット行列 ■■■■■



//【ビット行列】
/*
* ビット行列を表す構造体
*
* Matrix(m) : O(m n)
*	m * n 零行列で初期化する．
*
* Matrix(a) : O(m n)
*	配列 a の要素で初期化する．
*
* insert(col) : O(n)
*	最下行に col を追加する．
*/
template <int n> // n : 列数
struct BMatrix {
	int m; // 行数（行列のサイズは m × n）
	vector<bitset<n>> v; // 行列の成分


	// コンストラクタ（初期化なし）
	BMatrix() : m(0) {}

	// コンストラクタ（m 行の零行列で初期化）
	BMatrix(const int& m_) : m(m_), v(m_) {}

	// コンストラクタ（二次元配列で初期化）
	BMatrix(const vector<bitset<n>>& a) : m(sz(a)), v(a) {}

	// コピーコンストラクタ
	BMatrix(const BMatrix& old) = default;


	// 代入
	BMatrix& operator=(const BMatrix& other) = default;

	// 出力
	friend ostream& operator<<(ostream& os, const BMatrix& mat) {
		rep(i, mat.m) {
			// 逆順にして出力し，vector の出力順と合わせる．
			string s = mat.v[i].to_string();
			reverse(all(s));
			os << s << endl;
		}
		return os;
	}

	// 行の挿入
	void insert(const bitset<n>& col) {
		v.push_back(col);
		m++;
	}
};


//【行基本変形】O(m^2 n)
/*
* 行基本変形で mat を階段行列に変形する．
* 最も右下のピボットの位置 (i, j) を返す．零行列なら (-1, -1) を返す．
*
*（呼び出すとき priority_solve<n> としないと gcc でエラーになるので注意．）
*/
template <int n>
pii row_echelon_form(BMatrix<n>& mat) {
	auto m = mat.m;
	auto& v = mat.v;

	// 直前に見つけたピボットの位置
	int pi = -1, pj = -1;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	while (i < m && j < n) {
		// 同じ列の下方の行から 1 を見つける．
		int i2 = i;
		while (i2 < m && !v[i2][j]) {
			i2++;
		}

		// 見つからなかったら注目位置を右に移す．
		if (i2 == m) {
			j++;
			continue;
		}

		// 見つかったら i 行目とその行を入れ替える．
		pi = i;
		pj = j;
		swap(v[i], v[i2]);

		// v[i][j] より下方の行の成分が全て 0 になるよう xor をとる．
		for (i2++; i2 < m; i2++) {
			if (v[i2][j]) v[i2] ^= v[i];
		}

		// 注目位置を右下に移す．
		i++;
		j++;
	}
	return { pi, pj };
}


//【連立一次方程式（優先度付き）】O(m^2 n)
/*
* m * (n + 1) 拡大係数行列 mat で表される連立一次方程式の解の 1 つを sol に格納する．
* ただし条件式は上の行のものほど優先し，矛盾した下行の条件式は無視する．
*
*（呼び出すとき priority_solve<N> としないと gcc でエラーになるので注意．）
*/
template <int N>
void priority_solve(BMatrix<N>& mat, int n, bitset<N>& sol) {
	auto m = mat.m;
	auto& v = mat.v;

	// ピボットの位置を記録しておくリスト
	vector<pii> pivots;

	rep(i, m) {
		// 同じ行の右の係数から 1 を見つける．
		int j = 0;
		while (j < n && !v[i][j]) {
			j++;
		}

		// 見つからなかったら定数項は見ず次の行へ
		if (j == n) {
			continue;
		}

		// j 列目に見つかったら j 列目が 1 である他の行と xor をとる．
		pivots.push_back({ i, j });
		rep(i2, m) {
			if (v[i2][j] && i2 != i) v[i2] ^= v[i];
		}
	}

	sol.reset();
	repe(p, pivots) {
		sol[p.second] = v[p.first][n];
	}
}


