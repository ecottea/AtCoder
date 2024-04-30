#pragma once
#include "header.h"
// ■■■■■ 周期性 ■■■■■


//【周期数列（配列）】
/*
* Periodic_sequence_sum<T>(vT a, int n0) : O(n)
*	A = a[0..n0) + a[n0..n)*∞ なる数列 A[0..∞) で初期化する．
*
* T sum(ll n)
*	ΣA[0..n) を返す．
*/
template <class T>
class Periodic_sequence {
	// n0 : 非周期部分の長さ，n1 : 周期部分の長さ
	int n0, n1;

	// acc0[i] : ΣA[0..i)，acc1[i] : ΣA[n0..n0+i)
	vector<T> acc0, acc1;

public:
	// A[0..∞) = a[0..n0) + a[n0..n)*∞ なる無限数列 A で初期化する．
	Periodic_sequence(const vector<T>& a, int n0) : n0(n0), n1(sz(a) - n0) {
		// verify : https://projecteuler.net/problem=167

		Assert(n0 >= 0); Assert(n1 >= 0);
		acc0.resize(n0 + 1);
		acc1.resize(n1 + 1);
		rep(i, n0) acc0[i + 1] = acc0[i] + a[i];
		rep(i, n1) acc1[i + 1] = acc1[i] + a[n0 + i];
	}

	// Σi = [0..n) a[i] を返す．
	T sum(ll n) {
		// verify : https://projecteuler.net/problem=167

		if (n <= n0) return acc0[n];

		T res = acc0[n0];
		n -= n0;
		res += acc1[n1] * (n / n1) + acc1[n % n1];

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Periodic_sequence& p) {
		os << "acc0: " << p.n0 << "(" << p.acc0 << ")" << endl;
		os << "acc1: " << p.n1 << "(" << p.acc1 << ")" << endl;
		return os;
	}
#endif
};


//【周期文字列】
/*
* Cyclic_string(string s, int C = 26, char a = 'a', ll M = 1) : O(n C)
*	S[0..Mn) = s[0..n)×M で初期化する．文字種は a から始まる連続する C 種類とする．
*
* ll next(ll l, char c, ll k = 0) : O(1)
*	S[l..Mn) 内の文字 c の左から k 番目（0-indexed）の位置を返す（なければ Mn を返す）
*
* ll prev(ll r, char c, ll k = 0) : O(1)
*	S[0..r) 内の文字 c の右から k 番目（0-indexed）の位置を返す（なければ -1 を返す）
*
* ll count(ll l, ll r, char c) : O(1)
*	S[l..r) 内の文字 c の個数を返す．
*/
class Cyclic_string {
	int n;

	int C; int a; ll M;

	// pos[c] : s[0..n) 内の文字 c がある位置の昇順リスト
	vvi pos;

	// acc[c][i] : 文字 c が s[0..i) に含まれている個数
	vvi acc;

	//【備考】
	// acc に対してさらに c 方向に累積和をとっておけば，
	// 自身より大きい[小さい] 文字に関するなんやかんやも O(1) で処理できるようになる．

public:
	// S[0..Mn) = s[0..n)×M で初期化する．文字種は a から始まる連続する C 種類とする．
	Cyclic_string(const string& s, int C = 26, char a = 'a', ll M = 1)
		: n(sz(s)), C(C), a(a), M(M), pos(C), acc(C, vi(n + 1))
	{
		// verify : https://atcoder.jp/contests/abc346/tasks/abc346_f

		rep(i, n) {
			int c = s[i] - a;
			pos[c].emplace_back(i);
			rep(c2, C) acc[c2][i + 1] += acc[c2][i] + (c2 == c);
		}
	}

	// S[l..Mn) 内の文字 c の左から k 番目（0-indexed）の位置を返す（なければ Mn を返す）
	ll next(ll l, char c, ll k = 0) {
		// verify : https://atcoder.jp/contests/abc346/tasks/abc346_f

		c -= a;
		Assert(0 <= c && c < C);

		if (l >= M * n) return M * n;
		chmax(l, 0LL);

		// K : s[0..n) 内の文字 c の個数
		ll K = acc[c][n];
		if (K == 0) return M * n;

		// S[0..Mn) 内の左から k 番目とする．
		k += (l / n) * K + acc[c][l % n];
		if (k >= M * K) return M * n;

		// ni : S[0..Mn) 内の左から k 番目の文字 c の位置
		ll ni = (k / K) * n + pos[c][k % K];
		if (ni > M * n) ni = M * n;

		return ni;
	}

	// S[0..r) 内の文字 c の右から k 番目（0-indexed）の位置を返す（なければ -1 を返す）
	ll prev(ll r, char c, ll k = 0) {
		c -= a;
		Assert(0 <= c && c < C);

		if (r < 0) return -1LL;
		chmin(r, M * n);

		// K : s[0..n) 内の文字 c の個数
		ll K = acc[c][n];
		if (K == 0) return -1LL;

		// S[0..Mn) 内の左から k 番目とする．
		k = (r / n) * K + acc[c][r % n] - 1 - k;
		if (k < 0) return -1LL;

		// ni : S[0..Mn) 内の左から k 番目の文字 c の位置
		ll ni = (k / K) * n + pos[c][k % K];

		return ni;
	}

	// S[l..r) 内の文字 c の個数を返す．
	ll count(ll l, ll r, char c) {
		c -= a;
		Assert(0 <= c && c < C);

		chmax(l, 0LL); chmin(r, M * n);
		if (l >= r) return 0LL;

		// K : s[0..n) 内の文字 c の個数
		ll K = acc[c][n];

		ll res = (r / n) * K + acc[c][r % n];
		res -= (l / n) * K + acc[c][l % n];

		return res;
	}
};


//【周期数列の和（漸化式）】
/*
* Periodic_sequence_sum(function<T(T)> f, T a0) : O(nc + c)
*	a[i+1] = f(a[i]), a[0] = a0 なる数列で初期化する．
*
* int nc : O(1)
*	数列 a の非周期部分の長さ．
*
* int c : O(1)
*	数列 a の周期部分の長さ．
*
* T sum(ll n)
*	Σi=[0..n) a[i] を返す．
*
*（フロイドの循環検出法）
*/
template <class T>
struct Periodic_sequence_sum {
	// nc : 非周期部分の長さ，c : 周期部分の長さ
	int nc, c;

	// nc_acc[i] : Σa[0..i)，c_acc[i] : Σa[nc..nc+i)
	vector<T> nc_acc, c_acc;

	// a[i+1] = f(a[i]), a[0] = a0 なる数列で初期化する．
	Periodic_sequence_sum(const function<T(T)>& f, T a0) : nc(0), c(0), nc_acc({ 0 }), c_acc({ 0 }) {
		// verify : https://atcoder.jp/contests/abc179/tasks/abc179_e
		
		T x = a0, y = a0;
		do {
			x = f(x);
			y = f(f(y));
		} while (x != y);

		x = a0;
		while (x != y) {
			nc_acc.push_back(nc_acc[nc] + x);
			nc++;

			x = f(x);
			y = f(y);
		}

		do {
			c_acc.push_back(c_acc[c] + x);
			c++;

			x = f(x);
			y = f(f(y));
		} while (x != y);
	}

	// Σi = [0..n) a[i] を返す．
	T sum(ll n) {
		// verify : https://atcoder.jp/contests/abc179/tasks/abc179_e
		
		if (n <= nc) return nc_acc[n];

		T res = nc_acc[nc];
		n -= nc;
		res += c_acc[c] * (n / c);
		res += c_acc[n % c];

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Periodic_sequence_sum& pss) {
		os << "nc: " << pss.nc_acc << "(" << pss.nc << ")" << endl;
		os << " c: " << pss.c_acc << "(" << pss.c << ")" << endl;
		return os;
	}
#endif
};


//【フロイドの循環検出法】O(nc + c)
/*
* a[i+1] = f(a[i]), a[0] = a0 なる数列について，a[0] から始まる非周期列の長さ nc と
* a[nc] から始まる周期列の長さ c の組 {nc, c} を返す．
*/
template <class T>
pii floyds_cycle_finding(const function<T(T)>& f, T a0) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%95%E3%83%AD%E3%82%A4%E3%83%89%E3%81%AE%E5%BE%AA%E7%92%B0%E6%A4%9C%E5%87%BA%E6%B3%95
	// verify : https://atcoder.jp/contests/abc030/tasks/abc030_d

	T x = a0, y = a0;
	do {
		x = f(x);
		y = f(f(y));
	} while (x != y);

	x = a0;
	int nc = 0;
	while (x != y) {
		x = f(x);
		y = f(y);
		nc++;
	}

	int c = 0;
	do {
		x = f(x);
		y = f(f(y));
		c++;
	} while (x != y);

	return make_pair(nc, c);

	/* f の定義の雛形
	using T = int;
	function<T(T)> f = [&](T x) {
		return 0;
	};
	*/
}


//【列の周期の候補】O(n)
/*
* 与えられた列 a[0..n) に対し，a[n-2t..n-t) = a[n-t..n) を満たす t を
* 降順に 2 個求め，その GCD を返す（2 個なければ -1）
*
* 利用：【ローリングハッシュ（列）】
*/
template <class STR>
int pseudo_cycle(const STR& a) {
	// verify : https://atcoder.jp/contests/arc172/tasks/arc172_e

	int n = sz(a);
	Rolling_hash A(a);

	int res = 0; int k = 2;

	repir(t, n / 2, 1) {
		if (A.get(n - 2 * t, n - t) == A.get(n - t, n)) {
			res = gcd(res, t);
			if (--k == 0) break;
		}
	}
	if (k > 0) res = -1;

	return res;
}


//【周期境界条件の扱い】
/*
* 周期境界条件は扱いにくいので，次のようにして扱うとよい：
* 
* 列を 2 倍に伸ばし，任意の区間が連続的に並ぶようにする．
* verify : https://atcoder.jp/contests/arc024/tasks/arc024_2
* 
* 左端を決め打ちして計算し，右端で条件を満たすものだけを採用する．
* verify : https://atcoder.jp/contests/abc229/tasks/abc229_f
*/


//【列の最小周期】O(n)
/*
* 分析(文字列).h へ
*/

