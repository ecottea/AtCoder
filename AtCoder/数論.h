#pragma once
#include "header.h"
// ■■■■■ 数論 ■■■■■


//【約数列挙】O(√n)
/*
* n の約数全てを昇順に格納したリストを返す．
*/
vl divisors(ll n) {
	// verify : https://yukicoder.me/problems/no/2142

	vl ds;

	if (n == 1) {
		ds.push_back(1);
		return ds;
	}

	ll i = 1;
	for (; i * i < n; i++) {
		if (n % i == 0) {
			ds.push_back(i);
			ds.push_back(n / i);
		}
	}
	if (i * i == n) ds.push_back(i);

	sort(all(ds));

	return ds;
}


//【約数列挙（素因数分解済）】O(σ(n))
/*
* n の素因数分解結果 pps を利用して n の約数全てを昇順に格納したリストを返す．
*/
template <class T>
vector<T> divisors(map<T, int>& pps) {
	// verify : https://atcoder.jp/contests/arc068/tasks/arc068_c

	vector<T> divs{ T(1) };
	repe(pp, pps) {
		T p; int d;
		tie(p, d) = pp;

		vector<T> powp(d);
		powp[0] = p;
		rep(i, d - 1) powp[i + 1] = powp[i] * p;

		int m = sz(divs);
		repir(j, m - 1, 0) rep(i, d) divs.push_back(divs[j] * powp[i]);
	}
	sort(all(divs));

	return divs;
}


//【約数の個数】O(√n)
/*
* n の正の約数の個数を返す．
*/
ll count_divisors(ll n) {
	// verify : https://algo-method.com/tasks/344

	Assert(n != 0);
	n = abs(n);

	ll res = 0, i = 1;
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


//【素因数分解】O(√n)
/*
* n を素因数分解した結果を pps に格納し pps を返す．
* pps[p] = d は n に素因数 p が d 個含まれていることを表す．
*/
map<ll, int> factor_integer(ll n) {
	// verify : https://algo-method.com/tasks/457

	map<ll, int> pps;

	for (ll i = 2; i * i <= n; i++) {
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
		res *= (pp.first - 1) * pow(pp.first, pp.second - 1);
	}
	return res;
}


//【オイラーの定理】
/*
* gcd(a, m) = 1 のとき，a^φ(m) = 1 (mod m)
* 任意の a について a^(φ(m) + 1) = a (mod m)
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
	// verify : https://atcoder.jp/contests/jag2015summer-day4/tasks/icpc2015summer_day4_d

	// n を素因数分解した結果を pps に受け取る．
	auto pps = factor_integer(n);

	// λ(n) を計算する．
	ll res = 1;
	repe(pp, pps) {
		ll v;
		if (pp.first == 2) {
			int e = (pp.second >= 3 ? pp.second - 2 : pp.second - 1);
			v = pow(2, e);
		}
		else {
			v = (pp.first - 1) * pow(pp.first, pp.second - 1);
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
* n を割る p の最大べきを返す（p は素数でなくても動作する）
*/
int integer_exponent(ll n, ll p) {
	// verify : https://atcoder.jp/contests/agc047/tasks/agc047_a

	Assert(n != 0);

	int res = 0;
	while (n % p == 0) {
		n /= p;
		res++;
	}
	return res;
}


//【階乗のもつ素因数】O(log n)
/*
* n! がもつ素因数 p の個数を返す．
*
* 制約 : p は素数
*/
ll legendres(ll n, ll p) {
	// verify : https://algo-method.com/tasks/452

	ll res = 0;
	while (n > 0) {
		res += n / p;
		n /= p;
	}
	return res;
}


//【素因数と約数の列挙】O(√n)
/*
* n の互いに異なる素因数全てをリスト ps に，約数全てをリスト divs にそれぞれ昇順に格納する．
*/
void primefactors_and_divisors(ll n, vl& ps, vl& divs) {
	// verify : https://atcoder.jp/contests/abc212/tasks/abc212_g

	ps.clear();
	divs.clear();
	divs.push_back(1);

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
}


//【ウィルソンの定理の一般化】
/*
* [1..n] 中の n と互いに素な数の総積を P(n) とする．
* n = 4 であるか，ある奇素数 p と自然数 k を用いて n = p^k or 2 p^k と表されるとき
*		P(n) = -1 (mod n)
* その他のとき
*		P(n) = 1  (mod n)
*/


