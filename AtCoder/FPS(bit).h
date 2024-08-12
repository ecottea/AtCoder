#pragma once
#include "header.h"
// ■■■■■ 形式的冪級数（二元体 F_2 上） ■■■■■


//【形式的冪級数（F_2）】
/*
* BFPS<N>() : O(1)
*	零多項式 f = 0 で初期化する．
*   次数は N 未満とし，溢れた分は切り捨てられる．
*
* BFPS<N>(bool c0, int n = 1) : O(1)
*	n 次未満の項をもつ定数多項式 f = c0 で初期化する．
*
* BFPS<N>(bitset c, int n) : O(n / 64)
*	f(x) = c[0] + c[1] z + ... + c[n-1] z^(n-1) で初期化する．
*
* c + f, f + c : O(1)	f + g : O(n / 64)
* f * g : O(n^2 / 64)		f * g_sp : O(n |g|)
* f / g : O(n^2 / 64)		f / g_sp : O(n |g|)
*	形式的冪級数としての和，積，商の結果を返す．
*	g_sp はスパース多項式であり，係数が 1 である次数を昇順に並べた vector で表す．
*	制約 : 商では g(0) = 1
*
* BFPS f.inv(int d) : O(n^2 / 64)
*	1 / f mod z^d を返す．
*	制約 : f(0) = 1
*
* BFPS f.quotient(BFPS g) : O(n^2 / 64)
* BFPS f.reminder(BFPS g) : O(n^2 / 64)
* pair<BFPS, BFPS> f.quotient_remainder(BFPS g) : O(n^2 / 64)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
*	制約 : g の最項次の項の係数は 1
*
* BFPS f.quotient(SFPS g) : O((n + deg(g)) |g|)
* BFPS f.reminder(SFPS g) : O((n + deg(g)) |g|)
* pair<BFPS, BFPS> f.quotient_remainder(SFPS g) : O((n + deg(g)) |g|)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
*	制約 : g の最項次の項の係数は 1
*
* int f.deg(), int f.size() : O(1)
*	多項式 f の次数[+1]を返す．
*
* BFPS::monomial(int d) : O(d / 64)
*	単項式 z^d を返す．
*
* int popcount() : O(n / 64)
*	項数を返す．
*
* f.resize(int d) : O(1)
*	mod z^d をとる．
*
* f.resize() : O(n / 64)
*	不要な高次の項を削る．
*
* f >> d, f << d : O(n / 64)
*	係数列を d だけ右[左]シフトした多項式を返す．
*  （右シフトは z^d の乗算，左シフトは z^d で割った商と等価）
*/
template <int N>
struct BFPS {
	using SFPS = vi;

	int n; // 係数の個数（次数 + 1）
	bitset<N> c; // 係数列

	// コンストラクタ（零元，定数，係数列で初期化）
	BFPS() : n(0) {}
	BFPS(bool c0, int n = 1) : n(n) { c[0] = c0; }
	BFPS(const bitset<N>& c, int n) : n(n), c(c) {}

	// 代入
	BFPS(const BFPS& f) = default;
	BFPS& operator=(const BFPS& f) = default;
	BFPS& operator=(bool c0) { n = 1; c.reset(); c[0] = c0; return *this; }

	// 比較
	bool operator==(const BFPS& g) const { return c == g.c; }
	bool operator!=(const BFPS& g) const { return c != g.c; }

	// アクセス
	bool operator[](int i) const { return c[i]; }
	typename bitset<N>::reference operator[](int i) { return c[i]; }

	// 次数
	int deg() const { return n - 1; }
	int size() const { return n; }

	// 加算
	BFPS& operator+=(const BFPS& g) {
		chmax(n, g.n); c ^= g.c;
		return *this;
	}
	BFPS operator+(const BFPS& g) const { return BFPS(*this) += g; }

	// 定数加算
	BFPS& operator+=(bool sc) {
		chmax(n, 1); c[0] = c[0] ^ sc;
		return *this;
	}
	BFPS operator+(bool sc) const { return BFPS(*this) += sc; }
	friend BFPS operator+(bool sc, const BFPS& f) { return f + sc; }

	// 積
	BFPS& operator*=(const BFPS& g) {
		// verify : https://atcoder.jp/contests/abc009/tasks/abc009_4

		n += g.n - 1;
		bitset<N> res;
		rep(i, g.n) if (g[i]) res ^= c << i;
		c = move(res);
		return *this;
	}
	BFPS operator*(const BFPS& g) const { return BFPS(*this) *= g; }

	// 除算
	BFPS inv(int d) const {
		Assert(n >= 1 && c[0]);
		BFPS res(1, d);
		bitset<N> mul(c);
		repi(i, 1, d - 1) {
			if (mul[i]) {
				res[i] = 1;
				mul ^= c << i;
			}
		}
		bitset<N> mask; mask.set(); mask >>= N - d;
		res.c &= mask;
		return res;
	}
	BFPS& operator/=(const BFPS& g) { return *this *= g.inv(n); }
	BFPS operator/(const BFPS& g) const { return BFPS(*this) /= g; }

	// 余り付き除算
	pair<BFPS, BFPS> quotient_remainder(const BFPS& g) const {
		BFPS q, r(c, g.n - 1);
		repir(i, n - 1, g.n - 1) {
			if (r[i]) {
				q[i - g.n + 1] = 1;
				r.c ^= g.c << (i - g.n + 1);
			}
		}
		q.n = n - r.n;
		return make_pair(q, r);
	}
	BFPS quotient(const BFPS& g) const {
		// verify : https://atcoder.jp/contests/arc084/tasks/arc084_d

		return quotient_remainder(g).first;
	}
	BFPS reminder(const BFPS& g) const { return quotient_remainder(g).second; }

	// スパース積
	BFPS& operator*=(const SFPS& g) {
		n += *g.rbegin();
		bitset<N> res;
		repe(i, g) res ^= c << i;
		c = res;
		return *this;
	}
	BFPS operator*(const SFPS& g) const { return BFPS(*this) *= g; }

	// スパース商
	BFPS& operator/=(const SFPS& g) {
		// g の定数項だけ例外処理
		auto it0 = g.begin();
		Assert(*it0 == 0);
		it0++;

		// 前からインライン配る DP（後ろに累積効果あり）
		rep(i, n) {
			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				if (i + *it >= n) break;

				c[i + *it] = c[i + *it] ^ c[i];
			}
		}

		return *this;
	}
	BFPS operator/(const SFPS& g) const { return BFPS(*this) /= g; }

	// スパース余り付き除算
	pair<BFPS, BFPS> quotient_remainder(const SFPS& g) const {
		// verify : https://atcoder.jp/contests/arc147/tasks/arc147_f

		BFPS q, r(c, g.back());
		repir(i, n - 1, g.back()) {
			if (r[i]) {
				q[i - g.back()] = 1;

				repe(j, g) r[i + j - g.back()] = r[i + j - g.back()] ^ 1;
			}
		}
		q.n = n - r.n;
		return make_pair(q, r);
	}
	BFPS quotient(const SFPS& g) const { return quotient_remainder(g).first; }
	BFPS reminder(const SFPS& g) const { return quotient_remainder(g).second; }

	// 単項式 z^d を返す．
	static BFPS monomial(int d) {
		BFPS mono(0, d + 1);
		mono[d] = 1;
		return mono;
	}

	// 1 になっているビットの数を返す．
	int popcount() const {
		// verify : https://atcoder.jp/contests/arc156/tasks/arc156_d

		return (int)c.count();
	}

	// 不要な高次項の除去（最高次の係数が非 0 になるまで削る）
	BFPS& resize() {
		while (n > 0 && !c[n - 1]) n--;
		return *this;
	}

	// 高次項の除去（z^d 以上の項を除去する）
	BFPS& resize(int d) {
		n = d;
		bitset<N> mask; mask.set(); mask >>= N - d;
		c &= mask;
		return *this;
	}

	// 係数のシフト
	BFPS& operator>>=(int d) { n += d; c <<= d; return *this; }
	BFPS& operator<<=(int d) { n = max(n - d, 0); c >>= d; return *this; }
	BFPS operator>>(int d) const { return BFPS(*this) >>= d; }
	BFPS operator<<(int d) const { return BFPS(*this) <<= d; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const BFPS& f) {
		if (f.n == 0) os << 0;
		else {
			rep(i, f.n) {
				os << f[i] << "z^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
#endif
};


//【展開係数】O(n^2 log d / 64)
/*
* [z^d] f(z)/g(z) を返す．
*
* 制約 : deg f < deg g, g[0] = 1, 2 deg g < N
*/
template <int N>
bool bostan_mori(const BFPS<N>& f, const BFPS<N>& g, ll d) {
	// verify : https://atcoder.jp/contests/abc009/tasks/abc009_4

	Assert(g.n >= 1 && g[0]);

	// d = 0 のときは定数項を返す．
	if (d == 0) return f[0];

	// f2(x) = f(x) g(x), g2(x) = g(x)^2 を求める．
	BFPS<N> f2 = f * g, g2 = g * g;

	// f3(x) = E(x) or O(x), g3(x) = e(x) を求める．
	BFPS<N> f3, g3;
	if (d % 2 == 0) {
		for (int i = 0; 2 * i < f2.n; i++) {
			f3[i] = f2[2 * i]; f3.n++;
		}
	}
	else {
		for (int i = 0; 2 * i + 1 < f2.n; i++) {
			f3[i] = f2[2 * i + 1]; f3.n++;
		}
	}
	rep(i, g.n) {
		g3[i] = g2[2 * i]; g3.n++;
	}

	// d を半分にして再帰を回す．
	return bostan_mori(f3, g3, d / 2);
}


//【展開係数（累乗）】O((deg g + deg h)^2 log d / 64)
/*
* [x^d] f(z) g(z)^m / h(z) を返す．
*
* 制約 : h[0] = 1, deg f + 2 deg g + 2 deg h < N
*/
template <int N>
bool coefficient(BFPS<N> f, const BFPS<N>& g, const BFPS<N>& h, ll m, ll d) {
	// verify : https://atcoder.jp/contests/dwacon2018-final-open/tasks/dwacon2018_final_c

	//【方法】
	// m の第 i ビットを m[i] と書くことにすると，g(z)^m は
	//		g(z)^m
	//		= Πi≧0 (g(z)^(2^i))^m[i] （繰り返し二乗法）
	//		= Πi≧0 g(z^(2^i))^m[i] （フロベニウス写像の性質）
	// となる．また 1/h(z) は，H(z) = 1 - h(z) とおくと，
	//		1/h(z)
	//		= 1/(1-H(z))
	//		= Σ_i H(z)^i （等比級数の和）
	//		= Πi≧0 (1 + H(z)^(2^i))
	//		= Πi≧0 (1 + H(z^(2^i))) （フロベニウス写像の性質）
	//		= Πi≧0 h(z^(2^i)) （1 = -1 (mod 2)）
	// となる．
	// 
	// まず第 0 ビットについて考える．i=0 の部分の積を偶関数部分と奇関数部分に分けて
	//		f(z) g(z)^m[0] h(z) = F_e(z^2) + z F_o(z^2)
	// とおくと，i≧1 のときの z^(2^i) が偶関数であることから，d が偶数のときは
	//		[x^d] f(z) g(z)^m / h(z)
	//		= [x^d] F_e(z^2) Πi≧1 g(z^(2^i))^m[i] h(z^(2^i))
	//		= [x^(d/2)] F_e(z) Πi≧0 g(z^(2^i))^(m/2)[i] h(z^(2^i))
	// d が奇数のときは
	//		[x^d] f(z) g(z)^m / h(z)
	//		= [x^d] z F_o(z^2) Πi≧1 g(z^(2^i))^m[i] h(z^(2^i))
	//		= [x^(d/2)] F_o(z) Πi≧0 g(z^(2^i))^(m/2)[i] h(z^(2^i))
	// となる（/2 は切り捨て．）これで m が半分になった元と同じ形に帰着された．

	if (d < 0) return 0;

	while (d > 0) {
		if (m % 2 == 1) f *= g;
		f *= h;
		int n = sz(f);

		if (d % 2 == 0) {
			rep(i, (n + 1) / 2) f[i] = f[2 * i];
			f.resize((n + 1) / 2);
		}
		else {
			rep(i, n / 2) f[i] = f[2 * i + 1];
			f.resize(n / 2);
		}

		d /= 2;
		m /= 2;
	}

	return f[0];
}


//【線形漸化式】O(d^2 log n / 64)
/*
* 初項 a[0..d) と漸化式 a[i] = Σj=[0..d) c[j]a[i-1-j] で定義される
* 数列 a について，a[n] の値を返す．
*
* 利用：【展開係数】
*/
template <int N>
bool linearly_recurrent_sequence(const bitset<N>& a, const bitset<N>& c, int d, ll n) {
	// verify : https://atcoder.jp/contests/abc009/tasks/abc009_4

	BFPS<N> A(a, d), C(c, d);
	BFPS<N> Dnm = 1 + (C >> 1);
	BFPS<N> Num = (Dnm * A).resize(d);
	return bostan_mori(Num, Dnm, n);
}


//【累乗の剰余】O(m^2 log d / 64)　（m = deg g）
/*
* f(z)^d mod g(z) を返す．
*/
template <int N>
BFPS<N> power_mod(const BFPS<N>& f, ll d, const BFPS<N>& g) {
	// verify : https://atcoder.jp/contests/wtf19/tasks/wtf19_c1

	BFPS<N> res(1), pow2(f);
	while (d > 0) {
		if (d & 1) res = (res * pow2).reminder(g);
		pow2 = (pow2 * pow2).reminder(g);
		d /= 2;
	}
	return res;
}


//【累乗の剰余（スパース）】O((|f| + |g|) deg(g) log k)
/*
* f(z)^k mod g(z) を返す．
* 
* 制約：deg f + deg g < N
*/
template <int N>
BFPS<N> pow_mod(const typename BFPS<N>::SFPS& f_sp, ll k, const typename BFPS<N>::SFPS& g_sp) {
	// verify : https://atcoder.jp/contests/arc147/tasks/arc147_f

	//【方法】
	// k = [k_d, ..., k_1, k_0] と二進法で表示されるとき，
	//		f(x)^k = Πi∈[0..d] (k_i = 1 ? f(x)^(2^i) : 1) mod g(x)
	// として計算できる．
	//
	// フロベニウス写像の性質より |f(x)^(2^i)| = |f| であるから積は O(|f| deg(g)) で行える．
	// また mod g(x) の計算は右から配る DP で O(|g| deg(g)) で行える．

	int n = sz(f_sp); int deg_g = g_sp.back();

	BFPS<N> res(1);
	typename BFPS<N>::SFPS pow2(f_sp);
	while (k > 0) {
		if (k & 1) res = (res * pow2).reminder(g_sp);
		rep(i, n) pow2[i] = (pow2[i] * 2) % deg_g;
		sort(all(pow2)); // 本当は max を最後に回すだけでいい
		k /= 2;
	}

	return res;
}


//【累乗の指数の総 XOR】O((deg g)^2 log m / 64)
/*
* f(z) g(z)^m の各項の指数の総 XOR を返す．
*
* 制約：deg f deg g < N, 2 deg g < N
*/
template <int N>
ll pow_exponent_XOR(BFPS<N> f, const BFPS<N>& g, ll m) {
	//【方法】
	// m の第 i ビットを m[i] と書くことにすると，f(z) g(z)^m は
	//		f(z) g(z)^m = f(z) Πi≧0 (g(z)^(2^i))^m[i]
	// と表される．さらに F_2[z] においては 2 乗（フロベニウス写像）は環準同型なので
	//		f(z) g(z)^m = f(z) Πi≧0 g(z^(2^i))^m[i]
	// となる．
	// 
	// まず第 0 ビットについて考える．Πi≧1 g(z^(2^i))^m[i] の項の指数は全て偶数なので，
	//		h(z) = f(z) g(z)^m[0]
	// とおくと，
	//		(h(z) の奇数次の項数) * (Πi≧1 g(z^(2^i)^m[i]) の項数) mod 2
	// が求まれば良い．前者については h(z) を計算して素朴に数えれば良い．後者については
	//		(Πi≧1 g(z^(2^i))^m[i] の項数) mod 2
	//		= Πi≧1 (g(z^(2^i))^m[i] の項数) mod 2 （項が消えるのは同類項が偶数個のときに限る）
	//		= Πi≧1 (g(z)^m[i] の項数) mod 2
	//		= (g(z) の項数)^(popcount(m/2)) mod 2
	//		= Boole[(g(z) の項数が奇数) または m/2=0] （0 と 1 は 1 以上の累乗で不変）
	// となる．
	//
	// 以降は第 0 ビットのことは考えなくて良いので，
	//		f(z) ← h(z) の z^(2j) と z^(2j+1) をともに z^j に置き換えたもの（同類項は相殺）
	//		m ← m/2（切り捨て）
	// として，f(z) g(z)^m の各項の指数の総 XOR を求める問題に帰着する．

	//【注意】
	// 随所で mod 2 が効いているので一般化は容易ではない．
	// 例えば f(z) g(z)^m の項数を求めるだけでも難しい．
	// これをひな形にしていろいろするつもりだったけどだめそう．

	// B : 結果の msb
	int B = msb(f.deg() + g.deg() * m);

	int g_pc = g.popcount();

	ll res = 0;

	repi(b, 0, B) {
		if (m % 2 == 1) f *= g;
		m /= 2;

		int n = sz(f);

		if ((g_pc % 2 == 1) || m == 0) {
			int cnt = 0;
			rep(i, n / 2) cnt += f[2 * i + 1];
			if (cnt % 2 == 1) res |= 1LL << b;
		}

		if (n % 2 == 0) {
			rep(i, n / 2) f[i] = f[2 * i] ^ f[2 * i + 1];
			f.resize(n / 2);
		}
		else {
			rep(i, n / 2) f[i] = f[2 * i] ^ f[2 * i + 1];
			f[n / 2] = f[n - 1];
			f.resize(n / 2 + 1);
		}
	}

	return res;
}


//【累乗の項数】O(2^(deg g) (deg g)^2 log m / 64)
/*
* f(z) g(z)^m の項数を返す．
*
* 制約：deg f ≦ deg g, 2 deg g < N
*/
template <int N>
ll count_pow_terms(BFPS<N> f, const BFPS<N>& g, ll m) {
	// verify : https://projecteuler.net/problem=588

	//【方法】
	// m の第 i ビットを m[i] と書くことにすると，f(z) g(z)^m は
	//		f(z) g(z)^m = f(z) Πi≧0 (g(z)^(2^i))^m[i]
	// と表される．さらに F_2[z] においては 2 乗（フロベニウス写像）は環準同型なので
	//		f(z) g(z)^m = f(z) Πi≧0 g(z^(2^i))^m[i]
	// となる．
	// 
	// まず第 0 ビットについて考える．i=0 の部分の積を偶関数部分と奇関数部分に分けて
	//		f(z) g(z)^m[0] = F_e(z^2) + z F_o(z^2)
	// とおくと，i≧1 のときの z^(2^i) が偶関数であることから，偶数次の項は
	//		F_e(z^2) Πi≧1 g(z^(2^i))^m[i]
	// 奇数次の項は
	//		z F_o(z^2) Πi≧1 g(z^(2^i))^m[i]
	// で全てであり，それぞれの項数は
	//		F_e(z) Πi≧0 g(z^(2^i))^(m/2)[i]
	//		F_o(z) Πi≧0 g(z^(2^i))^(m/2)[i]
	// と変わらない．
	//		deg F_e(z), deg F_o(z) ≦ deg g
	// なので，次数が deg g 以下の bit 多項式全てを状態にもって bitDP すればよい．

	int n = sz(g);
	f.resize(n);

	vector<BFPS<N>> fs(1LL << n);
	repb(set, n) fs[set] = BFPS<N>(bitset<N>(set), n);

	vl dp(1LL << n);
	dp[f.c.to_ulong()] = 1;

	while (m > 0) {
		vl ndp(1LL << n);

		repb(set, n) {
			BFPS<N> f(fs[set]);
			if (m % 2 == 1) f *= g;
			f.resize(2 * n);

			int nset0 = 0;
			rep(i, n) nset0 |= (int)f[2 * i] << i;
			ndp[nset0] += dp[set];

			int nset1 = 0;
			rep(i, n) nset1 |= (int)f[2 * i + 1] << i;
			ndp[nset1] += dp[set];
		}
		dp = move(ndp);

		m /= 2;
	}

	ll res = 0;
	repb(set, n) res += dp[set] * popcount(set);

	return res;
}


//【拡張ユークリッドの互除法】O(deg(a) deg(b) / 64) (?)
/*
* g(x) = gcd(a(x), b(x)) を g に格納する．
* a(x) u(x) + b(x) v(x) = g(x) の解 (u(x), v(x)) を u, v に格納する．
*/
template <int N>
BFPS<N> extended_gcd(const BFPS<N> a, BFPS<N> b, BFPS<N>& u, BFPS<N>& v) {
	// verify : https://atcoder.jp/contests/arc084/tasks/arc084_d

	b.resize();
	if (sz(b) == 0) {
		u = BFPS<N>(1);
		v = BFPS<N>();

		return a;
	}

	BFPS<N> q, r;
	tie(q, r) = a.quotient_remainder(b);

	BFPS<N> g = extended_gcd(b, r, v, u);
	v += q * u;

	return g;
}


//【多項式逆元】O(deg(a) deg(b) / 64) (?)
/*
* a(x) u(x) = 1 (mod b(x)) を満たす u(x) を u に格納する．なければ false を返す．
*
* 利用：【拡張ユークリッドの互除法】
*/
template <int N>
bool polynomial_inverse(const BFPS<N>& a, const BFPS<N>& b, BFPS<N>& u) {
	// verify : https://atcoder.jp/contests/wtf19/tasks/wtf19_c1

	BFPS<N> v;
	auto g = extended_gcd(a, b, u, v);

	return g == BFPS<N>(1);
}


