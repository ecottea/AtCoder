#pragma once
#include "header.h"
#include "辞書(動的).h"
// ■■■■■ 混合基数 ■■■■■


//【数 → 混合基数表示】
/*
* 最下位を 0 桁目とし，[0..n) 桁目が b[0..n) 未満の非負整数で与えられる混合基数について，
* 値 val を混合基数表示したときの i 桁目の数字を d[i] に格納し d[0..n) を返す．
*/
template <class T>
vector<T> mixed_radix_form(const vector<T>& b, ll val) {
	// verify : https://atcoder.jp/contests/abc231/tasks/abc231_e

	int n = sz(b);

	vector<T> d(n);
	rep(i, n) {
		d[i] = (T)(val % b[i]);
		val /= b[i];
	}

	return d;
}


//【混合基数表示 → 数】
/*
* 最下位を 0 桁目とし，[0..n) 桁目が b[0..n) 未満の非負整数で与えられる混合基数について，
* i 桁目の数字が d[i] である混合基数表示 d[0..n) で表される値を返す．
*/
template <class T>
ll from_mixed_radix_form(const vector<T>& b, const vector<T>& d) {
	int n = sz(b);

	ll val = 0; ll w = 1;
	rep(i, n) {
		val += w * d[i];
		w *= b[i];
	}

	return val;
}


//【混合基数，下から桁 DP，桁上げフラグ，スコア最小化】O(n)
/*
* 混合基数 b[0..n) で num[0..n) と表示される数について，全ての整数 d についての
* (d の数字和) + (num+d の数字和) の最小値を返す．
*/
ll minimize_pair_digit_sum(const vl& num, const vl& b) {
	// verify : https://atcoder.jp/contests/abc231/tasks/abc231_e

	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の最小スコア：
	//	i : 下からの桁 d[0..i) まで決まっている．
	//	f : 第 i+1 桁への桁上げがあるなら 1，さもなくば 0（桁上げフラグ）
	vvl dp(n + 1, vl(1LL << 1, INFL));
	dp[0][0] = 0;

	// 下の桁から順に配る DP
	rep(i, n) {
		ll x = num[i];

		repb(f, 1) {
			// d の i 桁目を 0 にする場合
			chmin(dp[i + 1][0], dp[i][f] + (x + f));

			// num+d の i 桁目を 0 にする場合
			chmin(dp[i + 1][1], dp[i][f] + (b[i] - (x + f)));

			// この 2 つの場合以外はスコアを最小にすることはないので無視できる．
			// おつり問題でいうと，渡した硬貨がそのまま返ってくることに対応する．
		}
	}

	return dp[n][0];
}


//【冗長混合基数表示の列挙】O(?)（二進なら val = 400 くらいまで動く）
/*
* 最下位を 0 桁目とし，[0..n) 桁目の重みが a[0..n) で与えられる混合基数について，
* 値 val の i 番目の冗長混合基数表示の j 桁目の数字を ds[i][j] に格納し ds を返す．
* 冗長混合基数表示では，桁の数字に任意の非負整数を認める．
*
* 制約：a[0] = 1，a[i] は a[i+1] の真の約数
*/
vvl enumerate_redundant_mixed_radix(const vl& a, ll val) {
	int n = sz(a);
	vvl ds; vl d(n);

	function<void(int)> rf = [&](int j) {
		// a[0] = 1 の位に立つ数は残り全部に確定．
		if (j == 0) {
			d[0] = val;
			ds.push_back(d);
			d[0] = 0;

			return;
		}

		// q : a[j] の位に立つ数の最大値
		ll q = val / a[j];

		repi(k, 0, q) {
			val -= k * a[j];
			d[j] = k;

			rf(j - 1);

			d[j] = 0;
			val += k * a[j];
		}
	};
	rf(n - 1);

	return ds;
}


//【冗長混合基数表示の数え上げ】
/*
* 非負整数 val を混合基数 a[0..n) で冗長混合基数表示する方法の数は，以下の式で与えられる：
*	[z^val] Πi 1/(1 - z^a[i])
*/


//【フィボナッチ進法表示】
/*
* Fibonacci_representation(ll n) : O(log n)
*	n 以下の整数のフィボナッチ進法表示を求められるよう初期化する．
*
* ll fibonacci(int i) : O(1)
*	i 番目のフィボナッチ数 fib[i] を得る（fib[0] = 0, fib[1] = 1 とする．）
*
* vi get_digits(ll n) : O(log n)
*	n のフィボナッチ進法表示を返す．（下位から順）
*	桁の数は {0, 1} であり，1 は連続せず，下 2 桁は常に "00" である．
*/
class Fibonacci_representation {
	int m;
	vl fib;

public:
	// n 以下の整数のフィボナッチ進法表示を求められるよう初期化する．
	Fibonacci_representation(ll n) {
		// verify : https://atcoder.jp/contests/arc122/tasks/arc122_c
		
		fib = vl{ 0, 1 }; m = 2;
		while (fib[m - 1] <= n) {
			fib.push_back(fib[m - 1] + fib[m - 2]);
			m++;
		}
	}

	// i 番目のフィボナッチ数 fib[i] を得る（fib[0] = 0, fib[1] = 1 とする．）
	ll fibonacci(int i) {
		Assert(0 <= i && i < m);

		return fib[i];
	}

	// n のフィボナッチ進法表示を返す（下位から順）
	vi get_digits(ll n) {
		// verify : https://atcoder.jp/contests/arc122/tasks/arc122_c
		
		if (n == 0) return vi{ 0 };

		int i = 2;
		while (fib[i] <= n) i++;
		vi ds(i);
		i--;

		// 上位桁から順に貪欲に選ぶ（初項が 0, 1 以外だと嘘なので注意）
		while (i >= 2) {
			if (fib[i] <= n) {
				ds[i] = 1;
				n -= fib[i];
			}
			else ds[i] = 0;
			i--;
		}
		ds[1] = ds[0] = 0;

		return ds;
	}
};


//【多倍長整数 → 階乗進法】O(?)
/*
* 非負整数 x の n 桁階乗進法表記を返す．
*/
#include <boost/multiprecision/cpp_int.hpp>
vi bigint_to_factorial_base(boost::multiprecision::cpp_int x, int n = -1) {
	// verify : https://yukicoder.me/problems/no/2577

	//【例】
	// x = 7 を階乗進法表記になおすと
	//		[1, 0, 1, 0].[3!, 2!, 1!, 0!] = 6 + 0 + 1 + 0 = 7
	// より "1010" となる．

	vi ds;

	int i = 1;
	while (x > 0) {
		ds.emplace_back((int)(x % i));
		x /= i;
		i++;
	}

	if (n != -1) while (sz(ds) < n) ds.emplace_back(0);

	reverse(all(ds));

	return ds;
}


//【階乗進法 → 順列】O(n log n)
/*
* 階乗進法表記で上位桁から順に ds[0..n) が並んだ数を num とする．
* [0..n) の順列で辞書順で num 番目（0-indexed）の順列を返す．
*
* 利用：【多重集合】
*/
vi factorial_base_to_permutation(const vi& ds) {
	// verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_h

	//【例】
	// 階乗進法表記で "1010" と表される数は，
	//		[1, 0, 1, 0].[3!, 2!, 1!, 0!] = 6 + 0 + 1 + 0 = 7
	// である．[0..4) の順列のうち辞書順で 7 番目のものは，
	//		0: [0, 1, 2, 3]
	//		1: [0, 1, 3, 2]
	//		2: [0, 2, 1, 3]
	//		3: [0, 2, 3, 1]
	//		4: [0, 3, 1, 2]
	//		5: [0, 3, 2, 1]
	//		6: [1, 0, 2, 3]
	//		7: [1, 0, 3, 2]
	// より p[0..4) = [1, 0, 3, 2] である．

	int n = sz(ds);
	vi p(n);

	vi ini(n);
	iota(all(ini), 0);

	// s : [0..n) の中で残っている数
	Multi_set<int> s(n, ini);

	rep(i, n) {
		// [0..n) の中で残っている数のうち ds[i] 番目のものを選ぶ．
		p[i] = s.get(ds[i]);

		// 選んだ数は消去しておく．
		s.erase(p[i]);
	}

	return p;
}


//【順列 → 階乗進法】O(n log n)
/*
* [0..n) の順列 p が何番目（0-indexed）かを階乗進法表示したものを返す．
*
* 利用：【多重集合】
*/
vi permutation_to_factorial_base(const vi& p) {
	// verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_h

	int n = sz(p);
	vi ds(n);

	vi ini(n);
	iota(all(ini), 0);

	// s : [0..n) の中で残っている数
	Multi_set<int> s(n, ini);

	rep(i, n) {
		// [0..n) の中で残っている数のうち ds[i] が何番目かを調べる．
		// 自身より右にある要素との間の転倒対の個数をも言い換えられる．
		ds[i] = (int)s.lower_bound(p[i]);

		// 選んだ数は消去しておく．
		s.erase(p[i]);
	}

	return ds;
}


//【加算（階乗進法）】O(n)
/*
* 階乗進法表示された数 a[0..n), b[0..n) の和を mod n! で計算し結果の階乗進法表示を返す．
*/
vi factorial_base_add(const vi& a, const vi& b) {
	// verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_h

	int n = sz(a);
	vi res(n);

	int carry = 0;
	repir(i, n - 1, 0) {
		int w = n - i;
		res[i] = a[i] + b[i] + carry;

		carry = res[i] / w;
		res[i] %= w;
	}

	return res;
}


