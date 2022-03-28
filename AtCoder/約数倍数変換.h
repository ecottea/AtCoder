#pragma once
#include "header.h"
#include "数論.h"
// ■■■■■ 約数変換，倍数変換 ■■■■■


//【倍数変換】
/*
* Multiple_transform<T>(int n) : O(n log(log n))
*   n までの素数を持って初期化する．
*
* vT convolution_gcd(vT a, vT b) : O(n log(log n))
*   c[k] = Σ_(gcd(i, j) = k) a[i] b[j] なる c を返す．
*
* multiple_zeta(vT& a) : O(n log(log n))
*   A[j] = Σ_(j | i) a[i] なる A に上書きする．
*  （倍数ゼータ変換，約数への累積和）
*
* multiple_mobius(vT& A) : O(n log(log n))
*   A[j] = Σ_(j | i) a[i] なる a に上書きする．
*  （倍数メビウス変換，倍数への差分）
*
* 制約：1-indexed とし，a[0], b[0] は使用しない．
*
* 利用：【素数の列挙】
*/
template <typename T> struct Multiple_transform {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5
	// verify : https://judge.yosupo.jp/problem/gcd_convolution

	vi ps; // 素数のリスト

	Multiple_transform() {}
	Multiple_transform(int n) { eratosthenes(n, ps); }

	void multiple_zeta(vector<T>& f) {
		int n = sz(f);

		// 各素因数ごとに上からの累積和をとる
		repe(p, ps) {
			repir(i, (n - 1) / p, 1) f[i] += f[p * i];
		}
	}

	void multiple_mobius(vector<T>& f) {
		int n = sz(f);

		// 各素因数ごとに下からの差分をとる
		repe(p, ps) {
			repi(i, 1, (n - 1) / p) f[i] -= f[p * i];
		}
	}

	vector<T> convolution_gcd(vector<T> a, vector<T> b) {
		int n = sz(a);

		// 各素因数の min をとったものが gcd なので min 畳み込みを行う．
		multiple_zeta(a); multiple_zeta(b);
		rep(i, n) a[i] *= b[i];
		multiple_mobius(a);
		return a;
	}
};


//【約数変換】
/*
* Divisor_transform<T>(int n) : O(n log(log n))
*   n までの素数を持って初期化する．
*
* vT convolution_lcm(vT a, vT b) : O(n log(log n))
*   c[k] = Σ_(lcm(i, j) = k) a[i] b[j] なる c を返す．
*   ただし c[n] を含めそれ以降は切り捨てる．
*
* divisor_zeta(vT& a) : O(n log(log n))
*   A[j] = Σ_(i | j) a[i] なる A に上書きする．
*  （約数ゼータ変換，倍数への累積和）
*
* divisor_mobius(vT& A) : O(n log(log n))
*   A[j] = Σ_(i | j) a[i] なる a に上書きする．
*  （約数メビウス変換，約数への差分）
*
* 制約：1-indexed とし，a[0], b[0] は使用しない．
*
* 利用：【素数の列挙】
*/
template <typename T> struct Divisor_transform {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5
	// verify : https://judge.yosupo.jp/problem/lcm_convolution

	vi ps; // 素数のリスト

	Divisor_transform() {}
	Divisor_transform(int n) { eratosthenes(n, ps); }

	void divisor_zeta(vector<T>& f) {
		int n = sz(f);

		// 各素因数ごとに下からの累積和をとる
		repe(p, ps) {
			repi(i, 1, (n - 1) / p) f[p * i] += f[i];
		}
	}

	void divisor_mobius(vector<T>& f) {
		int n = sz(f);

		// 各素因数ごとに上からの差分をとる
		repe(p, ps) {
			repir(i, (n - 1) / p, 1) f[p * i] -= f[i];
		}
	}

	vector<T> convolution_lcm(vector<T> a, vector<T> b) {
		int n = sz(a);

		// 各素因数の max をとったものが lcm なので max 畳み込みを行う．
		divisor_zeta(a); divisor_zeta(b);
		rep(i, n) a[i] *= b[i];
		divisor_mobius(a);
		return a;
	}
};


//【倍数変換（添字約数制限）】
/*
* Limited_multiple_transform(ps, divs) : O(1)
*   定数 n を定め，n の素因数の昇順列を ps，約数の昇順列を divs とする．
*	添字集合を n の約数集合として初期化する．
*
* convolution_gcd(a, b) : O(σ(n) ω(n))
*   c[k] = Σ_(gcd(i, j) = k) a[i] b[j] なる c を返す．
* （σ(n) : n の約数の個数，ω(n) : n の素因数の種類数）
*
* multiple_zeta(a) : O(σ(n) ω(n))
*   A[j] = Σ_(j | i) a[i] なる A に上書きする．
*  （倍数ゼータ変換，約数への累積和）
*
* multiple_mobius(A) : O(σ(n) ω(n))
*   A[j] = Σ_(j | i) a[i] なる a に上書きする．
*  （倍数メビウス変換，倍数への差分）
*/
template <typename T> struct Limited_multiple_transform {
	vl ps; // ps : n の素因数の昇順リスト
	vl divs; // divs : n の約数の昇順リスト

	Limited_multiple_transform() {}
	Limited_multiple_transform(const vl& ps_, const vl& divs_) : ps(ps_), divs(divs_) {}

	void multiple_zeta(unordered_map<ll, T>& f) {
		// 各素因数ごとに上からの累積和をとる
		repe(p, ps) {
			for (auto it = divs.rbegin(); it != divs.rend(); it++) {
				ll d = *it;

				if (!f.count(p * d)) continue;

				f[d] += f[p * d];
			}
		}
	}

	void multiple_mobius(unordered_map<ll, T>& f) {
		// verify : https://atcoder.jp/contests/abc212/tasks/abc212_g

		// 各素因数ごとに下からの差分をとる
		repe(p, ps) {
			repe(d, divs) {
				if (!f.count(p * d)) continue;

				f[d] -= f[p * d];
			}
		}
	}

	unordered_map<ll, T> convolution_gcd(unordered_map<ll, T> a, unordered_map<ll, T> b) {
		// 各素因数の min をとったものが gcd なので min 畳み込みを行う．
		multiple_zeta(a);
		multiple_zeta(b);

		repe(d, divs) a[d] *= b[d];

		multiple_mobius(a);

		return a;
	}
};


//【約数変換（添字約数制限）】
/*
* Limited_divisor_transform(ps, divs) : O(1)
*   定数 n を定め，n の素因数の昇順列を ps，約数の昇順列を divs とする．
*	添字集合を n の約数集合として初期化する．
*
* convolution_lcm(a, b) : O(σ(n) ω(n))
*   c[k] = Σ_(lcm(i, j) = k) a[i] b[j] なる c を返す．
* （σ(n) : n の約数の個数，ω(n) : n の素因数の種類数）
*
* divisor_zeta(a) : O(σ(n) ω(n))
*   A[j] = Σ_(i | j) a[i] なる A に上書きする．
*  （約数ゼータ変換，倍数への累積和）
*
* divisor_mobius(A) : O(σ(n) ω(n))
*   A[j] = Σ_(i | j) a[i] なる a に上書きする．
*  （約数メビウス変換，約数への差分）
*/
template <typename T> struct Limited_divisor_transform {
	vl ps; // ps : n の素因数の昇順リスト
	vl divs; // divs : n の約数の昇順リスト

	Limited_divisor_transform() {}
	Limited_divisor_transform(const vl& ps_, const vl& divs_) : ps(ps_), divs(divs_) {}

	void divisor_zeta(unordered_map<ll, T>& f) {
		// 各素因数ごとに下からの累積和をとる
		repe(p, ps) {
			repe(d, divs) {
				if (!f.count(p * d)) continue;

				f[p * d] += f[d];
			}
		}
	}

	void divisor_mobius(unordered_map<ll, T>& f) {
		// verify : https://atcoder.jp/contests/arc064/tasks/arc064_d

		// 各素因数ごとに上からの差分をとる
		repe(p, ps) {
			for (auto it = divs.rbegin(); it != divs.rend(); it++) {
				ll d = *it;

				if (!f.count(p * d)) continue;

				f[p * d] -= f[d];
			}
		}
	}

	unordered_map<ll, T> convolution_gcd(unordered_map<ll, T> a, unordered_map<ll, T> b) {
		// 各素因数の max をとったものが lcm なので max 畳み込みを行う．
		divisor_zeta(a);
		divisor_zeta(b);

		repe(d, divs) a[d] *= b[d];

		divisor_mobius(a);

		return a;
	}
};


