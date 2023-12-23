#pragma once
#include "header.h"
#include "mint.h"
// ■■■■■ 高速な数論アルゴリズム ■■■■■


//【素数判定】O((log n)^3)
/*
* n が素数かを返す．
* 
* verify : https://atcoder.jp/contests/abc263/tasks/abc263_g
*/
//internal::is_prime_constexpr(int n);


//【素数判定】O((log n)^3)
/*
* n が素数かを返す．
*
* 利用：【有限体 F_p 上の計算（64 bit）】
*/
bool miller_rabin(ll n) {
	// 参考 : https://nyaannyaan.github.io/library/prime/fast-factorize.hpp.html
	// verify : https://judge.yosupo.jp/problem/primality_test

	//【方法】
	// p を奇素数とすると，任意の a∈[1..p) についてフェルマーの小定理より
	//		a^(p-1) ≡ 1 (mod p)
	// となる．これの平方根を考えていくと，
	//		p-1 = 2^s d　（d : 奇数）
	// と表せば，
	//		a^d ≡ 1 (mod p) or ∃r=[0..s), a^(2^r d) ≡ -1 (mod p)
	// と書き直せる．
	// 
	// この対偶を用いて判定することをランダムに選んだ a で繰り返す．
	// n < 2^64 に範囲を限定するなら擬素数を生じない a を固定的に選べる．

	const vl as = { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 };

	if (n == 2 || n == 3 || n == 5 || n == 13 || n == 19 || n == 73 || n == 193
		|| n == 407521 || n == 299210837) return true;
	if (n == 1 || n % 2 == 0) return false;

	mll::set_mod(n);
	int s = 0; ll d = n - 1;
	while (d % 2 == 0) {
		s++;
		d /= 2;
	}

	repe(a, as) {
		mll powa = mll(a).pow(d);
		if (powa == 1 || powa == -1) goto LOOP_END;
		rep(r, s - 1) {
			powa *= powa;
			if (powa == 1) return false;
			if (powa == -1) goto LOOP_END;
		}
		return false;

	LOOP_END:;
	}

	return true;
}


//【約数検出】O(n^(1/4))
/*
* n の真の約数を何か 1 つ返す（なければ n を返す）
*
* 利用：【有限体 F_p 上の計算（64 bit）】
*/
template <class T = ll>
T pollard_rho(T n) {
	// 参考 : https://qiita.com/Kiri8128/items/eca965fe86ea5f4cbb98
	// verify : https://judge.yosupo.jp/problem/factorize

	//【方法】
	// 適当な定数 c をとり関数 f : Z/nZ → Z/nZ を
	//		f(x) = x^2 + c
	// と定める．
	//
	// 適当な初期値 x[0] = y[0] (= 2) から始め，Z/nZ 上の数列を漸化式
	//		x[i+1] = f(x[i]), y[i+1] = f(f(y[i]))
	// で定める．フロイドの循環検出法より，もし
	//		gcd(x[i] - y[i], n) = g ∈ [2..n-1]
	// であれば，これは f が Z/gZ（g は n の真の約数）で巡回したことを意味する．
	//
	// 実際には，
	//		x は r = (2 冪) 個ずつ進める（定数 1/2 倍）
	//		gcd の計算を m = n^(1/8) 程度個まとめて行う（gcd の log を落とす）
	// ことにより高速化を図る．

	if (!(n & 1)) return 2;

	int m = 1 << (msb(n) / 8);
	mll::set_mod(n); // n は合成数だが割り算は使わないので問題ない

	const int c_max = 99; // c を最大どこまで試すか
	repi(c, 1, c_max) {
		auto f = [&](mll x) { return x * x + c; };

		mll x, y = 2, y_bak;
		T g = 1;
		int r = 1;

		// g = 1 である間は巡回未検出
		while (g == 1) {
			// x, y を r = 2^i だけ一気に進める．
			x = y;
			rep(hoge, r) y = f(y);

			// 次の r = 2^i 個をまとめて見る．
			for (int k = 0; k < r; k += m) {
				// 一気に掛けすぎて g = n となってしまった場合の復元用
				y_bak = y;

				// m 個ごとにまとめて見る．
				mll mul = 1;
				rep(i, min(m, r - k)) {
					y = f(y);

					// 複数個掛けておき，後でまとめて gcd を計算する．
					//（フロイドの循環検出法とは違い x を固定しているが，
					// 巡回は検出できるので問題ない．）
					mul *= x - y;
				}
				g = (T)gcd(mul.val(), (ll)n);

				// g != 1 なら巡回を検出できたので次の処理へ
				if (g != 1) goto LOOP_END;
			}

			r *= 2;
		}

	LOOP_END:;
		// 一気に掛けすぎて g = n となってしまった（であろう）場合
		if (g == n) {
			// 復元用に残しておいた x, y_bak から再スタート
			g = 1;
			while (g == 1) {
				y_bak = f(y_bak);
				g = (T)gcd((x - y_bak).val(), (ll)n);
			}
		}

		// g < n なら g が n の真の約数なのでそれを返す．
		if (g < n) return g;

		// 本当に g = n ならたまたま真の約数が全て同時検出されてしまったので，
		// 関数 f における定数項 c の値を別のものに取り替えて再挑戦．
	}

	// 複数個の c を試してなお失敗したなら諦める．
	return n;
}


//【素因数分解】O(n^(1/4))
/*
* n を素因数分解した結果を pps に格納し pps を返す．
* pps[p] = d : n に素因数 p が d 個含まれていることを表す．
*
* 利用：【素数判定】,【約数検出】
*/
template <class T = ll>
map<T, int> factor_integer(T n) {
	// verify : https://judge.yosupo.jp/problem/factorize

	map<T, int> pps;
	if (n == 1) return map<T, int>();

	// 検出した約数を記録しておくキュー
	queue<T> divs;
	divs.push(n);

	while (!divs.empty()) {
		T d = divs.front();
		divs.pop();

		// 約数が素数なら素因数発見
		if (miller_rabin(d)) {
			pps[d]++;
		}
		// 約数が合成数なら新たな約数を 2 つ発見する
		else {
			T d1 = pollard_rho<T>(d);
			T d2 = d / d1;
			divs.push(d1);
			divs.push(d2);
		}
	}

	return pps;
}


//【約数列挙】O(n^(1/4))
/*
* n の約数全てを昇順に格納したリストを返す．
*
* 利用：【素因数分解】
*/
vl divisors(ll n) {
	// verify : https://atcoder.jp/contests/chokudai_S002/tasks/chokudai_S002_j

	Assert(n > 0);

	map<ll, int> pps = factor_integer(n);

	vl divs{ 1 };
	repe(pp, pps) {
		ll p; int d;
		tie(p, d) = pp;

		vl powp(d);
		powp[0] = p;
		rep(i, d - 1) powp[i + 1] = powp[i] * p;

		int m = sz(divs);
		repir(j, m - 1, 0) {
			rep(i, d) {
				divs.push_back(divs[j] * powp[i]);
			}
		}
	}

	sort(all(divs));

	return divs;
}


//【約数の個数】O(n^(1/4))
/*
* n の約数の個数を返す．
*
* 利用：【素因数分解】
*/
int count_divisors(ll n) {
	Assert(n > 0);

	map<ll, int> pps = factor_integer(n);

	int res = 1;
	repe(pp, pps) res *= pp.second + 1;

	return res;
}


//【約数の総和】O(n^(1/4))
/*
* n の約数の総和を返す．
*
* 利用：【素因数分解】
*/
ll divisors_sum(ll n) {
	Assert(n > 0);

	map<ll, int> pps = factor_integer(n);

	ll res = 1;
	repe(pp, pps) {
		ll p; int d;
		tie(p, d) = pp;

		// オーバーフロー防止のため等比数列の和の公式は使わない
		ll sum = 0, pow = p, sumpow = 1;
		d++;
		while (d > 0) {
			if (d & 1) sum = sum * pow + sumpow;
			sumpow += sumpow * pow;
			pow *= pow;
			d /= 2;
		}

		res *= sum;
	}

	return res;
}


//【原始根】O(p^(1/4))
/*
* 素数の法 p における原始根を何か 1 つ返す．
*
* 利用：【素因数分解】
*/
ll find_primitive_root(ll p) {
	// verify : https://judge.yosupo.jp/problem/primitive_root

	if (p == 2) return 1LL;

	mt19937_64 mt((int)time(NULL));
	uniform_int_distribution<ll> rnd(1, p - 1);

	// p-1 の素因数を得る．
	auto pps = factor_integer(p - 1);
	mll::set_mod(p);

	while (true) {
		// r : 原始根の候補をランダムに選ぶ
		ll r = rnd(mt);

		// p-1 の任意の素因数 q について r^((p-1)/q) が 1 でないことが
		// r が原始根であるための必要十分条件となる．
		bool ok = true;
		for (auto [q, e] : pps) {
			if (mll(r).pow((p - 1) / q) == 1) {
				ok = false;
				break;
			}
		}

		if (ok) return r;
	}
	return -1LL;
}


//【素数計数関数】O(n^(3/4))
/*
* n 以下の素数の個数 π(n) を返す．
*
*（Lucy DP）
*/
ll prime_pi(ll n) {
	// 参考 : https://rsk0315.hatenablog.com/entry/2021/05/18/015511
	// verify : https://judge.yosupo.jp/problem/counting_primes

	//【方法】
	// S(v, p) = ([2..v] 内の "素数または p 以下の素因数をもたない合成数" の個数) とおく．
	// これはエラトステネスの篩において，p 以下の素数で篩い終えた後残っている数の個数である．
	//
	// p が合成数であるときと p^2 > v であるときは，篩のアルゴリズムより明らかに
	//		S(v, p) = S(v, p-1)
	// である．よって p が p^2 <= v なる素数の場合の遷移のみを考えれば良い．
	// 
	// p の 1 つ前の素数で篩い終わったときに残っている数の個数は S(v, p-1) である．
	// これらの数のうち p で篩われて消えるものの個数を考える．
	// そのような数は，[2..v] 内の
	//		(i) p より小さい素因数を持たない（まだ篩われていない）
	//		(ii) p より大きい p の倍数（次に篩われる）
	// という条件を共に満たす数である．
	// 
	// [2..v] に条件 (i), (ii) を課す代わりに，全体を p で割って，
	//		[2..v/p] 内の p より小さい素因数を持たない数
	// を数えても個数は変わらない．そのような数は，[2..v/p] 内の
	//		(iii) p-1 以下の素因数で篩い終えた後残っている
	//		(iv) p-1 以下の素数ではない
	// という条件を共に満たす数である．
	//
	// いま p^2 <= v なる場合を考えているので，p <= v/p である．
	// よって !(iv) ⇒ (iii) であるから，求める個数は
	//		S(v/p, p-1) - S(p-1, p-1)
	// と表される．
	//
	// 以上をまとめて，DP の遷移式
	//		S(v, p) = S(v, p-1) （p が合成数または p^2 > v のとき）
	//		S(v, p) = S(v, p-1) - (S(v/p, p-1) - S(p-1, p-1)) （その他の p のとき）
	// を得る．初項は
	//		S(v, 1) = v - 1
	// であり，求めたいものは
	//		π(n) = S(n, √n)
	// である．

	if (n <= 1) return 0;

	int m = (int)(sqrt(n) + 0.001);

	// S(v, p) : [2..v] 内の p 以下の素数で篩い終えた後残っている数の個数
	//	dp_p[0][v] : S(v, p)
	//	dp_p[1][v] : S(n/v, p)
	vvl dp(2, vl(m + 1));
	repi(v, 1, m) {
		dp[0][v] = v - 1;
		dp[1][v] = n / v - 1;
	}

	repi(p, 2, m) {
		// S(p - 1, p - 1)
		ll s = dp[0][p - 1];

		// p が素数でなければ次の p へ
		if (dp[0][p] == s) continue;

		// dp[1][v] = S(n/v, p) の更新
		repi(v, 1, m) {
			// p^2 > n/v なら更新不要
			if (p > n / v / p) break;

			if (v <= m / p) dp[1][v] -= dp[1][v * p] - s;
			else dp[1][v] -= dp[0][n / v / p] - s;
		}

		// dp[0][v] = S(v, p) の更新
		repir(v, m, 1) {
			// p^2 > v なら更新不要
			if (p > v / p) break;

			dp[0][v] -= dp[0][v / p] - s;
		}
	}

	return dp[1][1];
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
	// いま p^2 <= v なる場合を考えているので，p <= v/p である．
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

	if (n <= 1) return 0;

	int m = (int)(sqrt(n) + 0.001);

	// S(v, p) : [2..v] 内の p 以下の素数で篩い終えた後残っている数の和
	//	dp0_p[v] : S(v, p)
	//	dp1_p[v] : S(n/v, p)
	vl dp0(m + 1);
	vm dp1(m + 1);
	repi(v, 1, m) {
		dp0[v] = v * (v + 1) / 2 - 1;
		dp1[v] = mint(n / v) * (n / v + 1) / 2 - 1;
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


//【オイラー関数の総和】O(n^(3/4) / log n)
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
	// 例えば n = 40 のときの i = 2 を考えると，その子は
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

	int m = (int)(sqrt(n) + 0.001);

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


//【約数個数関数の総和】O(n^(3/4) / log n)
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

	if (n <= 1) return max(n, 0LL);

	int m = (int)(sqrt(n) + 0.001);

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

	// s : 注目頂点, i_gpf : s の最大素因数が何番目の素数か, sg : σ_0(s), c : s の最大素因数の指数
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


