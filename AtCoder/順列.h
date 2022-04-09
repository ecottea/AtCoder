#pragma once
#include "header.h"
#include "座標圧縮.h"
#include "二項係数.h"
// ■■■■■ 順列，対称群 ■■■■■


//【転倒数】O(n log n)
/*
* a[0..n) の転倒数を返す．
*/
template <class T> ll inversion_number(vector<T>& a) {
	// verify : https://atcoder.jp/contests/arc075/tasks/arc075_c

	int n = sz(a);

	// 値 a[i] と添字 i を組にしソートする．
	vector<pair<T, int>> ai(n);
	rep(i, n) {
		ai[i] = { a[i], i };
	}
	sort(all(ai));

	ll res = 0;

	// ft[i] : いままでに添字 i の要素が現れたか
	fenwick_tree<int> ft(n);

	// 値について昇順に見ていく．
	rep(j, n) {
		// id : 昇順で j 番目の値の添字
		int id = ai[j].second;

		// id より大きい添字をもつ数が今までに何個あったかを加算する．
		res += ft.sum(id + 1, n);

		// 添字 id の出現を記録する．
		ft.add(id, 1);
	}

	return res;
}


//【転倒数】O(n log n)
/*
* a[0..n) の転倒数を返す．
*
* 利用：【座標圧縮】
*/
template <class T> ll inversion_number_cc(vector<T>& a) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_5_D

	int n = sz(a);

	// a を [0, |a|) に座標圧縮した結果を b に格納する．
	vi b;
	int m = coordinate_compression(a, b);

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
void permutation_decomposition(const vi& p, vvi& cycles) {
	// verify : https://atcoder.jp/contests/abc175/tasks/abc175_d

	int n = sz(p);

	vb seen(n);

	rep(i, n) {
		// 抽出済のサイクルに含まれるなら次へ
		if (seen[i]) continue;

		// 新しいサイクルを発見
		cycles.push_back(vi());

		// サイクルを順に格納していく．
		int s = i;
		do {
			cycles.rbegin()->push_back(s);
			seen[s] = true;
			s = p[s];
		} while (s != i);
	}
}


//【順列の数え上げ（隣接大小関係指定）】O(n^2)
/*
* '<', '>', '?' からなる長さ n-1 の文字列 s で指定される
* 大小関係を満たすような長さ n の順列の個数を返す．
*
*（挿入 DP）
*/
mint count_permutations(const string& s) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_t

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


//【順列全探索 → bitDP】O(2^n n)
/*
* a[0..n) に対して，1 回の操作でコスト x で a[i]++, a[i]--，コスト y で swap(a[i], a[i+1])
* が行えるとき，a[0..n) を b[0..n) に一致させるための最小コストを返す．
*/
ll minimize_inc_dec_swap_cost(const vl& a, const vl& b, ll x, ll y) {
	// verify : https://atcoder.jp/contests/abc232/tasks/abc232_f

	int n = sz(a);

	// a のどの要素を b のどの要素に対応させるかで決め打ち順列全探索を行えば，
	// コストが (要素の差の和) x + (順列の転倒数) y であることは容易に分かる．
	// これで間に合わない場合でも，b に対応させていく順番を固定することで
	// bitDP を用いて高速化できる．

	// dp[set] : a[set] を b[0..|set|) に一致させるための最小コスト
	vl dp(1LL << n, INFL);
	dp[0] = 0;

	repb(set, n) {
		if (set == 0) continue;

		// set_size : 添字集合 set の大きさ
		int set_size = popcount(set);

		ll swap_cost = (set_size - 1) * y;

		// 添字集合 set の要素 i を昇順に走査する．
		rep(i, n) {
			if (set & (1 << i)) {
				int n_set = set - (1 << i);
				ll inc_dec_cost = abs(a[i] - b[set_size - 1]) * x;

				chmin(dp[set], dp[n_set] + inc_dec_cost + swap_cost);

				swap_cost -= y;
			}
		}
	}

	return dp[(1 << n) - 1];
}


