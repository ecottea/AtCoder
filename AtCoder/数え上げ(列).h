#pragma once
#include "header.h"
#include "整除算.h"
#include "二項係数.h"
// ■■■■■ 数え上げ（列） ■■■■■


//【連続整数の剰余の数え上げ】O(1)
/*
* x∈[l..r) のうち x ≡ k (mod m) を満たすものの個数を返す．
*/
template <class T>
T count_by_mod(T l, T r, T m, T k) {
	// verify : https://atcoder.jp/contests/abc334/tasks/abc334_b

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


//【連続整数の剰余の数え上げ（範囲指定）】O(1)
/*
* i∈[0..n) のうち i mod m ∈ [l..r) を満たすものの個数を返す．
* 
* 制約：0 ≦ l, r ≦ m
*/
template <class T>
T count_by_mod_range(T n, T m, T l, T r) {
	// verify : https://atcoder.jp/contests/agc072/tasks/agc072_c

	//【方法】
	// n = m Q + R と表す．
	// i∈[0..m Q) なる i については (r-l)Q 個ある．
	// 残りについては max(min(r,R)-l, 0) 個ある．

	Assert(m > 0);
	if (l >= r || n <= 0) return 0;

	T Q = n / m;
	T R = n % m;

	return (r - l) * Q + max(min(r, R) - l, T(0));
}


//【一次式の剰余の数え上げ（範囲指定）】O(log(n + m))
/*
* 各 i∈[0..n) に対する (a i + b) mod m のうち，値が [l..r) に属するものの個数を返す．
*
* 利用：【一次式の切り捨て和】
*/
template <class T>
T count_arithmetic_mod(T n, T m, T a, T b, T l, T r) {
	// 参考 : https://twitter.com/maspy_stars/status/1649421402573766656
	// verify : https://yukicoder.me/problems/no/2280

	//【方法】
	// 条件を同値変形していくと，
	//		l ≦ (ai+b) mod m < r
	//		⇔ l ≦ (ai+b) - floor((ai+b)/m) * m < r
	//		⇔ (ai+b-l)/m ≧ floor((ai+b)/m) > (ai+b-r)/m
	// となる．中辺が整数であることと
	//		(左辺) - (右辺) = (r-l)/m ≦ 1
	// であることに注意すると，
	//		(ai+b) mod m ∈ [l..r) ⇔ floor((ai+b-l)/m) - floor((ai+b-r)/m) = 1
	//		(ai+b) mod m !∈ [l..r) ⇔ floor((ai+b-l)/m) - floor((ai+b-r)/m) = 0
	// が分かる．よって floor_sum の差を取れば良い．

	Assert(m > 0);

	if (n <= 0) return 0;

	chmax(l, T(0)); chmin(r, m);
	if (l >= r) return 0;

	a = smod(a, m); b = smod(b, m);

	T res = arithmetic_floor_sum(n, m, a, b - l);
	res -= arithmetic_floor_sum(n, m, a, b - r);

	return res;
}


//【広義単調増加列の数え上げ（上限指定，mod 998244353）】O((n+m)log(n+m)^2) (m = a_max[n-1])
/*
* 各 i について 0 ≦ a[i] ≦ a_max[i] を満たす広義単調増加列 a[0..n) の個数を返す．
*
* 制約：fm は (2 max(n, m))! まで計算可能
*
*（分割統治法）
*/
mint count_increase_sequence(vi a_max, const Factorial_mint& fm) {
	// 参考 : https://noshi91.hatenablog.com/entry/2023/07/21/235339
	// verify : https://yukicoder.me/problems/no/3370

	int n = sz(a_max);
	if (n == 0) return 1;

	int m = a_max[n - 1];

	// a_max[0..n) を広義単調増加に整形する．
	repir(i, n - 2, 0) chmin(a_max[i], a_max[i + 1]);
	if (a_max[0] < 0) return 0;

	vm dp_x(n + 1), dp_y(m + 1);
	dp_x[0] = 1;

	function<void(int, int, int, int)> rect = [&](int x1, int x2, int y1, int y2) {
		int w = x2 - x1, h = y2 - y1;

		int K = 1 << (msb(h + w) + 1);
		mint Kinv = mint(K).inv();

		vm ndp_x(w + 1), ndp_y(h + 1);

		// 下 → 上
		{
			vm d(w + 1);
			repi(i, x1, x2) d[i - x1] = dp_x[i];

			vm c(w + 1);
			repi(i, 0, w) c[i] = fm.bin(i + h, i);

			vm u = convolution(d, c);
			repi(i, 0, w) ndp_x[i] += u[i];
		}

		// 左 → 上
		{
			vm l(K);
			repi(j, y1, y2) l[j - y1] = dp_y[j] * fm.fact_inv(h - (j - y1));

			vm c(K);
			repi(i, 0, h + w) c[i] = fm.fact(i);

			// middle product
			internal::butterfly(l);
			internal::butterfly(c);
			rep(k, K) l[k] *= c[k];
			internal::butterfly_inv(l);

			repi(i, 0, w) ndp_x[i] += l[i + h] * fm.fact_inv(i) * Kinv;
		}

		// 左 → 右
		{
			vm l(h + 1);
			repi(j, y1, y2) l[j - y1] = dp_y[j];

			vm c(h + 1);
			repi(j, 0, h) c[j] = fm.bin(j + w, j);

			vm r = convolution(l, c);
			repi(j, 0, h) ndp_y[j] += r[j];
		}

		// 下 → 右
		{
			vm d(K);
			repi(i, x1, x2) d[i - x1] = dp_x[i] * fm.fact_inv(w - (i - x1));

			vm c(K);
			repi(j, 0, h + w) c[j] = fm.fact(j);

			// middle product
			internal::butterfly(d);
			internal::butterfly(c);
			rep(k, K) d[k] *= c[k];
			internal::butterfly_inv(d);

			repi(j, 0, h) ndp_y[j] += d[j + w] * fm.fact_inv(j) * Kinv;
		}

		repi(i, 0, w) dp_x[i + x1] = ndp_x[i];
		repi(j, 0, h) dp_y[j + y1] = ndp_y[j];
	};

	function<void(int, int, int, int)> naive = [&](int x1, int x2, int y1, int y2) {
		int w = x2 - x1, h = y2 - y1;

		vvm dp(w + 2, vm(h + 2));
		repi(i, x1, x2) dp[i - x1 + 1][0] = dp_x[i];
		repi(j, y1, y2) dp[0][j - y1 + 1] = dp_y[j];

		repi(i, x1, x2) {
			int j_max = min(y2, i < n ? a_max[i] : m);
			repi(j, y1, j_max) {
				dp[i - x1 + 1][j - y1 + 1] = dp[i - x1][j - y1 + 1] + dp[i - x1 + 1][j - y1];
			}
		}

		repi(i, x1, x2) dp_x[i] = dp[i - x1 + 1][h + 1];
		repi(j, y1, y2) dp_y[j] = dp[w + 1][j - y1 + 1];
	};

	function<void(int, int, int, int)> rf = [&](int x1, int x2, int y1, int y2) {
		if ((ll)(x2 - x1) * (y2 - y1) <= 180) {
			naive(x1, x2, y1, y2);
			return;
		}

		if (a_max[x1] == y2) {
			rect(x1, x2, y1, y2);
			return;
		}

		int xm = (x1 + x2) / 2;
		int ym = a_max[xm];

		if (y1 <= ym) {
			rf(x1, xm, y1, ym);
			rect(xm + 1, x2, y1, ym);
		}
		if (ym + 1 <= y2) rf(xm + 1, x2, ym + 1, y2);
	};
	rf(0, n, 0, a_max[n - 1]);

	return dp_y[m];
}


//【広義単調増加列の数え上げ（上下限指定，mod 998244353）】O((n+m)log(n+m)^2) (m = a_max[n-1]-a_min[0])
/*
* 各 i について a_min[i] ≦ a[i] ≦ a_max[i] を満たす広義単調増加列 a[0..n) の個数を返す．
*
* 制約：fm は (2 max(n, m))! まで計算可能
*
*（分割統治法）
*/
mint count_increase_sequence(vi a_min, vi a_max, const Factorial_mint& fm) {
	// 参考 : https://noshi91.hatenablog.com/entry/2023/07/21/235339
	// verify : https://judge.yosupo.jp/problem/number_of_increasing_sequences_between_two_sequences

	int n = sz(a_min);

	// a_min[0..n), a_max[0..n) を広義単調増加に整形する．
	repi(i, 1, n - 1) chmax(a_min[i], a_min[i - 1]);
	repir(i, n - 2, 0) chmin(a_max[i], a_max[i + 1]);

	// a_min[0] = 0 になるように平行移動する．
	repir(i, n - 1, 0) {
		if (a_min[i] > a_max[i]) return 0;
		a_max[i] -= a_min[0];
		a_min[i] -= a_min[0];
	}
	int m = a_max[n - 1];

	// 横から見た場合の上下限を求める．
	vi b_min(m, n), b_max(m, n);
	rep(i, n) {
		int j_min = i == 0 ? 0 : a_max[i - 1];
		repi(j, j_min, a_max[i] - 1) b_min[j] = i;

		j_min = i == 0 ? 0 : a_min[i - 1];
		repi(j, j_min, a_min[i] - 1) b_max[j] = i;
	}

	vm dp_x(n + 1), dp_y(m + 1);
	dp_x[0] = 1;

	function<void(int, int, int, int)> rect = [&](int x1, int x2, int y1, int y2) {
		int w = x2 - x1, h = y2 - y1;

		int K = 1 << (msb(h + w) + 1);
		mint Kinv = mint(K).inv();

		vm ndp_x(w + 1), ndp_y(h + 1);

		// 下 → 上
		{
			vm d(w + 1);
			repi(i, x1, x2) d[i - x1] = dp_x[i];

			vm c(w + 1);
			repi(i, 0, w) c[i] = fm.bin(i + h, i);

			vm u = convolution(d, c);
			repi(i, 0, w) ndp_x[i] += u[i];
		}

		// 左 → 上
		{
			vm l(K);
			repi(j, y1, y2) l[j - y1] = dp_y[j] * fm.fact_inv(h - (j - y1));

			vm c(K);
			repi(i, 0, h + w) c[i] = fm.fact(i);

			// middle product
			internal::butterfly(l);
			internal::butterfly(c);
			rep(k, K) l[k] *= c[k];
			internal::butterfly_inv(l);

			repi(i, 0, w) ndp_x[i] += l[i + h] * fm.fact_inv(i) * Kinv;
		}

		// 左 → 右
		{
			vm l(h + 1);
			repi(j, y1, y2) l[j - y1] = dp_y[j];

			vm c(h + 1);
			repi(j, 0, h) c[j] = fm.bin(j + w, j);

			vm r = convolution(l, c);
			repi(j, 0, h) ndp_y[j] += r[j];
		}

		// 下 → 右
		{
			vm d(K);
			repi(i, x1, x2) d[i - x1] = dp_x[i] * fm.fact_inv(w - (i - x1));

			vm c(K);
			repi(j, 0, h + w) c[j] = fm.fact(j);

			// middle product
			internal::butterfly(d);
			internal::butterfly(c);
			rep(k, K) d[k] *= c[k];
			internal::butterfly_inv(d);

			repi(j, 0, h) ndp_y[j] += d[j + w] * fm.fact_inv(j) * Kinv;
		}

		repi(i, 0, w) dp_x[i + x1] = ndp_x[i];
		repi(j, 0, h) dp_y[j + y1] = ndp_y[j];
	};

	function<void(int, int, int, int)> naive = [&](int x1, int x2, int y1, int y2) {
		int w = x2 - x1, h = y2 - y1;

		vvm dp(w + 2, vm(h + 2));
		repi(i, x1, x2) dp[i - x1 + 1][0] = dp_x[i];
		repi(j, y1, y2) dp[0][j - y1 + 1] = dp_y[j];

		repi(i, x1, x2) {
			int j_min = max(y1, i > 0 ? a_min[i - 1] : 0);
			int j_max = min(y2, i < n ? a_max[i] : m);
			repi(j, j_min, j_max) {
				dp[i - x1 + 1][j - y1 + 1] = dp[i - x1][j - y1 + 1] + dp[i - x1 + 1][j - y1];
			}
		}

		repi(i, x1, x2) dp_x[i] = dp[i - x1 + 1][h + 1];
		repi(j, y1, y2) dp_y[j] = dp[w + 1][j - y1 + 1];
	};

	function<void(int, int, int, int)> rf = [&](int x1, int x2, int y1, int y2) {
		if ((ll)(x2 - x1) * (y2 - y1) <= 180) {
			naive(x1, x2, y1, y2);
			return;
		}

		if (a_max[x1] >= y2 && y1 >= a_min[x2 - 1]) {
			rect(x1, x2, y1, y2);
			return;
		}

		if (x2 - x1 > y2 - y1) {
			int xm = (x1 + x2) / 2;

			int ny2 = min(a_max[xm], y2);
			if (y1 <= ny2) rf(x1, xm, y1, ny2);

			int ny1 = max(a_min[xm], y1);
			if (ny1 <= y2) rf(xm + 1, x2, ny1, y2);
		}
		else {
			int ym = (y1 + y2) / 2;

			int nx2 = min(b_max[ym], x2);
			if (x1 <= nx2) rf(x1, nx2, y1, ym);

			int nx1 = max(b_min[ym], x1);
			if (nx1 <= x2) rf(nx1, x2, ym + 1, y2);
		}
	};
	rf(0, n, 0, m);

	return dp_y[m];
}


//【倍数列の数え上げ（長さ 2）】O(√n)
/*
* 1 ≦ x | y ≦ n を満たす組 (x, y) の個数を返す．
*
* 利用：【商列挙】
*/
template<class T>
T count_multiple_sequence_len2(ll n) {
	// 参考 : http://oeis.org/A006218
	// verify : https://yukicoder.me/problems/no/1276

	//【方法】
	// x を固定すれば，条件を満たす y は n 以下の x の倍数全てなので，その個数は floor(n/x) である．
	// よって求めるべき値は
	//		Σx∈[1..n] floor(n/x)
	// である．
	// これは floor(n/x) の値が等しいところをまとめて計算することにより高速化できる．

	T res = 0;

	auto f = [&](ll il, ll ir, ll q) {
		res += q * T(ir - il);
	};
	quotient_range(n, f);

	return res;
}


//【倍数列の数え上げ（長さ 3）】O(n^(2/3))
/*
* 1 ≦ x | y | z ≦ n を満たす組 (x, y, z) の個数を返す．
*
* 利用：【商列挙】
*/
template<class T>
T count_multiple_sequence_len3(ll n) {
	// 参考 : http://oeis.org/A061201
	// verify : https://yukicoder.me/problems/no/1276

	ll m = (ll)(pow(n, 1. / 3) + 1e-12); // 上下どちらにもズレちゃだめ

	T res = T(m) * m * m;

	repi(i, 1, m) {
		auto f = [&](ll il, ll ir, ll q) {
			chmax(il, m);
			res += T(3) * q * max(ir - il, 0LL);
		};
		quotient_range(n / i, f);
	}

	return res;
}


//【倍数列の数え上げ（真の倍数，初項と項数ごと）】O(n (log n)^2)
/*
* 各 i∈[0..n] について，各項が n 以下の自然数である真の倍数列で，
* 初項が i で項数が j であるものの個数を cnt[i][j] に格納し cnt を返す．
*/
vvm count_multiple_sequence(int n) {
	// verify : https://atcoder.jp/contests/arc116/tasks/arc116_c

	vvm dp(n + 1);

	repir(i, n, 1) {
		int j_max = msb(n / i) + 1;
		dp[i].resize(j_max + 1);

		dp[i][1] = 1;
		repi(j, 2, j_max) {
			for (int k = 2; (ll)i * k <= n && sz(dp[i * k]) >= j; k++) {
				dp[i][j] += dp[i * k][j - 1];
			}
		}
	}

	return dp;
}


//【貪欲増加部分列長が指定された列の数え上げ】
/*
* 各要素が [0..k) の元である列 a[0..n) で，貪欲増加部分列の長さが k であるものの個数は
* 第二種スターリング数 s(n, k) に等しい．
*
* 証明：各要素が [0..k) の元である列 a[0..n) で，貪欲増加部分列の長さが k であるものをとる．
* 各 j∈[0..k) について a[i]=j なる i の集合 a^(-1)(j)⊂[0..n) をとれば，
* [0..n) のちょうど k 個の集合への分割と 1:1 に対応する．
*
* 例えば n=4, k=3 のときの [0,1,2,0] は，集合の分割 {0,3}∪{1}∪{2} と対応する．
*/


//【単調増加列の数え上げ】
/*
* 広義単調増加列については，境界線を考えることで 格子路.h に帰着する．
* あるいは左右反転したものを考えれば ヤング図形.h に帰着する．
* 
* 狭義単調増加列については，iota(n) を引くことで広義単調増加列と 1:1 対応させることができる．
*/

