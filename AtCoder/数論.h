#pragma once
#include "header.h"
// ■■■■■ 数論 ■■■■■



//【最大公約数（複数）】O(n log a)
/*
* 長さ n の配列 a に格納された数の最大公約数を返す．
*/
ll gcd(vl& a) {
	int n = sz(a);
	ll g;

	if (n == 0) return 0;

	g = a[0];
	rep (i, n) g = gcd(g, a.at(i));
	
	return g;
}


//【最小公倍数】O(log a)
/*
* 2 数 a, b の最小公倍数を返す．
*/
ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }


//【最小公倍数（複数）】O(n log a)
/*
* 長さ n の配列 a に格納された数の最小公倍数を返す．
* 
* 利用：【最小公倍数】
*/
ll lcm(vl& a) {
	int n = sz(a);
	ll l;

	if (n == 0) return 0;
	
	l = a[0];
	repi(i, 1, n - 1) l = lcm(l, a[i]);
	
	return l;
}


//【拡張ユークリッドの互除法】O(log max(|a|, |b|))
/*
* g = gcd(a, b) > 0 を返しつつ，a x + b y = g の解 (x, y) を求める．
* |x| + |y| は最小になるよう選ばれる．
*/
ll ext_gcd(ll a, ll b, ll& x, ll& y) {
	// 参考：https://qiita.com/drken/items/b97ff231e43bce50199a

	//【方法】
	// b = 0 の場合は，明らかに g = a で，(x, y) = (1, 0) が解である．
	// 
	// b ≠ 0 の場合を考える．a を b で割り
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
		return abs(a);
	}

	// a, b を更新し解 X, Y を得る．
	// 一時変数を使用しないで済むよう y = X, x = Y と受け取っているので注意．
	ll d = ext_gcd(b, a % b, y, x);

	// X, Y から x, y を得る．
	y -= a / b * x;

	return d;
}


//【一次不定方程式】O(log max(|a|, |b|))
/*
* a x + b y = c の特殊解 (x, y) を求める．
*
* 戻り値 : 解があれば gcd(a, b) > 0，なければ -1
*
* 利用：【拡張ユークリッドの互除法】
*/
ll bezout(ll a, ll b, ll c, ll& x, ll& y) {
	ll g = ext_gcd(a, b, x, y);

	if (c % g != 0) return -1;
	
	x *= c / g;
	y *= c / g;

	return g;
}


//【約数関数 σ_k(n)】O(√n)
/*
* 約数関数 σ_k(n) = (n の約数それぞれを k 乗した和) を返す．
* 特に k = 0 なら約数の個数，k = 1 なら約数の総和を返す．
*/
ll divisor_sigma(int k, ll n) {
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


//【約数列挙】O(√n)
/*
* n の約数全てをリスト divs に昇順に格納する．
*/
void divisors(ll n, vl& divs) {
	divs.clear();

	if (n == 1) {
		divs.push_back(1);
		return;
	}

	ll i = 1;
	for (; i * i < n; i++) {
		if (n % i == 0) {
			divs.push_back(i);
			divs.push_back(n / i);
		}
	}
	if (i * i == n) divs.push_back(i);

	sort(all(divs));
}


//【素数の列挙／エラトステネスの篩】O(n log(log n))
/*
* エラトステネスの篩を用いて n 以下の素数を列挙し，ps に昇順に格納する．
*/
void eratosthenes(int n, vi& ps) {
	ps.clear();

	// 素数かどうかを記録しておくためのテーブル
	vb is_prime(n + 1LL, true);

	int i;

	// √n 以下の i の処理
	for (i = 2; i <= n / i; i++) {
		if (is_prime[i]) {
			ps.push_back(i);

			for (int j = i * i; j <= n; j += i) {
				is_prime[j] = false;
			}
		}
	}

	// √n より大きい i の処理
	for (; i <= n; i++) {
		if (is_prime[i]) ps.push_back(i);
	}
}


//【区間内の素数の列挙】O(√r + (r - l) √r / log r)
/*
* [l..r) に含まれる素数を ps に昇順に格納する．
*
* 利用：【素数の列挙／エラトステネスの篩】
*/
void eratosthenes_interval(ll l, ll r, vl& ps) {
	ps.clear();

	vi ps_sub;
	eratosthenes((int)sqrt(r) + 3, ps_sub);

	// 素数かどうかを記録しておくためのテーブル
	vb is_prime(r - l, true);
	repe(p, ps_sub) {
		for (ll j = (l + p - 1) / p * p; j < r; j += p) {
			if (j != p) is_prime[j - l] = false;
		}
	}

	rep(i, r - l) {
		if (is_prime[i]) ps.push_back(l + i);
	}
}


//【素因数分解／試し割り法】O(√n)
/*
* n を素因数分解した結果を pps に格納する．
* 
* pps[p] = d : n に素因数 p が d 個含まれていることを表す．
*/
void factor_integer(ll n, map<ll, int>& pps) {
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


//【素数判定／試し割り法】O(√n)
/*
* n が素数かを返す．
*/
bool primeQ(ll n) {
	if (n == 1) return false;
	
	// i = (合成数) もループを回ってしまうが気にしない
	for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0) return false;
	}
	return true;
}


//【オイラー関数 φ(n)】O(√n)
/*
* オイラー関数の値 φ(n) を返す．
* 
* 利用：【素因数分解】
*/
ll euler_phi(ll n) {
	// n を素因数分解した結果を pps に受け取る．
	map<ll, int> pps;
	factor_integer(n, pps);

	// φ(n) を計算する．
	ll res = 1;
	for (auto pp : pps) {
		res *= (pp.first - 1) * pow(pp.first, pp.second - 1);
	}
	return res;
}


//【メービウス関数 μ(n)】O(√n)
/*
* メービウス関数の値 μ(n) を返す．
* μ(n) = (-1)^k (n が相異なる k 個の素数の積) or 0 （n が平方因子を含む）
*/
int mobius_mu(ll n) {
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


//【p-進付値 ord_p(n)】O(log n)
/*
* n を割る p の最大べきを返す．（p は素数でなくても動作する）
*/
int integer_exponent(ll n, ll p) {
	int res = 0;
	while (n % p == 0) {
		n /= p;
		res++;
	}
	return res;
}


//【階乗のもつ素因数／ルジャンドルの公式】O(log n)
/*
* n! がもつ素因数 p の個数を返す．
*
* 制約 : p は素数
*/
ll legendre(ll n, ll p) {
	ll res = 0;
	while (n > 0) {
		res += n / p;
		n /= p;
	}
	return res;
}


//【互いに素な数の個数】O(2^m)（m : a の素因数の種類数）
/*
* l 以上 r 以下の整数のうち、a と互いに素な数の個数を返す．
*
* 利用：【素因数分解】
*/
ll count_coprime(ll a, ll l, ll r) {
	// a と互いに素 ⇔ a の各素因数で割り切れない，なので a を素因数分解する．
	map<ll, int> pps;
	factor_integer(a, pps);

	// a の素因数だけのリスト p を作る．（個数は使わない）
	vl p;
	repe(s, pps) p.push_back(s.first);
	int m = sz(p);

	// 包除原理を用いて数え上げる．
	// 例えば，6 と互いに素な数の個数は，
	//		1 の倍数を全て数え，そこから 2 の倍数の個数を引き，
	//		さらに 3 の倍数の個数を引き，引きすぎた 6 の倍数の個数を足す
	// ことにより数えることができる．
	ll res = 0;
	rep(bit, 1 << m) {
		// mul の倍数を考える．
		ll mul = 1;

		// mul が何個の素因数の積か．
		int ones = 0;

		rep(i, m) {
			if (bit & (1 << i)) {
				mul *= p[i];
				ones++;
			}
		}

		// 素因数の個数の偶奇で加減を切り替えつつ個数を数えていく．
		res += ((ones % 2) ? -1 : 1) * (r / mul - (l - 1) / mul);
	}
	return res;
}


//【位数[gcd]ごとの数え上げ】O(√n + d(n)^2 log d(n))　（d(n) : n の約数の個数）
/*
* Z/nZ に位数 i の元が何個あるかを cnt[i] に昇順に格納する．
* cnt[i] は Z/nZ に gcd(n, x) = n / i の元が何個あるかともみなせる．
*
* 利用：【約数列挙】
*/
void order_distribution(ll n, map<ll, ll>& cnt) {
	vl divs;
	divisors(n, divs);

	cnt.clear();
	repe(d, divs) {
		cnt[d] = d;

		repe(d2, divs) {
			if (d == d2) break;

			if (d % d2 == 0) cnt[d] -= cnt[d2];
		}
	}
}


