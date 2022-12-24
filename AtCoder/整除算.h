#pragma once
#include "header.h"
// ■■■■■ 整数除算，GCD 等 ■■■■■


//【商列挙】O(√n)
/*
* i=[1..n] に対し，n/i の商が q となる i の範囲が [i1..i2) であることを
* {q, i1, i2} として q について降順に格納したリストを返す．
* 各範囲においては余りは公差 -q の等差数列を成す．
*/
vector<tuple<ll, ll, ll>> quotient_range(ll n) {
	// verify : https://atcoder.jp/contests/abc230/tasks/abc230_e

	//【方法】
	// n/i の商が q となるような i の範囲を考える．条件を i について整理すると
	//		q = floor(n / i)
	//		⇔ q <= n / i < q + 1
	//		⇔ i q <= n < i(q + 1)
	//		⇔ n / (q + 1) < i <= n / q
	// となる．
	//
	// この幅が 1 以下であれば，q に対応する i は高々 1 個である．その条件は
	//		n / q - n / (q + 1) <= 1
	//		⇔ (q + 1)n - q n <= q(q + 1)
	//		⇔ n <= q(q + 1)
	// である．条件をやや弱めて
	//		n <= q^2
	//		⇔ √n <= q
	// としてもオーダーに影響はない．

	//（例）
	// 例えば n = 15 のときは以下のように分類できる：
	//		商 n/i	i の範囲		余り n%i
	//		15		[1..2)		[0]
	//		7		[2..3)		[1]
	//		5		[3..4)		[0]
	//		3		[4..6)		[3, 0]
	//		2		[6..8)		[3, 1]
	//		1		[8..16)		[7, 6, 5, 4, 3, 2, 1, 0]

	ll m = (ll)(sqrt(n) + 0.01);
	vector<tuple<ll, ll, ll>> qis;

	// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
	for (int i = 1; n / i > m; i++) {
		qis.push_back({ n / i, i, i + 1 });
	}

	// そうでない部分は q ごとにまとめて考える．
	repir(q, m, 1) {
		ll i0 = n / (q + 1LL) + 1;
		ll i1 = n / q + 1;
		qis.push_back({ q, i0, i1 });
	}

	return qis;
}


//【一次式の剰余の最小値】O(log(n + m + a + b))
/*
* min i∈[0..n) (a i + b) mod m を返す．
*/
ll mod_min_linear(ll n, ll m, ll a, ll b) {
	// verify : https://judge.yosupo.jp/problem/min_of_mod_of_linear

	Assert(m > 0);
	if (n <= 0) return INFL;

	a = smod(a, m); b = smod(b, m);

	ll res = b;

	while (a > 0) {
		// 単調増加な部分に分解し，その初項だけを並べた新たな列を考えると，
		// それもまた min i∈[0..nn) (na i + nb) mod nm の形で表される．
		ll nm = a;
		ll nn = (a * (n - 1) + b) / m;
		ll nb = a * (((m - b) / a) + 1) + b - m;
		ll na = (a * (((2 * m - b) / a) + 1) + b - 2 * m) - nb;

		n = nn; m = nm; a = smod(na, nm); b = nb % nm;
		dump(n, m, a, b);

		if (n == 0) break;

		// a が大きいときは左右反転することで，毎回大きさを半分以下にしていける．
		if (2 * a > m) {
			b = (a * (n - 1) + b) % m;
			a = (m - a) % m;
		}

		chmin(res, b);
	}

	return res;
}


//【m で割った余りの和】
/*
* Mod_sum(a) : O(n log n)
*	配列 a で初期化する．
*
* sum_mod(m) : O(max(a) log(n) / m)
*	a[0..n) mod m の和を返す．
*
* sum_lack(m) : O(max(a) log(n) / m)
*	a[0..n) を m で割った不足の和を返す．
*/
struct Mod_sum {
	vi a;    // ★ a でなくバケツで累積和を持てば O(log n) を落とせる．
	int n;
	ll asum; // a[0..n) の和

	// コンストラクタ（何もしない）
	Mod_sum() : n(0), asum(0) {}

	// 配列 a で初期化
	Mod_sum(const vi& a_) : a(a_), n(sz(a)), asum(0) {
		sort(all(a));
		rep(i, n) asum += a[i];
	}

	// a[0..n) mod m の和を返す．
	ll sum_mod(int m) {
		ll res = asum;

		for (ll v = m; v <= a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			res -= m * (ll)distance(lower_bound(all(a), v), a.end());
		}

		return res;
	}

	// a[0..n) を m で割った不足の和を返す．
	ll sum_lack(int m) {
		// sum : 1-indexed での a[0..n) mod m の和
		ll sum = asum;

		for (ll v = m; v < a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			sum -= m * (ll)distance(lower_bound(all(a), v + 1), a.end());
		}

		// 不足分を返す．
		return m * (ll)n - sum;
	}
};


//【長さ 2 の倍数列の数え上げ】O(√n)
/*
* 1 <= x | y <= n を満たす組 (x, y) の個数を返す．
*
* 利用：【商列挙】
*/
template<class T>
T count_multiple_2chain(ll n) {
	// 参考：http://oeis.org/A006218
	// verify : https://yukicoder.me/problems/no/1276

	//【方法】
	// x を固定すれば，条件を満たす y は n 以下の x の倍数全てなので，その個数は floor(n/x) である．
	// よって求めるべき値は
	//		Σx∈[1..n] floor(n/x)
	// である．
	// これは floor(n/x) の値が等しいところをまとめて計算することにより高速化できる．

	vector<tuple<ll, ll, ll>> qis;
	quotient_range(n, qis);

	T res = 0;

	repe(qi, qis) {
		ll q, i1, i2;
		tie(q, i1, i2) = qi;

		res += q * T(i2 - i1);
	}

	return res;
}


//【長さ 3 の倍数列の数え上げ】O(n^(2/3)) ?
/*
* 1 <= x | y | z <= n を満たす組 (x, y, z) の個数を返す．
*
* 利用：【整数累乗根】,【長さ 2 の倍数列の数え上げ】
*/
template<class T>
T count_multiple_3chain(ll n) {
	// 参考：http://oeis.org/A061201
	// verify : https://yukicoder.me/problems/no/1276

	int m = (int)integer_root(n, 3);

	T res = T(m) * m * m;

	repi(i, 1, m) {
		res += 3 * count_multiple_2chain<T>(n / i);
		repi(j, 1, m) {
			res -= 3 * (n / ((ll)i * j));
		}
	}

	return res;
}


//【切り捨て除算】O(1)
/*
* a, b の正負によらず，数学的な floor(a / b) を返す．
*/
template <class T>
T floor_div(T a, T b) {
	// verify : https://yukicoder.me/problems/no/2032

	return a / b - (T)(a % b < 0);
}


//【切り上げ除算】O(1)
/*
* a, b の正負によらず，数学的な ceil(a / b) を返す．
*/
template <class T>
T ceil_div(T a, T b) {
	// verify : https://yukicoder.me/problems/no/2099

	T num = a + b - 1;
	return num / b - (T)(num % b < 0);
}


//【切り捨て除算の結合法則】
/*
* 切り捨て除算は結合法則を満たす．すなわち以下の式が成り立つ：
*		floor(floor(a / b) / c) = floor(a / (b * c))
* 
* verify : https://atcoder.jp/contests/abc256/tasks/abc256_h
*/


//【切り捨て（余り指定）】O(1)
/*
* 与えられた x に対し，x 以下の y で y = k (mod m) を満たす最大のものを返す．
*/
template <class T>
T floor_mod(T x, T m, T k) {
	//【方法】
	// k = 0 の場合は
	//		y = x - (x mod m)
	// とすればよい．一般の k の場合は，k ずらして考えることにより
	//		y - k = (x - k) - ((x - k) mod m)
	// を得る．

	Assert(m > 0);

	k = smod(k, m);
	x -= k;
	T y = x - smod(x, m);

	return y + k;
}


//【切り上げ（余り指定）】O(1)
/*
* 与えられた x に対し，x 以上の y で y = k (mod m) を満たす最小のものを返す．
*/
template <class T>
T ceil_mod(T x, T m, T k) {
	//【方法】
	// k = 0 の場合は
	//		y = x + (-x mod m)
	// とすればよい．一般の k の場合は，k ずらして考えることにより
	//		y - k = (x - k) + (-(x - k) mod m)
	// を得る．

	Assert(m > 0);

	k = smod(k, m);
	x -= k;
	T y = x + smod(-x, m);

	return y + k;
}


//【余りが k である数の個数】O(1)
/*
* x∈[l..r) で x = k (mod m) を満たすものの個数を返す．
*/
template <class T>
T count_by_reminder(T l, T r, T m, T k) {
	// verify : https://codeforces.com/contest/628/problem/F
	
	//【方法】
	// l = k (mod m) になるように l を増加させても答えは変わらない．
	// こうすれば個数は [0..n) 内の m の倍数の数え上げと同様に考えて
	//		(r - l + m - 1) / m
	// で求められる．

	Assert(m > 0);
	if (l >= r) return 0;

	k = smod(k, m);

	l -= k;
	T l2 = l + smod(-l, m);
	l2 += k;

	return (r - l2 + m - 1) / m;
}


//【余りの取れる値の範囲】
/*
* 非負整数 a を m(<= a) で割った余りは a/2 未満になる．
*
* 証明：m <= a/2 のときは明らか．m > a/2 のときは
*		a mod m = a - m < a - a/2 = a/2
*
* verify : https://codeforces.com/contest/1617/problem/C
*/


//【gcd と階差】
/*
* 数列 a[0..n) の階差を d[0..n-1)（d[i] = a[i+1] - a[i]）とするとき，
*		gcd(a[l..r)) = gcd( a[i], gcd(d[l..r-1)) )（i∈[l..r)）
*
* verify : https://atcoder.jp/contests/arc017/tasks/arc017_4
*/


//【部分集合の gcd】
/*
* 与えられた非負整数の集合 U と非負整数 g について，
*	gcd(S) = g となるような S ⊂ U が存在する
*	⇔ g の倍数である U の元全ての gcd が g に一致する
*
* verify : https://codeforces.com/contest/1627/problem/D
*/


//【レピュニットの gcd】
/*
* rep(n) := (10^n - 1) / 9（1 が n 個並んだ数）とおくと，
*		gcd(rep(n), rep(m)) = rep(gcd(n, m))
*
* 証明：
* 筆算をイメージすることで自明に思える以下の式
*		rep(n) mod rep(m) = rep(n mod m)
* をユークリッドの互除法のように繰り返し用いればよい．
*
* verify : https://atcoder.jp/contests/arc050/tasks/arc050_c
*/


//【一次式の切り捨て和】O(log(n + m + a + b))
/*
* Σi∈[0..n) floor((a i + b) / m) を返す．
*/
ll floor_sum_linear(ll n, ll m, ll a, ll b) {
	// 参考：https://twitter.com/kyopro_friends/status/1304063876019793921?ref_src=twsrc%5Etfw

	//【方法】
	// m > 0, 0 <= a < m, 0 <= b < m として一般性を失わない．また i ← n - i とした
	//		 Σi∈(0..n] floor((a (n - i) + b) / m)
	// も値は変わらない．これは領域
	//		0 < x <= n
	//		0 < y <= (a (n - x) + b) / m
	// に含まれる格子点の個数と解釈できる．x と y の主従を入れ替えると
	//		0 < y <= (a n + b) / m
	//		0 < x <= (-m y + a n + b) / a
	// となる．ここに含まれる格子点の個数は
	//		Σi∈(0..floor((a n + b) / m)] floor((-m i + a n + b) / a)
	// である．n' = floor((a n + b) / m) とおき，i ← n' - i とした
	//		Σi∈[0..n') floor((-m (n' - i) + a n + b) / a)
	//		= Σi∈[0..n') floor((m i + (- m n' + a n + b)) / a)
	// も値は変わらない．これで分母がより小さい問題に帰着できた．
	//
	// 次のステップに進む前に m ← m % a とするので，収束の速さはユークリッドの互除法と同じである．

	Assert(m != 0);
	if (n <= 0) return 0;

	ll res = 0;

	// m < 0 の場合，分母分子を -1 倍して m > 0 とする．
	if (m < 0) { a *= -1; b *= -1; m *= -1; }

	// a を m だけ増減させた場合の影響は floor なしの和で計算できるので，0 <= a < m とする．
	res += (a / m - (ll)(a % m < 0)) * (n * (n - 1) / 2);
	a = smod(a, m);

	// b を m だけ増減させた場合の影響は floor なしの和で計算できるので，0 <= b < m とする．
	res += (b / m - (ll)(b % m < 0)) * n;
	b = smod(b, m);

	while (a > 0) {
		ll nn = (a * n + b) / m;
		ll nm = a;
		ll na = m;
		ll nb = -m * nn + a * n + b;

		res += (na / nm - (ll)(na % nm < 0)) * (nn * (nn - 1) / 2);
		na = smod(na, nm);

		res += (nb / nm - (ll)(nb % nm < 0)) * nn;
		nb = smod(nb, nm);

		n = nn; m = nm; a = na; b = nb;
	}

	return res;
}


