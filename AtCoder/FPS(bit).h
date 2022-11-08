#pragma once
#include "header.h"
// ■■■■■ 形式的冪級数（二元体 F2 上） ■■■■■


//【形式的冪級数（二元体 F2）】
/*
* BFPS<N>() : O(1)
*	零多項式 f = 0 で初期化する．
*   次数は N 未満とし，溢れた分は切り捨てられる．
*
* BFPS<N>(bool c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* BFPS<N>(bitset c, int n) : O(n / 64)
*	f(x) = c[0] + c[1] x + ... + c[n-1] x^(n-1) で初期化する．
*
* c + f, f + c : O(1)	f + g : O(n / 64)
* f * g : O(n^2 / 64)		f * g_sp : O(n |g|)
* f / g : O(n^2 / 64)		f / g_sp : O(n |g|)
*	形式的冪級数としての和，積，商の結果を返す．
*	g_sp はスパース多項式であり，係数が 1 である次数を昇順に並べた vector で表す．
*	制約 : 商では g(0) = 1
*
* BFPS f.inv(int d) : O(n^2 / 64)
*	1 / f mod x^d を返す．
*	制約 : f(0) = 1
*
* BFPS f.quotient(BFPS g) : O(n^2 / 64)
* BFPS f.reminder(BFPS g) : O(n^2 / 64)
* pair<BFPS, BFPS> f.quotient_remainder(BFPS g) : O(n^2 / 64)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
* 　制約 : g の最項次の項の係数は 1
*
* BFPS f.quotient(SFPS g) : O((n + deg(g)) |g|)
* BFPS f.reminder(SFPS g) : O((n + deg(g)) |g|)
* pair<BFPS, BFPS> f.quotient_remainder(SFPS g) : O((n + deg(g)) |g|)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
* 　制約 : g の最項次の項の係数は 1
*
* int f.deg(), int f.size() : O(1)
*	多項式 f の次数[+1]を返す．
*
* BFPS::monomial(int d) : O(d / 64)
*	単項式 x^d を返す．
*
* f.resize(int d) : O(1)
*	mod x^d をとる．
*
* f.resize() : O(n / 64)
*	不要な高次の項を削る．
*
* f >> d, f << d : O(n / 64)
*	係数列を d だけ右[左]シフトした多項式を返す．
*  （右シフトは x^d の乗算，左シフトは x^d で割った商と等価）
*
* BFPS power_mod(BFPS f, ll d, BFPS g) : O(m^2 log d / 64)　（m = deg g）
*	f(x)^d % g(x) を返す．
*/
template <int N> struct BFPS {
	using SFPS = vi;

	int n; // 係数の個数（次数 + 1）
	bitset<N> c; // 係数列

	// コンストラクタ（零元，定数，係数列で初期化）
	BFPS() : n(0) {}
	BFPS(bool c0) : n(1) { c[0] = c0; }
	BFPS(const bitset<N>& c_, int n_) : n(n_), c(c_) {}

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
		rep(i, g.n) {
			if (g[i]) res ^= c << i;
		}
		c = res;
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
	BFPS quotient(const BFPS& g) const { return quotient_remainder(g).first; }
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

	// 単項式
	static BFPS monomial(int d) {
		BFPS mono(0, d + 1);
		mono[d] = 1;
		return mono;
	}

	// 不要な高次項の除去（最高次の係数が非 0 になるまで削る）
	BFPS& resize() {
		while (n > 0 && !c[n - 1]) n--;
		return *this;
	}

	// 高次項の除去（x^d 以上の項を除去する）
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

	// 累乗の剰余
	friend BFPS power_mod(const BFPS& f, ll d, const BFPS& g) {
		BFPS res(1), pow2(f);
		while (d > 0) {
			if (d & 1) res = (res * pow2).reminder(g);
			pow2 = (pow2 * pow2).reminder(g);
			d /= 2;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const BFPS& f) {
		if (f.n == 0) os << 0;
		else {
			rep(i, f.n) {
				os << f[i] << "x^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
#endif
};


//【累乗（スパース）】O((|f| + |g|) deg(g) log k)
/*
* f(x)^k mod g(x) を返す．
*/
template <int N> BFPS<N> pow(const typename BFPS<N>::SFPS& f_sp, ll k, const typename BFPS<N>::SFPS& g_sp) {
	// verify : https://atcoder.jp/contests/arc147/tasks/arc147_f
	
	//【方法】
	// k = [k_d, ..., k_1, k_0] と二進法で表示されるとき，
	//		f(x)^k = Πi∈[0..d] (k_i = 1 ? f(x)^(2^i) : 1) mod g(x)
	// として計算できる．
	//
	// フロベニウス写像の性質より |f(x)^(2^i)| = |f| であるから積は O(|f| deg(g)) で行える．
	// また mod g(x) の計算は O(|g| deg(g)) で行える．

	int n = sz(f_sp); int deg_g = g_sp.back();

	BFPS<N> res(1);
	typename BFPS<N>::SFPS pow2(f_sp);
	while (k > 0) {
		if (k & 1) res = (res * pow2).reminder(g_sp);
		rep(i, n) pow2[i] = (pow2[i] * 2) % deg_g;
		sort(all(pow2));
		k /= 2;
	}

	return res;
}


//【展開係数】O(n^2 log d)
/*
* 有理式 f(x) / g(x) を形式的冪級数に展開したときの x^d の係数を返す．
*
* 制約 : deg f < deg g, g[0] = 1, 2 deg g < N
*/
template <int N> bool bostan_mori(const BFPS<N>& f, const BFPS<N>& g, ll d) {
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


//【線形漸化式】O(d log d log n)
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


//【拡張ユークリッドの互除法】O(deg(a) deg(b)) (?)
/*
* g(x) = gcd(a(x), b(x)) を g に格納する．
* a(x) u(x) + b(x) v(x) = g(x) の解 (u(x), v(x)) を u, v に格納する．
*/
template <int N>
void extended_gcd(const BFPS<N> a, BFPS<N> b, BFPS<N>& g, BFPS<N>& u, BFPS<N>& v) {
	b.resize();
	if (sz(b) == 0) {
		u = BFPS<N>(1);
		v = BFPS<N>();
		g = a;
		return;
	}

	BFPS<N> q, r;
	tie(q, r) = a.quotient_remainder(b);

	extended_gcd(b, r, g, v, u);
	v += q * u;
}


//【多項式逆元】O(deg(a) deg(b)) (?)
/*
* a(x) u(x) = 1 (mod b(x)) を満たす u(x) を u に格納する．なければ false を返す．
*
* 利用：【拡張ユークリッドの互除法】
*/
template <int N>
bool polynomial_inverse(const BFPS<N>& a, const BFPS<N>& b, BFPS<N>& u) {
	BFPS<N> g, v;
	extended_gcd(a, b, g, u, v);

	return g == BFPS<N>(1);
}


//【多項式の離散対数問題】O(2^(deg(g)/2) deg(g)^2)
/*
* a(x) f(x)^d = b(x) mod g(x) の最小解 d >= 0 を返す．（なければ INFL）
*
*（baby-step giant-step）
*/
template <int N>
ll log(const BFPS<N>& f, const BFPS<N>& a, const BFPS<N>& b, const BFPS<N>& g) {
	// verfy : https://atcoder.jp/contests/utpc2014/tasks/utpc2014_k

	ll m = 1LL << ((g.deg() + 1) / 2);

	// logf[v] : v = b f^j となる m 未満で最大の j
	unordered_map<unsigned long long, int> logf;
	BFPS<N> f_pow(1);
	rep(j, m) {
		if (sz((a * f_pow + b).reminder(g).resize()) == 0) return j;

		BFPS<N> v = (f_pow * b).reminder(g);
		logf[v.c.to_ullong()] = j;

		f_pow = (f_pow * f).reminder(g);
	}

	// a に f_pow = f^m を掛けながら解の候補を探していく．
	BFPS<N> af = a;
	repi(i, 1, m) {
		af = (af * f_pow).reminder(g);
		if (logf.count(af.c.to_ullong())) {
			// a f^(i m) = b f^j なる (i, j) が見つかった．
			ll d = i * m - logf[af.c.to_ullong()];

			// 実際に計算してみて一致するかを見る．
			if (sz((a * power_mod(f, d, g) + b).reminder(g).resize()) == 0) {
				return d;
			}
			return INFL; // ここで打ち切っていいとは限らないのでは？？
		}
	}

	return INFL;
}


