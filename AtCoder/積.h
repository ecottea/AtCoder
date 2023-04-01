#pragma once
#include "header.h"
// ■■■■■ 添字積畳込み（ディリクレ積） ■■■■■


//【数論関数】
/*
* N を定義域とする関数 a を数論関数という．vector<T> a の a[0] を無視することで表す．
*/


//【ディリクレ畳込み（素朴）】O(n log n)
/*
* 数論関数 a[1..n] と b[1..n] のディリクレ畳込みを切り詰めた数論関数 c[1..n] を返す．
* すなわち c[k] = Σ_(i×j=k) a[i] b[j] である．
*/
template <class T>
vector<T> naive_dirichlet_convolution(const vector<T>& a, const vector<T>& b) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c
	// verify : https://atcoder.jp/contests/abc172/tasks/abc172_d

	int n = sz(a) - 1;

	// 配る DP
	vector<T> c(n + 1);
	repi(i, 1, n) for (ll j = 1; i * j <= n; j++) c[i * j] += a[i] * b[j];

	return c;
}


//【ディリクレ逆畳込み（素朴）】O(n log n)
/*
* 数論関数 a[1..n] と b[1..n] のディリクレ畳込みが c[1..n] であるとし，b を返す．
* すなわち c[k] = Σ_(i×j=k) a[i] b[j] である．
*
* 制約：a[1] ≠ 0
*/
template <class T>
vector<T> naive_dirichlet_invconvolution(const vector<T>& a, const vector<T>& c) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c
	// verify : https://atcoder.jp/contests/yahoo-procon2018-final/tasks/yahoo_procon2018_final_a

	//【方法】
	// ディリクレ畳込みの式の総和を i=1 とその他に分解すると
	//		c[k] = a[1] b[k] + Σ_(i×j=k,i>1) a[i] b[j]
	// となる．これを b[k] について解くと
	//		b[k] = (c[k] - Σ_(i×j=k,i>1) a[i] b[j]) / a[1]
	// となる．

	Assert(a[1] != 0);
	int n = sz(a) - 1;

	// 配る DP
	vector<T> b(c);
	repi(j, 1, n) {
		b[j] /= a[1];
		for (ll i = 2; i * j <= n; i++) b[i * j] -= a[i] * b[j];
	}

	return b;
}


//【ディリクレ級数とディリクレ畳込み】
/*
* 数論関数 a[1..n] のディリクレ母関数を A(s) = Σi a[i] / i^s などとおくと，
* 数論関数 a, b のディリクレ畳込み c に対応するディリクレ母関数は C(s) となる．
*/


//【乗法的数論関数】
/*
* a[1] = 1 かつ互いに素な任意の i,j ∈ N に対して
*		a[i×j] = a[i] a[j]
* が成り立つ数論関数 a を乗法的数論関数という．
*/


//【乗法的数論関数のディリクレ母関数のオイラー積表示】
/*
* 乗法的数論関数 a[1..n] のディリクレ母関数 A(s) は
*		A(s) = Π_p A_p(s)
*		A_p(s) := Σ_d a[p^d] / (p^d)^s
* なるオイラー積表示をもつ．
*/


//【乗法的数論関数の例】
/*
* 乗法的数論関数 a[i] とそのディリクレ母関数 A(s) の例として以下のものが挙げられる：
*	デルタ：		a[i] = (i = 1 ? 1 : 0)							A(s) = 1
*	定数 1：		a[i] = 1										A(s) = ζ(s)
*	累乗：		a[i] = i^d  (d は定数)							A(s) = ζ(s-d)
*	約数和：		a[i] = i の約数の k 乗和  （k は定数）			A(s) = ζ(s)ζ(s-k)
*	メビウス：	a[i] = (i が異なる d 個の素数の積 ? (-1)^d : 0)	A(s) = 1/ζ(s)
*	オイラー：	a[i] = i と互いに素な i 以下の正整数の個数			A(s) = ζ(s-1)/ζ(s)
*	平方数：		a[i] = (i が平方数 ? 1 : 0)						A(s) = ζ(2s)
*	無平方数：	a[i] = (i が平方因子をもたない ? 1 : 0)			A(s) = ζ(s)/ζ(2s)
*	GCD：		a[i] = GCD(n, i)  (n は定数)
*	平方剰余：	a[i] = (a/p)  （ルジャンドル記号，p は定数）
*/


//【ディリクレ畳込み（乗法的）】
/*
* Multiplicative_dirichlet_convolution<T>(int n) : O(n log(log n))
*	n 以下の素数を持って初期化する．
*
* vT conv(vT a, vT b) : O(n log(log n))
*	乗法的数論関数 a[1..n] と数論関数 b[1..n] のディリクレ畳込み c[1..n] を返す．
*
* vT inv_conv(vT a, vT c) : O(n log(log n))
*	乗法的数論関数 a[1..n] と数論関数 b のディリクレ畳込みが c[1..n] であるとして b[1..n] を返す．
*/
template <class T>
class Multiplicative_dirichlet_convolution {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	vi ps; // 素数のリスト

public:
	// n 以下の素数を持って初期化する．
	Multiplicative_dirichlet_convolution(int n) {
		// verify : https://atcoder.jp/contests/abc172/tasks/abc172_d

		// is_prime[i] : i が素数か
		vb is_prime(n + 1, true);
		is_prime[0] = is_prime[1] = false;

		int i = 2;

		// √n 以下の i の処理
		for (; i <= n / i; i++) if (is_prime[i]) {
			ps.push_back(i);
			for (int j = i * i; j <= n; j += i) is_prime[j] = false;
		}

		// √n より大きい i の処理
		for (; i <= n; i++) if (is_prime[i]) ps.push_back(i);
	}

	// 乗法的数論関数 a[1..n] と数論関数 b[1..n] のディリクレ畳込み c[1..n] を返す．
	vector<T> conv(const vector<T>& a, vector<T> b) {
		// verify : https://atcoder.jp/contests/abc172/tasks/abc172_d

		//【方法】
		// A(s) = A_p(s) の場合を考え，各 p について同じ処理を繰り返せば良い．C(s) は
		//		C(s)
		//		= A_p(s) B(s)
		//		= (Σ_d a[p^d] / (p^d)^s) (Σ_j b[j] / j^s)
		//		= Σ_d Σ_j a[p^d] b[j] / (p^d j)^s
		// と表されるので，更新式は
		//		c[p^d j] += a[p^d] b[j]
		// となる．これをインラインで行う．

		int n = sz(a) - 1;

		// インライン配る DP
		repe(p, ps) repir(j, n / p, 1) {
			for (ll i = p; i * j <= n; i *= p) b[i * j] += a[i] * b[j];
		}

		return b;
	}

	// 乗法的数論関数 a[1..n] と数論関数 b のディリクレ畳込みが c[1..n] であるとして b[1..n] を返す．
	vector<T> inv_conv(const vector<T>& a, vector<T> c) {
		// verify : https://atcoder.jp/contests/yahoo-procon2018-final/tasks/yahoo_procon2018_final_a

		//【方法】
		// A(s) = A_p(s) の場合を考え，各 p について同じ処理を繰り返せば良い．C(s) は
		//		C(s)
		//		= A_p(s) B(s)
		//		= (Σ_d a[p^d] / (p^d)^s) (Σ_j b[j] / j^s)
		//		= Σ_d Σ_j a[p^d] b[j] / (p^d j)^s
		// と表されるので，b[p^d j] について整理すると
		//		b[p^d j] = (c[p^d j] - Σ_(d>1) a[p^d] b[j]) / a[1]
		// となる．これをインラインで行う．

		int n = sz(a) - 1;

		// インライン配る DP
		repe(p, ps) repi(j, 1, n / p) {
			c[j] /= a[1];
			for (ll i = p; i * j <= n; i *= p) c[i * j] -= a[i] * c[j];
		}

		return c;
	}
};


//【ディリクレ畳込みの累積和（素朴）】O(n)
/*
* 数論関数 a[1..n] と b[1..n] のディリクレ畳込みを c とし，Σc[1..n] を返す．
*/
template <class T>
T naive_dirichlet_convolution_acc(const vector<T>& a, const vector<T>& b) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	//【方法】
	// a[1..n] の累積和を A[i] = Σa[1..i] などとおく．求めたい C[n] は
	//		C[n]
	//		= Σ_k∈[1..n] Σ_(i×j=k) a[i] b[j]
	//		= Σ_(i×j≦n) a[i] b[j]
	// と表される．i ≦ √n の範囲からの寄与は
	//		Σ_(i≦√n) Σ_(j≦n/i) a[i] b[j] = Σ_i≦√n a[i] B[n/i]
	// となる．i > √n の範囲からの寄与は，j ≦ √n であることに注意すると，
	//		Σ_(j≦√n) Σ_(√n<i≦n/j) a[i] b[j] = Σ_j≦√n b[j] (A[n/j] - A[√n])
	// となる．

	int n = sz(a) - 1, m = (int)(sqrt(n) + 0.01);

	// A[i] = Σa[1..i], B[i] = Σb[1..i]
	vector<T> A(n + 1), B(n + 1);

	// 累積和を求めるのに O(n) 時間かかってしまう．
	repi(i, 1, n) {
		A[i] = A[i - 1] + a[i];
		B[i] = B[i - 1] + b[i];
	}

	// 寄与の足し合わせは平方分割しているので O(√n) 時間で済む．
	T res = 0;
	repi(i, 1, m) res += a[i] * B[n / i];
	repi(j, 1, m) res += b[j] * (A[n / j] - A[m]);

	return res;
}


//【ディリクレ畳込みの累積和】O(√n)
/*
* 数論関数 a と b のディリクレ畳込みを c とし，Σc[1..n] を返す．
* m = √n（切り捨て）とし，al[1..m], bl[1..m], Ah[1..m], Bh[1..m] は以下の通りとする：
*	al[i] = a[i],		bl[j] = b[j]
*	Ah[i] = Σa[1..n/i],	Bh[j] = Σb[1..n/j]
*/
template <class T>
T dirichlet_convolution_acc(const vector<T>& al, const vector<T>& Ah, const vector<T>& bl, const vector<T>& Bh) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	//【方法】
	// a[1..n] の累積和を A[i] = Σa[1..i] などとおく．求めたい C[n] は
	//		C[n]
	//		= Σ_k∈[1..n] Σ_(i×j=k) a[i] b[j]
	//		= Σ_(i×j≦n) a[i] b[j]
	// と表される．i ≦ m の範囲からの寄与は
	//		Σ_(i≦m) Σ_(j≦n/i) a[i] b[j] = Σ_(i≦m) a[i] B[n/i]
	// となる．i > m の範囲からの寄与は，j ≦ m であることに注意すると，
	//		Σ_(j≦m) Σ_(m<i≦n/j) a[i] b[j] = Σ_(j≦m) b[j] (A[n/j] - A[m])
	// となる．

	int m = sz(al) - 1;

	// Am : Σa[1..m]
	T Am = accumulate(all(al), T(0));

	T Cn = 0;
	repi(i, 1, m) Cn += al[i] * Bh[i];
	repi(j, 1, m) Cn += bl[j] * (Ah[j] - Am);

	return Cn;
}


//【ディリクレ畳込みの累積和（一括）】O(nl log nl + √(n nh))
/*
* 数論関数 a と b のディリクレ畳込みを c とし，以下で定まる cl, Ch を格納する．
* nl, nh は nh ≦ nl ≦ n ≦ nl nh を満たすとし，al, bl, cl, Ah, Bh, Ch は以下の通りとする：
*	al[i] = a[i]		(i∈[1..nl]),  bl, cl も同様
*	Ah[i] = Σa[1..n/i]	(i∈[1..nh]),  Bh, Ch も同様
*
* 特に nl = (n / log n)^(2/3) と選ぶと全体の計算量は O(n^(2/3) (log n)^(1/3)) になる．
*/
template <class T>
void dirichlet_convolution_acc(ll n, const vector<T>& al, const vector<T>& Ah,
	const vector<T>& bl, const vector<T>& Bh, vector<T>& cl, vector<T>& Ch)
{
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	int nl = sz(al) - 1, nh = sz(Ah) - 1;
	cl.assign(nl + 1, 0), Ch.assign(nh + 1, 0);

	// cl[1..nl] を素朴に計算する．
	repi(i, 1, nl) for (ll j = 1; i * j <= nl; j++) cl[i * j] += al[i] * bl[j];

	// Al[i] = Σa[1..i], Bl[i] = Σb[1..i]
	vector<T> Al(nl + 1), Bl(nl + 1);
	repi(i, 1, nl) {
		Al[i] = Al[i - 1] + al[i];
		Bl[i] = Bl[i - 1] + bl[i];
	}

	auto get_Ah = [&](ll i) { return i <= nh ? Ah[i] : Al[n / i]; };
	auto get_Bh = [&](ll i) { return i <= nh ? Bh[i] : Bl[n / i]; };

	// 各 Ch[k] を平方分割で計算する．
	repi(k, 1, nh) {
		int m = (int)(sqrt(n / k) + 1e-12);
		repi(i, 1, m) Ch[k] += al[i] * get_Bh((ll)k * i);
		repi(j, 1, m) Ch[k] += bl[j] * (get_Ah((ll)k * j) - Al[m]);
	}
}


//【ディリクレ逆畳込みの累積和（一括）】O(nl log nl + √(n nh))
/*
* 数論関数 a と b のディリクレ畳込みを c とし，以下で定まる bl, Bh を格納する．
* nl, nh は nh ≦ nl ≦ n ≦ nl nh を満たすとし，al, bl, cl, Ah, Bh, Ch は以下の通りとする：
*	al[i] = a[i]		(i∈[1..nl]),  bl, cl も同様
*	Ah[i] = Σa[1..n/i]	(i∈[1..nh]),  Bh, Ch も同様
*
* 特に nl = (n / log n)^(2/3) と選ぶと全体の計算量は O(n^(2/3) (log n)^(1/3)) になる．
*
* 制約：a[1] ≠ 0
*/
template <class T>
void dirichlet_invconvolution_acc(ll n, const vector<T>& al, const vector<T>& Ah,
	const vector<T>& cl, const vector<T>& Ch, vector<T>& bl, vector<T>& Bh)
{
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	Assert(al[1] != 0);
	int nl = sz(al) - 1, nh = sz(Ah) - 1;
	bl = cl, Bh = Ch;

	// bl[1..nl] を素朴に計算する（a が乗法的なら高速化できる）
	repi(j, 1, nl) {
		bl[j] /= al[1];
		for (ll i = 2; i * j <= nl; i++) bl[i * j] -= al[i] * bl[j];
	}

	// Al[i] = Σa[1..i], Bl[i] = Σb[1..i]
	vector<T> Al(nl + 1), Bl(nl + 1);
	repi(i, 1, nl) {
		Al[i] = Al[i - 1] + al[i];
		Bl[i] = Bl[i - 1] + bl[i];
	}

	auto get_Ah = [&](ll i) { return i <= nh ? Ah[i] : Al[n / i]; };
	auto get_Bh = [&](ll i) { return i <= nh ? Bh[i] : Bl[n / i]; };

	// 各 Bh[k] を平方分割で計算する．
	repir(k, nh, 1) {
		int m = (int)(sqrt(n / k) + 1e-12);
		repi(i, 2, m) Bh[k] -= al[i] * get_Bh((ll)k * i);
		repi(j, 1, m) Bh[k] -= bl[j] * (get_Ah((ll)k * j) - Al[m]);
		Bh[k] /= al[1];
	}
}


//【ディリクレ畳込みの累積和（乗法的，一括）】
/*
* Multiplicative_dirichlet_invconvolution_acc<T>(int p_max) : O(p_max log(log p_max))
*	p_max ≧ nl 以下の素数を持って初期化する．
*	乗法的数論関数 a[1..n] と数論関数 b[1..n] のディリクレ畳込みを c[1..n] とする．
*	nl, nh は nh ≦ nl ≦ n ≦ nl nh を満たすとし，al, bl, cl, Ah, Bh, Ch は以下の通りとする：
*		al[i] = a[i]		(i∈[1..nl]),  bl, cl も同様
*		Ah[i] = Σa[1..n/i]	(i∈[1..nh]),  Bh, Ch も同様
*
* conv_acc(ll n, vT al, vT Ah, vT bl, vT Bh, vT& cl, vT& Ch) : O(nl log(log nl) + √(n nh))
*	上記 al, Ah, bl, Bh をもとに cl, Ch を計算し格納する．
*
* inv_conv_acc(ll n, vT al, vT Ah, vT cl, vT Ch, vT& bl, vT& Bh) : O(nl log(log nl) + √(n nh))
*	上記 al, Ah, cl, Ch をもとに bl, Bh を計算し格納する．
*
* 特に nl = (n / log(log n))^(2/3) と選ぶと全体の計算量は O(n^(2/3) (log(log n))^(1/3)) になる．
*/
template <class T>
class Multiplicative_dirichlet_invconvolution_acc {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	int p_max;
	vi ps; // 素数のリスト

public:
	// nl 以下の素数を持って初期化する．
	Multiplicative_dirichlet_invconvolution_acc(int p_max) : p_max(p_max) {
		// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

		// is_prime[i] : i が素数か
		vb is_prime(p_max + 1, true);
		is_prime[0] = is_prime[1] = false;
		int i = 2;

		// √p_max 以下の i の処理
		for (; i <= p_max / i; i++) if (is_prime[i]) {
			ps.push_back(i);
			for (int j = i * i; j <= p_max; j += i) is_prime[j] = false;
		}

		// √p_max より大きい i の処理
		for (; i <= p_max; i++) if (is_prime[i]) ps.push_back(i);
	}

	// al, Ah, bl, Bh をもとに cl, Ch を計算し格納する．
	void conv_acc(ll n, const vector<T>& al, const vector<T>& Ah,
		const vector<T>& bl, const vector<T>& Bh, vector<T>& cl, vector<T>& Ch)
	{
		int nl = sz(al) - 1, nh = sz(Ah) - 1;
		Assert(nl <= p_max); Assert(nh <= nl); Assert(nl <= n); Assert(n <= (ll)nl * nh);

		cl = bl, Ch.assign(nh + 1, 0);

		// cl[1..nl] を計算する．
		repe(p, ps) repir(j, nl / p, 1) {
			for (ll i = p; i * j <= nl; i *= p) cl[i * j] += al[i] * cl[j];
		}

		// Al[i] = Σa[1..i], Bl[i] = Σb[1..i]
		vector<T> Al(nl + 1), Bl(nl + 1);
		repi(i, 1, nl) {
			Al[i] = Al[i - 1] + al[i];
			Bl[i] = Bl[i - 1] + bl[i];
		}

		auto get_Ah = [&](ll i) { return i <= nh ? Ah[i] : Al[n / i]; };
		auto get_Bh = [&](ll i) { return i <= nh ? Bh[i] : Bl[n / i]; };

		// 各 Ch[k] を平方分割で計算する．
		repi(k, 1, nh) {
			int m = (int)(sqrt(n / k) + 1e-12);
			repi(i, 1, m) Ch[k] += al[i] * get_Bh((ll)k * i);
			repi(j, 1, m) Ch[k] += bl[j] * (get_Ah((ll)k * j) - Al[m]);
		}
	}

	// al, Ah, cl, Ch をもとに bl, Bh を計算し格納する．
	void inv_conv_acc(ll n, const vector<T>& al, const vector<T>& Ah,
		const vector<T>& cl, const vector<T>& Ch, vector<T>& bl, vector<T>& Bh)
	{
		// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

		Assert(al[1] != 0);
		int nl = sz(al) - 1, nh = sz(Ah) - 1;
		Assert(nl <= p_max); Assert(nh <= nl); Assert(nl <= n); Assert(n <= (ll)nl * nh);
		bl = cl, Bh = Ch;

		// bl[1..nl] を計算する．
		repe(p, ps) repi(j, 1, nl / p) {
			bl[j] /= al[1];
			for (ll i = p; i * j <= nl; i *= p) bl[i * j] -= al[i] * bl[j];
		}

		// Al[i] = Σa[1..i], Bl[i] = Σb[1..i]
		vector<T> Al(nl + 1), Bl(nl + 1);
		repi(i, 1, nl) {
			Al[i] = Al[i - 1] + al[i];
			Bl[i] = Bl[i - 1] + bl[i];
		}

		auto get_Ah = [&](ll i) { return i <= nh ? Ah[i] : Al[n / i]; };
		auto get_Bh = [&](ll i) { return i <= nh ? Bh[i] : Bl[n / i]; };

		// 各 Bh[k] を平方分割で計算する．
		repir(k, nh, 1) {
			int m = (int)(sqrt(n / k) + 1e-12);
			repi(i, 2, m) Bh[k] -= al[i] * get_Bh((ll)k * i);
			repi(j, 1, m) Bh[k] -= bl[j] * (get_Ah((ll)k * j) - Al[m]);
			Bh[k] /= al[1];
		}
	}
};


//【ディリクレ畳込みの累積和（乗法的，mint，一括）】
/*
* Multiplicative_dirichlet_invconvolution_acc_mint(int p_max) : O(p_max log(log p_max))
*	p_max ≧ nl 以下の素数を持って初期化する．
*	乗法的数論関数 a[1..n] と数論関数 b[1..n] のディリクレ畳込みを c[1..n] とする．
*	nl, nh は nh ≦ nl ≦ n ≦ nl nh を満たすとし，al, bl, cl, Ah, Bh, Ch は以下の通りとする：
*		al[i] = a[i]		(i∈[1..nl]),  bl, cl も同様
*		Ah[i] = Σa[1..n/i]	(i∈[1..nh]),  Bh, Ch も同様
*
* conv_acc(ll n, vm al, vm Ah, vm bl, vm Bh, vm& cl, vm& Ch) : O(nl log(log nl) + √(n nh))
*	上記 al, Ah, bl, Bh をもとに cl, Ch を計算し格納する．
*
* inv_conv_acc(ll n, vm al, vm Ah, vm cl, vm Ch, vm& bl, vm& Bh) : O(nl log(log nl) + √(n nh))
*	上記 al, Ah, cl, Ch をもとに bl, Bh を計算し格納する．
*
* 特に nl = (n / log(log n))^(2/3) と選ぶと全体の計算量は O(n^(2/3) (log(log n))^(1/3)) になる．
*/
class Multiplicative_dirichlet_invconvolution_acc_mint {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	int p_max;
	vi ps; // 素数のリスト

public:
	// nl 以下の素数を持って初期化する．
	Multiplicative_dirichlet_invconvolution_acc_mint(int p_max) : p_max(p_max) {
		// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

		// is_prime[i] : i が素数か
		vb is_prime(p_max + 1, true);
		is_prime[0] = is_prime[1] = false;
		int i = 2;

		// √p_max 以下の i の処理
		for (; i <= p_max / i; i++) if (is_prime[i]) {
			ps.push_back(i);
			for (int j = i * i; j <= p_max; j += i) is_prime[j] = false;
		}

		// √p_max より大きい i の処理
		for (; i <= p_max; i++) if (is_prime[i]) ps.push_back(i);
	}

	// al, Ah, bl, Bh をもとに cl, Ch を計算し格納する．
	void conv_acc(ll n, const vm& al, const vm& Ah, const vm& bl, const vm& Bh, vm& cl, vm& Ch) {
		int nl = sz(al) - 1, nh = sz(Ah) - 1;
		Assert(nl <= p_max); Assert(nh <= nl); Assert(nl <= n); Assert(n <= (ll)nl * nh);

		cl = bl, Ch.assign(nh + 1, 0);

		// cl[1..nl] を計算する．
		repe(p, ps) repir(j, nl / p, 1) {
			for (ll i = p; i * j <= nl; i *= p) cl[i * j] += al[i] * cl[j];
		}

		// Al[i] = Σa[1..i], Bl[i] = Σb[1..i]
		vm Al(nl + 1), Bl(nl + 1);
		repi(i, 1, nl) {
			Al[i] = Al[i - 1] + al[i];
			Bl[i] = Bl[i - 1] + bl[i];
		}

		auto get_Ah = [&](ll i) { return i <= nh ? Ah[i] : Al[n / i]; };
		auto get_Bh = [&](ll i) { return i <= nh ? Bh[i] : Bl[n / i]; };

		// 各 Ch[k] を平方分割で計算する．
		repi(k, 1, nh) {
			int m = (int)(sqrt(n / k) + 1e-12);
			repi(i, 1, m) Ch[k] += al[i] * get_Bh((ll)k * i);
			repi(j, 1, m) Ch[k] += bl[j] * (get_Ah((ll)k * j) - Al[m]);
		}
	}

	// al, Ah, cl, Ch をもとに bl, Bh を計算し格納する．
	void inv_conv_acc(ll n, const vm& al, const vm& Ah, const vm& cl, const vm& Ch, vm& bl, vm& Bh) {
		// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

		Assert(al[1] != 0);
		mint a1_inv = al[1].inv();

		int nl = sz(al) - 1, nh = sz(Ah) - 1;
		Assert(nl <= p_max); Assert(nh <= nl); Assert(nl <= n); Assert(n <= (ll)nl * nh);
		bl = cl, Bh = Ch;

		// bl[1..nl] を計算する．
		repe(p, ps) repi(j, 1, nl / p) {
			bl[j] *= a1_inv;
			for (ll i = p; i * j <= nl; i *= p) bl[i * j] -= al[i] * bl[j];
		}

		// Al[i] = Σa[1..i], Bl[i] = Σb[1..i]
		vm Al(nl + 1), Bl(nl + 1);
		repi(i, 1, nl) {
			Al[i] = Al[i - 1] + al[i];
			Bl[i] = Bl[i - 1] + bl[i];
		}

		auto get_Ah = [&](ll i) { return i <= nh ? Ah[i] : Al[n / i]; };
		auto get_Bh = [&](ll i) { return i <= nh ? Bh[i] : Bl[n / i]; };

		// 各 Bh[k] を平方分割で計算する．
		repir(k, nh, 1) {
			int m = (int)(sqrt(n / k) + 1e-12);
			repi(i, 2, m) Bh[k] -= al[i] * get_Bh((ll)k * i);
			repi(j, 1, m) Bh[k] -= bl[j] * (get_Ah((ll)k * j) - Al[m]);
			Bh[k] *= a1_inv;
		}
	}
};


