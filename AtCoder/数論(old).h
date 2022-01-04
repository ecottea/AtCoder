#pragma once
#include "header.h"
#include "数論.h"
#include "畳み込み.h"


//【定数との gcd の分布】O(√n + d(n)^2)　（d(n) : n の約数の個数）
/*
* gcd(i, n) = g になるような i∈[1..K] の個数を cnt[g] に昇順に格納する．
*
* K=n のとき，cnt[g] は Z/nZ に位数 n/g の元が何個あるかとも解釈できる．
*
* 利用：【約数列挙】
*/
void gcd_distribution(ll K, ll n, unordered_map<ll, ll>& cnt) {
	// ds : n の約数の集合（昇順）
	vl ds;
	divisors(n, ds);
	int m = sz(ds);

	// cnt[d] : gcd(i, n) = d になるような i∈[1..K] の個数
	cnt.clear();

	repir(j, m - 1, 0) {
		// gcd(i, n) が d の倍数になるような i∈[1..K] の個数
		cnt[ds[j]] = K / ds[j];

		// gcd(i, n) = 2d, 3d, ... になるような i∈[1..K] の個数を引く．
		repi(j2, j + 1, m - 1) {
			if (ds[j2] % ds[j] == 0) cnt[ds[j]] -= cnt[ds[j2]];
		}
	}
}


//【定数との添字 gcd の和】O(√n + d(n)^2)　（d(n) : n の約数の個数）
/*
* gcd(i, n) = g になるような i∈[1..K] について，Σi を val[g] に昇順に格納する．
*
* 利用：【約数列挙】
*/
void gcd_indexed_sum(ll K, ll n, unordered_map<ll, mint>& val) {
	// verify : https://atcoder.jp/contests/abc020/tasks/abc020_d

	// ds : n の約数の集合（昇順）
	vl ds;
	divisors(n, ds);
	int m = sz(ds);

	mint inv2 = mint(2).inv();

	// val[d] : gcd(i, n) = d になるような i∈[1..K] についての Σi
	val.clear();

	repir(j, m - 1, 0) {
		// gcd(i, n) が d の倍数になるような i∈[1..K] について Σi を求める．
		// これは等差数列の和で，初項は d，末項は K / d * d，項数は K / d である．
		val[ds[j]] = mint(ds[j] + K / ds[j] * ds[j]) * (K / ds[j]) * inv2;

		// gcd(i, n) = 2d, 3d, ... になるような i∈[1..K] の Σi を引く
		repi(j2, j + 1, m - 1) {
			if (ds[j2] % ds[j] == 0) val[ds[j]] -= val[ds[j2]];
		}
	}
}


//【Z/nZ の位数分布】O(√n)
/*
* Z/nZ に位数 d の元が何個あるかを cnt[d] に格納する．
*
* 利用：【素因数分解】
*/
void order_distribution(ll n, unordered_map<ll, ll>& cnt) {
	// verify : https://atcoder.jp/contests/abc212/tasks/abc212_g

	// n を素因数分解する
	map<ll, int> pps;
	factor_integer(n, pps);

	vl ps; // ps : n の素因数の昇順リスト
	vl divs({ 1 }); // divs : n の約数の昇順リスト
	repe(pp, pps) {
		ll p; int d;
		tie(p, d) = pp;

		ps.push_back(p);

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

	// cnt[d] : Z/nZ に位数 d の元が何個あるか
	cnt.clear();
	repe(d, divs) cnt[d] = d;

	// 各素因数ごとに下からの差分をとる（倍数メビウス変換）
	repe(p, ps) {
		repe(d, divs) {
			if (n % (p * d)) continue;

			cnt[d] -= cnt[p * d];
		}
	}
}


//【倍数和 → f(1)】O(n log(log n) O(mf))
/*
* mf(d) := Σd|i f(i) を元に f(1) を計算して返す．
* ただし d が n より大きい素因数をもつとき mf(d) = 0 とする．
*
*（状態系包除原理）
*/
template <class T> T assign1(int n, function<T(ll)>& mf) {
	// verify : https://atcoder.jp/contests/abc206/tasks/abc206_e

	// 注意：【添字 gcd での畳込み】の mobius() でも同じことができる．

	// cnt[i] : i のもつ素因数の種類数（ただし平方因子をもつときは負の値）
	vi cnt(n + 1);

	repi(p, 2, n) {
		// p が素数でなければ何もしない．
		if (cnt[p] != 0) continue;

		// p の倍数のもつ素因数の種類数を 1 増やす．
		for (int i = p; i <= n; i += p) cnt[i]++;

		if (p > n / p) continue;

		// 素因数 p を 2 個以上もつ数は負の無限大としておく．
		for (int i = p * p; i <= n; i += p * p) cnt[i] = -INF;
	}

	T res = 0;

	repi(d, 1, n) {
		// d が平方因子をもつような mf(d) からの寄与はない．
		if (cnt[d] < 0) continue;

		// d のもつ素因数の種類数に応じた符号因子
		int sign = (cnt[d] % 2 == 0 ? 1 : -1);

		// mf(d) からの寄与を結果に加える．
		res += sign * mf(d);
	}

	return res;
}


//【倍数和 → 定数との gcd での分類和】O(√n)
/*
* d を n の約数とし，mf(d) := Σd|i f(i) を元にして，
* 各 d について Σgcd(i,n)=d f(i) を計算し val[d] に格納する．
*
*（状態系包除原理）
*
* 利用：【素因数分解】
*/
template <class T>
void fixed_gcd_sum(ll n, function<T(ll)>& mf, unordered_map<ll, T>& val) {
	// verify : https://atcoder.jp/contests/abc020/tasks/abc020_d

	// n を素因数分解する
	map<ll, int> pps;
	factor_integer(n, pps);

	vl ps; // ps : n の素因数の昇順リスト
	vl divs({ 1 }); // divs : n の約数の昇順リスト
	repe(pp, pps) {
		ll p; int d;
		tie(p, d) = pp;

		ps.push_back(p);

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

	// val[d] : Σgcd(i,n)=d f(i)
	val.clear();
	repe(d, divs) val[d] = mf(d);

	//【添字 gcd での畳込み】での mobius() と同じように，
	// 各素因数ごとに下からの差分をとる
	repe(p, ps) {
		repe(d, divs) {
			if (n % (p * d)) continue;

			val[d] -= val[p * d];
		}
	}
}


