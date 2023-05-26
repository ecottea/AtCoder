#pragma once
#include "header.h"
// ■■■■■ 数の表示 ■■■■■


//【桁の数の取得】O(log n)
/*
* n を b 進表記したときの桁の数字を上位桁から順に並べたリストを返す．
*
* 制約：|b| >= 2
*/
vi integer_digits(ll n, int b = 10) {
	// verify : https://atcoder.jp/contests/abc105/tasks/abc105_c

	Assert(abs(b) >= 2);

	// n = 0 の場合の例外処理
	if (n == 0) return vi{ 0 };

	// mod |b| を取れば最下位桁から順に決定していく．
	vi ds;
	while (n != 0) {
		int d = smod(n, abs(b));
		ds.push_back(d);
		n = (n - d) / b;
	}

	// 上位桁から順になるように並べ直す．
	reverse(all(ds));

	return ds;
}


//【桁の数の取得（桁数固定）】O(log n)
/*
* n を len 桁で b 進表記したときの桁の数字を上位桁から順に並べたリストを返す．
*
* 制約：|b| >= 2
*/
vi integer_digits(ll n, int len, int b = 10) {
	// verify : https://yukicoder.me/problems/no/327

	Assert(abs(b) >= 2);

	// mod |b| を取れば最下位桁から順に決定していく．
	vi ds(len);
	rep(i, len) {
		int d = smod(n, abs(b));
		ds[len - 1 - i] = d;
		n = (n - d) / b;
	}

	return ds;
}


//【桁の数からの復元】O(n)
/*
* b 進表記で上位桁から順に ds[0..n) が並んだ数の値を返す．
*/
ll from_digits(const vi& ds, ll b = 10) {
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
template <class T>
T from_digits(const string& s, int b = 10, char zero = '0') {
	// verify : https://atcoder.jp/contests/abc242/tasks/abc242_e

	T res = 0, powb = 1;

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
* b 進表記での数字和を s で表す．x + y の b 進での繰り上がりが k 回あるとき以下が成り立つ：
*	s(x + y) = s(x) + s(y) - (b - 1)k
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


//【二進数の繰り上がり[下がり] と XOR】
/*
* 非負整数 a, b が二進数表示されているものとするとき，
*	a + b で繰り上がりが起こらない ⇔ a + b = a XOR b
*	a - b で繰り下がりが起こらない ⇔ a - b = a XOR b
* 
* verify : https://yukicoder.me/problems/no/1674
* verify : https://atcoder.jp/contests/abc138/tasks/abc138_f
*/


