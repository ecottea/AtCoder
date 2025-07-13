#pragma once
#include "header.h"
// ■■■■■ スコア和（列） ■■■■■


//【連続整数の剰余の総和】O(1)
/*
* x∈[l..r) のうち x ≡ k (mod m) を満たすものの総和を返す．
*/
template <class T, class S = T>
S continuous_mod_sum(T l, T r, T m, T k) {
	// verify : https://atcoder.jp/contests/abc334/tasks/abc334_b

	Assert(m > 0);
	if (l >= r) return 0;

	l += smod(-l + k, m);
	r += smod(-r + k, m);
	T cnt = (r - l) / m;
	T sum = (l + r - m);
	S res = (cnt % 2 == 0) ? (S)(cnt / 2) * sum : (S)(sum / 2) * cnt;

	return res;
}


//【連続整数の剰余の総和（範囲指定）】O(1)
/*
* i∈[0..n) のうち i mod m ∈ [l..r) を満たすものの総和を返す．
*
* 制約：0 ≦ l, r ≦ m
*/
template <class T, class S = T>
S continuous_mod_range_sum(T n, T m, T l, T r) {
	// verify : https://yukicoder.me/problems/no/2489

	Assert(m > 0);
	if (l >= r || n <= 0) return 0;

	T Q = n / m;
	T R = n % m;

	T cnt0 = r - l;
	T sum0 = l + r - 1;
	S res = 0;
	if (cnt0 % 2 == 0) res = (S)(cnt0 / 2) * Q * (sum0 + m * (Q - 1));
	else if (Q % 2 == 0) res = (S)(Q / 2) * cnt0 * (sum0 + m * (Q - 1));
	else res = (S)((sum0 + m * (Q - 1)) / 2) * cnt0 * Q;

	l = m * Q + l;
	r = m * Q + min(r, R);
	if (l < r) {
		T cnt = r - l;
		T sum = l + r - 1;
		res += (cnt % 2 == 0) ? (S)(cnt / 2) * sum : (S)(sum / 2) * cnt;
	}

	return res;
}


//【一次式の切り捨て和】O(log(n + m))
/*
* Σi∈[0..n) floor((a i + b) / m) を返す．
*/
template <class T>
T arithmetic_floor_sum(T n, T m, T a, T b) {
	// 参考 : https://twitter.com/kyopro_friends/status/1304063876019793921?ref_src=twsrc%5Etfw
	// verify : https://judge.yosupo.jp/problem/sum_of_floor_of_linear

	//【方法】
	// m < 0 なら a, b, m をそれぞれ -1 倍して m > 0 とする．
	//		a = aq m + ar, b = bq m + br (0 ≦ ar, br < m)
	// と表すと，
	//		Σi∈[0..n) floor((a i + b) / m)
	//		= Σi∈[0..n) (floor((ar i + br) / m) + (aq i + bq))
	//		= Σi∈[0..n) floor((ar i + br) / m) + (aq n(n-1)/2 + bq n)
	// となるので 0 ≦ a < m, 0 ≦ b < m として一般性を失わない．
	// 
	// 求めるべき値は，領域
	//		{(x, y) | 0 ≦ x < n かつ 0 < y ≦ (a x + b) / m}
	// に含まれる格子点の個数である．u1 = floor((a x + b) / m) とおき，変数変換
	//		v = n - x, u = u1 - y
	// を施すと，直線 y = (a x + b) / m の式は
	//		u1 - u = (a (n - v) + b) / m
	//		⇔ m u1 - m u = a n - a v + b
	//		⇔ a v = m u + a n + b - m u1
	//		⇔ v = (m u + (a n + b - m u1)) / a
	// と書き換えられるので，先の領域は
	//		{(u, v) | 0 ≦ u < u1 かつ 0 < v ≦ (m u + (a n + b - m u1)) / a}
	// となる．ここに含まれる格子点の個数は
	//		Σi∈[0..u1) floor((m i + (a n + b - m u1)) / a)
	// であり，分母を m からより小さい a に書き換えられた．
	//
	// 次のステップに進む前に m ← m mod a とするので，収束の速さはユークリッドの互除法と同じである．

	Assert(m != 0);
	if (n <= 0) return 0;

	T res = 0;

	// m < 0 の場合，分母分子を -1 倍して m > 0 とする．
	if (m < 0) { a *= -1; b *= -1; m *= -1; }

	// a を m だけ増減させた場合の影響は floor なしの和で計算できるので，0 ≦ a < m とする．
	res += (a / m - (T)(a % m < 0)) * (n * (n - 1) / 2);
	a = smod(a, m);

	// b を m だけ増減させた場合の影響は floor なしの和で計算できるので，0 ≦ b < m とする．
	res += (b / m - (T)(b % m < 0)) * n;
	b = smod(b, m);

	while (a > 0) {
		T nn = (a * n + b) / m;
		T nm = a;
		T na = m;
		T nb = a * n + b - m * nn;

		res += (na / nm) * (nn * (nn - 1) / 2);
		na %= nm;

		res += (nb / nm) * nn;
		nb %= nm;

		n = nn; m = nm; a = na; b = nb;
	}

	return res;
}


//【一次式の線形加重 & 平方切り捨て和】O(log(n + m))
/*
*	S1 = Σi∈[0..n) i floor((a i + b) / m)
*	S2 = Σi∈[0..n) floor((a i + b) / m)^2
*	S3 = Σi∈[0..n) floor((a i + b) / m)
* とおき，3 つ組 (S1, S2, S3) を返す．
*/
template <class T>
tuple<T, T, T> arithmetic_linear_square_floor_sum(T n, T m, T a, T b) {
	// verify : https://yukicoder.me/problems/no/2362

	//【方法】
	//【一次式の切り捨て和】と同じように分母がより小さい問題への帰着を目指す．
	// ただし同じ形に帰着できるわけではないので，
	//		S1 = Σi∈[0..n) i floor((a i + b) / m)
	//		S2 = Σi∈[0..n) floor((a i + b) / m)^2
	//		S3 = Σi∈[0..n) floor((a i + b) / m) （通常の FloorSum）
	// の 3 つを並行して計算していく．

	Assert(m != 0);
	if (n <= 0) return make_tuple(T(0), T(0), T(0));

	// m < 0 の場合，分母分子を -1 倍して m > 0 とする．
	if (m < 0) { a *= -1; b *= -1; m *= -1; }

	// a を m だけ増減させた場合の影響は floor なしの和で計算できるので，0 ≦ a < m とする．
	T A = a / m - (T)(a % m < 0);
	a = smod(a, m);

	// b を m だけ増減させた場合の影響は floor なしの和で計算できるので，0 ≦ b < m とする．
	T B = b / m - (T)(b % m < 0);
	b = smod(b, m);

	function<tuple<T, T, T>(T, T, T, T)> rf1, rf2;

	// a ≧ m, b ≧ 0 用
	rf1 = [&](T n, T m, T a, T b) {
		if (n == 0) return make_tuple(T(0), T(0), T(0));

		T A = a / m, B = b / m;
		T n3 = n * (n - 1) * (2 * n - 1) / 6, n2 = n * (n - 1) / 2;

		T s1 = A * n3 + B * n2;
		T s2 = A * A * n3 + 2 * A * B * n2 + B * B * n;
		T s3 = A * n2 + B * n;

		a %= m; b %= m;
		auto [ns1, ns2, ns3] = rf2(n, m, a, b);

		s1 += ns1;
		s2 += ns2 + 2 * A * ns1 + 2 * B * ns3;
		s3 += ns3;

		return make_tuple(s1, s2, s3);
	};

	// 0 ≦ a < m, 0 ≦ b < m 用
	rf2 = [&](T n, T m, T a, T b) {
		if (a == 0) return make_tuple(T(0), T(0), T(0));

		T nn = (a * n + b) / m;
		T nm = a;
		T na = m;
		T nb = a * n + b - m * nn;
		auto [ns1, ns2, ns3] = rf1(nn, nm, na, nb);

		T s1 = ((2 * n - 1) * ns3 - ns2) / 2;
		T s2 = (2 * nn - 1) * ns3 - 2 * ns1;
		T s3 = ns3;

		return make_tuple(s1, s2, s3);
	};

	auto [s1, s2, s3] = rf2(n, m, a, b);

	T n2 = n * (n - 1) / 2;
	T n3 = n * (n - 1) * (2 * n - 1) / 6;

	s2 += 2 * A * s1;
	s2 += 2 * B * s3;
	s2 += A * A * n3;
	s2 += 2 * A * B * n2;
	s2 += B * B * n;

	s1 += A * n3;
	s1 += B * n2;

	s3 += A * n2;
	s3 += B * n;

	return { s1, s2, s3 };
}


//【一次式の累乗切り捨て和】O((P Q)^2 log(n + m))
/*
* Σi∈[0..n) i^P floor((a i + b) / m)^Q を返す．
*
* 利用：【直線に沿った格子路上の積（モノイド）】
*/
using T_apfs = mint; // 戻り値の型
T_apfs bin_apfs[21][21] = { // 足りなければ適宜追加する．
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,4,6,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,5,10,10,5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,6,15,20,15,6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,7,21,35,35,21,7,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,8,28,56,70,56,28,8,1,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,9,36,84,126,126,84,36,9,1,0,0,0,0,0,0,0,0,0,0,0},
	{1,10,45,120,210,252,210,120,45,10,1,0,0,0,0,0,0,0,0,0,0},
	{1,11,55,165,330,462,462,330,165,55,11,1,0,0,0,0,0,0,0,0,0},
	{1,12,66,220,495,792,924,792,495,220,66,12,1,0,0,0,0,0,0,0,0},
	{1,13,78,286,715,1287,1716,1716,1287,715,286,78,13,1,0,0,0,0,0,0,0},
	{1,14,91,364,1001,2002,3003,3432,3003,2002,1001,364,91,14,1,0,0,0,0,0,0},
	{1,15,105,455,1365,3003,5005,6435,6435,5005,3003,1365,455,105,15,1,0,0,0,0,0},
	{1,16,120,560,1820,4368,8008,11440,12870,11440,8008,4368,1820,560,120,16,1,0,0,0,0},
	{1,17,136,680,2380,6188,12376,19448,24310,24310,19448,12376,6188,2380,680,136,17,1,0,0,0},
	{1,18,153,816,3060,8568,18564,31824,43758,48620,43758,31824,18564,8568,3060,816,153,18,1,0,0},
	{1,19,171,969,3876,11628,27132,50388,75582,92378,92378,75582,50388,27132,11628,3876,969,171,19,1,0},
	{1,20,190,1140,4845,15504,38760,77520,125970,167960,184756,167960,125970,77520,38760,15504,4845,1140,190,20,1}
};
int ex_apfs, ey_apfs;
struct S_apfs {
	vector<T_apfs> v = vector<T_apfs>((ex_apfs + 1) * (ey_apfs + 1));
	T_apfs f = 0, g = 0;

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const S_apfs& x) {
		os << "(" << x.v << "," << x.f << "," << x.g << ")";
		return os;
	}
#endif
};
S_apfs op_apfs(S_apfs b, S_apfs a) {
	vector<T_apfs> bf_pow(ex_apfs + 1);
	bf_pow[0] = T_apfs(1);
	repi(i, 1, ex_apfs) bf_pow[i] = bf_pow[i - 1] * b.f;

	vector<T_apfs> bg_pow(ey_apfs + 1);
	bg_pow[0] = T_apfs(1);
	repi(i, 1, ey_apfs) bg_pow[i] = bg_pow[i - 1] * b.g;

	repi(jx, 0, ex_apfs) repi(jy, 0, ey_apfs) {
		repi(ix, jx, ex_apfs) repi(iy, jy, ey_apfs) {
			b.v[jx * (ey_apfs + 1) + jy] += a.v[ix * (ey_apfs + 1) + iy]
				* bin_apfs[ix][jx] * bf_pow[ix - jx]
				* bin_apfs[iy][jy] * bg_pow[iy - jy];
		}
	}

	b.f += a.f;
	b.g += a.g;

	return b;
}
S_apfs e_apfs() {
	S_apfs a;
	return a;
}
template <class T>
T_apfs arithmetic_powered_floor_sum(T n, T m, T a, T b, int P, int Q) {
	// 参考 : https://qiita.com/sounansya/items/51b39e0d7bf5cc194081
	// verify : https://yukicoder.me/problems/no/2996

	//【方法】
	// i^p floor((ai+b)/m)^q も一緒に計算していくことで行列積とみなせる．
	// クロネッカー積分解を考えることで計算量を落とせる．

	if (n <= 0) return T_apfs(0);

	Assert(m != 0);

	if (m < 0) {
		m = -m;
		a = -a;
		b = -b;
	}

	ex_apfs = P;
	ey_apfs = Q;

	S_apfs f;
	repi(i, 0, P) f.v[i * (Q + 1) + Q] = bin_apfs[P][i];
	f.f = T_apfs(1);

	S_apfs g;
	repi(i, 1, Q) g.v[(P + 1) * (Q + 1) - 1 - i] = bin_apfs[Q][i];
	g.g = T_apfs(1);

	// a < 0 のときは Σi∈[0..n) i^P (-floor((a i + b) / m))^Q を求め，後で (-1)^Q 倍する．
	if (a < 0) b = m - T(1) - b;

	T br = smod(b, m);
	T bq = (b - br) / m;

	// (0, b/m) → (n-1, (a(n-1)+b)/m) の移動に対応する行列積を計算する．
	auto h = multiple_along_line<ll, S_apfs, op_apfs, e_apfs>(n - 1, m, abs(a), br, f, g);

	// (0, 0) → (0, b/m) の移動に対応する行列を右から掛ける．
	T_apfs res(0); T_apfs bq_pow(1);
	repi(i, 0, Q) {
		res += h.v[i] * bq_pow;
		if (i < Q) bq_pow *= bq;
	}
	if (P == 0) res += bq_pow;

	if ((a < 0) && (Q & 1)) res *= T_apfs(-1);

	return res;
}


//【一次式の積の切り捨て和】O(log(n + m))
/*
* Σi∈[0..n) floor((a i + b1) / m) floor((a i + b2) / m) を返す．
*
* 利用：【一次式の線形加重 & 平方切り捨て和】
*/
template <class T>
T arithmetic_multiple_floor_sum(T n, T m, T a, T b1, T b2) {
	// verify : https://atcoder.jp/contests/arc182/tasks/arc182_e

	//【方法】
	// 0 ≦ b1 ≦ b2 < m とすれば，恒等式
	//		y(y+1) = 1/2 (y^2 + (y+1)^2 + y - (y+1))
	//		y y = 1/2 (y^2 + y^2 + y - y)
	// を用いて積を分離できる．

	Assert(m != 0);
	if (n <= 0) return T(0);

	// m < 0 の場合，分母分子を -1 倍して m > 0 とする．
	if (m < 0) { a *= -1; b1 *= -1; b2 *= -1; m *= -1; }

	// b を m だけ増減させた場合の影響は floor なしの和で計算できるので，0 ≦ b < m とする．
	T B1 = b1 / m - (T)(b1 % m < 0); b1 = smod(b1, m);
	T B2 = b2 / m - (T)(b2 % m < 0); b2 = smod(b2, m);

	// 0 ≦ b1 ≦ b2 < m とする．
	if (b1 > b2) { swap(b1, b2); swap(B1, B2); }

	auto [s1_ln, s1_sq, s1] = arithmetic_linear_square_floor_sum(n, m, a, b1);
	auto [s2_ln, s2_sq, s2] = arithmetic_linear_square_floor_sum(n, m, a, b2);

	T res = 0;
	res += (s1_sq + s2_sq + s1 - s2) / 2;
	res += B1 * s2;
	res += B2 * s1;
	res += B1 * B2 * n;

	return res;
}


//【一次式の剰余の総和】O(log(n + m))（オーバーフロー注意）
/*
* Σi∈[0..n) ((a i + b) mod m) を返す．
*
* 利用：【一次式の切り捨て和】
*/
template <class T>
T arithmetic_mod_sum(T n, T m, T a, T b) {
	//【方法】
	// 剰余と切り捨て商との関係より
	//		Σi∈[0..n) (a i + b) mod m
	//		= Σi∈[0..n) (a i + b) - m Σi∈[0..n) floor((a i + b) / m)
	//		= a n(n-1)/2 + b n - m Σi∈[0..n) floor((a i + b) / m)
	// を得る．よって floor_sum を利用できる．

	Assert(m > 0);
	if (n <= 0) return 0;

	a = smod(a, m);
	b = smod(b, m);

	T res = a * n * (n - 1) / 2 + b * n;
	res -= m * arithmetic_floor_sum(n, m, a, b);

	return res;
}


//【一次式の剰余の総和（範囲指定）】O(log(n + m))（オーバーフロー注意）
/*
* 各 i∈[0..n) に対する (a i + b) mod m のうち，値が [l..r) に属するものの総和を返す．
*
* 利用：【一次式の線形加重 & 平方切り捨て和】,【一次式の積の切り捨て和】
*/
template <class T>
T arithmetic_mod_sum(T n, T m, T a, T b, T l, T r) {
	// verify : https://atcoder.jp/contests/arc182/tasks/arc182_e

	//【方法】
	// 条件を同値変形していくと，
	//		l ≦ (ai+b) mod m < r
	//		⇔ l ≦ (ai+b) - floor((ai+b)/m) * m < r
	//		⇔ (ai+b-l)/m ≧ floor((ai+b)/m) > (ai+b-r)/m
	// となる．中辺が整数であることと
	//		(左辺) - (右辺) = (r-l)/m ≦ 1
	// であることに注意すると，
	//		(ai+b) mod m ∈ [l..r)  ⇔ floor((ai+b-l)/m) - floor((ai+b-r)/m) = 1
	//		(ai+b) mod m !∈ [l..r) ⇔ floor((ai+b-l)/m) - floor((ai+b-r)/m) = 0
	// が分るので，これに重み (ai+b) mod m を付けて足し合わせれば良い．
	//		((ai+b) mod m) floor((ai+b')/m)
	//		= (ai+b - floor((ai+b)/m) m) floor((ai+b')/m)
	// なので【一次式の線形加重切り捨て和】や【一次式の積の切り捨て和】が利用できる．

	Assert(m > 0);

	if (n <= 0) return 0;

	chmax(l, T(0)); chmin(r, m);
	if (l >= r) return 0;

	T res = 0;

	auto [l1, l2, l3] = arithmetic_linear_square_floor_sum(n, m, a, b - l);
	res += a * l1 + b * l3 - m * arithmetic_multiple_floor_sum(n, m, a, b - l, b);

	auto [r1, r2, r3] = arithmetic_linear_square_floor_sum(n, m, a, b - r);
	res -= a * r1 + b * r3 - m * arithmetic_multiple_floor_sum(n, m, a, b - r, b);

	return res;
}


//【連続整数の popcount の総和】O(log N)
/*
* Σi∈[0..N) popcount(i) を返す．
*/
template <class T>
T continuous_popcount_sum(ll N) {
	// verify : https://yukicoder.me/problems/no/2939

	//【方法】
	// ビット毎に独立に寄与を計算する．
	// 第 k ビットは ['0'×k, '1'×k] を 1 周期として変化することを利用できる．

	T res = T(0);

	repi(k, 0, msb(N)) {
		// w : 周期
		ll w = 1LL << (k + 1);
		ll hw = w / 2;

		ll q = N / w;
		ll r = N % w;

		res += T(q) * hw;
		res += T(max(r - hw, 0LL));
	}

	return res;
}


//【一次式の popcount の総和】O(log(an+b) log n)
/*
* Σi∈[0..n) popcount(a i + b) を返す．
*
* 利用：【一次式の切り捨て和】
*/
template <class T>
T arithmetic_popcount_sum(ll n, ll a, ll b) {
	// verify : https://atcoder.jp/contests/abc283/tasks/abc283_h

	//【方法】
	// popcount(x) は
	//		popcount(x) = x - Σk∈[1..∞) floor(x / 2^k)
	// と表すことができる．これを用いて変形すると，
	//		Σi∈[0..n) popcount(ai+b)
	//		= Σi∈[0..n) ((ai+b) - Σk∈[1..∞) floor((ai+b) / 2^k))
	//		= Σi∈[0..n) (ai+b) - Σk∈[1..∞) Σi∈[0..n) floor((ai+b) / 2^k)
	// となる．第一項は等差数列の和の公式より
	//		Σi∈[0..n) (ai+b) = a n(n-1)/2 + b n
	// となり，第二項は k 毎に floor_sum を用いれば良い．

	T res = a * (n & 1 ? (T((n - 1) / 2) * n) : T(n / 2) * (n - 1));
	res += T(b) * n;

	int K = msb(a * (n - 1) + b);
	repi(k, 1, K) res -= arithmetic_floor_sum<T>(n, 1LL << k, a, b);

	return res;
}


//【連続整数の総 XOR】O(1)
/*
* XOR[0..n) を返す．
*/
template <class T>
T continuous_XOR(T n) {
	// verify : https://atcoder.jp/contests/abc121/tasks/abc121_d

	//【方法】
	// s[i] := XOR[0..i) とおく．
	//		(2 k) XOR (2 k + 1) = 1
	// に注意すると，s[i] が周期 4 をもつことが分かり，
	//		s[4 k]     = 0
	//		s[4 k + 1] = 4 k
	//		s[4 k + 2] = 1
	//		s[4 k + 3] = 4 k + 3
	// と簡単に表すことができる．

	if (n <= 0) return 0;

	T res = -1;
	if (n % 4 == 0) res = 0;
	else if (n % 4 == 1) res = n - 1;
	else if (n % 4 == 2) res = 1;
	else if (n % 4 == 3) res = n;

	return res;
}


//【一次式の総 XOR】O(log(an+b) log n)
/*
* XOR_i∈[0..n) (ai+b) を返す．
*
* 利用：【一次式の切り捨て和】
*/
template <class T>
T arithmetic_XOR(T n, T a, T b) {
	// verify : https://mojacoder.app/users/Tonegawac/problems/linear-xor

	//【方法】
	// ビット毎に独立に寄与を計算する．
	// 第 k ビットが 1 かどうかは，Σi∈[0..n) (ai+b)/2^k の偶奇から判断できる．

	int K = msb(a * (n - 1) + b);

	T res = 0;
	repi(k, 0, K) {
		ll val = arithmetic_floor_sum(n, 1LL << k, a, b);
		if (val & 1LL) res |= T(1) << k;
	}

	return res;
}


