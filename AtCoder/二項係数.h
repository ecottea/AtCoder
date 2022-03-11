#pragma once
#include "header.h"
// ■■■■■ 階乗，二項係数 ■■■■■


//【階乗と二項係数（法が大きな素数，mint利用）】
/*
* Factorial_mint(int n_max) : O(n_max)
*	n_max! まで計算可能として初期化する．
*
* mint factorial(int n) : O(1)
*	n! を返す．
*
* mint factorial_inv(int n) : O(1)
*	1 / n! を返す．
*
* mint inv(int n) : O(1)
*	1 / n を返す．
*
* mint permutation(int n, int r) : O(1)
*	順列の数 nPr を返す．
*
* mint binomial(int n, int r) : O(1)
*	二項係数 nCr を返す．
*
* mint multinomial(vi r) : O(|r|)
*	多項係数 nC[r] を返す．（n = Σr）
*/
struct Factorial_mint {
	// 階乗，階乗の逆数，逆数の値を保持するテーブル
	int n_max;
	vm fac_, fac_inv_, inv_;

	// n! までの階乗とその逆数を前計算しておく．O(n)
	Factorial_mint(int n) : n_max(n) {
		fac_ = vm(n + 1);
		fac_[0] = 1;
		repi(i, 1, n) fac_[i] = fac_[i - 1] * i;

		fac_inv_ = vm(n + 1);
		fac_inv_[n] = fac_[n].inv();
		repir(i, n - 1, 1) fac_inv_[i] = fac_inv_[i + 1] * (i + 1);
		fac_inv_[0] = 1;

		inv_ = vm(n + 1);
		repi(i, 1, n) inv_[i] = fac_[i - 1] * fac_inv_[i];
	}

	// n! を返す．O(1)
	mint factorial(int n) const { assert(0 <= n && n <= n_max); return fac_[n]; }

	// 1 / n! を返す．O(1)
	mint factorial_inv(int n) const { assert(0 <= n && n <= n_max); return fac_inv_[n]; }

	// 1 / n を返す．O(1)
	mint inv(int n) const { assert(0 < n && n <= n_max); return inv_[n]; }

	// 順列の数 nPr を返す．O(1)
	mint permutation(int n, int r) const {
		assert(n <= n_max);

		if (r < 0 || n - r < 0) return 0;
		return fac_[n] * fac_inv_[n - r];
	}

	// 二項係数 nCr を返す．O(1)
	mint binomial(int n, int r) const {
		assert(n <= n_max);

		if (r < 0 || n - r < 0) return 0;
		return fac_[n] * fac_inv_[r] * fac_inv_[n - r];
	}

	// 多項係数 nC[r] を返す．O(|r|)
	mint multinomial(const vi& r) const {
		int n = accumulate(all(r), 0);
		assert(n <= n_max);

		mint res = fac_[n];
		repe(ri, r) res *= fac_inv_[ri];

		return res;
	}
};


//【階乗と二項係数（法が小さな素数）】
/*
* Factorial_small_prime_mod(int p) : O(p)
*	p を法として初期化する．
*
* int factorial(ll n) : O(log n)
*	n! mod p を返す．
*
* int binomial(ll n, ll r) : O(log n + log p)
*	nCr mod p を返す．
*/
struct Factorial_small_prime_mod {
	int p; // 利用する法（素数）

	// 階乗の値を保持するテーブル
	using mint_p = dynamic_modint<31415>; // 他と被らなければ何でも良い．
	vector<mint_p> fac_;

	// (p-1)! までの階乗を法を p として前計算しておく．
	Factorial_small_prime_mod(int p_) : p(p_) {
		mint_p::set_mod(p);

		fac_ = vector<mint_p>(p);
		fac_[0] = 1;
		repi(i, 1, p - 1) {
			fac_[i] = fac_[i - 1] * i;
		}
	}

	pair<ll, mint_p> factorial_qr(ll n) const {
		ll pow = 0;
		mint_p mod = 1;

		// ルジャンドルの公式を用いて pow = ord_p(n!) を求めるついでに，
		// ウィルソンの定理 (p-1)! = -1 (mod p) を利用して mod も求める．
		while (n > 0) {
			ll q = n / p;
			int r = (int)(n % p);

			pow += q;
			mod *= fac_[r] * (q % 2 ? -1 : 1);

			n /= p;
		}

		return { pow, mod };
	}

	// n! mod p を返す．
	int factorial(ll n) {
		// n が p 以上なら明らかに p の倍数
		if (n >= (ll)p) {
			return 0;
		}

		// そうでなければ n! mod p を返す．
		return factorial_qr(n).second.val();
	}

	// 二項係数 nCr mod p を返す．
	int binomial(ll n, ll r) {
		if (r < 0 || n - r < 0) {
			return 0;
		}

		// n, r, n-r それぞれの pow および mod を得る．
		auto fac_n = factorial_qr(n);
		auto fac_r = factorial_qr(r);
		auto fac_nr = factorial_qr(n - r);

		// pow は加減，mod は乗除して結果を得る．
		ll pow = fac_n.first - fac_r.first - fac_nr.first;
		mint_p mod = fac_n.second / fac_r.second / fac_nr.second;

		return pow == 0 ? mod.val() : 0;
	}
};


//【階乗と二項係数（法が小さい）】
/*
* Factorial_arbitrary_small_mod(int m) : O(m)
*	m を法として初期化する．
*
* int factorial(ll n) : O(ω(m) (log n + log m))
*	n! mod m を返す．
*  （ω(m) : m の素因数の種類数）
*
* int binomial(ll n, ll r) : O(ω(m) (log n + log m))
*	nCr mod m を返す．
*/
struct Factorial_arbitrary_small_mod {
	// verify : https://judge.yosupo.jp/problem/binomial_coefficient

	// m のもつ素因数の数
	int np;

	// ps[i], ds[i], pds[i] : m の i 番目の素因数，その個数，素数冪
	vi ps, ds; vl pds;

	// fac[i][j] : [1..j] で p[i] の倍数でない数の総積 mod pd[i]
	vvl fac;

	// m を法として初期化する．
	Factorial_arbitrary_small_mod(int m) {
		// m を素因数分解する．
		for (int p = 2; p * p <= m; p++) {
			int d = 0, pd = 1;
			while (m % p == 0) {
				d++;
				pd *= p;
				m /= p;
			}
			if (d > 0) {
				ps.push_back(p);
				ds.push_back(d);
				pds.push_back(pd);
			}
		}
		if (m > 1) {
			ps.push_back(m);
			ds.push_back(1);
			pds.push_back(m);
		}
		np = sz(ps);

		// fac[i][j] を前計算する．
		fac.resize(np);
		rep(i, np) {
			fac[i].resize(pds[i]);
			fac[i][0] = 1;
			repi(j, 1, pds[i] - 1) {
				if (j % ps[i] == 0) fac[i][j] = fac[i][j - 1];
				else fac[i][j] = (fac[i][j - 1] * j) % pds[i];
			}
		}
	}

	// m の各素因数 p = ps[i] について，ord_p(n!) を pw[i] に格納し，
	// (n! / p^pw[i]) mod pds[i] を rm[i] に格納する．
	void factorial_sub(ll n_, vl& pw, vl& rm) const {
		pw = vl(np, 0); rm = vl(np, 1);

		rep(i, np) {
			// ルジャンドルの公式を用いて pw = ord_p(n!) を求める．
			ll n = n_;
			while (n > 0) {
				ll q = n / ps[i];

				pw[i] += q;

				n = q;
			}

			// ウィルソンの定理の一般化を利用して rm を求める．
			n = n_;
			while (n > 0) {
				ll q = n / pds[i], r = n % pds[i];

				rm[i] = (rm[i] * fac[i][r]) % pds[i];
				if (q % 2 == 1) rm[i] = (rm[i] * fac[i][pds[i] - 1]) % pds[i];

				n /= ps[i];
			}
		}
	}

	// n! mod m を返す．
	int factorial(ll n) const {
		assert(n >= 0);

		// n! の情報を得る．
		vl pw, rm;
		factorial_sub(n, pw, rm);

		// 情報をまとめて連立合同式を作る．
		vl rgt(np);
		rep(i, np) {
			if (pw[i] >= ds[i]) rgt[i] = 0;
			else rgt[i] = rm[i] * pow(ps[i], (int)pw[i]);
		}

		// 中国剰余定理で連立合同式の解を求める．
		return (int)crt(rgt, pds).first;
	}

	// 二項係数 nCr mod m を返す．
	int binomial(ll n, ll r) const {
		if (r < 0 || n - r < 0) return 0;

		// n, r, n-r それぞれの pow および mod を得る．
		vl pw_n, pw_r, pw_s, rm_n, rm_r, rm_s;
		factorial_sub(n, pw_n, rm_n);
		factorial_sub(r, pw_r, rm_r);
		factorial_sub(n - r, pw_s, rm_s);

		// 情報をまとめて連立合同式を作る．
		vl rgt(np);
		rep(i, np) {
			ll pw = pw_n[i] - pw_r[i] - pw_s[i];
			ll rm = rm_n[i];
			rm = (rm * inv_mod(rm_r[i], pds[i])) % pds[i];
			rm = (rm * inv_mod(rm_s[i], pds[i])) % pds[i];

			if (pw >= ds[i]) rgt[i] = 0;
			else rgt[i] = rm * pow(ps[i], (int)pw);
		}

		// 中国剰余定理で連立合同式の解を求める．
		return (int)crt(rgt, pds).first;
	}
};


//【階乗と二項係数（法が任意）】
/*
* Factorial_arbitrary_mod(int m, int n_max) : O(min(m, n_max))
*	m を法として，n_max! まで計算可能として初期化する．
*
* int factorial(int n) : O(ω(m) (log n + log m))
*	n! mod m を返す．
*  （ω(m) : m の素因数の種類数）
*
* int binomial(int n, int r) : O(ω(m) (log n + log m))
*	nCr mod m を返す．
*/
struct Factorial_arbitrary_mod {
	// verify : https://atcoder.jp/contests/arc012/tasks/arc012_4

	// n_max! までは計算可能とする．
	int n_max;

	// m のもつ素因数の数
	int np;

	// ps[i], ds[i], pds[i] : m の i 番目の素因数，その個数，素数冪
	vi ps, ds; vl pds;

	// fac[i][j] : [1..j] で p[i] の倍数でない数の総積 mod pd[i]
	vvl fac;

	// m を法として初期化する．
	Factorial_arbitrary_mod(int m, int n) : n_max(n) {
		// m を素因数分解する．
		for (int p = 2; p * p <= m; p++) {
			int d = 0, pd = 1;
			while (m % p == 0) {
				d++;
				pd *= p;
				m /= p;
			}
			if (d > 0) {
				ps.push_back(p);
				ds.push_back(d);
				pds.push_back(pd);
			}
		}
		if (m > 1) {
			ps.push_back(m);
			ds.push_back(1);
			pds.push_back(m);
		}
		np = sz(ps);

		// fac[i][j] を前計算する．
		fac.resize(np);
		rep(i, np) {
			int len = (int)min(pds[i], (ll)n_max);

			fac[i].resize(len + 1);
			fac[i][0] = 1;
			repi(j, 1, len) {
				if (j % ps[i] == 0) fac[i][j] = fac[i][j - 1];
				else fac[i][j] = (fac[i][j - 1] * j) % pds[i];
			}
		}
	}

	// m の各素因数 p = ps[i] について，ord_p(n!) を pw[i] に格納し，
	// (n! / p^pw[i]) mod pds[i] を rm[i] に格納する．
	void factorial_sub(int n_, vi& pw, vl& rm) const {
		pw = vi(np, 0); rm = vl(np, 1);

		rep(i, np) {
			// ルジャンドルの公式を用いて pw = ord_p(n!) を求める．
			int n = n_;
			while (n > 0) {
				int q = n / ps[i];

				pw[i] += q;

				n = q;
			}

			// ウィルソンの定理の一般化を利用して rm を求める．
			n = n_;
			while (n > 0) {
				int q = n / (int)pds[i], r = n % (int)pds[i];

				rm[i] = (rm[i] * fac[i][r]) % pds[i];
				if (q % 2 == 1) rm[i] = (rm[i] * fac[i][pds[i] - 1]) % pds[i];

				n /= ps[i];
			}
		}
	}

	// n! mod m を返す．
	int factorial(int n) const {
		assert(0 <= n && n <= n_max);

		// n! の情報を得る．
		vi pw; vl rm;
		factorial_sub(n, pw, rm);

		// 情報をまとめて連立合同式を作る．
		vl rgt(np);
		rep(i, np) {
			if (pw[i] >= ds[i]) rgt[i] = 0;
			else rgt[i] = rm[i] * pow(ps[i], (int)pw[i]);
		}

		// 中国剰余定理で連立合同式の解を求める．
		return (int)crt(rgt, pds).first;
	}

	// 二項係数 nCr mod m を返す．
	int binomial(int n, int r) const {
		assert(n <= n_max);

		if (r < 0 || n - r < 0) return 0;

		// n, r, n-r それぞれの pow および mod を得る．
		vi pw_n, pw_r, pw_s; vl rm_n, rm_r, rm_s;
		factorial_sub(n, pw_n, rm_n);
		factorial_sub(r, pw_r, rm_r);
		factorial_sub(n - r, pw_s, rm_s);

		// 情報をまとめて連立合同式を作る．
		vl rgt(np);
		rep(i, np) {
			ll pw = pw_n[i] - pw_r[i] - pw_s[i];
			ll rm = rm_n[i];
			rm = (rm * inv_mod(rm_r[i], pds[i])) % pds[i];
			rm = (rm * inv_mod(rm_s[i], pds[i])) % pds[i];

			if (pw >= ds[i]) rgt[i] = 0;
			else rgt[i] = rm * pow(ps[i], (int)pw);
		}

		// 中国剰余定理で連立合同式の解を求める．
		return (int)crt(rgt, pds).first;
	}
};


//【階乗】O(n)
/*
* 愚直に階乗 n! を計算する．
*/
ll factorial(int n) {
	ll val = 1;

	repi(i, 1, n) {
		val *= i;
	}

	return val;
}


//【二項係数】O(min(r, n-r))
/*
* 愚直に二項係数 nCr を計算する．
*/
template <class T> T binomial(ll n, ll r) {
	// verify : https://atcoder.jp/contests/tokiomarine2020/tasks/tokiomarine2020_e

	T val = 1;
	chmin(r, n - r);

	if (r < 0) {
		return n == 0 ? 1 : 0;
	}

	rep(i, r) {
		val *= n - i;
		val /= i + 1;
	}
	return val;
}


