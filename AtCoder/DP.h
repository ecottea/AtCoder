#pragma once
#include "header.h"
#include "関数.h"
#include "畳込み.h"
#include "数論変換.h"
#include "monotone.h"
#include "FPS(mint).h"
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


// 【畳込み遷移（mod 998244353）】O(n log n)
/*
* DP の初項と漸化式が，dp0, c[0..n) を用いて
*	dp[0] = dp0
*	dp[i+1] = Σj∈[0..i] c[i-j] dp[j] （i≧0）
* で与えられるときの dp[0..n] を返す．
*
* 利用：【形式的冪級数】
*/
vm convolution_DP_div(mint dp0, const vm& c) {
	//【方法】
	// dp[0..n], c[0..n) の母関数をそれぞれ
	//		f(z) = Σi∈[0..∞) dp[i] z^i
	//		g(z) = Σi∈[0..∞) c[i] z^i
	// とおく．
	//		(f(z) - dp[0]) / z
	//		= Σi∈[1..∞) dp[i] z^(i-1)
	//		= Σi∈[0..∞) dp[i+1] z^i
	// となることと，漸化式の右辺が畳込みの形であることに注意すると，関数方程式
	//		(f(z) - dp[0]) / z = f(z) g(z)
	// を得る．これを f(z) について解くと，
	//		f(z) - dp[0] = z f(z) g(z)
	//		(1 - z g(z)) f(z) = dp[0]
	//		f(z) = dp[0] / (1 - z g(z))
	// を得る．

	int n = sz(c);

	MFPS g(c);
	MFPS f = MFPS(dp0, n + 1) / (1 - (g >> 1));
	f.resize(n + 1);

	return f.c;
}


//【畳込み遷移（mod 998244353，1/n 倍）】O(n log n)
/*
* DP の初項と漸化式が，dp0, c[0..n) を用いて
*	dp[0] = dp0
*	dp[i+1] = 1/(i+1) Σj∈[0..i] c[i-j] dp[j] （i≧0）
* で与えられるときの dp[0..n] を返す．
*
* 制約：fm は n! まで計算可能
*
* 利用：【形式的冪級数】,【不定積分】,【指数関数】
*/
vm convolution_DP_exp(mint dp0, const vm& c, const Factorial_mint& fm) {
	//【方法】
	// dp[0..n], c[0..n) の母関数をそれぞれ
	//		f(z) = Σi∈[0..∞) dp[i] z^i
	//		g(z) = Σi∈[0..∞) c[i] z^i
	// とおく．
	//		f'(z)
	//		= Σi∈[1..∞) i dp[i] z^(i-1)
	//		= Σi∈[0..∞) (i+1) dp[i+1] z^i
	// となることと，漸化式の右辺が畳込みの形であることに注意すると，微分方程式
	//		f'(z) = f(z) g(z)
	// を得る．これを f(z) について解くと，
	//		df/dz = g(z) f
	//		∫ 1/f df = ∫ g(z) dz
	//		log f = ∫ g(z) dz
	//		f(z) = exp( ∫ g(z) dz )
	// を得る．

	int n = sz(c);

	MFPS g(c);
	MFPS f = exp_fps(integral_fps(g, fm), n + 1, fm);
	f.resize(n + 1);
	f *= dp0;

	return f.c;
}


//【畳込み遷移（mod 998244353，任意関数）】O(n (log n)^2)
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
* で与えられるときの dp[0..n] を求めるには【Monge コスト完全 DAG 最短路】が使える．
*/


//【max-plus anti-Monge 遷移】O(n log n)
/*
* DP の初項と漸化式が，dp0, (n+1) 次狭義上三角 anti-Monge 行列 c を用いて
*	dp[0] = dp0
*	dp[i] = MAX_j∈[0..i) (dp[j] + c(j,i)) （i≧1）
* で与えられるときの dp[0..n] を返す．
*
* 利用：【Monge コスト完全 DAG 最短路】
*/
template <class T, class FUNC>
vl max_plus_antimonde_DP(int n, T dp0, const FUNC& c) {
	auto c_neg = [&](int j, int i) { return -c(j, i); };
	auto dp = lowest_cost_path_monge_DAG<ll>(n, c_neg);
	repi(i, 0, n) dp[i] = -dp[i] + dp0;
	return dp;

	/* c の定義の雛形
	auto c = [&](int s, int t) {
		return t - s;
	};
	*/
}


