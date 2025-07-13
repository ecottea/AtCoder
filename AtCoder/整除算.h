#pragma once
#include "header.h"
// ■■■■■ 整数除算（切り捨て，切り上げ，剰余） ■■■■■


//【剰余 と 切り捨て商 の関係】
/*
* a mod m = a - floor(a / m) m
*
* verify : https://atcoder.jp/contests/abc402/tasks/abc402_g
*/


//【剰余の範囲内判定 → 切り捨て商の差】
/*
* 0 ≦ l ≦ r ≦ m のとき，以下の等式が成り立つ：
*	Boole[l ≦ (a mod m) < r] = floor((a-l)/m) - floor((a-r)/m)
* 
* (証明)
* 左辺の不等式を同値変形していくと，
*		l ≦ a mod m < r
*		⇔ l ≦ a - floor(a / m) m < r
*		⇔ (a-l)/m ≧ floor(a/m) > (a-r)/m
* となる．
* 
* これが成り立つとき，floor(a/m) が整数であることと
*		(a-l)/m - (a-r)/m = (r-l)/m ≦ (m-0)/m = 1
* であることより元の等式の右辺は 1 となる．
* 
* 成り立たないとき，
*		(a-l)/m < floor(a/m)
* であれば r ≦ m と合わせて
*		floor(a/m) > (a-l)/m > (a-r)/m ≧ a/m-1 ≧ floor(a/m)-1
* なので元の等式の右辺は 0 となり，
*		floor(a/m) ≦ (a-r)/m
* であれば 0 ≦ l と合わせて
*		floor(a/m)+1 > a/m ≧ (a-l)/m ≧ (a-r)/m ≧ floor(a/m)
* なので元の等式の右辺は 0 となる．
* 
* verify : https://yukicoder.me/problems/no/2280
*/


//【剰余の大小判定 → 整数商の差】
/*
* 以下の等式が成り立つ：
*	Boole[(a mod m) < (b mod m)] = ceil((b-a)/m) - floor(b/m) + floor(a/m)
* 
* verify : https://yukicoder.me/problems/no/3054
*/


//【切り捨て商と切り上げ商の関係】
/*
* ceil(a / b) = floor((a - sgn(b)) / b) + 1
* ceil(a / b) = -floor(-a / b)
* 
* verify : https://yukicoder.me/problems/no/3054
*/


//【商列挙】O(√N)
/*
* 区間 [1..N] を N/i = q（切り捨て）となる半開区間 i∈(il..ir] に分割し，
* i について昇順にそれぞれに対して f(il, ir, q) を呼び出す．
* なお各範囲においては N mod i は公差 -q の等差数列を成す．
*/
template <class T, class FUNC>
void quotient_range(T N, const FUNC& f) {
	// 参考 : https://ei1333.github.io/luzhiled/snippets/math/quotient-range.html
	// verify : https://judge.yosupo.jp/problem/enumerate_quotients

	//【方法】
	// N/i の商が q となるような i の範囲を考える．条件を i について整理すると
	//		q = floor(N/i)
	//		⇔ q ≦ N/i < q+1
	//		⇔ i q ≦ N < i(q+1)
	//		⇔ N/(q+1) < i ≦ N/q  (⇔ floor(N/(q+1)) < i ≦ floor(N/q))
	// となる．
	//
	// この幅が 1 以下であれば，q に対応する i は高々 1 個である．その条件は
	//		N/q - N/(q+1) ≦ 1
	//		⇔ (q+1)N - q N ≦ q(q+1)
	//		⇔ N ≦ q(q+1)
	// である．条件をやや弱めて
	//		N ≦ q^2 ⇔ √N ≦ q
	// としてもオーダーに影響はない．

	//（例）
	// 例えば N = 15 のときは (0..15] を以下のように分割できる：
	//		i の範囲		q=N/i	N mod i
	//		(0..1]		15		[0]
	//		(1..2]		7		[1]
	//		(2..3]		5		[0]
	//		(3..5]		3		[3, 0]
	//		(5..7]		2		[3, 1]
	//		(7..15]		1		[7, 6, 5, 4, 3, 2, 1, 0]

	T sqrt_n = (T)(sqrt(N) - 1e-9);

	// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
	T i_max = N / (sqrt_n + 1);
	for (T i = 1; i <= i_max; ++i) f(i - 1, i, N / i);

	// そうでない部分は q ごとにまとめて考える．
	T il, ir = i_max;
	for (T q = sqrt_n; q >= 1; --q) {
		il = ir;
		ir = N / q;
		f(il, ir, q);
	}

	/* f の定義の雛形
	using T = ll;
	auto f = [&](T il, T ir, T q) {

	};
	quotient_range(N, f);
	*/
}


//【商列挙（組）】O(√max(n1,n2))
/*
* 区間 [1..max(n1,n2)] を (n1/i, n2/i) = (q1, q2)（切り捨て）となる半開区間 i∈(il..ir] に分割し，
* i について昇順にそれぞれに対して f(il, ir, q1, q2) を呼び出す．
* なお各範囲においては (n1 mod i, n2 mod i) は公差 (-q1, -q2) の等差数列を成す．
*/
template <class T, class FUNC>
void quotient_range(T n1, T n2, const FUNC& f) {
	// verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_i

	T n = max(n1, n2);

	T sqrt_n = (T)(sqrt(n) - 1e-12);

	// どちらかの q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
	T i_max = n / (sqrt_n + 1);
	for (T i = 1; i <= i_max; ++i) f(i - 1, i, n1 / i, n2 / i);

	// そうでない部分は (q1, q2) ごとにまとめて考える．
	T q1 = n1 / (i_max + 1), q2 = n2 / (i_max + 1);
	while (q1 > 0 || q2 > 0) {
		// (il1..ir1] : n1/i = q1 となる i の範囲
		T il1 = n1 / (q1 + 1), ir1 = (q1 > 0 ? n1 / q1 : (T)INFL);

		// (il2..ir2] : n2/i = q2 となる i の範囲
		T il2 = n2 / (q2 + 1), ir2 = (q2 > 0 ? n2 / q2 : (T)INFL);

		// 両区間の共通部分を求める．
		T il = max(il1, il2), ir = min(ir1, ir2);
		if (il < ir) f(il, ir, q1, q2);

		if (ir1 < ir2) q1--;
		else q2--;
	}

	/* f の定義の雛形
	using T = ll;
	auto f = [&](T il, T ir, T q1, T q2) {

	};
	quotient_range(n1, n2, f);
	*/
}


//【商列挙（平方）】O(N^(1/3))
/*
* 区間 [1..√N] を N/(i^2) = q（切り捨て）となる半開区間 i∈(il..ir] に分割し，
* i について昇順にそれぞれに対して f(il, ir, q) を呼び出す．
*/
template <class T, class FUNC>
void quotient_range_squared(T N, const FUNC& f) {
	//（例）
	// 例えば N = 99 のときは (0..9] を以下のように分割できる：
	//		i の範囲		q=N/(i^2)
	//		(0..1]		99
	//		(1..2]		24
	//		(2..3]		11
	//		(3..4]		6
	//		(4..5]		3
	//		(5..7]		2
	//		(7..9]		1

	T il = 0, ir = 1; T q = N;
	while (true) {
		T nir = ir + 1;
		T nq = N / (nir * nir);
		if (q == nq) break;

		f(il, ir, q);

		il = ir;
		ir = nir;
		q = nq;
	}

	while (q >= 1) {
		ir = (T)floor(sqrtl(1.L * N / q));

		f(il, ir, q);

		il = ir;
		q--;
	}

	/* f の定義の雛形
	using T = ll;
	auto f = [&](T il, T ir, T q) {

	};
	quotient_range_squared(N, f);
	*/
}


//【除原理】O(n^(3/4))
/*
* a と A が関係式
*	a[i] = Σd≧1 A[i/d]（切り捨て）
*	a[0] = 0
* を満たすとし，以下で定まる al, ah を Al, Ah で上書きする．
* m = √n（切り捨て）とし，al, bl, Ah, Ah は以下の通りとする：
*	al[i] = a[i]	(i∈[1..m]),  Al も同様
*	ah[i] = a[n/i]	(i∈[1..m]),  Ah も同様
*
* 利用：【商列挙】
*/
template <class T>
void exclusion_principle(ll n, vector<T>& al, vector<T>& ah) {
	// verify : https://atcoder.jp/contests/abc162/tasks/abc162_e

	//【方法】
	// a と A の関係式を変形すると，漸化式
	//		A[i] = a[i] - Σd≧2 A[i/d]
	// を得る．これを素朴に用いれば良い．
	//		floor(floor(i / d) / e) = floor(i / (d e))
	// に注意すれば，計算に必要な情報は足りていることが分かる．

	int m = sz(al) - 1;

	// Al を計算する：O(Σi∈[1..nl] √i) = O(nl√nl)
	repi(i, 1, m) {
		auto f = [&](ll il, ll ir, ll q) {
			if (il == 0) return;
			al[i] -= al[q] * (ir - il);
		};
		quotient_range(i, f);
	}

	auto get_ah = [&](ll i) { return i <= m ? ah[i] : al[n / i]; };

	// Ah を計算する：O(Σ_i∈[1..nh] √(n/i)) = O(√(n nh)) = O(nh√nl)
	repir(i, m, 1) {
		auto f = [&](ll il, ll ir, ll q) {
			if (il == 0) return;
			ah[i] -= get_ah(il * i) * (ir - il);
		};
		quotient_range(n / i, f);
	}
}


//【除原理（組）】O(r^(3/4))
/*
* N の部分多重集合からなる多重集合族 Ω が積と商について閉じているとする．
*	f(l..r] := #{S∈Ω | l<S≦r かつ GCD(S)=1}
* とおくと，以下の漸化式が成り立つ：
*	f(l..r] = #{S∈Ω | l<S≦r} - Σd≧2 f(l/d..r/d]（切り捨て）
*
* 証明：余事象を考えると，
*	f(l..r] = #{S∈Ω | l<S≦r} - Σd≧2 #{S∈Ω | l<S≦r かつ GCD(S)=d}
* を得る．GCD の性質 GCD(a, b) d = GCD(a d, b d) と Ω の積閉性より，
*	#{S∈Ω | l<S≦r かつ GCD(S)=d} = #{S∈Ω | l/d<S≦r/d かつ GCD(S)=1}
* であるから先の漸化式を得る．
*
* verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_i
*/


//【切り捨て除算】O(1)
/*
* a, b の正負によらず，数学的な floor(a / b) を返す．
*/
template <class T>
T floor_div(T a, T b) {
	// verify : https://atcoder.jp/contests/abc315/tasks/abc315_g

	Assert(b != 0);

	// 分母が負の場合は，分子と分母に -1 を掛けて分母を正にする．
	if (b < 0) { a *= -1; b *= -1; };

	// 分子が非負の場合は，a / b で切り捨てになる．
	if (a >= 0) return a / b;
	// 分子が負の場合は，左右反転して切り上げ商を計算し，再度左右反転する．
	else return -((-a + b - 1) / b);
}


//【切り上げ除算】O(1)
/*
* a, b の正負によらず，数学的な ceil(a / b) を返す．
*/
template <class T>
T ceil_div(T a, T b) {
	// verify : https://atcoder.jp/contests/abc378/tasks/abc378_b

	Assert(b != 0);

	// 分母が負の場合は，分子と分母に -1 を掛けて分母を正にする．
	if (b < 0) { a *= -1; b *= -1; };

	// 分子が非負の場合は，(a + b - 1) / b で切り上げになる．
	if (a >= 0) return (a + b - 1) / b;
	// 分子が負の場合は，左右反転して切り捨て商を計算し，再度左右反転する．
	else return -((-a) / b);
}


//【整除算の結合法則】
/*
* 切り捨て除算や切り上げ除算はある種の結合法則を満たす．すなわち以下の式が成り立つ：
*		floor(floor(a / b) / c) = floor(a / (b c))
*		ceil(ceil(a / b) / c) = ceil(a / (b c))
*
* verify : https://atcoder.jp/contests/abc256/tasks/abc256_h
*/


//【切り捨て（余り指定）】O(1)
/*
* x 以下の整数で mod m で k に等しい最大のものを返す．
*/
template <class T>
T floor_mod(T x, T m, T k) {
	// verify: https://atcoder.jp/contests/abc334/tasks/abc334_b
	Assert(m > 0);
	return x - smod(x - k, m);
}


//【切り上げ（余り指定）】O(1)
/*
* x 以上の整数で mod m で k に等しい最小のものを返す．
*/
template <class T>
T ceil_mod(T x, T m, T k) {
	// verify: https://atcoder.jp/contests/abc334/tasks/abc334_b
	Assert(m > 0);
	return x + smod(k - x, m);
}


//【剰余の反復 → 速く収束】
/*
* 非負整数 a を m(≦a) で割った余りは a/2 未満になる．
*
* 証明：m ≦ a/2 のときは明らか．m > a/2 のときは
*		a mod m = a - m < a - a/2 = a/2
*
* verify : https://atcoder.jp/contests/agc003/tasks/agc003_e
*/


//【整除順序集合の最大反鎖】
/*
* [1..2n] に整除関係を入れた順序集合における最大反鎖の大きさは n である．
* 
*（証明）[n+1..2n] は大きさ n の反鎖なので，大きさが n より大きい反鎖がとれないことを示せば良い．
* ディルワースの定理より，代わりに大きさ n のパス被覆がとれることを示せば良いが，
* これは各奇数 k に対して k-2k-4k-8k-... をパスとして選べば実現できる．
* 
* verify : https://atcoder.jp/contests/arc141/tasks/arc141_d
*/


//【floor_sum 系】
/*
* 列 ディレクトリへ
*/


//【約数からの寄与の除去】
/*
* 元の式：A[n] = Σd|n a[d]
*	A[1] = a[1]
*	A[2] = a[1] + a[2]
*	A[3] = a[1]        + a[3]
*	A[4] = a[1] + a[2]        + a[4]
*	A[5] = a[1]                      + a[5]
*	A[6] = a[1] + a[2] + a[3]               + a[6]
*	A[7] = a[1]                                    + a[7]
*	A[8] = a[1] + a[2]        + a[4]                      + a[8]
*	A[9] = a[1]        + a[3]                                    + a[9]
*	A[10]= a[1] + a[2]               + a[5]                             + a[10]
*	A[11]= a[1]                                                                + a[11]
*	A[12]= a[1] + a[2] + a[3] + a[4]        + a[6]                                    + a[12]
*
* A[1..12] から a[1..12] を一括で求めたい → 約数メビウス変換
*	a[1] =  A[1]
*	a[2] = -A[1] + A[2]
*	a[3] = -A[1]        + A[3]
*	a[4] =       - A[2]        + A[4]
*	a[5] = -A[1]                      + A[5]
*	a[6] =  A[1] - A[2] - A[3]               + A[6]
*	a[7] = -A[1]                                    + A[7]
*	a[8] =                     - A[4]                      + A[8]
*	a[9] =              - A[3]                                    + A[9]
*	a[10]=  A[1] - A[2]               - A[5]                             + A[10]
*	a[11]= -A[1]                                                                + A[11]
*	a[12]=         A[2]        - A[4]        - A[6]                                    + A[12]
*
* A[1,2,3,4,6,12] から a[1,2,3,4,6,12] を一括で求めたい → 約数メビウス変換（添字約数制限）
*	a[1] =  A[1]
*	a[2] = -A[1] + A[2]
*	a[3] = -A[1]        + A[3]
*	a[4] =       - A[2]        + A[4]
*	a[6] =  A[1] - A[2] - A[3]               + A[6]
*	a[12]=         A[2]        - A[4]        - A[6]                                    + A[12]
*
* A[1,2,3,4,6,12] から a[12] のみをピンポイントで求めたい → 約数系包除
*	a[12]=         A[2]        - A[4]        - A[6]                                    + A[12]
*
* A[1,2,3,4,6,12] から a[1,2,3,4,6,12] を逐次的に求めたい → 除原理（使う価値なし）
*	a[1] =  A[1]
*	a[2] = -a[1] + A[2]
*	a[3] = -a[1]        + A[3]
*	a[4] = -a[1] - a[2]        + A[4]
*	a[6] = -a[1] - a[2] - a[3]               + A[6]
*	a[12]= -a[1] - a[2] - a[3] - a[4]        - a[6]                                    + A[12]
*/


//【倍数からの寄与の除去】
/*
* 元の式：A[n] = Σn|d a[d]
*	A[1] = a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + a[8] + a[9] + a[10] + a[11] + a[12]
*	A[2] =        a[2]        + a[4]        + a[6]        + a[8]        + a[10]         + a[12]
*	A[3] =               a[3]               + a[6]               + a[9]                 + a[12]
*	A[4] =                      a[4]                      + a[8]                        + a[12]
*	A[5] =                             a[5]                             + a[10]
*	A[6] =                                    a[6]                                      + a[12]
*	A[7] =                                           a[7]
*	A[8] =                                                  a[8]
*	A[9] =                                                         a[9]
*	A[10]=                                                                a[10]
*	A[11]=                                                                        a[11]
*	A[12]=                                                                                a[12]
*
* A[1..12] から a[1..12] を一括で求めたい → 倍数メビウス変換
*	a[1] = A[1] - A[2] - A[3]        - A[5] + A[6] - A[7]               + A[10] - A[11]
*	a[2] =        A[2]        - A[4]        - A[6]                      - A[10]         + A[12]
*	a[3] =               A[3]               - A[6]               - A[9]
*	a[4] =                      A[4]                      - A[8]                        - A[12]
*	a[5] =                             A[5]                             - A[10]
*	a[6] =                                    A[6]                                      - A[12]
*	a[7] =                                           A[7]
*	a[8] =                                                  A[8]
*	a[9] =                                                         A[9]
*	a[10]=                                                                A[10]
*	a[11]=                                                                        A[11]
*	a[12]=                                                                                A[12]
*
* A[1..12] から a[1] のみをピンポイントで求めたい → 約数系包除（使う価値なし）
*	a[1] = A[1] - A[2] - A[3]        - A[5] + A[6] - A[7]               + A[10] - A[11]
*
* A[1..12] から a[1..12] を逐次的に求めたい → 除原理（使う価値なし）
*/


