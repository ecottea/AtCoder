#pragma once
#include "header.h"
#include "数論.h"
// ■■■■■ 累乗 ■■■■■


//【累乗（mint 利用）】
/*
* Pow_mint(mint B, int n) : O(n)
*	底を B とし，B^0 から B^n まで計算可能として初期化する．
*
* build_neg() : O(n)
*	B^(-1) から B^(-n) も計算可能にする．
*	制約 : B は mint の法と互いに素
*
* mint [](int i) : O(1)
*	B^i を返す．
*/
class Pow_mint {
	int n;
	vm powB, powB_inv;

public:
	Pow_mint(mint B, int n) : n(max(n, 2)) {
		// verify : https://yukicoder.me/problems/no/2709

		// B の累乗を計算する．
		powB.resize(n + 1);
		powB[0] = 1;
		rep(i, n) powB[i + 1] = powB[i] * B;
	};
	Pow_mint() : n(0) {}

	// 負冪も計算できるようにする．
	void build_neg() {
		// verify : https://atcoder.jp/contests/arc116/tasks/arc116_b

		// B の逆元の累乗を計算する．
		mint invB = powB[1].inv();
		powB_inv.resize(n + 1);
		powB_inv[0] = 1;
		rep(i, n) powB_inv[i + 1] = powB_inv[i] * invB;
	}

	// B^i を返す．
	mint const& operator[](int i) const {
		// verify : https://atcoder.jp/contests/arc116/tasks/arc116_b

		Assert(abs(i) <= n);

		return i >= 0 ? powB[i] : powB_inv[-i];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Pow_mint& pw) {
		os << pw.powB << endl;
		os << pw.powB_inv << endl;
		return os;
	}
#endif
};


//【累乗（モノイド）】O(log n)
/*
* モノイド (S, op, e) の元 x の n 乗を返す．
*
*（繰り返し二乗法）
*/
template <class S, S(*op)(S, S), S(*e)()>
S pow(const S& x, ll n) {
	// verify : https://atcoder.jp/contests/abc013/tasks/abc013_4

	S res(e()), pow2 = x;
	while (n > 0) {
		if (n & 1) res = op(res, pow2);
		pow2 = op(pow2, pow2);
		n /= 2;
	}
	return res;
}


//【累乗（切り詰め）】O(log_a(inf))
/*
* 非負整数 a, n に対し min(a^n, inf) を返す．
*/
ll truncated_pow(ll a, ll n, ll inf = INFL) {
	// verify : https://atcoder.jp/contests/abc322/tasks/abc322_g

	Assert(a >= 0 && n >= 0);

	if (n == 0 || a == 1) return 1;
	if (a == 0) return 0;

	ll val = 1;
	for (ll i = 0; i < n; i++) {
		// val * a >= inf
		if (val >= (inf + a - 1) / a) {
			val = inf;
			break;
		}
		val *= a;
	}

	return val;
}


//【n より大きい最小の 2 冪】
/*
* n より大きい最小の 2 冪は 1 << (msb(n) + 1) で得られる．
* n 以上の最小の 2 冪は 1 << (msb(n - 1) + 1) で得られる（ただし n > 0）
*/


//【整数累乗根】O(n log a)
/*
* 非負の数 a の n 乗根（a^(1/n)）の切り捨て値を返す．
*/
template <class T>
T integer_root(T a, int n = 2) {
	// verify : https://atcoder.jp/contests/abc166/tasks/abc166_d

	if (a <= 1 || n == 1) return a;

	// x^k を返す．ただし a を超えた場合は a + 1 を返す．
	auto pow_lim = [&](T x, int k) {
		T v = 1;
		rep(i, k) {
			if (v > a / x) return a + 1;
			v *= x;
		}
		return v;
	};

	T ok = 1, ng = a + 1;

	while (abs(ok - ng) > 1) {
		T mid = (ok + ng) / 2;

		if (pow_lim(mid, n) <= a) ok = mid;
		else ng = mid;
	}

	return ok;
}


//【離散対数問題（M-集合）】O(√N)
/*
* f^n s = t を満たす N 未満の最小の非負整数 n を返す（存在しなければ INFL）
* f[s,t] は M-集合 (S, F, act, comp, id) の F[S,S] の元とする．
* HASH はハッシュ関数 size_t operator()(const S& p) の定義された関数オブジェクトとする．
*/
template <class S, class F, S(*act)(F, S), F(*comp)(F, F), F(*id)(), class HASH>
ll discrete_logarithm(const F& f, const S& s, const S& t, ll N) {
	// 参考 : https://maspypy.com/%e3%83%a2%e3%83%8e%e3%82%a4%e3%83%89%e4%bd%9c%e7%94%a8%e3%81%ab%e9%96%a2%e3%81%99%e3%82%8b%e9%9b%a2%e6%95%a3%e5%af%be%e6%95%b0%e5%95%8f%e9%a1%8c
	// verify : https://atcoder.jp/contests/utpc2014/tasks/utpc2014_k

	int m = (int)(sqrt(N) + 1e-12) + 1;

	// T : {f^i t | i∈[1..m]}
	unordered_set<S, HASH> T;
	S f_t(t);
	repi(i, 1, m) {
		// f_t : f^i t
		f_t = act(f, f_t);

		if (T.count(f_t)) break;
		T.insert(f_t);
	}

	// fm : f^m
	F fm(id()), pow2 = f; int m_tmp(m);
	while (m_tmp > 0) {
		if ((m_tmp & 1) != 0) fm = comp(fm, pow2);
		pow2 = comp(pow2, pow2);
		m_tmp /= 2;
	}

	S fm_s_bak(s); int fail_cnt = 0;
	repi(k, 1, m) {
		// fm_s : f^(m k) s, fm_s_bak : f^(m (k-1)) s
		S fm_s = act(fm, fm_s_bak);

		// f^(m k) s ∈ T となったなら，∃i∈[0..m), f^(m (k-1) + i) s = t となることが期待される．
		if (T.count(fm_s)) {
			S f_s(fm_s_bak);

			// f^(m (k-1) + i) s = t となっているかを全て調べる．
			rep(i, m) {
				// f_s : f^(m (k-1) + i) s
				if (f_s == t) return (ll)m * (k - 1) + i;

				f_s = act(f, f_s);
			}

			// t → f t なる有向辺をもった functional graph S を考える．
			// 先の手続きに失敗したとしても，いま初めて t を含むループに s から合流してきた可能性が残されている．
			// だがもしもう一度失敗したならば，t がループに含まれていないことを意味するので非存在が確定する．
			if (++fail_cnt == 2) return INFL;
		}

		fm_s_bak = fm_s;
	}

	return INFL;
}


//【累乗で累乗】O(√m)
/*
* a^(b^c) mod m を返す．
*
* 利用：【オイラー関数】
*/
int power_power(ll a, ll b, ll c, int m) {
	// verify : https://atcoder.jp/contests/abc228/tasks/abc228_e

	using mint_pm = dynamic_modint<47157>; // 他と被らなければ何でも良い．

	// a^0 = 1 の例外処理
	// 以降は b^c != 0 としてよい．
	if (b == 0 && c > 0) return 1;

	// m^(b^c) = 0 (mod m) の例外処理
	// 以降は a は m の倍数ではないとしてよい．
	if (a % m == 0) return 0;

	// a^(φ(m) + 1) = a なる pm = φ(m) を得る．
	ll pm = euler_phi(m);

	// d = b^c mod pm を求める．
	mint_pm::set_mod((int)pm);
	int d = mint_pm(b).pow(c).val();

	// a^(d+φ(m)) mod m を求める．
	mint_pm::set_mod(m);
	return mint_pm(a).pow(d + pm).val();
}


//【テトレーション】O(√m log b) ?
/*
* a↑↑b mod m を返す．（0^0 = 1 とする）
*
* 利用：【オイラー関数】
*/
int tetration(ll a, ll b, int m) {
	// verify : https://judge.yosupo.jp/problem/tetration_mod

	using mint_m = dynamic_modint<97968>; // 他と被らなければ何でも良い．

	// 特殊ケースの処理
	if (m == 1) return 0;
	if (b == 0) return 1;
	if (a == 0) return (b + 1) % 2;
	if (a == 1) return 1;

	// a↑↑b mod m を返す．
	function<int(ll, int)> rf = [&](ll b, int m) {
		// mod 1 なら値は 0
		if (m == 1) return 0;

		// a↑↑1 = a
		if (b == 1) return (int)(a % m);

		// 小さいケースの処理
		if (a == 2 && b <= 4) {
			const vi ans = { 1, 2, 4, 16, 65536 };
			return ans[b] % m;
		}
		if (a == 3 && b <= 2) {
			const vi ans = { 1, 3, 27 };
			return ans[b] % m;
		}

		// 最も下の a の肩は mod φ(m) で考えれば良い．
		int phi = (int)euler_phi(m);

		// どうせ値は大きいので 1 周余分に回しても問題ない．
		int d = rf(b - 1, phi) + phi;

		mint_m::set_mod(m);
		return (int)mint_m(a).pow(d).val();
	};

	return rf(b, m);
}


//【フロベニウス写像】
/*
* 標数 p の可換環においては，p 乗する写像は自己準同型になる．すなわち以下が成り立つ：
*	(a + b)^p = a^p + b^p, (a b)^p = a^p b^p
*/


//【mod p^n での p 乗】
/*
* a^p (mod p^n) の値は a mod p^(n-1) のみに依存して決まる．すなわち以下が成り立つ：
*	(a + p^(n-1))^p ≡ a^p (mod p^n)
*
* verify : https://yukicoder.me/problems/no/2344
*/


//【累乗和】
/*
* FPS(mint).h へ
*/


