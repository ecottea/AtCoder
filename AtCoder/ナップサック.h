#pragma once
#include "header.h"
// ■■■■■ ナップサック問題 ■■■■■


//【0-1 ナップサック問題（重さが小）】O(n w_max)
/*
* 価値 v[i] と重さ w[i] の定まった n 個の品物から，重さ w_max 以下で
* 価値が最大になるよう品物を選んだときの価値を返す．
* また各品物を選んだかどうかの一例を sel に格納する．
*
*（重さを状態とした状態 DP）
*/
ll knapsack01_problem(const vl& v, const vi& w, int w_max, vb* sel = nullptr) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_d

	int n = sz(v); // 品物の個数

	// dp[i][j] : 品物 [0..i) の中で重さ j 以下で実現できる最大価値
	vvl dp(n + 1, vl(w_max + 1));

	// DP で 0-1 ナップサック問題を解く．
	rep(i, n) {
		repi(j, 0, w_max) {
			// i 番目の品物を選ばない場合
			dp[i + 1][j] = dp[i][j];

			// i 番目の品物の重さが j 以下の場合
			if (w[i] <= j) {
				// i 番目の品物を選ぶ方が価値を高められるなら更新する．
				chmax(dp[i + 1][j], dp[i][j - w[i]] + v[i]);
			}						
		}
	}

	// DP 復元を行う．
	if (sel != nullptr) {
		*sel = vb(n);
		int j = w_max;
		repir(i, n - 1, 0) {
			// i 番目の品物を選んだ場合と選ばなかった場合で価値の差があれば選んだ証拠．
			if (dp[i + 1][j] != dp[i][j]) {
				(*sel)[i] = true;
				j -= w[i];
			}
		}
	}

	return dp[n][w_max];
}


//【0-1 ナップサック問題（価値が小）】O(n Σv[i])
/*
* 価値 v[i] と重さ w[i] の定まった n 個の品物から，重さ w_max 以下で
* 価値が最大になるよう品物を選んだときの価値を返す．
* また各品物を選んだかどうかの一例を sel に格納する．
*
*（価値を状態とした状態 DP）
*/
ll knapsack01_problem(const vi& v, vl& w, ll w_max, vb* sel = nullptr) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_e

	int n = sz(v); // 品物の個数

	// 重さを無視した合計価値 v_max の計算
	int v_max = 0;
	rep(i, n) {
		v_max += v[i];
	}

	// dp[i][j] : 品物 [0..i) で価値ちょうど j を実現できる最小重さ
	vvl dp(n + 1, vl(v_max + 1, INFL));

	// 品物がなくても価値 0 は実現でき，その最小重さは 0 である．
	repi(i, 0, n) {
		dp[i][0] = 0;
	}

	// DP で 0-1 ナップサック問題を解く．
	repi(i, 1, n) {
		repi(j, 1, v_max) {
			// i 番目の品物を選ばない場合
			dp[i][j] = dp[i - 1][j];

			// i 番目の品物の価値が j を超えていれば選べない．
			if (j - v[i - 1] < 0) {
				continue;
			}

			// i 番目の品物を選ぶ方が重さを小さくできるなら更新する．
			dp[i][j] = min(dp[i][j], dp[i - 1][j - v[i - 1]] + w[i - 1]);
		}
	}

	// 重さ w_max 以下で実現できた中での最大の合計価値を得る．
	int j = v_max;
	while (j >= 0 && dp[n][j] > w_max) {
		j--;
	}
	v_max = j;

	// DP 復元を行う．
	if (sel != nullptr) {
		*sel = vb(n);
		repir(i, n, 1) {
			// i 番目の品物を選んだ場合と選ばなかった場合で重さの差があれば選んだ証拠．
			if (dp[i][j] != dp[i - 1][j]) {
				(*sel)[i - 1] = true;
				j -= v[i - 1];
			}
		}
	}

	return v_max;
}


//【0-1 ナップサック問題（個数が小）】O(2^(n/2) n)
/*
* 価値が v[0..n) で重さが w[0..n) である品物から，重さ w_max 以下で
* 価値が最大になるよう品物を選んだときの価値を返す．
*
*（半分全列挙）
*/
ll knapsack01_problem(const vl& v, const vl& w, ll w_max) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_1_H

	int n = sz(v); // 品物の個数

	// 前後半それぞれの品物の個数
	int n1 = n / 2;
	int n2 = n - n1;

	// 前半の品物の部分集合の重さに価値を対応付けるリスト
	map<ll, ll> w1_to_v1;

	// 前半の品物の部分集合をビット全探索する．
	repb(set1, n1) {
		ll w1 = 0, v1 = 0;

		rep(i, n1) {
			if (set1 & (1 << i)) {
				w1 += w[i];
				v1 += v[i];
			}
		}

		// 重さが上限以下ならばリストに追加
		if (w1 <= w_max) {
			chmax(w1_to_v1[w1], v1);
		}
	}

	// リストを再利用し，その重さ以下での最大価値に更新しておく．
	ll v1_max = 0;
	repea(p, w1_to_v1) {
		chmax(p.second, v1_max);
		v1_max = p.second;
	}

	ll res = 0;

	// 後半の品物の部分集合をビット全探索する．
	repb(set2, n2) {
		ll w2 = 0, v2 = 0;

		rep(i, n2) {
			if (set2 & (1 << i)) {
				w2 += w[n1 + i];
				v2 += v[n1 + i];
			}
		}

		// 重さが上限以下ならば前半のリストと照合し最大価値を更新する．
		if (w2 <= w_max) {
			// 価値が w_max - w2 以下である前半の部分集合の最大価値を見つける．
			auto it = w1_to_v1.upper_bound(w_max - w2);
			it--;

			chmax(res, it->second + v2);
		}
	}

	return res;
}


//【0-1 ナップサック問題（個数が小）】O(2^(N/2) N)
/*
* 価値 v[i] と重さ w[i] の定まった N 個の品物から，重さ W 以下で
* 価値が最大になるよう品物を選んだときの価値を返す．
*
*（半分全列挙，グレイコード）
*/
ll knapsack01_problem_gray(const vl& v, vl& w, ll W) {
	// 参考：https://qiita.com/keymoon/items/6cf46473b5421bfe1d48
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_1_H

	// せっかくグレイコードを使っても，二分探索を使用しているせいで計算量は改善しない．
	// 重さがちょうど W という問題なら代わりにハッシュが使えるので計算量が改善する．

	int N = sz(v); // 品物の個数

	// 前後半それぞれの品物の個数
	int N_a = N / 2;
	int N_b = (N + 1) / 2;

	// 前半の品物の部分集合の重さと価値
	ll W_a = 0, V_a = 0;

	// 前半の品物の部分集合の重さに価値を対応付けるリスト
	map<ll, ll> W_a_to_V_a;

	// i = 0 に対応する処理
	W_a_to_V_a[0] = 0;

	// グレイコードを用いた差分更新を行うため，i = 1 からループを回す．
	repi(i, 1, (1 << N_a) - 1) {
		// 差分更新が行われるのがどのビットか
		int change_index = lsb(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) {
			W_a += w[change_index];
			V_a += v[change_index];
		}
		else {
			W_a -= w[change_index];
			V_a -= v[change_index];
		}

		// 重さが上限以下ならばリストに追加
		if (W_a <= W) {
			W_a_to_V_a[W_a] = max(W_a_to_V_a[W_a], V_a);
		}
	}

	// リストを再利用し，その重さ以下での最大価値に更新しておく．
	ll max_V_a = 0;
	for (auto& p : W_a_to_V_a) {
		p.second = max(p.second, max_V_a);
		max_V_a = p.second;
	}

	// 後半の品物の部分集合の重さと価値
	ll W_b = 0, V_b = 0;

	// i = 0 に対応する処理（v は全体での最大価値）
	ll V = W_a_to_V_a.rbegin()->second;

	// グレイコードを用いた差分更新を行うため，i = 1 からループを回す．
	repi(i, 1, (1 << N_b) - 1) {
		// 差分更新が行われるのがどのビットか
		int change_index = lsb(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) {
			W_b += w[N_a + change_index];
			V_b += v[N_a + change_index];
		}
		else {
			W_b -= w[N_a + change_index];
			V_b -= v[N_a + change_index];
		}

		// 重さが上限以下ならば前半のリストと照合し最高価値で更新する．
		if (W_b <= W) {
			// 価値が w - wb 以下である前半の部分集合の最大価値を見つける．
			auto it = W_a_to_V_a.upper_bound(W - W_b);
			it--;

			V = max(V, it->second + V_b);
		}
	}

	return V;
}


//【重さ最小化 0-1 ナップサック問題（価値が小）】O(N V)
/*
* 価値 v[i] と重さ w[i] の定まった N 個の品物から，価値がちょうど V で
* 重さが最小になるよう品物を選んだときの重さを返す（不可能なら -1 を返す．）
* また可能なら各品物を選んだかどうかの一例を sel に格納する．
*
*（価値を状態とした状態 DP）
*/
ll knapsack01_problem_minimize_weight(const vi& v, const vl& w, int V, vb& sel) {
	int N = sz(v); // 品物の個数

	// dp[i][j] : i 番目の品物までで価値ちょうど j を実現できる最小重さ
	// v[i], w[i] は 0-indexed で dp[i] は 1-indexed なので注意．
	vvl dp(N + 1, vl(V + 1, INFL));

	// 価値 0 を実現できる最小重さは 0 である．
	repi(i, 0, N) {
		dp[i][0] = 0;
	}

	// DP で 0-1 ナップサック問題を解く．
	repi(i, 1, N) {
		repi(j, 1, V) {
			// i 番目の品物を選ばない場合
			dp[i][j] = dp[i - 1][j];

			// i 番目の品物の価値が j を超えていれば選べない．
			if (j - v[i - 1] < 0) {
				continue;
			}

			// i 番目の品物を選ぶ方が重さを小さくできるなら更新する．
			dp[i][j] = min(dp[i][j], dp[i - 1][j - v[i - 1]] + w[i - 1]);
		}
	}

	// 価値がちょうど V を実現できた中で最小の重さを得る．
	ll W = dp[N][V];

	// 不可能なら終了．
	if (W == INFL) {
		return -1;
	}

	// DP 復元を行う．
	sel = vb(N);
	int j = V;
	repir(i, N, 1) {
		// i 番目の品物を選んだ場合と選ばなかった場合で重さの差があれば選んだ証拠．
		if (dp[i][j] != dp[i - 1][j]) {
			sel[i - 1] = true;
			j -= v[i - 1];
		}
	}

	return W;
}


//【ナップサック問題（重さが小）】O(N W)
/*
* 価値 v[i] と重さ w[i] の定まった N 個の品物から，重さ w 以下で
* 価値が最大になるよう品物を選んだときの価値を返す．
* また各品物を何個選んだかの一例を sel に格納する．
*
*（重さを状態とした状態 DP）
*/
ll knapsack_problem(const vl& v, const vi& w, int W, vi& sel) {
	int N = sz(v); // 品物の個数

	// dp[i][j] : i 番目の品物までで重さ j 以下で実現できる最大価値
	// v[i], w[i] は 0-indexed で dp[i] は 1-indexed なので注意．
	vvl dp(N + 1, vl(W + 1));

	// DP で ナップサック問題を解く．
	repi(i, 1, N) {
		repi(j, 1, W) {
			// i 番目の品物を選ばない場合
			dp[i][j] = dp[i - 1][j];

			// i 番目の品物の重さが j を超えていれば選べない．
			if (j - w[i - 1] < 0) {
				continue;
			}

			// i 番目の品物を選ぶ方が価値を高められるなら更新する．
			dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i - 1]] + v[i - 1]);
			dp[i][j] = max(dp[i][j], dp[i][j - w[i - 1]] + v[i - 1]);
		}
	}

	// DP 復元を行う．
	sel = vi(N);
	int i = N, j = W;
	while (i >= 1) {
		// i 番目の品物を選んだ場合と選ばなかった場合で価値の差があれば選んだ証拠．
		if (dp[i][j] > dp[i - 1][j]) {
			// 選んでいたなら 1 個分記録し，重さを減じておく．
			sel[i - 1]++;
			j -= w[i - 1];
		}
		else {
			// 選んでいなかったなら 1 つ前の品物について調べに行く．
			i--;
		}
	}

	return dp[N][W];
}


//【ナップサック問題（価値が小）】O(N V)
/*
* 価値 v[i] と重さ w[i] の定まった N 個の品物から，重さ W 以下で
* 価値が最大になるよう品物を選んだときの価値 V を返す．
* また各品物を何個選んだかの一例を sel に格納する．
* （価値を状態とした状態 DP）
*/
ll knapsack_problem(const vi& v, const vl& w, ll W, vi& sel) {
	int N = sz(v); // 品物の個数

	// 単位重さあたりの価値が最も高い品物を見つける．
	int best_v = -1;
	ll best_w = 1;
	rep(i, N) {
		if (best_v * w[i] < v[i] * best_w) {
			best_v = v[i];
			best_w = w[i];
		}
	}

	// 連続緩和問題を解いて合計価値の上界を得る．
	int V = (int)(((ll)W * best_v + best_w - 1) / best_w);

	// dp[i][j] : i 番目の品物までで価値ちょうど j を実現できる最小重さ
	// v[i], w[i] は 0-indexed で dp[i] は 1-indexed なので注意．
	vector<vector<ll>> dp(N + 1, vector<ll>(V + 1, INFL));

	// 価値 0 を実現できる最小重さは 0 である．
	repi(i, 0, N) {
		dp[i][0] = 0;
	}

	// DP でナップサック問題を解く．
	repi(i, 1, N) {
		repi(j, 1, V) {
			// i 番目の品物を選ばない場合
			dp[i][j] = dp[i - 1][j];

			// i 番目の品物の価値が j を超えていれば選べない．
			if (j - v[i - 1] < 0) {
				continue;
			}

			// i 番目の品物を選ぶ方が重さを小さくできるなら更新する．
			dp[i][j] = min(dp[i][j], dp[i - 1][j - v[i - 1]] + w[i - 1]);
			dp[i][j] = min(dp[i][j], dp[i][j - v[i - 1]] + w[i - 1]);
		}
	}

	// 重さ w 以下で実現できた中での最大の合計価値を得る．
	int j = V;
	while (j >= 0 && dp[N][j] > W) {
		j--;
	}
	V = j;

	// DP 復元を行う．
	sel = vi(N);
	int i = N;
	while (i >= 1) {
		// i 番目の品物を選んだ場合と選ばなかった場合で重さの差があれば選んだ証拠．
		if (dp[i][j] != dp[i - 1][j]) {
			// 選んでいたなら 1 個分記録し，価値を減じておく．
			sel[i - 1]++;
			j -= v[i - 1];
		}
		else {
			// 選んでいなかったなら 1 つ前の品物について調べに行く．
			i--;
		}
	}

	return V;
}


//【重さ最小化ナップサック問題（価値が小）】時間 O(N V) / 空間 O(N V)
/*
* 価値 v[i] と重さ w[i] の定まった N 個の品物から，価値がちょうど V で
* 重さが最小になるよう品物を選んだときの重さを返す（不可能なら -1 を返す．）
* また可能なら各品物を何個選んだかの一例を sel に格納する．
* （価値を状態とした状態 DP）
*/
ll knapsack_problem_minimize_weight(const vi& v, const vl& w, int V, vi& sel) {
	int N = sz(v); // 品物の個数

	// dp[i][j] : i 番目の品物までで価値ちょうど j を実現できる最小重さ
	// v[i], w[i] は 0-indexed で dp[i] は 1-indexed なので注意．
	vvl dp(N + 1, vl(V + 1, INFL));

	// 価値 0 を実現できる最小重さは 0 である．
	repi(i, 0, N) {
		dp[i][0] = 0;
	}

	// DP で重さ最小化ナップサック問題を解く．
	repi(i, 1, N) {
		repi(j, 1, V) {
			// i 番目の品物を選ばない場合
			dp[i][j] = dp[i - 1][j];

			// i 番目の品物の価値が j を超えていれば選べない．
			if (j - v[i - 1] < 0) {
				continue;
			}

			// i 番目の品物を選ぶ方が重さを小さくできるなら更新する．
			dp[i][j] = min(dp[i][j], dp[i - 1][j - v[i - 1]] + w[i - 1]);
			dp[i][j] = min(dp[i][j], dp[i][j - v[i - 1]] + w[i - 1]);
		}
	}

	// 不可能なら終了．
	if (dp[N][V] == INFL) {
		return -1;
	}

	// 可能なら DP 復元を行う．
	sel = vi(N);
	int i = N;
	ll j = V;
	while (i >= 1) {
		// i 番目の品物を選んだ場合と選ばなかった場合で重さの差があれば選んだ証拠．
		if (dp[i][j] != dp[i - 1][j]) {
			// 選んでいたなら 1 個分記録し，価値を減じておく．
			sel[i - 1]++;
			j -= v[i - 1];
		}
		else {
			// 選んでいなかったなら 1 つ前の品物について調べに行く．
			i--;
		}
	}

	return dp[N][V];
}


//【重さ最小化ナップサック問題（価値が小）】時間 O(N V) / 空間 O(V)
/*
* 価値 v[i] と重さ w[i] の定まった N 個の品物から，価値がちょうど V で
* 重さが最小になるよう品物を選んだときの個数を返す（不可能なら -1 を返す．）
* （価値を状態としたインライン状態 DP）
*/
ll knapsack_problem_minimize_weight(const vi& v, const vl& w, int V) {
	int N = sz(v); // 品物の個数

	// dp[i] : 今まで見てきた品物の中で価値ちょうど i を実現できる最小重さ
	// 何番目の品物まで見たかを添え字に持たないことでメモリを節約している．
	// その代わり DP 復元はできなくなってしまった．
	vl dp(V + 1, INFL);

	// 価値 0 を実現できる最小重さは 0 である．
	dp[0] = 0;

	// 配る DP で重さ最小化ナップサック問題を解く．
	rep(i, V) {
		rep(j, N) {
			// j 番目の品物の価値が高すぎれば選べない．
			if (i + v[j] > V) {
				continue;
			}

			// j 番目の品物を選ぶ方が個数を少なくできるなら更新する．
			chmin(dp[i + v[j]], dp[i] + w[j]);
		}
	}

	// 不可能なら終了．
	if (dp[V] == INFL) {
		return -1;
	}

	return dp[V];
}


//【個数制限付きナップサック問題（重さが小）】O(N W log m) 
/*
* 価値 v[i]，重さ w[i]，最大個数 m[i] の定まった N 個の品物から，
* 重さ W 以下で価値が最大になるよう品物を選んだときの価値を返す．
* また各品物を何個選んだかの一例を sel に格納する．
*
* 利用：【0-1 ナップサック問題（重さが小）】
*/
ll knapsack_problem_limited(const vl& v, const vi& w, const vl& m, int W, vi& sel) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_1_G

	int N = sz(v); // 品物の個数

	// 品物を 1, 2, 4, 8, ... 個ずつセットにして 1 つの品物とみなす．
	// i 番目のセットがどの品物だったかを kind[i] に記録する．
	vl v2;
	vi w2;
	vi kind;
	rep(i, N) {
		ll m_i = m[i];
		ll c = 1;
		while (c < m_i) {
			// 重さが W を超えたセットは無意味なので追加しない．
			if (w[i] * c > (ll)W) {
				goto NEXT_LOOP;
			}
			v2.push_back(v[i] * c);
			w2.push_back(w[i] * (int)c);
			kind.push_back(i);

			m_i -= c;
			c *= 2;
		}
		v2.push_back(v[i] * m_i);
		w2.push_back(w[i] * (int)m_i);
		kind.push_back(i);
	NEXT_LOOP:;
	}

	// セットに対して 0-1 ナップサック問題を解く．
	vb chosen2;
	auto res = knapsack01_problem(v2, w2, W, &chosen2);

	// 個数の情報を復元する．
	sel = vi(N);
	rep(i, sz(v2)) {
		if (chosen2[i]) {
			sel[kind[i]] += w2[i] / w[kind[i]];
		}
	}

	return res;
}


//【個数制限付きナップサック問題（価値が小）】O(N Σv[i]m[i] log m) 
/*
* 価値 v[i]，重さ w[i]，最大個数 m[i] の定まった N 個の品物から，
* 重さ W 以下で価値が最大になるよう品物を選んだときの価値を返す．
* また各品物を何個選んだかの一例を sel に格納する．
*
* 利用：【0-1 ナップサック問題（価値が小）】
*/
ll knapsack_problem_limited(const vi& v, const vl& w, const vl& m, ll W, vi& sel) {
	int N = sz(v); // 品物の個数

	// 品物を 1, 2, 4, 8, ... 個ずつセットにして 1 つの品物とみなす．
	// i 番目のセットがどの品物だったかを kind[i] に記録する．
	vi v2;
	vl w2;
	vi kind;
	rep(i, N) {
		ll m_i = m[i];
		ll c = 1;
		while (c < m_i) {
			// 重さが W を超えたセットは無意味なので追加しない．
			if (w[i] * c > W) {
				goto NEXT_LOOP;
			}
			v2.push_back(v[i] * (int)c);
			w2.push_back(w[i] * c);
			kind.push_back(i);

			m_i -= c;
			c *= 2;
		}
		v2.push_back(v[i] * (int)m_i);
		w2.push_back(w[i] * m_i);
		kind.push_back(i);
	NEXT_LOOP:;
	}

	// セットに対して 0-1 ナップサック問題を解く．
	vb chosen2;
	auto res = knapsack01_problem(v2, w2, W, &chosen2);

	// 個数の情報を復元する．
	sel = vi(N);
	rep(i, sz(v2)) {
		if (chosen2[i]) {
			sel[kind[i]] += v2[i] / v[kind[i]];
		}
	}

	return res;
}


//【個数制限付き重さ最小化ナップサック問題（価値が小）】O(N V log m)
/*
* 価値 v[i]，重さ w[i]，最大個数 m[i] の定まった N 個の品物から，価値がちょうど V で
* 重さが最小になるよう品物を選んだときの重さを返す（不可能なら -1 を返す．）
* また可能なら各品物を何個選んだかの一例を sel に格納する．
* 
* 利用：【重さ最小化 0-1 ナップサック問題】
*/
ll knapsack_problem_minimize_weight_limited(const vi& v, const vl& w, const vl& m, int V, vi& sel) {
	int N = sz(v); // 品物の個数

	// 品物を 1, 2, 4, 8, ... 個ずつセットにして 1 つの品物とみなす．
	// i 番目のセットがどの品物だったかを kind[i] に記録する．
	vi v2;
	vl w2;
	vi kind;
	rep(i, N) {
		ll m_i = m[i];
		ll c = 1;
		while (c < m_i) {
			// 価値が V を超えたセットは無意味なので追加しない．
			if (v[i] * c > (ll)V) {
				goto NEXT_LOOP;
			}
			v2.push_back(v[i] * (int)c);
			w2.push_back(w[i] * c);
			kind.push_back(i);

			m_i -= c;
			c *= 2;
		}
		v2.push_back(v[i] * (int)m_i);
		w2.push_back(w[i] * m_i);
		kind.push_back(i);
	NEXT_LOOP:;
	}

	// セットに対して重さ最小化 0-1 ナップサック問題を解く．
	vb chosen2;
	auto res = knapsack01_problem_minimize_weight(v2, w2, V, chosen2);

	// 不可能なら終了．
	if (res == -1) {
		return -1;
	}

	// 個数の情報を復元する．
	sel = vi(N);
	rep(i, sz(v2)) {
		if (chosen2[i]) {
			sel[kind[i]] += v2[i] / v[kind[i]];
		}
	}

	return res;
}


//【色付き 0-1 ナップサック問題（重さが小）】O(n w_max c_max)
/*
* 価値 v[i] と重さ w[i] と色 c[i] の定まった n 個の品物から，重さ w_max 以下かつ
* 色数 c_max 以下で価値が最大になるよう品物を選んだときの価値を返す．
*
*（重さと色数を状態としたインライン状態 DP）
*/
ll knapsack01_problem(const vl& v, const vi& w, const vi& c, int w_max, int c_max) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_knapsack

	int n = sz(v); // 品物の個数

	const int m = *max_element(all(c)) + 1;
	vector<vector<pli>> vws(m);
	rep(i, n) {
		vws[c[i]].push_back({ v[i], w[i] });
	}

	// dp_i[j][k] : 品物 [0..i) の中で重さ j 以下かつ k 色以下で実現できる最大価値
	vvl dp(w_max + 1, vl(c_max + 1));

	// インライン化した貰う DP
	// c : 色
	rep(c, m) {
		// 色 c の品物を選ぶ場合の dp テーブル
		auto ndp = dp;

		// 色 c の品物それぞれについて
		repe(vw, vws[c]) {
			// 注目している品物の価値 v と重さ w を得る．
			ll v; int w;
			tie(v, w) = vw;

			repir(j, w_max, 0) {
				repi(k, 0, c_max) {
					// 注目している品物を選ぶ場合
					if (w <= j) {
						chmax(ndp[j][k], ndp[j - w][k] + v);
					}
				}
			}
		}

		// 色 c の品物を 1 つでも選ぶなら色数は 1 増える．
		repi(j, 1, w_max) {
			repi(k, 1, c_max) {
				chmax(dp[j][k], ndp[j][k - 1]);
			}
		}
	}

	return dp[w_max][c_max];
}


