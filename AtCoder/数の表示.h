#pragma once
#include "header.h"
// ■■■■■ 数の表示 ■■■■■


//【桁の数の取得】O(log n)
/*
* n を b 進表記したときの桁の数字を上位桁から順に並べたものを ds に格納する．
* 
* 制約：|b| >= 2
*/
void integer_digits(ll n, vi& ds, int b = 10) {
	// verify : https://atcoder.jp/contests/abc105/tasks/abc105_c

	Assert(abs(b) >= 2);
	ds.clear();

	// n = 0 の場合の例外処理
	if (n == 0) {
		ds.push_back(0);
		return;
	}

	// mod |b| を取れば最下位桁から順に決定していく．
	while (n != 0) {
		int d = smod(n, abs(b));
		ds.push_back(d);
		n = (n - d) / b;
	}

	// 上位桁から順になるように並べ直す．
	reverse(all(ds));
}


//【桁の数からの復元】O(n)
/*
* b 進表記で上位桁から順に ds[0..n) が並んだ数の値を返す．
*/
ll from_digits(vi& ds, ll b = 10) {
	// verify : https://atcoder.jp/contests/abc105/tasks/abc105_c

	int n = sz(ds);

	ll res = 0, powb = 1;
	repir(i, n - 1, 0) {
		res += ds[i] * powb;
		powb *= b;
	}

	return res;
}


//【桁の数からの復元（文字列）】O(n)
/*
* b 進表記で表された数 s[0..n) の値を返す．桁の '0' は zero とする．
*/
mint from_digits(const string& s, int b = 10, char zero = '0') {
	// verify : https://atcoder.jp/contests/abc242/tasks/abc242_e

	mint res = 0, powb = 1;

	int n = sz(s);
	repir(i, n - 1, 0) {
		res += (s[i] - zero) * powb;
		powb *= b;
	}

	return res;
}


//【数字和】O(log n)
/*
* 非負の数 n を b 進表記したときの桁の数字の和を返す．
* 
* 制約：b >= 2
*/
ll digit_sum(ll n, ll b = 10) {
	// verify : https://atcoder.jp/contests/abc080/tasks/abc080_b

	Assert(b >= 2);

	ll sum = 0;
	while (n > 0) {
		sum += n % b;
		n /= b;
	}
	return sum;
}


//【数字和と繰り上がり】
/*
* b 進表記での数字和を s で表す．a + b の b 進での繰り上がりが k 回あるとき以下が成り立つ：
*	s(a + b) = s(a) + s(b) - (b - 1)k
* 
* verify : https://atcoder.jp/contests/arc144/tasks/arc144_a
*/


//【数字根】
/*
* 正の数 n を b 進表記したときの数字根は以下で与えられる：
*	(n - 1) % (b - 1) + 1
* 
* verify : https://yukicoder.me/problems/no/1252
*/


//【正則連分数展開】O(log min(num, dnm))
/*
* 正の有理数 num/dnm の正則連分数展開を seq に格納し，また GCD(分子, 分母) を返す．
* すなわち，num/dnm = seq[0] + 1/(seq[1] + 1/(seq[2] + 1/(...))) である．
*/
ll continued_fraction(ll num, ll dnm, vl& seq) {
	// verify : https://atcoder.jp/contests/abc273/tasks/abc273_h

	while (dnm > 0) {
		seq.emplace_back(num / dnm);
		num %= dnm;
		swap(num, dnm);
	}

	return num;
}


//【正則連分数展開からの復元】O(n)
/*
* 正の有理数の正則連分数展開 seq[0..n) が表す有理数 r を既約表示した {分子, 分母} を返す．
* すなわち，r = seq[0] + 1/(seq[1] + 1/(seq[2] + 1/(...))) である．
*/
pll from_continued_fraction(const vl& seq) {
	int n = sz(seq);
	ll num = 1, dnm = 0;

	repir(i, n - 1, 0) {
		swap(num, dnm);
		num += seq[i] * dnm;
	}

	return make_pair(num, dnm);
}


//【真分数 → 循環小数】O(m)
/*
* 真分数 frac = n / m の非循環部分の桁の数を noncycle に，
* 循環部分の桁の数を cycle にそれぞれ格納する．
*/
void real_digits(pii frac, vi& noncycle, vi& cycle, int base = 10) {
	// verify : https://atcoder.jp/contests/abc174/tasks/abc174_c

	int n, m;
	tie(n, m) = frac; // n / m

	noncycle.clear();
	cycle.clear();

	// last[i] : 余りが i になる割り算が何回目に起こったか
	vi last(m, -1);
	last[n] = 0;

	// 循環を検出するまで割り算を続ける．
	int i = 1, q, r;
	while (true) {
		n *= base;
		q = n / m;
		r = n % m;

		noncycle.push_back(q);

		// 割り切れた場合
		if (r == 0) return;

		// 循環を見つけた場合
		if (last[r] >= 0) break;

		last[r] = i++;
		n = r;
	}

	repi(j, last[r], i - 1) {
		cycle.push_back(noncycle[j]);
	}
	noncycle.resize(last[r]);
}


//【循環小数 → 真分数】O(m)
/*
* 非循環部分の桁の数が noncycle，循環部分の桁の数が cycle であるような
* 既約真分数 frac = n / m を {n, m} の形式で返す．
*/
pll from_real_digits(const vi& noncycle, const vi& cycle, int base = 10) {
	// n_nc, m_nc : 非循環部分の分子，分母
	ll n_nc = 0, m_nc = 1;
	repir(i, sz(noncycle) - 1, 0) {
		n_nc += noncycle[i] * m_nc;
		m_nc *= base;
	}
	// 有限小数の場合
	if (cycle.empty()) {
		ll g = gcd(n_nc, m_nc);
		n_nc /= g;
		m_nc /= g;
		return { n_nc, m_nc };
	}

	// n_c, m_c : 非循環部分の分子，分母（循環節が長いとオーバーフローするので注意）
	ll n_c = 0, m_c = 1;
	repir(i, sz(cycle) - 1, 0) {
		n_c += cycle[i] * m_c;
		m_c *= base;
	}
	ll mul = m_c - 1;
	m_c = mul * m_nc;

	// 通分して和をとる
	ll n = mul * n_nc + n_c;
	ll m = m_c;

	// 約分する
	ll g = gcd(n, m);
	n /= g;
	m /= g;
	return { n, m };
}


//【小数第 n 位の数】
/*
* num / dnm の b 進法での小数第 n 位の数を返す．
*
* 制約：dnm * b <= 2 * 10^9 + 1000
*/
int real_digit(int num, int dnm, ll n, int b = 10) {
	// verify : https://yukicoder.me/problems/no/1842

	//【方法】
	// 整数問題に帰着することを目指して変形する．
	// 
	// num / dnm の小数第 n 位の数は，num * b^n / dnm の 1 の位の数に等しい．
	// num * b^n を dnm で割った余りを r とおくと，求める数 x は
	//		x = ((num * b^n - r) / dnm)  (mod b)
	// と表される．両辺および法を dnm 倍し，両辺に r を加えることで
	//		dnm x + r = num * b^n  (mod (dnm b))
	// を得る．
	//
	// x は桁の数より 0 <= x < b ⇔ 0 <= x <= b - 1 である．
	// また r は dnm で割った余りより 0 <= r < dnm である．
	// 両者より左辺は
	//		0 <= dnm x + r < dnm (b - 1) + dnm = dnm b
	// と評価できるから，
	//		y = num * b^n mod (dnm b)
	// とおくと，
	//		x = (y - r) / dnm = y / dnm - r / dnm
	// である．0 <= r < dnm であったから，これはより簡潔に
	//		x = floor(y / dnm)
	// と表される．

	modint::set_mod(dnm * b);
	modint y = num * modint(b).pow(n);
	int res = y.val() / dnm;

	return res;
}


//【小数の読み込み】
/*
* 小数部分が k 桁以下の実数を誤差なく扱いたい場合，10^k 倍すれば整数になるので，
*		double x_; cin >> x_;
*		ll x = (ll)(x_ * 1ek + (x_ > 0 ? 0.5 : -0.5));
* とすれば良い．（キャスト時に 0 方向への丸めが入ることに注意．）
* 
* verify : https://atcoder.jp/contests/arc015/tasks/arc015_2
*/


//【二進数の繰り上がり[下がり] と XOR】
/*
* 非負整数 a, b が二進数表示されているものとするとき，
*	a + b で繰り上がりが起こらない ⇔ a + b = a XOR b
*	a - b で繰り下がりが起こらない ⇔ a - b = a XOR b
* 
* verify : https://yukicoder.me/problems/no/1674
* verify : https://atcoder.jp/contests/abc138/tasks/abc138_f
*/


