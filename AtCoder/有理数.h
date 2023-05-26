#pragma once
#include "header.h"
// ■■■■■ 有理数 ■■■■■


//【有理数】
/*
* Frac<T>() : O(1)
*	0 で初期化する．
*
* Frac<T>(T num) : O(1)
*	num で初期化する．
*
* Frac<T>(T num, T dnm) : O(1)
*	num / dnm で初期化する（分母は自動的に正にする）
*
* a == b, a != b, a < b, a > b, a <= b, a >= b : O(1)
*	大小比較を行う（分母が共通の場合は積はとらない）
*
* a + b, a - b, a * b, a / b : O(1)
*	加減乗除を行う（和と差については，分母が共通の場合は積はとらない）
*	一方が整数でも構わない．複合代入演算子も使用可．
*
* reduction() : O(log min(num, dnm))
*	自身の約分を行う．
*/
template <class T = ll>
struct Frac {
	// verify : https://atcoder.jp/contests/abc057/tasks/abc057_d

	// 分子，分母
	T num, dnm;

	// コンストラクタ
	Frac() : num(0), dnm(1) {}
	Frac(T num) : num(num), dnm(1) {}
	Frac(T num_, T dnm_) : num(num_), dnm(dnm_) {
		// verify : https://atcoder.jp/contests/abc244/tasks/abc244_h

		Assert(dnm != 0);
		if (dnm < 0) { num *= -1; dnm *= -1; }
	}

	// 代入
	Frac(const Frac& b) = default;
	Frac& operator=(const Frac& b) = default;

	// キャスト
	operator double() const { return (double)num / dnm; }

	// 比較
	bool operator==(const Frac& b) const {
		// 分母が等しいときはオーバーフロー防止のために掛け算はせず比較する．
		if (dnm == b.dnm) return num == b.num;
		return num * b.dnm == b.num * dnm;
	}
	bool operator!=(const Frac& b) const { return !(*this == b); }
	bool operator<(const Frac& b) const {
		// verify : https://www.codechef.com/problems/ARCTR

		// 分母が等しいときはオーバーフロー防止のために掛け算はせず比較する．
		if (dnm == b.dnm) return num < b.num;
		return (num * b.dnm < b.num * dnm);
	}
	bool operator>=(const Frac& b) const { return !(*this < b); }
	bool operator>(const Frac& b) const { return b < *this; }
	bool operator<=(const Frac& b) const { return !(*this > b); }

	// 整数との比較
	bool operator==(T b) const { return num == b * dnm; }
	bool operator!=(T b) const { return num != b * dnm; }
	bool operator<(T b) const { return num < b * dnm; }
	bool operator>=(T b) const { return num >= b * dnm; }
	bool operator>(T b) const { return num > b * dnm; }
	bool operator<=(T b) const { return num <= b * dnm; }
	friend bool operator==(T a, const Frac& b) { return a * b.dnm == b.num; }
	friend bool operator!=(T a, const Frac& b) { return a * b.dnm != b.num; }
	friend bool operator<(T a, const Frac& b) { return a * b.dnm < b.num; }
	friend bool operator>=(T a, const Frac& b) { return a * b.dnm >= b.num; }
	friend bool operator>(T a, const Frac& b) { return a * b.dnm > b.num; }
	friend bool operator<=(T a, const Frac& b) { return a * b.dnm <= b.num; }

	// 四則演算
	Frac& operator+=(const Frac& b) {
		// verify : https://www.codechef.com/problems/ARCTR

		// 分母が等しいときはオーバーフロー防止のために掛け算はせず加算する．
		if (dnm == b.dnm) num += b.num;
		else { num = num * b.dnm + b.num * dnm; dnm *= b.dnm; }
		return *this;
	}
	Frac& operator-=(const Frac& b) {
		// verify : https://www.codechef.com/problems/ARCTR

		// 分母が等しいときはオーバーフロー防止のために掛け算はせず加算する．
		if (dnm == b.dnm) num -= b.num;
		else { num = num * b.dnm - b.num * dnm; dnm *= b.dnm; }
		return *this;
	}
	Frac& operator*=(const Frac& b) { num *= b.num; dnm *= b.dnm; return *this; }
	Frac& operator/=(const Frac& b) {
		// verify : https://atcoder.jp/contests/abc301/tasks/abc301_g

		Assert(b.num != 0);
		num *= b.dnm; dnm *= b.num;
		if (dnm < 0) { num *= -1; dnm *= -1; }
		return *this;
	}
	Frac operator+(const Frac& b) const { Frac a = *this; return a += b; }
	Frac operator-(const Frac& b) const { Frac a = *this; return a -= b; }
	Frac operator*(const Frac& b) const { Frac a = *this; return a *= b; }
	Frac operator/(const Frac& b) const { Frac a = *this; return a /= b; }
	Frac operator-() const { return Frac(*this) *= Frac(-1); }

	// 整数との四則演算
	Frac& operator+=(T c) { num += dnm * c; return *this; }
	Frac& operator-=(T c) { num -= dnm * c; return *this; }
	Frac& operator*=(T c) { num *= c; return *this; }
	Frac& operator/=(T c) {
		Assert(c != T(0));
		dnm *= c;
		if (dnm < 0) { num *= -1; dnm *= -1; }
		return *this;
	}
	Frac operator+(T c) const { Frac a = *this; return a += c; }
	Frac operator-(T c) const { Frac a = *this; return a -= c; }
	Frac operator*(T c) const { Frac a = *this; return a *= c; }
	Frac operator/(T c) const { Frac a = *this; return a /= c; }
	friend Frac operator+(T c, const Frac& a) { return a + c; }
	friend Frac operator-(T c, const Frac& a) { return Frac(c) - a; }
	friend Frac operator*(T c, const Frac& a) { return a * c; }
	friend Frac operator/(T c, const Frac& a) { return Frac(c) / a; }

	// 約分を行う．
	void reduction() {
		auto g = gcd(abs(num), abs(dnm));
		num /= g; dnm /= g;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Frac& a) { os << a.num << '/' << a.dnm; return os; }
#endif
};


//【有理数（mint）】
/*
* Frac_mint() : O(1)
*	0 で初期化する．
*
* Frac_mint(T num) : O(1)
*	num で初期化する．
*
* Frac_mint(T num, T dnm) : O(1)
*	num / dnm で初期化する．
*
* a + b, a - b, a * b, a / b : O(1)
*	加減乗除を行う．一方が mint でも構わない．複合代入演算子も使用可．
*
* Frac_mint inv() : O(1)
*	逆数を返す．
*
* Frac_mint pow(ll n) : O(log n)
*	自身の n 乗を返す．
*
* mint val() : O(log mod)
*	mint としての値を返す．
*/
struct Frac_mint {
	// verify : https://atcoder.jp/contests/agc060/tasks/agc060_c

	// 分子，分母
	mint num, dnm;

	// コンストラクタ
	Frac_mint() : num(0), dnm(1) {}
	Frac_mint(mint num) : num(num), dnm(1) {}
	Frac_mint(mint num, mint dnm) : num(num), dnm(dnm) {}
	Frac_mint(int num) : num(num), dnm(1) {}
	Frac_mint(int num, int dnm) : num(num), dnm(dnm) {}
	Frac_mint(ll num) : num(num), dnm(1) {}
	Frac_mint(ll num, ll dnm) : num(num), dnm(dnm) {}

	// 代入
	Frac_mint(const Frac_mint& b) = default;
	Frac_mint& operator=(const Frac_mint& b) = default;

	// キャスト
	operator mint() const { return num * dnm.inv(); }

	// 比較
	bool operator==(const Frac_mint& b) const { return num * b.dnm == b.num * dnm; }
	bool operator!=(const Frac_mint& b) const { return !(*this == b); }

	// 整数との比較
	bool operator==(mint b) const { return num == b * dnm; }
	bool operator!=(mint b) const { return !(*this == b); }
	bool operator==(int b) const { return num == b * dnm; }
	bool operator!=(int b) const { return !(*this == b); }
	bool operator==(ll b) const { return num == b * dnm; }
	bool operator!=(ll b) const { return !(*this == b); }
	friend bool operator==(mint b, const Frac_mint& a) { return a.num == b * a.dnm; }
	friend bool operator!=(mint b, const Frac_mint& a) { return !(a == b); }
	friend bool operator==(int b, const Frac_mint& a) { return a.num == b * a.dnm; }
	friend bool operator!=(int b, const Frac_mint& a) { return !(a == b); }
	friend bool operator==(ll b, const Frac_mint& a) { return a.num == b * a.dnm; }
	friend bool operator!=(ll b, const Frac_mint& a) { return !(a == b); }

	// 四則演算
	Frac_mint& operator+=(const Frac_mint& b) {
		num = num * b.dnm + b.num * dnm; dnm *= b.dnm;
		return *this;
	}
	Frac_mint& operator-=(const Frac_mint& b) {
		num = num * b.dnm - b.num * dnm; dnm *= b.dnm;
		return *this;
	}
	Frac_mint& operator*=(const Frac_mint& b) { num *= b.num; dnm *= b.dnm; return *this; }
	Frac_mint& operator/=(const Frac_mint& b) { num *= b.dnm; dnm *= b.num; return *this; }
	Frac_mint operator+(const Frac_mint& b) const { Frac_mint a = *this; return a += b; }
	Frac_mint operator-(const Frac_mint& b) const { Frac_mint a = *this; return a -= b; }
	Frac_mint operator*(const Frac_mint& b) const { Frac_mint a = *this; return a *= b; }
	Frac_mint operator/(const Frac_mint& b) const { Frac_mint a = *this; return a /= b; }
	Frac_mint operator-() const { return Frac_mint(*this) *= Frac_mint(-1); }
	Frac_mint inv() const { return Frac_mint(dnm, num); }

	// 整数との四則演算
	Frac_mint& operator+=(mint c) { num += dnm * c; return *this; }
	Frac_mint& operator-=(mint c) { num -= dnm * c; return *this; }
	Frac_mint& operator*=(mint c) { num *= c; return *this; }
	Frac_mint& operator/=(mint c) { dnm *= c; return *this; }
	Frac_mint operator+(mint c) const { Frac_mint a = *this; return a += c; }
	Frac_mint operator-(mint c) const { Frac_mint a = *this; return a -= c; }
	Frac_mint operator*(mint c) const { Frac_mint a = *this; return a *= c; }
	Frac_mint operator/(mint c) const { Frac_mint a = *this; return a /= c; }
	friend Frac_mint operator+(mint c, const Frac_mint& a) { return a + c; }
	friend Frac_mint operator-(mint c, const Frac_mint& a) { return Frac_mint(c) - a; }
	friend Frac_mint operator*(mint c, const Frac_mint& a) { return a * c; }
	friend Frac_mint operator/(mint c, const Frac_mint& a) { return Frac_mint(c) / a; }
	Frac_mint& operator+=(int c) { num += dnm * c; return *this; }
	Frac_mint& operator-=(int c) { num -= dnm * c; return *this; }
	Frac_mint& operator*=(int c) { num *= c; return *this; }
	Frac_mint& operator/=(int c) { dnm *= c; return *this; }
	Frac_mint operator+(int c) const { Frac_mint a = *this; return a += c; }
	Frac_mint operator-(int c) const { Frac_mint a = *this; return a -= c; }
	Frac_mint operator*(int c) const { Frac_mint a = *this; return a *= c; }
	Frac_mint operator/(int c) const { Frac_mint a = *this; return a /= c; }
	friend Frac_mint operator+(int c, const Frac_mint& a) { return a + c; }
	friend Frac_mint operator-(int c, const Frac_mint& a) { return Frac_mint(c) - a; }
	friend Frac_mint operator*(int c, const Frac_mint& a) { return a * c; }
	friend Frac_mint operator/(int c, const Frac_mint& a) { return Frac_mint(c) / a; }
	Frac_mint& operator+=(ll c) { num += dnm * c; return *this; }
	Frac_mint& operator-=(ll c) { num -= dnm * c; return *this; }
	Frac_mint& operator*=(ll c) { num *= c; return *this; }
	Frac_mint& operator/=(ll c) { dnm *= c; return *this; }
	Frac_mint operator+(ll c) const { Frac_mint a = *this; return a += c; }
	Frac_mint operator-(ll c) const { Frac_mint a = *this; return a -= c; }
	Frac_mint operator*(ll c) const { Frac_mint a = *this; return a *= c; }
	Frac_mint operator/(ll c) const { Frac_mint a = *this; return a /= c; }
	friend Frac_mint operator+(ll c, const Frac_mint& a) { return a + c; }
	friend Frac_mint operator-(ll c, const Frac_mint& a) { return Frac_mint(c) - a; }
	friend Frac_mint operator*(ll c, const Frac_mint& a) { return a * c; }
	friend Frac_mint operator/(ll c, const Frac_mint& a) { return Frac_mint(c) / a; }

	// 累乗
	Frac_mint pow(ll d) const {
		// verify : https://atcoder.jp/contests/abc295/tasks/abc295_e

		Frac_mint res(1), pow2 = *this;
		while (d > 0) {
			if (d & 1) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// 約分を行う．
	void reduction() {
		int x = num.val(), y = dnm.val();
		auto g = gcd(x, y);
		num = x / g; dnm = y / g;
	}

	// mint としての値を返す．
	mint val() {
		return num / dnm;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Frac_mint a) {
		if ((int)(a.dnm.val()) >= (int)(mint::mod()) / 2) a *= Frac_mint(-1, -1);
		if ((int)(a.num.val()) < (int)(mint::mod()) / 2) {
			a.reduction();
			os << a.num << '/' << a.dnm;
		}
		else {
			a *= -1;
			a.reduction();
			os << '-' << a.num << '/' << a.dnm;
		}
		return os;
	}
#endif
};


//【mint → 有理数】（実験用）
/*
* x を分母と分子の絶対値が v_max 以下の有理数表示に変換する（不可能ならそのまま）
*/
string mint_to_frac(mint x, int v_max = 31595) {
	// verify : https://www.codechef.com/problems/SUMOVERALL

	repi(dnm, 1, v_max) {
		int num = (x * dnm).val();
		if (num == 0) {
			return "0";
		}
		if (num <= v_max) {
			if (dnm == 1) return to_string(num);
			return to_string(num) + "/" + to_string(dnm);
		}
		if (mint::mod() - num <= v_max) {
			if (dnm == 1) return "-" + to_string(mint::mod() - num);
			return "-" + to_string(mint::mod() - num) + "/" + to_string(dnm);
		}
	}

	return to_string(x.val());
}


//【正則連分数展開】O(log min(num, dnm))
/*
* 正の有理数 num/dnm の正則連分数展開を seq に格納し seq を返す．
* すなわち，num/dnm = seq[0] + 1/(seq[1] + 1/(seq[2] + 1/(...))) である．
*/
vl continued_fraction(ll num, ll dnm) {
	// verify : https://atcoder.jp/contests/abc273/tasks/abc273_h

	vl seq;

	while (dnm > 0) {
		seq.emplace_back(num / dnm);
		num %= dnm;
		swap(num, dnm);
	}

	return seq;
}


//【正則連分数展開からの復元】O(n)
/*
* 正の有理数の正則連分数展開 seq[0..n) が表す有理数 r を既約表示した {分子, 分母} を返す．
* すなわち，r = seq[0] + 1/(seq[1] + 1/(seq[2] + 1/(...))) である．
*/
pll from_continued_fraction(const vl& seq) {
	int n = sz(seq);
	ll num = 1, dnm = 0;

	repir(i, n - 1, 0) {
		swap(num, dnm);
		num += seq[i] * dnm;
	}

	return make_pair(num, dnm);
}


//【有理数近似】O(log dnm)
/*
* 実数 x を分母が dnm_max 以下の既約分数 num / dnm で近似し，組 {num, dnm} を返す．
* 最良の近似であるとは限らないので注意．
*/
template <class T>
pair<T, T> rationalize(long double x, T dnm_max = T(INFL), long double EPS = 1e-17) {
	// 参考 : https://ja.wikipedia.org/wiki/%E9%80%A3%E5%88%86%E6%95%B0
	// verify : https://yukicoder.me/problems/no/2266

	T sign = (x >= 0 ? 1 : -1);
	x = abs(x);

	vector<T> ps{ 1, (ll)x };
	vector<T> qs{ 0, 1 };

	// x の正則連分数展開に基づく近似を行う．
	repi(i, 2, INF) {
		x -= floor(x);
		if (x < EPS) break;
		x = 1 / x; // 誤差やばい

		T a = T(x);

		// a * qs[i - 1] + qs[i - 2] > dnm_max となるなら打ち切り
		if (a > (dnm_max - qs[i - 2]) / qs[i - 1]) break;

		ps.push_back(a * ps[i - 1] + ps[i - 2]);
		qs.push_back(a * qs[i - 1] + qs[i - 2]);
	}

	return { sign * ps.back(), qs.back() };
}


//【真分数 → 循環小数】O(m)
/*
* 真分数 frac = n / m の非循環部分の桁の数を noncycle に，
* 循環部分の桁の数を cycle にそれぞれ格納する．
*/
void real_digits(pii frac, vi& noncycle, vi& cycle, int base = 10) {
	// verify : https://atcoder.jp/contests/abc174/tasks/abc174_c

	int n, m;
	tie(n, m) = frac; // n / m

	noncycle.clear();
	cycle.clear();

	// last[i] : 余りが i になる割り算が何回目に起こったか
	vi last(m, -1);
	last[n] = 0;

	// 循環を検出するまで割り算を続ける．
	int i = 1, q, r;
	while (true) {
		n *= base;
		q = n / m;
		r = n % m;

		noncycle.push_back(q);

		// 割り切れた場合
		if (r == 0) return;

		// 循環を見つけた場合
		if (last[r] >= 0) break;

		last[r] = i++;
		n = r;
	}

	repi(j, last[r], i - 1) {
		cycle.push_back(noncycle[j]);
	}
	noncycle.resize(last[r]);
}


//【循環小数 → 真分数】O(m)
/*
* 非循環部分の桁の数が noncycle，循環部分の桁の数が cycle であるような
* 既約真分数 frac = n / m を {n, m} の形式で返す．
*/
pll from_real_digits(const vi& noncycle, const vi& cycle, int base = 10) {
	// n_nc, m_nc : 非循環部分の分子，分母
	ll n_nc = 0, m_nc = 1;
	repir(i, sz(noncycle) - 1, 0) {
		n_nc += noncycle[i] * m_nc;
		m_nc *= base;
	}
	// 有限小数の場合
	if (cycle.empty()) {
		ll g = gcd(n_nc, m_nc);
		n_nc /= g;
		m_nc /= g;
		return { n_nc, m_nc };
	}

	// n_c, m_c : 非循環部分の分子，分母（循環節が長いとオーバーフローするので注意）
	ll n_c = 0, m_c = 1;
	repir(i, sz(cycle) - 1, 0) {
		n_c += cycle[i] * m_c;
		m_c *= base;
	}
	ll mul = m_c - 1;
	m_c = mul * m_nc;

	// 通分して和をとる
	ll n = mul * n_nc + n_c;
	ll m = m_c;

	// 約分する
	ll g = gcd(n, m);
	n /= g;
	m /= g;
	return { n, m };
}


//【完全循環素数と原始根】
/*
* b 進法において，b と互いに素な素数 p について以下が成り立つ：
*	1/p の循環節の長さが p-1 ⇔ b は mod p の原始根
*
* verify : https://yukicoder.me/problems/no/1177
*/


//【小数第 n 位の数】O(log n)
/*
* num / dnm の b 進法での小数第 n 位の数を返す．
*
* 制約：dnm * b <= 2*10^9+1000
*/
int real_digit(int num, int dnm, ll n, int b = 10) {
	// verify : https://yukicoder.me/problems/no/1842

	//【方法】
	// 整数問題に帰着することを目指して変形する．
	// 
	// num / dnm の小数第 n 位の数は，num * b^n / dnm の 1 の位の数に等しい．
	// num * b^n を dnm で割った余りを r とおくと，求める数 x は
	//		x = ((num * b^n - r) / dnm)  (mod b)
	// と表される．両辺および法を dnm 倍し，両辺に r を加えることで
	//		dnm x + r = num * b^n  (mod (dnm b))
	// を得る．
	//
	// x は桁の数より 0 <= x < b ⇔ 0 <= x <= b - 1 である．
	// また r は dnm で割った余りより 0 <= r < dnm である．
	// 両者より左辺は
	//		0 <= dnm x + r < dnm (b - 1) + dnm = dnm b
	// と評価できるから，
	//		y = num * b^n mod (dnm b)
	// とおくと，
	//		x = (y - r) / dnm = y / dnm - r / dnm
	// である．0 <= r < dnm であったから，これはより簡潔に
	//		x = floor(y / dnm)
	// と表される．

	modint::set_mod(dnm * b);
	modint y = num * modint(b).pow(n);
	int res = y.val() / dnm;

	return res;
}


//【小数の読み込み】
/*
* 小数部分が k 桁以下の実数を誤差なく扱いたい場合，10^k 倍すれば整数になるので，
*		double x_; cin >> x_;
*		ll x = (ll)(x_ * 1ek + (x_ > 0 ? 0.5 : -0.5));
* とすれば良い．（キャスト時に 0 方向への丸めが入ることに注意．）
*
* verify : https://atcoder.jp/contests/arc015/tasks/arc015_2
*/


//【スターンブロコット木上の探索】(TODO)
/*
* https://atcoder.jp/contests/abc294/editorial/6017
*/


