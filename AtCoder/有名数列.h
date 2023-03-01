#pragma once
#include "header.h"
#include "行列.h"
#include "二項係数.h"
#include "FPS(mint).h"
// ■■■■■ 有名数列 ■■■■■


//【フィボナッチ数】O(n)
/*
* フィボナッチ数のリスト fib[0..n) を返す（fib[0]=0, fib[1]=1 とする）
*/
template <class T>
vector<T> fibonacci(int n) {
	// verify : https://atcoder.jp/contests/tenka1-2012-qualA/tasks/tenka1_2012_qualA_1

	vector<T> fib(n);
	fib[0] = 0;
	fib[1] = 1;

	repi(i, 2, n - 1) fib[i] = fib[i - 1] + fib[i - 2];

	return fib;
}


//【フィボナッチ数】O(log n)
/*
* n 番目のフィボナッチ数 fib[n] を返す（fib[0]=0, fib[1]=1 とする）
*
* 利用：【行列】
*/
mint fibonacci(ll n) {
	// verify : https://atcoder.jp/contests/math-and-algorithm/tasks/math_and_algorithm_at

	//【方法】
	// フィボナッチ数列 fib[] の満たす漸化式は
	//		fib[n] = fib[n - 1] + fib[n - 2]（fib[0] = 0, fib[1] = 1）
	// なる 3 項間線形漸化式だが，
	//		a0[n] = fib[n]    （a0[0] = 0）
	//		a1[n] = fib[n - 1]（a1[0] = 1）
	// とおくことにより，これを
	//		a0[n] = 1 * a0[n - 1] + 1 * a1[n - 1]
	//		a1[n] = 1 * a0[n - 1] + 0 * a1[n - 1]
	// なるベクトルの 2 項間線形漸化式に書き換えることができる．
	// これの一般項は容易に
	//		[a0[n]] = [1 1]^n [0]
	//		[a1[n]] = [1 0]   [1]
	// と表示することができるので，行列の累乗を計算する問題に帰着する．

	Matrix<mint> coef({ {1, 1}, {1, 0} });
	return coef.pow(n).v[0][1];
}


//【k-ナッチ数】O(n)
/*
* i∈[0..n) について，i 番目の k-ナッチ数を seq[i] に格納する．
* 初項は seq[0..k-1) = 0, seq[k-1] = 1 とする．
*
*（累積和で高速化した DP）
*/
template <class T>
void k_nacci_acc(int n, int k, vector<T>& seq) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_semiexp

	seq = vector<T>(n);
	if (n < k) return;
	seq[k - 1] = 1;

	// acc[i] : Σseq[0..i)
	vector<T> acc(n + 1);
	acc[k] = 1;

	repi(i, k, n - 1) {
		// seq[i] = Σseq[i-k..i) = acc[i] - acc[i-k]
		seq[i] = acc[i] - acc[i - k];

		// acc[i+1] = Σseq[0..i] = acc[i] + seq[i]
		acc[i + 1] = acc[i] + seq[i];
	}
}


//【k-ナッチ数】O(n + k)
/*
* i∈[0..n) について，i 番目の k-ナッチ数を seq[i] に格納する．
* 初項は seq[0..k-1) = 0, seq[k-1] = 1 とする．
*
*（いもす法で高速化した DP）
*/
template <class T>
void k_nacci_imos(int n, int k, vector<T>& seq) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_semiexp

	seq = vector<T>(n + k + 1);
	seq[k - 1] = 1;

	// 初項からの寄与の種を蒔く．
	seq[k] += seq[k - 1];
	seq[k + k] -= seq[k - 1];

	repi(i, k, n - 1) {
		// seq[i] からの寄与の種を蒔く．
		seq[i + 1] += seq[i];
		seq[i + k + 1] -= seq[i];

		// 累積和をとる．
		seq[i + 1] += seq[i];
	}

	seq.resize(n);
}


//【一般化デラノイ数】O(h w)
/*
* 初項を seq[0][0] = 1 とし，任意の (x, y) ∈ [0..h)*[0..w) \ {(0, 0)} について
*		seq[x][y] = Σseq(x-s..x](y-t..y] - seq[x][y] （範囲外の値は 0 とみなす）
* なる漸化式で定められる数列を seq[x][y] に格納する．
*
*（二次元累積和で高速化した格子 DP）
*/
template <class T>
void delannoy_number_acc(int h, int w, int s, int t, vector<vector<T>>& seq) {
	seq = vector<vector<T>>(h, vector<T>(w));
	seq[0][0] = 1;

	// acc[x][y] : Σseq[0..x][0..y]
	vector<vector<T>> acc(h, vector<T>(w));
	acc[0][0] = 1;

	rep(x, h) rep(y, w) {
		// 初項は更新しない．
		if (x == 0 && y == 0) continue;

		// tmp : acc[x-1][y] + acc[x][y-1] - acc[x-1][y-1]
		T tmp = 0;
		if (x - 1 >= 0) tmp += acc[x - 1][y];
		if (y - 1 >= 0) tmp += acc[x][y - 1];
		if (x - 1 >= 0 && y - 1 >= 0) tmp -= acc[x - 1][y - 1];

		// seq[x][y]
		// = Σseq(x-s..x](y-t..y] - seq[x][y]
		// = acc[x-1][y] + acc[x][y-1] - acc[x-1][y-1]
		//		- acc[x-s-1][y] - acc[x][y-t-1] + acc[x-s-1][y-t-1] 
		seq[x][y] += tmp;
		if (x - s - 1 >= 0) seq[x][y] -= acc[x - s - 1][y];
		if (y - t - 1 >= 0) seq[x][y] -= acc[x][y - t - 1];
		if (x - s - 1 >= 0 && y - t - 1 >= 0) seq[x][y] += acc[x - s - 1][y - t - 1];

		// acc[x][y]
		// = Σseq[0..x][0..y]
		// = seq[x][y] + acc[x-1][y] + acc[x][y-1] - acc[x-1][y-1]
		acc[x][y] = seq[x][y] + tmp;
	}
}


//【一般化デラノイ数】O(h w)
/*
* 初項を seq[0][0] = 1 とし，任意の (x, y) ∈ [0..h)*[0..w) \ {(0, 0)} について
*		seq[x][y] = Σseq(x-s..x](y-t..y] - seq[x][y] （範囲外の値は 0 とみなす）
* なる漸化式で定められる数列を seq[x][y] に格納する．
*
*（二次元いもす法で高速化した格子 DP）
*/
template <class T>
void delannoy_number_imos(int h, int w, int s, int t, vector<vector<T>>& seq) {
	seq = vector<vector<T>>(h, vector<T>(w));
	seq[0][0] = 1;

	rep(x, h) rep(y, w) {
		T v = seq[x][y];

		// seq[x][y] からの寄与の種を蒔く．
		if (x + s + 1 < h) seq[x + s + 1][y] -= v;
		if (y + t + 1 < w) seq[x][y + t + 1] -= v;
		if (x + s + 1 < h && y + t + 1 < w) seq[x + s + 1][y + t + 1] += v;

		if (x != 0 || y != 0) v *= 2;

		// 累積和をとる．
		if (x + 1 < h) seq[x + 1][y] += v;
		if (y + 1 < w) seq[x][y + 1] += v;
		if (x + 1 < h && y + 1 < w) seq[x + 1][y + 1] -= v;
	}
}


//【ベルヌーイ数（mod 998244353）】O(n log n)
/*
* i=[0..n) についてベルヌーイ数 B(i) を b[i] に格納する．
*
* 制約：fm は (2(n+1))! まで計算可能であること
*
* 利用：【形式的冪級数】,【指数関数】,【階乗など（法が大きな素数）】
*/
void bernoulli(int n, vm& b, const Factorial_mint& fm) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%99%E3%83%AB%E3%83%8C%E3%83%BC%E3%82%A4%E6%95%B0
	// verify : https://judge.yosupo.jp/problem/bernoulli_number

	//【方法】
	// ベルヌーイ数 B(n) はそもそも
	//		x / (exp(x) - 1) = Σn=[0..∞) B(n) / n! x^n
	// で定義される．

	MFPS f = exp(MFPS(vm({ 0, 1 })), n + 1, fm);
	f <<= 1;
	f = f.inv(n);

	b.resize(n);
	rep(i, n) b[i] = f[i] * fm.fact(i);
}


//【第 1 種スターリング数】
/*
* fps(mint).h の【下降階乗冪（符号付き第 1 種スターリング数）】または
* 数え上げ(順列).h の【j 個の巡回置換の積で表される順列の数え上げ】を利用すればいい．
*/


//【第 1 種スターリング数（法が小さな素数）】
/*
* Stirling_S1_small_prime_mod(int p) : O(p^2)
*	p を法として初期化する．
*
* int get(ll n, ll r) : O(log n)
*	符号付き第 1 種スターリング数 S(n, r) mod p を返す．
*/
ostream& operator<<(ostream& os, const dynamic_modint<82645>& x) { os << x.val(); return os; }
class Stirling_S1_small_prime_mod {
	int p; // 利用する法（素数）

	using mint_p = dynamic_modint<82645>; // 他と被らなければ何でも良い．
	using vmp = vector<mint_p>;
	using vvmp = vector<vmp>;

	vvmp bin; // bin[i][j] : binomial(i, j)
	vvmp s1;  // s1[i][j] : S(i, j)

public:
	// p を法として初期化する．
	Stirling_S1_small_prime_mod(int p) : p(p), bin(p, vmp(p)), s1(p, vmp(p)) {
		// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind_small_p_large_n

		mint_p::set_mod(p);

		// bin[0..p)[0..p) を愚直に計算する．
		bin[0][0] = 1;
		rep(i, p - 1) repi(j, 0, i) {
			bin[i + 1][j] += bin[i][j];
			bin[i + 1][j + 1] += bin[i][j];
		}

		// s1[0..p)[0..p) を愚直に計算する．
		s1[0][0] = 1;
		rep(i, p - 1) repi(j, 0, i) {
			s1[i + 1][j] -= s1[i][j] * i;
			s1[i + 1][j + 1] += s1[i][j];
		}
	}

	// 符号付き第 1 種スターリング数 S(n, r) mod p を返す．
	int get(ll n, ll r) {
		// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind_small_p_large_n

		if (n == 0) return (int)(r == 0);
		if (n < r) return 0;

		// S(n, r) の三角形は歪んでいるので補正する．
		r -= (n + p - 1) / p;
		if (r < 0) return 0;

		// dn : n の p 進表示の桁の数（上位から順）
		vi dn; ll n_ = n, pow_p = 1;
		while (n_ > 0) {
			dn.push_back((int)(n_ % p));
			n_ /= p;
			if (n_ == 0) break;
			pow_p *= p;
		}
		pow_p /= p;
		reverse(all(dn));
		int k = sz(dn);

		mint_p res = 1;
		rep(i, k - 1) {
			ll ri = r / (pow_p * (p - 1));
			if (dn[i] - ri < 0) return 0;
			res *= bin[p - 1 - ri][dn[i] - ri];
			r %= pow_p * (p - 1);
			pow_p /= p;
		}
		res *= s1[dn[k - 1]][r + (dn[k - 1] > 0 ? 1 : 0)];

		return res.val();
	}
};


//【第 2 種スターリング数】
/*
* 写像12相.h の【集合の分割の数（ボールの区別あり，箱の区別なし，箱の中身は 1 個以上）】を利用すればいい．
*/


//【第 2 種スターリング数（法が小さな素数）】
/*
* Stirling_S2_small_prime_mod(int p) : O(p^2)
*	p を法として初期化する．
*
* int get(ll n, ll r) : O(log n)
*	第 2 種スターリング数 s(n, r) mod p を返す．
*/
ostream& operator<<(ostream& os, const dynamic_modint<124572>& x) { os << x.val(); return os; }
class Stirling_S2_small_prime_mod {
	int p; // 利用する法（素数）

	using mint_p = dynamic_modint<124572>; // 他と被らなければ何でも良い．
	using vmp = vector<mint_p>;
	using vvmp = vector<vmp>;

	vvmp bin; // bin[i][j] : binomial(i, j)
	vvmp s2;  // s1[i][j] : S(i, j)

public:
	// p を法として初期化する．
	Stirling_S2_small_prime_mod(int p) : p(p), bin(p, vmp(p)), s2(p + 1, vmp(p + 1)) {
		// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind_small_p_large_n

		mint_p::set_mod(p);

		// bin[0..p)[0..p) を愚直に計算する．
		bin[0][0] = 1;
		rep(i, p - 1) repi(j, 0, i) {
			bin[i + 1][j] += bin[i][j];
			bin[i + 1][j + 1] += bin[i][j];
		}

		// s2[0..p)[0..p) を愚直に計算する．
		s2[0][0] = 1;
		rep(i, p) repi(j, 0, i) {
			s2[i + 1][j] += s2[i][j] * j;
			s2[i + 1][j + 1] += s2[i][j];
		}
	}

	// 第 2 種スターリング数 s(n, r) mod p を返す．
	int get(ll n, ll r) {
		// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind_small_p_large_n

		if (n == 0) return (int)(r == 0);
		if (n < r) return 0;

		// s(n, r) の三角形は歪んでいるので補正する．
		n -= r / p + 1; r--;
		if (r < 0) return 0;

		// dn : n の p 進（最下位だけ p-1 進）表示の桁の数（上位から順）
		// dr : r の p 進表示の桁の数（上位から順）
		vi dn, dr;
		do {
			dn.push_back((int)(n % (dr.empty() ? p - 1 : p)));
			n /= (dr.empty() ? p - 1 : p);
			dr.push_back((int)(r % p));
			r /= p;
		} while (n > 0);
		reverse(all(dn)); reverse(all(dr));
		int k = sz(dn);

		mint_p res = 1;
		rep(i, k - 1) res *= bin[dn[i]][dr[i]];
		res *= s2[dn[k - 1] + (dr[k - 1] == p - 1 ? 2 : 1)][dr[k - 1] + 1];

		return res.val();
	}
};


//【カタラン数】
/*
* n 番目のカタラン数 C[n] は
*	C[n] = bin(2n, n)/(n+1) = (2n)!/((n+1)! n!) = bin(2n, n) - bin(2n, n-1)
* と表され，最初の数項は
*	C[0]=1, C[1]=1, C[2]=2, C[3]=5, C[4]=14, C[5]=42, ...
* である．また C[n] の母関数は
*	f(z) = (1 - √(1-4z)) / (2z)
* である．
*/


//【カタラン数の自己畳込み】
/*
* カタラン数の列 {C[n]} を k(>0) 回自己畳込みした数列を {C^(k)[n]} とすると，
*	C^(k)[n] = bin(2n + k, n) * k / (2n + k)
* である．
* 
* verify : https://atcoder.jp/contests/xmascon22/tasks/xmascon22_d
*/

