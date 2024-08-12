#pragma once
#include "header.h"
#include "FPS(mint).h"
#include "FPS(スパース).h"
// ■■■■■ 数え上げ（集合の分割） ■■■■■


//【ベル数】
/*
* 大きさ n の集合を分割する方法の数はベル数で与えられ，その指数型母関数は exp(exp(z) - 1) である．
*/


//【分割数】O(n√n)
/*
* 各 i∈[0..n] について自然数 i を自然数に分割する方法の数を格納したリストを返す．
*/
vm partition_function(int n) {
	// 参考 : https://ja.wikipedia.org/wiki/%E5%88%86%E5%89%B2%E6%95%B0
	// verify : https://judge.yosupo.jp/problem/partition_function

	// pen : 一般五角数の昇順列
	vi pen;
	repi(i, 1, n) {
		pen.push_back(i * (3 * i - 1) / 2);
		pen.push_back(i * (3 * i + 1) / 2);
		if (*pen.rbegin() > n) break;
	}
	int m = sz(pen);

	// 漸化式により計算する．
	vm p(n + 1);
	p[0] = 1;

	repi(i, 1, n) rep(j, m) {
		if (i - pen[j] < 0) break;

		// 符号は 4 で割った余りで場合分けされる．
		p[i] += (j & 2 ? -1 : 1) * p[i - pen[j]];
	}

	return p;
}


//【オイラーの五角数定理】O(√N)
/*
* スパース FPS [z^[0..N]] Πi∈[1..∞) (1 - z^i) を返す．
*/
SMFPS pentagonal_number_theorem(int N) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E3%81%AE%E4%BA%94%E8%A7%92%E6%95%B0%E5%AE%9A%E7%90%86
	// verify : https://yukicoder.me/problems/no/2763

	//【備考】
	// これの FPS 逆元をとったものが分割数の母関数である．

	vector<pim> dcs;
	dcs.push_back({ 0, 1 });

	repi(i, 1, INF) {
		mint sgn = i & 1 ? -1 : 1;

		int pos = i * (3 * i - 1) / 2;
		if (pos > N) break;
		dcs.push_back({ pos, sgn });

		pos = i * (3 * i + 1) / 2;
		if (pos > N) break;
		dcs.push_back({ pos, sgn });
	}

	return SMFPS(dcs);
}


//【自然数の順序付き分割】
/*
* 自然数 n を順序付きで分割する方法の数は 2^(n-1) である．
* 
*（証明）[1] から始めて，1 を (直前の要素に加える or 末尾に追加する) の 2 択が n-1 回ある．
*/


