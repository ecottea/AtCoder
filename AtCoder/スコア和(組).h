#pragma once
#include "header.h"
#include "整除.h"
#include "二項係数.h"
#include "アダマール変換.h"
#include "畳込み.h"
// ■■■■■ 組（スコア和） ■■■■■


//【組の積の総和（オンライン）】
/*
* Pair_multiple_sum<T>() : O(1)
*	空で初期化する．
*
* insert(T x) : O(1)
*	x を追加する．
*
* erase(T x) : O(1)
*	x を削除する．
*
* T get() : O(1) or O(log mod)
*	組の積の総和を返す．
*/
template <class T>
class Pair_multiple_sum {
	T sum, sqsum;

public:
	// 空で初期化する．
	Pair_multiple_sum() : sum(0), sqsum(0) {
		// verify : https://yukicoder.me/problems/no/2336
	}

	// x を追加する．
	void insert(T x) {
		// verify : https://yukicoder.me/problems/no/2336

		sum += x;
		sqsum += x * x;
	}

	// x を削除する．
	void erase(T x) {
		// verify : https://yukicoder.me/problems/no/2336

		sum -= x;
		sqsum -= x * x;
	}

	// 組の積の総和を返す．
	T get() {
		// verify : https://yukicoder.me/problems/no/2336

		return (sum * sum - sqsum) / 2;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Pair_multiple_sum& PMS) {
		os << "sum:" << PMS.sum << " ,sqsum:" << PMS.sqsum;
		return os;
	}
#endif
};


//【組の差の総和】O(n log n)
/*
* Σi<j |a[j] - a[i]| の値を返す．
*/
template <class T>
mint pair_difference_sum(vector<T> a) {
	// verify : https://atcoder.jp/contests/abc058/tasks/arc071_b

	int n = sz(a);

	// a の順序は無関係なので昇順ソートしておく．
	sort(all(a));

	mint res = 0;

	// 隣り合う要素の差に重みを付けて足し込んでいく．
	repi(i, 1, n - 1) res += mint(a[i] - a[i - 1]) * i * (n - i);

	return res;
}


//【組の XOR の総和】O((n + m) log A)（A = max(a[i], b[j])）
/*
* Σi∈[0..n) Σj∈[0..m) (a[i] XOR b[j]) の値を返す．
*/
template <class T>
T pair_xor_sum(const vl& a, const vl& b) {
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


//【組の XOR の総和】O(n + m + A log A)（A = max(a[i], b[j])）
/*
* Σi∈[0..n) Σj∈[0..m) (a[i] XOR b[j]) の値を返す．
*
* 利用：【XOR 畳込み】
*/
template <class T>
T pair_xor_sum(const vi& a, const vi& b) {
	// verify : https://www.codechef.com/problems/PALIXOR

	//【方法】
	// a, b の度数分布をそれぞれ u, v とすると，
	//		Σi∈[0..n) Σj∈[0..m) (a[i] XOR b[j])
	//		= Σx Σy u[x] v[y] (x XOR y)  （XOR 加重和の形）
	//		= Σz Σ(x XOR y = z) u[x] v[y] z
	//		= Σz z Σ(x XOR y = z) u[x] v[y]
	// と書き直せるので，XOR 畳込みを用いて高速化できる．

	int n = sz(a), m = sz(b);

	int A = 1;
	rep(i, n) chmax(A, 1 << (msb(a[i]) + 1));
	rep(j, m) chmax(A, 1 << (msb(b[j]) + 1));

	vector<T> cnt_a(A), cnt_b(A);
	rep(i, n) cnt_a[a[i]]++;
	rep(j, m) cnt_b[b[j]]++;

	auto cnt = xor_convolution(cnt_a, cnt_b);

	T res = 0;
	rep(set, A) res += cnt[set] * set;

	return res;
}


//【組の XOR の総和の一般化】
/*
*【組の XOR の総和】で用いた手法と同様にして，
*	Σi∈[0..n) Σj∈[0..m) F(a[i]) G(b[j]) H(a[i] XOR b[j])
* の形や，XOR 加重和
*	Σx Σy u[x] v[y] (x XOR y)
* の形を XOR 畳込みを用いて高速に計算できる．
*/


//【組の和の総 XOR】O(n log m log A)（A = max(a[i], b[j])）
/*
* XOR_i∈[0..n) XOR_j∈[0..m) (a[i] + b[j]) の値を返す．
*/
int pair_sum_xor(const vi& a, const vi& b) {
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


//【組の GCD の総和】O(n + A log(log A))（A = max(a[i], b[j])）
/*
* Σi∈[0..n) Σj∈[0..m) GCD(a[i], b[j]) の値を返す．
*
* 利用：【約数倍数変換】
*/
template <class T>
T pair_gcd_sum(const vi& a, const vi& b) {
	//【方法】
	// a, b の度数分布をそれぞれ u, v とすると，
	//		Σi∈[0..n) Σj∈[0..m) f(GCD(a[i], b[j]))
	//		= Σx Σy u[x] v[y] f(GCD(x, y))  （GCD 加重和の形）
	//		= Σg Σ(GCD(x, y) = g) u[x] v[y] f(g)
	//		= Σg f(g) Σ(GCD(x, y) = g) u[x] v[y]
	// と書き直せるので，GCD 畳込みを用いて高速化できる．

	int n = sz(a), m = sz(b);

	int A = max(*max_element(all(a)), *max_element(all(b)));
	vector<T> cnt_a(A + 1), cnt_b(A + 1);
	rep(i, n) cnt_a[a[i]]++;
	rep(j, m) cnt_b[b[j]]++;

	Div_mul_transform<T> g(A);
	auto cnt = g.gcd_convolution(cnt_a, cnt_b);

	T res = 0;
	repi(g, 1, A) res += cnt[g] * g;

	return res;
}


//【組の GCD の総和の一般化】
/*
*【組の GCD の総和】で用いた手法と同様にして，
*	Σi∈[0..n) Σj∈[0..m) F(a[i]) G(b[j]) H(GCD(a[i], b[j]))
* の形や，GCD 加重和
*	Σx Σy u[x] v[y] GCD(x, y)
* の形を GCD 畳込みを用いて高速に計算できる．
*/


//【組の LCM の総和】O(n + m + A log(log A))（A = max(a[i], b[j])）
/*
* Σi∈[0..n) Σj∈[0..m) LCM(a[i], b[j]) の値を返す．
*
* 利用：【約数倍数変換】,【階乗など（法が大きな素数）】
*/
mint pair_lcm_sum(const vi& a, const vi& b) {
	//【方法】
	// a, b の度数分布をそれぞれ u, v とすると，
	//		Σi∈[0..n) Σj∈[0..m) LCM(a[i], b[j])
	//		= Σx Σy u[x] v[y] LCM(x, y)  （LCM 加重和の形）
	//		= Σx Σy u[x] v[y] x y / GCD(x, y)
	//		= Σg Σ(GCD(x, y) = g) u[x] v[y] x y / g
	//		= Σg (1/g) Σ(GCD(x, y) = g) (x u[x]) (y v[y])
	// と書き直せるので，GCD 畳込みを用いて高速化できる．

	//【注意】
	// LCM 畳込みは値域が広くなりすぎるので適切ではない．

	int n = sz(a), m = sz(b);

	int A = max(*max_element(all(a)), *max_element(all(b)));
	vm a_cnt(A + 1), b_cnt(A + 1);
	rep(i, n) a_cnt[a[i]] += a[i];
	rep(j, m) b_cnt[b[j]] += b[j];

	Div_mul_transform<mint> g(A);
	auto c = g.gcd_convolution(a_cnt, b_cnt);

	Factorial_mint fm(A);
	mint res = 0;
	repi(k, 1, A) res += c[k] * fm.inv(k);

	return res;
}


//【組の整数商の総和】O(n + m + A log A)（A = max(a[i])）
/*
* Σi∈[0..n) Σj∈[0..m) floor(a[i] / b[j]) の値を返す．
*
* 制約：b[j] ≧ 1
*
* 利用：【整数商畳込み】
*/
template <class T>
T pair_floordiv_sum(const vi& a, const vi& b) {
	//【方法】
	// a, b の度数分布をそれぞれ u, v とすると，
	//		Σi∈[0..n) Σj∈[0..m) floor(a[i] / b[j])
	//		= Σx Σy u[x] v[y] floor(x / y)  （floor_div 加重和の形）
	//		= Σq Σ(floor(x / y) = q) u[x] v[y] q
	//		= Σq q Σ(floor(x / y) = q) u[x] v[y]
	// と書き直せるので，整数商畳込みを用いて高速化できる．

	int n = sz(a), m = sz(b);

	int A = *max_element(all(a));
	vector<T> cnt_a(A + 1), cnt_b(A + 1);
	rep(i, n) cnt_a[a[i]]++;
	rep(j, m) if (b[j] <= A) cnt_b[b[j]]++; // q=0 確定な要素は無視

	auto cnt = floordiv_convolution(cnt_a, cnt_b);

	T res = 0;
	repi(q, 1, A) res += cnt[q] * q; // q=0 は無視

	return res;
}


//【組の整数商の総和の一般化】
/*
*【組の整数商の総和】で用いた手法と同様にして，
*	Σi∈[0..n) Σj∈[0..m) F(a[i]) G(b[j]) H(floor(a[i] / b[j]))
* の形や，floor_div 加重和
*	Σx Σy u[x] v[y] floor(x / y)
* の形を整数商畳込みを用いて高速に計算できる．
*/


//【組の剰余の総和】O(n + m + A log A)（A = max(a[i])）
/*
* Σi∈[0..n) Σj∈[0..m) (a[i] mod b[j]) の値を返す．
*
* 制約：b[j] ≧ 1
*
* 利用：【整数商畳込み】
*/
template <class T>
T pair_mod_sum(const vi& a, const vi& b) {
	// verify : https://yukicoder.me/problems/no/1233

	//【方法】
	// a, b の度数分布をそれぞれ u, v とすると，
	//		Σi∈[0..n) Σj∈[0..m) (a[i] mod b[j])
	//		= Σx Σy u[x] v[y] (x mod y)  （mod 加重和の形）
	//		= Σx Σy u[x] v[y] (x - floor(x / y) y)
	//		= Σx Σy u[x] v[y] x - Σx Σy u[x] v[y] floor(x / y) y
	//		= (Σx x u[x])(Σy v[y]) - Σq Σ(floor(x / y) = q) u[x] v[y] q y
	//		= Σa[0..n) m - Σq q Σ(floor(x / y) = q) u[x] (y v[y])
	// と書き直せるので，整数商畳込みを用いて高速化できる．

	int n = sz(a), m = sz(b);

	int A = *max_element(all(a));
	vector<T> u(A + 1), v(A + 1);
	rep(i, n) u[a[i]]++;
	rep(j, m) if (b[j] <= A) v[b[j]]++; // q=0 確定な要素は無視

	vector<T> yv(A + 1);
	repi(y, 0, A) yv[y] = y * v[y];
	auto w = floordiv_convolution(u, yv);

	T res = accumulate(all(a), T(0)) * m;
	repi(q, 1, A) res -= w[q] * q; // q=0 は無視

	return res;
}


//【組の差の総積】O(n (log n)^2)
/*
* FPS(mint).h の【差積】を用いれば良い．
*/


//【組の和の総積】O(n (log n)^2)
/*
* 与えられた a[0..n) について Πi<j (a[i]+a[j]) を返す．
*
* 利用：【形式的冪級数】
*/
template <class T>
mint pair_sum_product(const vector<T>& a) {
	int n = sz(a);
	int N = 1 << (msb(n - 1) + 1);

	// muls : (z - a[i]) の連続する 2 冪個の積からなる完全二分木
	vector<MFPS> muls(N * 2);
	repi(i, N, N + n - 1) muls[i] = MFPS(vm({ -a[i - N], 1 }));
	repi(i, N + n, 2 * N - 1) muls[i] = MFPS(1);
	repir(i, N - 1, 1) muls[i] = muls[2 * i] * muls[2 * i + 1];

	// muls2 : (z + a[i]) の連続する 2 冪個の積からなる完全二分木
	vector<MFPS> muls2(muls);
	repi(i, 1, N + n - 1) {
		int K = sz(muls2[i]);
		for (int k = K - 2; k >= 0; k -= 2) muls2[i][k] *= -1;
	}

	// mods : Π(z + a[m..r)) を Π(z - a[l..m)) で割った余りからなる完全二分木
	vector<MFPS> mods(N * 2);
	mods[1] = MFPS(1);
	repi(i, 2, N + n - 1) {
		if (i & 1) mods[i] = (mods[i / 2] * muls2[i - 1]).reminder(muls[i]);
		else mods[i] = mods[i / 2].reminder(muls[i]);
	}

	// mods の葉は (z - x[i]) で割った余りなので，因数定理よりこれが Π(a[i] + a[0..i)) に等しい．
	mint res = 1;
	rep(i, n) {
		if (sz(mods[N + i]) == 0) return 0;
		res *= mods[N + i][0];
	}

	return res;
}


