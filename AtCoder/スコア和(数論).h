#pragma once
#include "header.h"
#include "ディリクレ畳込み.h"
#include "構造(数論).h"
#include "列挙(数論).h"
// ■■■■■ スコア和（数論） ■■■■■


//【乗法的数論関数の総和】O(N^(3/4) / log N)
/*
* 与えられた乗法的数論関数 f について，Σi∈[1..N] f(i) を返す．
*	acc[i] は Σp∈[1..i] f(p)  (p:素数) とする．
*	f(p, e) は f(p^e) を返す．
*
* 利用：【素数の列挙】
*/
template <class T, class FUNC>
T Multiplicative_sum(const FUNC& f, const Floor_vector<T>& acc) {
	// verify : https://judge.yosupo.jp/problem/sum_of_multiplicative_function

	//【方法】
	// 自然数 i の最大素因数を gpf(i) と表す．
	// 頂点 [1..n] をもち，i の親が i / gpf(i) である木 T を考える．（根は 1） 
	// 求める和は次の形に書き直せる：
	//		f(1) + Σ_i:葉でない頂点 Σ_j:iの子 f(j)
	//
	// 例えば n=60 のときの i=3 を考えると，その子は
	//		9, 15, 21, 33, 39, 51, 57
	// である．これらに f を施した値の総和は，f の乗法性より
	//		Σ_j:iの子 f(j)
	//		= f(9) + f(3) (f(5) + f(7) + f(11) + f(13) + f(17) + f(19))
	//		= f(3*3) + f(3) Σ_p∈(3..n/3] f(p)
	//		= f(i g) + f(g) (Σ_p∈[1..n/g] f(p) - Σ_p∈[1..g] f(p))  (g := gpf(i))
	// として求められる．
	//
	// またこの場合 i * 5^2 > n となるので，3*5 以上の子は全て葉であることが探索しなくても分かる．
	// T はほとんどが葉なので，葉のみの枝刈りとはいえ真に計算量が改善する．

	ll N = acc.N; int nl = acc.nl;

	if (N <= 0) return 0;
	if (N == 1) return 1;
	if (N == 2) return 1 + f(2, 1);
	if (N == 3) return 1 + f(2, 1) + f(3, 1);

	// ps : nl = √n 以下の素数の昇順リスト
	auto ps = eratosthenes(nl);

	T res = 1;

	// s     : 注目頂点
	// i_gpf : s の最大素因数 p が ps で何番目の素数か
	// cnt   : s に素因数 p が含まれている個数
	// val   : f(s / p^cnt)
	function<void(ll, int, int, T)> dfs = [&](ll s, int i_gpf, int cnt, T val) {
		ll p = (ll)ps[i_gpf];

		// s の最小の子 s * p からの寄与を加算する．
		res += val * f(p, cnt + 1);

		// その他の s の子からの寄与をまとめて加算する．
		T nval = val * f(p, cnt);
		res += nval * (acc.get_h(s) - acc.get_l((int)p));

		// s の最小の子 s * p を探索する．
		if (s <= N / (p * p)) dfs(s * p, i_gpf, cnt + 1, val);

		// その他の s の子を探索する．
		for (int i = i_gpf + 1; i < sz(ps) && s <= N / ((ll)ps[i] * ps[i]); i++) {
			dfs(s * ps[i], i, 1, nval);
		}
	};
	dfs(1, 0, 0, 1);

	return res;

	/* f, acc の定義の雛形
	using T = mint;
	auto f = [&](ll p, int e) {
		if (e == 0) return T(1);
		return a * e + b * p;
	};

	int nl = (int)(sqrt(N) + 1e-9);
	Floor_vector<T> acc(N, nl);
	int nh = acc.nh;
	repi(i, 1, nl) acc.set_l(i, i);
	repi(d, 1, nh) acc.set_h(d, N / d);

	T res = Multiplicative_sum<T>(f, acc);
	*/
}


//【乗法的数論関数の総和（差分更新）】O(N^(3/4) / log N)
/*
* 与えられた乗法的数論関数 f について，Σi∈[1..N] f(i) を返す．
*	acc[i] は Σp∈[1..i] f(p)  (p:素数) とする．
*	i が素因数 p を cnt 個含んでおり f(i) = val のとき，mul(val, p, cnt) は f(i p) を返す．
*
* 利用：【素数の列挙】
*/
template <class T, class FUNC>
T Multiplicative_sum_mul(const FUNC& mul, const Floor_vector<T>& acc) {
	// verify : https://atcoder.jp/contests/jsc2024-final/tasks/jsc2024_final_b

	//【方法】
	// 自然数 i の最大素因数を gpf(i) と表す．
	// 頂点 [1..n] をもち，i の親が i / gpf(i) である木 T を考える．（根は 1） 
	// 求める和は次の形に書き直せる：
	//		f(1) + Σ_i:葉でない頂点 Σ_j:iの子 f(j)
	//
	// 例えば n=60 のときの i=3 を考えると，その子は
	//		9, 15, 21, 33, 39, 51, 57
	// である．これらに f を施した値の総和は，f の乗法性より
	//		Σ_j:iの子 f(j)
	//		= f(9) + f(3) (f(5) + f(7) + f(11) + f(13) + f(17) + f(19))
	//		= f(3*3) + f(3) Σ_p∈(3..n/3] f(p)
	//		= f(i g) + f(g) (Σ_p∈[1..n/g] f(p) - Σ_p∈[1..g] f(p))  (g := gpf(i))
	// として求められる．
	//
	// またこの場合 i * 5^2 > n となるので，3*5 以上の子は全て葉であることが探索しなくても分かる．
	// T はほとんどが葉なので，葉のみの枝刈りとはいえ真に計算量が改善する．

	ll N = acc.N; int nl = acc.nl;

	if (N <= 0) return 0;
	if (N == 1) return 1;
	if (N == 2) return 1 + mul(1, 2, 0);
	if (N == 3) return 1 + mul(1, 2, 0) + mul(1, 3, 0);

	// ps : nl = √n 以下の素数の昇順リスト
	auto ps = eratosthenes(nl);

	T res = 1;

	// s     : 注目頂点
	// i_gpf : s の最大素因数 p が ps で何番目の素数か
	// cnt   : s に素因数 p が含まれている個数
	// val   : f(s)
	function<void(ll, int, int, T)> dfs = [&](ll s, int i_gpf, int cnt, T val) {
		ll p = (ll)ps[i_gpf];

		// s の最小の子 s * p からの寄与を加算する．
		T nval = mul(val, p, cnt);
		res += nval;

		// その他の s の子からの寄与をまとめて加算する．
		res += val * (acc.get_h(s) - acc.get_l((int)p));

		// s の最小の子 s * p を探索する．
		if (s <= N / (p * p)) dfs(s * p, i_gpf, cnt + 1, nval);

		// その他の s の子を探索する．
		for (int i = i_gpf + 1; i < sz(ps) && s <= N / ((ll)ps[i] * ps[i]); i++) {
			T nval = mul(val, ps[i], 0);
			dfs(s * ps[i], i, 1, nval);
		}
	};
	dfs(1, 0, 0, 1);

	return res;

	/* mul, acc の定義の雛形
	using T = mint;
	auto mul = [&](T val, ll p, int cnt) {
		if (cnt == 0) {
			return val;
		}
		else {
			return val;
		}
	};

	int nl = (int)(sqrt(N) + 1e-9);
	Floor_vector<T> acc(N, nl);
	int nh = acc.nh;
	repi(i, 1, nl) acc.set_l(i, i);
	repi(d, 1, nh) acc.set_h(d, N / d);

	T res = Multiplicative_sum_mul<T>(mul, acc);
	*/
}


//【素数上の関数の累積和（一括）】O(N^(3/4))
/*
* 素数上で定義された関数 f を延長して完全乗法的数論関数とみなし，F をその累積和のリストとする．
* i 以下の素数 p をわたる f(p) の総和を S(i) とし，S[N/d] のリストを返す．
*
* 制約：nl = (int)(sqrt(N) + 1e-9)
*
* 利用：【素数の列挙】
*
*（Lucy DP）
*/
template <class T>
Floor_vector<T> func_on_prime_acc(const Floor_vector<T>& F) {
	// verify : https://judge.yosupo.jp/problem/sum_of_multiplicative_function

	ll N = F.N; int nl = F.nl, nh = F.nh;

	// dp_p[i] : [2..i] 内の p 以下の素数で篩い終えた後残っている数の f の総和
	Floor_vector<T> dp(N, nl);
	repi(i, 1, nl) dp.set_l(i, F.get_l(i) - 1);
	repi(d, 1, nh) dp.set_h(d, F.get_h(d) - 1);

	auto ps = eratosthenes(nl);

	repe(p, ps) {
		// sum : p 未満の素数をわたる f の総和
		T sum = dp.get_l(p - 1);

		// fp : f(p)
		T fp = F.get_l(p) - F.get_l(p - 1);

		repi(d, 1, nh) {
			// p^2 > N/d なら更新不要
			if (p > (N / d) / p) break;

			dp[N / d] -= fp * (dp.get_h(d * p) - sum);
		}

		repir(i, nl, 1) {
			// p^2 > i なら更新不要
			if (p > i / p) break;

			dp[i] -= fp * (dp.get_l(i / p) - sum);
		}
	}

	return dp;
}


//【[i=1] の累積和（一括）】O(nl + nh)
/*
* 1(s) をディリクレ母関数にもつ乗法的数論関数 f[i]=(i=1?1:0) について，f[1..N] の累積和を返す．
*/
template <class T>
Floor_vector<T> id_acc(ll N, int nl) {
	// verify : https://projecteuler.net/problem=439

	Floor_vector<T> Id(N, nl);
	int nh = Id.nh;

	repi(i, 1, nl) Id.set_l(i, 1);

	repi(d, 1, nh) Id.set_h(d, 1);

	return Id;
}


//【1 の累積和（一括）】O(nl + nh)
/*
* ζ(s) をディリクレ母関数にもつ乗法的数論関数 f[i]=1 について，f[1..N] の累積和を返す．
*/
template <class T>
Floor_vector<T> one_acc(ll N, int nl) {
	// verify : https://projecteuler.net/problem=448

	Floor_vector<T> Z(N, nl);
	int nh = Z.nh;

	repi(i, 1, nl) Z.set_l(i, i);

	repi(d, 1, nh) Z.set_h(d, N / d);

	return Z;
}


//【i の累積和（一括）】O(nl + nh)
/*
* ζ(s-1) をディリクレ母関数にもつ乗法的数論関数 f[i]=i について，f[1..N] の累積和を返す．
*/
template <class T>
Floor_vector<T> linear_acc(ll N, int nl) {
	// verify : https://projecteuler.net/problem=448

	Floor_vector<T> Z1(N, nl);
	int nh = Z1.nh;

	repi(i, 1, nl) Z1.set_l(i, (T)((ll)i * (i + 1) / 2));

	// 除算が要らないように Σ[1..i] = i(i+1)/2 を計算する．
	repi(d, 1, nh) {
		ll i = N / d;
		if (i & 1) {
			Z1.set_h(d, (i + 1) / 2 * (T)i);
		}
		else {
			Z1.set_h(d, i / 2 * (T)(i + 1));
		}
	}

	return Z1;
}


//【i^2 の累積和（一括）】O(nl + nh)
/*
* ζ(s-2) をディリクレ母関数にもつ乗法的数論関数 f[i]=i^2 について，f[1..N] の累積和を返す．
*/
template <class T>
Floor_vector<T> square_acc(ll N, int nl) {
	// verify : https://projecteuler.net/problem=448

	Floor_vector<T> Z2(N, nl);
	int nh = Z2.nh;

	// 除算が要らないように Σ[1..i]^2 = i(i+1)(2i+1)/6 を計算する．
	repi(i, 1, nl) {
		ll x = i, y = i + 1, z = 2 * i + 1;

		if (x & 1) y /= 2;
		else x /= 2;

		if (x % 3 == 0) x /= 3;
		else if (y % 3 == 0) y /= 3;
		else z /= 3;

		Z2.set_l(i, (T)x * y * z);
	}

	// 除算が要らないように Σ[1..i]^2 = i(i+1)(2i+1)/6 を計算する．
	repi(d, 1, nh) {
		ll i = N / d;
		ll x = i, y = i + 1, z = 2 * i + 1;

		if (x & 1) y /= 2;
		else x /= 2;

		if (x % 3 == 0) x /= 3;
		else if (y % 3 == 0) y /= 3;
		else z /= 3;

		Z2.set_h(d, (T)x * y * z);
	}

	return Z2;
}


//【i^3 の累積和（一括）】O(nl + nh)
/*
* ζ(s-3) をディリクレ母関数にもつ乗法的数論関数 f[i]=i^3 について，f[1..N] の累積和を返す．
*/
template <class T>
Floor_vector<T> cube_acc(ll N, int nl) {
	// verify : https://projecteuler.net/problem=715

	Floor_vector<T> Z3(N, nl);
	int nh = Z3.nh;

	// 除算が要らないように Σ[1..i]^3 = (i(i+1)/2)^2 を計算する．
	repi(i, 1, nl) {
		T val = (ll)i * (i + 1) / 2;
		val *= val;
		Z3.set_l(i, val);
	}

	// 除算が要らないように Σ[1..i]^3 = (i(i+1)/2)^2 を計算する．
	repi(d, 1, nh) {
		ll i = N / d; T val;
		if (i & 1) {
			val = (i + 1) / 2 * (T)i;
			val *= val;
		}
		else {
			val = i / 2 * (T)(i + 1);
			val *= val;
		}
		Z3.set_h(d, val);
	}

	return Z3;
}


//【オイラー関数の累積和（一括）】O(n^(2/3) log(log n)^(1/3))
/*
* φ[1..N] の累積和を返す．
*
* 利用：【ディリクレ畳込みの累積和（乗法的，一括）】
*/
template <class T>
Floor_vector<T> euler_phi_acc(ll N) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c
	// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

	//【方法】
	// オイラー関数 φ(i) と対応するディリクレ級数を Φ(s) とおくと，
	//		ζ(s) Φ(s) = ζ(s-1)
	// が成り立つ．
	// ζ(s) は乗法的数論関数 a[i] = 1 に対応するディリクレ級数であり，
	// ζ(s-1) は乗法的数論関数 c[i] = i に対応するディリクレ級数である．

	int nl = (int)max(N <= 30 ? 1. : pow(N / log(log(N)), 2. / 3), sqrt(N) + 1e-9);

	Floor_vector<T> Z(N, nl);
	int nh = Z.nh;
	repi(i, 1, nl) Z.set_l(i, i);
	repi(d, 1, nh) Z.set_h(d, N / d);

	Floor_vector<T> Z1(N, nl);
	repi(i, 1, nl) Z1.set_l(i, (T)((ll)i * (i + 1) / 2));
	repi(d, 1, nh) {
		ll i = N / d;
		if (i & 1) {
			Z1.set_h(d, (i + 1) / 2 * (T)i);
		}
		else {
			Z1.set_h(d, i / 2 * (T)(i + 1));
		}
	}

	Multiplicative_dirichlet_convolution_acc<T> M(N);
	return M.inv_conv_acc(Z, Z1);
}


//【メビウス関数の累積和（一括）】O(n^(2/3) log(log n)^(1/3))
/*
* μ[1..N] の累積和を返す．
*
* 利用：【ディリクレ畳込みの累積和（乗法的，一括）】
*/
Floor_vector<ll> mertens(ll N) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c
	// verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_i

	//【方法】
	// メビウス関数 μ(i) と対応するディリクレ級数を M(s) とおくと，
	//		ζ(s) M(s) = 1
	// が成り立つ．
	// ζ(s) は乗法的数論関数 a[i] = 1 に対応するディリクレ級数であり，
	// 1 は乗法的数論関数 c[i] = (i = 1 ? 1 : 0) に対応するディリクレ級数である．

	int nl = (int)max(N <= 30 ? 1. : pow(N / log(log(N)), 2. / 3), sqrt(N) + 1e-9);

	Floor_vector<ll> Id(N, nl);
	int nh = Id.nh;
	repi(i, 1, nl) Id.set_l(i, 1);
	repi(d, 1, nh) Id.set_h(d, 1);

	Floor_vector<ll> Z(N, nl);
	repi(i, 1, nl) Z.set_l(i, i);
	repi(d, 1, nh) Z.set_h(d, N / d);

	Multiplicative_dirichlet_convolution_acc<ll> M(N);
	return M.inv_conv_acc(Z, Id);
}


//【約数平方和関数の総和】O(√N)
/*
* Σi∈[1..N] σ_2(i) を返す．
*
* 利用：【1 の累積和（一括）】,【i^2 の累積和（一括）】,【ディリクレ畳込みの総和】
*/
mint divisor_square_sum(ll N) {
	// verify : https://projecteuler.net/problem=401

	//【方法】
	// 約数平方和関数 σ_2(i) と対応するディリクレ級数を C(s) とおくと，
	//		C(s) = ζ(s) ζ(s-2)
	// が成り立つ．
	// ζ(s) は乗法的数論関数 a[i] = 1 に対応するディリクレ級数であり，
	// ζ(s-2) は乗法的数論関数 b[i] = i^2 に対応するディリクレ級数である．

	int nl = (int)(sqrt(N) + 1e-9);

	auto Z = one_acc<mint>(N, nl);
	auto Z2 = square_acc<mint>(N, nl);

	return dirichlet_convolution_sum(Z, Z2);
}


//【素数の総和】O(N^(3/4))
/*
* N 以下の素数の総和を返す．
*
*（Lucy DP）
*/
template <class T>
T prime_sum(ll N) {
	// 参考 : https://rsk0315.github.io/slides/prime-counting.pdf

	//【方法】
	// j 番目（1-indexed）の素数を p[j](≦ √N) と表し，dp_j[i] を
	//		dp_j[i]
	//		= ([2..i] 内の "素数または p[j] 以下の素因数をもたない合成数" の総和)
	//		= (エラトステネスの篩において，[2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の総和)
	// とおく．dp_j[i] の求め方を考える．
	// 
	// p[j]^2 > i のときは，[2..i] 内には p[j] で新たに篩われる数は無いので
	//		dp_j[i] = dp_(j-1)[i]
	// である．
	// 
	// p[j]^2 ≦ i のときは，[2..i] 内の p[j] で新たに篩われる数は
	//		(i) p[j-1] 以下の素因数を持たない（まだ篩われていない）
	//		(ii) 2 p[j] 以上の p[j] の倍数（次に篩われる）
	// という条件を共に満たす数である．
	// 
	// [2..i] に条件 (i), (ii) を課す代わりに，全体を p[j] で割って，
	//		[2..i/p[j]] 内の p[j-1] 以下の素因数を持たない数
	// の和を求めてから p[j] 倍しても値は変わらない．そのような数は，[2..i/p[j]] 内の
	//		(iii) p[j-1] 以下の素数で篩い終えた後残っている
	//		(iv) p[j-1] 以下の素数ではない
	// という条件を共に満たす数であり，!(iv) ⇒ (iii) に注意するとその総和は
	//		dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]
	// と表される．
	//
	// 以上をまとめると，DP の初期化は
	//		dp_0[i] = i(i+1)/2 - 1
	// で行い，遷移式は
	//		dp_j[i] = dp_(j-1)[i] （p[j]^2 > i のとき）
	//		dp_j[i] = dp_(j-1)[i] - p[j] (dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]) （p[j]^2 ≦ i のとき）
	// を用いれば良い．

	//【備考】
	// 途中で DP テーブルを参照すれば，K-rough number の総和にも対応できる．
	// また d 乗和への一般化も容易である．

	if (N <= 1) return 0;

	int m = (int)(sqrt(N) + 1e-9);

	// dp_j[i] : [2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の総和
	//	dp_l_j[i] : dp_j[i]
	//	dp_h_j[d] : dp_j[N/d]
	vl dp_l(m + 1);
	vector<T> dp_h(m + 1);
	repi(i, 1, m) {
		dp_l[i] = (ll)i * (i + 1) / 2 - 1;

		ll j = N / i;
		if (j & 1) dp_h[i] = T(j) * ((j + 1) / 2) - 1;
		else dp_h[i] = T(j + 1) * (j / 2) - 1;
	}

	repi(p, 2, m) {
		// sum : p-1 以下の素数の総和
		ll sum = dp_l[p - 1];

		// p が素数でなければ次の p へ
		if (dp_l[p] == sum) continue;

		repi(d, 1, m) {
			// p^2 > N/d なら更新不要
			if (p > (N / d) / p) break;

			if (d <= m / p) dp_h[d] -= T(p) * (dp_h[d * p] - sum);
			else dp_h[d] -= T(p) * (dp_l[(N / d) / p] - sum);
		}

		repir(i, m, 1) {
			// p^2 > i なら更新不要
			if (p > i / p) break;

			dp_l[i] -= p * (dp_l[i / p] - sum);
		}
	}

	return dp_h[1];
}


//【無平方数の総和】O(N^(4/9))
/*
* N 以下の無平方数の総和を返す．
*
*（除原理）
*/
template <class T>
T square_free_sum(ll N) {
	int nl = (int)(pow(2. * N, 1. / 3) + 1e-9);
	int nh = (int)sqrt(N / (nl + 1) + 1);

	// isqrt[i] : floor(√i)
	vi isqrt(nl + 1);
	repi(i, 1, nl) isqrt[i] = (int)sqrtl((long double)i);

	// dp_l[i] : i 以下の無平方数の総和
	// dp_h[d] : N/d 以下の無平方数の総和
	vector<T> dp_l(nl + 1), dp_h(nh + 1);

	// ちゃんとやれば O(nl log(log nl)) でできるが，全体の計算量は変わらない．
	repi(i, 1, nl) {
		ll x = i;
		ll y = i + 1;
		if (x & 1) y /= 2;
		else x /= 2;

		dp_l[i] = T(x) * y;

		int m = (int)(pow(2. * i, 2. / 3) + 1e-9);

		int j_max = isqrt[m];
		int k_max = i / (m + 1);

		x = j_max, y = j_max + 1; ll z = 2 * j_max + 1;
		if (x & 1) y /= 2;
		else x /= 2;
		if (x % 3 == 0) x /= 3;
		else if (y % 3 == 0) y /= 3;
		else z /= 3;
		T j_max_2acc = T(x) * y * z;

		repi(j, 2, j_max) {
			dp_l[i] -= dp_l[i / j / j] * j * j;
		}

		repi(k, 1, k_max) {
			ll x = isqrt[i / k], y = isqrt[i / k] + 1, z = 2 * isqrt[i / k] + 1;
			if (x & 1) y /= 2;
			else x /= 2;
			if (x % 3 == 0) x /= 3;
			else if (y % 3 == 0) y /= 3;
			else z /= 3;
			T s2acc = T(x) * y * z;

			dp_l[i] -= (dp_l[k] - dp_l[k - 1]) * (s2acc - j_max_2acc);
		}
	}

	repir(d, nh, 1) {
		ll i = N / d / d;

		ll x = i;
		ll y = i + 1;
		if (x & 1) y /= 2;
		else x /= 2;

		dp_h[d] = T(x) * y;

		ll m = (ll)(pow(2. * i, 2. / 3) + 1e-9);

		int j_max = (int)sqrtl((long double)m);
		int k_max = (int)(i / (m + 1));

		x = j_max, y = j_max + 1; ll z = 2 * j_max + 1;
		if (x & 1) y /= 2;
		else x /= 2;
		if (x % 3 == 0) x /= 3;
		else if (y % 3 == 0) y /= 3;
		else z /= 3;
		T j_max_2acc = T(x) * y * z;

		repi(j, 2, j_max) {
			if (i / j / j <= nl) {
				dp_h[d] -= dp_l[i / j / j] * j * j;
			}
			else {
				dp_h[d] -= dp_h[d * j] * j * j;
			}
		}

		repi(k, 1, k_max) {
			int s = (int)sqrtl((long double)(i / k));

			ll x = s, y = s + 1, z = 2 * s + 1;
			if (x & 1) y /= 2;
			else x /= 2;
			if (x % 3 == 0) x /= 3;
			else if (y % 3 == 0) y /= 3;
			else z /= 3;
			T s2acc = T(x) * y * z;

			dp_h[d] -= (dp_l[k] - dp_l[k - 1]) * (s2acc - j_max_2acc);
		}
	}

	return dp_h[1];
}


//【乗法的数論関数の総和（f(p) = 0）】O(√N)
/*
* f(p) = 0 なる乗法的数論関数 f についての S(N) = Σi∈[1..N] f(i) を求めたいとする．
* f(n) ≠ 0 なる n は O(√N) 個の多冪数（p|n ⇒ p^2|n を満たす数）に限られるので，
* f(n) の計算が O(β) でできる場合，O(√N β) で S(N) を求められる．
*/


//【乗法的数論関数の総和（f(p) = 1）】O(√N)
/*
* f(p) = 1 なる乗法的数論関数 f についての S(N) = Σi∈[1..N] f(i) を求めたいとする．
* f をメビウス反転した乗法的数論関数を g(n) = Σ_d|n μ(d) f(n/d) とすると，g は g(p) = 0 を満たす．
* g(n) ≠ 0 なる n は O(√N) 個の多冪数（p|n ⇒ p^2|n を満たす数）に限られるので，
* g(n) の計算が O(β) でできる場合，尺取法を用いて O(√N β) で Σi∈[1..N/d] g(i) を列挙できる．
* よって【ディリクレ畳込みの総和】を用いれば S(N) を求められる．
*/


//【floor(n/i) μ(i) の総和 → 1】
/*
* Σi∈[1..n] floor(n/i) μ(i) = 1
* 
*（証明）floor(n/i) を [1..n] 内の i の倍数の個数と解釈して和の順序交換を行うと，
*	Σi∈[1..n] floor(n/i) μ(i)
*	= Σi∈[1..n] Σj=ik∈[1..n] μ(i)
*	= Σj∈[1..n] Σi|j μ(i)
*	= Σj∈[1..n] Boole[j=1]  （恒等関数 1 のメビウス反転）
*	= 1
*
* verify : https://yukicoder.me/problems/no/1781
*/


//【商列挙の数式表現】
/*
* Σij≦N f(i) = Σq∈{N/.} q Σi∈(N/(q+1)..N/q] f(i) （商は切り捨て）
* 
*（証明）
*	Σij≦N f(i)
*	= Σi∈[1..N] Σj∈[1..N/i] f(i)
*	= Σi∈[1..N] (N/i) f(i)
*	= Σq∈{N/.} Σi:N/i=q q f(i)
*	= Σq∈{N/.} q Σi∈(N/(q+1)..N/q] f(i)
* 
* verify : https://yukicoder.me/problems/no/1781
*/


//【商列挙とメビウス反転】
/*
* Σq∈{N/.} q Σi∈(N/(q+1)..N/q] Σd|i μ(i/d) f(d) = Σi∈[1..N] f(i) （商は切り捨て）
* 
*（証明）
*	Σq∈{N/.} q Σi∈(N/(q+1)..N/q] Σd|i μ(i/d) f(d)
*	= Σij≦N Σd|i μ(i/d) f(d)  （【商列挙の数式表現】より）
*	= Σd∈[1..N] Σi=dk∈[1..N] Σj∈[1..N/i] μ(i/d) f(d)
*	= Σd∈[1..N] Σk∈[1..N/d] Σj∈[1..N/dk] μ(k) f(d)
*	= Σd∈[1..N] f(d) Σk∈[1..N/d] (N/dk) μ(k)
*	= Σd∈[1..N] f(d) 1  （【floor(n/i) μ(i) の総和 → 1】より）
*	= Σi∈[1..N] f(i)
*
* verify : https://yukicoder.me/problems/no/1781
*/


//【余りの和】（遅い）
/*
* Mod_sum(vl a) : O(n log n)
*	整数列 a[0..n) で初期化する．
*
* ll mod_sum(ll m) : O(max(a) log(n) / m)
*	Σi∈[0..n) (a[i] mod m) を返す．
*
* ll lack_sum(ll m) : O(max(a) log(n) / m)
*	a[0..n) を m で割った不足の和を返す．
*
* ll floor_sum(ll m) : O(max(a) log(n) / m)
*	Σi∈[0..n) floor(a[i] / m) を返す．
*
* ll ceil_sum(ll m) : O(max(a) log(n) / m)
*	Σi∈[0..n) ceil(a[i] / m) を返す．
*/
template <class T>
class Mod_sum {
	int n;
	vector<T> a;
	vl acc;

public:
	// 整数列 a[0..n) で初期化する．
	Mod_sum(const vector<T>& a_) : n(sz(a_)), a(a_), acc(n + 1) {
		// verify : https://atcoder.jp/contests/arc126/tasks/arc126_c

		sort(all(a));
		rep(i, n) acc[i + 1] = acc[i] + a[i];
	}
	Mod_sum() : n(0) {}

	// Σi∈[0..n) (a[i] mod m) を返す．
	ll mod_sum(T m) const {
		ll res = 0;

		T v = a[0] - smod(a[0], m);
		int pi = 0;

		while (pi < n) {
			int i = lbpos(a, v + m);
			res += (acc[i] - acc[pi]) - (ll)v * (i - pi);

			v += m;
			pi = i;
		}

		return res;
	}

	// a[0..n) を m で割った不足の和を返す．
	ll lack_sum(T m) const {
		// verify : https://atcoder.jp/contests/arc126/tasks/arc126_c

		ll res = 0;

		T v = a[0] + smod(-a[0], m);
		int pi = 0;

		while (pi < n) {
			int i = ubpos(a, v);
			res += (ll)v * (i - pi) - (acc[i] - acc[pi]);

			v += m;
			pi = i;
		}

		return res;
	}

	// Σi∈[0..n) floor(a[i] / m) を返す．
	ll floor_sum(T m) const {
		return (acc[n] - mod_sum(m)) / m;
	}

	// Σi∈[0..n) ceil(a[i] / m) を返す．
	ll ceil_sum(T m) const {
		return (acc[n] + lack_sum(m)) / m;
	}
};


