#pragma once
#include "header.h"
#include "探索.h"
// ■■■■■ monotone 行列 ■■■■■


//【monotone】
/*
* 行列 a[0..n)[0..m) が monotone 行列であるとは，任意の i1 < i2 について，
*	argmin_j a[i1][j] ≦ argmin_j a[i2][j]
* が成り立つことをいう（下の行ほど最小値が右にある）
* 
* また大小を逆にした性質を anti-monotone という．
*/


//【monotone 性判定】O(n m)
/*
* 行列 a[0..n)[0..m) が monotone 行列かを返す
*/
template <class T>
bool monotoneQ(const vector<vector<T>>& a) {
	if (a.empty()) return true;

	int n = sz(a), m = sz(a[0]);

	int prv_j = -INF;
	rep(i, n) {
		int j_min = -1; T a_min = numeric_limits<T>::max();
		rep(j, m) if (chmin(a_min, a[i][j])) j_min = j;

		if (j_min < prv_j) return false;
		prv_j = j_min;
	}
	return true;
}


//【anti-monotone 性判定】O(n m)
/*
* 行列 a[0..n)[0..m) が anti-monotone 行列かを返す
*/
template <class T>
bool antimonotoneQ(const vector<vector<T>>& a) {
	if (a.empty()) return true;

	int n = sz(a), m = sz(a[0]);

	int prv_j = -INF;
	rep(i, n) {
		int j_max = -1; T a_max = numeric_limits<T>::lowest();
		rep(j, m) if (chmax(a_max, a[i][j])) j_max = j;

		if (j_max < prv_j) return false;
		prv_j = j_max;
	}
	return true;
}


//【totally monotone】
/*
* 行列 a[0..n)[0..m) が totally monotone 行列であるとは，任意の i1 < i2, j1 < j2 について
*	[ a[i1][j1] > a[i1][j2] ]
*	[ a[i2][j1] < a[i2][j2] ] ではない
* を満たすことをいう（Monge のように 2x2 小行列を全チェックするだけではだめなので注意）
* 
* 性質：
*	a が totally monotone ⇒ a は monotone
*	列ベクトルを 1 つの折れ線とみると，どの 2 つの折れ線も高々 1 回しか交差しない．
* 
* また大小を逆にした性質を anti-totally monotone という．
*/


//【Monge】
/*
* 行列 a[0..n)[0..m) が Monge 行列であるとは，任意の i1 < i2, j1 < j2 について
*	a[i1][j1] + a[i2][j2] ≦ a[i1][j2] + a[i2][j1]
* を満たすことをいう（右上や左下であれば無効値があっても構わない）
* 
* 性質：
*	a が Monge ⇒ a は totally monotone
*	条件は ∀i,j, a[i][j] + a[i+1][j+1] ≦ a[i][j+1] + a[i+1][j] と同値．
*	条件は「j を増やしたときの a[i][j] の増加量は i が小さいほど大きい」と解釈できる．
* 
* また大小を逆にした性質を anti-Monge という．
*/


//【Monge 行列の例】
/*
* Monge 行列 a[0..n)[0..m) の例には以下のようなものがある：
*	線形結合：	a[i][j] = k b[i][j] + l c[i][j] （b,c:Monge，k,l≧0）
*	行一定：		a[i][j] = c[i]
*	列一定：		a[i][j] = c[j]
*	単調列の積：	a[i][j] = x[i] y[j] （x:単調増加，y:単調減少）
*	区間和：		a[i][j] = Σc[i..j)
*	区間最小値：	a[i][j] = min c[i..j)
*	区間最大値：	a[i][j] = (-1) * max c[i..j)
*	凸関数：		a[i][j] = f(j-i) （f:下に凸）
*	領域和：		a[i][j] = Σc[i..j)[i..j)
* 
* また狭義上三角 Monge 行列 a[0..n)[0..n) の例には以下のようなものがある：
*	非負副対角：	a[i][i+1] ≧ 0，その他の a[i][j] = 0
*/


//【Monge でない行列の例】
/*
* Monge でない行列 a[0..n)[0..m) の例には以下のようなものがある：
*	丸め区間和：	a[i][j] = Boole[Σc[i..j) ≧ S]
*/


//【Monge 性判定】O(n m)
/*
* 行列 a[0..n)[0..m) が Monge 行列かを返す
*/
template <class T>
bool mongeQ(const vector<vector<T>>& a) {
	// verify : https://atcoder.jp/contests/abc224/tasks/abc224_b

	if (a.empty()) return true;

	int n = sz(a), m = sz(a[0]);
	rep(i, n - 1) rep(j, m - 1) {
		if (a[i][j] + a[i + 1][j + 1] > a[i][j + 1] + a[i + 1][j]) return false;
	}
	return true;
}


//【Monge 性判定（上三角）】O(n^2)
/*
* 行列 a[0..n)[0..n) の狭義上三角部分が Monge かを返す
*/
template <class T>
bool mongeQ_UT(const vector<vector<T>>& a) {
	if (a.empty()) return true;

	int n = sz(a);
	repi(i, 0, n - 3) repi(j, i + 2, n - 2) {
		if (a[i][j] + a[i + 1][j + 1] > a[i][j + 1] + a[i + 1][j]) return false;
	}
	return true;
}


//【anti-Monge 性判定（上三角）】O(n^2)
/*
* 行列 a[0..n)[0..n) の狭義上三角部分が anti-Monge かを返す
*/
template <class T>
bool antimongeQ_UT(const vector<vector<T>>& a) {
	if (a.empty()) return true;

	int n = sz(a);
	repi(i, 0, n - 3) repi(j, i + 2, n - 2) {
		if (a[i][j] + a[i + 1][j + 1] < a[i][j + 1] + a[i + 1][j]) return false;
	}
	return true;
}


//【monotone minima】O(w log h + h)
/*
* 与えられた monotone 行列 a[0..h)[0..w) について，各行の最小値の位置を並べたリストを返す．
*/
template <class T, class FUNC>
vi monotone_minima(int h, int w, const FUNC& a, T T_INF = INFL) {
	// 参考 : https://speakerdeck.com/tatyam_prime/monge-noshou-yin-shu
	// verify : https://atcoder.jp/contests/colopl2018-final/tasks/colopl2018_final_c

	//【方法】
	// lsb の大きい行から順に最小値の位置を調べていく．
	// 1 つ lsb の大きい行の結果を参照することにより調べるべき範囲を各回 O(w) に制限できる．

	vi j_min(h);

	// di : 行を調べる間隔 / 2（最大の 2 冪から始めて半分ずつにしていく）
	for (int di = 1 << msb(h); di > 0; di >>= 1) {
		// i : 調べる行番号（1-indexed）
		// 2 di ずつ増加させるので lsb は変化しない．
		for (int i = di; i <= h; i += di << 1) {
			int jL = (i - di > 0 ? j_min[i - di - 1] : 0);
			int jR = (i + di <= h ? j_min[i + di - 1] : w - 1);

			T a_min = T_INF;
			repi(j, jL, jR) if (chmin(a_min, a(i - 1, j))) j_min[i - 1] = j;
		}
	}

	return j_min;

	/* a の定義の雛形
	auto a = [&](int i, int j) {
		return 0;
	};
	*/
}


//【monotone minima（再帰）】O(m log n + n)
/*
* 与えられた monotone 行列 a[0..n)[0..m) について，各行 i∈[0..n) の argmin_j を並べたリストを返す．
*/
template <class T, class FUNC>
vi monotone_minima_recursive(int n, int m, const FUNC& a, T T_INF = INFL) {
	// 参考 : https://future-architect.github.io/articles/20210707a/
	// verify : https://atcoder.jp/contests/colopl2018-final/tasks/colopl2018_final_c

	//【方法】
	// ある行 i における行最小値の位置が列 j だとわかった場合，
	// a は monotone なので (i, j) の右上と左下はもう考慮しなくてよい．
	// この性質を利用して素直な分割統治法を使う．

	vi argmin(n);

	// 行 [iL..iR) についての答えを求める（答えが列 [jL..jR) の範囲にあることはわかっている）
	function<void(int, int, int, int)> rf = [&](int iL, int iR, int jL, int jR) {
		if (iR - iL <= 0) return;

		// iM : 行 [iL..iR) の真ん中の行の番号
		int iM = (iL + iR) / 2;

		// jM : 行 iM の中の最小要素のある列の番号
		int jM = -1; T a_min = T_INF;
		repi(j, jL, jR - 1) if (chmin(a_min, a(iM, j))) jM = j;

		// 行 iM の行最小値の位置が jM であることを記録する．
		argmin[iM] = jM;

		// 左上と右下の部分を再帰的に調べていく．
		rf(iL, iM, jL, jM + 1);
		rf(iM + 1, iR, jM, jR);
	};
	rf(0, n, 0, m);

	return argmin;

	/* a の定義の雛形
	auto a = [&](int i, int j) {
		return 0;
	};
	*/
}


//【SMAWK algorithm】O(h + w)
/*
* 与えられた totally monotone 行列 a[0..h)[0..w) について，各行の最小値の位置を並べたリストを返す．
*/
template <class T, class FUNC>
vi smawk(int h, int w, const FUNC& a, T T_INF = INFL) {
	// 参考 : https://speakerdeck.com/tatyam_prime/monge-noshou-yin-shu
	// verify : https://atcoder.jp/contests/colopl2018-final/tasks/colopl2018_final_c

	int K = msb(h);

	// js[k][pt] : lsb が k 未満である行と，それまでの行最小値をとり得ない列を捨てた行列について，
	//		その pt 番目の列に対応する a の列番号
	vvi js(K + 2);
	js[0].resize(w);
	iota(all(js[0]), 0);
	repi(k, 0, K) js[k + 1].reserve((h >> k) + 1);

	repi(k, 0, K) {
		int di = 1 << k, i = di, pt = 0;
		while (pt < sz(js[k])) {
			if (js[k + 1].empty()) {
				js[k + 1].push_back(js[k][pt++]);
				continue;
			}

			if (a(i - 1, js[k + 1].back()) <= a(i - 1, js[k][pt])) {
				if (i + di <= h) {
					js[k + 1].push_back(js[k][pt]);
					i += di;
				}
				pt++;
			}
			else {
				js[k + 1].pop_back();
				i -= di;
			}
		}
	}

	vi j_min(h);

	repir(k, K, 0) {
		int di = 1 << k, pt = 0;

		for (int i = di; i <= h; i += di << 1) {
			T a_min = T_INF;
			int jR = (i + di <= h ? j_min[i + di - 1] : w - 1);

			while (pt < sz(js[k + 1]) && js[k + 1][pt] < jR) {
				if (chmin(a_min, a(i - 1, js[k + 1][pt]))) j_min[i - 1] = js[k + 1][pt];
				pt++;
			}
			if (chmin(a_min, a(i - 1, jR))) j_min[i - 1] = jR;
		}
	}

	return j_min;

	/* a の定義の雛形
	auto a = [&](int i, int j) {
		return 0;
	};
	*/
}


//【Monge コスト完全 DAG 最短路】O(n log n)
/*
* 頂点集合が [0..n] で，辺 s→t（s<t）のコストが n+1 次狭義上三角 Monge 行列の成分 c(s,t)
* で与えられる DAG における頂点 0 から各頂点 [0..n] までの距離のリストを返す．
*/
template <class T, class FUNC>
vector<T> lowest_cost_path_monge_DAG(int n, const FUNC& c, T T_INF = INFL) {
	// 参考 : https://noshi91.hatenablog.com/entry/2023/02/18/005856
	// verify : https://yukicoder.me/problems/no/705

	//【方法】
	// (n+1)×(n+1) 行列 M を
	//		M[i][j] = dp[j] + c(j,i)
	// と定めると，M は狭義下三角な Monge 行列となるので，M の行最小値をオンラインで
	// （第 i 行の行最小値を求めてから第 i 列にアクセス）求められれば良い．

	// dp[i] : 第 i 行の最小値，j_min[j] : その位置
	vector<T> dp(n + 1, T_INF); vi j_min(n + 1, 0);
	dp[0] = 0;

	// M[i][j] を用いて第 i 行の（暫定）最小値 dp[i] を更新する．
	auto update = [&](int i, int j) {
		if (chmin(dp[i], dp[j] + c(j, i))) j_min[i] = j;
	};

	// dp(iL..iR] を計算する．
	// 呼び出す際は dp(0..iL] と M[iR][0..iL] の（暫定）行最小値が計算済みであること．
	function<void(int, int)> solve = [&](int iL, int iR) {
		if (iR - iL <= 1) return;
		int iM = (iL + iR) / 2;

		// 計算済：dp(0..iL], min M[iR][0..iL]
		repi(j, j_min[iL], j_min[iR]) update(iM, j); // まだ j_min[iR]≦iL なので大丈夫

		// 計算済：dp(0..iL], min M[iM][0..iR], min M[iR][0..iL]
		solve(iL, iM);

		// 計算済：dp(0..iM], min M[iR][0..iL]
		repi(j, iL + 1, iM) update(iR, j);

		// 計算済：dp(0..iM], min M[iR][0..iM]
		solve(iM, iR);
	};
	update(n, 0);
	solve(0, n);

	return dp;

	/* c の定義の雛形
	auto c = [&](int s, int t) {
		return t - s;
	};
	*/
}


//【Monge コスト完全 DAG 最短路（長さ D）】O(n (log n)^2)
/*
* 頂点集合が [0..n] で，辺 s→t（s<t）のコストが n+1 次狭義上三角 Monge 行列の成分 c(s,t)
* で与えられる DAG における 0 から n への長さ D のパスの最小コストを返す．
* c_max は c(s,t) の最大値とする．
*
* 利用：【Monge コスト完全 DAG 最短路】,【三分探索（最大値）】
*/
template <class T, class FUNC>
T alien_DP(int n, int D, const FUNC& c, T c_max) {
	// 参考 : https://noshi91.github.io/algorithm-encyclopedia/d-edge-shortest-path-monge
	// verify : https://atcoder.jp/contests/abc218/tasks/abc218_h

	//【方法】
	// 最短路問題は LP 定式化でき，パスの長さが D という制約をラグランジュ緩和する方針をとれる．
	// LP だから強双対性は実数の範囲で成り立つが，辺コストの Monge 性から実は整数の範囲でも成り立つ．

	// 各辺のコストを lambda だけ増加させる代わりに総コストから D lambda を引いたときのコストを返す．
	auto L = [&](T lambda) {
		auto c2 = [&](int s, int t) {
			return c(s, t) + lambda;
		};

		auto dist = lowest_cost_path_monge_DAG<T>(n, c2);
		return dist[n] - D * lambda;
	};

	auto lambda = ternary_search_max(-3 * c_max - 1, 3 * c_max + 1, L);
	return L(lambda);

	/* c の定義の雛形
	auto c = [&](int s, int t) {
		return t - s;
	};
	*/
}


