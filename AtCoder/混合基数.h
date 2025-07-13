#pragma once
#include "header.h"
#include "辞書(動的).h"
// ■■■■■ 混合基数 ■■■■■


//【混合基数表示】
/*
* Mixed_radix<T>() : O(1)
*	注意：to_string() とは逆で添字が小さい方が下位
*
* set_base(vT b) : O(n)
*	各桁の上界 b[0..n) を設定する．
*	制約：b[n-1] = INFL（省略すると自動で追加される）
*
* set_weight(vl w) : O(n)
*	各桁の重み w[0..n) を設定する．
*	制約：w[0] = 1, w[i+1] は w[i] の倍数
*
* vT integer_digits(ll val) : O(n)
*	val の混合基数表示を返す．
*
* ll from_digits(vT ds) : O(n)
*	混合基数表示 ds が表す値を返す．（上位桁は省略可）
*/
template <class T>
struct Mixed_radix {
	// n : 桁数
	int n;

	// w[i] : 第 i 桁の重み（w[0] = 1）
	vl w;

	// b[i] : 第 i 桁の数の範囲が [0..b[i]) であることを表す（b[n-1] = INFL）
	vector<T> b;

	Mixed_radix() : n(0) {};

	// 混合基数 b[0..n) を設定する．（制約：b[n-1] = INFL（省略可））
	void set_base(const vector<T>& b_) {
		// verify : https://projecteuler.net/problem=774

		n = sz(b_);
		b = b_;

		if (b.back() != (T)INFL) {
			n++;
			b.push_back((T)INFL);
		}

		w.resize(n);
		w[0] = 1;
		rep(i, n - 1) w[i + 1] = w[i] * b[i];
	}

	// 各桁の重み w[0..n) を設定する．（制約：w[i+1] は w[i] の倍数，w[0] = 1）
	void set_weight(const vl& w_) {
		// verify : https://atcoder.jp/contests/abc231/tasks/abc231_e

		n = sz(w_);

		Assert(w_[0] == 1);
		w = w_;

		b.resize(n);
		rep(i, n - 1) {
			Assert(w[i + 1] % w[i] == 0);
			b[i] = (T)(w[i + 1] / w[i]);
		}
		b[n - 1] = (T)INFL;
	}

	// val の混合基数表示を返す．
	vector<T> integer_digits(ll val) {
		// verify : https://atcoder.jp/contests/abc231/tasks/abc231_e

		vector<T> ds(n);
		rep(i, n) ds[i] = (T)(val / w[i] % b[i]);
		return ds;
	};

	// 混合基数表示 ds が表す値を返す．
	ll from_digits(const vector<T>& ds) {
		// verify : https://projecteuler.net/problem=774

		ll val = 0;
		rep(i, sz(ds)) val += w[i] * ds[i];
		return val;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Mixed_radix& MR) {
		os << "b: " << MR.b << endl;
		os << "w: " << MR.w << endl;
		return os;
	}
#endif
};


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
* これは部分和問題（無限個）と同じである．
*/


//【フィボナッチ進法表示】
/*
* Zeckendorf_representation(ll n) : O(log n)
*	n 以下の整数のフィボナッチ進法表示を求められるよう初期化する．
*
* ll fibonacci(int i) : O(1)
*	i 番目のフィボナッチ数 fib[i] を得る（fib[0]=0, fib[1]=1 とする．）
*
* vi get_digits(ll n) : O(log n)
*	n のフィボナッチ進法表示を返す．（下位から順）
*	桁の数は {0, 1} であり，1 は連続せず，下 2 桁は常に "00" である．
*/
class Zeckendorf_representation {
	int m;
	vl fib;

public:
	// n 以下の整数のフィボナッチ進法表示を求められるよう初期化する．
	Zeckendorf_representation(ll n) {
		// verify : https://atcoder.jp/contests/arc122/tasks/arc122_c
		
		fib = vl{ 0, 1 }; m = 2;
		while (fib[m - 1] <= n) {
			fib.push_back(fib[m - 1] + fib[m - 2]);
			m++;
		}
	}

	// i 番目のフィボナッチ数 fib[i] を得る（fib[0]=0, fib[1]=1 とする．）
	ll fibonacci(int i) const {
		// verify : https://projecteuler.net/problem=692

		Assert(0 <= i && i < m);

		return fib[i];
	}

	// n のフィボナッチ進法表示を返す（下位から順）
	vi get_digits(ll n) const {
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
* 利用：【index 付き多重集合】
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
* 利用：【index 付き多重集合】
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


