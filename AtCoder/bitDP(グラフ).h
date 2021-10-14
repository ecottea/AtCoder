#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ bit DP を用いるグラフの問題 ■■■■■



//【巡回セールスマン問題】O(|V|^2 2^|V|)
/*
* コスト付き有向グラフ g の最小コストハミルトン閉路のコストを返す．
* ハミルトン閉路が存在しない場合は -1 を返す．
*
*（bit DP）
*/
ll traveling_salesman_problem(const WGraph& g) {
	int n = sz(g);

	// dp[i][set] : 頂点 i から set を通り頂点 n - 1 までのハミルトンパスの最小コスト
	//	i !∈ set だが，n - 1 ∈ set なので注意．
	vvl dp(n, vl(1LL << n, INFL));
	vvb seen(n, vb(1LL << n));
	dp[n - 1][0] = 0;
	seen[n - 1][0] = true;

	// メモ化再帰用の関数の定義
	function<ll(int, int)> rf = [&](int s, int set) {
		// もし確定済ならば DP テーブルの値をそのまま返す．
		if (seen[s][set]) {
			return dp[s][set];
		}

		// s から出ている各辺 e について
		repe(e, g[s]) {
			auto t = e.to;
			auto c = e.cost;

			// e の行き先 t が set に含まれていなければ何もしない．
			if (!(set & (1 << t))) {
				continue;
			}

			// s → t と進む方がコストが小さければ更新する．
			chmin(dp[s][set], rf(t, set - (1 << t)) + c);
		}

		seen[s][set] = true;
		return dp[s][set];
	};

	// メモ化再帰を用いて bit DP を行う．
	auto res = rf(n - 1, (1 << n) - 1);
	return (res == INFL ? -1 : res);
}


//【最小コストハミルトンパス】O(|V|^2 2^|V|)
/*
* コスト付き有向グラフ g の最小コストハミルトンパスのコストを返す．
* ハミルトンパスが存在しない場合は -1 を返す．
*
*（bit DP）
*/
ll shortest_hamiltonian_path(const WGraph& g) {
	int n = sz(g);

	// dp[s][set] : 頂点 s から出発し set を通るハミルトンパスの最小コスト
	//	s !∈ set とする．
	vvl dp(n, vl(1LL << n, INFL));
	vvb seen(n, vb(1LL << n));
	rep(s, n) {
		dp[s][0] = 0;
		seen[s][0] = true;
	}

	// メモ化再帰用の関数の定義
	function<ll(int, int)> rf = [&](int s, int set) {
		// もし確定済ならば DP テーブルの値をそのまま返す．
		if (seen[s][set]) {
			return dp[s][set];
		}
		seen[s][set] = true;

		// s から出ている各辺 e について
		repe(e, g[s]) {
			auto t = e.to;
			auto c = e.cost;

			// e の行き先 t が set に含まれていなければ何もしない．
			if (!(set & (1 << t))) {
				continue;
			}

			// s → t と進む方がコストが小さければ更新する．
			chmin(dp[s][set], rf(t, set - (1 << t)) + c);
		}

		return dp[s][set];
	};

	// メモ化再帰を用いて bit DP を行う．
	ll res = INFL;
	rep(s, n) {
		chmin(res, rf(s, (1 << n) - 1 - (1 << s)));
	}
	return (res == INFL ? -1 : res);
}


//【単純パスの数え上げ】O(|V|^2 2^|V|)
/*
* グラフ g について単純パス s → t の個数を cnt[s][t] に格納する．
*
*（bit DP）
*/
void count_simple_path(const Graph& g, vvl& cnt) {
	int n = sz(g);

	// dp[s][t][set] : 単純パス s → t で途中 set を通るものの個数
	//	s !∈ set, t ∈ set とする．
	vvvl dp(n, vvl(n, vl(1LL << n)));
	vvvb seen(n, vvb(n, vb(1LL << n)));
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


