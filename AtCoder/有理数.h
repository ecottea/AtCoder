#pragma once
#include "header.h"
#include "数論(一括).h"
// ■■■■■ 有理数 ■■■■■


//【有理数】
/*
* Frac<T>() : O(1)
*	0 で初期化する．
*	制約：T は int, ll, __int128, boost::multiprecision::int256_t 等
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
*
* together(Frac& a, Frac& b) : O(log min(a.dnm, b.dnm))
*	a と b を通分する．
*
* together(vector<Frac>& as) : O(|as| log dnm)
*	as を通分する．
*
* T floor() : O(1)
*	自身の floor を返す．
*
* T ceil() : O(1)
*	自身の ceil を返す．
*
* bool integerQ() : O(1)
*	自身が整数かを返す．
*/
template <class T = ll>
struct Frac {
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
	operator double() const { return (double)num / (double)dnm; }

	// 比較
	bool operator==(const Frac& b) const {
		// 分母が等しいときはオーバーフロー防止のために掛け算はせず比較する．
		if (dnm == b.dnm) return num == b.num;
		return num * b.dnm == b.num * dnm;
	}
	bool operator!=(const Frac& b) const { return !(*this == b); }
	bool operator<(const Frac& b) const {
		// verify : https://atcoder.jp/contests/abc308/tasks/abc308_c

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
		// verify : https://atcoder.jp/contests/abc229/tasks/abc229_h

		auto g = gcd(num, dnm);
		num /= g; dnm /= g;
	}

	// a と b を通分する．
	friend void together(Frac& a, Frac& b) {
		// verify : https://atcoder.jp/contests/abc229/tasks/abc229_h

		T dnm = lcm(a.dnm, b.dnm);
		a.num *= dnm / a.dnm; a.dnm = dnm;
		b.num *= dnm / b.dnm; b.dnm = dnm;
	}

	// as を通分する．
	friend void together(vector<Frac>& as) {
		// verify : https://yukicoder.me/problems/617

		T dnm = 1;
		repe(a, as) dnm = lcm(dnm, a.dnm);

		repea(a, as) {
			a.num *= dnm / a.dnm;
			a.dnm = dnm;
		}
	}

	// 自身の floor を返す．
	T floor() const {
		// verify : https://www.codechef.com/problems/LINEFIT?tab=statement

		if (num >= 0) return num / dnm;
		else return -((-num + dnm - 1) / dnm);
	}

	// 自身の ceil を返す．
	T ceil() const {
		// verify : https://www.codechef.com/problems/LINEFIT?tab=statement

		if (num >= 0) return (num + dnm - 1) / dnm;
		else return -((-num) / dnm);
	}

	// 自身が整数かを返す．
	bool integerQ() const {
		// verify : https://atcoder.jp/contests/ttpc2022/tasks/ttpc2022_g

		return num % dnm == 0;
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


//【通分】O(B log(log B) + n (log B)^2)（B = max(b)）
/*
* Σi∈[0..n) a[i]/b[i] を既約分数 num/dnm で表したときの {num, dnm} を返す．
*
* 利用：【素因数分解（複数）】，【素因数の個数】
*/
pair<mint, mint> together(const vi& a, const vi& b) {
	// verify : https://yukicoder.me/problems/no/1931

	int n = sz(a);

	int B = *max_element(all(b));
	Osa_k O(B);

	// pps_b[i] : b[i] の素因数分解
	vector<map<int, int>> pps_b(n);
	rep(i, n) pps_b[i] = O.factor_integer(b[i]);

	// pps_lcm : L = LCM(b[0..n)) の素因数分解
	// p_to_ei[p] : 素因数 p をもつ b[i] の {ord_p(b[i]), i} のリスト
	unordered_map<int, int> pps_lcm;
	unordered_map<int, vector<pii>> p_to_ei;
	rep(i, n) for (auto [p, e] : pps_b[i]) {
		chmax(pps_lcm[p], e);
		p_to_ei[p].push_back({ e, i });
	}

	// dnm : 分母
	mint dnm = 1;

	for (auto [p, e] : pps_lcm) {
		// (分子)/L が p で何回約分できるか調べるため e = ord_p(L) とおき mod p^e で考える．
		int pe = (int)pow(p, e);
		modint::set_mod(pe);

		// sum : p^e Σi∈[0..n) a[i]/b[i] (mod p^e)
		modint sum = 0;

		// ord_p(b[i]) = 0 の項は 0 になるので，その他の項だけの和をとる．
		for (auto [ei, i] : p_to_ei[p]) {
			int pei = (int)pow(p, pps_b[i][p]);
			sum += ll(a[i]) * (pe / pei) * modint(b[i] / pei).inv();
		}

		// e_del : sum が p で何回割り切れるか
		int e_del = min(integer_exponent(sum.val(), p), e);

		// 分母に残る素因数 p の個数は e - e_del 個となる．
		dnm *= powi(p, e - e_del);
	}

	// 素朴に和を計算して dnm 倍すれば分子 num が求まる．
	mint num = 0;
	rep(i, n) num += mint(a[i]) / b[i];
	num *= dnm;

	return { num, dnm };
}


//【スターン・ブロコット木】
/*
* vector<pcT> to_path(T n, T d) : O(log min(n, d))
*	1/1 から n/d までのパスを，左[右] への移動を 'L'['R'] と表した上で連長圧縮して返す．
*
* pTT from_path(vector<pcT> path) : O(|path|)
*	1/1 から path に沿って移動した先の既約分数を n/d とし，組 {n, d} を返す．
*
* pTT lca(T n1, T d1, T n2, T d2) : O(log min(n1, d1, n2, d2))
*	n1/d1 と n2/d2 との LCA を n/d とし，組 {n, d} を返す．
*
* pTT ancestor(T n, T d, T dep) : O(log min(n, d, dep))
*	n/d の祖先であって深さが dep の有理数を np/dp とし，組 {np, dp} を返す（なければ {-1, -1}）
*
* tTTTT range(T n, T d) : O(log min(n, d))
*	n/d の子孫が属する開区間を (nl/dl, nr/dr) とし，4 つ組 {nl, dl, nr, dr} を返す．
*
* tTTTT bin_search<T>(bool okQ(ll n, ll d), T v_max = INFL) : O(log v_max)
*	分母分子がともに v_max 以下の有理数のうち，okQ() の true と false の境界の左右で
*	最も深い位置にあるものを nl/dl < nr/dr とし，組 {nl, dl, nr, dr} を返す．
*
* pair<vector<tTTTTT>, vector<tTTTTT>> best_approximation_fraction(T n, T d) : O(log min(n, d))
*	n/d の正の {下側最良近似分数の列の列, 上側最良近似分数の列の列} の組を返す．
*	最良近似分数の列 n0/d0, (n0+Δn)/(d0+Δd), ...(k 個)..., (n0+(k-1)Δn)/(d0+(k-1)Δd) は
*	5 つ組 {n0, d0, Δn, Δd, k} > 0 で表す．
*	注意 : 正の制約がなければ 0/1 が最良近似分数である可能性もある．
*/
namespace Stern_brocot_tree {
	// 1/1 から n/d までのパスを，左[右] への移動を 'L'['R'] と表した上で連長圧縮して返す．
	template <class T = ll>
	vector<pair<char, T>> to_path(T n, T d) {
		// verify : https://judge.yosupo.jp/problem/stern_brocot_tree

		T g = gcd(n, d);
		n /= g;
		d /= g;

		T nl = 0, dl = 1;
		T nr = 1, dr = 0;
		T nm = 1, dm = 1;
		vector<pair<char, T>> path;

		// nm/dm < n/d なら始めは右に移動，さもなくば左に移動．
		int dir = (nm * d < n * dm) ? 1 : -1;

		while (1) {
			if (nm == n && dm == d) break;

			// 右に移動
			if (dir == 1) {
				// k : num/dnm 以上の値になるまでの移動回数
				T tmp = d * nr - dr * n;
				T k = (dm * n - d * nm + tmp - 1) / tmp;

				path.emplace_back('R', k);
				nm += k * nr;
				dm += k * dr;
				nl = nm - nr;
				dl = dm - dr;
			}
			// 左に移動
			else {
				// k : num/dnm 以下の値になるまでの移動回数
				T tmp = dl * n - d * nl;
				T k = (d * nm - dm * n + tmp - 1) / tmp;

				path.emplace_back('L', k);
				nm += k * nl;
				dm += k * dl;
				nr = nm - nl;
				dr = dm - dl;
			}

			dir *= -1;
		}

		return path;
	}

	// 1/1 から path に沿って移動した先の分数を n/d とし，組 {n, d} を返す．
	template <class T = ll>
	pair<T, T> from_path(const vector<pair<char, T>>& path) {
		// verify : https://judge.yosupo.jp/problem/stern_brocot_tree

		T nl = 0, dl = 1;
		T nr = 1, dr = 0;
		T nm = 1, dm = 1;

		for (auto [c, k] : path) {
			// 右に移動
			if (c == 'R') {
				nm += k * nr;
				dm += k * dr;
				nl = nm - nr;
				dl = dm - dr;
			}
			// 左に移動
			else {
				nm += k * nl;
				dm += k * dl;
				nr = nm - nl;
				dr = dm - dl;
			}
		}

		return { nm, dm };
	}

	// n1/d1 と n2/d2 との LCAを n/d とし，組 {n, d} を返す．
	template <class T = ll>
	pair<T, T> lca(T n1, T d1, T n2, T d2) {
		// verify : https://judge.yosupo.jp/problem/stern_brocot_tree

		T g1 = gcd(n1, d1);
		n1 /= g1;
		d1 /= g1;

		T g2 = gcd(n2, d2);
		n2 /= g2;
		d2 /= g2;

		T nl = 0, dl = 1;
		T nr = 1, dr = 0;
		T nm = 1, dm = 1;

		// nm/dm < n/d なら始めは右に移動，さもなくば左に移動．
		int dir1 = (nm * d1 < n1 * dm) ? 1 : -1;
		int dir2 = (nm * d2 < n2 * dm) ? 1 : -1;
		if (dir1 != dir2) return { 1, 1 };

		while (1) {
			if (nm == n1 && dm == d1) return { n1, d1 };
			if (nm == n2 && dm == d2) return { n2, d2 };

			// 右に移動
			if (dir1 == 1) {
				// k : num/dnm 以上の値になるまでの移動回数
				T tmp1 = d1 * nr - dr * n1;
				T k1 = (dm * n1 - d1 * nm + tmp1 - 1) / tmp1;
				T tmp2 = d2 * nr - dr * n2;
				T k2 = (dm * n2 - d2 * nm + tmp2 - 1) / tmp2;

				if (k1 < k2) return { nm + k1 * nr, dm + k1 * dr };
				if (k1 > k2) return { nm + k2 * nr, dm + k2 * dr };

				nm += k1 * nr;
				dm += k1 * dr;
				nl = nm - nr;
				dl = dm - dr;
			}
			// 左に移動
			else {
				// k : num/dnm 以下の値になるまでの移動回数
				T tmp1 = dl * n1 - d1 * nl;
				T k1 = (d1 * nm - dm * n1 + tmp1 - 1) / tmp1;
				T tmp2 = dl * n2 - d2 * nl;
				T k2 = (d2 * nm - dm * n2 + tmp2 - 1) / tmp2;

				if (k1 < k2) return { nm + k1 * nl, dm + k1 * dl };
				if (k1 > k2) return { nm + k2 * nl, dm + k2 * dl };

				nm += k1 * nl;
				dm += k1 * dl;
				nr = nm - nl;
				dr = dm - dl;
			}

			dir1 *= -1;
		}

		return { -1, -1 };
	}

	// n/d の祖先であって深さが dep の有理数を np/dp とし，組 {np, dp} を返す．
	template <class T = ll>
	pair<T, T> ancestor(T n, T d, T dep) {
		// verify : https://judge.yosupo.jp/problem/stern_brocot_tree

		T g = gcd(n, d);
		n /= g;
		d /= g;

		T nl = 0, dl = 1;
		T nr = 1, dr = 0;
		T nm = 1, dm = 1;

		// nm/dm < n/d なら始めは右に移動，さもなくば左に移動．
		int dir = (nm * d < n * dm) ? 1 : -1;

		while (1) {
			if (nm == n && dm == d) break;

			// 右に移動
			if (dir == 1) {
				// k : num/dnm 以上の値になるまでの移動回数
				T tmp = d * nr - dr * n;
				T k = (dm * n - d * nm + tmp - 1) / tmp;

				if (k >= dep) return { nm + dep * nr, dm + dep * dr };
				dep -= k;

				nm += k * nr;
				dm += k * dr;
				nl = nm - nr;
				dl = dm - dr;
			}
			// 左に移動
			else {
				// k : num/dnm 以下の値になるまでの移動回数
				T tmp = dl * n - d * nl;
				T k = (d * nm - dm * n + tmp - 1) / tmp;

				if (k >= dep) return { nm + dep * nl, dm + dep * dl };
				dep -= k;

				nm += k * nl;
				dm += k * dl;
				nr = nm - nl;
				dr = dm - dl;
			}

			dir *= -1;
		}

		return { -1, -1 };
	}

	// n/d の子孫が属する開区間を (nl/dl, nr/dr) とし，4 つ組 {nl, dl, nr, dr} を返す．
	template <class T = ll>
	tuple<T, T, T, T> range(T n, T d) {
		// verify : https://judge.yosupo.jp/problem/stern_brocot_tree

		T g = gcd(n, d);
		n /= g;
		d /= g;

		T nl = 0, dl = 1;
		T nr = 1, dr = 0;
		T nm = 1, dm = 1;

		// nm/dm < n/d なら始めは右に移動，さもなくば左に移動．
		int dir = (nm * d < n * dm) ? 1 : -1;

		while (1) {
			if (nm == n && dm == d) break;

			// 右に移動
			if (dir == 1) {
				// k : num/dnm 以上の値になるまでの移動回数
				T tmp = d * nr - dr * n;
				T k = (dm * n - d * nm + tmp - 1) / tmp;

				nm += k * nr;
				dm += k * dr;
				nl = nm - nr;
				dl = dm - dr;
			}
			// 左に移動
			else {
				// k : num/dnm 以下の値になるまでの移動回数
				T tmp = dl * n - d * nl;
				T k = (d * nm - dm * n + tmp - 1) / tmp;

				nm += k * nl;
				dm += k * dl;
				nr = nm - nl;
				dr = dm - dl;
			}

			dir *= -1;
		}

		return { nl, dl, nr, dr };
	}

	// okQ() の true と false の境界を返す．
	template <class T = ll, class FUNC>
	tuple<T, T, T, T> bin_search(const FUNC& okQ, T v_max = T(INFL)) {
		// verify : https://projecteuler.net/problem=192

		T nl = 0, dl = 1; bool bl = okQ(nl, dl);
		T nr = 1, dr = 0; bool br = okQ(nr, dr);
		T nm = 1, dm = 1; bool bm = okQ(nm, dm);

		while (1) {
			// 右に移動
			if (bl == bm) {
				// k_max : nm, dm が v_max を超えない k の最大値
				T k_max = T(INFL);
				if (nr > 0) chmin(k_max, (v_max - nm) / nr);
				if (dr > 0) chmin(k_max, (v_max - dm) / dr);

				// k : okQ(nm/dm) が切り替わるまでの移動回数
				T k_ng = 0, k_ok = 1;

				// k の丁度いい上限がわからないのでまず指数探索を行う．
				while (okQ(nm + k_ok * nr, dm + k_ok * dr) == bm) {
					k_ng = k_ok;
					k_ok *= 2;

					// 十分深くまで探しても T/F が切り替わらなかったら，
					// nm/dm の子孫は全て同じ T/F であると判断し開区間の右の境界を返す．
					if (k_ng > k_max) return { nm + k_max * nr, dm + k_max * dr, nr, dr };
				}

				// 判明した k の上下界を用いて二分探索を行う．
				while (k_ok - k_ng > 1) {
					T k_mid = (k_ok + k_ng) / 2;

					if (okQ(nm + k_mid * nr, dm + k_mid * dr) != bm) k_ok = k_mid;
					else k_ng = k_mid;
				}

				if (k_ok > k_max) return { nm + k_max * nr, dm + k_max * dr, nr, dr };

				bm = br;
				nm += k_ok * nr;
				dm += k_ok * dr;
				nl = nm - nr;
				dl = dm - dr;
			}
			// 左に移動
			else {
				// k_max : nm, dm が v_max を超えない k の最大値
				T k_max = T(INFL);
				if (nl > 0) chmin(k_max, (v_max - nm) / nl);
				if (dl > 0) chmin(k_max, (v_max - dm) / dl);

				// k : okQ(nm/dm) が切り替わるまでの移動回数
				T k_ng = 0, k_ok = 1;

				// k の丁度いい上限がわからないのでまず指数探索を行う．
				while (okQ(nm + k_ok * nl, dm + k_ok * dl) == bm) {
					k_ng = k_ok;
					k_ok *= 2;

					// 十分深くまで探しても T/F が切り替わらなかったら，
					// nm/dm の子孫は全て同じ T/F であると判断し開区間の左の境界を返す．
					if (k_ng > k_max) return { nl, dl, nm + k_max * nl, dm + k_max * dl };
				}

				// 判明した k の上下界を用いて二分探索を行う．
				while (k_ok - k_ng > 1) {
					T k_mid = (k_ok + k_ng) / 2;

					if (okQ(nm + k_mid * nl, dm + k_mid * dl) != bm) k_ok = k_mid;
					else k_ng = k_mid;
				}

				if (k_ok > k_max) return { nl, dl, nm + k_max * nl, dm + k_max * dl };

				bm = bl;
				nm += k_ok * nl;
				dm += k_ok * dl;
				nr = nm - nl;
				dr = dm - dl;
			}
		}
	}

	// n/d の最良近似分数を全て返す．
	template <class T = ll>
	pair<vector<tuple<T, T, T, T, T>>, vector<tuple<T, T, T, T, T>>> best_approximation_fraction(T n, T d) {
		// verify : https://atcoder.jp/contests/abc333/tasks/abc333_g

		T g = gcd(n, d);
		n /= g;
		d /= g;

		T nl = 0, dl = 1;
		T nr = 1, dr = 0;
		T nm = 1, dm = 1;
		vector<tuple<T, T, T, T, T>> fl, fr;

		// nm/dm < n/d なら始めは右に移動，さもなくば左に移動．
		int dir = (nm * d < n * dm) ? 1 : -1;

		while (1) {
			if (nm == n && dm == d) break;

			// 右に移動
			if (dir == 1) {
				// k : num/dnm 以上の値になるまでの移動回数
				T tmp = d * nr - dr * n;
				T k = (dm * n - d * nm + tmp - 1) / tmp;

				fl.emplace_back(nm, dm, nr, dr, k);

				nm += k * nr;
				dm += k * dr;
				nl = nm - nr;
				dl = dm - dr;
			}
			// 左に移動
			else {
				// k : num/dnm 以下の値になるまでの移動回数
				T tmp = dl * n - d * nl;
				T k = (d * nm - dm * n + tmp - 1) / tmp;

				fr.emplace_back(nm, dm, nl, dl, k);

				nm += k * nl;
				dm += k * dl;
				nr = nm - nl;
				dr = dm - dl;
			}

			dir *= -1;
		}

		return { fl, fr };
	}

	/* okQ の定義の雛形
	using T = ll;
	auto okQ = [&](T num, T dnm) {
		return true || false;
	};
	*/
};


//【最良近似分数（有理数）】O(log(num + dnm + N))
/*
* 分母と分子がともに N 以下であるような num/dnm の正の下[上]側最良近似分数を pl/ql[ pu/qu ] とし，
* 4 つ組 (pl, ql, pu, qu} を返す（存在しなければ pl/ql=0/1, pu/qu=1/0 を返す．）
*
* 制約 : num > 0, dnm > 0，N > 0
*
* 利用：【一次式の剰余の最小値】,【二元一次不定方程式】
*/
template <class T>
tuple<T, T, T, T> best_rational_approximation(T num, T dnm, T N) {
	// verify : https://judge.yosupo.jp/problem/rational_approximation

	bool swap_flag = false;
	if (num > dnm) {
		swap(num, dnm);
		swap_flag = true;
	}

	auto val_l = min_of_mod_of_linear<T>(N, dnm, num, num);
	T ql, tmp2;
	auto gl = bezout<T>(num, -dnm, val_l, ql, tmp2);
	if (ql == 0) ql = dnm / gl;
	auto pl = num * ql / dnm;

	auto val_u = min_of_mod_of_linear<T>(N, dnm, -num, -num);
	T qu, tmp3;
	auto gu = bezout<T>(-num, -dnm, val_u, qu, tmp3);
	if (qu == 0) qu = dnm / gu;
	auto pu = (num * qu + dnm - 1) / dnm;

	if (swap_flag) {
		swap(pl, qu);
		swap(pu, ql);
	}

	return { pl, ql, pu, qu };
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


//【真分数 → 循環小数】O(m)
/*
* 真分数 n/m の非循環部分の桁の数を noncycle に，
* 循環部分の桁の数を cycle にそれぞれ格納する．
*/
void real_digits(int n, int m, vi& noncycle, vi& cycle, int base = 10) {
	// verify : https://atcoder.jp/contests/abc174/tasks/abc174_c

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
* 既約真分数 frac = n/m を {n, m} の形式で返す．
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


//【[非]循環節の長さ】
/*
* 有理数 r を既約分数表示したものを n/m とするとき，r の 10 進小数表示において
*	(r の非循環節の長さ) = max(ord_2(m), ord_5(m)) （ord_p(m) は m のもつ素因数 p の個数）
* である．また
*	m' = m / (2^ord_2(m) * 5^ord_5(m))
* とおくと，
*	(r の循環節の長さ) = ord(10) in (Z/m'Z)*（ord は乗法群 (Z/m'Z)* における位数）
* である．
* 
* verify : https://mojacoder.app/users/YSatUT/problems/repeating_decimal
*/


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
* 制約：dnm * b ≦ 2 * 10^9 + 1000
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
	// x は桁の数より 0 ≦ x ≦ b - 1 である．
	// また r は dnm で割った余りより 0 ≦ r < dnm である．
	// 両者より左辺は
	//		0 ≦ dnm x + r < dnm (b - 1) + dnm = dnm b
	// と評価できるから，
	//		y = (num * b^n) mod (dnm b)
	// とおくと，
	//		x = (y - r) / dnm = y / dnm - r / dnm
	// である．0 ≦ r < dnm であったから，これはより簡潔に
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


//【有理数近似】O(log dnm)（誤差注意）
/*
* 実数 x を分母が dnm_max 以下の既約分数 num / dnm で近似し，組 {num, dnm} を返す．
* 結果は最良近似分数ではあるが，分母が dnm_max 以下の範囲での最良の近似であるとは限らないので注意．
*/
template <class T = ll>
pair<T, T> rationalize(long double x, T dnm_max = T(INFL), long double EPS = 1e-17) {
	// 参考 : https://ja.wikipedia.org/wiki/%E9%80%A3%E5%88%86%E6%95%B0
	// verify : https://yukicoder.me/problems/no/2266

	T sign = (x >= 0 ? 1 : -1);
	x = abs(x);

	vector<T> ps{ 1, (T)x };
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


//【既約分数】（激遅）
/*
* オーバーフローに気をつけて常に既約分数になるようにした結果実用性がなくなるほど遅くなった．
*
* IFrac<T>() : O(1)
*	0 で初期化する．
*
* IFrac<T>(T num) : O(1)
*	num で初期化する．
*
* IFrac<T>(T num, T dnm) : O(1)
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
*
* together(IFrac& a, IFrac& b) : O(log min(a.dnm, b.dnm))
*	a と b を通分する．
*
* together(vector<IFrac>& as) : O(|as| log dnm)
*	as を通分する．
*
* T floor() : O(1)
*	自身の floor を返す．
*
* T ceil() : O(1)
*	自身の ceil を返す．
*/
template <class T = ll>
struct IFrac {
	// 分子，分母
	T num, dnm;

	// コンストラクタ
	IFrac() : num(0), dnm(1) {}
	IFrac(T num) : num(num), dnm(1) {}
	IFrac(T num_, T dnm_) : num(num_), dnm(dnm_) {
		Assert(dnm != T(0));

		auto g = gcd(num, dnm);
		num /= g;
		dnm /= g;

		if (dnm < 0) { num *= -1; dnm *= -1; }
	}

	// 代入
	IFrac(const IFrac& b) = default;
	IFrac& operator=(const IFrac& b) = default;

	// キャスト
	operator double() const { return (double)num / (double)dnm; }

	// 比較
	bool operator==(const IFrac& b) const {
		auto g_num = gcd(num, b.num);
		if (g_num == 0) return true;

		auto g_dnm = gcd(dnm, b.dnm);
		return (num / g_num) * (b.dnm / g_dnm) == (b.num / g_num) * (dnm / g_dnm);
	}
	bool operator!=(const IFrac& b) const { return !(*this == b); }
	bool operator<(const IFrac& b) const {
		auto g_num = gcd(num, b.num);
		if (g_num == 0) return false;

		auto g_dnm = gcd(dnm, b.dnm);
		return (num / g_num) * (b.dnm / g_dnm) < (b.num / g_num) * (dnm / g_dnm);
	}
	bool operator>=(const IFrac& b) const { return !(*this < b); }
	bool operator>(const IFrac& b) const { return b < *this; }
	bool operator<=(const IFrac& b) const { return !(*this > b); }

	// 整数との比較
	bool operator==(T b) const {
		auto g_num = gcd(num, b);
		if (g_num == 0) return true;

		return (num / g_num) == (b / g_num) * dnm;
	}
	bool operator!=(T b) const { return !(*this == b); }
	bool operator<(T b) const {
		auto g_num = gcd(num, b);
		if (g_num == 0) return false;

		return (num / g_num) < (b / g_num) * dnm;
	}
	bool operator>=(T b) const { return !(*this < b); }
	bool operator>(T b) const { return b < *this; }
	bool operator<=(T b) const { return !(*this > b); }
	friend bool operator==(T a, const IFrac& b) { return b == a; }
	friend bool operator!=(T a, const IFrac& b) { return b != a; }
	friend bool operator<(T a, const IFrac& b) { return b > a; }
	friend bool operator>=(T a, const IFrac& b) { return b <= a; }
	friend bool operator>(T a, const IFrac& b) { return b < a; }
	friend bool operator<=(T a, const IFrac& b) { return b >= a; }

	// 四則演算
	IFrac& operator+=(const IFrac& b) {
		T l_dnm = lcm(dnm, b.dnm);
		num = num * (l_dnm / dnm) + b.num * (l_dnm / b.dnm);
		dnm = l_dnm;

		auto g = gcd(num, dnm);
		num /= g;
		dnm /= g;

		return *this;
	}
	IFrac& operator-=(const IFrac& b) {
		T l_dnm = lcm(dnm, b.dnm);
		num = num * (l_dnm / dnm) - b.num * (l_dnm / b.dnm);
		dnm = l_dnm;

		auto g = gcd(num, dnm);
		num /= g;
		dnm /= g;

		return *this;
	}
	IFrac& operator*=(const IFrac& b) {
		auto g1 = gcd(num, b.dnm);
		auto g2 = gcd(dnm, b.num);

		num = (num / g1) * (b.num / g2);
		dnm = (dnm / g2) * (b.dnm / g1);

		return *this;
	}
	IFrac& operator/=(const IFrac& b) {
		Assert(b.num != T(0));

		auto g1 = gcd(num, b.num);
		auto g2 = gcd(dnm, b.dnm);

		num = (num / g1) * (b.dnm / g2);
		dnm = (dnm / g2) * (b.num / g1);

		if (dnm < 0) { num *= -1; dnm *= -1; }

		return *this;
	}
	IFrac operator+(const IFrac& b) const { IFrac a = *this; return a += b; }
	IFrac operator-(const IFrac& b) const { IFrac a = *this; return a -= b; }
	IFrac operator*(const IFrac& b) const { IFrac a = *this; return a *= b; }
	IFrac operator/(const IFrac& b) const { IFrac a = *this; return a /= b; }
	IFrac operator-() const {
		IFrac a = *this;
		a.num *= -1;

		return a;
	}

	// 整数との四則演算
	IFrac& operator+=(T c) { num += dnm * c; return *this; }
	IFrac& operator-=(T c) { num -= dnm * c; return *this; }
	IFrac& operator*=(T c) {
		auto g2 = gcd(dnm, c);

		num = num * (c / g2);
		dnm = (dnm / g2);

		return *this;
	}
	IFrac& operator/=(T c) {
		Assert(c != T(0));

		auto g1 = gcd(num, c);

		num = (num / g1);
		dnm = dnm * (c / g1);

		if (dnm < 0) { num *= -1; dnm *= -1; }

		return *this;
	}
	IFrac operator+(T c) const { IFrac a = *this; return a += c; }
	IFrac operator-(T c) const { IFrac a = *this; return a -= c; }
	IFrac operator*(T c) const { IFrac a = *this; return a *= c; }
	IFrac operator/(T c) const { IFrac a = *this; return a /= c; }
	friend IFrac operator+(T c, const IFrac& a) { return a + c; }
	friend IFrac operator-(T c, const IFrac& a) { return IFrac(c) - a; }
	friend IFrac operator*(T c, const IFrac& a) { return a * c; }
	friend IFrac operator/(T c, const IFrac& a) { return IFrac(c) / a; }

	// a と b を通分する．
	friend void together(IFrac& a, IFrac& b) {
		T dnm = lcm(a.dnm, b.dnm);
		a.num *= dnm / a.dnm; a.dnm = dnm;
		b.num *= dnm / b.dnm; b.dnm = dnm;
	}

	// as を通分する．
	friend void together(vector<IFrac>& as) {
		T dnm = 1;
		repe(a, as) dnm = lcm(dnm, a.dnm);

		repea(a, as) {
			a.num *= dnm / a.dnm;
			a.dnm = dnm;
		}
	}

	// 自身の floor を返す．
	T floor() const {
		if (num >= 0) return num / dnm;
		else return -((-num + dnm - 1) / dnm);
	}

	// 自身の ceil を返す．
	T ceil() const {
		if (num >= 0) return (num + dnm - 1) / dnm;
		else return -((-num) / dnm);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const IFrac& a) { os << a.num << '/' << a.dnm; return os; }
#endif
};


