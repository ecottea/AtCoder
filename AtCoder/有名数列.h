#pragma once
#include "header.h"
#include "行列.h"
#include "二項係数.h"
#include "FPS(mint).h"
#include "FPS(スパース).h"
#include "数論変換.h"
// ■■■■■ 有名数列 ■■■■■


//【フィボナッチ数】O(n)
/*
* フィボナッチ数のリスト fib[0..n] を返す（fib[0]=0, fib[1]=1 とする）
*/
template <class T>
vector<T> fibonacci(int n) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/math_and_algorithm_ap

	vector<T> fib(n + 1);
	fib[0] = 0;
	fib[1] = 1;

	repi(i, 2, n) fib[i] = fib[i - 1] + fib[i - 2];

	return fib;
}


//【フィボナッチ数】O(log n)
/*
* n 番目のフィボナッチ数 fib[n] を返す（fib[0]=0, fib[1]=1 とする）
*
* 利用：【正方行列（固定サイズ）】
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

	Fixed_matrix<mint, 2> coef({ {1, 1}, {1, 0} });
	return coef.pow(n).v[0][1];
}


//【k-ナッチ数】O(n)
/*
* 各 i∈[0..n) について，i 番目の k-ナッチ数を格納したリスト seq を返す．
* 初項は seq[0..k-1) = 0, seq[k-1] = 1 とする．
*
*（累積和で高速化した DP）
*/
template <class T>
vector<T> k_nacci_acc(int n, int k) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_semiexp

	vector<T> seq(n);
	if (n < k) return seq;
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

	return seq;
}


//【k-ナッチ数】O(n + k)
/*
* 各 i∈[0..n) について，i 番目の k-ナッチ数を格納したリスト seq を返す．
* 初項は seq[0..k-1) = 0, seq[k-1] = 1 とする．
*
*（いもす法で高速化した DP）
*/
template <class T>
vector<T> k_nacci_imos(int n, int k) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_semiexp

	vector<T> seq(n + k + 1);
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
	return seq;
}


//【k-ナッチ数】O(k log k log n)
/*
* n 番目の k-ナッチ数 a[n] を返す（初項は a[0..k-1) = 0, a[k-1] = 1 とする）
*
* 利用：【形式的冪級数】,【線形漸化式】
*/
mint k_nacci_fps(ll n, int k) {
	Assert(k > 0);
	vm a(k), c(k, 1);
	a[k - 1] = 1;
	return linearly_recurrent_sequence(a, c, n);
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
* i∈[0..n) についてのベルヌーイ数 B(i) のリストを返す．
*
* 制約：fm は n! まで計算可能
*
* 利用：【形式的冪級数】
*/
vm bernoulli_all_n(int n, const Factorial_mint& fm) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%99%E3%83%AB%E3%83%8C%E3%83%BC%E3%82%A4%E6%95%B0
	// verify : https://judge.yosupo.jp/problem/bernoulli_number

	//【方法】
	// ベルヌーイ数 B(n) はそもそも
	//		z / (exp(z) - 1) = Σn=[0..∞) B(n) / n! z^n
	// で定義される．

	MFPS b(0, n);
	rep(i, n) b[i] = fm.fact_inv(i + 1);
	b = b.inv(n);

	rep(i, n) b[i] *= fm.fact(i);

	return b.c;
}


//【ベルヌーイ数】O(n log(log(n)))
/*
* ベルヌーイ数 B(n) を返す．
*/
mint bernoulli(int n, const Factorial_mint& fm) {
	// 参考 : https://atcoder.jp/contests/xmascon23/editorial/8975
	// verify : https://atcoder.jp/contests/xmascon23/tasks/xmascon23_e

	if (n == 0) return 1;
	if (n == 1) return mint(-2).inv();

	vm dp(n + 1);
	dp[n] = (n & 1 ? -1 : 1) * fm.inv(n + 1);
	dp[n - 1] = (n & 1 ? 1 : -1) * (fm.inv(n) + fm.inv(n + 1) * fm.bin(n, n - 1));
	repir(i, n - 2, 0) {
		dp[i] += (-2LL * i * i + (n - 5LL) * i + (n - 4)) * dp[i + 1];
		dp[i] += (i + 2LL) * (i + 2) * dp[i + 2];
		dp[i] *= fm.inv(n - i) * fm.inv(i + 1);
	}

	// d[i] : i を割り切る最大の素数
	vi d(n + 1);
	iota(all(d), 0);

	for (int p = 2; p * p <= n; p++) {
		if (d[p] != p) continue;
		for (int i = p; i <= n; i += p) d[i] = p;
	}

	// pow_i[i] : i^n
	vm pow_i(n + 1);
	repi(i, 1, n) {
		if (d[i] == i) pow_i[i] = mint(i).pow(n);
		else pow_i[i] = pow_i[d[i]] * pow_i[i / d[i]];
	}

	mint res;
	repi(i, 0, n) res += dp[i] * pow_i[i];

	return res;
}


//【第 1 種スターリング数】
/*
* FPS(mint).h の【一次式の積の展開（等差数列）】で a = -1, b = 0 とすれば良い．
* 数え上げ(順列).h の【j 個の巡回置換の積で表される順列の数え上げ】を利用すれば良い．
*/


//【第 1 種スターリング数（一括，k が固定，mod 998244353）】O((N-K) log(N-K))
/*
* 各 n∈[K..N] に対する符号付き第 1 種スターリング数 S(n,K) を格納したリストを返す．
*
* 制約 : fm は (N+1)! まで計算可能
*
* 利用：【指数関数】
*/
vm stirling_S1_fixed_K(int N, int K, const Factorial_mint& fm) {
	// 参考 : https://en.wikipedia.org/wiki/Stirling_numbers_of_the_first_kind
	// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind_fixed_k

	//【方法】
	// 符号付き第 1 種スターリング数 S(n,K) の定義は
	//		Σk∈[0..n] S(n,k) z^k = Πi∈[0..n) (z-i)
	// であった．
	// 
	// 2 変数関数 (1+z)^w を 2 通りの方法で w についての形式的冪級数に展開する． 
	// 一般化二項定理を用いると，
	//		(1+z)^w
	//		= Σn∈[0..∞) bin(w, n) z^n  (一般化二項定理)
	//		= Σn∈[0..∞) ((Πi∈[0..n) (w-i)) / n!) z^n  (一般化二項係数の定義)
	//		= Σn∈[0..∞) ((Σk∈[0..n] S(n,k) w^k) / n!) z^n  (S(n,k) の定義)
	//		= Σk∈[0..∞) w^k Σn∈[k..∞) S(n,k) z^n / n!  (和の順序交換)
	// となる．一方 exp-log テクを用いると，
	//		(1+z)^w
	//		= exp(log(1+z)^w)  (exp と log は互いに逆関数)
	//		= exp(w log(1+z))  (log の性質)
	//		= Σk∈[0..∞) 1/k! (w log(1+z))^k  (exp のマクローリン展開)
	//		= Σk∈[0..∞) w^k (log(1+z)^k / k!)
	// となる．両者の w^k の係数を比較して，
	//		Σn∈[k..∞) S(n,k) z^n / n! = log(1+z)^k / k!
	// を得る．これは K を固定したときの S(n,K) の指数型母関数が
	//		log(1+z)^K / K!
	// であることを意味している．
	//
	// 実際の計算では [z^0]log(1+z) = 0 なので，1 つシフトしてから累乗を計算する．

	int W = N - K + 1;

	// f : log(1+z)/z（[z^0]f(z) = 1）
	MFPS f(0, W);
	rep(i, W) f[i] = (i & 1 ? -1 : 1) * fm.inv(i + 1);

	// f : log(1+z)^K / z^K
	f = exp_fps(log_fps(f, W, fm) * K, W, fm);

	vm res(W);
	rep(i, W) res[i] = f[i] * fm.fact_inv(K) * fm.fact(K + i);

	return res;
}


//【第 1 種スターリング数（法が小さな素数）】
/*
* Stirling_S1_small_prime_mod(int p) : O(p^2)
*	p を法として初期化する．
*
* int get(ll n, ll k) : O(log n)
*	符号付き第 1 種スターリング数 S(n, k) mod p を返す．
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


//【第 2 種スターリング数（一括，k が固定，mod 998244353）】O((N-K) log(N-K))
/*
* 各 n∈[K..N] に対する第 2 種スターリング数 s(n,K) を格納したリストを返す．
*
* 制約 : fm は (N+1)! まで計算可能
*
* 利用：【指数関数】
*/
vm stirling_s2_fixed_K(int N, int K, const Factorial_mint& fm) {
	// 参考 : https://en.wikipedia.org/wiki/Stirling_numbers_of_the_second_kind
	// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind_fixed_k

	int W = N - K + 1;

	// f : (exp(z)-1)/z（[z^0]f(z) = 1）
	MFPS f(0, W);
	rep(i, W) f[i] = fm.fact_inv(i + 1);

	// f : (exp(z)-1)^K / z^K
	f = exp_fps(log_fps(f, W, fm) * K, W, fm);

	vm res(W);
	rep(i, W) res[i] = f[i] * fm.fact_inv(K) * fm.fact(K + i);

	return res;
}


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


//【ワイエルシュトラスの p 関数（mod 998244353）】O(n (log n)^2)
/*
* [z^[0..n]] p(z; g2, g3) を返す．（[z^(-2)] p(z; g2, g3) = 1 を含めていないので注意）
*
* 利用：【オンライン畳込み（mod 998244353）】
*/
vm weierstrass_p(int n, mint g2, mint g3) {
	// verify : https://mojacoder.app/users/googol_S0/problems/Most_Difficult_Exam_Plus

	vm a(n + 1);
	if (2 < n + 1) a[2] = g2 / 20;
	if (4 < n + 1) a[4] = g3 / 28;
	if (n <= 5) return a;

	Online_convolution O2(n + 1), O2w(n + 1), O3(n + 1);

	O2.set(a[2], a[2]);
	O2w.set(a[2], a[2]);
	O3.set(O2.back(), a[2]);

	O2.set(a[4], a[4]);
	O2w.set(2 * a[4], 2 * a[4]);
	O3.set(O2.back(), a[4]);

	for (int i = 1; 2 * (i + 2) <= n; i++) {
		int id = 2 * (i + 2);

		a[id] += g2 * a[id - 4];
		a[id] -= 12 * O2[i - 1];
		a[id] += 4 * O2w[i - 1];
		if (i >= 3) a[id] -= 4 * O3[i - 3];
		a[id] /= 8 * i + 28;

		O2.set(a[id], a[id]);
		O2w.set((i + 2) * a[id], (i + 2) * a[id]);
		O3.set(O2.back(), a[id]);
	}

	return a;
}


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


