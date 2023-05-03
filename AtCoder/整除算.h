#pragma once
#include "header.h"
// ■■■■■ 整数除算（切り捨て，切り上げ，剰余） ■■■■■


//【商列挙】O(√n)
/*
* i=[1..n] に対し，n/i = q（切り捨て）となる i の範囲が [il..ir) であることを
* {il, ir, q} として il について昇順に格納したリストを返す．
* 各範囲においては余りは公差 -q の等差数列を成す．
*/
vector<tuple<ll, ll, ll>> quotient_range(ll n) {
	// 参考 : https://ei1333.github.io/luzhiled/snippets/math/quotient-range.html
	// verify : https://atcoder.jp/contests/abc230/tasks/abc230_e

	//【方法】
	// n/i の商が q となるような i の範囲を考える．条件を i について整理すると
	//		q = floor(n/i)
	//		⇔ q ≦ n/i < q+1
	//		⇔ i q ≦ n < i(q+1)
	//		⇔ n/(q+1) < i ≦ n/q
	// となる．
	//
	// この幅が 1 以下であれば，q に対応する i は高々 1 個である．その条件は
	//		n/q - n/(q+1) ≦ 1
	//		⇔ (q+1)n - q n ≦ q(q+1)
	//		⇔ n ≦ q(q+1)
	// である．条件をやや弱めて
	//		n ≦ q^2 ⇔ √n ≦ q
	// としてもオーダーに影響はない．

	//（例）
	// 例えば n = 15 のときは以下のように分類できる：
	//		i の範囲		n/i		n mod i
	//		[1..2)		15		[0]
	//		[2..3)		7		[1]
	//		[3..4)		5		[0]
	//		[4..6)		3		[3, 0]
	//		[6..8)		2		[3, 1]
	//		[8..16)		1		[7, 6, 5, 4, 3, 2, 1, 0]

	ll m = (ll)(sqrt(n) + 1e-12);
	vector<tuple<ll, ll, ll>> res;

	// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
	for (int i = 1; n / i > m; i++) res.emplace_back(i, i + 1, n / i);

	// そうでない部分は q ごとにまとめて考える．
	repir(q, m, 1) {
		ll i0 = n / (q + 1) + 1;
		ll i1 = n / q + 1;
		res.emplace_back(i0, i1, q);
	}

	return res;
}


//【商列挙（組）】O(√max(n1, n2))
/*
* i=[1..max(n1,n2)] に対し，(n1/i, n2/i) = (q1, q2)（切り捨て）となる i の範囲が [il..ir) であることを
* {il, ir, q1, q2} として il について昇順に格納したリストを返す．
* 各範囲においては余りは公差 (-q1, -q2) の等差数列を成す．
*/
vector<tuple<ll, ll, ll, ll>> quotient_range(ll n1, ll n2) {
	// verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_i

	ll n_max = max(n1, n2);

	ll m = (ll)(sqrt(n_max) + 1e-12);
	vector<tuple<ll, ll, ll, ll>> res;

	// どちらかの q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
	int i = 1;
	for (; n_max / i > m; i++) res.emplace_back(i, i + 1, n1 / i, n2 / i);

	// そうでない部分は (q1, q2) ごとにまとめて考える．
	ll q1 = n1 / i, q2 = n2 / i;
	while (q1 > 0 || q2 > 0) {
		// [il1..ir1) : n1/i = q1 となる i の範囲
		ll il1 = n1 / (q1 + 1) + 1, ir1 = (q1 > 0 ? n1 / q1 + 1 : INFL);

		// [il2..ir2) : n2/i = q2 となる i の範囲
		ll il2 = n2 / (q2 + 1) + 1, ir2 = (q2 > 0 ? n2 / q2 + 1 : INFL);

		// 両区間の共通部分を記録する．
		ll il = max(il1, il2), ir = min(ir1, ir2);
		if (il < ir) res.emplace_back(il, ir, q1, q2);

		if (ir1 < ir2) q1--;
		else q2--;
	}

	return res;
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
	//		A[i] = a[i] - Σd≧2 b[i/d]
	// を得る．これを素朴に用いれば良い．
	//		floor(floor(i / d) / e) = floor(i / (d e))
	// に注意すれば，計算に必要な情報は足りていることが分かる．

	int m = sz(al) - 1;

	// Al を計算する：O(Σ_i∈[1..nl] √i) = O(nl√nl)
	repi(i, 1, m) {
		auto QR = quotient_range(i);

		for (auto [i1, i2, q] : QR) {
			if (i1 == 1) continue;
			al[i] -= al[q] * (i2 - i1);
		}
	}

	auto get_ah = [&](ll i) { return i <= m ? ah[i] : al[n / i]; };

	// Ah を計算する：O(Σ_i∈[1..nh] √(n/i)) = O(√(n nh)) = O(nh√nl)
	repir(i, m, 1) {
		auto QR = quotient_range(n / i);

		for (auto [i1, i2, q] : QR) {
			if (i1 == 1) continue;
			ah[i] -= get_ah(i1 * i) * (i2 - i1);
		}
	}
}


//【一次式の切り捨て和】O(log(n + m + a + b))
/*
* Σi∈[0..n) floor((a i + b) / m) を返す．
*/
template <class T>
T sum_of_floor_of_linear(T n, T m, T a, T b) {
	// 参考 : https://twitter.com/kyopro_friends/status/1304063876019793921?ref_src=twsrc%5Etfw
	// verify : https://judge.yosupo.jp/problem/sum_of_floor_of_linear

	//【方法】
	// m > 0, 0 ≦ a < m, 0 ≦ b < m として一般性を失わない．また i ← n - i とした
	//		 Σi∈(0..n] floor((a (n - i) + b) / m)
	// も値は変わらない．これは領域
	//		0 < x ≦ n
	//		0 < y ≦ (a (n - x) + b) / m
	// に含まれる格子点の個数と解釈できる．x と y の主従を入れ替えると
	//		0 < y ≦ (a n + b) / m
	//		0 < x ≦ (-m y + a n + b) / a
	// となる．ここに含まれる格子点の個数は
	//		Σi∈(0..floor((a n + b) / m)] floor((-m i + a n + b) / a)
	// である．n' = floor((a n + b) / m) とおき，i ← n' - i とした
	//		Σi∈[0..n') floor((-m (n' - i) + a n + b) / a)
	//		= Σi∈[0..n') floor((m i + (- m n' + a n + b)) / a)
	// も値は変わらない．これで分母がより小さい問題に帰着できた．
	//
	// 次のステップに進む前に m ← m % a とするので，収束の速さはユークリッドの互除法と同じである．

	Assert(m != 0);
	if (n <= 0) return 0;

	T res = 0;

	// m < 0 の場合，分母分子を -1 倍して m > 0 とする．
	if (m < 0) { a *= -1; b *= -1; m *= -1; }

	// a を m だけ増減させた場合の影響は floor なしの和で計算できるので，0 ≦ a < m とする．
	res += (a / m - (T)(a % m < 0)) * (n * (n - 1) / 2);
	a = smod(a, m);

	// b を m だけ増減させた場合の影響は floor なしの和で計算できるので，0 ≦ b < m とする．
	res += (b / m - (T)(b % m < 0)) * n;
	b = smod(b, m);

	while (a > 0) {
		T nn = (a * n + b) / m;
		T nm = a;
		T na = m;
		T nb = -m * nn + a * n + b;

		res += (na / nm - (T)(na % nm < 0)) * (nn * (nn - 1) / 2);
		na = smod(na, nm);

		res += (nb / nm - (T)(nb % nm < 0)) * nn;
		nb = smod(nb, nm);

		n = nn; m = nm; a = na; b = nb;
	}

	return res;
}


//【一次式の剰余の数え上げ】O(log(n + m))
/*
* 各 i∈[0..n) に対する (a i + b) mod m のうち，値が [l..r) に属するものの個数を返す．
*
* 利用：【一次式の切り捨て和】
*/
template <class T>
T count_mod_of_linear(T n, T m, T a, T b, T l, T r) {
	// 参考 : https://twitter.com/maspy_stars/status/1649421402573766656
	// verify : https://yukicoder.me/problems/no/2280

	//【方法】
	// 条件を同値変形していくと，
	//		l ≦ (ai+b) mod m < r
	//		⇔ l ≦ (ai+b) - floor((ai+b)/m) * m < r
	//		⇔ (ai+b-l)/m ≧ floor((ai+b)/m) > (ai+b-r)/m
	// となる．中辺が整数であることと
	//		(左辺) - (右辺) = (r-l)/m ≦ 1
	// であることに注意すると，
	//		(ai+b) mod m ∈ [l..r) ⇔ floor((ai+b-l)/m) - floor((ai+b-r)/m) = 1
	//		(ai+b) mod m !∈ [l..r) ⇔ floor((ai+b-l)/m) - floor((ai+b-r)/m) = 0
	// が分かる．よって floor_sum の差を取れば良い．

	Assert(m > 0);

	if (n <= 0) return 0;

	chmax(l, 0LL); chmin(r, m);
	if (l >= r) return 0;

	a = smod(a, m); b = smod(b, m);

	T res = sum_of_floor_of_linear(n, m, a, b - l);
	res -= sum_of_floor_of_linear(n, m, a, b - r);

	return res;
}


//【一次式の剰余の最小値】O(log(n + m))
/*
* min i∈[0..n) (a i + b) mod m を返す．
*/
ll min_of_mod_of_linear(ll n, ll m, ll a, ll b) {
	// verify : https://judge.yosupo.jp/problem/min_of_mod_of_linear

	Assert(m > 0);
	if (n <= 0) return INFL;

	a = smod(a, m); b = smod(b, m);

	ll res = b;

	while (a > 0) {
		// 単調増加な部分に分解し，その初項だけを並べた新たな列を考えると，
		// それもまた min i∈[0..nn) (na i + nb) mod nm の形で表される．
		ll nm = a;
		ll nn = (a * (n - 1) + b) / m;
		ll nb = a * (((m - b) / a) + 1) + b - m;
		ll na = (a * (((2 * m - b) / a) + 1) + b - 2 * m) - nb;

		n = nn; m = nm; a = smod(na, nm); b = nb % nm;

		if (n == 0) break;

		// a が大きいときはむしろ単調減少していると捉え，
		// 左右反転して単調増加とみなすことで毎回大きさを半分以下にしていける．
		if (2 * a > m) {
			b = (a * (n - 1) + b) % m;
			a = (m - a) % m;
		}

		chmin(res, b);
	}

	return res;
}


//【整数の数え上げ（余り指定）】O(1)
/*
* x∈[l..r) で x ≡ k (mod m) を満たすものの個数を返す．
*/
template <class T>
T count_by_reminder(T l, T r, T m, T k) {
	// verify : https://codeforces.com/contest/628/problem/F

	//【方法】
	// l = k (mod m) になるように l を増加させても答えは変わらない．
	// こうすれば個数は [0..n) 内の m の倍数の数え上げと同様に考えて
	//		(r - l + m - 1) / m
	// で求められる．

	Assert(m > 0);
	if (l >= r) return 0;

	k = smod(k, m);

	l -= k;
	T l2 = l + smod(-l, m);
	l2 += k;

	return (r - l2 + m - 1) / m;
}


//【切り捨て除算】O(1)
/*
* a, b の正負によらず，数学的な floor(a / b) を返す．
*/
template <class T>
T floor_div(T a, T b) {
	// verify : https://yukicoder.me/problems/no/2032

	return a / b - (T)(a % b < 0);
}


//【切り上げ除算】O(1)
/*
* a, b の正負によらず，数学的な ceil(a / b) を返す．
*/
template <class T>
T ceil_div(T a, T b) {
	// verify : https://yukicoder.me/problems/no/2099

	T num = a + b - 1;
	return num / b - (T)(num % b < 0);
}


//【整除算の結合法則】
/*
* 切り捨て除算や切り上げ除算は結合法則を満たす．すなわち以下の式が成り立つ：
*		floor(floor(a / b) / c) = floor(a / (b c))
*		ceil(ceil(a / b) / c) = ceil(a / (b c))
*
* verify : https://atcoder.jp/contests/abc256/tasks/abc256_h
*/


//【切り捨て（余り指定）】O(1)
/*
* 与えられた x に対し，x 以下の y で y ≡ k (mod m) を満たす最大のものを返す．
*/
template <class T>
T floor_mod(T x, T m, T k) {
	//【方法】
	// k = 0 の場合は
	//		y = x - (x mod m)
	// とすればよい．一般の k の場合は，k ずらして考えることにより
	//		y - k = (x - k) - ((x - k) mod m)
	// を得る．

	Assert(m > 0);

	k = smod(k, m);
	x -= k;
	T y = x - smod(x, m);

	return y + k;
}


//【切り上げ（余り指定）】O(1)
/*
* 与えられた x に対し，x 以上の y で y ≡ k (mod m) を満たす最小のものを返す．
*/
template <class T>
T ceil_mod(T x, T m, T k) {
	//【方法】
	// k = 0 の場合は
	//		y = x + (-x mod m)
	// とすればよい．一般の k の場合は，k ずらして考えることにより
	//		y - k = (x - k) + (-(x - k) mod m)
	// を得る．

	Assert(m > 0);

	k = smod(k, m);
	x -= k;
	T y = x + smod(-x, m);

	return y + k;
}


//【余りの取れる値の範囲】
/*
* 非負整数 a を m(≦a) で割った余りは a/2 未満になる．
*
* 証明：m ≦ a/2 のときは明らか．m > a/2 のときは
*		a mod m = a - m < a - a/2 = a/2
*
* verify : https://codeforces.com/contest/1617/problem/C
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


//【余りの和】
/*
* Mod_sum(a) : O(n log n)
*	配列 a で初期化する．
*
* sum_mod(m) : O(max(a) log(n) / m)
*	a[0..n) mod m の和を返す．
*
* sum_lack(m) : O(max(a) log(n) / m)
*	a[0..n) を m で割った不足の和を返す．
*/
struct Mod_sum {
	vi a;    // ★ a でなくバケツで累積和を持てば O(log n) を落とせる．
	int n;
	ll asum; // a[0..n) の和

	// コンストラクタ（何もしない）
	Mod_sum() : n(0), asum(0) {}

	// 配列 a で初期化
	Mod_sum(const vi& a_) : a(a_), n(sz(a)), asum(0) {
		sort(all(a));
		rep(i, n) asum += a[i];
	}

	// a[0..n) mod m の和を返す．
	ll sum_mod(int m) {
		ll res = asum;

		for (ll v = m; v <= a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			res -= m * (ll)distance(lower_bound(all(a), v), a.end());
		}

		return res;
	}

	// a[0..n) を m で割った不足の和を返す．
	ll sum_lack(int m) {
		// sum : 1-indexed での a[0..n) mod m の和
		ll sum = asum;

		for (ll v = m; v < a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			sum -= m * (ll)distance(lower_bound(all(a), v + 1), a.end());
		}

		// 不足分を返す．
		return m * (ll)n - sum;
	}
};


