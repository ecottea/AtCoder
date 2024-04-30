#pragma once
#include "header.h"
#include "探索.h"
// ■■■■■ 最適化（ビット列） ■■■■■


//【最小 swap 回数】O(n)
/*
* ビット列 s[0..n) に対し，隣接要素の swap を繰り返すことで t[0..n) に変化させるときの
* swap の最小回数を返す（不可能なら INFL）
*/
ll swap_distance(const string& s, const string& t, char one = '1') {
	// verify : https://atcoder.jp/contests/tkppc6-2/tasks/tkppc6_2_b

	int n = sz(s);

	// 長さが異なるならもちろん不可能
	if (sz(t) != n) return INFL;

	// a[i] : s[0..n) における i 番目の 1 の位置
	// b[i] : t[0..n) における i 番目の 1 の位置
	vi a, b;
	rep(i, n) {
		if (s[i] == one) a.push_back(i);
		if (t[i] == one) b.push_back(i);
	}

	int m = sz(a);
	if (sz(b) != m) return INFL;

	ll res = 0;

	// 1 同士の swap をしても無駄なので，追い越しはせず 1 を順に対応させていくのが最善
	rep(j, m) res += abs(a[j] - b[j]);

	return res;
}


//【0-1 間距離の和の最小化】O(n)
/*
* 0,1 を各 n 個含むビット列 s[0..2n) について，(0,1) のペアを n 個作る．
* s[i]='0' と s[j]='1' をペアにするとコスト |j-i| がかかるとし，コストの総和の最小値を返す．
* またそれを実現するようなペアの作り方が何通りあるかを cnt に格納する．
*/
ll minimize_01distance_sum(const string& s, mint* cnt = nullptr, char one = '1') {
	// verify : https://atcoder.jp/contests/agc037/tasks/agc037_b

	//【方法】
	// ('0','1') を (-1,1) に置き換えて累積和 acc を計算していく．
	// 累積和 acc が負[正] のときは相手の居ない '0'['1'] が |acc| 個余っているので，
	// もし '1'['0'] を見たらそれらからペアの相手を選択するのが一律で最善である．
	// コストについては，s[i] を右に選んだら +i，左に選んだら -i とすれば別々で考えられる．

	int n2 = sz(s);
	if (cnt == nullptr) cnt = new mint;
	*cnt = 1;

	ll sc = 0; int acc = 0;
	rep(i, n2) {
		if (acc < 0 && s[i] == one) {
			*cnt *= -acc;
			sc += i;
		}
		else if (acc > 0 && s[i] != one) {
			*cnt *= acc;
			sc += i;
		}
		else {
			sc -= i;
		}

		acc += (s[i] == one ? 1 : -1);
	}

	return sc;
}


//【隣接互換での 1 の連続】O(n log n)（n = popcount(s)）
/*
* ビット列 s[0..n) に対して隣接要素の交換を k 回以下行えるとき，
* 連続させることのできる '1' の最大個数を返す．
*
* 利用：【めぐる式二分探索】
*/
int maximize_run_length(const string& s, ll k, char one = '1') {
	// verify : https://atcoder.jp/contests/abc229/tasks/abc229_g

	//【方法】
	// i 番目の '1' の位置を a[i] とおくと，
	//		各 a[i] に対して k 回の +1[-1] の操作が行えるとき，
	//		最大でいくつの数を連続させることができるか
	// という問題と読み替えられる．
	// 
	// さらに a[i] の i からの偏差を b[i] = a[i] - i とおくと，
	//		各 b[i] に対して k 回の +1[-1] の操作が行えるとき，
	//		最大でいくつの数を等しくできるか
	// という問題と読み替えられる．
	//
	// 等しくできる数の個数には単調性があるので，答えで二分探索を行う．
	// そのためには，
	//		各 b[i] に対して +1[-1] の操作が行えるとき，
	//		k 回以内の操作で m 個の数を等しくできるか
	// という判定問題が解ければ良い．
	// 
	// m 個の数としては明らかに連続した b[i..i+m) を選ぶのが最善なので，
	// 固定された各 i について
	//		各 b[j] に対して +1[-1] の操作が行えるとき，
	//		k 回以内の操作で b[i..i+m) を等しくできるか
	// という判定問題が高速に解ければ良い．
	//
	// b[i..i+m) を x に揃えるとすると，必要な操作回数は
	//		Σj∈[i..i+m) |b[j] - x|
	// である．x は自由に動かせるので，先の判定問題は
	//		min_x Σj∈[i..i+m) |b[j] - x| <= k
	// と読み替えられる．
	//
	// 左辺を中央値の性質を用いて書き換えることで，先の判定問題は
	//		Σj∈[i..i+m) |b[j] - median(b[i..i+m))| <= k
	//		⇔ Σj∈[i..i+m) |b[j] - b[i+m/2]| <= k
	// と読み替えられる．
	// これは b の累積和の前計算のもと O(1) で判定できる．

	// a[i] : i 番目の '1' の位置
	vi a;
	rep(j, sz(s)) {
		if (s[j] == one) a.push_back(j);
	}
	int n = sz(a);

	// b[i] : i 番目の '1' の位置 a[i] の位置 i に対する偏差
	vl b(n);
	rep(i, n) b[i] = a[i] - i;

	// acc_b[i] : Σb[0..i)
	vl acc_b(n + 1);
	rep(i, n) acc_b[i + 1] = acc_b[i] + b[i];

	// k 回以内の +1[-1] で b のうち m 個の数を揃えられるか
	function<bool(int)> okQ = [&](int m) {
		int hm = m / 2;

		// i : b[i..i+m) に注目していることを表す
		rep(i, n - m + 1) {
			ll sum = 0;

			// 中央値より大きい部分
			sum += acc_b[i + m] - acc_b[i + hm] - b[i + hm] * (m - hm);

			// 中央値以下の部分
			sum -= acc_b[i + hm] - acc_b[i + 0] - b[i + hm] * (hm - 0);

			if (sum <= k) return true;
		}
		return false;
	};

	return meguru_search(0, n + 1, okQ);
}


