#pragma once
#include "header.h"
#include "座標圧縮.h"
// ■■■■■ 部分列 ■■■■■



//【部分列判定】O(n)
/*
* 長さ n の列 seq が部分列として sub を含むかを返す．
*/
template <class T>
bool subsequenceQ(const vector<T>& seq, const vector<T>& sub) {
	int n = sz(seq);
	int m = sz(sub);

	int j = 0;
	rep(i, n) {
		if (seq[i] == sub[j]) {
			j++;
		}
		if (j == m) {
			return true;
		}
	}
	return false;
}


//【最長増加部分列】O(n log n)
/*
* 長さ n の配列 a の最長増加部分列の長さを返す．
* また最長増加部分列の一例を lis に構成する．
*
*（セグメント木で高速化したインライン DP）
*
* 利用：【座標圧縮】
*/
int op3(int a, int b) { return max(a, b); }
int e3() { return 0; }
template <class T>
int longest_increasing_subsequence(const vector<T>& a, vector<T>* lis = nullptr) {
	int n = sz(a);

	// a を座標圧縮した結果を b に格納する．
	vi b;
	vector<T> x;
	int m = coordinate_compression(a, b, x);

	// dp : 区間最大値を計算できるセグメント木
	// dp[j] : 今まで見てきた中での，右端の値が j であるような最長増加部分列の長さ
	segtree<int, op3, e3> dp(m);

	// j = b[i] を順に見ていく
	rep(i, n) {
		int j = b[i];

		// j を右端にもてるのは，それまでの右端が j 未満のもののみ．
		// よってその中での最長増加部分列の長さを求め，それに 1 を加える．
		int len = dp.prod(0, j) + 1;

		// j を右端とするより長いものが作れれば更新する．
		// dp[j] 以外は更新されることはないので，更新は O(log n) で終わる．
		// この性質が dp テーブルのインライン化と相性が良い．
		if (len > dp.get(j)) {
			dp.set(j, len);
		}
	}

	// 右端の値を任意としたときの最長増加部分列の長さを得る．
	int len = dp.prod(0, m);

	// DP 復元を行う．
	if (lis != nullptr) {
		*lis = vector<T>(len);
		int i = len;
		repir(j, m - 1, 0) {
			if (dp.get(j) == i) {
				(*lis)[i - 1] = x[j];
				i--;
			}
		}
		dump(*lis);
	}

	return len;
}


//【最長共通部分列】O(|s| |t|)
/*
* 2 つの文字列 s, t の最長共通部分列の長さを返す．
* またその一例を lcs に格納する．
*
*（二次元 DP）
*/
template <class T>
int longest_common_subsequence(const vector<T>& s, const vector<T>& t, vector<T>* lcs = nullptr) {
	// 文字列の長さ
	int n = sz(s);
	int m = sz(t);

	// dp[i][j] : s[0..i) と t[0..j) の最長共通部分列の長さ
	vvi dp(n + 1LL, vi(m + 1LL));
	rep(i, n) {
		rep(j, m) {
			// s の i 文字目と t の j 文字目が等しい場合
			if (s[i] == t[j]) {
				// その文字は採用し，1 つ短い文字列に帰着する．
				dp[i + 1LL][j + 1LL] = dp[i][j] + 1;
			}
			// s の i 文字目と t の j 文字目が異なる場合
			else {
				// どちらかを 1 文字削った文字列に帰着する．
				dp[i + 1LL][j + 1LL] = max(dp[i][j + 1LL], dp[i + 1LL][j]);
			}
		}
	}

	// DP 復元を行い最長共通部分列 lcs を求める．
	if (lcs != nullptr) {
		*lcs = vector<T>(dp[n][m]);
		int i = n - 1, j = m - 1, pt = dp[n][m] - 1;
		while (i >= 0 && j >= 0) {
			// s の i 文字目と t の j 文字目が等しい場合
			if (s[i] == t[j]) {
				// その文字は採用し，1 つ短い文字列に帰着する．
				(*lcs)[pt--] = s[i];
				i--;
				j--;
			}
			// s の i 文字目と t の j 文字目が異なる場合
			else {
				// どちらを 1 文字削る方が長い文字列が得られるかを調べて短い文字列に帰着する．
				if (dp[i][j + 1LL] > dp[i + 1LL][j]) {
					i--;
				}
				else {
					j--;
				}
			}
		}
	}

	return dp[n][m];
}


