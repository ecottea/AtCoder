#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 数え上げ（部分列） ■■■■■


//【部分列の数え上げ（左優先，貰う DP）】O(n)
/*
* 列 a[0..n) の部分列の個数を返す（空列含む）
*
*（累積和で高速化した部分列 DP）
*/
template <class T>
mint count_unique_subsequences_Lpri_getDP(const vector<T>& a) {
	// verify : https://judge.yosupo.jp/problem/number_of_subsequences

	int n = sz(a);

	// dp[i] : a[0..i) の部分列で，a[i-1] を選ぶものの個数
	//	ただし同じ部分列については選ぶ要素は左優先とし，重複して数えないよう気をつける．
	vm dp(n + 1);
	dp[0] = 1;

	// acc[i] : Σdp[0..i)
	vm acc(n + 2);
	acc[1] = 1;

	// prv_i[j] : a[0..i) に現れる要素 j の最も右の位置
	unordered_map<T, int> prv;

	// 累積和で高速化した貰う DP
	rep(i, n) {
		T j = a[i];
		int pi = prv.count(j) ? prv[j] : -1;

		// a[i] = j を選ぶなら，左優先の規則より直前に現れた j も選んでいなければならない．
		// よって直前に選ばれた i' としては i'∈(pi..i) なるもののみを考えれば良い．
		dp[i + 1] = acc[i + 1] - acc[pi + 1];
		acc[i + 2] = acc[i + 1] + dp[i + 1];

		// j が位置 i に現れたことを記録する．
		prv[j] = i;
	}

	// 最後に選んだ要素は何でも良い．
	return acc[n + 1];
}


//【部分列の数え上げ（右優先，貰う DP）】O(n)
/*
* 列 a[0..n) の部分列の個数を返す（空列含む）
*
*（総和で高速化したインライン部分列 DP）
*/
template <class T>
mint count_unique_subsequences_Rpri_getDP(const vector<T>& a) {
	// verify : https://judge.yosupo.jp/problem/number_of_subsequences

	int n = sz(a);

	// b[i] : a[i] が a[0..n) の何種類目の要素か
	vi b(n);
	unordered_map<T, int> ids; int id = 0;

	// a[0..n) の要素そのものには興味が無いので，適当に圧縮して b[0..n) にしてしまう．
	rep(i, n) {
		auto it = ids.find(a[i]);
		if (it != ids.end()) b[i] = it->second;
		else {
			ids[a[i]] = id;
			b[i] = id++;
		}
	}

	// dp_i[j] : b[0..i) の空でない部分列で，最後が j であるものの個数．
	//	ただし同じ部分列については選ぶ要素は右優先とし，重複して数えないよう気をつける．
	vm dp(id);

	// sum_i : b[0..i) の空でない部分列の個数
	mint sum = 0;

	rep(i, n) {
		int j = b[i];

		// b[i] = j を選ぶと，最後が j である列が今までの列の個数 + 1 になる．
		// 今までの列で最後が j であるものについて b[i] = j を選ばないことは禁止されている．
		mint old = dp[j];
		dp[j] = sum + 1; // += ではない

		// 総和の差分更新
		sum = sum - old + dp[j];
	}

	// 空列の分を加算
	return sum + 1;
}


//【部分列の数え上げ（左優先，配る DP）】O(26 n)
/*
* 英小文字からなる文字列 s[0..n) の部分列の個数を返す（空列含む）
*
*（前処理で高速化した部分列 DP）
*/
mint count_unique_subsequences_Lpri_giveDP(const string& s) {
	// 参考 : https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd

	int n = sz(s);
	const int C = 26;

	// nxt[i][c] : s[i..n) で最初に文字 c が現れる位置（無いなら n）
	vvi nxt(n + 1, vi(C, n));
	repir(i, n - 1, 0) {
		rep(c, C) {
			nxt[i][c] = nxt[i + 1][c];
		}
		nxt[i][s[i] - 'a'] = i;
	}

	// dp[i + 1] : s[0..i] から得られる s[i] を含む部分列の個数．
	//	ただし同じ部分列については選ぶ要素は左優先とし，重複して数えないよう気をつける．
	vm dp(n + 1);
	dp[0] = 1; // 空文字列に対応

	// 配る DP
	rep(i, n) {
		// c : 次に選ぶ文字
		rep(c, C) {
			// s[i..n) で最初に文字 c が現れる位置
			int j = nxt[i][c];

			// もう c が現れないなら c を選ぶことはできない．
			if (j == n) continue;

			// 課した制約のため最も左の c 以外は選べない．
			dp[j + 1] += dp[i];
		}
	}

	// 最も右の選んだ文字について場合分けし結果を足し合わせる．
	mint res = 0;
	repi(i, 0, n) res += dp[i];

	return res;
}


//【部分列の数え上げ（右優先，配る DP）】O(26 n)
/*
* 英小文字からなる文字列 s[0..n) の部分列の個数を返す（空列含む）
*/
mint count_unique_subsequences_Rpri_giveDP(const string& s) {
	int n = sz(s);
	const int C = 26;

	// dp[i][c] : s[0..i) の部分列で，最後に選んだ文字が c であるものの個数
	//	ただし同じ部分列については選ぶ要素は右優先とし，重複して数えないよう気をつける．
	vvm dp(n + 1, vm(C + 1));
	dp[0][C] = 1;

	rep(i, n) rep(c, C + 1) {
		// s[i] を選ばない場合（s[i] = c の場合は右優先規則より許されない）
		if (c != s[i] - 'a') dp[i + 1][c] += dp[i][c];

		// s[i] を選ぶ場合
		dp[i + 1][s[i] - 'a'] += dp[i][c];
	}

	return accumulate(all(dp[n]), mint(0));
}


//【共通部分列の数え上げ（左優先，貰う DP）】O(n m)
/*
* a[0..n) と b[0..m) の共通部分列の種類数を返す（空列含む）
*/
template <class T>
mint count_unique_common_subsequences_Lpri_getDP(const vector<T>& a, const vector<T>& b) {
	int n = sz(a), m = sz(b);

	// a[0..n), b[0..m) の要素そのものには興味が無いので，適当に圧縮して A[0..n), B[0..m) にしてしまう．
	vi A(n), B(m);
	unordered_map<T, int> ids; int C = 0;
	rep(i, n) {
		auto it = ids.find(a[i]);
		if (it != ids.end()) A[i] = it->second;
		else ids[a[i]] = A[i] = C++;
	}
	rep(j, m) {
		auto it = ids.find(b[j]);
		if (it != ids.end()) B[j] = it->second;
		else ids[b[j]] = B[j] = C++;
	}

	// prvA[i][c] : A[0..i) で最後に要素 c が現れる位置（無いなら -1）
	vvi prvA(n + 1, vi(C, -1));
	rep(i, n) {
		rep(c, C) prvA[i + 1][c] = prvA[i][c];
		prvA[i + 1][A[i]] = i;
	}

	// prvB[i][c] : B[0..i) で最後に要素 c が現れる位置（無いなら -1）
	vvi prvB(m + 1, vi(C, -1));
	rep(j, m) {
		rep(c, C) prvB[j + 1][c] = prvB[j][c];
		prvB[j + 1][B[j]] = j;
	}

	// dp[i][j] : A[0..i) と B[0..j) の共通部分列で，A[i-1] と B[j-1] を共に含むものの個数
	//	ただし A, B の部分列ともに左優先で要素を選択するものとする．
	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1; // 空列に対応

	// acc[i][j] : Σdp[0..i)[0..j)
	vvm acc(n + 2, vm(m + 2));
	repi(i, 1, n) acc[i][1] = 1;
	repi(j, 1, m) acc[1][j] = 1;

	// 累積和で高速化した貰う DP
	repi(i, 1, n + 1) repi(j, 1, m + 1) {
		acc[i][j] = acc[i][j - 1] + acc[i - 1][j] - acc[i - 1][j - 1] + dp[i - 1][j - 1];
		if (i == n + 1 || j == m + 1 || A[i - 1] != B[j - 1]) continue;

		int c = A[i - 1];
		int pi = prvA[i - 1][c], pj = prvB[j - 1][c];

		// A[i-1] = B[j-1] = c を選ぶなら，左優先の規則より直前に現れた c も選んでいなければならない．
		// よって dp(pi..i)(pj..j) からの寄与のみを考えれば良い．
		dp[i][j] = acc[i][j] - acc[i][pj + 1] - acc[pi + 1][j] + acc[pi + 1][pj + 1];
	}

	// 最後に選んだ要素は何でも良い．
	return acc[n + 1][m + 1];
}


//【共通部分列の数え上げ（左優先，配る DP）】O(26 n m)
/*
* s[0..n) と t[0..m) の共通部分列の種類数を返す（空列含む）
*/
mint count_unique_common_subsequences_Lpri_giveDP(const string& s, const string& t) {
	int n = sz(s), m = sz(t); const int C = 26;

	// nxt_s[i][c] : s[i..n) で最初に文字 c が現れる位置（無いなら n）
	vvi nxt_s(n + 1, vi(C, n));
	repir(i, n - 1, 0) {
		rep(c, C) nxt_s[i][c] = nxt_s[i + 1][c];
		nxt_s[i][s[i] - 'a'] = i;
	}

	// nxt_t[j][c] : t[i..m) で最初に文字 c が現れる位置（無いなら m）
	vvi nxt_t(m + 1, vi(C, m));
	repir(j, m - 1, 0) {
		rep(c, C) nxt_t[j][c] = nxt_t[j + 1][c];
		nxt_t[j][t[j] - 'a'] = j;
	}

	// dp[i][j] : s[0..i) と t[0..j) の共通部分列で，s[i-1] と t[j-1] を共に含むもの
	//	ただし s, t の部分列ともに左優先で要素を選択するものとする．
	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1; // 空列に対応

	// 配る DP
	repi(i, 0, n) repi(j, 0, m) {
		// c : 次に選ぶ文字
		rep(c, C) {
			// s[i..n) で最初に文字 c が現れる位置
			int ni = nxt_s[i][c];

			// t[j..m) で最初に文字 c が現れる位置
			int nj = nxt_t[j][c];

			// もう c が現れないなら c を選ぶことはできない．
			if (ni == n || nj == m) continue;

			// 課した制約のため選べる c が一意に限定される．
			dp[ni + 1][nj + 1] += dp[i][j];
		}
	}

	mint res = 0;
	repi(i, 1, n) repi(j, 1, m) res += dp[i][j];

	return res + 1;
}


//【共通部分列の数え上げ（右優先，配る DP）】O(26 n m)
/*
* s[0..n) と t[0..m) の共通部分列の種類数を返す（空列含む）
*/
mint count_unique_common_subsequences_Rpri_giveDP(const string& s, const string& t) {
	int n = sz(s), m = sz(t); const int C = 26;

	// dp[i][j][c] : s[0..i) と t[0..j) の共通部分列で，最後が c であるものの個数
	//	ただし s, t の部分列ともに右優先で要素を選択するものとする．
	vvvm dp(n + 1, vvm(m + 1, vm(C + 1)));
	dp[0][0][C] = 1;

	// 配る DP
	repi(i, 0, n) repi(j, 0, m) {
		// s[i] と s[j] を共に選ぶ場合
		if (i < n && j < m && s[i] == t[j]) {
			rep(c, C + 1) {
				dp[i + 1][j + 1][s[i] - 'a'] += dp[i][j][c];
			}
		}

		// s[i] を選ばず，t[j] を選ぶことを保留する場合
		if (i < n) {
			rep(c, C + 1) {
				// 右優先規則により，s[i] = c のとき選ばないことは許されない．
				if (s[i] - 'a' != c) {
					dp[i + 1][j][c] += dp[i][j][c];
				}
			}
		}

		// t[j] を選ばず，s[i] を選ぶことを保留する場合
		if (j < m) {
			rep(c, C + 1) {
				// 右優先規則により，t[j] = c のとき選ばないことは許されない．
				if (t[j] - 'a' != c) {
					dp[i][j + 1][c] += dp[i][j][c];
				}
			}
		}

		// s[i] も t[j] も選ばない場合を足しすぎたので引く
		if (i < n && j < m) {
			rep(c, C + 1) {
				if (s[i] - 'a' != c && t[j] - 'a' != c) {
					dp[i + 1][j + 1][c] -= dp[i][j][c];
				}
			}
		}
	}

	mint res = 0;
	rep(c, C + 1) res += dp[n][m][c];

	return res;
}


//【回文部分列の数え上げ】O(26 n^2)
/*
* 英小文字からなる文字列 s[0..n) の回文部分列の個数を返す（空列含む）
*
*（前処理で高速化した部分列 DP）
*/
mint count_unique_palindrome_subsequences(const string& s) {
	// 参考 : https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/2895

	int n = sz(s);
	const int C = 26;

	// nxt[i][c] : s[i..n-1] で最初に文字 c が現れる位置（無いなら n）
	vvi nxt(n + 1, vi(C, n));
	repir(i, n - 1, 0) {
		rep(c, C) {
			nxt[i][c] = nxt[i + 1][c];
		}
		nxt[i][s[i] - 'a'] = i;
	}

	// prv[i + 1][c] : s[0..i] で最後に文字 c が現れる位置（無いなら -1）
	vvi prv(n + 1, vi(C, -1));
	rep(i, n) {
		rep(c, C) {
			prv[i + 1][c] = prv[i][c];
		}
		prv[i + 1][s[i] - 'a'] = i;
	}

	// dp[i + 1][j] : 回文の前半が s[0..i] から，後半が s[j..n-1] から得られる
	//		s[i] と s[j] を含む部分列の個数．
	// ただし選ぶ要素は前半は左優先，後半は右優先とし，重複して数えないよう気をつける．
	vvm dp(n + 1, vm(n + 1));
	dp[0][n] = 1; // 空列に対応

	// 配る DP
	rep(i, n) repir(j, n, i + 1) {
		// c : 次に選ぶ文字
		rep(c, C) {
			// s[i..n-1] で最初に文字 c が現れる位置
			int l = nxt[i][c];

			// s[0..j] で最後に文字 c が現れる位置
			int r = prv[j][c];

			// もう c が現れないか前後が逆転するなら c を選ぶことはできない．
			if (l > r) continue;

			// 課した制約のため選べる c が一意に限定される．
			dp[l + 1][r] += dp[i][j];
		}
	}

	// 空列は除いて数え上げる．
	mint res = 0;
	repi(i, 1, n) repi(j, 0, n - 1) {
		// 長さが奇数の回文
		if (i == j + 1) {
			res += dp[i][j];
		}
		// 長さが偶数の回文
		else {
			// 真ん中を削れば長さが奇数の回文も作れるので倍カウント
			res += 2 * dp[i][j];
		}
	}

	// 空列の分を加算する．
	return res + 1;
}


//【指定部分列の数え上げ（位置区別あり）】O(n m)
/*
* s[0..n) の部分列のうち t[0..m) に一致するものの個数を返す．
* ただし部分列として同じでも，添字列として異なるものは区別する．
*
*（耳 DP）
*/
template <class T>
mint count_same_subsequences(const vector<T>& s, const vector<T>& t) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_h

	int n = sz(s), m = sz(t);

	// dp[i][j] : s[0..i) が部分列として t[0..j) を何個含むか
	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1;

	// 配る DP
	rep(i, n) {
		repi(j, 0, m) {
			// s[i] を採用しない場合
			dp[i + 1][j] += dp[i][j];

			// s[i] を t[j] に一致する文字として採用する場合
			if (j < m && s[i] == t[j]) {
				dp[i + 1][j + 1] += dp[i][j];
			}
		}
	}

	return dp[n][m];
}


//【共通部分列の数え上げ（位置区別あり）】O(n m)
/*
* s[0..n) の部分列と t[0..m) の部分列の組のうち両者が一致するものの個数を返す（空列含む）
* ただし部分列として同じでも，添字列として異なるものは区別する．
* 
*（二次元 DP）
*/
template <class T>
mint count_common_subsequences(const vector<T>& s, const vector<T>& t) {
	// verify : https://atcoder.jp/contests/abc130/tasks/abc130_e

	int n = sz(s), m = sz(t);

	// dp[i][j] : s[0..i) の部分列と t[0..j) の部分列の組のうち両者が一致するものの個数
	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1;

	// 配る DP
	repi(i, 0, n) repi(j, 0, m) {
		// s[i] を採用しない場合
		if (i < n) dp[i + 1][j] += dp[i][j];

		// t[j] を採用しない場合
		if (j < m) dp[i][j + 1] += dp[i][j];

		// s[i], t[j] を共に採用しない場合を足しすぎたので引いておく
		if (i < n && j < m) dp[i + 1][j + 1] -= dp[i][j];

		// s[i] と t[j] を共通部分列の文字として採用する場合
		if (i < n && j < m && s[i] == t[j]) dp[i + 1][j + 1] += dp[i][j];
	}

	return dp[n][m];
}


//【最長増加部分列の数え上げ（位置区別あり）】O(n log n)
/*
* 数列 a[0..n) の（狭義）最長増加部分列の (長さ, 個数) の組を返す．
* ただし部分列として同じでも，添字列として異なるものは区別する．
*
* 利用：【動的セグメント木（モノイド）】
*/
int op_clis(int a, int b) { return max(a, b); }
int e_clis() { return 0; } // max の単位元が -INF でなく 0 であることに注意
template <class T>
pair<int, mint> count_LIS(const vector<T>& a) {
	// verify : https://yukicoder.me/problems/no/992

	int n = sz(a);

	// a[0..n) を座標圧縮した結果を b[0..n) に格納する（番兵 -INF 付き）
	vector<T> a_uniqed(a); a_uniqed.push_back(numeric_limits<T>::lowest());
	uniq(a_uniqed); int m = sz(a_uniqed);
	vi b(n); rep(i, n) b[i] = lbpos(a_uniqed, a[i]);

	// seg[i][j] : 長さが i で右端の値が j であるような増加部分列の個数（最長以外は正しくない値が入る）
	vector<Dynamic_segtree<Add_monoid>> seg(n + 1, Dynamic_segtree<Add_monoid>(m));
	seg[0].set(0, 1);

	// dp_i[j] : b[0..i] までで右端の値が j であるような最長増加部分列の長さ
	segtree<int, op_clis, e_clis> dp(m);

	//（例）b[0..5) = [3, 1, 2, 2, 0] のとき
	//	dp_0[0..3) = [0, 0, 0, 0]
	//	dp_1[0..3) = [0, 0, 0, 1] (max(0, 0, 0) + 1 = 1)
	//	dp_2[0..3) = [0, 1, 0, 1] (max(0)       + 1 = 1)
	//	dp_3[0..3) = [0, 1, 2, 1] (max(0, 1)    + 1 = 2)
	//	dp_4[0..3) = [0, 1, 2, 1] (max(0, 1)    + 1 = 2)
	//	dp_5[0..3) = [1, 1, 2, 1] (max()        + 1 = 1)

	// j = b[i] を順に見ていく
	rep(i, n) {
		int j = b[i];

		// j を右端にもてるのは，それまでの右端が j 未満のもののみ．
		// よってその中での最長増加部分列の長さを求め，それに 1 を加える．
		int len = dp.prod(0, j);
		len++;

		// j を右端とするより長いものが作れれば更新する．
		// dp[j] 以外は更新されることはないので，更新は O(log n) で終わる．
		// この性質が dp テーブルのインライン化と相性が良い．
		int len_prv = dp.get(j);
		if (len >= len_prv) {
			dp.set(j, len);

			// 長さが len-1 で右端の数字が j 未満であるような増加部分列の個数を加算する．
			mint cnt = seg[len - 1].prod(0, j);
			seg[len].apply_left(j, cnt);
		}
	}

	// 右端の値を任意としたときの最長増加部分列の長さを得る．
	int len = dp.prod(0, m);
	mint cnt = seg[len].all_prod();

	return { len, cnt };
}


