#pragma once
#include "header.h"
#include "二項係数.h"
#include "数論.h"
#include "約数倍数変換.h"
// ■■■■■ 群論 ■■■■■


//【対称群の元の位数分布】O(n の分割数）（n = 50 くらいまで動く）
/*
* n 次対称群に位数 d の元が k 個あることを cnt[d] = k として格納する．
*
*（自然数の分割）
*
* 利用：【階乗と二項係数（mint利用）】
*/
void permutation_order_distribution(int n, map<int, mint>& cnt) {
	//【方法】
	// 巡回置換の積に分解したときの各巡回置換の長さの集合は n の分割になる．
	// 分割が得られれば位数は長さの最小公倍数である．
	// 
	// 各 n の分割 n = n_1 + ... + n_k に対応する置換の個数を求めることを考える．
	// まずそれぞれの n_i に [1..n] のどの元を割り当てるかが多項係数 (n_1, ..., n_k) 通り
	// 次にそれぞれの中でどの順に並べるかが (n_i - 1)! 通り（巡回対称性に注意）
	// 最後に同じ長さの巡回置換には区別は無いので，その階乗通りで割ればよい．

	map<int, int> ip; // ip[i] : 分割に i を何個用いたか

	Factorial_mint fm(n);

	// n を k 以下の数で分割する．
	function<void(int, int)> rf = [&](int n, int k) {
		// 分割しきった場合
		if (n == 0) {
			vi a; // n の分割
			int l = 1; // n の分割の最小公倍数
			mint res = 1;

			repe(tmp, ip) {
				int v, c; // v : 数，c : 個数
				tie(v, c) = tmp;

				// n の分割を構成
				rep(hoge, c) a.push_back(v);

				// n の分割の最小公倍数を計算
				l = l / gcd(l, v) * v;

				// 上の方法での最後の調整を先に済ませておく
				res *= fm.factorial(v - 1).pow(c) * fm.factorial_inv(c);
			}
			res *= fm.multinomial(a);

			cnt[l] += res;

			return;
		}

		// 分割に使える数がもうない場合
		if (k == 0) return;

		// n が k 以上のときは，n を k と n-k に分割できる．
		if (n >= k) {
			ip[k]++;

			rf(n - k, k);

			if (--ip[k] == 0) ip.erase(k);
		}

		// これ以上 n の分割に k を使わない場合
		rf(n, k - 1);
	};

	rf(n, n);
}


//【Z/nZ の位数分布】O(√n)
/*
* Z/nZ に位数 d の元が何個あるかを cnt[d] に格納する．
*
* 利用：【倍数変換（添字約数制限）】,【素因数と約数の列挙】
*/
void order_distribution(ll n, unordered_map<ll, ll>& cnt) {
	// verify : https://atcoder.jp/contests/abc212/tasks/abc212_g

	vl ps, divs;
	primefactors_and_divisors(n, ps, divs);

	Limited_multiple_transform<ll> lmt(ps, divs);

	cnt.clear();
	repe(d, divs) cnt[d] = d;

	lmt.multiple_mobius(cnt);
}



//【互換で生成される対称群の部分群】
/*
* 対称群 S_n の互換の集合 E で生成される部分群 H は次のように決定できる：
*
* 無向グラフ G を，[0..n) を頂点とし互換 e ∈ E に対応する辺をもつよう定める．
* G の k 個の連結成分それぞれの頂点集合を A[0..k) とおくと，
*		H ~= S_(A[0]) * S_(A[1]) * ... * S_(A[k-1])
* となる．これは Union-Find を用いることで効率的に計算できる．
*
* verify : https://atcoder.jp/contests/arc107/tasks/arc107_c
*/


//【既約剰余類群の構造】
/*
* C(m) = Z/mZ とおく．C(m) の単元群 U(C(m)) の構造は次のように決定できる：
* 
* m = p1^d1 p2^d2 ... pn^dn と素因数分解されるとすると，中国式剰余定理より
*	U(C(m)) = U(C(p1^d1)) *  U(C(p2^d2)) * ... * U(C(pn^dn))
* となる．それぞれの直積因子は，『代数学 2』p.226 命題 4.7.15 より
*	U(C(2)) = {0}, U(C(4)) = C(2)
*	U(C(2^d)) = C(2^(d-2)) * C(2)  (d >= 3)
*	U(C(p^d)) = C(p^(d-1) (p-1))  (p : 奇素数)
* となる．
* 
* verify : https://yukicoder.me/problems/no/1881
*/


