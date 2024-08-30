#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 数え上げ（数の表示） ■■■■■


//【数の数え上げ（1 桁の数指定）】O(n B)
/*
* B 進数で n 桁の数 num 以下の正の整数のうち，各 i∈[0..n) と d∈[0..B) について
* 上から i 桁目が d である数の個数を cnt[i][d] に格納し cnt を返す．
*/
template <class T>
vector<vector<T>> count_digits(const string& num, int B = 10) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_dj

	int n = sz(num);

	// res[i][d] : 上から i 桁目が d である数の個数
	using vT = vector<T>;
	using vvT = vector<vT>;
	vvT res(n, vT(B));

	// powB[i] : B^i
	vT powB(n + 1);
	powB[0] = 1;
	rep(i, n) powB[i + 1] = powB[i] * B;

	// accR[i] : num[i..n) の値
	vT accR(n + 1);
	repir(i, n - 1, 0) accR[i] = (num[i] - '0') * powB[n - 1 - i] + accR[i + 1];
	
	// accL_i : num[0..i) の値
	T accL = 0;

	rep(i, n) {
		int num_i = num[i] - '0';

		// 第 i 桁より上位の桁で num より小さいことが確定している数からの寄与（前 0 可）
		T cnt = accL * powB[n - 1 - i];
		rep(d, B) res[i][d] += cnt;

		// 第 i 桁より上位の桁が num に等しい数からの寄与（前 0 可）
		repi(d, 0, num_i - 1) res[i][d] += powB[n - 1 - i];
		res[i][num_i] += accR[i + 1] + 1;

		// 第 i 桁が前 0 になる数からの寄与を取り除く．
		res[i][0] -= powB[n - 1 - i];

		accL = accL * B + num_i;
	}

	return res;
}


//【数の数え上げ（上限，種類数指定）】O(n B)
/*
* B 進数で n 桁の数 num 以下の正整数で，K 種類の数字からなるものの個数を返す．
*
* 制約：fm は B! まで計算可能
*/
mint count_numbers_K_kind(const vi& num, int K, const Factorial_mint& fm, int B = 10) {
	// verify : https://atcoder.jp/contests/abc194/tasks/abc194_f

	if (K > B) return 0;

	int n = sz(num);

	// pows[k][i] : k^i
	vvm pows(K + 1, vm(n));
	repi(k, 0, K) {
		pows[k][0] = 1;
		repi(i, 1, n - 1) pows[k][i] = pows[k][i - 1] * k;
	}

	mint res = 0;

	// step1. n 桁未満の数で K 種類の数字からなるものの個数を求める．

	// i : 0 でない最上位桁
	repir(i, n - 1, 1) {
		// sum : 使う数字 K 種類を固定したときの数の個数
		mint sum = 0;

		// k : 使用可能な数字の種類数，として包除原理を適用する．
		repir(k, K, 0) {
			sum += ((K - k) & 1 ? -1 : 1) * fm.bin(K - 1, k - 1) * pows[k][n - i - 1];
		}

		// 第 i 桁の選び方が [1..B) の B-1 通り
		// 桁の数字 K 種類の選び方が，使用が決定している第 i 桁の数字を除いて bin(B-1, K-1) 通り
		res += (B - 1) * fm.bin(B - 1, K - 1) * sum;
	}

	// step2. n 桁の num 未満の数で K 種類の数字からなるものの個数を求める．

	// used[d] : 数字 d が使用済か，used_cnt : 使用済の数字の種類数
	vb used(B); int used_cnt = 0;

	// i : num と一致しない最上位桁
	rep(i, n) {
		// c0 : num[i] 未満の数のうち，第 i 桁より上位で使用済でないものの種類数
		// c1 : num[i] 未満の数のうち，第 i 桁より上位で使用済であるものの種類数
		int c0 = 0, c1 = 0;

		int d_min = (i == 0 ? 1 : 0);
		repi(d, d_min, num[i] - 1) {
			if (used[d]) c1++;
			else c0++;
		}

		if (c0 > 0) {
			// c : 第 i 桁以上で使用済である数字の種類数
			int c = used_cnt + 1;
			mint sum = 0;
			repir(k, K, c) {
				sum += ((K - k) & 1 ? -1 : 1) * fm.bin(K - c, k - c) * pows[k][n - i - 1];
			}
			res += c0 * fm.bin(B - c, K - c) * sum;
		}

		if (c1 > 0) {
			// c : 第 i 桁以上で使用済である数字の種類数
			int c = used_cnt;
			mint sum = 0;
			repir(k, K, c) {
				sum += ((K - k) & 1 ? -1 : 1) * fm.bin(K - c, k - c) * pows[k][n - i - 1];
			}
			res += c1 * fm.bin(B - c, K - c) * sum;
		}

		// 以降で調べる数の第 i 桁は num[i] に確定する．
		used_cnt += !used[num[i]];
		used[num[i]] = true;
	}

	// step3. num が K 種類の数字からなるかを調べる．
	res += (int)(used_cnt == K);

	return res;
}


//【桁の数字の分布】O(n B)
/*
* B 進数で n 桁の数 num 以下の正の整数すべてについて，
* 桁の数字に現れる数字 d の個数を cnt[d] に格納し cnt を返す．
*/
template <class T = ll>
vector<T> digits_distribution(const string& num, int B = 10) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_dj

	int n = sz(num);

	using vT = vector<T>;
	using vvT = vector<vT>;
	using vvvT = vector<vvT>;

	// dp[i][f][t] : 以下の条件を満たす数に含まれる数字 t の個数（cnt は数の個数）：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//      d[0..i) の全てが '0' なら 2，さもなくば 0（前 0 フラグ）
	//      f はこれら 2 つのフラグの OR をとったもの
	vvvT dp(n + 1, vvT(1LL << 2, vT(B)));
	vvT cnt(n + 1, vT(1LL << 2));
	cnt[0][0 | 2] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		//--- case1: smaller = true, leading zero = true ---
		// d[i] = 0 にする場合
		cnt[i + 1][1 | 2] += cnt[i][1 | 2];

		// d[i] ∈ [1..B) にする場合
		repi(t, 1, B - 1) {
			dp[i + 1][1 | 0][t] += cnt[i][1 | 2];
			cnt[i + 1][1 | 0] += cnt[i][1 | 2];
		}

		//--- case2: smaller = true, leading zero = false ---
		// d[i] ∈ [0..B) にする場合
		rep(t, B) {
			dp[i + 1][1 | 0][t] += dp[i][1 | 0][t] * B;
			dp[i + 1][1 | 0][t] += cnt[i][1 | 0];
			cnt[i + 1][1 | 0] += cnt[i][1 | 0];
		}

		//--- case3: smaller = false, leading zero = true ---
		// d[i] = 0 にする場合
		if (x > 0) cnt[i + 1][1 | 2] += cnt[i][0 | 2];
		else cnt[i + 1][0 | 2] += cnt[i][0 | 2];

		// d[i] ∈ [1..num[i]) にする場合
		repi(t, 1, x - 1) {
			dp[i + 1][1 | 0][t] += cnt[i][0 | 2];
			cnt[i + 1][1 | 0] += cnt[i][0 | 2];
		}

		// d[i] = num[i] にする場合
		dp[i + 1][0 | 0][x] += cnt[i][0 | 2];
		cnt[i + 1][0 | 0] += cnt[i][0 | 2];

		//--- case4: smaller = false, leading zero = false
		rep(t, B) {
			// d[i] ∈ [0..num[i]) にする場合
			dp[i + 1][1 | 0][t] += dp[i][0 | 0][t] * x;
			if (t < x) {
				dp[i + 1][1 | 0][t] += cnt[i][0 | 0];
				cnt[i + 1][1 | 0] += cnt[i][0 | 0];
			}

			// d[i] = num[i] にする場合
			dp[i + 1][0 | 0][t] += dp[i][0 | 0][t];
			if (t == x) {
				dp[i + 1][0 | 0][x] += cnt[i][0 | 0];
				cnt[i + 1][0 | 0] += cnt[i][0 | 0];
			}
		}
	}

	vT res(B);
	rep(t, B) res[t] = dp[n][0 | 0][t] + dp[n][1 | 0][t];

	return res;
}


