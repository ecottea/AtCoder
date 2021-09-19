#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 動的計画法の典型問題 ■■■■■



//【部分和数え上げ問題（状態 DP）】O(n v)
/*
* 長さ n の非負整数の列 a の部分和として v を作る方法が何通りあるかを返す．
* 
* → 部分和問題.h
*/
mint count_partial_sum(vi& a, int v);


//【部分和列挙問題（ビット演算で高速化した bool DP）】O(n v)（v = Σa[i]）
/*
* n 個の非負整数の集合 a の部分和として作れる数を列挙する．
*
* bitset<A> set : A は Σa[i] + 1 以上の定数．i が作れるなら set[i] = true．
* 
* → 部分和問題.h
*/
template <size_t A>
void enumerate_partial_sum(vi& a, bitset<A>& set);


//【劣数列和数え上げ問題（状態 DP，累積和で高速化）】O(n sum)
/*
* 長さ n の非負整数の列 a と非負整数 sum について，
* 各項が a 以下の非負整数の列で和がちょうど sum になるものの個数を返す．
*/
template <class T>
mint count_minorant_sum(vector<T>& a, T sum) {
	int n = sz(a);

	// dp[i + 1][j] : i 番目の数までで，和が j 未満という状態をとる場合の数
	vvm dp(n + 1, vm(sum + 2));

	// 空集合の和は 0 とみなす．
	dp[0][0] = 0;
	repi(j, 1, sum + 1) {
		dp[0][j] = 1;
	}

	// 貰う DP
	// いきなり累積和で書けなくても，貰う DP で書いて Σ が現れれば直せる可能性あり．
	rep(i, n) {
		repi(j, 0, sum) {
			// i 番目の数までで，和がちょうど j という状態をとる場合の数
			//   i - 1 番目の数までで，和が max(j - a[i], 0) 以上 j 以下であれば，
			//   i 番目の数を 0 以上 a[i] 以下の範囲で適切に定めれば和をちょうど j にできる．
			//   よってこれらの和をとればいいが，累積和の差として O(1) で計算できる．
			mint cnt = dp[i][j + 1] - dp[i][max(j - a[i], 0)];

			// 累積和になおしてテーブルに記録する．
			dp[i + 1][j + 1] += dp[i + 1][j] + cnt;
		}
	}

	// 累積和の差分をとり，和がちょうど sum になるものの個数を返す．
	return dp[n][sum + 1] - dp[n][sum];
}


//【最長増加部分列（インライン DP，セグ木で高速化）】O(n log n)
/*
* 長さ n の配列 a の最長増加部分列の長さを返す．
* また最長増加部分列の一例を lis に構成する．
* 
* → 部分列.h
*/
template <class T>
int longest_increasing_subsequence(vector<T>& a, vector<T>& lis);


//【区間スコア和最大化（インライン DP，遅延評価セグ木で高速化）】O((n + m) log n)
/*
* ビット列 [0, n) 上の m 個の区間 [l, r] とそのスコア a が与えられる．
* 区間内に 1 があればスコア a が加算されるとき，スコアの最大値を返す．
* 
* → 区間.h
*/
ll maximize_interval_score(int n, vi& l, vi& r, vl& a);


//【最長共通部分列（二次元 DP）】O(|s| |t|)
/*
* 2 つの文字列 s, t の最長共通部分列の一例を lcs に格納する．
* また最長共通部分列の長さを返す．
* 
* → 部分列.h
*/
int longest_common_subsequence(const string& s, const string& t, string& lcs);


//【連鎖行列積問題（区間 DP）】O(n^3)
/*
* 行数 d[i]，列数 d[i + 1] の n 個の行列 M[i] の積を計算する場合の最小乗算回数を返す．
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


//【数字和数え上げ問題（桁 DP）】O(n d)
/*
* n 桁の数 num 以下の非負の整数で，数字和が d の倍数であるものの個数を返す．
*/
mint count_digit_sum(string& num, int d) {
	int n = sz(num);

	// dp[i][j][k] : 以下の条件を満たす num 以下の数が何通りあるか．
	// i : 上から i 桁目まで決まっている（0-indexed）
	// j : ここまでだけで num より小さいことが確定していれば 1，さもなくば 0
	// k : 数字和が k (mod d)
	vvvm dp(n + 1, vvm(2, vm(d)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// num の上から i 桁目の数
		int x = num[i] - '0';

		rep(k, d) {
			// もう num より小さいことが確定している場合，
			// i + 1 桁目の数は何であっても num より小さいと確定する．
			rep(t, 10) {
				dp[i + 1][1][(k + t) % d] += dp[i][1][k];
			}

			// まだ num より小さいと確定していない場合，
			// i + 1 桁目の数によって以下の場合分けが生じる：
			//		x 未満であれば num より小さいと確定する．
			//		x ちょうどであればまだ確定しない．
			//		x を超える数は選ぶことができない．
			rep(t, x) {
				dp[i + 1][1][(k + t) % d] += dp[i][0][k];
			}
			dp[i + 1][0][(k + x) % d] += dp[i][0][k];
		}
	}

	return dp[n][0][0] + dp[n][1][0];
}


//【完全マッチングの数え上げ（bit DP）】O(2^|S| |S|)
/*
* |S| = |T| なる二部グラフ (S, T) の完全マッチングの個数を返す．
*
* e[i][j] : S[i] と T[j] の間に辺が存在するなら true, なければ false．
* 
* → 列挙(グラフ)
*/
mint count_perfect_matching(vvb& e);


//【最長パス（グラフ上 DP）】O(|V| + |E|)
/*
* 有向非巡回グラフ g の最長パスの長さを返す．
*
* len[i] : 頂点 i からの最長パスの長さを格納する．
* 戻り値 : g の最長パスの長さ
* 
* → グラフ最適化.h
*/
int longest_path(Graph& g, vi& len);


//【独立集合の数え上げ（木の状態 DP）】O(|V|)
/*
* 木 g の独立集合の個数を返す．
* 
* → グラフ.h
*/
mint count_independent_set(Graph& g);


//【部分木の数え上げ（全方位木 DP）】O(|V|)
/*
* 木 g の部分木のうち頂点 i を含むものの個数を cnt[i] に格納する．
* 
* → グラフ.h
*/
void count_subtree(Graph& g, vm& cnt);


//【キング配置問題（盤上 DP）】O(h w 1.6^w)
/*
* h * w の穴あき盤に互いの効きに入らないようにキングを配置する方法が何通りあるかを返す．
*
* hall[i][j] : マス (i, j) に穴が空いているなら true，さもなくば false
* 戻り値 : キングを配置する場合の数
* 
* → 格子.h
*/
mint king_problem(vvb& hall);


//【部分列の数え上げ（部分列 DP）】O(k n)
/*
* k = 26 種類の英小文字からなる長さ n の文字列の部分列の個数を返す．
* 
* → 部分列.h
*/
mint count_subseq(const string& s);

