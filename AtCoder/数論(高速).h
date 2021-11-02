#pragma once
#include "header.h"
#include "mint.h"
// ■■■■■ 高速な数論アルゴリズム ■■■■■



//【素数判定／ミラー - ラビン法】O((log n)^3)
/*
* n が素数かを返す．
*/
//internal::is_prime_constexpr(int n);


//【素数判定／ミラー - ラビン法】
/*
* n が素数かを返す．
*
* 利用：【有限体 F_p 上の計算（64 bit）】
*/
bool miller_rabin(ll n) {
	// 参考 : https://nyaannyaan.github.io/library/prime/fast-factorize.hpp.html

	//【方法】
	// p を奇素数とすると，任意の a=[1..p) についてフェルマーの小定理より
	//		a^(p-1) = 1 (mod p)
	// となる．これの平方根を考えていくと，
	//		p - 1 = 2^s d　（d : 奇数）
	// と表せば，
	//		a^d = 1 (mod p) or ∃r=[0..s), a^(2^r d) = -1 (mod p)
	// と書き直せる．
	// 
	// この対偶を用いて判定することをランダムに選んだ a で繰り返す．
	// n の範囲を限定するなら擬素数を生じない a を固定的に選べる．

	const vl as = { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 };

	if (n == 2 || n == 3 || n == 5 || n == 13 || n == 19 || n == 73 || n == 193
		|| n == 407521 || n == 299210837) return true;
	if (n == 1 || n % 2 == 0) return false;

	mll::set_mod(n);
	int s = 0; ll d = n - 1LL;
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


//【約数検出／ポラードのρ法】O(n^(1/4))
/*
* n の真の約数を何か 1 つ返す．
*
* 制約 : n は合成数
*
* 利用：【有限体 F_p 上の計算（64 bit）】
*/
ll pollard_rho(ll n) {
	// 参考 : https://qiita.com/Kiri8128/items/eca965fe86ea5f4cbb98

	//【方法】
	// 適当な定数 c をとり関数 f : Z/nZ → Z/nZ を
	//		f(x) = x^2 + c
	// と定める．
	//
	// 適当な初期値 x[0] = y[0] (= 2) から始め，Z/nZ 上の数列を漸化式
	//		x[i + 1] = f(x[i]), y[i + 1] = f(f(y[i]))
	// で定める．フロイドの循環検出法より，もし
	//		gcd(x[i] - y[i], n) = g ∈ (1..n)
	// であれば，これは f が Z/gZ（g は n の真の約数）で巡回したことを意味する．
	//
	// 実際には，
	//		x は r = (2 冪) 個ずつ進める（定数 1/2 倍）
	//		gcd の計算を m = n^(1/8) 程度個まとめて行う（gcd の log を落とす）
	// ことにより高速化を図る．

	if (!(n & 1)) return 2;

	int m = 1 << (msbll(n) / 8);
	mll::set_mod(n); // n は合成数だが割り算は使わないので問題ない

	const int c_max = 99; // c を最大どこまで試すか
	repi(c, 1, c_max) {
		auto f = [&](mll x) { return x * x + c; };

		mll x, y = 2, y_bak;
		ll g = 1;
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
				g = gcd(mul.val(), n);

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
				g = gcd((x - y_bak).val(), n);
			}
		}

		// g < n なら g が n の真の約数なのでそれを返す．
		if (g < n) return g;

		// g = n ならたまたま真の約数が全て同時検出されてしまったので，
		// 関数 f における定数項 c の値を別のものに取り替えて再挑戦．
	}

	// 複数個の c を試してなお失敗したなら諦める．
	return n;
}


//【素因数分解／ポラードのρ法】O(n^(1/4))
/*
* n を素因数分解した結果を pps に格納する．
*
* pps[p] = d : n に素因数 p が d 個含まれていることを表す．
*
* 利用：【素数判定／ミラー - ラビン法】，【約数検出／ポラードのρ法】
*/
void factor_integer(ll n, map<ll, int>& pps) {
	pps.clear();
	if (n == 1) return;

	// 検出した約数を記録しておくキュー
	queue<ll> divs;
	divs.push(n);

	while (!divs.empty()) {
		ll d = divs.front();
		divs.pop();

		// 約数が素数なら素因数発見
		if (miller_rabin(d)) {
			pps[d]++;
		}
		// 約数が合成数なら新たな約数を 2 つ発見する
		else {
			ll d1 = pollard_rho(d);
			ll d2 = d / d1;
			divs.push(d1);
			divs.push(d2);
		}
	}
}


//【約数列挙】O(n^(1/4))
/*
* n の約数全てをリスト divs に昇順に格納する．
*
* 利用：【素因数分解／ポラードのρ法】
*/
void divisors(ll n, vl& divs) {
	assert(n > 0);

	map<ll, int> pps;
	factor_integer(n, pps);

	divs = vl({ 1 });
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
}


//【約数の個数】O(n^(1/4))
/*
* n の約数の個数を返す．
*
* 利用：【素因数分解／ポラードのρ法】
*/
int count_divisors(ll n) {
	assert(n > 0);

	map<ll, int> pps;
	factor_integer(n, pps);

	int res = 1;
	repe(pp, pps) res *= pp.second + 1;

	return res;
}


//【約数の総和】O(n^(1/4))
/*
* n の約数の総和を返す．
*
* 利用：【素因数分解／ポラードのρ法】
*/
ll divisors_sum(ll n) {
	assert(n > 0);

	map<ll, int> pps;
	factor_integer(n, pps);

	ll res = 1;
	repe(pp, pps) {
		ll p; int d;
		tie(p, d) = pp;

		// オーバーフロー防止のため等比数列の和の公式は使わない
		ll sum = 0, pow = p, sumpow = 1;
		d++;
		while (d > 0) {
			if (d & 1LL) sum = sum * pow + sumpow;
			sumpow += sumpow * pow;
			pow *= pow;
			d /= 2;
		}

		res *= sum;
	}

	return res;
}


//【素数計数関数】O(n^(3/4))
/*
* n 以下の素数の個数 π(n) を返す．
*/
ll prime_pi(ll n) {
	// 参考 : https://rsk0315.hatenablog.com/entry/2021/05/18/015511#fn-bab5992c

	if (n <= 1) return 0;

	int m = (int)(sqrt(n) + EPS);

	// S(v, p) を [1..v] で "素数または p 以下の素因数をもたない合成数" の個数とする．
	// dp_p[0][v] : S(v, p)，dp_p[1][v] : S(n/v, p)
	vvl dp(2, vl(m + 1LL));
	repi(v, 1, m) {
		dp[0][v] = v - 1;
		dp[1][v] = n / v - 1;
	}

	repi(p, 2, m) {
		// S(p - 1, p - 1)
		ll s = dp[0][p - 1LL];

		// p が素数でなければ次の p へ
		if (dp[0][p] == s) continue;

		// dp[1][v] = S(n/v, p) の更新
		repi(v, 1, m) {
			// p^2 > n/v なら次の p へ
			if (p > n / v / p) break;

			if (v <= m / p)
				dp[1][v] -= dp[1][(ll)v * p] - s;
			else
				dp[1][v] -= dp[0][n / v / p] - s;
		}

		// dp[0][v] = S(v, p) の更新
		repir(v, m, 1) {
			// p^2 > v なら次の p へ
			if (p > v / p) break;

			dp[0][v] -= dp[0][v / p] - s;
		}
	}

	return dp[1][1];
}


