#pragma once
#include "header.h"
// ■■■■■ グラフ上の数え上げ問題 ■■■■■


//【単純パスの数え上げ】O(2^|V| |V|^2)
/*
* グラフ g について単純パス s → t の個数を cnt[s][t] に格納する．
*
*（bit DP）
*/
void count_simple_path(const Graph& g, vvl& cnt) {
	int n = sz(g);

	// dp[s][t][set] : 単純パス s → t で途中 set を通るものの個数
	//		s !∈ set, t ∈ set とする．
	vvvl dp(n, vvl(n, vl(1 << n)));
	vvvb seen(n, vvb(n, vb(1 << n)));
	rep(s, n) {
		dp[s][s][0] = 1;
		seen[s][s][0] = true;
	}

	// 単純パス s → t で途中 set を通るものの個数を返す．
	function<ll(int, int, int)> rf = [&](int s, int t, int set) {
		// もし確定済ならば DP テーブルの値をそのまま返す．
		if (seen[s][t][set]) return dp[s][t][set];
		seen[s][t][set] = true;

		// s から行ける各頂点 v について
		repe(v, g[s]) {
			// v が set に含まれていなければ何もしない．
			if (!(set & (1 << v))) continue;

			// s → v と進む単純パスの個数を加算する．
			dp[s][t][set] += rf(v, t, set - (1 << v));
		}

		return dp[s][t][set];
	};

	// 結果の格納
	cnt = vvl(n, vl(n));
	rep(s, n) {
		rep(t, n) {
			repb(set, n) {
				if ((set & (1 << s)) || !(set & (1 << t))) continue;
				cnt[s][t] += rf(s, t, set);
			}

			// 不動の場合もカウントする．
			if (s == t) cnt[s][t]++;
		}
	}
}


//【トポロジカルソートの数え上げ】O(2^|V| |E|)
/*
* 有向グラフ g をトポロジカルソートする方法が何通りあるかを返す．
*
*（bit DP）
*/
ll count_topological_sort(const Graph& g) {
	// verify : https://atcoder.jp/contests/abc041/tasks/abc041_d

	int n = sz(g);

	// dp[set] : 位置降順で [0..|set|) 番目の頂点までが set に対応する場合の数
	vl dp(1 << n);
	dp[0] = 1;

	repb(set, n) {
		// s : 次に割り当てる頂点
		rep(s, n) {
			// s が既に割り当て済の頂点なら何もしない．
			if (set & (1 << s)) continue;

			// 位置降順に頂点を対応させていっているので，
			// s → t なる頂点 t は既に選ばれていなければならない．
			bool choosable = true;
			repe(t, g[s]) {
				if (!(set & (1 << t))) {
					choosable = false;
					break;
				}
			}

			if (choosable) {
				dp[set + (1 << s)] += dp[set];
			}
		}
	}

	return dp[(1 << n) - 1];
}


