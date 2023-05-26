#pragma once
#include "header.h"
#include "関数.h"
#include "畳込み.h"
#include "数論変換.h"
// ■■■■■ 典型的な DP ■■■■■


//【判定問題 → 数え上げ問題】
/*
*	(判定問題) 列 seq[0..n)（変域 O(k)）が条件 P を満たすか判定せよ
* が，O(1) 個のパラメータ（変域 O(m)）を保持しながらの線形走査で解けるならば，
* そのパラメータの値を状態にもつ愚直な DP を行うことで，
*	(数え上げ問題) 条件 P を満たす列の総数を求めよ
* を O(n m k) で解くことができる．
*
* verify : https://atcoder.jp/contests/abc237/tasks/abc237_f
*/


//【畳込み遷移（mod 998244353）】O(n (log n)^2)
/*
* DP の初項と漸化式が，dp0, c[0..n), {f_i} を用いて
*	dp[0] = dp0
*	dp[i+1] = f_i( Σj∈[0..i] c[i-j] dp[j] ) （i≧0）
* で与えられるときの dp[0..n] を返す．
*
* 利用：【オンライン畳込み（片側固定，mod 998244353）】
*/
vm convolution_DP(mint dp0, const vm& c, const function<mint(int i, mint val)>& f) {
	int n = sz(c);
	if (n == 0) return vm{ dp0 };

	Semi_online_convolution SOC(c);

	vm dp(n + 1);
	dp[0] = dp0;

	rep(i, n) {
		SOC.set(dp[i]);
		dp[i + 1] = f(i, SOC[i]);
	}

	return dp;
}


//【一次式の min 遷移】O(n log n)
/*
* DP の初項と漸化式が，dp0, a, b, x, {f_i} を用いて
*	dp[0] = dp0
*	dp[i] = f_i( MIN_j∈[0..i) (a(j) x(i) + b(j)) ) （i≧1）
* で与えられるときの dp[0..n] を返す．
* min_flag = false とすると代わりに max 遷移で計算する．
*
* 利用：【Convex-Hull Trick】
*/
vl linear_min_DP(int n, ll dp0, const function<ll(int j, const vl& dp)>& a,
	const function<ll(int j, const vl& dp)>& b, const function<ll(int i, const vl& dp)>& x,
	const function<ll(int i, ll val)>& f, bool min_flag = true)
{
	// verify : https://atcoder.jp/contests/dp/tasks/dp_z

	vl dp(n + 1);
	dp[0] = dp0;

	Convex_hull_trick<ll> CHT;

	repi(i, 1, n) {
		CHT.insert(a(i - 1, dp), b(i - 1, dp));
		ll val = CHT.get(x(i, dp));
		dp[i] = f(i, val);
	}

	return dp;
}


//【凸関数の平行移動族の min 遷移】O(n log n)
/*
* DP の初項と漸化式が，dp0, 凸関数 g, p, q, x, {f_i} を用いて
*	dp[0] = dp0
*	dp[i] = f_i( MIN_j∈[0..i) (g(x(i) - p(j)) + q(j)) ) （i≧1）
* で与えられるときの dp[0..n] を求めるには【Li Chao Tree（1 交差関数群）】が使える．
*/


//【min-plus Monge 遷移】O(n log n)
/*
* DP の初項と漸化式が，dp0, (n+1) 次狭義上三角 Monge 行列 c を用いて
*	dp[0] = dp0
*	dp[i] = MIN_j∈[0..i) (dp[j] + c(j,i)) （i≧1）
* で与えられるときの dp[0..n] を返す．
*/
vl min_plus_monde_DP(int n, ll dp0, const function<ll(int i, int j)>& c) {
	// 参考 : https://noshi91.hatenablog.com/entry/2023/02/18/005856
	// verify : https://yukicoder.me/problems/no/705

	//【方法】
	// (n+1)×(n+1) 行列 M を
	//		M[i][j] = dp[j] + c(j,i)
	// と定めると，M は狭義下三角な Monge 行列となるので，M の行最小値をオンラインで
	// （第 i 行の行最小値を求めてから第 i 列にアクセス）求められれば良い．

	// dp[i] : 第 i 行の最小値，j_min[j] : その位置
	vl dp(n + 1, INFL); vi j_min(n + 1, 0);
	dp[0] = dp0;

	// M[i][j] を用いて第 i 行の（暫定）最小値 dp[i] を更新する．
	auto update = [&](int i, int j) {
		if (chmin(dp[i], dp[j] + c(j, i))) j_min[i] = j;
	};

	// dp(iL..iR] を計算する．
	// 呼び出す際は dp(0..iL] と M[iR][0..iL] の（暫定）行最小値が計算済みであること．
	function<void(int, int)> solve = [&](int iL, int iR) {
		if (iR - iL <= 1) return;
		int iM = (iL + iR) / 2;

		// 計算済：dp(0..iL], min M[iR][0..iL]
		repi(j, j_min[iL], j_min[iR]) update(iM, j); // まだ j_min[iR]≦iL なので大丈夫

		// 計算済：dp(0..iL], min M[iM][0..iR], min M[iR][0..iL]
		solve(iL, iM);

		// 計算済：dp(0..iM], min M[iR][0..iL]
		repi(j, iL + 1, iM) update(iR, j);

		// 計算済：dp(0..iM], min M[iR][0..iM]
		solve(iM, iR);
	};
	update(n, 0);
	solve(0, n);

	return dp;
}


//【max-plus anti-Monge 遷移】O(n log n)
/*
* DP の初項と漸化式が，dp0, (n+1) 次狭義上三角 anti-Monge 行列 c を用いて
*	dp[0] = dp0
*	dp[i] = MAX_j∈[0..i) (dp[j] + c(j,i)) （i≧1）
* で与えられるときの dp[0..n] を返す．
*
* 利用：【min-plus Monge 遷移】
*/
vl max_plus_antimonde_DP(int n, ll dp0, const function<ll(int j, int i)>& c) {
	auto c_neg = [&](int j, int i) { return -c(j, i); };
	auto dp = min_plus_monde_DP(n, -dp0, c_neg);
	repi(i, 0, n) dp[i] *= -1;
	return dp;
}


