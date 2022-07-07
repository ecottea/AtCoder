#pragma once
#include "header.h"
// ■■■■■ ビット行列 ■■■■■


//【ビット行列】
/*
* ビット行列を表す構造体
*
* Bit_matrix<N>(int m, int n) : O(m N / 64)
*	m * n 零行列で初期化する．
*	制約：n <= N
*
* Bit_matrix<N>(int n) : O(n N / 64)
*	n * n 単位行列で初期化する．
*
* Bit_matrix<N>(vector<bitset<N>> a, int n) : O(m N / 64)
*	配列 a の要素で初期化する．
*
* push_back(bitset<N> col) : O(N / 64)
*	最下行に col を追加する．
*
* A * x : O(m N / 64)
*	m * n 行列 A と n 次元列ベクトル x の積を返す．
*	制約：m <= N
*
* A * B : O(l m n)
*	l * m 行列 A と m * n 行列 B の積を返す．
*
* pow(ll d) : O(n^3 log d)
*	自身を d 乗した行列を返す．
*
* transpose() : O(m n)
*	自身を転置した行列を返す．
*/
template <int N> struct Bit_matrix {
	int m, n; // 行数, 列数（行列のサイズは m * n）
	vector<bitset<N>> v; // 行列の成分

	// コンストラクタ（初期化なし，零行列，単位行列，二次元配列）
	Bit_matrix() : m(0), n(0) {}
	Bit_matrix(int m_, int n_) : m(m_), n(n_), v(m) {}
	Bit_matrix(int n_) : m(n_), n(n_), v(m) { rep(i, n) v[i][i] = 1; }
	Bit_matrix(const vector<bitset<N>>& a, int n_) : m(sz(a)), n(n_), v(a) {}

	// 代入
	Bit_matrix(const Bit_matrix& old) = default;
	Bit_matrix& operator=(const Bit_matrix& other) = default;

	// 比較
	bool operator==(const Bit_matrix& g) const { return m == g.m && n == g.n && v == g.v; }
	bool operator!=(const Bit_matrix& g) const { return !(*this == g); }

	// アクセス
	bitset<N> const& operator[](int i) const { return v[i]; }
	bitset<N>& operator[](int i) { return v[i]; }

	// 行の追加
	void push_back(const bitset<N>& col) { v.push_back(col); m++; }

	// 行列ベクトル積
	bitset<N> operator*(const bitset<N>& x) const {
		bitset<N> y;
		rep(i, m) y[i] = (v[i] & x).count() % 2;
		return y;
	}

	// 積
	Bit_matrix operator*(const Bit_matrix& b) const {
		Bit_matrix res(m, b.n);
		rep(i, res.m) rep(j, res.n) rep(k, n) res[i][j] = res[i][j] ^ (v[i][k] & b[k][j]);
		return res;
	}
	Bit_matrix& operator*=(const Bit_matrix& b) { *this = *this * b; return *this; }

	// 累乗
	Bit_matrix pow(ll d) const {
		Bit_matrix res(n), pow2 = *this;
		while (d > 0) {
			if ((d & 1) != 0) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// 転置（A^T）
	Bit_matrix transpose() const {
		Bit_matrix res(n, m);
		rep(i, res.m) rep(j, res.n) res[i][j] = v[j][i];
		return res;
	}
		
#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Bit_matrix& a) {
		rep(i, a.m) {
			rep(j, a.n) os << a.v[i][j] << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【転置との積】: O(l m n / 64)
/*
* l * m 行列 A と n * m 行列 B について，積 A * B^T を返す．
*/
template <int N> Bit_matrix<N> prod_transpose(const Bit_matrix<N>& A, const Bit_matrix<N>& B) {
	Bit_matrix<N> res(A.m, B.m);
	rep(i, res.m) rep(j, res.n) res[i][j] = (A[i] & B[j]).count() % 2;
	return res;
}


//【行簡約階段形】O(m^2 n / 64)
/*
* 行基本変形で mat を行簡約階段形に変形する（ピボットの上下は全て 0．）
* また i 行目のピボットが何列目かを pjs[i] に格納し，mat の階数を返す．
*
*（呼び出すとき reduced_row_echelon_form<N> としないと gcc でエラーになるので注意．）
*/
template <int N> int reduced_row_echelon_form(Bit_matrix<N>& mat, vi* pjs = nullptr) {
	// verify : https://atcoder.jp/contests/abc249/tasks/abc249_g

	int m = mat.m, n = mat.n;
	if (pjs != nullptr) pjs->clear();

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	while (i < m && j < n) {
		// 注目列の下方の行から 1 を見つける．
		int i2 = i;
		while (i2 < m && !mat[i2][j]) i2++;

		// 見つからなかったら注目位置を右に移す．
		if (i2 == m) {
			j++;
			continue;
		}

		// 見つかったら i 行目とその行を入れ替える．
		swap(mat[i], mat[i2]);

		// i 行目以外の j 列目の成分が全て 0 になるよう XOR をとる．
		rep(i2, m) {
			if (i2 == i) continue;

			if (mat[i2][j]) mat[i2] ^= mat[i];
		}

		// ピボット位置の記録
		if (pjs != nullptr) pjs->push_back(j);

		// 注目位置を右下に移す．
		i++; j++;
	}

	return i;
}


//【連立一次方程式】O(m^2 n / 64)
/*
* m * (n + 1) 拡大係数行列 mat で表される連立一次方程式の解の 1 つを sol に格納する．
* 解が存在しないなら false を返す．
*
*（呼び出すとき solve_eq<N> としないと gcc でエラーになるので注意．）
*/
template <int N> bool solve_eq(Bit_matrix<N>& mat, bitset<N>* sol = nullptr) {
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/1308

	int m = mat.m, n = mat.n - 1;
	auto& v = mat.v;

	// ピボットの位置を記録しておくリスト
	vector<pii> pivots;

	// 未確定の列を記録しておくリスト
	list<int> rmd;
	repi(j, 0, n) rmd.push_back(j);

	rep(i, m) {
		// i 行目の係数を左から走査し 1 を見つける．
		auto it = rmd.begin();
		for (; it != rmd.end(); it++) {
			if (v[i][*it] == 1) break;
		}

		// 全てが 0 なら無視
		if (it == rmd.end()) continue;
		int j = *it;
		rmd.erase(it);

		// 定数項のみが 1 なら解なし
		if (j == n) return false;

		// j 列目に見つかったら j 列目が 1 である他の行と XOR をとる．
		pivots.push_back({ i, j });
		rep(i2, m) {
			if (v[i2][j] && i2 != i) v[i2] ^= v[i];
		}
	}

	// 解の例の構成
	if (sol != nullptr) {
		sol->reset();
		repe(p, pivots) {
			(*sol)[p.second] = v[p.first][n];
		}
	}

	return true;
}


//【連立一次方程式（優先度付き）】O(m^2 n / 64)
/*
* m * (n + 1) 拡大係数行列 mat で表される連立一次方程式の解の 1 つを sol に格納する．
* ただし条件式は上の行のものほど優先し，矛盾した下行の条件式は無視する．
*
*（呼び出すとき priority_solve_eq<N> としないと gcc でエラーになるので注意．）
*/
template <int N> void priority_solve_eq(Bit_matrix<N>& mat, bitset<N>& sol) {
	// verify : https://atcoder.jp/contests/abc141/tasks/abc141_f

	int m = mat.m, n = mat.n - 1;
	auto& v = mat.v;

	// ピボットの位置を記録しておくリスト
	vector<pii> pivots;

	// 未確定の列を記録しておくリスト
	list<int> rmd;
	rep(j, n) rmd.push_back(j);

	rep(i, m) {
		// i 行目の係数を左から走査し 1 を見つける．
		auto it = rmd.begin();
		for (; it != rmd.end(); it++) {
			if (v[i][*it] == 1) break;
		}

		// 全てが 0 なら無視（定数項はチェックしない）
		if (it == rmd.end()) continue;
		int j = *it;
		rmd.erase(it);

		// j 列目に見つかったら j 列目が 1 である他の行と XOR をとる．
		pivots.push_back({ i, j });
		rep(i2, m) {
			if (v[i2][j] && i2 != i) v[i2] ^= v[i];
		}
	}

	// 解の例の構成
	sol.reset();
	repe(p, pivots) {
		sol[p.second] = v[p.first][n];
	}
}


//【基底の選択】O(m^2 n / 64)
/*
* m * n 行列 mat の行ベクトルで張られる空間を V とし，
* mat の第 i 行ベクトル（i ∈ bis）が V の基底となるよう bis に格納する．
* 基底は i が小さいものを優先して構成する．
*
*（呼び出すとき find_base<N> としないと gcc でエラーになるので注意．）
*/
template <int N> void find_base(Bit_matrix<N>& mat, vi& bis) {
	// verify : https://atcoder.jp/contests/agc045/tasks/agc045_a

	int m = mat.m, n = mat.n;
	auto v = mat.v;
	bis.clear();

	// 未確定の列を記録しておくリスト
	list<int> rmd;
	rep(j, n) rmd.push_back(j);

	rep(i, m) {
		// i 行目の係数を左から走査し 1 を見つける．
		auto it = rmd.begin();
		for (; it != rmd.end(); it++) {
			if (v[i][*it] == 1) break;
		}

		// 全てが 0 なら無視
		if (it == rmd.end()) continue;
		int j = *it;
		rmd.erase(it);

		// 1 が残っているなら基底に採用する．
		bis.push_back(i);

		// j 列目に見つかったら j 列目が 1 である他の行と XOR をとる．
		rep(i2, m) {
			if (v[i2][j] && i2 != i) v[i2] ^= v[i];
		}
	}
}


//【行列の離散対数問題】O(2^(dim(x)/2) dim(x)^3 / 64)
/*
* A^d x = y の最小解 d >= 0 を返す．（なければ INFL）
*
*（呼び出すとき log<N> としないと gcc でエラーになるので注意．）
*
*（baby-step giant-step）
*/
template <int N>
ll log(const Bit_matrix<N>& A, const bitset<N>& x, const bitset<N>& y) {
	// verify : https://atcoder.jp/contests/utpc2014/tasks/utpc2014_k

	ll hn = 1LL << ((A.m + 1) / 2);

	// logA[v] : v = A^j x となる m 未満で最大の j
	unordered_map<unsigned long long, int> logA;
	Bit_matrix<N> A_pow(A.n), A_trans = A.transpose();
	rep(j, hn) {
		if (((A_pow * x) ^ y).none()) return j;

		logA[(A_pow * y).to_ullong()] = j;

		A_pow = prod_transpose(A_pow, A_trans);
	}

	// x に A_pow = A^hn を掛けながら解の候補を探していく．
	bitset<N> Ax = x;
	repi(i, 1, hn) {
		Ax = A_pow * Ax;
		if (logA.count(Ax.to_ullong())) {
			// A^(i hn) x = A^j y なる (i, j) が見つかった．
			ll d = i * hn - logA[Ax.to_ullong()];

			// 実際に計算してみて一致するかを見る．
			if (((A.pow(d) * x) ^ y).none()) {
				return d;
			}
			return INFL; // ここで打ち切っていいとは限らないのでは？？
		}
	}

	return INFL;
}


