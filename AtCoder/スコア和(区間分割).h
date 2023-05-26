#pragma once
#include "header.h"
// ■■■■■ スコア和（区間分割） ■■■■■


//【積スコアの和 → 数え上げ（積の和典型）】
/*
* ある区間分割のスコアが，各区間のスコアの積で与えられるとする．
* 各区間のスコアがその区間についての何らかの数え上げ問題の答えとみなせるならば，
* 積スコアの和を求める問題を数え上げの問題に帰着でき，遷移を細かくできることが期待される．
*
* 区間長の d 乗 → 区間に区別のできる d 個の玉を配置する（重複可）
* verify : https://atcoder.jp/contests/agc013/tasks/agc013_e
*/


//【連の長さに応じたスコア】
/*
* 連の長さに応じたスコアが得られる場合，連の長さが 1 増加したときの
* スコアの増加量に読み替えることができれば，連の長さではなく
* 直前に選んだ要素のみを状態にもった DP ができる．
*
* verify : https://atcoder.jp/contests/arc010/tasks/arc010_3
*/


//【区間最大値の積の和】O(n)
/*
* 数列 a[0..n) に対して
*	a[l..r) のスコア：max a[l..r)
*	a[0..n) の区間分割のスコア：各区間スコアの積
* と定め，2^(n-1) 通り全ての区間分割をわたるスコアの総和を返す．
*
*（スタックで高速化した DP）
*/
mint max_product_sum(const vi& a) {
	int n = sz(a);

	//【方法】
	// dp[i] を a[0..i) に対する答えとする．
	// dp[i+1] は，i を右端とする区間の左端 j で場合分けすることにより，漸化式
	//		dp[i+1] = Σj=[0..i] dp[j] max(a[j..i])
	// を用いて求めることができる．
	//
	// j0 < i を a[j0] ≧ a[i] を満たす最大のもの（なければ -1）と定めると，
	//		max(a[j..i]) = max(a[j..i))	(j ≦ j0 のとき)
	//		max(a[j..i]) = a[i]			(j > j0 のとき)
	// となる．これを踏まえると，漸化式は
	//		dp[i+1] = dp[i] - Σj=(j0..i] dp[j] max(a[j..i)) + a[i] Σdp(j0..i]
	// と書き直せる．
	//
	// この更新を高速に行うため，max(a[j..i]) の値で j の値を分類し，
	// それぞれの j の範囲について dp の和を保持しておくスタックを用いる．

	// dp[i] : a[0..i) に対する答え
	vm dp(n + 1);
	dp[0] = 1;

	// st : max(a[j..i]) のユニークな値とその範囲の dp の和の組を j について昇順に保持する
	stack<pair<int, mint>> st;

	rep(i, n) {
		dp[i + 1] = (i == 0 ? 0 : dp[i]);

		// sum : max(a[j..i]) = a[i] となる j についての dp[j] の和
		mint sum = dp[i];

		// スタックトップの値が a[i] 以上になるまでスタックを掘っていく．
		while (!st.empty() && st.top().first < a[i]) {
			int v; mint s;
			tie(v, s) = st.top(); st.pop();

			dp[i + 1] -= v * s;
			sum += s;
		}
		st.push({ a[i], sum });

		dp[i + 1] += a[i] * sum;
	}

	return dp[n];
}


//【区間レンジの積の和】O(n)
/*
* 数列 a[0..n) に対して
*	a[l..r) のスコア：max a[l..r) - min a[l..r)
*	a[0..n) の区間分割のスコア：各区間スコアの積
* と定め，2^(n-1) 通り全ての区間分割をわたるスコアの総和を返す．
*
*（スタックで高速化した DP）
*/
mint range_product_sum(const vi& a) {
	// verify : https://atcoder.jp/contests/abc234/tasks/abc234_g

	//【方法】
	//【区間最大値の積の和】と同様．

	int n = sz(a);

	// dp[i] : a[0..i) に対する答え
	vm dp(n + 1);
	dp[0] = 1;

	stack<pair<int, mint>> st_max;
	stack<pair<int, mint>> st_min;

	mint res_max = 0, res_min = 0;

	rep(i, n) {
		mint sum = dp[i];
		while (!st_max.empty() && st_max.top().first < a[i]) {
			int v; mint s;
			tie(v, s) = st_max.top(); st_max.pop();

			res_max -= v * s;
			sum += s;
		}
		st_max.push({ a[i], sum });
		res_max += a[i] * sum;

		sum = dp[i];
		while (!st_min.empty() && st_min.top().first > a[i]) {
			int v; mint s;
			tie(v, s) = st_min.top(); st_min.pop();

			res_min -= v * s;
			sum += s;
		}
		st_min.push({ a[i], sum });
		res_min += a[i] * sum;

		dp[i + 1] = res_max - res_min;
	}

	return dp[n];
}


//【区間数値の積の和】O(n)
/*
* 数値文字列 s[0..n) に対して
*	s[l..r) のスコア：s[l..r) を B 進数とみなしたときの値
*	s[0..n) の区間分割のスコア：各区間スコアの積
* と定め，2^(n-1) 通り全ての区間分割をわたるスコアの総和を返す．
*/
mint numstr_product_sum(const string& s, int B = 10) {
	// verify : https://atcoder.jp/contests/abc288/tasks/abc288_f

	//【方法】
	// 例えば s[0..3) = "234" のスコア和は
	//		234 + 2 * 34 + 23 * 4 + 2 * 3 * 4
	// であるが，これを
	//		1 個の 234，2 個の 34，23 個の 4，2 * 3 個の 4
	// の和と解釈し，スコア和だけでなく個数の和も状態にもって DP を行う．

	int n = sz(s);

	// dp[i] : s[0..i) のスコア和，cnt[i] : s[0..i) の個数の和
	vector<mint> dp(n + 1), cnt(n + 1);
	cnt[0] = 1;

	rep(i, n) {
		// 分ける場合
		dp[i + 1] += dp[i] * (s[i] - '0');
		cnt[i + 1] += dp[i];

		// 繋ぐ場合
		dp[i + 1] += dp[i] * B + (s[i] - '0') * cnt[i];
		cnt[i + 1] += cnt[i];
	}

	return dp[n];
}


//【区間数値の OR の和】O(n^2)
/*
* 01 文字列 s[0..n) に対して
*	s[l..r) のスコア：s[l..r) を 2 進数とみなしたときの値
*	s[0..n) の区間分割のスコア：各区間スコアの OR
* と定め，2^(n-1) 通り全ての区間分割をわたるスコアの総和を返す．
*/
mint bitstr_OR_sum(const string& s) {
	// verify : https://yukicoder.me/problems/no/2279

	//【方法】
	// 主客転倒しビットごとに独立に寄与を考え，さらに余事象を考える．
	//		区間分割のスコアの第 k ビットが 0 になる
	//		⇔ 全ての区間スコアの第 k ビットが 0 になる
	// なので，そのような区間分割が何通りあるかを求められれば良い．
	// これには区間の右端を固定したとき左端をどこまで伸ばせるか調べ累積和で DP を高速化すれば良い．

	int n = sz(s);

	mint res = (mint(2).pow(n) - 1) * mint(2).pow(n - 1), pow2 = 1;

	rep(k, n) {
		// dp[i] : s[0..i) の，全ての区間スコアの第 k ビットが 0 になるような区間分割の個数
		vm dp(n + 1), acc(n + 2);
		dp[0] = 1; acc[1] = 1;

		repi(i, 1, n) {
			// l_min : 区間 s[l..i) のスコアの第 k ビットが 0 になるような l の最小値
			int l_min = (i - k - 1 >= 0 && s[i - k - 1] == '1' ? i - k : 0);

			dp[i] = acc[i] - acc[l_min];
			acc[i + 1] = acc[i] + dp[i];
		}

		res -= dp[n] * pow2;
		pow2 *= 2;
	}

	return res;
}


//【区間分割のスコア和（任意演算）】O(2^n n)
/*
* 列 a[0..n) に対して
*	a[l..r) のスコア：a[l..r) の総 op1
*	a[0..n) の区間分割のスコア：各区間スコアの総 op2
*	a のスコア：2^(n-1) 通りの区間分割のスコアの総 op3
* と定め，a のスコア返す．
*/
template <class T, T(*op1)(T, T), T(*op2)(T, T), T(*op3)(T, T)>
T interval_partitions_score(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/abc197/tasks/abc197_c

	int n = sz(a);

	T val3(0);

	repb(set, n - 1) {
		T val2; bool first_call = true;

		// val2 = op3(val3, x) とする（val2 が仮の単位元でも大丈夫）
		auto apply2 = [&](T x) {
			if (first_call) val2 = x, first_call = false;
			else val2 = op2(val2, x);
		};

		T val1 = a[0];

		rep(i, n - 1) {
			if (get(set, i)) {
				apply2(val1);
				val1 = a[i + 1];
			}
			else {
				val1 = op1(val1, a[i + 1]);
			}
		}
		apply2(val1);

		val3 = (set == 0 ? val2 : op3(val3, val2));
	}

	return val3;
}


