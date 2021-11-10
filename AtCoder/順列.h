#pragma once
#include "header.h"
#include "座標圧縮.h"
#include "二項係数.h"
// ■■■■■ 順列，対称群 ■■■■■



//【転倒数】O(n log n)
/*
* 長さ n の配列 a の転倒数を返す．
*
* 利用：【座標圧縮】
*/
template <class T> ll inversion_number(vector<T>& a) {
	int n = sz(a);

	// a を [0, |a|) に座標圧縮した結果を b に格納する．
	vi b;
	int m = coordinate_compression(a, b);

	// fw[i] : 今まで見てきた範囲に値 i が何個あったか．
	fenwick_tree<int> fw(m);
	ll res = 0;
	rep(i, n) {
		fw.add(b[i], 1);

		// 自身より大きい数が今までに何個あったか調べ，加算する．
		res += fw.sum(b[i] + 1, m);
	}
	return res;
}


//【置換の分解】O(n)
/*
* [0..n) の置換 p を巡回置換の積に分解して cycles に格納する．
* p は任意の i を p[i] に動かすような置換を表す．
*/
int permutation_decomposition(const vi& p, vvi& cycles) {
	int n = sz(p);

	int m = 0;
	vb seen(n);

	rep(i, n) {
		// 抽出済のサイクルに含まれるなら次へ
		if (seen[i]) continue;

		// 新しいサイクルを発見
		cycles.push_back(vi());
		m++;

		// サイクルを順に格納していく．
		int s = i;
		do {
			cycles[m - 1].push_back(s);
			seen[s] = true;
			s = p[s];
		} while (s != i);
	}

	return m;
}


//【対称群の元の位数分布】O(n の分割数）（n = 50 くらいまで動く）
/*
* n 次対称群に位数 d の元が k 個あることを cnt[d] = k として格納する．
*
*（自然数の分割）
*
* 利用：【階乗と二項係数（mint利用）】
*/
void permutation_order_distribution(int n, map<int, mint>& cnt) {
	//【方法】
	// 巡回置換の積に分解したときの各巡回置換の長さの集合は n の分割になる．
	// 分割が得られれば位数は長さの最小公倍数である．
	// 
	// 各 n の分割 n = n_1 + ... + n_k に対応する置換の個数を求めることを考える．
	// まずそれぞれの n_i に [1..n] のどの元を割り当てるかが多項係数 (n_1, ..., n_k) 通り
	// 次にそれぞれの中でどの順に並べるかが (n_i - 1)! 通り（巡回対称性に注意）
	// 最後に同じ長さの巡回置換には区別は無いので，その階乗通りで割ればよい．

	map<int, int> ip; // ip[i] : 分割に i を何個用いたか

	factorial_mint fm(n);

	// n を k 以下の数で分割する．
	function<void(int, int)> rf = [&](int n, int k) {
		// 分割しきった場合
		if (n == 0) {
			vi a; // n の分割
			int l = 1; // n の分割の最小公倍数
			mint res = 1;

			repe(tmp, ip) {
				int v, c; // v : 数，c : 個数
				tie(v, c) = tmp;

				// n の分割を構成
				rep(hoge, c) a.push_back(v);

				// n の分割の最小公倍数を計算
				l = l / gcd(l, v) * v;

				// 上の方法での最後の調整を先に済ませておく
				res *= fm.fac(v - 1).pow(c) * fm.fac_inv(c);
			}
			res *= fm.multinomial(a);

			cnt[l] += res;

			return;
		}

		// 分割に使える数がもうない場合
		if (k == 0) {
			return;
		}

		// n が k 以上のときは，n を k と n-k に分割できる．
		if (n >= k) {
			ip[k]++;

			rf(n - k, k);

			if (--ip[k] == 0) ip.erase(k);
		}

		// これ以上 n の分割に k を使わない場合
		rf(n, k - 1);
	};

	rf(n, n);
}


//【順列の数え上げ（大小関係指定）】O(n^2)
/*
* '<', '>', '?' からなる長さ n-1 の文字列 s で指定される
* 大小関係を満たすような長さ n の順列の個数を返す．
*
*（挿入 DP）
*/
mint count_permutations(const string& s) {
	int n = sz(s) + 1;

	// dp[i][j] : 以下の条件を満たす長さ i + j の順列が何通りあるか．
	//	 i : 直前の桁より小さい数が何個使えるか
	//	 j : 直前の桁より大きい数が何個使えるか
	vvm dp(n, vm(n));
	rep(i, n) dp[i][n - 1 - i] = 1;

	// k = i + j
	repir(k, n - 2, 0) {
		// 直前より大きい数字を使う場合
		if (s[n - 2 - k] == '<') {
			// 右からの累積和を計算する
			dp[0][k] += dp[0][k + 1];
			repi(i, 1, k) {
				int j = k - i;
				dp[i][j] += dp[i][j + 1] + dp[i - 1][j + 1];
			}
		}
		// 直前より小さい数字を使う場合
		else if (s[n - 2 - k] == '>') {
			// 左からの累積和を計算する
			dp[k][0] += dp[k + 1][0];
			repi(j, 1, k) {
				int i = k - j;
				dp[i][j] += dp[i + 1][j] + dp[i + 1][j - 1];
			}
		}
		// 自由に数字を使える場合
		else {
			// 総和を計算する．
			mint sum = 0;
			repi(i, 0, k + 1) sum += dp[i][k + 1 - i];
			repi(i, 0, k) dp[i][k - i] = sum;
		}
	}

	return dp[0][0];
}


