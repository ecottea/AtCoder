#pragma once
#include "header.h"
#include "座標圧縮.h"
// ■■■■■ 置換，順列 ■■■■■



//【転倒数】O(n log n)
/*
* 長さ n の配列 a の転倒数を返す．
*
* 利用：【座標圧縮】
*/
template <class T> ll inversion_number(vector<T>& a) {
	int n = sz(a);

	// a を [0, |a|) に座標圧縮した結果を b に格納する．
	vi b;
	vector<T> tmp;
	int m = coordinate_compression(a, b, tmp);

	// fw[i] : 今まで見てきた範囲に値 i が何個あったか．
	fenwick_tree<int> fw(m);
	ll res = 0;
	rep(i, n) {
		fw.add(b[i], 1);

		// 自身より大きい数が今までに何個あったか調べ，加算する．
		res += fw.sum(b[i] + 1, m);
	}
	return res;
}


//【置換の分解】O(n)
/*
* [0..n) の置換 p を巡回置換の積に分解して cycles に格納する．
* p は任意の i を p[i] に動かすような置換を表す．
*/
int permutation_decomposition(const vi& p, vvi& cycles) {
	int n = sz(p);

	int m = 0;
	vb seen(n);

	rep(i, n) {
		// 抽出済のサイクルに含まれるなら次へ
		if (seen[i]) continue;

		// 新しいサイクルを発見
		cycles.push_back(vi());
		m++;

		// サイクルを順に格納していく．
		int s = i;
		do {
			cycles[m - 1].push_back(s);
			seen[s] = true;
			s = p[s];
		} while (s != i);
	}

	return m;
}



//【順列の数え上げ（大小関係指定）】O(n^2)
/*
* '<', '>', '?' からなる長さ n-1 の文字列 s で指定される
* 大小関係を満たすような長さ n の順列の個数を返す．
*
*（順列 DP）
*/
mint count_permutations(const string& s) {
	int n = sz(s) + 1;

	// dp[i][j] : 以下の条件を満たす長さ i + j の順列が何通りあるか．
	//	 i : 直前の桁より小さい数が何個使えるか
	//	 j : 直前の桁より大きい数が何個使えるか
	vvm dp(n, vm(n));
	rep(i, n) dp[i][n - 1 - i] = 1;

	// k = i + j
	repir(k, n - 2, 0) {
		// 直前より大きい数字を使う場合
		if (s[n - 2 - k] == '<') {
			// 右からの累積和を計算する
			dp[0][k] += dp[0][k + 1];
			repi(i, 1, k) {
				int j = k - i;
				dp[i][j] += dp[i][j + 1] + dp[i - 1][j + 1];
			}
		}
		// 直前より小さい数字を使う場合
		else if (s[n - 2 - k] == '>') {
			// 左からの累積和を計算する
			dp[k][0] += dp[k + 1][0];
			repi(j, 1, k) {
				int i = k - j;
				dp[i][j] += dp[i + 1][j] + dp[i + 1][j - 1];
			}
		}
		// 自由に数字を使える場合
		else {
			// 総和を計算する．
			mint sum = 0;
			repi(i, 0, k + 1) sum += dp[i][k + 1 - i];
			repi(i, 0, k) dp[i][k - i] = sum;
		}
	}

	return dp[0][0];
}


