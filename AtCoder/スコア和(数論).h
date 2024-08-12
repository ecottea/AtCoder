#pragma once
#include "header.h"
#include "ディリクレ畳込み.h"
// ■■■■■ スコア和（数論） ■■■■■


//【GPF 木上の DP】O(n^(3/4) / log n)
/*
* 与えられた乗法的数論関数 f について，Σi∈[1..n] f(i) を返す．
*	i が素因数 p を cnt 個含んでおり f(i) = val のとき，mul(val, p, cnt) は f(i p) を返す．
*	sum_l[p] は Σi∈[1..p],i:素数 f(i) を返す．
*	sum_h[d] は Σi∈[1..n/d],i:素数 f(i) を返す．
*
* 利用：【素数の列挙】
*/
template <class T, class FUNC1, class FUNC2, class FUNC3>
T gpf_tree_DP(ll n, const FUNC1& mul, const FUNC2& sum_l, const FUNC3& sum_h) {
	// verify : https://projecteuler.net/problem=715

	if (n <= 0) return 0;
	if (n == 1) return 1;
	if (n == 2) return 1 + mul(1, 2, 0);
	if (n == 3) return 1 + mul(1, 2, 0) + mul(1, 3, 0);

	// ps : √n 以下の素数の昇順リスト
	auto ps = eratosthenes((int)(sqrt(n) + 1e-6));

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
		res += val * (sum_h(s) - sum_l(p));

		// s の最小の子 s * p を探索する．
		if (s <= n / (p * p)) dfs(s * p, i_gpf, cnt + 1, nval);

		// その他の s の子を探索する．
		for (int i = i_gpf + 1; i < sz(ps) && s <= n / ((ll)ps[i] * ps[i]); i++) {
			T nval = mul(val, ps[i], 0);
			dfs(s * ps[i], i, 1, nval);
		}
	};
	dfs(1, 0, 0, 1);

	return res;

	/* mul, sum_l, sum_h の定義の雛形
	using T = mint;
	int sqrt_N = (int)(sqrt(N) + 1e-6);
	auto mul = [&](T val, ll p, int cnt) {
		if (cnt == 0) {
			retrun val;
		}
		else {
			return val;
		}
	};
	auto sum_l = [&](int p) {
		return dp_l[p];
	};
	auto sum_h = [&](ll i) {
		if (i <= sqrt_N) {
			return dp_h[i];
		}
		else {
			return dp_l[N / i];
		}
	};
	T res = gpf_tree_DP<T>(N, mul, sum_l, sum_h);
	*/
}


//【素数の総和】O(n^(3/4))
/*
* n 以下の素数の総和を返す．
*
*（Lucy DP）
*/
mint prime_sum(ll n) {
	// 参考 : https://rsk0315.github.io/slides/prime-counting.pdf

	//【方法】
	// S(v, p) = ([2..v] 内の "素数または p 以下の素因数をもたない合成数" の和) とおく．
	// これはエラトステネスの篩において，p 以下の素数で篩い終えた後残っている数の和である．
	//
	// p が合成数であるときと p^2 > v であるときは，篩のアルゴリズムより明らかに
	//		S(v, p) = S(v, p-1)
	// である．よって p が p^2 <= v なる素数の場合の遷移のみを考えれば良い．
	// 
	// p の 1 つ前の素数で篩い終わったときに残っている数の和は S(v, p-1) である．
	// これらの数のうち p で篩われて消えるものの和を考える．
	// そのような数は，[2..v] 内の
	//		(i) p より小さい素因数を持たない（まだ篩われていない）
	//		(ii) p より大きい p の倍数（次に篩われる）
	// という条件を共に満たす数である．
	// 
	// [2..v] に条件 (i), (ii) を課す代わりに，全体を p で割った
	//		[2..v/p] 内の p より小さい素因数を持たない数
	// の和を求めて p 倍しても値は変わらない．そのような数は，[2..v/p] 内の
	//		(iii) p-1 以下の素因数で篩い終えた後残っている
	//		(iv) p-1 以下の素数ではない
	// という条件を共に満たす数である．
	//
	// いま p^2 ≦ v なる場合を考えているので，p ≦ v/p である．
	// よって !(iv) ⇒ (iii) であるから，求める和は
	//		S(v/p, p-1) - S(p-1, p-1)
	// と表される．
	//
	// 以上をまとめて，DP の遷移式
	//		S(v, p) = S(v, p-1) （p が合成数または p^2 > v のとき）
	//		S(v, p) = S(v, p-1) - p (S(v/p, p-1) - S(p-1, p-1)) （その他の p のとき）
	// を得る．初項は
	//		S(v, 1) = v(v+1)/2 - 1
	// であり，求めたいものは
	//		S(n, √n)
	// である．

	//【備考】
	// 途中で DP テーブルを参照すれば，k-rough number の総和にも対応できる．

	if (n <= 1) return 0;

	int m = (int)(sqrt(n) + 1e-6);

	// S(v, p) : [2..v] 内の p 以下の素数で篩い終えた後残っている数の和
	//	dp0_p[v] : S(v, p)
	//	dp1_p[v] : S(n/v, p)
	vl dp0(m + 1);
	vm dp1(m + 1); mint inv2 = mint(2).inv();
	repi(v, 1, m) {
		dp0[v] = (ll)v * (v + 1) / 2 - 1;
		dp1[v] = mint(n / v) * (n / v + 1) * inv2 - 1;
	}

	repi(p, 2, m) {
		// S(p - 1, p - 1)
		ll s = dp0[p - 1];

		// p が素数でなければ次の p へ
		if (dp0[p] == s) continue;

		// dp1[v] = S(n/v, p) の更新
		repi(v, 1, m) {
			// p^2 > n/v なら更新不要
			if (p > n / v / p) break;

			if (v <= m / p) dp1[v] -= p * (dp1[v * p] - s);
			else dp1[v] -= p * (dp0[n / v / p] - s);
		}

		// dp0[v] = S(v, p) の更新
		repir(v, m, 1) {
			// p^2 > v なら更新不要
			if (p > v / p) break;

			dp0[v] -= p * (dp0[v / p] - s);
		}
	}

	return dp1[1];
}


//【約数平方和関数の総和】O(√n)
/*
* Σi∈[1..n] σ_2(i) を返す．
*
* 利用：【ディリクレ畳込みの累積和】
*/
mint divisor_square_sum(ll n) {
	// verify : https://projecteuler.net/problem=401

	//【備考】
	// 平方和以外にも容易に変更可能

	int m = (int)(sqrt(n) + 1e-6);

	// a[i] = 1（定数）, b[j] = j^2 とする．
	//	al[i] = a[i],       bl[j] = b[j]
	//	Ah[i] = Σa[1..n/i], Bh[j] = Σb[1..n/j]
	vm al(m + 1), bl(m + 1), Ah(m + 1), Bh(m + 1);

	repi(i, 1, m) {
		al[i] = 1;
		bl[i] = mint(i) * i;

		ll j = n / i;
		Ah[i] = j;

		// 除算が要らないように Σt∈[1..j] t^2 = j(j+1)(2j+1)/6 を計算する．
		ll x = j, y = j + 1, z = 2 * j + 1;
		if (x & 1) y /= 2;
		else x /= 2;
		if (x % 3 == 0) x /= 3;
		else if (y % 3 == 0) y /= 3;
		else z /= 3;
		Bh[i] = mint(x) * y * z;
	}

	return dirichlet_convolution_acc(al, Ah, bl, Bh);
}


//【無平方数の総和】O(n^0.45) ?
/*
* n 以下の無平方数の総和を返す．
*/
constexpr ll W = 2000000;
ll dp_l[W], dp_h[W];
ll square_free_sum(ll n) {
	//【注意】
	// 境界の決め方がかなり雑なので信用してはいけない．
	// あと余裕でオーバーフローする．

	rep(i, W) dp_l[i] = dp_h[i] = -1;

	ll N = n;

	function<ll(int)> rf_l = [&](int n) {
		if (dp_l[n] != -1) return dp_l[n];

		ll res = n * (n + 1) / 2;

		int m = (int)(pow(n * 0.5, 1. / 3) + 1e-9);

		int i_max = (int)sqrt(1. * n / (m + 1));

		// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
		for (int i = 2; i <= i_max; i++) res -= rf_l(n / (i * i)) * i * i;

		// そうでない部分は q ごとにまとめて考える．
		int il, ir = i_max + 1;
		repir(q, m, 1) {
			il = ir;
			ir = (int)sqrt(1. * n / q) + 1;
			if (il != ir) res -= rf_l(q) * (ir * (ir - 1) * (2 * ir - 1) / 6 - il * (il - 1) * (2 * il - 1) / 6);
		}

		return dp_l[n] = res;
	};

	function<ll(int)> rf_h = [&](int I) {
		if (dp_h[I] != -1) return dp_h[I];

		ll n = N / ((ll)I * I);

		ll res = n * (n + 1) / 2;

		ll m = (ll)(pow(n * 0.5, 1. / 3));

		ll i_max = (ll)sqrtl(1.L * n / (m + 1));

		// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
		for (ll i = 2; i <= i_max; i++) {
			ll q = n / (i * i);
			if (q < W) res -= rf_l((int)q) * i * i;
			else res -= rf_h(i * I) * i * i;
		}

		// そうでない部分は q ごとにまとめて考える．
		ll il, ir = i_max + 1;
		repir(q, m, 1) {
			il = ir;
			ir = (ll)sqrtl(1.L * n / q) + 1;
			if (il != ir) {
				if (q < W) res -= rf_l((int)q) * (ir * (ir - 1) * (2 * ir - 1) / 6 - il * (il - 1) * (2 * il - 1) / 6);
				else res -= rf_h(il * I) * (ir * (ir - 1) * (2 * ir - 1) / 6 - il * (il - 1) * (2 * il - 1) / 6);
			}
		}

		return dp_h[I] = res;
	};

	ll res = (n < W ? rf_l(n) : rf_h(1));

	return res;
}


//【オイラー関数の累積和（一括）】O(n^(2/3) log(log n)^(1/3))
/*
* 各 i∈[1..nl] について bl[i] = φ(i) を，
* 各 i∈[1..nh] について Bh[i] = Σj∈[1..n/i] φ(j) をそれぞれ格納する．
*
* 制約：nh ≦ nl ≦ n ≦ nl nh
*
* 利用：【ディリクレ畳込みの累積和（乗法的，mint，一括）】
*/
void euler_phi_sum(ll n, int nl, int nh, vm& bl, vm& Bh) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c
	// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

	//【方法】
	// オイラー関数 φ(i) と対応するディリクレ級数を Φ(s) とおくと，
	//		ζ(s) Φ(s) = ζ(s-1)
	// が成り立つ．
	// ζ(s) は乗法的数論関数 a[i] = 1 に対応するディリクレ級数であり，
	// ζ(s-1) は数論関数 c[i] = i に対応するディリクレ級数である．

	if (nl <= 0 || nh <= 0) return;
	Multiplicative_dirichlet_convolution_acc_mint M(nl);

	vm al(nl + 1), cl(nl + 1), Ah(nh + 1), Ch(nh + 1); mint inv2 = mint(2).inv();
	repi(i, 1, nl) {
		al[i] = 1;
		cl[i] = i;
	}
	repi(i, 1, nh) {
		Ah[i] = n / i;
		Ch[i] = mint(n / i) * (n / i + 1) * inv2;
	}

	M.inv_conv_acc(n, al, Ah, cl, Ch, bl, Bh);
}


//【メビウス関数の累積和（一括）】O(n^(2/3) log(log n)^(1/3))
/*
* 各 i∈[1..nl] について bl[i] = μ(i) を，
* 各 i∈[1..nh] について Bh[i] = Σj∈[1..n/i] μ(j) をそれぞれ格納する．
*
* 制約：nh ≦ nl ≦ n ≦ nl nh
*
* 利用：【ディリクレ畳込みの累積和（乗法的，一括）】
*/
void mertens(ll n, int nl, int nh, vl& bl, vl& Bh) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c
	// verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_i

	//【方法】
	// メビウス関数 μ(i) と対応するディリクレ級数を M(s) とおくと，
	//		ζ(s) M(s) = 1
	// が成り立つ．
	// ζ(s) は乗法的数論関数 a[i] = 1 に対応するディリクレ級数であり，
	// 1 は数論関数 c[i] = (i = 1 ? 1 : 0) に対応するディリクレ級数である．

	if (nl <= 0 || nh <= 0) return;
	Multiplicative_dirichlet_convolution_acc<ll> M(nl);

	vl al(nl + 1, 1), cl(nl + 1), Ah(nh + 1), Ch(nh + 1, 1);
	cl[1] = 1;
	repi(i, 1, nh) Ah[i] = n / i;

	M.inv_conv_acc(n, al, Ah, cl, Ch, bl, Bh);
}


//【乗法的数論関数の総和（f(p) = 0）】O(√N)
/*
* f(p) = 0 なる乗法的数論関数 f についての S(N) = Σi∈[1..N] f(i) を求めたいとする．
* f(n) != 0 なる n は O(√N) 個の多冪数（p|n ⇒ p^2|n を満たす数）に限られるので，
* f(n) の計算が O(β) でできる場合，O(√N β) で S(N) を求められる．
*/


//【乗法的数論関数の総和（f(p) = 1）】O(√N)
/*
* f(p) = 1 なる乗法的数論関数 f についての S(N) = Σi∈[1..N] f(i) を求めたいとする．
* f をメビウス反転した乗法的数論関数を g(n) = Σ_d|n μ(d) f(n/d) とすると，g は g(p) = 0 を満たす．
* g(n) != 0 なる n は O(√N) 個の多冪数（p|n ⇒ p^2|n を満たす数）に限られるので，
* g(n) の計算が O(β) でできる場合，尺取法を用いて O(√N β) で Σi∈[1..N/d] g(i) を列挙できる．
* よって【ディリクレ畳込みの累積和】を用いれば S(N) を求められる．
*/


//【オイラー関数の総和】O(n^(3/4) / log n)（遅い）
/*
* Σi∈[1..n] φ(i) を返す．
*/
mint euler_phi_sum(ll n) {
	// 参考 : https://rsk0315.github.io/slides/prime-counting.pdf
	// verify : https://judge.yosupo.jp/problem/sum_of_totient_function

	//【方法】
	// 自然数 i の最大素因数を gpf(i) と表す．
	// 頂点 [1..n] をもち，i の親が i / gpf(i) である木 T を考える．（根は 1） 
	// T はほとんどが葉であるという性質をもつ．
	// 葉でない各節点 i について Σj∈(iの子) φ(j) を求められれば，(それらの総和) + 1 が求める値である．
	//
	// 例えば n = 40 のときの i = 2 を考えると，その子は gpf(2)=2 以上の素数を 2 に乗じた
	//		4, 6, 10, 14, 22, 26, 34, 38
	// である．これらに φ を施した値の総和は，φ の乗法性より
	//		Σj∈(iの子) φ(j)
	//		= φ(4) + φ(2) (φ(3) + φ(5) + φ(7) + φ(11) + φ(13) + φ(17) + φ(19))
	//		= φ(4) + φ(2) (3-1 + 5-1 + 7-1 + 11-1 + 13-1 + 17-1 + 19-1)
	//		= φ(4) + φ(2) (([3..n/2] 内の素数の総和) - ([3..n/2] 内の素数の個数))
	// として求められるので，素数の総和と素数の個数を前計算で求めておけば良い．
	//
	// またこの場合 i * 5^2 > n となるので，10 以上の子は全て葉であることが探索しなくても分かる．
	// T はほとんどが葉なので，葉のみの枝刈りとはいえ真に計算量が改善する．

	if (n <= 1) return max(n, 0LL);

	int m = (int)(sqrt(n) + 1e-6);

	// 1 と素数の昇順リスト
	vl ps{ 1 };

	// cnt0_p[v] : [2..v] 内の p 以下の素数で篩い終えた後残っている数の個数
	// cnt1_p[v] : [2..n/v] 内の p 以下の素数で篩い終えた後残っている数の個数
	vl cnt0(m + 1), cnt1(m + 1);

	// sum0_p[v] : [2..v] 内の p 以下の素数で篩い終えた後残っている数の和
	// sum1_p[v] : [2..n/v] 内の p 以下の素数で篩い終えた後残っている数の和
	vm sum0(m + 1), sum1(m + 1);

	repi(v, 1, m) {
		cnt0[v] = v - 1;
		cnt1[v] = n / v - 1;
		sum0[v] = mint(v) * (v + 1) / 2 - 1;
		sum1[v] = mint(n / v) * (n / v + 1) / 2 - 1;
	}

	repi(p, 2, m) {
		ll c = cnt0[p - 1];
		mint s = sum0[p - 1];

		// p が素数でなければ次の p へ
		if (cnt0[p] == c) continue;
		ps.push_back(p);

		// cnt1, sum1 の更新
		repi(v, 1, m) {
			// p^2 > n/v なら更新不要
			if (p > n / v / p) break;

			if (v <= m / p) {
				cnt1[v] -= cnt1[v * p] - c;
				sum1[v] -= p * (sum1[v * p] - s);
			}
			else {
				cnt1[v] -= cnt0[n / v / p] - c;
				sum1[v] -= p * (sum0[n / v / p] - s);
			}
		}

		// cnt0, sum0 の更新
		repir(v, m, 1) {
			// p^2 > v なら更新不要
			if (p > v / p) break;

			cnt0[v] -= cnt0[v / p] - c;
			sum0[v] -= p * (sum0[v / p] - s);
		}
	}

	mint res = 1;

	// s : 注目頂点, i_gpf : s の最大素因数が何番目の素数か, phi : φ(s)
	function<void(ll, int, mint)> dfs = [&](ll s, int i_gpf, mint phi) {
		ll p = ps[i_gpf];

		// s の最小の子 s * p からの寄与を加算する．
		if (s != 1) res += phi * p;

		// その他の s の子からの寄与をまとめて加算する．
		if (s <= m)	res += phi * ((sum1[s] - cnt1[s]) - (sum0[p] - cnt0[p]));
		else res += phi * ((sum0[n / s] - cnt0[n / s]) - (sum0[p] - cnt0[p]));

		// s の最小の子 s * p を探索する．
		if (s != 1 && s <= n / (p * p)) dfs(s * p, i_gpf, phi * p);

		// その他の s の子を探索する．
		for (int i = i_gpf + 1; i < sz(ps) && s <= n / (ps[i] * ps[i]); i++) {
			dfs(s * ps[i], i, phi * (ps[i] - 1));
		}
	};

	dfs(1, 0, 1);

	return res;
}


//【約数個数関数の総和】O(n^(3/4) / log n)（遅い）
/*
* Σi∈[1..n] σ_0(i) を返す．
*/
mint divisor_count_sum(ll n) {
	//【方法】
	// 自然数 i の最大素因数を gpf(i) と表す．
	// 頂点 [1..n] をもち，i の親が i / gpf(i) である木 T を考える．（根は 1） 
	// T はほとんどが葉であるという性質をもつ．
	// 葉でない各節点 i について Σj∈(iの子) σ_0(j) を求められれば，(それらの総和) + 1 が求める値である．
	//
	// 例えば n = 40 のときの i = 2 を考えると，その子は
	//		4, 6, 10, 14, 22, 26, 34, 38
	// である．これらに σ_0 を施した値の総和は，σ_0 の乗法性より
	//		Σj∈(iの子) σ_0(j)
	//		= σ_0(4) + σ_0(2) (σ_0(3) + σ_0(5) + σ_0(7) + σ_0(11) + σ_0(13) + σ_0(17) + σ_0(19))
	//		= σ_0(4) + σ_0(2) (3^0+1 + 5^0+1 + 7^0+1 + 11^0+1 + 13^0+1 + 17^0+1 + 19^0+1)
	//		= σ_0(4) + σ_0(2) (2 * ([3..n/2] 内の素数の個数))
	// として求められるので，素数の個数を前計算で求めておけば良い．
	//
	// またこの場合 i * 5^2 > n となるので，10 以上の子は全て葉であることが探索しなくても分かる．
	// T はほとんどが葉なので，葉のみの枝刈りとはいえ真に計算量が改善する．

	//【備考】
	// Σi∈[1..n] σ_0(i) = Σi∈[1..n] floor(n/i) なので，商列挙を使えば O(√n) でより高速．

	if (n <= 1) return max(n, 0LL);

	int m = (int)(sqrt(n) + 1e-6);

	// inv[i] : i の逆数
	vm inv(msb(n) + 2);
	repi(i, 1, sz(inv) - 1) inv[i] = mint(i).inv();

	// 1 と素数の昇順リスト
	vl ps{ 1 };

	// cnt0_p[v] : [2..v] 内の p 以下の素数で篩い終えた後残っている数の個数
	// cnt1_p[v] : [2..n/v] 内の p 以下の素数で篩い終えた後残っている数の個数
	vl cnt0(m + 1), cnt1(m + 1);

	repi(v, 1, m) {
		cnt0[v] = v - 1;
		cnt1[v] = n / v - 1;
	}

	repi(p, 2, m) {
		ll c = cnt0[p - 1];

		// p が素数でなければ次の p へ
		if (cnt0[p] == c) continue;
		ps.push_back(p);

		// cnt1 の更新
		repi(v, 1, m) {
			// p^2 > n/v なら更新不要
			if (p > n / v / p) break;

			if (v <= m / p) {
				cnt1[v] -= cnt1[v * p] - c;
			}
			else {
				cnt1[v] -= cnt0[n / v / p] - c;
			}
		}

		// cnt0 の更新
		repir(v, m, 1) {
			// p^2 > v なら更新不要
			if (p > v / p) break;

			cnt0[v] -= cnt0[v / p] - c;
		}
	}

	mint res = 1;

	// s     : 注目頂点
	// i_gpf : s の最大素因数が何番目の素数か
	// sg    : σ_0(s)
	// c     : s の最大素因数の指数
	function<void(ll, int, mint, int)> dfs = [&](ll s, int i_gpf, mint sg, int c) {
		ll p = ps[i_gpf];

		// s の最小の子 s * p からの寄与を加算する．
		if (s != 1) res += sg * inv[c + 1] * (c + 2);

		// その他の s の子からの寄与をまとめて加算する．
		if (s <= m)	res += sg * ((2 * cnt1[s]) - (2 * cnt0[p]));
		else res += sg * ((2 * cnt0[n / s]) - (2 * cnt0[p]));

		// s の最小の子 s * p を探索する．
		if (s != 1 && s <= n / (p * p)) dfs(s * p, i_gpf, sg * inv[c + 1] * (c + 2), c + 1);

		// その他の s の子を探索する．
		for (int i = i_gpf + 1; i < sz(ps) && s <= n / (ps[i] * ps[i]); i++) {
			dfs(s * ps[i], i, sg * 2, 1);
		}
	};

	dfs(1, 0, 1, 0);

	return res;
}


