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
*	(判定問題) 列 a[0..n)（変域 O(K)）が条件 P を満たすか判定せよ
* が，O(1) 個のパラメータ（変域 O(m)）を保持しながらの線形走査で解けるならば，
* そのパラメータの値を状態にもつ愚直な DP を行うことで，
*	(数え上げ問題) 条件 P を満たす列の総数を求めよ
* を O(n m K) で解くことができる．
*
* verify : https://atcoder.jp/contests/abc237/tasks/abc237_f
*/


//【入力が連長圧縮された DP】
/*
* 入力が連長圧縮されて与えられる DP では，行列累乗や多項式累乗を使っての高速化が見込める．
* 特に多項式累乗では二項定理や多項定理が使える場合がある．
* 
* verify : https://yukicoder.me/problems/no/2788
*/


//【畳込み遷移（mod 998244353）】O(n log n)
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
template <class FUNC>
vm convolution_DP(mint dp0, const vm& c, const FUNC& f) {
	// verify : https://atcoder.jp/contests/abc318/tasks/abc318_h

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

	/* f の定義の雛形
	auto f = [&](int i, mint x) {
		return x;
	};
	*/
}


//【畳込み遷移（mod 998244353，任意関数，分割統治 FFT）】O(n (log n)^2)
/*
* DP の初項と漸化式が，dp0, c[0..n), {f_i} を用いて
*	dp[0] = dp0
*	dp[i+1] = f_i( Σj∈[0..i] c[i-j] dp[j] ) （i≧0）
* で与えられるときの dp[0..n] を返す．
*
*（分割統治 FFT）
*/
template <class FUNC>
vm convolution_DP_dc(mint dp0, const vm& c, const FUNC& f) {
	// verify : https://atcoder.jp/contests/abc318/tasks/abc318_h

	int n = sz(c);

	vm dp(n + 1);

	// dp[l..r) に正しい値を書き込む．ただし以下の条件が満たされていることを前提とする：
	//		dp[0..l) には正しい値が書き込まれている．
	//		dp[0..l) から dp[l..r) への寄与は既に dp[l..r) に加算済である．      
	function<void(int, int)> rf = [&](int l, int r) {
		// 単一要素のみになったら f_i を作用させて終了する．
		if (r - l == 1) {
			if (l == 0) dp[l] = dp0; // 初項だけは例外
			else dp[l] = f(l - 1, dp[l]);
			return;
		}

		// m : 中央位置
		int m = (l + r) / 2;

		// dp[l..m) を正しい値に設定する（前提条件は [l..r) のそれより弱い）
		rf(l, m);

		// dp[l..m) から dp[m..r) への寄与は
		//	dp[m]   += c[0] dp[m-1] + c[1] dp[m-2] + c[2] dp[m-3] + ... + c[m-l-1] dp[l]
		//	dp[m+1] +=                c[1] dp[m-1] + c[2] dp[m-2] + ... + c[m-l]   dp[l]
		//	dp[m+2] +=                               c[2] dp[m-1] + ... + c[m-l+1] dp[l]
		//	...
		//	dp[r-1] +=                                                    c[r-l-2] dp[l]
		// であるが，右辺は c[0..r-l-1) と dp[l..m) との畳込みで一括計算できる．
		vm c_sub(c.begin(), c.begin() + min(r - l - 1, n));
		vm dp_sub(dp.begin() + l, dp.begin() + m);
		vm tmp = convolution(c_sub, dp_sub);
		repi(i, m, r - 1) dp[i] += tmp[i - l - 1];

		// 右側を正しい値に設定する．
		rf(m, r);
	};
	rf(0, n + 1);

	return dp;

	/* f の定義の雛形
	auto f = [&](int i, mint x) {
		return x;
	};
	*/
}


//【畳込み遷移（mod 998244353，ランダムアクセス）】O(n√T log n)
/*
* DP の初項と漸化式が，dp0, c[0..n], x[0..T), y[0..T), {f_t} を用いて
*	dp[0] = dp0
*	dp[y[t]] += f_t( Σj∈[0..x[t]] c[x[t]-j] dp[j] ) （t∈[0..T)）
* で与えられるときの dp[0..n] を返す．
*
*（平方分割 FFT）
*/
template <class FUNC>
vm convolution_DP_ra(mint dp0, const vm& c, const vi& x, const vi& y, const FUNC& f) {
	// verify : https://atcoder.jp/contests/arc196/tasks/arc196_c

	//【備考】
	// c[0..n] を dp2[0..n] に取り替えることも可能．

	int n = sz(c) - 1;
	int T = sz(x);

	int sqrtT = (int)sqrt(T) * 7 + 1;

	vm dp(n + 1);
	dp[0] = dp0;

	vector<pim> lazy;

	vm ans;

	rep(t, T) {
		if (t % sqrtT == 0) {
			for (auto [i, v] : lazy) {
				dp[i] += v;
			}
			ans = convolution(dp, c);
			lazy.clear();
		}

		mint val = ans[x[t]];
		for (auto [i, v] : lazy) {
			if (x[t] - i >= 0) val += v * c[x[t] - i];
		}
		val = f(t, val);

		lazy.emplace_back(y[t], val);
	}
	for (auto [i, v] : lazy) {
		dp[i] += v;
	}

	return dp;

	/* f の定義の雛形
	auto f = [&](int t, mint x) {
		return x;
	};
	*/
}


//【累積畳込み遷移（mod 998244353）】O((n + N) K log n log N)
/*
* DP の初項と漸化式が，a[0..n], K 次未満の多項式族 {f_i} を用いて
*	dp[i] = [z^n] a(z) Πj∈[0..i) f_j(z)  (i≧0)
* で与えられるときの dp[0..N] を返す．
* 
* 制約 : f_i(z) は dp[0..i] しか参照しない．
* 
*（分割統治 FFT）
*/
template <class FUNC>
vm acc_convolution_DP(vm a, int K, const FUNC& f, int N) {
	// verify : https://atcoder.jp/contests/abc345/tasks/abc345_g

	//【方法】
	// dp[l..r) の計算に必要になるのは
	//		[z^[n-(r-l-1)(K-1)..n]] a(z) Πi∈[0..l) f_i(z)
	// のみである．
	//		z^[n-(r-l-1)(K-1)..n] → [0..n]
	//		a(z) Πi∈[0..l) f_i(z) → a(z)
	// と置き換えれば，これは元の問題と同じ構造で規模の小さい問題であることが分かる．
	// 従って分割統治法を用いて再帰的に計算していくことができる．

	int n = sz(a) - 1;

	vm dp(N + 1);

	// Πi∈[l..r) f_i(z) を返す．
	// 計算には，g : [z^[n-(r-l-1)(K-1)..n]] a(z) Πi∈[0..l) f_i(z) を利用する．
	function<vm(int, int, vm)> rf = [&](int l, int r, vm g) {
		if (r - l == 1) {
			// 返すべきものは
			//		f_l(z)
			// である．引数に与えられた g は
			//		[z^n] a(z) Πi∈[0..l) f_i(z)
			// であり，漸化式よりこれが dp[l] に等しい．
			dp[l] = g.back();
			return f(l, dp);
		}

		int m = (l + r) / 2;

		// f_lm = Πi∈[l..m) f_i(z)
		int w = min((m - l - 1) * (K - 1), n);
		vm f_lm = rf(l, m, vm(g.begin() + (sz(g) - w - 1), g.end()));

		// g_mr : [z^[n-(r-m-1)(K-1)..n]] a(z) Πi∈[0..m) f_i(z)
		vm g_mr = convolution(g, f_lm); // 必要な部分以外は壊れるが，もう用はない
		w = min((r - m - 1) * (K - 1), n);
		g_mr = vm(g_mr.begin() + (sz(g) - w - 1), g_mr.begin() + sz(g)); // [z^[n-(r-m-1)(K-1)..n]] だけ切り取る

		// f_mr = Πi∈[m..r) f_i(z)
		vm f_mr = rf(m, r, g_mr);

		// f_lr = Πi∈[l..r) f_i(z)
		vm f_lr = convolution(f_lm, f_mr);
		if (sz(f_lr) > n + 1) f_lr.resize(n + 1);

		return f_lr;
	};
	rf(0, N + 1, a); // 本当はちゃんと長さを調節するべき

	return dp;

	/* f の定義の雛形
	int K = 2;
	auto f = [&](int i, const vm& dp) {
		vm poly(K);
		return poly;
	};
	*/
}


//【累積畳込み遷移（mod 998244353，[z^i]）】O(n (log n)^2)
/*
* DP の初項と漸化式が，f[0..n) を用いて
*	dp[i] = [z^i] f(z) Πj∈[0..i) (1 + dp[j] z)  (i≧0)
* で与えられるときの dp[0..n) を返す．
*
*（分割統治 FFT）
*/
vm acc_convolution_DP(const vm& f) {
	// verify : https://atcoder.jp/contests/abc281/tasks/abc281_h

	//【方法】
	// dp[l..r) の計算に必要になるのは
	//		[z^[l..r)] f(z) Πi∈[0..l) (1 + dp[i] z)
	// のみである．
	//		[l..r) → [0..n)
	//		f(z) Πi∈[0..l) (1 + dp[i] z) → f(z)
	// と置き換えれば，これは元の問題と同じ構造で規模の小さい問題であることが分かる．
	// 従って分割統治法を用いて再帰的に計算していくことができる．

	int n = sz(f);

	vm dp(n);

	// Πi∈[l..r) (1 + dp[i] z) を返す．
	// 計算には，g : [z^[l..r)] f(z) Πi∈[0..l) (1 + dp[i] z) を利用する．
	function<vm(int, int, vm)> rf = [&](int l, int r, vm g) {
		if (r - l == 1) {
			// 返すべきものは
			//		1 + dp[l] z
			// である．引数に与えられた g は
			//		[z^l] f(z) Πi∈[0..l) (1 + dp[i] z)
			// であり，漸化式よりこれが dp[l] に等しい．
			dp[l] = g[0];
			return vm{ 1, g[0] };
		}

		int m = (l + r) / 2;

		// f_lm = Πi∈[l..m) (1 + dp[i] z)
		vm f_lm = rf(l, m, vm(g.begin(), g.begin() + (m - l)));

		// g_mr : [z^[m..r)] f(z) Πi∈[0..m) (1 + dp[i] z)
		vm g_mr = convolution(g, f_lm); // [z^[l..m)] は壊れるが，もう用はない
		g_mr = vm(g_mr.begin() + (m - l), g_mr.end()); // [z^[m..r)] だけ切り取る

		// f_mr = Πi∈[m..r) (1 + dp[i] z)
		vm f_mr = rf(m, r, g_mr);

		// f_lr = Πi∈[l..r) (1 + dp[i] z)
		vm f_lr = convolution(f_lm, f_mr);

		return f_lr;
	};
	rf(0, n, f);

	return dp;
}


//【一次式の min 遷移】O(n log n)
/*
* DP の初項と漸化式が，dp0, a, b, x, {f_i} を用いて
*	dp[0] = dp0
*	dp[i] = f_i( MIN_j∈[0..i) (a(dp; j) x(dp; i) + b(dp; j)) ) （i≧1）
* で与えられるときの dp[0..n] を返す．
* max_flag = true とすると代わりに max 遷移で計算する．
*
* 利用：【Convex-Hull Trick】
*/
vl linear_min_DP(int n, ll dp0, const function<ll(int j, const vl& dp)>& a,
	const function<ll(int j, const vl& dp)>& b, const function<ll(int i, const vl& dp)>& x,
	const function<ll(int i, ll val)>& f, bool max_flag = false)
{
	// verify : https://atcoder.jp/contests/dp/tasks/dp_z

	vl dp(n + 1);
	dp[0] = dp0;

	Convex_hull_trick<ll> CHT(max_flag);

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
*	dp[i] = f_i( MIN_j∈[0..i) (g(x(dp; i) - p(dp; j)) + q(dp; j)) ) （i≧1）
* で与えられるときの dp[0..n] を求めるには【Li Chao Tree（1 交差関数群）】が使える．
*/


//【min-plus Monge 遷移】O(n log n)
/*
* DP の初項と漸化式が，dp0, (n+1) 次狭義上三角 Monge 行列 c を用いて
*	dp[0] = dp0
*	dp[i] = MIN_j∈[0..i) (dp[j] + c(j,i)) （i≧1）
* で与えられるときの dp[0..n] を求めるには【Monge DAG 最短路】が使える．
*/


//【max-plus anti-Monge 遷移】O(n log n)
/*
* DP の初項と漸化式が，dp0, (n+1) 次狭義上三角 anti-Monge 行列 c を用いて
*	dp[0] = dp0
*	dp[i] = MAX_j∈[0..i) (dp[j] + c(j,i)) （i≧1）
* で与えられるときの dp[0..n] を返す．
*
* 利用：【Monge DAG 最短路】
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


