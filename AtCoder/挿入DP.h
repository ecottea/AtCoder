#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 挿入 DP ■■■■■


//【列の数え上げ（隣接禁止）】O(n m max(cnt[i])^2) ?
/*
* [0..n) それぞれを cnt[0..n) 個ずつ含む長さ m の列で，
* 同じ数が隣り合わないものの個数を返す．
*
*（挿入 DP）
*
* 利用：【階乗と二項係数（mint利用）】
*/
mint count_nonadjacent_sequence(const vi& cnt_) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_string

	vi cnt;
	int m = 0; // 列の長さ
	repe(c, cnt_) {
		// 0 個の文字は無視する．
		if (c > 0) cnt.push_back(c);

		m += c;
	}
	int n = sz(cnt);
	Factorial_mint fm(m);

	// dp[i][j] : 文字 [0..i) で同じ文字の隣接が j 箇所ある文字列の個数
	vvm dp(n + 1);
	dp[0] = vm({ 1, 0 });

	int len = 2; // 文字列の長さ（両端の番兵含む）

	// i : 次に挿入する文字の種類
	rep(i, n) {
		dp[i + 1] = vm(len + cnt[i]);

		// j : 同じ文字の隣接箇所の個数
		rep(j, len - 1) {
			// k : 文字 i をいくつの固まりに分けるか
			repi(k, 1, cnt[i]) {
				// l : 同じ文字の間にいくつ固まりを挿入するか
				repi(l, 0, min(k, j)) {
					int nj = j + (cnt[i] - k) - l;

					mint add = dp[i][j];

					// cnt[i] 個の文字を順序込みで k 個に分ける方法の数
					//	まず文字を k 個減らしておき，重複組合せの考え方を用いて
					//	'o' cnt[i] - k 個と '|' k - 1 個の並べ方を数えれば良い．
					add *= fm.binomial(cnt[i] - 1, k - 1);

					// k 個の固まりをどこに挿入するか
					//	順序は先に定めたので，後は挿入位置だけを考えれば良い．
					//	同じ文字の間が j 箇所中 l 箇所，
					//	異なる文字の間が残り len - 1 - j 箇所中 k - l 箇所．
					add *= fm.binomial(j, l) * fm.binomial(len - 1 - j, k - l);

					dp[i + 1][nj] += add;
				}
			}
		}
		len += cnt[i];
	}

	return dp[n][0];
}


//【列の数え上げ（差 1 以下禁止）】O(n m max(cnt[i])^7) ? 
/*
* [0..n) それぞれを cnt[0..n) 個ずつ含む長さ m の列で，
* 差が 1 以下の数が隣り合わないものの個数を返す．
*
*（挿入 DP）
*
* 利用：【階乗と二項係数（mint利用）】
*/
mint count_noncontinuous_sequence(const vi& cnt_) {
	// verify : https://atcoder.jp/contests/joi2019yo/tasks/joi2019_yo_f

	vi cnt;
	int m = 0; // 列の長さ
	int cnt_max = -INF;
	repe(c, cnt_) {
		// 0 個の文字は無視する．
		if (c > 0) cnt.push_back(c);

		m += c;
		chmax(cnt_max, c);
	}
	int n = sz(cnt);
	Factorial_mint fm(m);

	// unordered_map<ll, -> 用
	auto hash = [&](int j0, int j1, int J1) {
		// j0 の最大値は i がひとかたまりになった場合の cnt_max - 1
		// j1 の最大値は i と i-1 が交互に並んだ場合の 2 * cnt_max
		// これらを基数として j0, j1, J1 を混ぜ合わせる．
		return j0 + (ll)cnt_max * (j1 + (2LL * cnt_max + 1) * J1);
	};
	auto unhash = [&](ll v) {
		int j0 = v % cnt_max; v /= cnt_max;
		int j1 = v % (2 * cnt_max); v /= (2LL * cnt_max + 1);
		int J1 = (int)v;
		return make_tuple(j0, j1, J1);
	};

	// dp[i][{j0, j1, J1}] : [0..i) までで以下の条件を満たす列の個数：
	//		j0 : i-1 を含む差が 0 の隣接箇所（i-1 と i-1）の個数
	//		j1 : i-1 を含む差が 1 の隣接箇所（i-1 と i-2）の個数
	//		j2 : i-1 を含む差が 2 以上の隣接箇所の個数
	//		J1 : i-1 を含まない差が 1 以下の隣接箇所の個数
	//		J2 : i-1 を含まない差が 2 以上の隣接箇所の個数
	vector<unordered_map<ll, mint>> dp(n + 1);
	dp[0][hash(0, 0, 0)] = 1;
	dump(dp[0]);

	int len = 2; // 文字列の長さ（両端の番兵 -inf, inf を含む）

	// i : 次に挿入する数
	rep(i, n) {
		repe(tmp, dp[i]) {
			int j0, j1, j2, J1, J2;
			tie(j0, j1, J1) = unhash(tmp.first);
			j2 = 2 * (i > 0 ? cnt[i - 1] : 0) - (2 * j0 + j1);
			J2 = (len - 1) - (j0 + j1 + j2 + J1);
			
			// k : 数 i をいくつの固まりに分けるか
			// ここからのループがひどいが，定数倍 1/5! = 1/120 が掛かっている．
			repi(k, 1, min(cnt[i], len - 1)) {
				// ij* : 対応する j* 個ある隣接箇所にいくつ固まりを挿入するか
				repi(ij0, 0, min(k, j0)) {
					repi(ij1, 0, min(k - ij0, j1)) {
						repi(ij2, 0, min(k - (ij0 + ij1), j2)) {
							repi(iJ1, 0, min(k - (ij0 + ij1 + ij2), J1)) {
								int iJ2 = k - (ij0 + ij1 + ij2 + iJ1);
								if (iJ2 > J2) continue;

								// nj* : 対応する隣接箇所の個数がいくつになるか
								int nj0 = cnt[i] - k;
								int nj1 = 2 * ij0 + ij1 + ij2;
								int nJ1 = (J1 - iJ1) + (j0 - ij0) + (j1 - ij1);
								
								mint add = tmp.second;

								// cnt[i] 個の文字を順序込みで k 個に分ける方法の数
								//	まず文字を k 個減らしておき，重複組合せの考え方を用いて
								//	'o' cnt[i] - k 個と '|' k - 1 個の並べ方を数えれば良い．
								add *= fm.binomial(cnt[i] - 1, k - 1);

								// k 個の固まりをどこに挿入するか
								//	順序は先に定めたので，後は挿入位置だけを考えれば良い．
								//	それぞれ j* 箇所中 ij* 箇所を選び順に挿入する．
								add *= fm.binomial(j0, ij0);
								add *= fm.binomial(j1, ij1);
								add *= fm.binomial(j2, ij2);
								add *= fm.binomial(J1, iJ1);
								add *= fm.binomial(J2, iJ2);

								dp[i + 1][hash(nj0, nj1, nJ1)] += add;
							}
						}
					}
				}
			}
		}
		len += cnt[i];
	}

	return dp[n][hash(0, 0, 0)];
}


//【容積の和】O(n log n)
/*
* 幅と高さが w[i], h[i] で与えられる n 個の長方形の並べ方全てについての容積の和を返す．
*
*（挿入 DP）
*/
mint cup_capacity_sum(const vl& h, const vl& w) {
	// verify : https://atcoder.jp/contests/tenka1-2015-final-open/tasks/tenka1_2015_final_e

	int n = sz(h);

	// 長方形を高さ降順にソートする．
	vector<pll> hw(n);
	rep(i, n) {
		hw[i] = { h[i], w[i] };
	}
	sort(all(hw), greater<pll>());

	// dp[i] : 深さ h[i] の隙間を選ぶ確率（h[n] = 0 とする．）
	// 
	// dp[i] が well-defined であることの確認：
	//	長方形 j を挿入する直前の深さ h[i] の隙間を選ぶ確率を p[i] とおく．
	//	長方形 j を挿入する直前には長方形が j 個あり，したがって隙間は j + 1 個ある．
	//	よって深さ h[i] の隙間の個数の期待値は (j + 1)p[i] 個である．
	// 
	//	長方形 j を深さ h[i] の隙間に挿入する確率は p[i] であり，
	//	このとき深さ h[i] の隙間は 1 個増える．
	//	よって長方形 j を挿入した直後の深さ h[i] の隙間の個数の期待値は
	//		(j + 1)p[i] + p[i] = (j + 2)p[i]
	//	である．
	//	長方形 j を挿入した直後には隙間は j + 2 個なので，
	//	結局どの長方形を挿入するタイミングかによって確率が変わることはない．
	//
	//	（ポリアの壺と同じ議論である．）
	vm dp(n + 1);
	dp[n] = 1;

	repi(i, 1, n - 1) {
		// 深さ h[i] の隙間ができるのは，直前の深さ 0 の隙間（両端のこと）に
		// 長方形 i を挿入した場合に限られる．
		// よって深さ h[i] の隙間の個数の期待値は 1 * dp[n] = dp[n] である．
		// 長方形 i を挿入した直後は隙間は i + 2 個なので，
		// 以降深さ h[i] の隙間が選ばれる確率は dp[n] / (i + 2) である．
		dp[i] = dp[n] / (i + 2);

		// 新しい隙間が選ばれる確率の分だけ両端が選ばれる確率を引いておく．
		dp[n] -= dp[i];
	}

	// acc_prob[i] : 深さ h[i] 以上の隙間を選ぶ確率
	// acc_dep[i] : 深さ h[i] 以上の隙間のみについての深さの期待値
	vm acc_prob(n + 1), acc_dep(n + 1);
	repi(i, 1, n - 1) {
		// 累積確率と累積期待値を計算する．
		acc_prob[i] = acc_prob[i - 1] + dp[i];
		acc_dep[i] = acc_dep[i - 1] + hw[i].first * dp[i];
	}

	mint res = 0;
	repi(i, 1, n - 1) {
		// 長方形 i を挿入することによって増える容積の期待値を考える．
		// 長方形 i（高さ h[i]）を h[i] より深い隙間に挿入する確率は acc_prob[i - 1] で，
		// このとき容積は，w[i] * (深さ - h[i]) だけ増える．
		// よって増える容積の期待値は
		//		w[i] * (深さ - h[i]) * acc_prob[i - 1]
		//		= (深さ * acc_prob[i - 1] - h[i] * acc_prob[i - 1]) * w[i]
		//		= (深さの期待値 - h[i] * 確率の和) * w[i]
		// である．
		res += (acc_dep[i - 1] - hw[i].first * acc_prob[i - 1]) * hw[i].second;
	}

	// 求めるべきものは総和なので，長方形の並べ方の数 n! を掛ける．
	repi(i, 1, n) res *= i;

	return res;
}


