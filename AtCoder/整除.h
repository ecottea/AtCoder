#pragma once
#include "header.h"
// ■■■■■ 約数変換・倍数変換，GCD 畳込み，LCM 畳込み ■■■■■


//【約数倍数変換】
/*
* Div_mul_transform<T>(int n) : O(n log(log n))
*   n 以下の素数を持って初期化する．
*
* divisor_zeta(vT& a) : O(n log(log n))
*   A[i] = Σ_(j | i) a[j] なる A に上書きする（約数からの寄与を足し込む）
*
* divisor_mobius(vT& A) : O(n log(log n))
*   A[i] = Σ_(j | i) a[j] なる a に上書きする（約数からの寄与を取り除く）
*
* vT lcm_convolution(vT a, vT b) : O(n log(log n))
*   c[k] = Σ_(LCM(i, j) = k) a[i] b[j] なる c を返す．
*   ただし c[n] を含めそれ以降は切り捨てる．
*
* multiple_zeta(vT& a) : O(n log(log n))
*   A[i] = Σ_(i | j) a[j] なる A に上書きする（倍数からの寄与を足し込む）
*
* multiple_mobius(vT& A) : O(n log(log n))
*   A[i] = Σ_(i | j) a[j] なる a に上書きする（倍数からの寄与を取り除く）
*
* vT gcd_convolution(vT a, vT b) : O(n log(log n))
*   c[k] = Σ_(GCD(i, j) = k) a[i] b[j] なる c を返す．
*
* 制約：1-indexed とし，a[0], b[0] は使用しない．
*/
template <typename T>
class Div_mul_transform {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	vi ps; // 素数のリスト

public:
	// n 以下の素数を持って初期化する．
	Div_mul_transform(int n) {
		// verify : https://judge.yosupo.jp/problem/gcd_convolution

		// is_prime[i] : i が素数か
		vb is_prime(n + 1, true);
		is_prime[0] = is_prime[1] = false;
		int i = 2;

		// √n 以下の i の処理
		for (; i <= n / i; i++) if (is_prime[i]) {
			ps.push_back(i);
			for (int j = i * i; j <= n; j += i) is_prime[j] = false;
		}

		// √n より大きい i の処理
		for (; i <= n; i++) if (is_prime[i]) ps.push_back(i);
	}
	Div_mul_transform() {}

	// A[i] = Σ_(j | i) a[j] なる A に上書きする（約数からの寄与を足し込む）
	void divisor_zeta(vector<T>& a) {
		// verify : https://judge.yosupo.jp/problem/lcm_convolution

		//【例（n = 8 のとき）】
		//	A[1] = a[1]
		//	A[2] = a[1] + a[2]
		//	A[3] = a[1]        + a[3]
		//	A[4] = a[1] + a[2]        + a[4]
		//	A[5] = a[1]                      + a[5]
		//	A[6] = a[1] + a[2] + a[3]               + a[6]
		//	A[7] = a[1]                                    + a[7]
		//	A[8] = a[1] + a[2]        + a[4]                      + a[8]

		//【備考】
		// a[1..n] のディリクレ母関数を α(s) = Σ_i a[i] i^(-s) とすると，
		// α(s) にゼータ関数 ζ(s) = Σ_i i^(-s) を掛けることに対応する．

		int n = sz(a) - 1;

		// 各素因数ごとに下からの累積和をとる
		repe(p, ps) repi(i, 1, n / p) a[p * i] += a[i];
	}

	//  A[i] = Σ_(j | i) a[j] なる a に上書きする（約数からの寄与を取り除く）
	void divisor_mobius(vector<T>& A) {
		// verify : https://judge.yosupo.jp/problem/lcm_convolution

		//【例（n = 8 のとき）】
		//	a[1] =  A[1]
		//	a[2] = -A[1] + A[2]
		//	a[3] = -A[1]        + A[3]
		//	a[4] =       - A[2]        + A[4]
		//	a[5] = -A[1]                      + A[5]
		//	a[6] =  A[1] - A[2] - A[3]               + A[6]
		//	a[7] = -A[1]                                    + A[7]
		//	a[8] =                     - A[4]                      + A[8]

		int n = sz(A) - 1;

		// 各素因数ごとに上からの差分をとる
		repe(p, ps) repir(i, n / p, 1) A[p * i] -= A[i];
	}

	// c[k] = Σ_(LCM(i, j) = k) a[i] b[j] なる c を返す．
	vector<T> lcm_convolution(vector<T> a, vector<T> b) {
		// verify : https://judge.yosupo.jp/problem/lcm_convolution

		int n = sz(a) - 1;

		// 各素因数の max をとったものが LCM なので max 畳込みを行う．
		divisor_zeta(a); divisor_zeta(b);
		repi(i, 1, n) a[i] *= b[i];
		divisor_mobius(a);
		return a;
	}

	// A[i] = Σ_(i | j) a[j] なる A に上書きする（倍数からの寄与を足し込む）
	void multiple_zeta(vector<T>& a) {
		// verify : https://judge.yosupo.jp/problem/gcd_convolution

		//【例（n = 8 のとき）】
		//	A[1] = a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + a[8]
		//	A[2] =        a[2]        + a[4]        + a[6]        + a[8]
		//	A[3] =               a[3]               + a[6]              
		//	A[4] =                      a[4]                      + a[8]
		//	A[5] =                             a[5]                     
		//	A[6] =                                    a[6]              
		//	A[7] =                                           a[7]       
		//	A[8] =                                                  a[8]

		//【備考】
		// a[1..n] のディリクレ母関数を α(s) = Σ_i a[i] i^(-s) とすると，
		// α(s) にゼータ関数の変種 ζ(-s) = Σ_i i^s を掛けることに対応する．

		int n = sz(a) - 1;

		// 各素因数ごとに上からの累積和をとる
		repe(p, ps) repir(i, n / p, 1) a[i] += a[p * i];
	}

	// A[i] = Σ_(i | j) a[j] なる a に上書きする（倍数からの寄与を取り除く）
	void multiple_mobius(vector<T>& A) {
		// verify : https://judge.yosupo.jp/problem/gcd_convolution

		//【例（n = 8 のとき）】
		//	a[1] = A[1] - A[2] - A[3]        - A[5] + A[6] - a[7]       
		//	a[2] =        A[2]        - A[4]        - A[6]              
		//	a[3] =               A[3]               - A[6]              
		//	a[4] =                      A[4]                      - A[8]
		//	a[5] =                             A[5]                     
		//	a[6] =                                    A[6]              
		//	a[7] =                                           A[7]       
		//	a[8] =                                                  A[8]

		int n = sz(A) - 1;

		// 各素因数ごとに下からの差分をとる
		repe(p, ps) repi(i, 1, n / p) A[i] -= A[p * i];
	}

	// c[k] = Σ_(GCD(i, j) = k) a[i] b[j] なる c を返す．
	vector<T> gcd_convolution(vector<T> a, vector<T> b) {
		// verify : https://judge.yosupo.jp/problem/gcd_convolution

		int n = sz(a) - 1;

		// 各素因数の min をとったものが GCD なので min 畳込みを行う．
		multiple_zeta(a); multiple_zeta(b);
		repi(i, 1, n) a[i] *= b[i];
		multiple_mobius(a);
		return a;
	}
};


//【約数倍数変換（添字約数制限）】
/*
* Limited_div_mul_transform<T>(ll n) : O(√n)
*   添字集合を n の約数集合として初期化する．
*  （σ(n) : n の約数の個数，ω(n) : n の素因数の種類数）
*
* divisor_zeta(umap<ll, T>& a) : O(σ(n) ω(n))
*   A[i] = Σ_(j | i) a[j] なる A に上書きする（約数からの寄与を足し込む）
*
* divisor_mobius(umap<ll, T>& A) : O(σ(n) ω(n))
*	A[i] = Σ_(j | i) a[j] なる a に上書きする（約数からの寄与を取り除く）
*
* umap<ll, T> lcm_convolution(umap<ll, T>& a, umap<ll, T>& b) : O(σ(n) ω(n))
*   c[k] = Σ_(LCM(i, j) = k) a[i] b[j] なる c を返す．
*
* multiple_zeta(umap<ll, T>& a) : O(σ(n) ω(n))
*   A[i] = Σ_(i | j) a[j] なる A に上書きする（倍数からの寄与を足し込む）
*
* multiple_mobius(umap<ll, T>& A) : O(σ(n) ω(n))
*   A[i] = Σ_(i | j) a[j] なる a に上書きする（倍数からの寄与を取り除く）

* umap<ll, T> gcd_convolution(umap<ll, T> a, umap<ll, T> b) : O(σ(n) ω(n))
*   c[k] = Σ_(GCD(i, j) = k) a[i] b[j] なる c を返す．
*/
template <typename T>
struct Limited_div_mul_transform {
	vl ps;   // n の素因数の昇順リスト
	vl divs; // n の約数の昇順リスト
	unordered_set<ll> divs_s;

public:
	// 添字集合を n の約数集合として初期化する．
	Limited_div_mul_transform(ll n) : divs{ 1 } {
		for (ll p = 2; p * p <= n; p++) {
			int d = 0;
			while (n % p == 0) {
				d++;
				n /= p;
			}
			if (d == 0) continue;

			ps.push_back(p);

			vl powp(d);
			powp[0] = p;
			rep(i, d - 1) powp[i + 1] = powp[i] * p;

			repir(j, sz(divs) - 1, 0) {
				rep(i, d) {
					divs.push_back(divs[j] * powp[i]);
				}
			}
		}

		if (n > 1) {
			ps.push_back(n);

			repir(j, sz(divs) - 1, 0) {
				divs.push_back(divs[j] * n);
			}
		}
		sort(all(divs));

		divs_s = unordered_set<ll>(all(divs));
	}
	Limited_div_mul_transform() {}

	// A[i] = Σ_(j | i) a[j] なる A に上書きする（約数からの寄与を足し込む）
	void divisor_zeta(unordered_map<ll, T>& f) {
		// verify : https://atcoder.jp/contests/abc335/tasks/abc335_g

		// 各素因数ごとに下からの累積和をとる
		repe(p, ps) {
			repe(d, divs) {
				if (!divs_s.count(p * d)) continue;
				f[p * d] += f[d];
			}
		}
	}

	//  A[i] = Σ_(j | i) a[j] なる a に上書きする（約数からの寄与を取り除く）
	void divisor_mobius(unordered_map<ll, T>& f) {
		// verify : https://atcoder.jp/contests/arc064/tasks/arc064_d

		// 各素因数ごとに上からの差分をとる
		repe(p, ps) {
			for (auto it = divs.rbegin(); it != divs.rend(); it++) {
				ll d = *it;
				if (!divs_s.count(p * d)) continue;
				f[p * d] -= f[d];
			}
		}
	}

	// c[k] = Σ_(LCM(i, j) = k) a[i] b[j] なる c を返す．
	unordered_map<ll, T> lcm_convolution(unordered_map<ll, T> a, unordered_map<ll, T> b) {
		// 各素因数の max をとったものが LCM なので max 畳込みを行う．
		divisor_zeta(a);
		divisor_zeta(b);
		repe(d, divs) a[d] *= b[d];
		divisor_mobius(a);
		return a;
	}

	// A[i] = Σ_(i | j) a[j] なる A に上書きする（倍数からの寄与を足し込む）
	void multiple_zeta(unordered_map<ll, T>& f) {
		// 各素因数ごとに上からの累積和をとる
		repe(p, ps) {
			for (auto it = divs.rbegin(); it != divs.rend(); it++) {
				ll d = *it;
				if (!divs_s.count(p * d)) continue;
				f[d] += f[p * d];
			}
		}
	}

	// A[i] = Σ_(i | j) a[j] なる a に上書きする（倍数からの寄与を取り除く）
	void multiple_mobius(unordered_map<ll, T>& f) {
		// verify : https://atcoder.jp/contests/abc212/tasks/abc212_g

		// 各素因数ごとに下からの差分をとる
		repe(p, ps) {
			repe(d, divs) {
				if (!divs_s.count(p * d)) continue;
				f[d] -= f[p * d];
			}
		}
	}

	// c[k] = Σ_(GCD(i, j) = k) a[i] b[j] なる c を返す．
	unordered_map<ll, T> gcd_convolution(unordered_map<ll, T> a, unordered_map<ll, T> b) {
		// 各素因数の min をとったものが GCD なので min 畳込みを行う．
		multiple_zeta(a);
		multiple_zeta(b);
		repe(d, divs) a[d] *= b[d];
		multiple_mobius(a);
		return a;
	}
};


