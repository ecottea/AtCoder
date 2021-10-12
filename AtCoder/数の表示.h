#pragma once
#include "header.h"
// ■■■■■ 数の表示 ■■■■■



//【桁の数の取得】O(log n)
/*
* n を b 進表記したときの桁の数字を下位桁から順に並べたものを digits に格納する．
*/
void integer_digits(ll n, int b, vi& digits) {
	digits.clear();
	while (n > 0) {
		digits.push_back(n % b);
		n /= b;
	}
}


//【桁の数からの復元】O(|digits|)
/*
* b 進表記で下位桁から順に digits が並んだ数の値を返す．
*/
ll from_digits(vi& digits, int b = 10) {
	ll res = 0;
	ll powb = 1;
	repe(d, digits) {
		res += d * powb;
		powb *= b;
	}
	return res;
}


//【数字和】O(d)
/*
* d 桁の数 n を b 進表記したときの桁の数字の和を返す．
*/
int digit_sum(ll n, int b = 10) {
	int sum = 0;
	while (n > 0) {
		sum += n % b;
		n /= b;
	}
	return sum;
}


//【数字根】O(d log(log d))?
/*
* d 桁の数 n を b 進表記したときの数字根を返す．
*/
int digit_root(ll n, int b) {
	while (n >= b) {
		ll sum = 0;
		while (n > 0) {
			sum += n % b;
			n /= b;
		}
		n = sum;
	}
	return (int)n;
}


//【真分数 → 循環小数】O(m)
/*
* 真分数 frac = n / m の非循環部分の桁の数を noncycle に，
* 循環部分の桁の数を cycle にそれぞれ格納する．
*/
void real_digits(pii frac, vi& noncycle, vi& cycle, int base = 10) {
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
		return { n_nc , m_nc };
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
	return { n , m };
}


