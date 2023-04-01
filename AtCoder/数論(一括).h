#pragma once
#include "header.h"
#include "数論.h"
#include "整除,GCD,LCM.h"
#include "積.h"
// ■■■■■ 一括で求めるための数論アルゴリズム ■■■■■


//【素数の列挙】O(n log(log n))
/*
* n 以下の素数を昇順に列挙したリストを返す．
*/
vi eratosthenes(int n) {
	// verify : https://judge.yosupo.jp/problem/enumerate_primes

	vi ps;

	// is_prime[i] : i が素数か
	vb is_prime(n + 1, true);
	is_prime[0] = is_prime[1] = false;

	int i = 2;

	// √n 以下の i の処理
	for (; i <= n / i; i++) {
		if (is_prime[i]) {
			ps.push_back(i);

			for (int j = i * i; j <= n; j += i) is_prime[j] = false;
		}
	}

	// √n より大きい i の処理
	for (; i <= n; i++) if (is_prime[i]) ps.push_back(i);

	return ps;
}


//【素数の列挙（区間）】O((√r + (r - l))log(log r))
/*
* [l..r) に含まれる素数を昇順に格納したリストを返す．
*
* 利用：【素数の列挙】
*/
vl eratosthenes_interval(ll l, ll r) {
	// verify : https://algo-method.com/tasks/332

	vl ps;
	vi ps_sub = eratosthenes(int(sqrt(r) + 0.01));

	// 素数かどうかを記録しておくためのテーブル
	vb is_prime(r - l, true);
	if (l == 1) is_prime[0] = false; // 1 は素数ではない
	repe(p, ps_sub) {
		for (ll j = (l + p - 1) / p * p; j < r; j += p) {
			if (j != p) is_prime[j - l] = false;
		}
	}

	rep(i, r - l) if (is_prime[i]) ps.push_back(l + i);

	return ps;
}


//【素因数分解（複数）】
/*
* Factor_integer(int n) : O(n log(log n))
*	n 以下の自然数を高速に素因数分解する準備を行う．
*
* get(int i, map<int, int>& pps) : O(log n)
*	i の素因数分解結果を pps に格納する．
*/
struct Factor_integer {
	int n;

	// d[i] : i を割り切る最小の素数
	vi d;

	// n 以下の自然数を高速に素因数分解する準備を行う．
	Factor_integer(int n_) : n(n_), d(n + 1) {
		// verify : https://yukicoder.me/problems/no/2207

		iota(all(d), 0);

		for (int p = 2; p * p <= n; p++) {
			if (d[p] != p) continue;
			for (int i = p; i <= n; i += p) d[i] = p;
		}
	}

	// i の素因数分解結果を返す．
	map<int, int> get(int i) {
		// verify : https://yukicoder.me/problems/no/2207

		Assert(i <= n);

		map<int, int> pps;
		while (i > 1) {
			pps[d[i]]++;
			i /= d[i];
		}
		return pps;
	}
};


//【一括素因数分解】O(n log(log n))
/*
* n 以下の自然数 i の素因数分解を pps[i] に格納し pps を返す（pps[0] は使わない）
*/
vector<map<int, int>> factor_integer_all(int n) {
	// verify : https://atcoder.jp/contests/abc052/tasks/arc067_a

	vector<map<int, int>> pps(n + 1);

	// 順に素数で割っていった残りの値を記録しておくためのテーブル
	vi a(n + 1);
	iota(all(a), 0);

	int p = 2;

	// √n 以下の p の処理（continue されない場合は p は素数）
	for (; p * p <= n; p++) {
		if (a[p] == 1) continue;

		for (int i = p; i <= n; i += p) {
			while (a[i] % p == 0) {
				pps[i][p]++;
				a[i] /= p;
			}
		}
	}

	// √n より大きい p の処理（if 内に入っても p は素数とは限らないので注意）
	for (; p <= n; p++) if (a[p] != 1) pps[p][a[p]]++;

	return pps;
}


//【一括素因数分解（区間）】O((√r + (r - l))log(log r))
/*
* [l..r) に含まれる自然数 i の素因数分解を pps[i - l] に格納し，pps を返す．
*
* 利用：【素数の列挙】
*/
vector<map<ll, int>> factor_integer_interval(ll l, ll r) {
	vector<map<ll, int>> pps(r - l);

	// ps : √r 以下の素数のリスト
	vi ps = eratosthenes(int(sqrt(r) + EPS));

	// 順に素数で割っていった残りの値を記録しておくためのテーブル
	vl a(r - l);
	iota(all(a), l);

	repe(p, ps) {
		for (ll j = (l + p - 1) / p * p; j < r; j += p) {
			while (a[j - l] % p == 0) {
				pps[j - l][p]++;
				a[j - l] /= p;
			}
		}
	}

	for (ll j = l; j < r; j++) if (a[j - l] != 1) pps[j - l][a[j - l]]++;

	return pps;
}


//【約数和関数（一括）】O(n log(log n))
/*
* 各 i∈[1..n] について約数和関数 σ_k(i) = (i の約数の k 乗和) を格納したリストを返す．
* 特に k = 0 なら約数の個数，k = 1 なら約数の総和と等価である．
*
* 利用：【約数倍数変換】
*/
template <class T>
vector<T> divisor_sigma(int k, int n) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6
	// verify : https://atcoder.jp/contests/abc172/tasks/abc172_d

	//【方法】
	// 約数和関数の定義より，等式
	//		σ_k(i) = Σ_(d|i) d^k
	// を得る．これは σ_k が a[i] = i^k を約数ゼータ変換したものであることを意味する．

	vector<T> a(n + 1);
	a[0] = 0;
	repi(i, 1, n) a[i] = T(pow(i, k));

	Div_mul_transform<T> dt(n);
	dt.divisor_zeta(a);

	return a;
}


//【オイラー関数（一括）】O(n log(log n))
/*
* 各 i∈[1..n] についてオイラー関数 φ(i) の値を格納したリストを返す．
*
* 利用：【約数倍数変換】
*/
vl euler_phi(int n) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6
	// verify : https://yukicoder.me/problems/no/2249

	//【方法】
	// 各 i の約数 d について，GCD(i, x) = d となる x∈[1..i] の個数は，
	// x が GCD(i/d, y) = 1 なる y∈[1..i/d] を用いて x = y d と表されるので
	// オイラー関数の定義より φ(i/d) に等しい．
	// これらを全ての d にわたって足し合わせることで，等式
	//		i = Σ_(d|i) φ(i/d)
	//		⇔ i = Σ_(d|i) φ(d)
	// を得る．これは φ を約数ゼータ変換したものが a[i] = i であることを意味する．

	vl a(n + 1);
	repi(i, 1, n) a[i] = i;

	// int にすると途中計算でオーバーフローするので注意
	Div_mul_transform<ll> dt(n);
	dt.divisor_mobius(a);

	return a;
}


//【メビウス関数（一括）】O(n log(log n))
/*
* 各 i∈[1..n] についてメビウス関数 μ(i) の値を格納したリストを返す．
*
* 利用：【約数倍数変換】
*/
vi mobius_mu(int n) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6
	// verify : https://yukicoder.me/problems/no/1514

	vi a(n + 1, 0);
	a[1] = 1;

	Div_mul_transform<int> dt(n);
	dt.divisor_mobius(a);

	return a;
}


//【オイラー関数の累積和（一括）】O(n^(2/3) log(log n)^(1/3))
/*
* 各 i∈[1..nl] について bl[i] = φ(i) を，
* 各 i∈[1..nh] について Bh[i] = Σj∈[1..n/i] φ(j) をそれぞれ格納する．
*
* 制約：nh ≦ nl ≦ n ≦ nl nh
*
* 利用：【ディリクレ畳込みの累積和（乗法的，mint，一括）】
*/
void euler_phi_sum(ll n, int nl, int nh, vm& bl, vm& Bh) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c
	// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

	//【方法】
	// オイラー関数 φ(i) と対応するディリクレ級数を Φ(s) とおくと，
	//		ζ(s) Φ(s) = ζ(s-1)
	// が成り立つ．
	// ζ(s) は乗法的数論関数 a[i] = 1 に対応するディリクレ級数であり，
	// ζ(s-1) は数論関数 c[i] = i に対応するディリクレ級数である．

	if (nl <= 0 || nh <= 0) return;
	Multiplicative_dirichlet_invconvolution_acc_mint mdia(nl);

	vm al(nl + 1), cl(nl + 1), Ah(nh + 1), Ch(nh + 1); mint inv2 = mint(2).inv();
	repi(i, 1, nl) {
		al[i] = 1;
		cl[i] = i;
	}
	repi(i, 1, nh) {
		Ah[i] = n / i;
		Ch[i] = mint(n / i) * (n / i + 1) * inv2;
	}

	mdia.inv_conv_acc(n, al, Ah, cl, Ch, bl, Bh);
}


//【メビウス関数の累積和（一括）】O(n^(2/3) log(log n)^(1/3))
/*
* 各 i∈[1..nl] について bl[i] = μ(i) を，
* 各 i∈[1..nh] について Bh[i] = Σj∈[1..n/i] μ(j) をそれぞれ格納する．
*
* 制約：nh ≦ nl ≦ n ≦ nl nh
*
* 利用：【ディリクレ畳込みの累積和（乗法的，一括）】
*/
void mertens(ll n, int nl, int nh, vl& bl, vl& Bh) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c
	// verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_i

	//【方法】
	// メビウス関数 μ(i) と対応するディリクレ級数を M(s) とおくと，
	//		ζ(s) M(s) = 1
	// が成り立つ．
	// ζ(s) は乗法的数論関数 a[i] = 1 に対応するディリクレ級数であり，
	// 1 は数論関数 c[i] = (i = 1 ? 1 : 0) に対応するディリクレ級数である．

	if (nl <= 0 || nh <= 0) return;
	Multiplicative_dirichlet_invconvolution_acc<ll> mdia(nl);

	vl al(nl + 1, 1), cl(nl + 1), Ah(nh + 1), Ch(nh + 1, 1);
	cl[1] = 1;
	repi(i, 1, nh) Ah[i] = n / i;

	mdia.inv_conv_acc(n, al, Ah, cl, Ch, bl, Bh);
}


