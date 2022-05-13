#pragma once
#include "header.h"
#include "約数変換.h"
#include "二項係数.h"
#include "数え上げ(区間).h"
// ■■■■■ 二重の和など ■■■■■


//【組の差の総和】O(n log n)
/*
* Σi<j |a[j] - a[i]| の値を返す．
*/
template <class T> mint difference_sum(vector<T> a) {
	// verify : https://atcoder.jp/contests/abc058/tasks/arc071_b

	int n = sz(a);

	// a の順序は無関係なので昇順ソートしておく．
	sort(all(a));

	mint res = 0;

	// 隣り合う要素の差に重みを付けて足し込んでいく．
	repi(i, 1, n - 1) {
		res += mint(a[i] - a[i - 1]) * i * (n - i);
	}

	return res;
}


//【区間の min の総和】O(n log n)
/*
* Σi<j min( a[i..j) ) の値を返す．
*
* 利用：【区間の数え上げ（最小値指定）】
*/
template <class T> T interval_min_sum(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc005/tasks/agc005_b

	unordered_map<T, ll> cnt;
	count_min_intervals(a, cnt);

	T res = T(0);
	repe(p, cnt) res += p.first * p.second;

	return res;
}


//【組の XOR の和】O((n + m) log max(a, b))
/*
* Σi=[0..n)j=[0..m) a[i] XOR b[j] の値を返す．
*/
template <class T> T xor_sum(const vl& a, const vl& b) {
	// verify : https://atcoder.jp/contests/abc147/tasks/abc147_d

	int n = sz(a), m = sz(b);
	const int d = 63;

	// 各ビット位置の 1 の個数を数えておく．
	vi ones_a(d), ones_b(d);
	rep(i, n) {
		rep(j, d) {
			if (a[i] & (1LL << j)) ones_a[j]++;
		}
	}
	rep(i, m) {
		rep(j, d) {
			if (b[i] & (1LL << j)) ones_b[j]++;
		}
	}

	// ビット毎に独立に寄与を計算し和をとればよい．
	T res = 0;
	rep(j, d) {
		// (0,1), (1,0) の組だけがビット位置に応じた寄与をもつ．
		res += ((T)ones_a[j] * (m - ones_b[j])
			+ (T)ones_b[j] * (n - ones_a[j])) * (1LL << j);
	}

	return res;
}


//【組の和の XOR】O(n log m log(max(a, b)))
/*
* XORi=[0..n)j=[0..m) (a[i] + b[j]) の値を返す．
*/
int sum_xor(const vi& a, const vi& b) {
	// verify : https://atcoder.jp/contests/abc091/tasks/arc092_b

	int n = sz(a), m = sz(b);
	const int d = 30;

	// 各ビット位置に現れる 1 の個数の偶奇
	int a_xor = accumulate(all(a), 0, [](int x, int y) { return x ^ y; });
	int b_xor = accumulate(all(b), 0, [](int x, int y) { return x ^ y; });

	int res = 0;
	rep(k, d) {
		ll cnt = 0;

		// (1,0), (0,1) の組合せがあるごとに xor 結果のビットが反転する．
		if ((a_xor & (1 << k)) && (((b_xor & (1 << k)) ^ (n % 2 == 1)))) cnt++;
		if ((b_xor & (1 << k)) && (((a_xor & (1 << k)) ^ (n % 2 == 1)))) cnt++;

		// 注目ビットより上を全て 0 にする．
		vi a_masked(n), b_masked(n);
		rep(i, n) a_masked[i] = a[i] & ((1 << k) - 1);
		rep(i, m) b_masked[i] = b[i] & ((1 << k) - 1);
		sort(all(b_masked));

		// 各 a[i] ごとに下位桁からの繰り上がりが起こる回数を数える．
		rep(i, n) {
			auto it = lower_bound(all(b_masked), (1 << k) - a_masked[i]);
			cnt += distance(it, b_masked.end());
		}

		// これらの和の偶奇に応じて xor 結果のビットが求まる．
		res += (cnt % 2) << k;
	}

	return res;
}


//【組の gcd の総和】O(n + K log(log K))（K = max(a[i], b[j])）
/*
* Σi=[0..n)j=[0..m) gcd(a[i], b[j]) の値を返す．
*
* 利用：【倍数変換，GCD 畳込み】
*/
mint gcd_sum(const vi& a, const vi& b) {
	//【方法】
	// a[i] に含まれる x の個数を a_cnt[x] などとおき，a, b の最大値を K とおくと，
	//		Σi=[0.n) Σj=[0..m) gcd(a[i], b[j])
	//		= Σx=[1..K] Σy=[1..K] a_cnt[x] b_cnt[y] gcd(x, y)
	//		= Σk=[1..K] ΣΣgcd(x,y)=k a_cnt[x] b_cnt[y] k
	// となる．この
	//		ΣΣgcd(x,y)=k a_cnt[x] b_cnt[y]
	// は gcd 畳込みであるから高速に求まる．
	//
	// これは gcd 加重和を高速に計算する方法にもなる．

	int n = sz(a), m = sz(b);

	int K = max(*max_element(all(a)), *max_element(all(b)));
	vm a_cnt(K + 1), b_cnt(K + 1);
	rep(i, n) a_cnt[a[i]]++;
	rep(j, m) b_cnt[b[j]]++;

	Multiple_transform<mint> g(K);
	auto c = g.gcd_convolution(a_cnt, b_cnt);

	mint res = 0;
	repi(k, 1, K) res += c[k] * k;

	return res;
}


//【組の lcm の総和】O(n + m + K log(log K))（K = max(a[i], b[j])）
/*
* Σi=[0..n)j=[0..m) lcm(a[i], b[j]) の値を返す．
*
* 利用：【倍数変換，GCD 畳込み】，【階乗など（法が大きな素数）】
*/
mint lcm_sum(const vi& a, const vi& b) {
	//【方法】
	// a[i] に含まれる x の個数を a_cnt[x] などとおき，a, b の最大値を K とおくと，
	//		Σi=[0.n) Σj=[0..m) lcm(a[i], b[j])
	//		= Σx=[1..K] Σy=[1..K] a_cnt[x] b_cnt[y] lcm(x, y)
	//		= Σx=[1..K] Σy=[1..K] a_cnt[x] b_cnt[y] x y / gcd(x, y)
	//		= Σk=[1..K] ΣΣgcd(x,y)=k (x a_cnt[x]) (y b_cnt[y]) / k
	// となる．この
	//		ΣΣgcd(x,y)=k (x a_cnt[x]) (y b_cnt[y])
	// は gcd 畳込みであるから高速に求まる．
	//
	// これは lcm 加重和を高速に計算する方法にもなる．

	int n = sz(a), m = sz(b);

	int K = max(*max_element(all(a)), *max_element(all(b)));
	vm a_cnt(K + 1), b_cnt(K + 1);
	rep(i, n) a_cnt[a[i]] += a[i];
	rep(j, m) b_cnt[b[j]] += b[j];

	Multiple_transform<mint> g(K);
	auto c = g.gcd_convolution(a_cnt, b_cnt);

	Factorial_mint fm(K);
	mint res = 0;
	repi(k, 1, K) res += c[k] * fm.inv(k);

	return res;
}


