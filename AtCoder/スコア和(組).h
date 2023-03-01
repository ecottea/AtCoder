#pragma once
#include "header.h"
#include "約数倍数変換.h"
#include "二項係数.h"
// ■■■■■ 組（スコア和） ■■■■■


//【組の差の総和】O(n log n)
/*
* Σi<j |a[j] - a[i]| の値を返す．
*/
template <class T>
mint difference_sum(vector<T> a) {
	// verify : https://atcoder.jp/contests/abc058/tasks/arc071_b

	int n = sz(a);

	// a の順序は無関係なので昇順ソートしておく．
	sort(all(a));

	mint res = 0;

	// 隣り合う要素の差に重みを付けて足し込んでいく．
	repi(i, 1, n - 1) res += mint(a[i] - a[i - 1]) * i * (n - i);

	return res;
}


//【組の XOR の総和】O((n + m) log max(a, b))
/*
* Σi∈[0..n) Σj∈[0..m) a[i] XOR b[j] の値を返す．
*/
template <class T>
T xor_sum(const vl& a, const vl& b) {
	// verify : https://atcoder.jp/contests/abc147/tasks/abc147_d

	int n = sz(a), m = sz(b);
	const int d = 63;

	// 各ビット位置の 1 の個数を数えておく．
	vi ones_a(d), ones_b(d);
	rep(i, n) rep(j, d) if (a[i] & (1LL << j)) ones_a[j]++;
	rep(i, m) rep(j, d) if (b[i] & (1LL << j)) ones_b[j]++;

	// ビット毎に独立に寄与を計算し和をとればよい．
	T res = 0;
	rep(j, d) {
		// (0,1), (1,0) の組だけがビット位置に応じた寄与をもつ．
		res += ((T)ones_a[j] * (m - ones_b[j])
			+ (T)ones_b[j] * (n - ones_a[j])) * (1LL << j);
	}

	return res;
}


//【組の和の総 XOR】O(n log m log(max(a, b)))
/*
* XOR_i∈[0..n) XOR_j∈[0..m) (a[i] + b[j]) の値を返す．
*/
int sum_xor(const vi& a, const vi& b) {
	// verify : https://atcoder.jp/contests/abc091/tasks/arc092_b

	//【方法】
	// 一旦繰り上がりを無視すると全ての演算が XOR になるので，総 XOR を分解して
	//		XOR_i∈[0..n) XOR_j∈[0..m) (a[i] ^ b[j])
	//		= XOR_j∈[0..m) (XOR_i∈[0..n) a[i]) ^ XOR_i∈[0..n) (XOR_j∈[0..m) b[j]))
	// となる．x ^ x = 0 に注意すると，第一項は
	//		Boole[m ≡ 1 (mod 2)] XOR_i∈[0..n) a[i]
	// と書き直せる（第二項も同様）
	//
	// 後は（下位からの累積も加味して）各桁ごとに繰り上がりが何回起こるかを求めればよい．
	// これは下位ビットでソートして二分探索を行うことで求められる．

	int n = sz(a), m = sz(b);
	const int K = 31;

	// 繰り上がりを無視した場合の結果を求める．
	int a_xor = accumulate(all(a), 0, [](int x, int y) { return x ^ y; });
	int b_xor = accumulate(all(b), 0, [](int x, int y) { return x ^ y; });
	int res = (m & 1 ? a_xor : 0) ^ (n & 1 ? b_xor : 0);

	rep(k, K) {
		// 第 k ビット以上を全て 0 にし，b を昇順ソートしておく．
		vi a_masked(n), b_masked(m); int mask = (1 << k) - 1;
		rep(i, n) a_masked[i] = a[i] & mask;
		rep(j, m) b_masked[j] = b[j] & mask;
		sort(all(b_masked));

		int bit = 0;
		rep(i, n) {
			// cnt : a[i] と足すと繰り上がりが起こる b[j] の個数
			int cnt = m - lbpos(b_masked, (1 << k) - a_masked[i]);

			// 繰り上がりが起こるたびに結果のビットが反転する．
			bit ^= cnt & 1;
		}

		// 結果の第 k ビットが確定する．
		res ^= bit << k;
	}

	return res;
}


//【組の剰余の総和】O(n + A log A)（A = max(a)）
/*
* 正整数列 a[0..n) について，Σi∈[0..n) Σj∈[0..n) a[i] mod a[j] の値を返す．
*/
ll mod_sum(const vi& a) {
	// verify : https://yukicoder.me/problems/no/1233

	int n = sz(a);
	int a_max = *max_element(all(a));
	ll a_sum = accumulate(all(a), 0LL);

	vi cnt(a_max + 1);
	rep(i, n) cnt[a[i]]++;
	dump(cnt);

	vl acc(a_max + 2);
	rep(i, a_max + 1) acc[i + 1] = acc[i] + cnt[i];
	dump(acc);

	ll res = a_sum * n;
	repi(i, 1, a_max) {
		for (int l = i; l <= a_max; l += i) {
			int r = min(l + i, a_max + 1);
			res -= cnt[i] * (acc[r] - acc[l]) * l;
		}
		dump(i, res);
	}

	return res;
}


//【組の gcd の総和】O(n + K log(log K))（K = max(a[i], b[j])）
/*
* Σi∈[0..n) Σj∈[0..m) gcd(a[i], b[j]) の値を返す．
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
* Σi∈[0..n) Σj∈[0..m) lcm(a[i], b[j]) の値を返す．
*
* 利用：【倍数変換，GCD 畳込み】,【階乗など（法が大きな素数）】
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


