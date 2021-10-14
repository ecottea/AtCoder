#pragma once
#include "header.h"
#include "行列.h"
// ■■■■■ 組合せ論 ■■■■■



//【階乗と二項係数（mint利用）】
/*
* 十分大きな素数を法として，階乗，その逆数，二項係数を計算する．
*
* factorial_mint(n) : O(n)
*	n! までの階乗とその逆数を前計算する．
*
* factorial(n) : O(1)
*	n! を返す．
*
* factorial_inv(n) : O(1)
*	1 / n! を返す．
* 
* inv(n) : O(1)
*	1 / n を返す．
*
* permutation(n, r) : O(1)
*	順列の数 nPr を返す．
* 
* binomial(n, r) : O(1)
*	二項係数 nCr を返す．
*
* multinomial(r) : O(|r|)
*	多項係数 nC[r] を返す．（n = Σr）
*/
struct factorial_mint {
	// 階乗，階乗の逆数，逆数の値を保持するテーブル
	vm fac, fac_inv, inv;

	// n! までの階乗とその逆数を前計算しておく．O(n)
	factorial_mint(int n) {
		fac = vm(n + 1LL);
		fac[0] = 1;
		repi(i, 1, n) fac[i] = fac[i - 1LL] * i;
		
		fac_inv = vm(n + 1LL);
		fac_inv[n] = fac[n].inv();
		repir(i, n - 1, 1) fac_inv[i] = fac_inv[i + 1LL] * (i + 1);
		fac_inv[0] = 1;

		inv = vm(n + 1LL);
		repi(i, 1, n) inv[i] = fac[i - 1LL] * fac_inv[i];
	}

	// n! を返す．O(1)
	mint factorial(int n) { return fac[n]; }

	// 1 / n! を返す．O(1)
	mint factorial_inv(int n) { return fac_inv[n]; }

	// 1 / n を返す．O(1)
	mint inv(int n) { return inv[n]; }

	// 順列の数 nPr を返す．O(1)
	mint permutation(int n, int r) {
		if (r < 0 || n - r < 0) return 0;
		return fac[n] * fac_inv[(ll)n - r];
	}

	// 二項係数 nCr を返す．O(1)
	mint binomial(int n, int r) {
		if (r < 0 || n - r < 0) return 0;
		return fac[n] * fac_inv[r] * fac_inv[(ll)n - r];
	}

	// 多項係数 nC[r] を返す．O(|r|)
	mint multinomial(const vi& r) {
		int len = sz(r);

		int sum = 0;
		rep(i, len) sum += r[i];
		
		mint res = fac[sum];
		repe(ri, r) res *= fac_inv[ri];
		
		return res;
	}
};


//【階乗と二項係数】
/*
* 小さな素数 p を法とし，階乗と二項係数を計算する．
*
* factorial_mod(n, p) : O(p)
*	n! までの階乗を法を p として前計算する．
*
* factorial(n) : O(n log n)
*	n! mod p を返す．
*
* factorial_pr(n) : O(n log n)
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
	vector<mint_p> fac;

	// (p-1)! までの階乗を法を p として前計算しておく．
	factorial_mod(int p_) : p(p_) {
		mint_p::set_mod(p);

		fac = vector<mint_p>(p);
		fac[0] = 1;
		repi(i, 1, p - 1) {
			fac[i] = fac[i - 1] * i;
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
			mod *= fac[r] * (q % 2 ? -1 : 1);

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


//【二項係数】O(min(r, n-r))
/*
* 愚直に二項係数 nCr を計算する．
*/
template <class T>
T binomial(ll n, ll r) {
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
ll factorial(int n) {
	ll val = 1;

	repi(i, 1, n) {
		val *= i;
	}

	return val;
}


//【最短経路数（禁止点あり）】O(n^2)
/*
* h × w の格子路の (0, 0) から (h-1, w-1) までの最短路のうち，
* n 個の禁止点 fb[i] = {r[i], c[i]} を 1 つも通らないものの個数を返す．
*
* 利用：【階乗と二項係数（mint利用）】
*/
mint dummy_path_lemma(int h, int w, const vector<pii>& fb) {
	int n = sz(fb);

	factorial_mint fm(h + w);

	// s, t : 対応する始点と終点の列
	// 番号の小さい方へ戻るパスは存在してはいけない．
	vector<pii> s = { {0,0} }, t = { {h - 1, w - 1} };
	rep(i, n) {
		s.push_back(fb[i]);
	}
	sort(next(s.begin()), s.end());
	repi(i, 1, n) {
		t.push_back(s[i]);
	}

	// DPL 用の行列を作成する．
	vvm dpl(n + 1, vm(n + 1));
	repi(i, 0, n) {
		repi(j, 0, n) {
			int h = t[j].first - s[i].first;
			int w = t[j].second - s[i].second;
			dpl[i][j] = fm.binomial(h + w, h);
		}
	}

	// 列基本変形で第 1 列の 2 行目以降を消去する．
	repir(j, n, 1) {
		rep(i, j) {
			dpl[i][0] -= dpl[j][0] * dpl[i][j];
		}
	}

	// DPL 行列の行列式，すなわち (0,0) 成分が答え．
	return dpl[0][0];
}


