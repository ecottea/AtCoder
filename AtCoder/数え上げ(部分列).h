#pragma once
#include "header.h"
// ■■■■■ 数え上げ（部分列） ■■■■■


//【部分列の数え上げ】O(n)
/*
* 列 a[0..n) の部分列の個数を返す（空列も含む）
*
*（総和で高速化したインライン部分列 DP）
*/
template <class T>
mint count_subsequences(const vector<T>& a) {
	// verify : https://judge.yosupo.jp/problem/number_of_subsequences

	int n = sz(a);

	// b[i] : a[i] が a[0..n) の何種類目の要素か
	vi b(n);
	unordered_map<T, int> ids; int id = 0;

	// a[0..n) の要素そのものには興味が無いので，適当に圧縮して b[0..n) にしてしまう．
	rep(i, n) {
		auto it = ids.find(a[i]);
		if (it != ids.end()) b[i] = it->second;
		else {
			ids[a[i]] = id;
			b[i] = id++;
		}
	}

	// dp_i[j] : b[0..i) の空でない部分列で，最後が j であるものの個数．
	//	ただし同じ部分列については選択する位置の組が辞書順最大になるもののみを認める．
	//	この制約を設けることにより同じ部分列を重複して数えてしまわないようにする．
	vm dp(id);

	// sum_i : b[0..i) の空でない部分列の個数
	mint sum = 0;

	rep(i, n) {
		int j = b[i];
		mint old = dp[j];

		// b[i] = j を選ぶと，最後が j である列が今までの列の個数 + 1 になる．
		// 今までの列で最後が j であるものについて b[i] = j を選ばないことは禁止されている．
		dp[j] = sum + 1;

		// 総和の差分更新
		sum = sum - old + dp[j];
	}

	// 空列の分を加算
	return sum + 1;
}


//【指定部分列の数え上げ】O(n m)
/*
* s[0..n) の部分列のうち t[0..m) に一致するものの個数を返す．
* ただし部分列として同じでも，添字列として異なるものは区別する．
*
*（耳 DP）
*/
template <class T>
mint count_same_subsequences(const vector<T>& s, const vector<T>& t) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_h

	int n = sz(s), m = sz(t);

	// dp[i][j] : s[0..i) が部分列として t[0..j) を何個含むか
	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1;

	// 配る DP
	rep(i, n) {
		repi(j, 0, m) {
			// s[i] を採用しない場合
			dp[i + 1][j] += dp[i][j];

			// s[i] を t[j] に一致する文字として採用する場合
			if (j < m && s[i] == t[j]) {
				dp[i + 1][j + 1] += dp[i][j];
			}
		}
	}

	return dp[n][m];
}


//【共通部分列の数え上げ】O(n m)
/*
* s[0..n) の部分列と t[0..m) の部分列の組のうち両者が一致するものの個数を返す．
* ただし部分列として同じでも，添字列として異なるものは区別する．
*/
template <class T>
mint count_common_subsequences(const vector<T>& s, const vector<T>& t) {
	// verify : https://atcoder.jp/contests/abc130/tasks/abc130_e

	int n = sz(s), m = sz(t);

	// dp[i][j] : s[0..i) の部分列と t[0..j) の部分列の組のうち両者が一致するものの個数
	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1;

	// 配る DP
	repi(i, 0, n) {
		repi(j, 0, m) {
			// s[i] を採用しない場合
			if (i < n) {
				dp[i + 1][j] += dp[i][j];
			}

			// t[j] を採用しない場合
			if (j < m) {
				dp[i][j + 1] += dp[i][j];
			}

			// s[i], t[j] を共に採用しない場合を足しすぎたので引いておく
			if (i < n && j < m) {
				dp[i + 1][j + 1] -= dp[i][j];
			}

			// s[i] と t[j] を共通部分列の文字として採用する場合
			if (i < n && j < m && s[i] == t[j]) {
				dp[i + 1][j + 1] += dp[i][j];
			}
		}
	}

	return dp[n][m];
}


