#pragma once
#include "header.h"
// ■■■■■ 数論 ■■■■■


//【約数列挙】O(√n)
/*
* n の約数全てを昇順に格納したリストを返す．
*/
template <class T>
vector<T> divisors(T n) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_fb

	vector<T> ds;

	if (n == 1) {
		ds.push_back(1);
		return ds;
	}

	T i = 1;
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


//【素因数分解】O(√n)
/*
* n を素因数分解した結果を pps に格納し pps を返す．
* pps[p] = d は n に素因数 p が d 個含まれていることを表す．
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
* n を割る p の最大べきを返す（n = 0 なら INF を返す）
*/
int integer_exponent(ll n, ll p) {
	// verify : https://atcoder.jp/contests/agc047/tasks/agc047_a

	if (n == 0) return INF;

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

	//【注意】
	// ルジャンドル多項式を求める std::legendre() と衝突しないように legendres() にしている．

	ll res = 0;
	while (n > 0) {
		res += n / p;
		n /= p;
	}
	return res;
}


//【ウィルソンの定理の一般化】
/*
* [1..n] 中の n と互いに素な数の総積を P(n) とする．
* n = 4 であるか，ある奇素数 p と自然数 k を用いて n = p^k or 2 p^k と表されるとき
*		P(n) = -1 (mod n)
* その他のとき
*		P(n) = 1  (mod n)
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


