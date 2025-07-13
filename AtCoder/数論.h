#pragma once
#include "header.h"
// ■■■■■ 数論 ■■■■■


//【素数判定】O(√n)
/*
* n が素数かを返す．
*/
bool primeQ(ll n) {
	// verify : https://algo-method.com/tasks/319

	if (n == 1) return false;

	// i = (合成数) もループを回ってしまうが気にしない
	for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0) return false;
	}
	return true;
}


//【素因数分解】O(√n)
/*
* n を素因数分解した結果を pps に格納し pps を返す．
* pps[p] = e は n に素因数 p が e 個含まれていることを表す．
*/
template <class T>
map<T, int> factor_integer(T n) {
	// verify : https://algo-method.com/tasks/457

	map<T, int> pps;

	for (T i = 2; i * i <= n; i++) {
		int d = 0;
		while (n % i == 0) {
			d++;
			n /= i;
		}
		if (d > 0) pps[i] = d;
	}
	if (n > 1) pps[n] = 1;

	return pps;
}


//【約数の個数】O(√n)
/*
* n の正の約数の個数を返す．
*/
int count_divisors(ll n) {
	// verify : https://algo-method.com/tasks/344

	Assert(n != 0);
	n = abs(n);

	int res = 0; ll i = 1;
	for (; i * i < n; i++) if (n % i == 0) res += 2;
	if (i * i == n) res++;

	return res;
}


//【約数の総和】O(√n)
/*
* n の正の約数の総和を返す．
*/
ll divisors_sum(ll n) {
	if (n == 1) return 1;

	ll res = 0, i = 1;
	for (; i * i < n; i++) {
		if (n % i == 0) {
			res += i;
			res += n / i;
		}
	}
	if (i * i == n) res += i;

	return res;
}


//【約数の総積】
/*
* n = Π_i p[i]^e[i] と素因数分解されるとき，以下の等式が成り立つ：
*	Πd|n d = n^(1/2 Π_i(e[i]+1))
* 
* 証明：
*	(Πd|n d)^2 = (Πd|n d)(Πd|n n/d) = Πd|n n = n^(Σd|n 1) = n^(Π_i(e[i]+1))
* 
* verify : https://atcoder.jp/contests/arc167/tasks/arc167_b
*/


//【最小の非約数 = 素数冪】
/*
* 任意の自然数 N について，N の約数でない最小の正整数 x は x = p^e の形で表される．
* 
*（証明）x = p1^e1 p2^e2 ... と表されると仮定する．
* x の選び方より p1^e1, p2^e2, ... < x は全て N の約数であり，
* 従ってそれらの積である x も N の約数となり矛盾する．
* 
* verify : https://yukicoder.me/problems/9085
*/


//【高度合成数の早見表】
/*
* 1 桁の最大  : 6（約数 4 個）
* 2 桁の最大  : 60（約数 12 個）
* 3 桁の最大  : 840（約数 32 個）
* 4 桁の最大  : 7560（約数 64 個）
* 5 桁の最大  : 83160（約数 128 個）
* 6 桁の最大  : 720720（約数 240 個）
* 7 桁の最大  : 8648640（約数 448 個）
* 8 桁の最大  : 73513440（約数 768 個）
* 9 桁の最大  : 735134400（約数 1344 個）
* 10 桁の最大 : 6983776800（約数 2304 個）
* 11 桁の最大 : 97772875200（約数 4032 個）
* 12 桁の最大 : 963761198400（約数 6720 個）
* 13 桁の最大 : 9316358251200（約数 10752 個）
* 14 桁の最大 : 97821761637600（約数 17280 個）
* 15 桁の最大 : 866421317361600（約数 26880 個）
* 16 桁の最大 : 8086598962041600（約数 41472 個）
* 17 桁の最大 : 74801040398884800（約数 64512 個）
* 18 桁の最大 : 897612484786617600（約数 103680 個）
* 
* 参考 : https://algo-method.com/descriptions/92
*/


//【フェルマーの小定理】
/*
* p を素数とする．
*	a が p の倍数でないとき，a^(p-1) ≡ 1 (mod p)
*	a が無条件で a^p ≡ a (mod p)
*/


//【オイラー関数】O(√n)
/*
* オイラー関数の値 φ(n) を返す．
* 
* 利用：【素因数分解】
*/
ll euler_phi(ll n) {
	// verify : https://judge.yosupo.jp/problem/tetration_mod

	// n を素因数分解した結果を pps に受け取る．
	auto pps = factor_integer(n);

	// φ(n) を計算する．
	ll res = 1;
	repe(pp, pps) {
		res *= (pp.first - 1) * powi(pp.first, pp.second - 1);
	}
	return res;
}


//【オイラーの定理】
/*
* gcd(a, m) = 1 のとき，a^φ(m) ≡ 1 (mod m)
* 
* 注意：
* gcd(a, m) = 1 でないとき a^(φ(m) + 1) ≡ a (mod m) は一般には成り立たない．
* 例えば (a, m) = (2, 8) のとき，φ(8) = 4 だが，2^(4+1) !≡ 2 (mod 8) である． 
* 
* verify : https://atcoder.jp/contests/arc113/tasks/arc113_b
*/


//【カーマイケル関数】O(√n)
/*
* カーマイケル関数の値 λ(n) を返す．
* λ(n) は (Z/nZ)* の冪数（すべての元の位数の最小公倍数）に等しい．
*
* 利用：【素因数分解】
*/
ll carmichael_lambda(ll n) {
	// verify : https://judge.yosupo.jp/problem/tetration_mod

	// n を素因数分解した結果を pps に受け取る．
	auto pps = factor_integer(n);

	// λ(n) を計算する．
	ll res = 1;
	repe(pp, pps) {
		ll v;
		if (pp.first == 2) {
			int e = (pp.second >= 3 ? pp.second - 2 : pp.second - 1);
			v = 1LL << e;
		}
		else {
			v = (pp.first - 1) * powi(pp.first, pp.second - 1);
		}
		res = res / gcd(res, v) * v;
	}
	return res;
}


//【メビウス関数】O(√n)
/*
* メビウス関数の値 μ(n) を返す．
* μ(n) = (-1)^k（n が相異なる k 個の素数の積）or 0（n が平方因子を含む）
*/
int mobius_mu(ll n) {
	// verify : https://algo-method.com/tasks/494

	int res = 1;

	for (ll i = 2; i * i <= n; i++) {
		if (n % (i * i) == 0) return 0;

		if (n % i == 0) {
			n /= i;
			res *= -1;
		}
	}
	if (n > 1) res *= -1;

	return res;
}


//【素因数の個数】O(log n)
/*
* n に含まれる素因数 p の個数を返す（n = 0 なら INF を返す）
*/
int integer_exponent(ll n, ll p) {
	// verify : https://atcoder.jp/contests/agc047/tasks/agc047_a

	//【備考】
	// p は素数でなくても正しく動作する．

	if (n == 0) return INF;

	int res = 0;
	while (n % p == 0) {
		n /= p;
		res++;
	}
	return res;
}


//【ウィルソンの定理の一般化】
/*
* [1..n] 中の n と互いに素な数の総積を g(n) とする．g(n) は以下の式で表される：
*	g(n) := Π_d|n ((n/d)! d^(n/d))^μ(d)
* 
* n=4 であるか，ある奇素数 p と自然数 k を用いて n=p^k or 2 p^k と表されるとき
*		g(n) = -1 (mod n)
* その他のとき
*		g(n) = 1  (mod n)
*/


//【k 番目の倍数】O((k + n) log n)
/*
* a[0..n) のいずれかの倍数であるような正整数のうち昇順で k 番目のものを返す．
*/
ll k_th_multiple(vl a, int k) {
	// verify : https://mojacoder.app/users/_kanpurin_/problems/kanpurin003

	uniq(a);
	int n = sz(a);

	priority_queue_rev<pli> q;

	rep(i, n) q.push({ a[i], i });
	vi pt(n, 1);

	ll val = 0;

	while (1) {
		if (k == -1) EXIT(val);

		auto [v, i] = q.top(); q.pop();

		pt[i]++;
		q.push({ a[i] * pt[i], i });

		if (v == val) continue;

		val = v;
		k--;
	}

	return -1;
}


