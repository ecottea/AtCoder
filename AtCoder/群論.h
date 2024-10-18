#pragma once
#include "header.h"
#include "二項係数.h"
#include "数論.h"
#include "整除.h"
#include "列挙(分割).h"
// ■■■■■ 群論 ■■■■■


//【置換の数え上げ（型指定）】O(k)
/*
* 自然数の分割 p[0..k)（広義単調減少）で表される型をもつ置換の個数を返す．
*
* 制約 : fm は (Σp[0..k))! まで計算可能であること．
*/
mint count_permutation_type(const vi& p, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc226/tasks/abc226_f

	//【方法】
	// n = Σp[0..k) とおく．求める置換の個数は次の (1), (2), (3) の計算で得られる：
	// 
	// (1) 各 p[i] に対応する巡回置換に [0..n) のどの元を割り当てるかが多項係数 (n, [p[0..k)]) 通り．
	// (2) p[i] に対応する巡回置換の中でどの順に並べるかが (p[i] - 1)! 通り．（円順列）
	// (3) 同じ長さの巡回置換には区別は無いので，各長さごとに (個数)! で割り引く．
	//
	// 実際には (1) の分母と (2) の分子は打ち消し合い，残るのは分母の p[i] のみである．

	int k = sz(p);

	// n : 置換の大きさ，cnt : 各長さごとの個数，dnm : 分母
	int n = 0, cnt = 0; mint dnm = 1;

	rep(i, k) {
		// 置換の大きさを更新する．
		n += p[i];

		// (1) の分母と (2) の分子が打ち消し合い残った分母の p[i] を掛けておく．
		dnm *= p[i];

		// 新しい長さに変わったらそれまでの (個数)! を分母に掛けておく．
		if (i > 0 && p[i - 1] != p[i]) {
			dnm *= fm.fact(cnt);
			cnt = 1;
		}
		else cnt++;
	}
	dnm *= fm.fact(cnt);

	return fm.fact(n) / dnm;
}


//【位数分布（対称群）】O(n の分割数）（n = 50 くらいまで動く）
/*
* n 次対称群に位数 d の元が c 個あるとし，{d, c} を昇順に並べたリストを返す．
*
* 制約 : fm は n! まで計算可能であること．
*
* 利用：【自然数の分割の列挙（値が k 以下）】,【置換の数え上げ（型指定）】
*/
vector<pim> permutation_order_distribution(int n, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc226/tasks/abc226_f

	//【備考】
	// EGF からのメビウス変換を考えることで
	//		c = Σ_i|d μ(d/i) [z^n] exp(Σ_j|d z^j/j)
	// が得られるが，d の値域がクソ広いので微妙．
	// 乗法的数論関数 f について Σd c f(d) を求めるくらいならできる．

	auto ips = integer_partitions_val(n, n);

	map<int, mint> d_to_c;
	repe(ip, ips) {
		// d : 型 ip をもつ置換の位数（各巡回置換の長さの最小公倍数）
		int d = 1;
		repe(i, ip) d = d / gcd(d, i) * i;

		// c : 型 ip をもつ置換の個数
		mint c = count_permutation_type(ip, fm);

		d_to_c[d] += c;
	}

	vector<pim> res;
	for (auto& [d, c] : d_to_c) res.emplace_back(d, c);

	return res;
}


//【位数分布（Z/nZ）】O(√n)
/*
* Z/nZ に位数 d の元が c 個あるとし，{d, c} を昇順に並べたリストを返す．
*
* 利用：【約数倍数変換（添字約数制限）】
*/
vector<pll> order_distribution(ll n) {
	// verify : https://atcoder.jp/contests/abc212/tasks/abc212_g

	Limited_div_mul_transform<ll> D(n);

	unordered_map<ll, ll> cnt;
	repe(d, D.divs) cnt[d] = d;

	D.divisor_mobius(cnt);

	vector<pll> res;
	for (auto& [d, c] : cnt) res.emplace_back(d, c);
	sort(all(res));

	return res;
}


//【バーンサイドの補題】
/*
* G-集合 X について，|X/G| を軌道の数，|X^g| を g∈G によって固定される X の元の個数とするとき，
*		|X/G| = 1/|G| Σg∈G |X^g|
* が成り立つ．
*
* G = Z/nZ のとき
* verify : https://yukicoder.me/problems/no/125
* 
* G = D_n のとき
* verify : https://yukicoder.me/problems/no/1728
* 
* G = V_4 = Z/2Z × Z/2Z のとき
* verify : https://yukicoder.me/problems/no/2383
*/


//【ポリアの数え上げ定理】
/*
* 置換群 G ⊂ S_n の作用で移り合うものを同一視する条件で，点群 [0..n) を k 色で彩色する方法の数は
*		1/|G| Σσ∈G k^(<σ> の [0..n) への作用の軌道の個数)
*		= 1/|G| Σσ∈G k^(σ がもつ巡回置換の個数)
* と表される．
*/


//【互換で生成される置換群】
/*
* 対称群 S_n の互換の集合 E で生成される部分群 H は次のように決定できる：
*
* 無向グラフ G を，[0..n) を頂点とし互換 e∈E に対応する辺をもつよう定める．
* G の k 個の連結成分それぞれの頂点集合を A[0..k) とおくと，
*		H = S_(A[0]) * S_(A[1]) * ... * S_(A[k-1])
* となる．
*
* verify : https://atcoder.jp/contests/arc107/tasks/arc107_c
*/


//【置換 → 巡回置換の積】
/*
* 順列.h へ
*/


//【置換 → 隣接互換の積】
/*
* 順列.h へ
*/


//【巡回置換 → 互換の積】
/*
* (12...n) = (12)(23)...(n-1 n) を用いれば良い．
*/


//【置換の位数】
/*
* 置換 p の位数は，p を巡回置換の積に分解したときの，各巡回置換の長さの最小公倍数に等しい．
* 
* verify : https://atcoder.jp/contests/abc226/tasks/abc226_f
*/


//【巡回置換の累乗の位数】
/*
* σ を長さ n の巡回置換とする．σ^i は g=GCD(n,i) とおいたとき，
* 長さ n/g の巡回置換 g 個の積に分解する．
*/


//【巡回群の自己同型群】
/*
* Aut(Z/nZ) = (Z/nZ)*
*/


//【既約剰余類群の構造】
/*
* C(m) = Z/mZ とおく．C(m) の単元群 C(m)* の構造は次のように決定できる：
* 
* m = p1^d1 p2^d2 ... pn^dn と素因数分解されるとすると，中国剰余定理より
*	C(m)* = C(p1^d1)* × C(p2^d2)* × ... × C(pn^dn)*
* となる．それぞれの直積因子は
*	C(2)* = {0}, U(C(4)) = C(2)
*	C(2^d)* = C(2^(d-2)) × C(2)  (d ≧ 3)
*	C(p^d)* = C(p^(d-1) (p-1))  (p : 奇素数)
* となる．
* 
* 参考 :『代数学 2』p.226 命題 4.7.15
* verify : https://yukicoder.me/problems/no/1881
*/


//【Z[i] の剰余環の位数】
/*
* |Z[i]/(a+bi)| = N(a+bi) = a^2 + b^2
* 
*（証明）
* a+bi で割るということは，複素平面上で a+bi と i(a+bi) で張られた正方形内の格子点を
* 代表させることに相当する．正方形の 1 辺の長さは三平方の定理より √N(a+bi) なので，
* 正方形の面積は N(a+bi) である．1 つの格子点が占める面積は 1 なので，
* 正方形内の格子点の個数は N(a+bi) である（周上に格子点がこないよう適当にずらす）
* 
* 参考 : https://mathlog.info/articles/2088
* verify : https://atcoder.jp/contests/ttpc2023/tasks/ttpc2023_e
*/

