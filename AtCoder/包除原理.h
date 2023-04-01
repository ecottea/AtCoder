#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 包除原理 ■■■■■


//【状態系包除原理】O(2^n)
/*
* 集合族 S[0..n) について，添字集合が set で表されるような集合族の交わりの大きさ
* #(∩i∈set S[i]) が c[set] であるときの，どの集合にも属さない要素の個数を返す．
*/
template <class T>
T state_PIE(const vector<T>& c) {
	// verify : https://atcoder.jp/contests/tokiomarine2020/tasks/tokiomarine2020_e

	//【方法】
	// 求める値は以下の式で計算できる：
	//		Σset⊂[0..n) (-1)^|set| c[set]

	int n = msb(sz(c));

	T res = 0;
	repb(set, n) {
		int sign = (popcount(set) % 2 ? -1 : 1);
		res += (T)sign * c[set];
	}

	return res;
}


//【状態系包除原理（一括）】O(2^n n)
/*
* 集合族 S[0..n) について，添字集合が set で表されるような集合族の交わりの大きさ
* #(∩i∈set S[i]) が c[set] であるとする．
* 
* 添字集合 set に含まれる添字をもつ集合のみに属する要素の個数を c'[set] とおくと，
* c' は c に上位集合メビウス変換を施すことで得られる．
*/


//【状態系包除原理（個数ごと）】O(n)
/*
* 集合族 S[0..n) について，添字集合が set で表されるような集合族の交わりの大きさ
* #(∩i∈set S[i]) が c[set] であるとする．
*
* もしちょうど k 個の集合の交わりの大きさの和
*	f(k) := Σ|set|=k c[set]
* が効率よく求まるなら，どの集合にも属さない要素の個数は
*	Σset⊂[0..n) (-1)^|set| c[set]
*	= Σk∈[0..n] Σ|set|=k (-1)^|set| c[set]
*	= Σk∈[0..n] (-1)^k Σ|set|=k c[set]
*	= Σk∈[0..n] (-1)^k f(k)
* として求められる．
*
* verify : https://atcoder.jp/contests/abl/tasks/abl_f
*/


//【個数系包除原理】O(n)
/*
* 集合族 S[0..n) について，k 個の集合の交わり（k = 0 なら全体集合とする）の大きさが
* 集合の選び方によらず一律 c[k] であるときの，どの集合にも属さない要素の個数を返す．
*
* 制約：fm は n! まで計算可能であること
*
* 利用：【階乗など（法が大きな素数）】
*/
mint counting_PIE(const vm& c, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc172/tasks/abc172_e

	int n = sz(c) - 1;

	mint res = 0;

	int sign = 1;
	repi(k, 0, n) {
		res += sign * fm.bin(n, k) * c[k];
		sign *= -1;
	}

	return res;
}


//【個数系包除原理（一括）】O(n^2)
/*
* 集合族 S[0..n) について，k 個の集合の交わり（k = 0 なら全体集合とする）の大きさが
* 集合の選び方によらず一律 c[k] であるときの，各 i について特定の i 個の集合のみに属する
* 要素の総数を格納したリストを返す．
*
* 制約：fm は n! まで計算可能であること
*
* 利用：【階乗など（法が大きな素数）】
*/
vm counting_PIE_all(const vm& c, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc217/tasks/abc217_g

	//【方法】
	// 包除原理より以下の式が成り立つ：
	//		cnt[i] = Σj∈[i..n] (-1)^(j-i) bin(n-i, j-i) c[j]

	//【例】（n = 3 のとき）
	//		cnt[3] = 0C0 c[3]
	//		cnt[2] = 1C0 c[2] - 1C1 c[3]
	//		cnt[1] = 2C0 c[1] - 2C1 c[2] + 2C2 c[3]
	//		cnt[0] = 3C0 c[0] - 3C1 c[1] + 3C2 c[2] - 3C3 c[3]

	int n = sz(c) - 1;
	vm cnt(n + 1);

	repi(i, 0, n) repi(j, i, n) {
		cnt[i] += ((j - i) % 2 ? -1 : 1) * fm.bin(n - i, j - i) * c[j];
	}

	return cnt;
}


//【個数系包除原理（一括，mod 998244353）】O(n log n)
/*
* 集合族 S[0..n) について，k 個の集合の交わり（k = 0 なら全体集合とする）の大きさが
* 集合の選び方によらず一律 c[k] であるときの，各 i について特定の i 個の集合のみに属する
* 要素の総数を格納したリストを返す．
* 
* 制約：fm は n! まで計算可能であること
*
* 利用：【階乗など（法が大きな素数）】
*/
vm counting_PIE_all_fast(vm c, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc217/tasks/abc217_g

	//【方法】
	// 計算すべき式は
	//		cnt[i] = Σj∈[i..n] (-1)^(j-i) bin(n-i, j-i) c[j]
	// であった．これを
	//		cnt[i] = Σj∈[i..n] (-1)^(j-i) (n-i)! / ((j-i)! (n-j)!) c[j]
	//		cnt[i] / (n-i)! = Σj∈[i..n] ((-1)^(j-i) / (j-i)!) (c[j] / (n-j)!)
	// と変形する．ここで
	//		coef[i] = (-1)^(n-i) / (n-i)!
	//		c'[i] = c[i] / (n-i)!
	//		cnt'[i] = cnt[i] / (n-i)!
	// とおけば，cnt' は coef と c' の上側畳込みで得られる．

	int n = sz(c) - 1;

	repi(i, 0, n) c[i] *= fm.fact_inv(n - i);

	vm coef(n + 1);
	repi(i, 0, n) coef[i] = ((n - i) % 2 ? -1 : 1) * fm.fact_inv(n - i);

	// coef と c を畳み込んで上側を取得する．
	vm cnt = convolution(coef, c);
	cnt.erase(cnt.begin(), cnt.begin() + n);

	repi(i, 0, n) cnt[i] *= fm.fact(n - i);

	return cnt;
}


//【約数系包除原理（一括）】O(n log(log n))
/*
*【約数倍数変換】のメビウス変換を利用すればよい．
*/


//【除原理】O(r^(3/4))
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


//【メビウス変換・包除原理・除原理の関係？】
/*
* 約数からの寄与を取り除きたい場合を例にする．
* 
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
* A[1,2,3,4,6,12] から a[6] のみをピンポイントで求めたい → 約数系包除
*	a[12]=         A[2]        - A[4]        - A[6]                                    + A[12]
*
* A[1,2,3,4,6,12] から a[1,2,3,4,6,12] を逐次的に求めたい → 除原理
*	a[1] =  A[1]
*	a[2] = -a[1] + A[2]
*	a[3] = -a[1]        + A[3]
*	a[4] = -a[1] - a[2]        + A[4]
*	a[6] = -a[1] - a[2] - a[3]               + A[6]
*	a[12]= -a[1] - a[2] - a[3] - a[4]        - a[6]                                    + A[12]
*/


//【指定バウンディングボックスをもつ点配置の数え上げ】O(1)
/*
* バウンディングボックスが [0..h)×[0..w) になるような n 個の格子点の配置の数を返す．
* 
* 制約：fm は (h * w)! まで計算可能であること
*
*（状態系包除原理）
*
* 利用：【階乗など（法が大きな素数）】
*/
mint count_points_in_BB(int n, int h, int w, Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc003/tasks/abc003_4

	mint res = 0;

	// 無条件の場合
	res += fm.bin(h * w, n);

	// 少なくとも 1 個の辺が条件を満たしていない場合
	res -= 2 * fm.bin((h - 1) * w, n); // 上または下
	res -= 2 * fm.bin(h * (w - 1), n); // 左または右

	// 少なくとも 2 個の辺が条件を満たしていない場合
	res += fm.bin((h - 2) * w, n); // 上下
	res += fm.bin(h * (w - 2), n); // 左右
	res += 4 * fm.bin((h - 1) * (w - 1), n); //（上または下）かつ（左または右）

	// 少なくとも 3 個の辺が条件を満たしていない場合
	res -= 2 * fm.bin((h - 2) * (w - 1), n); // 上下かつ（左または右）
	res -= 2 * fm.bin((h - 1) * (w - 2), n); //（上または下）かつ左右

	// 全ての辺が条件を満たしていない場合
	if (h >= 2 && w >= 2) res += fm.bin((h - 2) * (w - 2), n); // 上下かつ左右

	return res;
}
