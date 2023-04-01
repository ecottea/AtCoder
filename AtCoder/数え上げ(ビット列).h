#pragma once
#include "header.h"
#include "FPS(mint).h"
// ■■■■■ 数え上げ（ビット列） ■■■■■


//【1 が隣り合わないビット列の数え上げ（1 の個数ごと）】
/*
* 長さ n のビット列で，1 を m 個含み，1 が隣り合わないものの数は bin(n+1-m, m) 個である．
* 
*（証明）n+1-2m 個の 'o' と m 個の '|' の順列に対し，'o' → "0", '|' → "01" と置き換え，
* 最左ビット（必ず '0'）を取り除くと，条件を満たすビット列が一意に得られる．
* 
* verify : https://atcoder.jp/contests/agc023/tasks/agc023_c
*/


//【1 が k 個連続しないビット列の数え上げ】O(n)
/*
* 長さ i∈[0..n] のビット列のうち，'1' が k 個連続しないものの個数を cnt[i] に格納する．
*/
void count_noncontinuous_bitsequences(int n, int k, vm& cnt) {
	cnt.resize(n + 1);

	// acc[i] : 長さ i 以下のビット列で，最後が '0' のものの個数
	vm acc(n + 1);
	acc[0] = 1;

	repi(i, 1, n) {
		// "[j個の1]0" を付けられるのは長さが i-1-j のものである．
		// acc[i] - acc[i-1] は j=[0..k) についてこれらを足し合わせたものとなる．
		acc[i] = acc[i - 1] + (acc[i - 1] - (i - 1 - k >= 0 ? acc[i - 1 - k] : 0));
	}

	repi(i, 0, n) {
		// 最後に "[j個の1]" を付けられるのは長さが i-j のものである．
		// j=[0..k) についてこれらを足し合わせたものが求める場合の数となる．
		cnt[i] = acc[i] - (i - k >= 0 ? acc[i - k] : 0);
	}
}


//【1 が k 個連続しないビット列の数え上げ（mod 998244353）】O(k log k log n)
/*
* 長さ n のビット列のうち，'1' が k 個連続しないものの個数を返す．
*
* 利用：【展開係数】
*/
mint count_noncontinuous_bitsequences(ll n, int k) {
	if (k == 0) return 0;

	// 母関数は (1+z+...+z^(k-1)) / (1-z-...-z^k) である．
	vm fc(k, 1), gc(k + 1, -1);
	gc[0] = 1;

	return bostan_mori(MFPS(fc), MFPS(gc), n);
}


//【ビット列の数え上げ（区間 OR 指定）】O(n + m)
/*
* ビット列 s[0..n) で，m 個の半開区間 [l[j]..r[j]) それぞれについて
* OR s[l[j]..r[j]) = 1 であるものの個数を返す．
*/
mint count_non0interval_bitsequences(int n, const vi& l, const vi& r) {
	int m = sz(l);

	// r_to_l[r] : 右端が r の半開区間のうちの左端の最大値（なければ -1）
	vi r_to_l(n + 1, -1);
	rep(j, m) chmax(r_to_l[r[j]], l[j]);
	rep(i, n) chmax(r_to_l[i + 1], r_to_l[i]);

	// dp[i] : s[0..i) のうち，s[i-1] = '1' であるものの個数（s[-1] = '1' とみなす）
	vm dp(n + 1);
	dp[0] = 1;

	// acc[i] : Σdp[0..i)
	vm acc(n + 2);
	acc[1] = dp[0];

	repi(r, 1, n) {
		// s[0..r) の接尾辞が "[ちょうどj-1個の0]1" となっているものの個数は dp[r-j] である．
		// '0' を連続して並べて良い左端の限界は r_to_l[r-1]+1 なので，その範囲で累積和をとる．
		dp[r] = acc[r] - acc[r_to_l[r - 1] + 1];

		acc[r + 1] = acc[r] + dp[r];
	}

	return acc[n + 1] - acc[r_to_l[n] + 1];
}


//【連結で得られるビット列の数え上げ】O(4^m m k log n) （m = max(|s[i]|)）
/*
* n 個のビット列 s[i] を好きに並べて得られる長さ k のビット列の個数を返す．
* 
*（スライド bit DP）
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
	vvvm dp(k + 1, vvm(1LL << m, vm(1LL << m)));
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


