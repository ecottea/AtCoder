#pragma once
#include "header.h"
// ■■■■■ 区間 DP ■■■■■



//【区間スコア和最大化】O((n + m) log n)
/*
* ビット列 [0, n) 上の m 個の区間 [l, r] とそのスコア a が与えられる．
* 区間内に 1 があればスコア a が加算されるとき，スコアの最大値を返す．
*
*（遅延評価セグメント木で高速化したインライン DP）
*/
ll op2(ll x, ll y) { return max(x, y); } // 区間最大値を得たい
ll e2() { return -INFL; }
ll mapping2(ll f, ll x) { return f + x; } // 区間への加算を行いたい
ll composition2(ll f, ll g) { return f + g; }
ll id2() { return 0; }
ll maximize_interval_score(int n, vi& l, vi& r, vl& a) {
	// 参考 : https://kyopro-friends.hatenablog.com/entry/2019/01/12/231106

	int m = sz(l);

	// 区間 [l, r] のスコアが a であることを r_to_la[r] ∋ {l, a} で記録する．
	vector<vector<pil>> r_to_la(n);
	rep(i, m) {
		r_to_la[r[i]].push_back({ l[i], a[i] });
	}

	// dp : 区間最大値の計算と区間への加算ができる遅延評価セグメント木．
	// dp[j + 1] : 今まで見てきた区間の中で考えたときの，
	//   最も右の 1 の位置が j であるようなものの中での最高スコア
	//   （j + 1 = 0 は 1 が全くないことを表す．）
	lazy_segtree<ll, op2, e2, ll, mapping2, composition2, id2> dp(n + 1);

	// 1 が全くないときのスコアは 0 である．
	dp.set(0, 0);

	// 区間の右端 r について昇順に見ていく．
	rep(r, n) {
		ll a_sum = 0;
		repe(la, r_to_la[r]) {
			a_sum += la.second;
		}

		// 位置 r を 1 にする場合
		//   r を右端にもつ区間のスコアの和 A が加算される．
		//   よって今までのスコアの最大値 + A が右端位置 r の最高スコアとなる．
		//   区間最大値を必要とするので遅延評価セグメント木が有効．
		dp.set(r + 1, dp.prod(0, r + 1) + a_sum);

		// 位置 r を 0 にする場合
		//   r を右端にもつ各区間 [l, r] とそのスコア a について，
		//   最も右の 1 が [l, r) に含まれている場合は，a が加算される．
		//   区間への加算を必要とするので遅延評価セグメント木が有効．
		repe(la, r_to_la[r]) {
			dp.apply(la.first + 1, r + 1, la.second);
		}
	}

	// 右端の 1 の位置を任意としたときの最高スコアを返す．
	return dp.all_prod();
}


//【連鎖行列積問題】O(n^3)
/*
* 行数 d[i]，列数 d[i + 1] の n 個の行列 M[i] の積を計算する場合の最小乗算回数を返す．
*
*（区間 DP）
*/
ll matrix_chain_multiplication_problem(vl& d) {
	int n = sz(d) - 1;

	// dp[i][j] : M[i..j] の積を計算する場合の最小乗算回数．
	vvl dp(n, vl(n, (ll)1e18));
	rep(i, n) {
		dp[i][i] = 0;
	}
	repi(w, 1, n - 1) { // w = j - i について昇順に考えていく
		rep(i, n) {
			int j = i + w;
			if (j >= n) {
				break;
			}

			repi(k, i, j - 1) {
				// M[i, k] * M[k + 1, j] と計算する場合
				chmin(dp[i][j], dp[i][k] + dp[k + 1][j] + d[i] * d[k + 1] * d[j + 1]);
			}
		}
	}

	return dp[0][n - 1];
}


//【発電計画問題】O(n^2)
/*
* 時刻 [l..r) に発電機をオンにすると c[l][r] の電力が得られるときの最大電力を返す．
* ただし [l..m) と [m..r) に同時に発電機をオンにすることはできない．
*/
ll unit_commitment_problem(const vvl& c) {
	int n = sz(c);

	// dp[r] : 時刻 [0..r) に得られる最大電力
	vl dp(n + 1);

	repi(r, 1, n) {
		// 時刻 [r-1..r) に発電機を動かさない場合
		dp[r] = dp[r - 1];

		// 時刻 [r-1..r) に発電機を動かす場合
		chmax(dp[r], c[0][r]);
		repi(l, 1, r - 1) {
			chmax(dp[r], dp[l - 1] + c[l][r]);
		}
	}

	return dp[n];
}


