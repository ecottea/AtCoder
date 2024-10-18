#pragma once
#include "header.h"
#include "構造(数論).h"
// ■■■■■ 添字積畳込み（ディリクレ積） ■■■■■


//【数論関数】
/*
* 自然数の集合 N を定義域とする関数 a を数論関数という．vector<T> a の a[0] を無視することで表す．
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
	repi(i, 1, n) {
		int j_max = n / i;
		repi(j, 1, j_max) c[i * j] += a[i] * b[j];
	}

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
	// 除原理を用いる．ディリクレ畳込みの式の総和を i=1 とその他に分解すると
	//		c[k] = a[1] b[k] + Σ_(i×j=k,i>1) a[i] b[j]
	// となる．これを b[k] について解くと
	//		b[k] = (c[k] - Σ_(i×j=k,i>1) a[i] b[j]) / a[1]
	// となる．

	Assert(a[1] != 0);
	int n = sz(a) - 1;

	// 配る DP
	vector<T> b(c);
	repi(j, 1, n) {
		b[j] /= a[1]; // mint だと遅いので注意

		int i_max = n / j;
		repi(i, 2, i_max) b[i * j] -= a[i] * b[j];
	}

	return b;
}


//【ディリクレ畳込みと約数ゼータ変換】
/*
* 約数ゼータ変換
*		A[n] = Σd|n a[d]
* は，
*		A[n] = Σ_d×j=n a[d] 1
* と書き直すことにより 1 とのディリクレ畳込みであると解釈できる．
*/


//【ディリクレ母関数とディリクレ畳込み】
/*
* 数論関数 a[1..n] のディリクレ母関数を A(s) = Σi a[i] / i^s などとおくと，
* 数論関数 a, b のディリクレ畳込み c に対応するディリクレ母関数は C(s) = A(s) B(s) となる．
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
*		A(s) = Πp A_p(s)
*		A_p(s) := Σd a[p^d] / (p^d)^s
* なるオイラー積表示をもつ．z = p^(-s) とおくと
*		A_p(z) = Σd∈[0..∞) a[p^d] z^d
* と表され，素因数ごとに FPS のように扱うことができる．
*/


//【乗法的数論関数の例】
/*
* 乗法的数論関数 a[i] とそのディリクレ母関数 A(s) の例として以下のものが挙げられる：
*	D畳込み：	a[i] = Σj×k=i b[j] c[k] （b, c は乗法的数論関数）	A(s) = B(s) C(s)	A_p(z) = B_p(z) C_p(z)
*	積：			a[i] = b[i] c[i] （b, c は乗法的数論関数）
*	逆数：		a[i] = 1/b[i] （b は乗法的数論関数）
*	デルタ：		a[i] = (i = 1 ? 1 : 0)							A(s) = 1			A_p(z) = 1
*	定数 1：		a[i] = 1										A(s) = ζ(s)			A_p(z) = 1/(1-z)
*	恒等：		a[i] = i										A(s) = ζ(s-1)		A_p(z) = 1/(1-pz)
*	累乗：		a[i] = i^k  (k は定数)							A(s) = ζ(s-k)		A_p(z) = 1/(1-p^k z)
*	約数和：		a[i] = i の約数の k 乗和  （k は定数）			A(s) = ζ(s)ζ(s-k)	A_p(z) = 1/(1-z)(1-p^k z)
*	メビウス：	a[i] = (i が異なる d 個の素数の積 ? (-1)^d : 0)	A(s) = 1/ζ(s)		A_p(z) = 1-z
*	オイラー：	a[i] = i と互いに素な i 以下の正整数の個数			A(s) = ζ(s-1)/ζ(s)	A_p(z) = (1-z)/(1-pz)
*	平方数：		a[i] = (i が平方数 ? 1 : 0)						A(s) = ζ(2s)		A_p(z) = 1/(1-z^2)
*	無平方数：	a[i] = (i が平方因子をもたない ? 1 : 0)			A(s) = ζ(s)/ζ(2s)	A_p(z) = 1+z
*	GCD：		a[i] = GCD(n, i)  (n は定数)	
*	LCM：		a[i] = LCM(n, i)  (n は定数)	
*	平方剰余：	a[i] = (i/p)  （ルジャンドル記号，p は定数）
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
		int i_max = n / i;
		for (; i <= i_max; i++) if (is_prime[i]) {
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
			ll i_max = n / j;
			for (ll i = p; i <= i_max; i *= p) b[i * j] += a[i] * b[j];
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
		//		b[p^d j]
		//		= (c[p^d j] - Σ_(d>1) a[p^d] b[j]) / a[1]
		//		= c[p^d j] - Σ_(d>1) a[p^d] b[j]  (a : 乗法的より a[1] = 1)
		// となる．これをインラインで行う．

		int n = sz(a) - 1;

		// インライン配る DP
		repe(p, ps) repi(j, 1, n / p) {
			ll i_max = n / j;
			for (ll i = p; i <= i_max; i *= p) c[i * j] -= a[i] * c[j];
		}

		return c;
	}
};


//【ディリクレ畳込みの総和（素朴）】O(n)
/*
* 数論関数 a[1..n] と b[1..n] のディリクレ畳込みを c とし，Σc[1..n] を返す．
*/
template <class T>
T naive_dirichlet_convolution_sum(const vector<T>& a, const vector<T>& b) {
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


//【ディリクレ畳込みの総和】O(√N)
/*
* 数論関数 a[1..N] と b[1..N] のディリクレ畳込みを c[1..N] とする．
* 与えられた a, b の累積和 A, B に対し，Σc[1..N] を返す．
*
* 制約：nl = (int)(sqrt(N) + 1e-9)
*/
template <class T>
T dirichlet_convolution_sum(const Floor_vector<T>& A, const Floor_vector<T>& B) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c
	// verify : https://projecteuler.net/problem=401

	//【方法】
	// m=√N，a[1..N] の累積和を A[i] = Σa[1..i] などとおく．求めたい C[N] は
	//		C[N]
	//		= Σ_k∈[1..N] Σ_(i×j=k) a[i] b[j]
	//		= Σ_(i×j≦N) a[i] b[j]
	// と表される．i ≦ m の範囲からの寄与は
	//		Σ_(i≦m) Σ_(j≦N/i) a[i] b[j] = Σ_(i≦m) a[i] B[N/i]
	// となる．i > m の範囲からの寄与は，j ≦ m であることに注意すると，
	//		Σ_(j≦m) Σ_(m<i≦N/j) a[i] b[j] = Σ_(j≦m) b[j] (A[N/j] - A[m])
	// となる．

	int nl = A.nl;

	T c_sum = 0;

	// i ≦ √N からの寄与を加える．
	c_sum += A.get_l(1) * B.get_h(1);
	repi(i, 2, nl) c_sum += (A.get_l(i) - A.get_l(i - 1)) * B.get_h(i);

	// i > √N からの寄与を加える．
	c_sum += B.get_l(1) * (A.get_h(1) - A.get_l(nl));
	repi(j, 2, nl) c_sum += (B.get_l(j) - B.get_l(j - 1)) * (A.get_h(j) - A.get_l(nl));

	return c_sum;
}


//【ディリクレ畳込みの累積和（一括）】O(n^(2/3) (log n)^(1/3))
/*
* 数論関数 a[1..N] と b[1..N] のディリクレ畳込みを c[1..N] とする．
* 与えられた a, b の累積和 A, B に対し，c の累積和 C を返す．
*
* 制約：nl = (N <= 2 ? 1 : (int)max(pow(N / log(N), 2. / 3), sqrt(N) + 1e-9))
*/
template <class T>
Floor_vector<T> dirichlet_convolution_acc(const Floor_vector<T>& A, const Floor_vector<T>& B) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	ll N = A.N; int nl = A.nl, nh = A.nh;
	Floor_vector<T> C(N, nl);

	// a[1..nl], b[1..nl] を計算する．
	vector<T> al(nl + 1), bl(nl + 1);
	repi(i, 1, nl) {
		al[i] = A.get_l(i);
		bl[i] = B.get_l(i);
	}
	repir(i, nl, 1) {
		al[i] -= al[i - 1];
		bl[i] -= bl[i - 1];
	}

	// c[1..nl] を素朴に計算する．
	vector<T> cl(nl + 1);
	repi(i, 1, nl) {
		int j_max = nl / i;
		repi(j, 1, j_max) cl[i * j] += al[i] * bl[j];
	}

	// c[1..nl] の累積和を素朴に計算する．
	repi(i, 1, nl) {
		cl[i] += cl[i - 1];
		C.set_l(i, cl[i]);
	}

	// 各 C[N/d] を平方分割で計算する．
	repi(d, 1, nh) {
		T Cd = 0;

		int mid = (int)(sqrt(N / d) + 1e-12);
		repi(i, 1, mid) Cd += al[i] * B.get_h(d * i);
		repi(j, 1, mid) Cd += bl[j] * (A.get_h(d * j) - A.get_l(mid));

		C.set_h(d, Cd);
	}

	return C;
}


//【ディリクレ逆畳込みの累積和（一括）】O(n^(2/3) (log n)^(1/3))
/*
* 数論関数 a[1..N] と b[1..N] のディリクレ畳込みを c[1..N] とする．
* 与えられた a, c の累積和 A, C に対し，b の累積和 B を返す．
*
* 制約：a[1] != 0, nl = (N <= 2 ? 1 : (int)max(pow(N / log(N), 2. / 3), sqrt(N) + 1e-9))
*/
template <class T>
Floor_vector<T> dirichlet_invconvolution_acc(const Floor_vector<T>& A, const Floor_vector<T>& C) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	Assert(A.get_l(1) != 0);

	ll N = A.N; int nl = A.nl, nh = A.nh;
	Floor_vector<T> B(N, nl);

	// a[1..nl], c[1..nl] を計算する．
	vector<T> al(nl + 1), cl(nl + 1);
	repi(i, 1, nl) {
		al[i] = A.get_l(i);
		cl[i] = C.get_l(i);
	}
	repir(i, nl, 1) {
		al[i] -= al[i - 1];
		cl[i] -= cl[i - 1];
	}

	// b[1..nl] を素朴に計算する
	vector<T> bl(cl);
	repi(j, 1, nl) {
		bl[j] /= al[1]; // mint だと遅くなるので注意

		int i_max = nl / j;
		repi(i, 2, i_max) bl[i * j] -= al[i] * bl[j];
	}

	// b[1..nl] の累積和を素朴に計算する．
	vector<T> Bl(nl + 1);
	repi(i, 1, nl) {
		Bl[i] = Bl[i - 1] + bl[i];
		B.set_l(i, Bl[i]);
	}

	// 各 B[N/d] を平方分割で計算する．
	repir(d, nh, 1) {
		T Bd = C.get_h(d);

		int mid = (int)(sqrt(N / d) + 1e-12);
		repi(i, 2, mid) Bd -= al[i] * B.get_h(d * i);
		repi(j, 1, mid) Bd -= bl[j] * (A.get_h(d * j) - A.get_l(mid));
		Bd /= al[1]; // mint だと遅くなるので注意

		B.set_h(d, Bd);
	}

	return B;
}


//【ディリクレ畳込みの累積和（乗法的，一括）】
/*
* Multiplicative_dirichlet_convolution_acc<T>(ll N) : O(N^(2/3))
*	[1..N] 上の数論関数を扱えるよう初期化する．
*
* fvT conv_acc(fvT A, fvT B) : O(n^(2/3) (log(log n))^(1/3))
*	乗法的数論関数 a[1..N] と数論関数 b[1..N] のディリクレ畳込みを c[1..N] とする．
*	与えられた a, b の累積和 A, B に対し，c の累積和 C を返す．
*
* fvT inv_conv_acc(fvT A, fvT C) : O(n^(2/3) (log(log n))^(1/3))
*	乗法的数論関数 a[1..N] と数論関数 b[1..N] のディリクレ畳込みを c[1..N] とする．
*	与えられた a, c の累積和 A, C に対し，b の累積和 B を返す．
*
* 制約：nl = (int)max(N <= 30 ? 1. : pow(N / log(log(N)), 2. / 3), sqrt(N) + 1e-9)
* 
* 利用：【添字整数商 vector】
*/
template <class T>
class Multiplicative_dirichlet_convolution_acc {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	vi ps; // 素数の昇順リスト

public:
	// [1..N] 上の数論関数を扱えるよう初期化する．
	Multiplicative_dirichlet_convolution_acc(ll N) {
		// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

		int nl_max = (int)max(N <= 30 ? 1. : pow(N / log(log(N)), 2. / 3), sqrt(N) + 1e-9);

		// is_prime[i] : i が素数か
		vb is_prime(nl_max + 1, true);
		is_prime[0] = is_prime[1] = false;

		int i = 2;

		// nl_max 以下の i の処理
		for (; i * i <= nl_max; i++) if (is_prime[i]) {
			ps.push_back(i);
			for (int j = i * i; j <= nl_max; j += i) is_prime[j] = false;
		}

		// nl_max より大きい i の処理
		for (; i <= nl_max; i++) if (is_prime[i]) ps.push_back(i);
	}

	// 与えられた a, b の累積和 A, B に対し，c の累積和 C を返す．
	Floor_vector<T> conv_acc(const Floor_vector<T>& A, const Floor_vector<T>& B) {
		ll N = A.N; int nl = A.nl, nh = A.nh;
		Floor_vector<T> C(N, nl);

		// a[1..nl], b[1..nl] を計算する．
		vector<T> al(nl + 1), bl(nl + 1);
		repi(i, 1, nl) {
			al[i] = A.get_l(i);
			bl[i] = B.get_l(i);
		}
		repir(i, nl, 1) {
			al[i] -= al[i - 1];
			bl[i] -= bl[i - 1];
		}

		// c[1..nl] をインライン配る DP で計算する．
		vector<T> cl(bl);
		repe(p, ps) {
			int j_max = nl / p;
			if (j_max == 0) break;

			repir(j, j_max, 1) {
				ll i_max = nl / j;
				for (ll i = p; i <= i_max; i *= p) cl[i * j] += al[i] * cl[j];
			}
		}

		// c[1..nl] の累積和を素朴に計算する．
		repi(i, 1, nl) {
			cl[i] += cl[i - 1];
			C.set_l(i, cl[i]);
		}

		// 各 C[N/d] を平方分割で計算する．
		repi(d, 1, nh) {
			T Cd = 0;

			int mid = (int)(sqrt(N / d) + 1e-12);
			repi(i, 1, mid) Cd += al[i] * B.get_h(d * i);
			repi(j, 1, mid) Cd += bl[j] * (A.get_h(d * j) - A.get_l(mid));

			C.set_h(d, Cd);
		}

		return C;
	}

	// 与えられた a, c の累積和 A, C に対し，b の累積和 B を返す．
	Floor_vector<T> inv_conv_acc(const Floor_vector<T>& A, const Floor_vector<T>& C) {
		// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

		ll N = A.N; int nl = A.nl, nh = A.nh;
		Floor_vector<T> B(N, nl);

		// a[1..nl], c[1..nl] を計算する．
		vector<T> al(nl + 1), cl(nl + 1);
		repi(i, 1, nl) {
			al[i] = A.get_l(i);
			cl[i] = C.get_l(i);
		}
		repir(i, nl, 1) {
			al[i] -= al[i - 1];
			cl[i] -= cl[i - 1];
		}

		// b[1..nl] をインライン配る DP で計算する．
		vector<T> bl(cl);
		repe(p, ps) {
			int j_max = nl / p;
			if (j_max == 0) break;

			repi(j, 1, j_max) {
				ll i_max = nl / j;
				for (ll i = p; i <= i_max; i *= p) bl[i * j] -= al[i] * bl[j];
			}
		}

		// b[1..nl] の累積和を素朴に計算する．
		vector<T> Bl(nl + 1);
		repi(i, 1, nl) {
			Bl[i] = Bl[i - 1] + bl[i];
			B.set_l(i, Bl[i]);
		}

		// 各 B[N/d] を平方分割で計算する．
		repir(d, nh, 1) {
			T Bd = C.get_h(d);

			int mid = (int)(sqrt(N / d) + 1e-12);
			repi(i, 2, mid) Bd -= al[i] * B.get_h(d * i);
			repi(j, 1, mid) Bd -= bl[j] * (A.get_h(d * j) - A.get_l(mid));

			B.set_h(d, Bd);
		}

		return B;
	}
};
