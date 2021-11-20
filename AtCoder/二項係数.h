#pragma once
#include "header.h"
// ■■■■■ 階乗，二項係数 ■■■■■


//【階乗と二項係数（mint利用）】
/*
* 十分大きな素数を法として，階乗，その逆数，二項係数を計算する．
*
* factorial_mint(n) : O(n)
*	n! までの階乗とその逆数を前計算する．
*
* fac(n) : O(1)
*	n! を返す．
*
* fac_inv(n) : O(1)
*	1 / n! を返す．
*
* inv(n) : O(1)
*	1 / n を返す．
*
* permutation(n, r) : O(1)
*	順列の数 permutation を返す．
*
* binomial(n, r) : O(1)
*	二項係数 nCr を返す．
*
* multinomial(r) : O(|r|)
*	多項係数 nC[r] を返す．（n = Σr）
*/
struct factorial_mint {
	// 階乗，階乗の逆数，逆数の値を保持するテーブル
	int n_;
	vm fac_, fac_inv_, inv_;

	// n! までの階乗とその逆数を前計算しておく．O(n)
	factorial_mint(int n) : n_(n) {
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
	mint fac(int n) const { assert(n <= n_); return fac_[n]; }

	// 1 / n! を返す．O(1)
	mint fac_inv(int n) const { assert(n <= n_); return fac_inv_[n]; }

	// 1 / n を返す．O(1)
	mint inv(int n) const { assert(n != 0 && n <= n_); return inv_[n]; }

	// 順列の数 permutation を返す．O(1)
	mint permutation(int n, int r) const {
		assert(n <= n_);

		if (r < 0 || n - r < 0) return 0;
		return fac_[n] * fac_inv_[n - r];
	}

	// 二項係数 nCr を返す．O(1)
	mint binomial(int n, int r) const {
		assert(n <= n_);

		if (r < 0 || n - r < 0) return 0;
		return fac_[n] * fac_inv_[r] * fac_inv_[n - r];
	}

	// 多項係数 nC[r] を返す．O(|r|)
	mint multinomial(const vi& r) const {
		int n = accumulate(all(r), 0);
		assert(n <= n_);

		mint res = fac_[n];
		repe(ri, r) res *= fac_inv_[ri];

		return res;
	}
};


//【階乗と二項係数】
/*
* 小さな素数 p を法とし，階乗と二項係数を計算する．
*
* factorial_mod(p) : O(p)
*	(p-1)! までの階乗を法を p として前計算する．
*
* fac(n) : O(n log n)
*	n! mod p を返す．
*
* factorial_qr(n) : O(n log n)
*	n! が p で割り切れる回数と p で割った余りを返す．
*
* binomial(n, r) : O(n log n log p)
*	nCr mod p を返す．
*/
struct factorial_mod {
	int p; // 利用する法（素数）

	// 階乗とその逆数の値を保持するテーブル
	// 全ての値 v は (pow, mod) の形で保持する．
	// pow : v が p で何回割り切れるか．
	// mod : v / (p^pow) を p で割った余り
	using mint_p = dynamic_modint<31415>; // 他と被らなければ何でも良い．
	vector<mint_p> fac_;

	// (p-1)! までの階乗を法を p として前計算しておく．
	factorial_mod(int p_) : p(p_) {
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
	int fac(ll n) {
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


//【二項係数】O(min(r, n-r))
/*
* 愚直に二項係数 nCr を計算する．
*/
template <class T> T binomial(ll n, ll r) {
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


//【階乗】O(n)
/*
* 愚直に階乗 n! を計算する．
*/
ll fac(int n) {
	ll val = 1;

	repi(i, 1, n) {
		val *= i;
	}

	return val;
}


