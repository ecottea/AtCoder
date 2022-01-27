#pragma once
#include "header.h"
#include "座標圧縮.h"
// ■■■■■ 部分列 ■■■■■


//【部分列判定】O(n)
/*
* seq[0..n) が部分列として sub[0..m) を含むかを返す．
*/
template <class T> bool subsequenceQ(const vector<T>& seq, const vector<T>& sub) {
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
* 列 a[0..n) の最長増加部分列の長さを返す．またその一例を lis に構成する．
* 
* なお戻り値は「広義減少部分列への分割の最小個数」とも解釈できる．
*
*（セグメント木で高速化したインライン DP）
*
* 利用：【座標圧縮】
*/
int op_lis(int a, int b) { return max(a, b); }
int e_lis() { return 0; }
template <class T>
int longest_increasing_subsequence(const vector<T>& a, vector<T>* lis = nullptr) {
	int n = sz(a);

	// a を座標圧縮した結果を b に格納する．
	vi b;
	vector<T> x;
	int m = coordinate_compression(a, b, &x);

	// dp : 区間最大値を計算できるセグメント木
	// dp[j] : 今まで見てきた中での，右端の値が j であるような最長増加部分列の長さ
	segtree<int, op_lis, e_lis> dp(m);

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
	}

	return len;
}


//【コスト最大増加部分列】O(n log n)
/*
* 非負コスト c[0..n) が与えられた列 a[0..n) のコスト最大増加部分列のコストを返す．
*
*（セグメント木で高速化したインライン DP）
*
* 利用：【座標圧縮】
*/
ll op_mis(ll a, ll b) { return max(a, b); }
ll e_mis() { return 0; }
template <class T>
ll maxcost_increasing_subsequence(const vector<T>& a, const vl& c) {
	int n = sz(a);

	// a を座標圧縮した結果を b に格納する．
	vi b; vector<T> x;
	int m = coordinate_compression(a, b, &x);

	// dp[j] : 今まで見てきた中での，右端の値が j であるような増加部分列の最大コスト
	segtree<ll, op_mis, e_mis> dp(m);

	// j = b[i] を順に見ていく
	rep(i, n) {
		int j = b[i];

		// j を右端にもてるのは，それまでの右端が j 未満のもののみ．
		// よってその中での増加部分列の最大コストを求め，それに c[i] を加える．
		ll cost = dp.prod(0, j) + c[i];

		// j を右端とするよりコストの大きいものが作れれば更新する．
		// dp[j] 以外は更新されることはないので，更新は O(log n) で終わる．
		// この性質が dp テーブルのインライン化と相性が良い．
		if (cost > dp.get(j)) {
			dp.set(j, cost);
		}
	}

	// 右端の値を任意としたときの増加部分列の最大コストを得る．
	ll cost = dp.prod(0, m);

	return cost;
}


//【最長共通部分列】O(|s| |t|)
/*
* 2 つの文字列 s, t の最長共通部分列の長さを返す．またその一例を lcs に格納する．
*
*（二次元 DP）
*/
template <class T>
int longest_common_subsequence(const vector<T>& s, const vector<T>& t, vector<T>* lcs = nullptr) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_f
	
	// 文字列の長さ
	int n = sz(s);
	int m = sz(t);

	// dp[i][j] : s[0..i) と t[0..j) の最長共通部分列の長さ
	vvi dp(n + 1, vi(m + 1));
	rep(i, n) {
		rep(j, m) {
			// s の i 文字目と t の j 文字目が等しい場合
			if (s[i] == t[j]) {
				// その文字は採用し，1 つ短い文字列に帰着する．
				dp[i + 1][j + 1] = dp[i][j] + 1;
			}
			// s の i 文字目と t の j 文字目が異なる場合
			else {
				// どちらかを 1 文字削った文字列に帰着する．
				dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
			}
		}
	}

	// DP 復元を行い最長共通部分列 lcs を求める．
	// verify : https://atcoder.jp/contests/dp/tasks/dp_f
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
				if (dp[i][j + 1] > dp[i + 1][j]) {
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


//【レーベンシュタイン距離】O(|s| |t|)
/*
* 文字列 s から文字列 t へのレーベンシュタイン距離を返す．
* また必要なら削除や挿入の位置を '-' で表した文字列を s2, t2 に格納する．
*
* dist = {fit, ins, del, sub} :
*	順に，一致，挿入，削除，置換 1 回あたりの距離
*
*（二次元 DP）
*/
template <class T>
ll levenshtein_distance(const vector<T>& s, const vector<T>& t,
	const tuple<ll, ll, ll, ll>& dist = { 0, 1, 1, 1 },
	vector<T>* s2 = nullptr, vector<T>* t2 = nullptr)
{
	// verify : https://atcoder.jp/contests/genocon2021/tasks/genocon2021_b

	int n = sz(s);
	int m = sz(t);

	ll fit, ins, del, sub;
	tie(fit, ins, del, sub) = dist;

	// dp[i][j] : s[0..i) から t[0..j) への距離
	vvl dp(n + 1, vl(m + 1, INF));
	dp[0][0] = 0;

	// 貰う DP
	repi(i, 0, n) {
		repi(j, 0, m) {
			// 以下の 3 通りのうちの最大のものを選ぶ：
			//   s[i - 1] と t[j - 1] を対にする．
			//   s[i - 1] を削除する．
			//   t[j - 1] を挿入する．
			if (i > 0 && j > 0) {
				chmin(dp[i][j], dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? fit : sub));
			}
			if (i > 0) {
				chmin(dp[i][j], dp[i - 1][j] + del);
			}
			if (j > 0) {
				chmin(dp[i][j], dp[i][j - 1] + ins);
			}
		}
	}

	// DP 復元を行う
	if (s2 != nullptr) {
		s2->clear();
		t2->clear();

		int i = n, j = m;
		while (i > 0 || j > 0) {
			if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? fit : sub)) {
				s2->push_back(s[--i]);
				t2->push_back(t[--j]);
			}
			else if (i > 0 && dp[i][j] == dp[i - 1][j] + del) {
				s2->push_back(s[--i]);
				t2->push_back('-');
			}
			else {
				s2->push_back('-');
				t2->push_back(t[--j]);
			}
		}
		reverse(all(*s2));
		reverse(all(*t2));
	}

	return dp[n][m];
}


