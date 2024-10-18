#pragma once
#include "header.h"
#include "多項式.h"
#include "探索.h"
#include "行列.h"
#include "FPS(mint).h"
#include "数論変換.h"
// ■■■■■ 期待値 ■■■■■


//【しっぽ確率定理】
/*
* X が [0..∞) を台とする確率変数のとき，
*		E[X] = Σk∈[0..∞) P(X > k)
* 
* 証明：期待値の定義に従い計算すると以下のようになる：
*	E[X]
*	= Σx∈[0..∞) x P(X = x)
*	= Σx∈[0..∞) Σk∈[0..x) P(X = x)
*	= Σk∈[0..∞) Σx∈(k..∞) P(X = x)
*	= Σk∈[0..∞) P(X > k)
* 
* verify : https://atcoder.jp/contests/abc295/tasks/abc295_e
*/


//【ターン数の期待値 → 停止確率の和】
/*
* すごろくにおいて，
*	p[i] : マス i に止まる確率
*	e[i] : マス i に止まる（or 通り過ぎる）までのターン数の期待値
* とおくと，
*	e[i] = Σp[0..i)
* が成り立つ．
* 
* 証明：e[i] はマス [0..i) の中で止まったものの数と言い換えられる．
* 0-1 確率変数では期待値と 1 になる確率は一致するので，期待値の線形性から従う．
* 
* 備考：各マスに高々 1 回しか止まらない状況に一般化可能
* 
* verify : https://atcoder.jp/contests/abc280/tasks/abc280_e
*/


//【すごろく（目が任意，確率が任意）】O(n m)
/*
* a[0..m) の目が順に p[0..m) の確率で出る m 面サイコロを用いてすごろくを行う．
* 各 i∈[0..n] に対し，i マス以上進むのにかかるターン数の期待値を e[i] に格納し e を返す．
*/
template <class T>
vector<T> sugoroku(const vi& a, const vector<T>& p, int n) {
	// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e
	
	//【方法】
	// 出る目で場合分けを行って dp[i] を貰う DP で計算する．
	// a[j] の目が出た場合，
	//		a[j] の目を出すのに 1 ターン
	//		残り i-a[j] マス以上進むのにかかるターン数の期待値が dp[i-a[j]]
	// かかる．これを a[j] の目が出る確率 p[j] で重み付けながら足し合わせることで，
	//		dp[i] = Σj p[j] (1 + dp[i-a[j]])
	// なる遷移式を得る．

	int m = sz(a);

	// dp[i] : i マス以上進むのにかかるターン数の期待値
	vector<T> dp(n + 1);

	// 貰う DP
	repi(i, 1, n) rep(j, m) dp[i] += p[j] * (1 + dp[max(i - a[j], 0)]);

	return dp;
}


//【すごろく（確率が任意，mod 998244353）】O(n log n)
/*
* [1..m] の目が順に p[0..m) の確率で出る m 面サイコロを用いてすごろくを行う．
* 各 i∈[0..n] に対し，i マス以上進むのにかかるターン数の期待値を e[i] に格納し e を返す．
*
* 利用：【形式的冪級数】
*/
vm sugoroku(const vm& p, int n) {
	// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e

	//【方法】
	// 出る目で場合分けを行って dp[i] を求める漸化式を導出する．
	// 便宜上 p を 1-indexed とし p[1..m] として記述する．
	// 
	// j の目が出た場合，
	//		j の目を出すのに 1 ターン
	//		残り i-j マス以上進むのにかかるターン数の期待値が dp[i-j]
	// かかる．これを j の目が出る確率 p[j] で重み付けながら足し合わせることで，
	//		dp[i] = Σj∈[1..m] p[j] (1 + dp[i-j])
	// なる漸化式を得る．
	//
	// 先の漸化式を変形すると，Σp[1..m]=1 より
	//		dp[i] = 1 + Σj∈[1..m] p[j] dp[i-j]
	// となる．右辺に畳込みの形が現れていることに注目し，FPS を用いてさらに変形する．
	//		f(z) = Σi dp[i] z^i
	//		p(z) = Σi p[i] z^i（p[0] = 0 とする）
	// とおくと，dp[0] = 0 に注意して
	//		[z^i] f(z) = [z^i] (z/(1-z)) + [z^i] p(z) f(z)
	//		f(z) = z/(1-z) + p(z) f(z)
	//		f(z) = z / (1-z)(1-p(z))
	// を得る．

	int m = sz(p);

	MFPS dnm(p);
	dnm >>= 1;
	dnm *= -1;
	dnm[0] = 1;

	dnm.resize(m + 2);
	repir(j, m + 1, 1) dnm[j] -= dnm[j - 1];

	MFPS f = MFPS::monomial(1);
	f.resize(n + 1);
	f /= dnm;
	f.resize(n + 1);

	return f.c;
}


//【すごろく（確率が任意，mod 998244353）】O(n (log n)^2)
/*
* [1..k] の目が順に p[0..k) の確率で出る k 面サイコロを用いてすごろくを行う．
* 各 i∈[0..n] に対し，i マス以上進むのにかかるターン数の期待値を e[i] に格納し e を返す．
*
*（分割統治 FFT）
*/
vm sugoroku_dcFFT(const vm& p, int n) {
	// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e

	//【方法】
	// 出る目で場合分けを行って dp[i] を求める漸化式を導出する．
	// 便宜上 p を 1-indexed とし p[1..k] として記述する．
	// 
	// j の目が出た場合，
	//		j の目を出すのに 1 ターン
	//		残り i-j マス以上進むのにかかるターン数の期待値が dp[i-j]
	// かかる．これを j の目が出る確率 p[j] で重み付けながら足し合わせることで，
	//		dp[i] = Σj∈[1..k] p[j] (1 + dp[i-j])
	// なる漸化式が得られ，Σp[1..k]=1 を用いて変形すると
	//		dp[i] = 1 + Σj∈[1..k] p[j] dp[i-j]
	// となる．
	// 
	// 簡単のため n=4 として記述する．
	// dp[1..4] を左 dp[1..2] と右 dp[3..4] に分け，分割統治法を用いて先の漸化式に従い計算を行う．
	// 
	// (i) まず左部分内で完結する寄与の計算を行う：
	//		dp[1] += 1
	//		dp[2] += 1 + p[1] dp[1]
	// これで dp[1..2] は正しい値になった．
	// 
	// (ii) 次に左部分から右部分への寄与の計算を行う：
	//		dp[3] += p[1] dp[2] + p[2] dp[1]  (dp[m] += p[1] dp[m-1] + p[2] dp[m-2] + ...)
	//		dp[4] += p[2] dp[2] + p[3] dp[1]  (dp[m+1] += p[2] dp[m-1] + p[3] dp[m-2] + ...)
	// この式の右辺は既知の数列 p[1..3] と dp[1..2] の畳込みなのでまとめて計算できる．
	// 
	// (iii) 最後に右部分内で完結する寄与の計算を行う：
	//		dp[3] += 1
	//		dp[4] += 1 + p[1] dp[3]
	// これで dp[3..4] は正しい値になった．
	//
	// 実際には (i) と (iii) についてもさらに 3 ステップに分割して再帰的に計算を行う．

	int k = sz(p);

	// i マス以上進むのにかかるターン数の期待値
	vm dp(n + 1);

	function<void(int, int)> rf = [&](int l, int r) {
		// 単一要素のみになったら，どこからの寄与でもない定数 1 を加算して終了．
		if (r - l == 1) {
			dp[l] += 1;
			return;
		}

		// m : 中央位置
		int m = (l + r) / 2;

		// 左側を正しい値に設定する．
		rf(l, m);

		// 左側から右側への寄与をまとめて計算する．
		// x : 左側の値，y : 各移動距離にかかる係数
		vm x(m - l), y(r - l);
		repi(i, l, m - 1) x[i - l] = dp[i];
		rep(i, min(r - l, k)) y[i] = p[i];

		vm z = convolution(x, y);
		repi(i, m, r - 1) dp[i] += z[i - l - 1];

		// 右側を正しい値に設定する．
		rf(m, r);
	};
	rf(1, n + 1);

	return dp;
}


//【すごろく（確率が任意，mod 998244353）】O(n (log n)^2)
/*
* [1..k] の目が順に p[0..k) の確率で出る k 面サイコロを用いてすごろくを行う．
* 各 i∈[0..n] に対し，i マス以上進むのにかかるターン数の期待値を e[i] に格納し e を返す．
*
*（オンライン畳込み）
*
* 利用：【オンライン畳込み（片側固定，mod 998244353）】
*/
vm sugoroku_oc(vm p, int n) {
	// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e

	//【方法】
	// 出る目で場合分けを行って dp[i] を求める漸化式を導出する．
	// 便宜上 p を 1-indexed とし p[1..k] として記述する．
	// 
	// j の目が出た場合，
	//		j の目を出すのに 1 ターン
	//		残り i-j マス以上進むのにかかるターン数の期待値が dp[i-j]
	// かかる．これを j の目が出る確率 p[j] で重み付けながら足し合わせることで，
	//		dp[i] = Σj∈[1..k] p[j] (1 + dp[i-j])
	// なる漸化式が得られ，Σp[1..k]=1 を用いて変形すると
	//		dp[i] = 1 + Σj∈[1..k] p[j] dp[i-j]
	// となる．
	//
	// i → i+1 とすると
	//		dp[i+1] = 1 + Σj∈[1..k] p[j] dp[i+1-j]
	// となり，j → j+1 とすると
	//		dp[i+1] = 1 + Σj∈[0..k) p[j+1] dp[i-j]
	// となる．
	//		a[i] = p[i+1]（i∈[0..k)）
	// とおき，その他の a[i] を 0 とおけば，
	//		dp[i+1] = 1 + Σj∈[0..i] a[j] dp[i-j]
	// となる．これをオンライン畳込みで計算する．

	p.resize(n);
	Semi_online_convolution soc(p);

	vm dp(n + 1);

	rep(i, n) {
		soc.set(dp[i]);
		dp[i + 1] = 1 + soc[i];
	}

	return dp;
}


//【すごろく】O(n)
/*
* [1..m] の目が各 1/m の確率で出る m 面サイコロを用いてすごろくを行う．
* 各 i∈[0..n] に対し，i マス以上進むのにかかるターン数の期待値を e[i] に格納し e を返す．
*/
template <class T>
vector<T> sugoroku(int m, int n) {
	// verify : https://mojacoder.app/users/YSatUT/problems/can_you_stop_exactly

	//【方法】
	// 出る目で場合分けを行って dp[i] を貰う DP で計算する．
	// 
	// j の目が出た場合，
	//		j の目を出すのに 1 ターン
	//		残り i-j マス以上進むのにかかるターン数の期待値が dp[i-j]
	// かかる．これを j の目が出る確率 1/m で重み付けながら足し合わせることで，
	//		dp[i] = Σj∈[1..m] (1/m) (1 + dp[i-j])
	// なる遷移式を得る．
	//
	// 先の遷移式を変形すると，
	//		dp[i] = 1 + (1/m) Σdp[i-m..i)
	// となる．これは累積和を用いて高速化可能である．

	// dp[i] : i マス以上進むのにかかるターン数の期待値
	vector<T> dp(n + 1);

	// acc[i] : Σdp[0..i)
	vector<T> acc(n + 2);

	// 貰う DP
	repi(i, 1, n) {
		dp[i] = 1 + (acc[i] - acc[max(i - m, 0)]) / m;
		acc[i + 1] = acc[i] + dp[i];
	}

	return dp;
}


//【すごろく（振り出しに戻る付き）】O(n + k log k)
/*
* [1..m] の目が各 1/m の確率で出る m 面サイコロを用いてすごろくを行う．
* ただしマス a[0..k) に止まるとマス 0 まで戻される．
* マス 0 からスタートしてマス n に辿り着くまでの回数の期待値を返す．
* ゴール不可能なら (double)INFL を返す．
*
*（累積和で高速化した期待値 DP）
* 
* 利用：【一次多項式】
*/
double sugoroku_back_poly(int m, int n, vi a) {
	// verify : https://atcoder.jp/contests/abc189/tasks/abc189_f

	//【方法】
	// マス i に居るときの回数の期待値を e[i] とおくと，振り出しに戻るマスでなければ
	//		e[i] = 1 + Σe[i+1..i+m] / m
	// などとなるため後ろからの累積和で計算できる．一方振り出しに戻るマスでは
	//		e[a[j]] = e[0]
	// となるため循環参照が起こってしまう．
	//
	// そこで，e[0] = x とおき，x を含んだ式を許して e[0..n) を後ろから計算する．
	// 最後に e[0] について一次方程式を立ててそれを解けば x = e[0] が求まる．

	int k = sz(a);
	sort(all(a));

	// ゴール不可能な場合を判定
	int len = 1;
	rep(i, k) {
		// len : 振り出しに戻すマスが連続する個数
		if (i > 0 && a[i] == a[i - 1] + 1) len++;
		else len = 1;

		// 振り出しに戻すマスが m 個連続したらゴール不可能
		if (len == m) return (double)INFL;
	}

	// マス i からマス n に辿り着くまでの回数の期待値を e[i] とおく．
	// dp[i] : Σe[i..n)
	vector<Poly1<double>> dp(n + 1);

	// 変数 x = e[0]
	const Poly1<double> X(1., 0.);

	int pt = k - 1;
	repir(i, n - 1, 0) {
		Poly1<double> e(1);

		// 振り出しに戻すマスの場合
		if (pt >= 0 && i == a[pt]) {
			e = X;
			pt--;
		}
		// 通常のマスの場合
		else {
			Poly1<double> sub = (1 + i + m) <= n ? dp[i + 1 + m] : 0.;
			e = 1. + (dp[i + 1] - sub) * (1. / m);
		}

		dp[i] = dp[i + 1] + e;
	}

	// 一次方程式を解く．
	return (dp[0] - dp[1]).solve(X);
}


//【すごろく（振り出しに戻る付き）】O(n + k log k)
/*
* 1 から m の目が等確率で出る m 面サイコロを用いてすごろくを行う．
* ただしマス a[0..k) に止まるとマス 0 まで戻される．
* マス 0 からスタートしてマス n に辿り着くまでの回数の期待値を返す．
* ゴール不可能なら (long double)INFL を返す．
*
*（累積和で高速化した期待値 DP）
*
* 利用：【二分探索（実数）】
*/
template <class T = long double>
T sugoroku_back_bs(int m, int n, vi a) {
	// verify : https://atcoder.jp/contests/abc189/tasks/abc189_f

	//【方法】
	// マス i に居るときの回数の期待値を e[i] とおくと，振り出しに戻るマスでなければ
	//		e[i] = 1 + Σe[i+1..i+m] / m
	// などとなるため後ろからの累積和で計算できる．一方振り出しに戻るマスでは
	//		e[a[j]] = e[0]
	// となるため循環参照が起こってしまう．
	//
	// そこで，e[0] = e0 と決め打って e[0..n) を後ろから計算する．
	// その結果得られた e[0] が e0 以下ならまだ余裕があるとして二分探索を行う．

	int k = sz(a);
	sort(all(a));

	// ゴール不可能な場合を判定
	int len = 1;
	rep(i, k) {
		// len : 振り出しに戻すマスが連続する個数
		if (i > 0 && a[i] == a[i - 1] + 1) len++;
		else len = 1;

		// 振り出しに戻すマスが m 個連続したらゴール不可能
		if (len == m) return (T)INFL;
	}

	// e[0] = e0 と決め打って計算した答えが e0 以下かを返す．
	function<boolT)> okQ = [&](T e0) {
		// マス i からマス n に辿り着くまでの回数の期待値を e[i] とおく．
		// dp[i] : Σe[i..n)
		vector<T> dp(n + 1);

		int pt = k - 1; T e = 1;
		repir(i, n - 1, 0) {
			// 振り出しに戻すマスの場合
			if (pt >= 0 && i == a[pt]) {
				e = e0;
				pt--;
			}
			// 通常のマスの場合
			else {
				T sub = (1 + i + m) <= n ? dp[i + 1 + m] : T(0);
				e = T(1) + (dp[i + 1] - sub) * (T(1) / m);
			}

			dp[i] = dp[i + 1] + e;
		}

		return e <= e0;
	};

	return binary_search((T)INFL, (T)0, okQ);
}


//【クーポンコレクター問題】O(n^max(a))
/*
* 等確率で当たる n 枚のクーポンを各 a[i] (∈[0..3]) 枚揃えるまでの回数の期待値を返す．
*
*（期待値 DP）
*/
double coupon_collectors_problem(const vi& a) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_j

	int n = sz(a);

	vector<int> cnt(4);
	rep(i, n) {
		cnt[a[i]]++;
	}

	auto f1 = [&]() {
		double res = 0;
		repir(i, cnt[1], 1) res += 1. / i;
		res *= n;
		return res;
	};

	auto f2 = [&]() {
		// dp[i][j] : 残り 1, 2 個のクーポンが各 i, j 種類残っている場合の
		// 揃えるまでの残り試行回数の期待値
		vvd dp(n + 1, vd(n + 1));

		for (int j = 0; j <= cnt[2]; j++) {
			for (int i = 0; i + j <= cnt[1] + cnt[2]; i++) {
				if (i == 0 && j == 0) {
					dp[i][j] = 0;
					continue;
				}

				dp[i][j] = (double)n / (i + j);
				if (i > 0) {
					dp[i][j] += dp[i - 1][j] * i / (i + j);
				}
				if (j > 0) {
					dp[i][j] += dp[i + 1][j - 1] * j / (i + j);
				}
			}
		}

		return dp[cnt[1]][cnt[2]];
	};

	auto f3 = [&]() {
		// dp[i][j][k] : 残り 1, 2, 3 個のクーポンが各 i, j, k 種類残っている場合の
		// 揃えるまでの残り試行回数の期待値
		vvvd dp(n + 1, vvd(n + 1, vd(n + 1)));

		for (int k = 0; k <= cnt[3]; k++) {
			for (int j = 0; j + k <= cnt[2] + cnt[3]; j++) {
				for (int i = 0; i + j + k <= cnt[1] + cnt[2] + cnt[3]; i++) {
					if (i == 0 && j == 0 && k == 0) {
						dp[i][j][k] = 0;
						continue;
					}

					dp[i][j][k] = (double)n / (i + j + k);
					if (i > 0) {
						dp[i][j][k] += dp[i - 1][j][k] * i / (i + j + k);
					}
					if (j > 0) {
						dp[i][j][k] += dp[i + 1][j - 1][k] * j / (i + j + k);
					}
					if (k > 0) {
						dp[i][j][k] += dp[i][j + 1][k - 1] * k / (i + j + k);
					}
				}
			}
		}

		return dp[cnt[1]][cnt[2]][cnt[3]];
	};

	if (cnt[3] > 0) return f3();
	else if (cnt[2] > 0) return f2();
	else if (cnt[1] > 0) return f1();
	else return 0;
}


//【不正確な射撃】O(2^n n)
/*
* 位置 x[0..n) それぞれに的がある．左右それぞれに確率 1/3 でぶれる射撃を行うとき，
* 全ての的に命中するまでの射撃回数の期待値を返す．
*
*（bit DP）
*/
double blurred_shooting(const vi& x) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_ball

	int n = *max_element(all(x)) + 1;

	// 初期状態の的の集合
	int set = 0;
	repe(i, x) {
		set += (1 << i);
	}

	// dp[set] : set に的がある場合の射撃回数の期待値
	vd dp(1LL << n, INFL);
	vb seen(1LL << n);
	dp[0] = 0;
	seen[0] = true;

	// set に的がある場合の射撃回数の期待値を返す．
	function<double(int)> rf = [&](int set) {
		// 計算済なら結果を返す．
		if (seen[set]) return dp[set];
		seen[set] = true;

		// i : 狙う位置
		rep(i, n) {
			double sum = 0; // 的に当たった場合の残り回数の期待値の和
			int tgts = 0; // 当たる可能性のある的の数

			// 狙いが左にぶれた場合
			if (i > 0 && (set & (1 << (i - 1)))) {
				sum += rf(set - (1 << (i - 1)));
				tgts++;
			}

			// 狙い通りに弾が飛んだ場合
			if (set & (1 << i)) {
				sum += rf(set - (1 << i));
				tgts++;
			}

			// 狙いが右にぶれた場合
			if (i < n - 1 && (set & (1 << (i + 1)))) {
				sum += rf(set - (1 << (i + 1)));
				tgts++;
			}

			// 範囲内に的が無いような所を狙う意味は無いので無視する．
			if (tgts == 0) continue;

			// いずれかの的に命中するまでの回数の期待値が 3 / tgts，
			// いずれかの的に命中してからの回数の期待値が sum / tgts，
			// それらの和をとったものが i を狙った場合の回数の期待値となる．
			chmin(dp[set], (3 + sum) / tgts);
		}

		return dp[set];
	};

	return rf(set);
}


//【ランダムウォーク】
/*
* Random_walk<T>(int n) : O(1)
*	n 頂点 0 辺のグラフで初期化する．
*
* add_edge(int s, int t, T prob) : O(1)
*	有向辺 s→t を，選択確率 prob で追加する．
*	制約：任意の s について Σs→t p[s][t] = 1
*
* vT arrive_probability_to(int GL) : O(n^3)
*	各頂点から出発し GL に到着する確率のリストを返す．
*	制約：GL から GL 以外へ移動可能
*
* vT expected_turn_to(int GL) : O(n^3)
*	各頂点から出発し GL に初めて到着するまでのターン数の期待値のリストを返す．
*	制約：どの頂点からも GL に到達可能
*
* vT stationary_distribution() : O(n^3)
*	定常分布を返す．
*	制約：どの頂点からどの頂点へも移動可能
* 
* vT distribution(int ST, ll k) : O(n^3 log k)
*	ST から出発して k 回移動した後の確率分布を返す．
*
* 利用：【行列】，【線形方程式】
*/
template <class T>
class Random_walk {
	int n;

	// 推移確率行列（p[i][j] : i から j に移動する確率）
	vector<vector<T>> p;

public:
	// n 頂点 0 辺のグラフで初期化する．
	Random_walk(int n) : n(n), p(n, vector<T>(n)) {
		// verify : https://yukicoder.me/problems/no/813
	}
	Random_walk() : n(0) {}

	// 有向辺 s→t を，選択確率 prob で追加する．
	void add_edge(int s, int t, T prob) {
		// verify : https://yukicoder.me/problems/no/813

		p[s][t] += prob;
	}

	// 各頂点から出発し GL に到着する確率のリストを返す．
	vector<T> arrive_probability_to(int GL) {
		// verify : https://yukicoder.me/problems/no/813

		//【方法】
		// s から GL に到着する確率を x[s] とすると，線形方程式
		//		x[s] = Σs→t p[s][t] x[t] (s ≠ GL)
		//		x[GL] = 1
		// を得る．これを整理すると
		//		(1 - p[s][s])x[s] - Σs→t,t≠s p[s][t] x[t] = 0
		//		x[GL] = 1
		// となる．

		Matrix<T> mat(n); vector<T> vec(n);
		rep(i, n) rep(j, n) if (i != GL) mat[i][j] -= p[i][j];
		vec[GL] = 1;

		return gauss_jordan_elimination(mat, vec);
	}

	// 各頂点から出発し GL に初めて到着するまでのターン数の期待値のリストを返す．
	vector<T> expected_turn_to(int GL) {
		//【方法】
		// s→GL にかかるターン数の期待値を e[s] とすると，線形方程式
		//		e[s] = 1 + Σs→t p[s][t] e[t] (s ≠ GL)
		//		e[GL] = 0
		// を得る．これを整理すると
		//		(1 - p[s][s])e[s] - Σs→t,t≠s p[s][t] e[t] = 1
		//		e[GL] = 0
		// となる．

		Matrix<T> mat(n); vector<T> vec(n, 1);
		rep(i, n) rep(j, n) if (i != GL) mat[i][j] -= p[i][j];
		vec[GL] = 0;

		return gauss_jordan_elimination(mat, vec);
	}

	// 定常分布を返す．
	vector<T> stationary_distribution() {
		//【方法】
		// 定常分布を π[0..n) とすると，線形方程式
		//		π[t] = Σs→t p[s][t] π[s]
		//		Σπ[0..n) = 1
		// を得る．これを整理すると
		//		(1 - p[t][t])π[t] - Σs→t,t≠s p[s][t] π[s] = 0
		//		Σπ[0..n) = 1
		// となる．

		Matrix<T> mat(n); vector<T> vec(n);
		rep(i, n - 1) rep(j, n) mat[i][j] -= p[j][i];
		rep(j, n) mat[n - 1][j] = 1;
		vec[n - 1] = 1;

		return gauss_jordan_elimination(mat, vec);
	}

	// ST から出発して k 回移動した後の確率分布を返す．
	vector<T> distribution(int ST, ll k) {
		// verify : https://yukicoder.me/problems/no/2832

		Matrix<T> mat(n); vector<T> vec(n);
		rep(i, n) rep(j, n) mat[i][j] = p[j][i];
		vec[ST] = 1;

		vec = mat.pow(k) * vec;

		return vec;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Random_walk& rw) {
		rep(i, rw.n) {
			rep(j, rw.n) os << rw.p[i][j] << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【重み付きランダムウォーク】
/*
* Random_walk_weighted<T>(int n) : O(1)
*	n 頂点 0 辺のグラフで初期化する．
*
* add_edge(int s, int t, T w, T p) : O(1)
*	有向辺 s→t を，重み w，選択確率 p で追加する．
*
* vT solve(int t) : O(n^3)
*	各頂点から出発し t に初めて到着するまでの経路の重みの和の期待値のリストを返す．
*	制約：どの頂点からも t に到達可能
*
* 利用：【行列】，【線形方程式】
*/
template <class T>
class Weighted_random_walk {
	int n;
	Matrix<T> mat;
	vector<T> vec;

public:
	// n 頂点 0 辺のグラフで初期化する．
	Weighted_random_walk(int n) : n(n), mat(n, n), vec(n) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2171

		rep(i, n) mat[i][i] = 1;
	}
	Weighted_random_walk() : n(0) {}

	// 有向辺 s→t を，重み w，選択確率 p で追加する．
	void add_edge(int s, int t, T w, T p) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2171

		mat[s][t] -= p;
		vec[s] += w * p;
	}

	// 各頂点から出発し t に初めて到着するまでの経路の重みの和の期待値のリストを返す．
	vector<T> solve(int t) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2171

		Matrix<T> mat2(mat); vector<T> vec2(vec);
		rep(j, n) mat2[t][j] = (T)(t == j);
		vec2[t] = 0;

		vector<T> sol = gauss_jordan_elimination(mat2, vec2);

		return sol;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Weighted_random_walk& rw) {
		rep(i, rw.n) {
			rep(j, rw.n) os << rw.mat[i][j] << " ";
			os << " " << rw.vec[i] << endl;
		}
		return os;
	}
#endif
}; 


//【DAG 上のランダムウォーク】
/*
* DAG g 上のランダムウォークで始点 ST から終点 GL まで移動するのにかかるターン数の期待値は，
* Σv∈V\{GL} Pr(v に止まる) / Pr(v から出る | v に居る) で与えられる．
* 
* verify : https://atcoder.jp/contests/abc242/tasks/abc242_h
*/

