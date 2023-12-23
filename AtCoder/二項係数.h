#pragma once
#include "header.h"
#include "列クエリ.h"
// ■■■■■ 階乗，二項係数など ■■■■■


//【階乗など（法が大きな素数）】
/*
* Factorial_mint(int N) : O(n)
*	N まで計算可能として初期化する．
*
* mint fact(int n) : O(1)
*	n! を返す．
*
* mint fact_inv(int n) : O(1)
*	1/n! を返す（n が負なら 0 を返す）
*
* mint inv(int n) : O(1)
*	1/n を返す．
*
* mint perm(int n, int r) : O(1)
*	順列の数 nPr を返す．
*
* mint bin(int n, int r) : O(1)
*	二項係数 nCr を返す．
*
* mint mul(vi rs) : O(|rs|)
*	多項係数 nC[rs] を返す．（n = Σrs）
*/
class Factorial_mint {
	int n_max;
	
	// 階乗と階乗の逆数の値を保持するテーブル
	vm fac, fac_inv;

public:
	// n! までの階乗とその逆数を前計算しておく．O(n)
	Factorial_mint(int n) : n_max(n), fac(n + 1), fac_inv(n + 1) {
		// verify : https://atcoder.jp/contests/dwacon6th-prelims/tasks/dwacon6th_prelims_b

		fac[0] = 1;
		repi(i, 1, n) fac[i] = fac[i - 1] * i;

		fac_inv[n] = fac[n].inv();
		repir(i, n - 1, 0) fac_inv[i] = fac_inv[i + 1] * (i + 1);
	}
	Factorial_mint() : n_max(0) {} // ダミー

	// n! を返す．
	mint fact(int n) const {
		// verify : https://atcoder.jp/contests/dwacon6th-prelims/tasks/dwacon6th_prelims_b

		Assert(0 <= n && n <= n_max);
		return fac[n];
	}

	// 1/n! を返す（n が負なら 0 を返す）
	mint fact_inv(int n) const {
		// verify : https://atcoder.jp/contests/abc289/tasks/abc289_h

		Assert(n <= n_max);
		if (n < 0) return 0;
		return fac_inv[n];
	}

	// 1/n を返す．
	mint inv(int n) const {
		// verify : https://atcoder.jp/contests/exawizards2019/tasks/exawizards2019_d

		Assert(0 < n && n <= n_max);
		return fac[n - 1] * fac_inv[n];
	}

	// 順列の数 nPr を返す．
	mint perm(int n, int r) const {
		// verify : https://atcoder.jp/contests/abc172/tasks/abc172_e
		
		Assert(n <= n_max);

		if (r < 0 || n - r < 0) return 0;
		return fac[n] * fac_inv[n - r];
	}

	// 二項係数 nCr を返す．
	mint bin(int n, int r) const {
		// verify : https://atcoder.jp/contests/abc034/tasks/abc034_c

		Assert(n <= n_max);
		if (r < 0 || n - r < 0) return 0;
		return fac[n] * fac_inv[r] * fac_inv[n - r];
	}

	// 多項係数 nC[rs] を返す．
	mint mul(const vi& rs) const {
		// verify : https://yukicoder.me/problems/no/2141

		if (*min_element(all(rs)) < 0) return 0;
		int n = accumulate(all(rs), 0);
		Assert(n <= n_max);

		mint res = fac[n];
		repe(r, rs) res *= fac_inv[r];

		return res;
	}
};


//【階乗など（法が小さな素数）】
/*
* Factorial_small_prime_mod(int p, ll N = INFL) : O(min(N, p))
*	素数 p を法として，N! まで計算可能として初期化する．
*
* int fact(ll n) : O(log n)
*	n! mod p を返す．
*
* int bin(ll n, ll r) : O(log n + log p)
*	nCr mod p を返す．
*
* mint mul(vi rs) : O(|rs|)
*	多項係数 nC[rs] mod p を返す．（n = Σrs）
*/
struct Factorial_small_prime_mod {
	int p;

	// 階乗の値を保持するテーブル
	using mint_p = dynamic_modint<5362894>;
	vector<mint_p> fac;

	// (p-1)! までの階乗を法を p として前計算しておく．
	Factorial_small_prime_mod(int p, ll n_max = INFL) : p(p) {
		// verify : https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod

		mint_p::set_mod(p);
		int len = (p <= n_max ? p : (int)n_max + 1);
		fac.resize(len);
		fac[0] = 1;
		repi(i, 1, len - 1) fac[i] = fac[i - 1] * i;
	}
	Factorial_small_prime_mod() : p(0) {}

	pair<ll, mint_p> factorial_qr(ll n) const {
		ll pow = 0; mint_p mod = 1;

		// ルジャンドルの公式を用いて pow = ord_p(n!) を求めるついでに，
		// ウィルソンの定理 (p-1)! = -1 (mod p) を利用して mod も求める．
		while (n > 0) {
			ll q = n / p;
			int r = (int)(n % p);

			pow += q;
			mod *= fac[r] * (q % 2 ? -1 : 1);

			n /= p;
		}

		return { pow, mod };
	}

	// n! mod p を返す．
	int fact(ll n) {
		// n が p 以上なら明らかに p の倍数
		if (n >= (ll)p) return 0;

		// そうでなければ n! mod p を返す．
		return factorial_qr(n).second.val();
	}

	// 二項係数 nCr mod p を返す．
	int bin(ll n, ll r) {
		// verify : https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod

		if (r < 0 || n - r < 0) return 0;

		// n, r, n-r それぞれの pow および mod を得る．
		auto fac_n = factorial_qr(n);
		auto fac_r = factorial_qr(r);
		auto fac_nr = factorial_qr(n - r);

		// pow は加減，mod は乗除して結果を得る．
		ll pow = fac_n.first - (fac_r.first + fac_nr.first);
		if (pow > 0) return 0;

		mint_p mod = fac_n.second / (fac_r.second * fac_nr.second);
		return mod.val();
	}

	// 多項係数 nC[rs] を返す．
	int mul(const vi& rs) const {
		if (*min_element(all(rs)) < 0) return 0;
		ll n = accumulate(all(rs), 0);

		auto num = factorial_qr(n);
		ll dnm_pow = 0; mint_p dnm_mod = 1;
		repe(r, rs) {
			auto dnm = factorial_qr(r);
			dnm_pow += dnm.first, dnm_mod *= dnm.second;
		}

		ll pow = num.first - dnm_pow;
		if (pow > 0) return 0;

		mint_p mod = num.second / dnm_mod;
		return mod.val();
	}
};


//【階乗など（法が小さい）】
/*
* Factorial_arbitrary_small_mod(int m) : O(m)
*	m を法として初期化する．
*
* int fact(ll n) : O(ω(m) (log n + log m))
*	n! mod m を返す．
*  （ω(m) : m の素因数の種類数）
*
* int bin(ll n, ll r) : O(ω(m) (log n + log m))
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
	int fact(ll n) const {
		Assert(n >= 0);

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
	int bin(ll n, ll r) const {
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


//【階乗など（法が任意）】
/*
* Factorial_arbitrary_mod(int m, int N) : O(min(m, N))
*	m を法として，N! まで計算可能として初期化する．
*
* int fact(int n) : O(ω(m) (log n + log m))
*	n! mod m を返す．
*  （ω(m) : m の素因数の種類数）
*
* int bin(int n, int r) : O(ω(m) (log n + log m))
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
	int fact(int n) const {
		Assert(0 <= n && n <= n_max);

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
	int bin(int n, int r) const {
		Assert(n <= n_max);

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


//【階乗など（対数）】
/*
* Factorial_log<D>(int N) : O(N)
*	N! まで計算可能として初期化する．
*
* D fact(int n) : O(1)
*	log n! を返す．
*
* D perm(int n, int r) : O(1)
*	順列の数の対数 log nPr を返す．
*
* D bin(int n, int r) : O(1)
*	二項係数の対数 log nCr を返す．
*
* D mul(vi rs) : O(|rs|)
*	多項係数の対数 log nC[rs] を返す．（n = Σrs）
*/
template <class D = double>
class Factorial_log {
	// 階乗，階乗の逆数，逆数の値を保持するテーブル
	int n_max;
	vector<D> fac;

public:
	// n! までの階乗とその逆数を前計算しておく．O(n)
	Factorial_log(int n) : n_max(n) {
		// verify : https://atcoder.jp/contests/arc035/tasks/arc035_d

		fac.resize(n + 1);
		fac[0] = 0;
		repi(i, 1, n) fac[i] = fac[i - 1] + log(i);
	}
	Factorial_log() : n_max(0) {} // ダミー

	// log n! を返す．O(1)
	D fact(int n) const {
		Assert(0 <= n && n <= n_max);
		return fac[n];
	}

	// 順列の数の対数 log nPr を返す．O(1)
	D perm(int n, int r) const {
		Assert(n <= n_max);
		if (r < 0 || n - r < 0) return -D(INFL);
		return fac[n] - fac[n - r];
	}

	// 二項係数の対数 log nCr を返す．O(1)
	D bin(int n, int r) const {
		// verify : https://atcoder.jp/contests/arc035/tasks/arc035_d

		Assert(n <= n_max);
		if (r < 0 || n - r < 0) return -D(INFL);
		return fac[n] - fac[r] - fac[n - r];
	}

	// 多項係数の対数 log nC[rs] を返す．O(|rs|)
	D mul(const vi& rs) const {
		if (*min_element(all(rs)) < 0) return -D(INFL);
		int n = accumulate(all(rs), 0);
		Assert(n <= n_max);

		double res = fac[n];
		repe(i, rs) res -= fac[i];

		return res;
	}
};


//【階乗（n が小さい）】O(n)
/*
* n! を返す．
*/
template <class T>
T fact(int n) {
	// verify : https://atcoder.jp/contests/arc106/tasks/arc106_f

	T val = 1;
	repi(i, 1, n) val *= i;
	return val;
}


//【順列の数（r が小さい）】O(r)
/*
* nPr を返す．
*/
template <class T>
T perm(ll n, int r) {
	// verify : https://mojacoder.app/users/milkcoffee/contests/milkcoffee-contest-001/tasks/3

	if (r < 0 || n - r < 0) return 0;

	T val = 1;
	rep(i, r) val *= n - i;
	return val;
}


//【二項係数（r か n-r が小さい）】O(min(r, n-r))
/*
* nCr を返す．
*/
ll bin(ll n, ll r) {
	// verify : https://atcoder.jp/contests/arc106/tasks/arc106_f

	Assert(n >= 0);

	ll val = 1;
	chmin(r, n - r);

	if (r < 0) return 0;

	rep(i, r) {
		val *= n - i;
		val /= i + 1;
	}
	return val;
}


//【二項係数（r か n-r が小さい）】O(min(r, n-r))
/*
* nCr を返す．
*/
mint bin_mint(ll n, ll r) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_ad

	Assert(n >= 0);

	mint num = 1, dnm = 1;
	chmin(r, n - r);

	if (r < 0) return 0;

	rep(i, r) {
		num *= n - i;
		dnm *= i + 1;
	}
	return num / dnm;
}


//【二項係数（mod 2）】O(1)
/*
* nCr mod 2 を返す．
*/
template <class T>
int bin2(T n, T r) {
	// verify : https://atcoder.jp/contests/tenka1-2014-qualb/tasks/tenka1_2014_qualB_c

	if (r < 0 || n < r) return 0;
	return (n & r) == r;
}


//【二項係数のくくり出し】
/*
* bin(n, r) = (n/r) bin(n-1, r-1)
*/


//【リュカの定理】
/*
* p を素数とし，n, r が p 進表記で
*	n = [n[0], n[1], ..., n[k-1]]_(p)
*	r = [r[0], r[1], ..., r[k-1]]_(p)
* と表されるとき，以下の合同式が成り立つ：
*	bin(n, r) ≡ Πi∈[0..k) bin(n[i], r[i])  (mod p)
*
* 特に p = 2 のときは以下の合同式が成り立つ：
*	bin(n, r) ≡ Boole[(n & r) == r]  (mod 2)
* 
* verify : https://mojacoder.app/users/YSatUT/problems/divisible_nCr
*/


//【クンマーの定理】
/*
* 多項係数 mul(n, rs)（n=Σrs）がもつ素因数 p の個数は，
* rs 全てを p 進表記で加算したときの繰り上がり回数に等しい．
* 
* 正確には，r の p 進表記での数字和を s_p(r) と表すとき，以下の等式が成り立つ：
*	ord_p mul(n, rs) = (Σi s_p(r[i]) - s_p(n)) / (p-1)
* 
* verify : https://projecteuler.net/problem=154
*/


//【二項係数（一括，n が固定，r が小さい，法が大きな素数）】O(r)
/*
* 各 i∈[0..r] について bin(n, i) を格納したリストを返す．
* 
* 制約：r < mint::mod
*/
vm binomial_fixed_n(ll n, int r) {
	// verify : https://atcoder.jp/contests/arc144/tasks/arc144_d

	Assert(n >= 0);

	// perm[i] : nPi
	vm perm(r + 1);
	perm[0] = 1;
	repi(i, 1, r) perm[i] = perm[i - 1] * (n + 1 - i);

	// fac[i] : i!
	vm fac(r + 1);
	fac[0] = 1;
	repi(i, 1, r) fac[i] = fac[i - 1] * i;

	// fac_inv[i] : 1 / i!
	vm fac_inv(r + 1);
	fac_inv[r] = fac[r].inv(); // mint の法は r より大きくないといけない
	repir(i, r - 1, 0) fac_inv[i] = fac_inv[i + 1] * (i + 1);

	// bin(n, i) = nPi / i!
	vm bin(r + 1);
	repi(i, 0, r) bin[i] = perm[i] * fac_inv[i];

	return bin;
}


//【二項係数（一括，r が固定，n が小さい，法が大きな素数）】O(n - r)
/*
* 各 i∈[r..n] について bin(i, r) を順に格納したリストを返す．
*/
vm binomial_fixed_r(int n, int r) {
	// verify : https://yukicoder.me/problems/no/1102

	// fac[i] : i!
	vm fac(n - r + 1);
	fac[0] = 1;
	repi(i, 1, n - r) fac[i] = fac[i - 1] * i;

	// fac_inv : 1 / (n - r)!
	mint fac_inv = fac[n - r].inv();

	// inv[i] : 1 / i
	vm inv(n - r + 1);
	repir(i, n - r, 1) {
		inv[i] = fac[i - 1] * fac_inv;
		fac_inv *= i;
	}

	// bin[i] : bin(r + i, r)
	vm bin(n - r + 1);
	bin[0] = 1;
	repi(i, r + 1, n) bin[i - r] = bin[i - 1 - r] * i * inv[i - r];

	return bin;
}


//【二項係数（一括，r が固定で小さい，法が大きな素数）】O((n2 - n1) + r)
/*
* 各 i∈[n1..n2) について bin(i, r) を順に格納したリストを返す．
*/
vm binomial_fixed_r(ll n1, ll n2, int r) {
	// verify : https://atcoder.jp/contests/arc144/tasks/arc144_d

	Assert(n1 >= 0 && n1 <= n2);

	int dn = (int)(n2 - n1);
	if (dn == 0) return vm();
	vm bin(dn);

	// p[i] : n2Pi（ただし mint::mod の倍数は 1 に置き換える）
	vm p(dn + r + 1);
	p[0] = 1;
	repi(i, 1, dn + r) {
		mint mul = n2 + 1 - i;
		if (mul == 0) mul = 1;

		p[i] = p[i - 1] * mul;
	}

	// p_inv[i] : 1 / n2Pi（ただし mint::mod の倍数は 1 に置き換える）
	vm p_inv(dn + r + 1);
	p_inv[dn + r] = p[dn + r].inv();
	repir(i, dn + r - 1, 0) {
		mint mul = n2 - i;
		if (mul == 0) mul = 1;

		p_inv[i] = p_inv[i + 1] * mul;
	}

	// perm[i] : (n1+i)Pr = (n2-dn+i)Pr = n2P(r+dn-i) / n2P(dn-i)
	vm perm(dn);
	rep(i, dn) {
		// [n1+i..n1+i-r) に mint::mod の倍数が含まれているところは 0 にする．
		if (mint(n1 + i).val() >= mint(n1 + i - r).val()) {
			perm[i] = p[r + dn - i] * p_inv[dn - i];
		}
	}

	// fac : r!
	mint fac = 1;
	repi(i, 1, r) fac *= i;

	// fac_inv : 1 / r!
	mint fac_inv = fac.inv();

	// bin(i, r) = iPr / r!
	rep(i, dn) bin[i] = perm[i] * fac_inv;

	return bin;
}


//【二項係数（一括，n-r が固定，r が小さい，法が大きな素数）】O(r)
/*
* 各 i∈[0..r) について bin(n+i, i) を順に格納したリストを返す．
*/
vm binomial_fixed_diff(ll n, int r) {
	// verify : https://yukicoder.me/problems/no/1489

	//【方法】
	// bin(n+i, i) = Π[n+1..n+i] / i! なので，
	// 分子を累積積で計算していき，最後に階乗の逆数を掛ければ良い．

	if (r <= 0) return vm();

	// bin[i] : bin(n+i, i)
	// fac : (r-1)!
	vm bin(r); mint fac = 1;
	bin[0] = 1;
	repi(i, 1, r - 1) {
		bin[i] = bin[i - 1] * (n + i);
		fac *= i;
	}

	// fac_inv : 1/(r-1)!
	mint fac_inv = fac.inv();

	repir(i, r - 1, 1) {
		bin[i] *= fac_inv;
		fac_inv *= i;
	}

	return bin;
}


//【二項係数（一括，n が固定，法が小さな素数）】O((r2 - r1) + p^2 + log n) (?)
/*
* 各 i∈[r1..r2) について bin(n, i) mod p を順に格納したリストを返す．
*
* 制約：p は素数
*/
vi binomial_fixed_n(ll n, ll r1, ll r2, int p) {
	// verify : https://atcoder.jp/contests/abc251/tasks/abc251_h

	r2--; // 閉区間 [r1..r2] にする．
	vi bin;

	vvi bin_sml(p, vi(p)); // bin_sml[i][j] : binomial(i, j) mod p
	bin_sml[0][0] = 1;
	repi(i, 1, p - 1) {
		repi(j, 0, i) {
			if (j > 0) bin_sml[i][j] += bin_sml[i - 1][j - 1];
			if (j < i) bin_sml[i][j] += bin_sml[i - 1][j];
			bin_sml[i][j] %= p;
		}
	}

	// dn, dr1, dr2 : n, r1, r2 の p 進表示の桁の数（下位から順）
	vi dn, dr1, dr2; ll n_ = n, r1_ = r1, r2_ = r2;
	while (n_ > 0) {
		dn.push_back((int)(n_ % p));
		dr1.push_back((int)(r1_ % p));
		dr2.push_back((int)(r2_ % p));
		n_ /= p;
		r1_ /= p;
		r2_ /= p;
	}
	int k = sz(dn);

	vvi dp(k + 1);
	dp[0] = { 1 };

	function<void(int, bool, bool, int)> rfunc = [&](int b, bool lf, bool rf, int mul) {
		if (b == -1) {
			bin.push_back(mul);
			return;
		}

		if (lf) {
			// 左右端ともに浮いていないとき
			if (rf) {
				if (!dp[b + 1].empty()) {
					repe(v, dp[b + 1]) {
						bin.push_back((v * mul) % p);
					}
				}
				else {
					rep(i, p) {
						rfunc(b - 1, true, true, (mul * bin_sml[dn[b]][i]) % p);
					}

					int m = (int)pow(p, b + 1);
					dp[b + 1].resize(m);
					rep(i, p) {
						rep(j, m / p) {
							dp[b + 1][i * m / p + j] = (dp[b][j] * bin_sml[dn[b]][i]) % p;
						}
					}
				}
			}
			// 右端のみ浮いているとき
			else {
				repi(i, 0, dr2[b] - 1) {
					rfunc(b - 1, true, true, (mul * bin_sml[dn[b]][i]) % p);
				}
				rfunc(b - 1, true, false, (mul * bin_sml[dn[b]][dr2[b]]) % p);
			}
		}
		else {
			// 左端のみ浮いているとき
			if (rf) {
				rfunc(b - 1, false, true, (mul * bin_sml[dn[b]][dr1[b]]) % p);
				repi(i, dr1[b] + 1, p - 1) {
					rfunc(b - 1, true, true, (mul * bin_sml[dn[b]][i]) % p);
				}
			}
			// 左右端ともに浮いているとき
			else {
				if (dr1[b] == dr2[b]) {
					rfunc(b - 1, false, false, (mul * bin_sml[dn[b]][dr1[b]]) % p);
				}
				else {
					rfunc(b - 1, false, true, (mul * bin_sml[dn[b]][dr1[b]]) % p);
					repi(i, dr1[b] + 1, dr2[b] - 1) {
						rfunc(b - 1, true, true, (mul * bin_sml[dn[b]][i]) % p);
					}
					rfunc(b - 1, true, false, (mul * bin_sml[dn[b]][dr2[b]]) % p);
				}
			}
		}
	};

	rfunc(k - 1, false, false, 1);

	return bin;
}


//【二項係数の累積和（法が大きな素数）】
/*
* Binomial_sum_mint(int n) : O(n√n)
*	bin(n, -) まで計算可能として初期化する．
*
* get(int n, int r) : O(√n)
*	Σbin[n][0..r) を返す．
*
* 利用：【階乗など（法が大きな素数）】
*/
class Binomial_sum_mint {
	// 参考 : https://yukicoder.me/problems/no/2206/editorial

	//【方法】
	// S(n, r) := Σj∈[0..r) bin(n, j) とおくと，
	//		S(n+1, r) = 2 S(n, r) - bin(n, r-1)
	// が成り立つ．
	// よって適当な間隔の n について S(n, r) 全てを前計算しておけば途中から計算を始められる．
	//
	// なお，他の漸化式
	//		S(n-1, r) = (S(n, r) + bin(n-1, r-1)) / 2
	//		S(n, r+1) = S(n, r) + bin(n, r)
	//		S(n, r-1) = S(n, r) - bin(n, r-1)
	// も使えば，記憶量を減らしたり，1/2 倍高速化したり，Mo's に乗せたりできる．

	int N, M;

	// S[i][r] : Σj∈[0..r) bin(M i, j)
	vvm S;

	Factorial_mint fm;

public:
	// bin(n, -) まで計算可能として初期化する．
	Binomial_sum_mint(int n) : N(n), M((int)(sqrt(N) + 0.01)), S(N / M + 1), fm(N) {
		// verify : https://yukicoder.me/problems/no/2206

		repi(i, 0, N / M) {
			S[i].resize(M * i + 2);
			repi(j, 0, M * i) S[i][j + 1] = S[i][j] + fm.bin(M * i, j);
		}
	}
	Binomial_sum_mint() : N(0), M(0) {}

	// Σbin[n][0..r) を返す．
	mint get(int n, int r) {
		// verify : https://yukicoder.me/problems/no/2206

		Assert(n <= N);
		chmax(r, 0);

		int i0 = n / M;
		mint res = S[i0][min(r, sz(S[i0]) - 1)];
		repi(i, i0 * M + 1, n) res = 2 * res - fm.bin(i - 1, r - 1);

		return res;
	}
};


//【二項係数の累積和（法が小さな奇素数）】
/*
* Binomial_sum_small_prime_mod(int p) : O(p^2)
*	法を p として初期化する．
*
* get(ll n, ll r) : O(log n)
*	Σbin[n][0..r) mod p を返す．
* 
* 制約：p は奇素数
*/
class Binomial_sum_small_prime_mod {
	// verify : https://atcoder.jp/contests/abc251/tasks/abc251_h

	int p; // 法となる素数
	vvi bin; // bin[i][j] : binomial(i, j)
	vvi acc; // acc[i][j] : Σbin[i][0..j)
	vi pow2; // pow2[i] : 2^i

public:
	// p を法として初期化する． : O(p^2)
	Binomial_sum_small_prime_mod(int p_) : p(p_) {
		bin = vvi(p, vi(p));
		acc = vvi(p, vi(p + 1));
		pow2.resize(p - 1);

		bin[0][0] = 1;
		repi(i, 1, p - 1) repi(j, 0, i) {
			if (j > 0) bin[i][j] += bin[i - 1][j - 1];
			if (j < i) bin[i][j] += bin[i - 1][j];
			bin[i][j] %= p;
		}

		rep(i, p) rep(j, p) {
			acc[i][j + 1] = acc[i][j] + bin[i][j];
			acc[i][j + 1] %= p;
		}

		pow2[0] = 1;
		repi(i, 1, p - 2) pow2[i] = (pow2[i - 1] * 2) % p;
	}
	Binomial_sum_small_prime_mod() : p(0) {} // ダミー

	// Σbin[n][0..r) を返す． : O(log n)
	int get(ll n, ll r) {
		if (n == 0) return (int)(r > 0);
		if (r <= 0) return 0;
		if (r > n) return pow2[n % (p - 1)];

		// dn, dr : n, r の p 進表示の桁の数（上位から順）
		vi dn, dr; ll n_ = n, r_ = r;
		while (n_ > 0) {
			dn.push_back((int)(n_ % p));
			dr.push_back((int)(r_ % p));
			n_ /= p;
			r_ /= p;
		}
		reverse(all(dn));
		reverse(all(dr));
		int k = sz(dn);

		int res = 0, mul = 1;
		rep(i, k) {
			n -= dn[i] * pow(p, k - 1 - i);
			res += acc[dn[i]][dr[i]] * pow2[n % (p - 1)] * mul;
			mul *= bin[dn[i]][dr[i]];
			res %= p;
			mul %= p;
		}

		return res;
	}
};


//【二項係数と累積和】
/*
* 二項係数は列 1, 0, 0, ... に対して累積和を繰り返しとったものとみなせる．
*             i: 0  1  2  3  4  5  6  7 ...
*	bin(i-1,-1): 1  0  0  0  0  0  0  0 ...
*	bin(i+0, 0): 1  1  1  1  1  1  1  1 ...
*	bin(i+1, 1): 1  2  3  4  5  6  7  8 ...
*	bin(i+2, 2): 1  3  6 10 15 21 28 36 ...
* 
* verify : https://atcoder.jp/contests/abc256/tasks/abc256_f
*/


//【二項係数の累積和（一括，r が固定）】O(n)
/*
* 各 i∈[0..n) について Σj∈[0..r) bin(i, j) を格納したリストを返す．
*
* 制約：fm は (n-1)! まで計算可能
*/
vm binomial_sum_fixed_r(int n, int r, const Factorial_mint& fm) {
	// verify : https://www.codechef.com/problems/MEX_SEQ

	//【方法】
	// 大体 2 倍でいいが，パスカルの三角形で右にはみ出た分だけ引き算する．

	vm res(n);
	if (n == 0 || r <= 0) return res;

	res[0] = 1;
	repi(i, 1, n - 1) res[i] = res[i - 1] * 2 - fm.bin(i - 1, r - 1);

	return res;
}


//【ホッケースティック恒等式】
/*
* パスカルの三角形の斜め方向の累積和は以下の式で求められる：
*	Σi=[i0..i1) bin(i, r) = bin(i1, r+1) - bin(i0, r+1)
* 
* verify : https://atcoder.jp/contests/abc154/tasks/abc154_f
*/


//【1 つ抜きホッケースティック和（法が大きな素数）】
/*
* Hockey_stick_thinning_sum_mint(int n) : O(n√n)
*	bin(n, -) まで計算可能として初期化する．
*
* get(int n, int r) : O(√n)
*	Σ_i≧0 bin(n-2i, r) を返す．
*
* 利用：【階乗など（法が大きな素数）】
*/
class Hockey_stick_thinning_sum_mint {
	//【方法】
	// S(n, r) := Σ_i≧0 bin(n-2i, r) とおくと，
	//		S(n, r) = 1/2 (S(n-1, r-1) + bin(n+1, r+1))
	// が成り立つ．
	// よって適当な間隔の r について S(n, r) 全てを前計算しておけば途中から計算を始められる．

	int N, M;

	// S[j][n] : Σ_i≧0 bin(2n-2i, M j)
	vvm S;

	Factorial_mint fm;

public:
	// bin(n, -) まで計算可能として初期化する．
	Hockey_stick_thinning_sum_mint(int n) : N(n), M((int)(sqrt(N) + 0.01)), S(N / M + 1, vm(N / 2 + 1)), fm(max(N + 1, 2)) {
		// verify : https://yukicoder.me/problems/no/2512

		repi(j, 0, N / M) {
			int r = j * M;
			S[j][0] = fm.bin(0, r);
			repi(i, 1, N / 2) S[j][i] = S[j][i - 1] + fm.bin(2 * i, r);
		}
	}
	Hockey_stick_thinning_sum_mint() : N(0), M(0) {}

	// Σ_i≧0 bin(n-2i, r) を返す．
	mint get(int n, int r) {
		// verify : https://yukicoder.me/problems/no/2512

		Assert(n <= N);
		if (n < 0 || r < 0 || n < r) return 0;

		int j0 = r / M, k0 = r % M;

		mint res = S[j0][(n - k0) / 2];
		if ((n - k0) & 1) res = fm.bin(n - k0 + 1, M * j0 + 1) - res;

		repir(k, k0 - 1, 0) res = (res + fm.bin(n + 1 - k, r + 1 - k)) * fm.inv(2);

		return res;
	}
};


//【二項係数の線形加重和】
/*
* 二項係数と一次式の積の和について，以下の式が成り立つ：
*	Σr=[0..n] (a r + b) bin(n, r) = (a n + 2 b) 2^(n-1)
* 
* verify : https://atcoder.jp/contests/abc150/tasks/abc150_e
*/


//【二項係数の二次元累積和（菱形領域）】
/*
* パスカルの三角形における菱形領域の二次元累積和は以下の式で求められる：
*	Σi∈[i0..i1)Σj∈[j0..j1) bin(i + j, i)
*	= bin(i1 + j1, i1) - bin(i1 + j0, i1) - bin(i0 + j1, i0) + bin(i0 + j0, i0)
*
* verify : https://atcoder.jp/contests/abc154/tasks/abc154_f
*/


//【半整数の階乗】
/*
* n ≧ 0 のとき，以下の式が成り立つ：
*	(n + 1/2)!
*	= ((2n+1)!! / 2^(n+1)) √π
*	= ((2n+1)! / (2^(2n+1) n!)) √π
* 
* また n ≧ 2 のとき，以下の式が成り立つ：
*	(1/2 - n)!
*	= ((-2)^(n-1) / (2n-3)!!) √π
*	= ((-1)^(n-1) 2^(2n-3) (n-2)! / (2n-3)!) √π
* 
* 例外は，(-1/2)! = √π
* 
* verify : https://atcoder.jp/contests/abc290/tasks/abc290_f
*/


//【二重階乗（mint 利用）】
/*
* Double_factorial(int N) : O(N)
*	N!! まで計算可能として初期化する．
*
* mint dfact(int i) : O(1)
*	i!! を返す．
*
* mint dfact_inv(int i) : O(1)
*	1/i!! を返す（n≦-2 なら 0 を返す）
*/
class Double_factorial_mint {
	int n_max;
	vm dfac, dfac_inv;

public:
	Double_factorial_mint(int n) : n_max(max(n, 1)), dfac(n_max + 1), dfac_inv(n_max + 1) {
		// verify : https://atcoder.jp/contests/agc053/tasks/agc053_c

		dfac[0] = dfac[1] = 1;
		repi(i, 2, n) dfac[i] = dfac[i - 2] * i;

		dfac_inv[n] = dfac[n].inv();
		dfac_inv[n - 1] = dfac[n - 1].inv();
		repir(i, n - 2, 1) dfac_inv[i] = dfac_inv[i + 2] * (i + 2);
	};
	Double_factorial_mint() : n_max(0) {};

	// i!! を返す．
	mint dfact(int i) const {
		// verify : https://atcoder.jp/contests/abl/tasks/abl_f

		Assert(i <= n_max);
		return i <= 0 ? 1 : dfac[i];
	}

	// 1/i!! を返す．
	mint dfact_inv(int i) const {
		// verify : https://atcoder.jp/contests/agc053/tasks/agc053_c

		Assert(i <= n_max);
		return i <= -2 ? 0 : (i <= 0 ? 1 : dfac_inv[i]);
	}
};


//【順列の数（一括，r が固定で小さい）】O(n2 - n1 + r)
/*
* 各 n∈[n1..n2) について nPr を格納したリストを返す．
*
* 利用：【キュー（モノイド）】
*/
mint op_pfr(mint x, mint y) { return x * y; }
mint e_pfr() { return 1; }
vm perm_fixed_r(ll n1, ll n2, int r) {
	// verify : https://atcoder.jp/contests/arc160/tasks/arc160_d

	//【方法】
	// perm(n1, r) = Π[n1-r+1..n1] であり，n を増やすと区間が右に 1 つずれる．
	// mint の積はモノイドを成すので，これらは SWAG で効率的に計算できる．

	if (n1 >= n2) return vm();
	if (r == 0) return vm(n2 - n1, 1);

	Queue_SWAG<mint, op_pfr, e_pfr> SWAG;
	for (ll i = n1 - r + 1; i < n1; i++) SWAG.push(i);

	vm res(n2 - n1);
	for (ll n = n1; n < n2; n++) {
		SWAG.push(n);
		res[n - n1] = SWAG.prod();
		SWAG.pop();
	}

	return res;
}


//【順列の数の和】
/*
* 順列の数の累積和は以下の式で求められる：
*	Σi=[r..n) perm(i, r) = perm(n, r) (n-r) / (r+1)
*
* verify : https://yukicoder.me/problems/no/1886
*/


//【q-階乗など】
/*
* Q_Factorial_mint(int N, int q) : O(N)
*	[N]_q まで計算可能として初期化する．
*
* mint val(int n) : O(1)
*	q-数 [n]_q を返す．
*	[n]_q = (1 - q^n) / (1 - q)
*
* mint fact(int n) : O(1)
*	q-階乗 [n]_q ! を返す．
*	[n]_q ! = (Πi∈[1..n] (1 - q^i)) / (1 - q)^n
*
* mint fact_inv(int n) : O(1)
*	q-階乗の逆数 1/([n]_q !) を返す（n が負なら 0 を返す）
*
* mint inv(int n) : O(1)
*	q-数の逆数 1/[n]_q を返す．
*
* mint bin(int n, int r) : O(1)
*	q-二項係数 nCr_q を返す．
*	nCr_q = (Πi∈[n-k+1..n] (1 - q^i)) / (Πi∈[1..k] (1 - q^i))
*
* mint mul(vi rs) : O(|rs|)
*	q-多項係数 nC[rs]_q を返す．（n = Σrs）
*
* int get_q() : O(1)
*	q を返す．
*/
class Q_Factorial_mint {
	int n_max, q;

	// q-数，階乗，階乗の逆数の値を保持するテーブル
	vm q_num, fac, fac_inv;

public:
	// n! までの階乗とその逆数を前計算しておく．O(n)
	Q_Factorial_mint(int n, int q) : n_max(n), q(q), q_num(n + 1), fac(n + 1), fac_inv(n + 1) {
		mint q_pow = 1;
		repi(i, 1, n) {
			q_num[i] = q_num[i - 1] + q_pow;
			q_pow *= q;
		}

		fac[0] = 1;
		repi(i, 1, n) fac[i] = fac[i - 1] * q_num[i];

		fac_inv[n] = fac[n].inv();
		repir(i, n - 1, 0) fac_inv[i] = fac_inv[i + 1] * q_num[i + 1];
	}
	Q_Factorial_mint() : n_max(0), q(1) {} // ダミー

	// q-数 [n]_q を返す．
	mint val(int n) const {
		Assert(0 <= n && n <= n_max);
		return q_num[n];
	}

	// n! を返す．
	mint fact(int n) const {
		// verify : https://atcoder.jp/contests/abc278/tasks/abc278_h

		Assert(0 <= n && n <= n_max);
		return fac[n];
	}

	// 1/n! を返す（n が負なら 0 を返す）
	mint fact_inv(int n) const {
		// verify : https://atcoder.jp/contests/abc278/tasks/abc278_h

		Assert(n <= n_max);
		if (n < 0) return 0;
		return fac_inv[n];
	}

	// 1/n を返す．
	mint inv(int n) const {
		Assert(0 < n && n <= n_max);
		return fac[n - 1] * fac_inv[n];
	}

	// 順列の数 nPr を返す．
	mint perm(int n, int r) const {
		Assert(n <= n_max);

		if (r < 0 || n - r < 0) return 0;
		return fac[n] * fac_inv[n - r];
	}

	// 二項係数 nCr を返す．
	mint bin(int n, int r) const {
		Assert(n <= n_max);
		if (r < 0 || n - r < 0) return 0;
		return fac[n] * fac_inv[r] * fac_inv[n - r];
	}

	// 多項係数 nC[rs] を返す．
	mint mul(const vi& rs) const {
		if (*min_element(all(rs)) < 0) return 0;
		int n = accumulate(all(rs), 0);
		Assert(n <= n_max);

		mint res = fac[n];
		repe(r, rs) res *= fac_inv[r];

		return res;
	}

	int get_q() const {
		return q;
	}
};


//【階乗などの埋め込み】
/*
* 階乗とその逆数を埋め込み，任意の n に対し n!, 1/n! を O(WIDTH) で得られるようにする．
* AtCoder の提出コードは 512KB が上限なので，WIDTH ≧ 4*10^4 あたりが限界．
*/
const int WIDTH = (int)1e7; int MOD = mint::mod();
void embed_factorial() {
	// verify : https://judge.yosupo.jp/problem/many_factorials

	mint fac = 1;
	vm res;
	rep(i, MOD) {
		if (i % WIDTH == 0) res.emplace_back(fac);
		if (i < MOD - 1) fac *= i + 1;
	}

	mint fac_inv = fac.inv();
	vm res_inv;
	rep(i, MOD) {
		if (i % WIDTH == 0) res_inv.emplace_back(fac_inv);
		fac_inv *= MOD - 1 - i;
	}

	cout << "vi FACT={";
	rep(i, sz(res)) cout << res[i] << (i < sz(res) - 1 ? "," : "};\n");

	cout << "vi FACT_INV={";
	rep(i, sz(res_inv)) cout << res_inv[i] << (i < sz(res_inv) - 1 ? "," : "};\n");

	exit(0);

	/* 埋め込んだテーブルを元に階乗やその逆数の値を計算する．
	mint fac(ll n) {
		if (n >= MOD) return 0;
		ll q = n / WIDTH;
		mint res = FACT[q];
		repi(i, q * WIDTH + 1, n) res *= i;
		return res;
	}

	mint fac_inv(ll n) {
		Assert(n < MOD);
		ll q = (MOD - 1 - n) / WIDTH;
		mint res = FACT_INV[q];
		repi(i, q * WIDTH + 1, MOD - 1 - n) res *= MOD - i;
		return res;
	}
	*/
}
vi FACT = { };
vi FACT_INV = { };


//【二項係数の畳込み】
/*
* 母関数.h へ
*/


