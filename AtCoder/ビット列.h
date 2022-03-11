#pragma once
#include "header.h"
#include "探索.h"
#include "線形計画問題.h"
// ■■■■■ ビット列 ■■■■■


//【別の表現】
/*
* 0, 1 からなるビット列の有用な別の表現として，
*	(a) 累積和（区間内の 1 の個数が分かる）
*	(b) 初項と差分
*	(c) -1 と 1 の列
*	(d) -1 と 1 の列の累積和（区間内の 0, 1 の個数の偏りが分かる）
*	(e) 1 がある位置を並べた列
*	(f) i 番目の 1 がある位置の位置 i からの偏差を並べた列（1 の連が判別できる）
*	(g) 0 で区切られた 1 の連の長さを並べた列
* などがある．
* 
* 例：
*	元  :     0  1  0  1  1  1  0  0  1  0  1
*	(a) :  0  0  1  1  2  3  4  4  4  5  5  6
*	(b) :     0  1 -1  1  0  0 -1  0  1 -1  1
*	(c) :    -1  1 -1  1  1  1 -1 -1  1 -1  1
*	(d) :  0 -1  0 -1  0  1  2  1  0  1  0  1
*	(e) :        1     3  4  5        8    10
*	(f) :        1     2  2  2        4     5
*	(g) :  0  1     3           0  1     1
* 
* 例題：
*	(d) : https://atcoder.jp/contests/agc056/tasks/agc056_c
*	(f) : https://atcoder.jp/contests/abc229/tasks/abc229_g
*	(g) : https://atcoder.jp/contests/agc046/tasks/agc046_c
*/


//【1 の連の長さ】O(n)
/*
* ビット列 s[0..n) について，'0' で区切られた '1' の連の長さを順に並べた列を len に格納する．
*/
void length1(const string& s, vi& len, char one = '1') {
	// verify : https://atcoder.jp/contests/agc046/tasks/agc046_c

	len.clear();

	int l = 0;
	repe(c, s) {
		if (c == one) {
			l++;
		}
		else {
			len.push_back(l);
			l = 0;
		}
	}
	len.push_back(l);
}


//【区間幅固定ビット反転】O(n)
/*
* ビット列 s[0..n) について，幅 k の連続部分列 s[i..i+k) に対するビット反転操作を
* 任意の位置と回数で行うことで全てのビットを '0' とできるかを返す．
*/
bool interval_flip(const string& s, int k, char one = '1') {
	int n = sz(s);

	//【方法】
	// 操作の順序は無視でき，2 回行うと元に戻るので，
	// 左から順に操作を行うか否かを貪欲に定めていき，右側が全て 0 となるかを見れば良い．
	// 実際に操作を行うのではなく，操作によって反転されたかどうかが分かるようにする．

	// flip : 今走査している箇所がビット反転されているか
	bool flip = false;

	// ed[i] : s[i-k..i) のビット反転を行ったか
	vb ed(n + 1);

	repi(i, 0, n - k) {
		flip ^= ed[i];

		// s[i] == '1' なら s[i..i+k) を反転する．
		if (flip ^ (s[i] == one)) {
			ed[i + k] = true;
			flip = !flip;
		}
	}
	dump(ed);

	repi(i, n - k + 1, n - 1) {
		flip ^= ed[i];

		// s[i] == '1' なら不可能
		if (flip ^ (s[i] == one)) {
			return false;
		}
	}

	return true;
}


//【隣接互換での 1 の連続】O(n log n)（n = popcount(s)）
/*
* ビット列 s に対して隣接ビットの交換を k 回以下行えるとき，
* 連続させることのできる '1' の最大個数を返す．
*
* 利用：【めぐる式二分探索】
*/
int maximize_1sequence_length(const string& s, ll k, char one = '1') {
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


//【ビット列の連結】O(4^S S k log n)（ただし S = max(|s[i]|)）
/*
* n 個のビット列 s[i] を好きに並べて得られる長さ k のビット列の個数を返す．
*/
mint count_string_concat(const vector<string>& s_, int k, char one = '1') {
	// 参考 : https://suikaba.hatenablog.com/entry/2017/08/27/181249
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_concatenation

	int n = sz(s_);

	// m : 文字の長さの最大値
	int m = 0;
	rep(i, n) chmax(m, sz(s_[i]));

	// 扱いやすいように int のビット列に変換し，長さごとに記録しておく．
	vector<set<int>> s(m + 1);
	rep(i, n) {
		int seq = 0, len = sz(s_[i]);
		rep(j, len) {
			seq = seq * 2 + (s_[i][j] == one ? 1 : 0);
		}
		s[len].insert(seq);
	}

	// dp[i][seq][set] : 長さ i で直前の m 文字が seq であるもので，
	//	文字列 [0..i-j) が s を並べて得られるような j の集合が set であるものの個数
	vvvm dp(k + 1, vvm(1 << m, vm(1 << m)));
	dp[0][0][1] = 1;
	int mask = (1 << m) - 1;

	// i : 文字列の長さ
	rep(i, k) {
		// seq : 文字列（直前 m 文字のみ）
		repb(seq, m) {
			// b : seq に追加する文字
			repi(b, 0, 1) {
				// nseq : seq の末尾に b を追加した文字列
				int nseq = ((seq << 1) & mask) + b;

				// set : 文字列 [0..i-j) が s を並べて得られるような j の集合
				repb(set, m) {
					// cut_flag : 文字列 [0..i+1) が s を並べて得られるか
					int cut_flag = 0;

					// j : 文字列 [0..i-j) が s を並べて得られる
					rep(j, m) {
						if (!(set & (1 << j))) continue;

						// 文字列 nseq[i-j..i+1) が s に含まれるか
						if (s[j + 1].count(nseq & ((1 << (j + 1)) - 1))) {
							cut_flag = 1;
							break;
						}
					}

					// nset : 文字列 [0..i+1-j) が s を並べて得られるような j の集合
					int nset = ((set << 1) & mask) + cut_flag;

					dp[i + 1][nseq][nset] += dp[i][seq][set];
				}
			}
		}
	}

	mint res = 0;
	repb(seq, m) {
		repb(set, m) {
			if (set & 1) {
				res += dp[k][seq][set];
			}
		}
	}

	return res;
}


//【区間 01 平衡化】O(n + m)
/*
* ビット列 b[0..n) で，m 個の区間 [l[i], r[i]) に含まれる 0, 1 の個数が等しいもののうち，
* 辞書順最小のものを b に格納する．（存在しなければ false を返す．）
*
* 利用：【牛ゲー（01-上界指定のみ）】
*/
bool interval_balanced_bitseq(int n, const vi& l, const vi& r, vi& b) {
	// verify : https://atcoder.jp/contests/agc056/tasks/agc056_c

	int m = sz(l);
	b.resize(n);

	//【方法】
	// b[0..n) のビット '0'['1'] を数値 1[-1] に置き換え，
	// さらに累積和 acc[i] = Σb[0..i) を導入すると，満たすべき条件は
	//		|acc[i] - acc[i-1]| = 1  ...(1)
	//		acc[r] - acc[l-1] = 0    ...(2)
	// と書き直すことができ，目指すべきは「acc の辞書順最大化」となる．
	// 
	// 満たすべき条件 (1) を緩めて
	//		|acc[i] - acc[i-1]| <= 1 ...(3)
	// とすれば，acc の各項の大きさにはトレードオフの関係は無いので，
	// 単に「acc の各項の最大化」を目指すだけで良い．
	// 
	// 条件 (2), (3) はまとめて
	//		acc[i] - acc[i-1] <= 1
	//		acc[i-1] - acc[i] <= 1
	//		acc[r] - acc[l-1] <= 0
	//		acc[l-1] - acc[r] <= 0
	// と差の上界指定のみで書き直せるので，これは 01-BFS を用いた牛ゲーで解ける．
	//
	// 条件を (1) → (3) と緩めてしまったことが気になるが，最適解では
	//		acc[i] = acc[i-1]
	// となる i は存在しないので問題ない．
	// 
	// 実際，もしそのような i があるとすれば，
	// 条件 (2) より (i, j) が区間端を跨がないような j であって
	//		acc[j] = acc[j-1]
	// を満たすものが存在する．
	// しかしそれならば acc[i] と acc[j-1] の間を 1 ずつ増やせるはずだから矛盾である．

	Ushige_ub01_only ushi(n + 1);

	rep(j, m) {
		if ((r[j] - l[j]) % 2 == 1) return false;

		ushi.set_ub(l[j], r[j], 0);
		ushi.set_ub(r[j], l[j], 0);
	}

	rep(i, n) {
		ushi.set_ub(i, i + 1, 1);
		ushi.set_ub(i + 1, i, 1);
	}

	vi diff;
	ushi.maximize_diff(0, diff);

	rep(i, n) {
		if (diff[i + 1] - diff[i] == 1) b[i] = 0;
		else b[i] = 1;
	}

	return true;
}


