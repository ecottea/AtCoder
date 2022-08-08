#pragma once
#include "header.h"
// ■■■■■ 数論 ■■■■■


//【最大公約数】O(log min(a, b))
/*
* gcd(a, b) >= 0 を返す．
*/
ll euclid(ll a, ll b) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/1/ALDS1_1_B

	a = abs(a);
	b = abs(b);

	// 改変しやすいよう再帰を用いずに書く
	while (b > 0) {
		a %= b;
		swap(a, b);
	}

	return a;
}


//【最大公約数（複数）】O(n + log(max a[0..n)))
/*
* gcd a[0..n) を返す．（空列の gcd は 0 とする）
*/
ll gcd(const vl& a) {
	// verify : https://atcoder.jp/contests/arc048/tasks/arc048_c

	int n = sz(a);

	ll g = 0;
	rep(i, n) g = gcd(g, a[i]);

	return g;
}


//【最小公倍数】O(log max(a, b))
/*
* lcm(a, b) を返す．
*/
ll lcm(ll a, ll b) {
	//verify: https://atcoder.jp/contests/abc131/tasks/abc131_c

	return a / gcd(a, b) * b;
}


//【最小公倍数（複数，結果が小さな数）】O(n log(max a[0..n))) 
/*
* lcm a[0..n) を返す．（空列の lcm は 1 とする）
*/
ll lcm(const vl& a) {
	int n = sz(a);
	
	ll l = 1;
	rep(i, n) l = l / gcd(l, a[i]) * a[i];
	
	return l;
}


//【最小公倍数（複数，結果が大きな数）】O(n √max(a)) 
/*
* lcm a[0..n) を返す．（空列の lcm は 1 とする）
*
* 利用：【素因数分解】
*/
mint lcm(const vi& a) {
	// verify : https://atcoder.jp/contests/abc152/tasks/abc152_e

	int n = sz(a);

	map<ll, int> lpps;

	rep(i, n) {
		map<ll, int> pps;
		factor_integer(a[i], pps);

		repe(pp, pps) chmax(lpps[pp.first], pp.second);
	}

	mint res = 1;
	repe(pp, lpps) res *= pow(pp.first, pp.second);

	return res;
}


//【拡張ユークリッドの互除法】O(log max(|a|, |b|))
/*
* g = gcd(a, b) > 0 を返しつつ，a x + b y = g の解 (x, y) を求める．
* |x| + |y| は最小になるよう選ばれる．
*/
ll extended_gcd(ll a, ll b, ll& x, ll& y) {
	// 参考：https://qiita.com/drken/items/b97ff231e43bce50199a
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/all/NTL_1_E

	//【方法】
	// b = 0 の場合は，明らかに g = a で，(x, y) = (1, 0) が解である．
	// 
	// b != 0 の場合を考える．a を b で割り
	//		a = q b + r (0 <= r < b)
	// なる q, r を得ておく．これを元の式に代入すると
	//		(q b + r) x + b y = g
	//		⇔ b (q x + y) + r x = g
	// となるので，
	//		b X + r Y = g
	// の解 (X, Y) = (q x + y, x) を求めれば
	//		(x, y) = (Y, X - q Y)
	// として元の式の解が得られる．

	// b = 0 になったら自明解を返す．
	if (b == 0) {
		// 最大公約数は正とする．
		x = (a > 0) ? 1 : -1;
		y = 0;
		return a * x;
	}

	// a を b で割った商 q と余り r を求めておく．
	ll q = a / b, r = a % b;

	// a, b を更新し解 X, Y を得る．
	ll X, Y;
	ll d = extended_gcd(b, r, X, Y);

	// X, Y から x, y を得る．
	x = Y;
	y = X - q * Y;

	return d;
}


//【一次不定方程式】O(log max(|a|, |b|))
/*
* a x + b y = c の特殊解 (x, y) を求める．
* 解があれば gcd(a, b) > 0，なければ -1 を返す．
*
* 利用：【拡張ユークリッドの互除法】
*/
ll bezout(ll a, ll b, ll c, ll& x, ll& y) {
	// verify : https://atcoder.jp/contests/arc091/tasks/arc091_d

	ll g = extended_gcd(a, b, x, y);

	if (c % g != 0) return -1;
	
	x *= c / g;
	y *= c / g;

	// もし x を非負の範囲で最小にしたければ，x = smod(x, b / g) とする．
	// もし y を非負の範囲で最小にしたければ，y = smod(y, a / g) とする．
	// verify : https://atcoder.jp/contests/arc091/tasks/arc091_d

	return g;
}


//【約数列挙】O(√n)
/*
* n の約数全てをリスト ds に昇順に格納する．
*/
void divisors(ll n, vl& ds) {
	// verify : https://algo-method.com/tasks/346

	ds.clear();

	if (n == 1) {
		ds.push_back(1);
		return;
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
}


//【約数列挙（素因数分解済）】O(σ(n))
/*
* n の素因数分解結果 pps を利用して n の約数全てをリスト divs に昇順に格納する．
*/
template <class T> void divisors(map<T, int>& pps, vector<T>& divs) {
	// verify : https://atcoder.jp/contests/arc068/tasks/arc068_c

	divs = vector<T>({ T(1) });
	repe(pp, pps) {
		T p; int d;
		tie(p, d) = pp;

		vector<T> powp(d);
		powp[0] = p;
		rep(i, d - 1) powp[i + 1] = powp[i] * p;

		int m = sz(divs);
		repir(j, m - 1, 0) {
			rep(i, d) {
				divs.push_back(divs[j] * powp[i]);
			}
		}
	}
	sort(all(divs));
}


//【約数関数】O(√n k)
/*
* 約数関数 σ_k(n) = (n の約数それぞれを k 乗した和) を返す．
* 特に k = 0 なら約数の個数，k = 1 なら約数の総和と等価である．
*/
ll divisor_sigma(int k, ll n) {
	// verify(k=0) : https://algo-method.com/tasks/344

	if (n == 1) return 1;
	
	ll res = 0, i = 1;
	for (; i * i < n; i++) {
		if (n % i == 0) {
			res += pow(i, k);
			res += pow(n / i, k);
		}
	}
	if (i * i == n) res += pow(i, k);
	
	return res;
}


//【素因数分解】O(√n)
/*
* n を素因数分解した結果を pps に格納する．
* 
* pps[p] = d : n に素因数 p が d 個含まれていることを表す．
*/
void factor_integer(ll n, map<ll, int>& pps) {
	// verify : https://algo-method.com/tasks/457

	pps.clear();

	for (ll i = 2; i * i <= n; i++) {
		int d = 0;
		while (n % i == 0) {
			d++;
			n /= i;
		}
		if (d > 0) pps[i] = d;
	}
	if (n > 1) pps[n] = 1;
}


//【素因数分解（複数）】
/*
* Factor_integer(int n) : O(n log(log n))
*	n 以下の自然数を高速に素因数分解する準備を行う．
*
* factor_integer(int i, map<int, int>& pps) : O(log n)
*	i の素因数分解結果を pps に格納する．
*/
struct Factor_integer {
	// verify : https://atcoder.jp/contests/abc215/tasks/abc215_d

	int n;

	// d[i] : i を割り切る最小の素数
	vi d;

	// n 以下の自然数を高速に素因数分解する準備を行う．
	Factor_integer(int n_) : n(n_), d(n + 1) {
		iota(all(d), 0);

		for (int p = 2; p * p <= n; p++) {
			if (d[p] != p) continue;

			for (int i = p; i <= n; i += p) {
				d[i] = p;
			}
		}
	}

	// i の素因数分解結果を pps に格納する．
	void factor_integer(int i, map<int, int>& pps) {
		Assert(i <= n);

		pps.clear();

		while (i > 1) {
			pps[d[i]]++;
			i /= d[i];
		}
	}
};


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
	map<ll, int> pps;
	factor_integer(n, pps);

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
* λ(n) は (Z/nZ)^* の冪数（すべての元の位数の最小公倍数）に等しい．
*
* 利用：【素因数分解】
*/
ll carmichael_lambda(ll n) {
	// verify : https://atcoder.jp/contests/jag2015summer-day4/tasks/icpc2015summer_day4_d

	// n を素因数分解した結果を pps に受け取る．
	map<ll, int> pps;
	factor_integer(n, pps);

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
* μ(n) = (-1)^k (n が相異なる k 個の素数の積) or 0 （n が平方因子を含む）
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


//【p-進付値】O(log n)
/*
* n を割る p の最大べきを返す．（p は素数でなくても動作する）
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
ll legendre(ll n, ll p) {
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


