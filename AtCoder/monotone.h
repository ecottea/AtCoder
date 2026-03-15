#pragma once
#include "header.h"
#include "探索.h"
// ■■■■■ monotone 行列 ■■■■■


//【monotone】
/*
* 行列 a[0..h)[0..w) が monotone 行列であるとは，任意の i1 < i2 について，
*	argmin_j a[i1][j] ≦ argmin_j a[i2][j]
* が成り立つことをいう（下の行ほど最小値が右にある）
* 
* また大小を逆にした性質を anti-monotone という．
*/


//【monotone 性判定】O(h w)
/*
* 行列 a[0..h)[0..w) が monotone かを返す．NIL は無効値を表す．
*
* 制約：無効値は左下か右上にしか存在しない．
*/
template <class FUNC>
bool monotoneQ(int h, int w, const FUNC& a, ll NIL = 2 * INFL + 100) {
	int prv_j_min = -INF;

	rep(i, h) {
		// j_min : a の第 i 行の最小値のある列番号
		int j_min = -1; ll a_min = 2 * INFL + 10;

		rep(j, w) {
			ll val = a(i, j);

			// 無効値は無視する．
			if (val == NIL) continue;

			// 最小値が真に更新されれば列番号を記録する．
			if (chmin(a_min, val)) j_min = j;
		}

		// 上の行の最小値のある列よりも左で最小値が見つかったなら monotone ではない．
		if (j_min < prv_j_min) return false;

		prv_j_min = j_min;
	}

	return true;

	/* A の定義の雛形
	auto A = [&](int i, int j) {
		return a[i][j];
	};
	*/
}


//【anti-monotone 性判定】O(h w)
/*
* 行列 a[0..h)[0..w) が anti-monotone 行列かを返す
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
* 行列 a[0..h)[0..w) が totally monotone 行列であるとは，任意の i1 < i2, j1 < j2 について
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


//【totally monotone 性判定】O(h w^2)
/*
* 行列 a[0..h)[0..w) が monotone 行列かを返す
*/
template <class FUNC>
bool totally_monotoneQ(int h, int w, const FUNC& a) {
	rep(j1, w) repi(j2, j1 + 1, w - 1) {
		rep(i, h - 1) {
			if (a(i, j1) > a(i, j2) && a(i + 1, j1) < a(i + 1, j2)) return false;
		}
	}
	return true;
}


//【Monge】
/*
* 行列 a[0..h)[0..w) が Monge 行列であるとは，任意の i1 < i2, j1 < j2 について
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
* Monge 行列 a[0..h)[0..w) の例には以下のようなものがある：
*	線形結合：	a[i][j] = k b[i][j] + l c[i][j] （b,c:Monge，k,l≧0）
*	行列積：		a[i][j] = min_k (b[i][k] + c[k][j]) （b,c:Monge）
*	転置：		a[i][j] = b[j][i] （b:Monge）
*	間引き：		a[i][j] = b[p[i]][q[j]] （b:Monge, p,q:単調増加）
*	行一定：		a[i][j] = c[i]
*	列一定：		a[i][j] = c[j]
*	単調列の積：	a[i][j] = x[i] y[j] （x:単調増加，y:単調減少）
*				a[i][j] = x[i] y[j] （x:単調減少，y:単調増加）
*	区間和：		a[i][j] = Σc[i..j)
*	区間最小値：	a[i][j] = min c[i..j)
*	区間 AND：	a[i][j] = AND c[i..j)
*	下に凸：		a[i][j] = f(j-i) （f:下に凸）
*	領域和：		a[i][j] = Σc[i..j)[i..j)
* 
* また狭義上三角 Monge 行列 a[0..n)[0..n) の例には以下のようなものがある：
*	非負副対角：	a[i][i+1] ≧ 0，その他の a[i][j] = 0
*/


//【anti-Monge 行列の例】
/*
* anti-Monge 行列 a[0..h)[0..w) の例には以下のようなものがある：
*	-1 倍：		a[i][j] = -b[i][j] （b:Monge）
*	左右反転：	a[i][j] = b[i][m-1-j] （b:Monge）
*	上下反転：	a[i][j] = b[n-1-i][j] （b:Monge）
*	行一定：		a[i][j] = c[i]
*	列一定：		a[i][j] = c[j]
*	単調列の積：	a[i][j] = x[i] y[j] （x:単調増加，y:単調増加）
*				a[i][j] = x[i] y[j] （x:単調減少，y:単調減少）
*	区間和：		a[i][j] = Σc[i..j)
*	区間最大値：	a[i][j] = max c[i..j)
*	区間 OR：	a[i][j] = OR c[i..j)
*	上に凸：		a[i][j] = f(j-i) （f:上に凸）
*/


//【Monge でない行列の例】
/*
* Monge でない行列 a[0..h)[0..w) の例には以下のようなものがある：
*	丸め区間和：	a[i][j] = Boole[Σc[i..j) ≧ S]
*/


//【Monge 性判定】O(h w)
/*
* 行列 A[0..h)[0..w) が Monge かを返す．NIL は無効値を表す．
*
* 制約：無効値は右上または左下にしか存在しない．
*/
template <class FUNC>
bool mongeQ(int h, int w, const FUNC& A, ll NIL = 2 * INFL + 100) {
	// verify : https://atcoder.jp/contests/abc224/tasks/abc224_b

	vvl a(h, vl(w));
	rep(i, h) rep(j, w) a[i][j] = A(i, j);
	dumpel(a);

	rep(i, h - 1) rep(j, w - 1) {
		// 左上や右下に無効値があったら Monge ではない．
		if (a[i][j] == NIL || a[i + 1][j + 1] == NIL) {
			if (a[i + 1][j] != NIL && a[i][j + 1] != NIL) return false;
			continue;
		}

		// 右上や左下に無効値があったら無視する．
		if (a[i + 1][j] == NIL || a[i][j + 1] == NIL) continue;

		if (a[i][j] + a[i + 1][j + 1] > a[i][j + 1] + a[i + 1][j]) return false;
	}
	return true;

	/* A の定義の雛形
	auto A = [&](int i, int j) {
		return a[i][j];
	};
	dump("is Monge? :", mongeQ(h, w, A));
	*/
}


//【monotone minima】O(w log h + h)
/*
* 与えられた monotone 行列 a[0..h)[0..w) について，各行の最小値の位置を並べたリストを返す．
* NIL は無効値を表す．行全てが NIL のときは，上方なら -1，下方なら w とする．
*
* 制約：無効値は右上または左下にしか存在しない．
*/
template <class FUNC>
vi monotone_minima(int h, int w, const FUNC& a, ll NIL = 2 * INFL + 100) {
	// 参考 : https://speakerdeck.com/tatyam_prime/monge-noshou-yin-shu
	// verify : https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary

	//【方法】
	// lsb の大きい行から順に最小値の位置を調べていく．
	// 1 つ lsb の大きい行の結果を参照することにより調べるべき範囲を各回 O(w) に制限できる．

	vi j_min(h);

	// i0 : 無効値以外が現れる最初の行番号
	int i0 = 0;
	while (i0 < h) {
		if (a(i0, 0) != NIL || a(i0, w - 1) != NIL) break;
		j_min[i0] = -1;
		i0++;
	}
	if (i0 == h) return j_min;

	// i1 : 無効値以外が現れる最後の行番号
	int i1 = h - 1;
	while (1) {
		if (a(i1, w - 1) != NIL || a(i1, 0) != NIL) break;
		j_min[i1] = w;
		i1--;
	}

	// 無効値を除いたときのバウンディングボックスの高さ
	int H = i1 - i0 + 1;

	// 0-indexed への変換のため 1 引いておく．
	i0--;

	// di : 行を調べる間隔 / 2（最大の 2 冪から始めて半分ずつにしていく）
	for (int di = 1 << msb(H); di > 0; di >>= 1) {
		// i : 調べる行番号（1-indexed）
		//	2 di ずつ増加させるので lsb は変化しない．
		int di2 = 2 * di;
		for (int i = di; i <= H; i += di2) {
			int jL = (i - di > 0 ? j_min[i0 + i - di] : 0);
			int jR = (i + di <= H ? j_min[i0 + i + di] : w - 1);

			ll a_min = 2 * INFL + 10;
			repi(j, jL, jR) {
				ll val = a(i0 + i, j);
				if (val == NIL) continue;

				if (chmin<ll>(a_min, val)) j_min[i0 + i] = j;
			}
		}
	}

	return j_min;

	/* A の定義の雛形
	ll NIL = 2 * INFL + 100;
	auto A = [&](int i, int j) {
		return 0LL;
	};
	auto pos = monotone_minima(h, w, A, NIL);
	*/
}


//【SMAWK algorithm】O(h + w)
/*
* 与えられた totally monotone 行列 a[0..h)[0..w) について，各行の最小値の位置を並べたリストを返す．
*/
template <class FUNC>
vi smawk(int h, int w, const FUNC& a) {
	// 参考 : https://speakerdeck.com/tatyam_prime/monge-noshou-yin-shu
	// verify : https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary

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

			if (i == 0 || a(i - 1, js[k + 1].back()) <= a(i - 1, js[k][pt])) {
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
			ll a_min = 2 * INFL + 10;
			int jR = (i + di <= h ? j_min[i + di - 1] : w - 1);

			while (pt < sz(js[k + 1]) && js[k + 1][pt] < jR) {
				if (chmin<ll>(a_min, a(i - 1, js[k + 1][pt]))) j_min[i - 1] = js[k + 1][pt];
				pt++;
			}
			if (chmin<ll>(a_min, a(i - 1, jR))) j_min[i - 1] = jR;
		}
	}

	return j_min;

	/* a の定義の雛形
	auto A = [&](int i, int j) {
		return 0LL;
	};
	*/
}


//【Monge DAG 最短路】O(n log n)
/*
* 頂点集合が [0..n] で，辺 s→t（s<t）のコストが n+1 次狭義上三角 Monge 行列の成分 A(s,t)
* で与えられる DAG における，頂点 0 から各頂点 [0..n] までの距離のリストを返す．
*/
template <class T = ll, class FUNC>
vector<T> monge_DAG_shortest_path(int n, const FUNC& A) {
	// 参考 : https://noshi91.hatenablog.com/entry/2023/02/18/005856
	// verify : https://yukicoder.me/problems/no/705

	//【方法】
	// (n+1)×(n+1) 行列 M を
	//		M[i][j] = dp[j] + c(j,i)
	// と定めると，M は狭義下三角な Monge 行列となるので，M の行最小値をオンラインで
	// （第 i 行の行最小値を求めてから第 i 列にアクセス）求められれば良い．

	// dp[i] : 第 i 行の最小値，j_min[j] : その位置
	vector<T> dp(n + 1, T(INFL)); vi j_min(n + 1, 0);
	dp[0] = 0;

	// M[i][j] を用いて第 i 行の（暫定）最小値 dp[i] を更新する．
	auto update = [&](int i, int j) {
		if (chmin(dp[i], dp[j] + A(j, i))) j_min[i] = j;
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

	/* A の定義の雛形
	auto A = [&](int s, int t) {
		return t - s;
	};
	*/
}


//【Monge DAG 最短路（長さ指定）】O(n (log n)^2)
/*
* 頂点集合が [0..n] で，辺 s→t（s<t）のコストが n+1 次狭義上三角 Monge 行列の成分 c(s,t)
* で与えられる DAG における，頂点 0 から n への長さ D のパスの最小コストを返す．
* c_max は |c(s,t)| の最大値とする．
*
* 利用：【Monge DAG 最短路】,【三分探索（最大値）】
*/
template <class T, class FUNC>
T alien_DP(int n, int D, const FUNC& c, T c_max) {
	// 参考 : https://noshi91.github.io/algorithm-encyclopedia/d-edge-shortest-path-monge
	// verify : https://atcoder.jp/contests/abc218/tasks/abc218_h

	//【方法】
	// 最短路問題は LP 定式化でき，パスの長さが D という制約をラグランジュ緩和する方針をとれる．
	// LP だから強双対性は実数の範囲で成り立つが，辺コストの Monge 性から実は整数の範囲でも成り立つ．

	//【備考】
	// 長さの指定が D1 以上 D2 以下という形であれば，ペナルティ関数を D λ から
	//		λ ≧ 0 ? D1 λ : D2 λ
	// に変更すれば良い．
	// 参考 : https://noshi91.hatenablog.com/entry/2022/01/13/001217

	// 各辺のコストを λ だけ増加させる代わりに総コストから D λ を引いたときのコストを返す．
	auto L = [&](T lambda) {
		auto c2 = [&](int s, int t) {
			return c(s, t) + lambda;
		};

		auto dist = monge_DAG_shortest_path<T>(n, c2);
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


//【Monge DAG 最短路（長さごと）】O(n^2 log n)
/*
* 重み付き DAG G を
*	頂点集合が [0..n]
*	辺 s→t（s<t）の重みが n+1 次狭義下三角 Monge 行列の成分 M[t][s]（向き注意！）
* と定める．各 k∈[0..n], i∈[0..n] について，
* 長さ k の頂点 0 から i へのパスの重みの最小値を格納した二次元リストを返す．
*/
template <class T>
vector<vector<T>> monge_DAG_shortest_path_all(const vector<vector<T>>& M) {
	// verify : https://atcoder.jp/contests/colopl2018-qual/tasks/colopl2018_qual_d

	//【方法】
	//		dp[k][i] : 頂点 0 から i までの長さ k のパスの重みの最小値
	// と定めると，遷移は
	//		dp[k+1][t] = MIN_s∈[0..t) (dp[k][s] + M[t][s])
	// となる．これは min-plus 代数における行列ベクトル積として
	//		dp[k+1] = M * dp[k]
	// と表される．
	// 
	// k を固定する．
	// M は下三角 Monge なのでその第 s 列に dp[k][s] を加えた行列も下三角 Monge である．
	// よって monotone minima で行最小値を求めることができる．

	//【注意】
	// M[s][t] でなく M[t][s] なのが気持ち悪いが，こうしないとキャッシュヒット率が悪くなる．

	//【備考】
	// 斜めに埋めるテクで log を落とせるらしいのだが詳細がわからない．

	int n = sz(M) - 1;

	// dp[k][i] : 頂点 0 から i までの長さ k のパスの重みの最小値
	vector<vector<T>> dp(n + 1, vector<T>(n + 1, T(INFL)));
	dp[0][0] = 0;

	rep(k, n) {
		// monotone minima
		vi j_min(n + 1);
		for (int di = 1 << msb(n + 1); di > 0; di >>= 1) {
			int di2 = 2 * di;
			for (int i = di; i <= n + 1; i += di2) {
				int jL = (i - di > 0 ? j_min[i - di - 1] : 0);
				int jR = (i + di <= n + 1 ? j_min[i + di - 1] : n);
				chmin(jR, i - 1);

				T a_min = T(INFL);
				repi(j, jL, jR) {
					T val = M[i - 1][j] + dp[k][j];

					if (chmin(a_min, val)) j_min[i - 1] = j;
				}
			}
		}

		repi(t, k + 1, n) dp[k + 1][t] = M[t][j_min[t]] + dp[k][j_min[t]];
	}

	return dp;
}


//【Monge DAG 最短路（長さごと，関数）】O(K n log n)
/*
* 重み付き DAG G を
*	頂点集合が [0..n]
*	辺 s→t（s<t）の重みが n+1 次狭義上三角 Monge 行列の成分 M(s,t)
* と定める．各 k∈[0..K], i∈[0..n] について，
* 長さ k の頂点 0 から i へのパスの重みの最小値を格納した二次元リストを返す．
*/
template <class T, class FUNC>
vector<vector<T>> monge_DAG_shortest_path_all(int K, int n, const FUNC& M) {
	//【方法】
	//		dp[k][i] : 頂点 0 から i までの長さ k のパスの重みの最小値
	// と定めると，遷移は
	//		dp[k+1][t] = MIN_s∈[0..t) (dp[k][s] + M(s,t))
	// となる．これは min-plus 代数における行列ベクトル積として
	//		dp[k+1] = M^T * dp[k]
	// と表される．
	// 
	// k を固定する．
	// M^T は下三角 Monge なのでその第 s 列に dp[k][s] を加えた行列も下三角 Monge である．
	// よって monotone minima で行最小値を求めることができる．

	// dp[k][i] : 頂点 0 から i までの長さ k のパスの重みの最小値
	vector<vector<T>> dp(K + 1, vector<T>(n + 1, T(INFL)));
	dp[0][0] = 0;

	rep(k, K) {
		// monotone minima
		vi j_min(n + 1);
		for (int di = 1 << msb(n + 1); di > 0; di >>= 1) {
			int di2 = 2 * di;
			for (int i = di; i <= n + 1; i += di2) {
				int jL = (i - di > 0 ? j_min[i - di - 1] : 0);
				int jR = (i + di <= n + 1 ? j_min[i + di - 1] : n);
				chmin(jR, i - 1);

				T a_min = T(INFL);
				repi(j, jL, jR) {
					T val = M(j, i - 1) + dp[k][j];

					if (chmin(a_min, val)) j_min[i - 1] = j;
				}
			}
		}

		repi(t, k + 1, n) dp[k + 1][t] = M(j_min[t], t) + dp[k][j_min[t]];
	}

	return dp;

	/* M の定義の雛形
	auto M = [&](int i, int j) {
		return 0LL;
	};
	*/
}


//【Monge DAG 最長路（長さごと）】O(n^2 (log n)^2)
/*
* 重み付き DAG G を
*	頂点集合が [0..n]
*	辺 s→t（s<t）の重みが n+1 次狭義下三角 Monge 行列の成分 M(t,s)（向き注意！）
* と定める．各 k∈[0..n], i∈[0..n] について，
* 頂点 0 から i への長さ k のパスの重みの最大値を格納した二次元リストを返す．
*/
template <class T>
vector<vector<T>> monge_DAG_longest_path_all(const vector<vector<T>>& M) {
	//【方法】
	//		dp[k][i] : 頂点 0 から i までの長さ k のパスの重みの最大値
	// と定めると，遷移は
	//		dp[k+1][t] = MAX_s∈[0..t) (dp[k][s] + M[t][s])
	// となる．これは max-plus 代数における行列ベクトル積として
	//		dp[k+1] = M * dp[k]
	// と表される．
	// 
	// k を固定する．
	// M は下三角 Monge なのでその第 s 列に dp[k][s] を加えた行列も下三角 Monge である．
	// さらに下三角部分を矩形に分割したそれぞれも Monge である．
	// よって各矩形領域について monotone minima で行最大値を求めることができ，
	// その結果を統合することで dp[k+1] が得られる．

	//【注意】
	// M[s][t] でなく M[t][s] なのが気持ち悪いが，こうしないとキャッシュヒット率が悪くなる．

	//【注意】
	// 下三角のまま行最大値を求めようとしても無効値が邪魔をして失敗する．

	int n = sz(M) - 1;

	// dp[k][i] : 頂点 0 から i までの長さ k のパスの重みの最大値
	vector<vector<T>> dp(n + 1, vector<T>(n + 1, -T(INFL)));
	dp[0][0] = 0;

	// rects_sml : 下三角部分を矩形に分割したときの小ブロック（はみ出しあり）
	vector<pii> rects_sml;

	// rects_lrg : 下三角部分を矩形に分割したときの大ブロック
	vector<tuple<int, int, int, int>> rects_lrg;

	// {(i,j) | l≦j<i<r} を矩形に分割する．
	function<void(int, int)> rf = [&](int l, int r) {
		// 小さくなったらはみ出しも許して打ち切って記録する．
		if (r - l <= 20) {
			rects_sml.emplace_back(l, r);
			return;
		}

		int m = (l + r) / 2;
		rects_lrg.emplace_back(m, r, l, m);

		rf(l, m);
		rf(m, r);
	};
	rf(0, n + 1);

	rep(k, n) {
		// M の下三角部分を分割した各小ブロック M[l..r)[l..r) について処理を行う．
		for (auto [l, r] : rects_sml) {
			// 矩形が小さいときは素朴に処理する．
			repi(t, l, r - 1) repi(s, l, t - 1) {
				chmax(dp[k + 1][t], dp[k][s] + M[t][s]);
			}
		}

		// M の下三角部分を分割した各大ブロック M[t1..t2)[s1..s2) について処理を行う．
		for (auto [t1, t2, s1, s2] : rects_lrg) {
			int Dt = t2 - t1;
			int Ds = s2 - s1;

			// monotone minima
			vi s_min(Dt);
			for (int di = 1 << msb(Dt); di > 0; di >>= 1) {
				int di2 = 2 * di;
				for (int i = di; i <= Dt; i += di2) {
					int jL = (i - di > 0 ? s_min[i - di - 1] : 0);
					int jR = (i + di <= Dt ? s_min[i + di - 1] : Ds - 1);

					T a_min = T(INFL);
					repi(j, jL, jR) {
						T val = -(M[t1 + i - 1][s2 - 1 - j] + dp[k][s2 - 1 - j]);

						if (chmin(a_min, val)) s_min[i - 1] = j;
					}
				}
			}

			// 注目ブロックからの影響を反映する．
			repi(t, t1, t2 - 1) {
				int s = s2 - 1 - s_min[t - t1];
				chmax(dp[k + 1][t], M[t][s] + dp[k][s]);
			}
		}
	}

	return dp;
}


//【Knuth-Yao speedup】
/*
* 与えられた Monge かつ単調なコスト c[0..n][0..n] に対し，
*	dp[l][r] = MIN_m∈(l..r) (dp[l][m] + dp[m][r]) + c[l][r]
* で定まる dp[0..n][0..n] を返す．
*
* c が単調であるとは，
*	∀l1, r1, l2, r2, [l1..r1) ⊂ [l2..r2) ⇒ c[l1][r1] ≦ c[l2][r2]
* が成り立つことをいう．
*/
template <class T>
vector<vector<T>> knuth_yao_speedup(const vector<vector<T>>& c) {
	// 参考 : https://topcoder-g-hatena-ne-jp.jag-icpc.org/spaghetti_source/20120915/1347668163.html

	int n = sz(c) - 1;

	// dp[l][r] : [l..r) のマージにかかる最小コスト
	vector<vector<T>> dp(n + 1, vector<T>(n + 1, INFL));
	rep(l, n) dp[l][l + 1] = c[l][l + 1];

	// sp[l][r] = m : [l..m) と [m..r) を最後にマージしたことを表す．
	vvi sp(n + 1, vi(n + 1, -1));
	rep(i, n) sp[i][i + 1] = i + 1;

	repir(l, n - 2, 0) repi(r, l + 2, n) {
		// [l..m) と [m..r) を最後にマージする場合を考える．
		//	調べる m の範囲を 1 つ短い区間の分割位置の間に限定している．
		repi(m, sp[l][r - 1], sp[l + 1][r]) {
			if (chmin(dp[l][r], dp[l][m] + dp[m][r])) sp[l][r] = m;
		}

		// [l..m) と [m..r) のマージにかかる m に依らないコストを加算する．
		dp[l][r] += c[l][r];
	}

	return dp;
}


//【monotone minima（再帰）】O(w log h + h)
/*
* 与えられた monotone 行列 a[0..h)[0..w) について，各行の最小値の位置を並べたリストを返す．
*/
template <class FUNC>
vi monotone_minima_rec(int h, int w, const FUNC& a) {
	// 参考 : https://future-architect.github.io/articles/20210707a/
	// verify : https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary

	//【方法】
	// ある行 i における行最小値の位置が列 j だとわかった場合，
	// a は monotone なので (i, j) の右上と左下はもう考慮しなくてよい．
	// この性質を利用して素直な分割統治法を使う．

	vi argmin(h);

	// 行 [iL..iR) についての答えを求める（答えが列 [jL..jR) の範囲にあることはわかっている）
	function<void(int, int, int, int)> rf = [&](int iL, int iR, int jL, int jR) {
		if (iR - iL <= 0) return;

		// iM : 行 [iL..iR) の真ん中の行の番号
		int iM = (iL + iR) / 2;

		// jM : 行 iM の中の最小要素のある列の番号
		int jM = -1; ll a_min = 2 * INFL + 10;
		repi(j, jL, jR - 1) if (chmin<ll>(a_min, a(iM, j))) jM = j;

		// 行 iM の行最小値の位置が jM であることを記録する．
		argmin[iM] = jM;

		// 左上と右下の部分を再帰的に調べていく．
		rf(iL, iM, jL, jM + 1);
		rf(iM + 1, iR, jM, jR);
	};
	rf(0, h, 0, w);

	return argmin;

	/* a の定義の雛形
	auto A = [&](int i, int j) {
		return 0LL;
	};
	*/
}


