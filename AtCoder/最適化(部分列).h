#pragma once
#include "header.h"
// ■■■■■ 最適化（部分列） ■■■■■


//【最長増加部分列】O(n log n)
/*
* 数列 a[0..n) の（狭義）最長増加部分列の長さを返す．
*
*（二分探索で高速化したインライン DP）
*/
template <class T>
int LIS_length_to_val(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_x

	int n = sz(a);

	// dp_i[j] : a[0..i) で，長さが j である増加部分列の右端の値の最小値
	//	短い増加部分列はそれより長い増加部分列の部分列なので，広義単調増加性がある．
	vector<T> dp(n + 1, T(INFL));
	dp[0] = -T(INFL);

	//（例）a[0..5) = [4, 2, 3, 3, 1] のとき
	//	dp_0[0..5] = [-INF, INF, INF, INF, INF, INF]
	//	dp_1[0..5] = [-INF,   4, INF, INF, INF, INF]
	//	dp_2[0..5] = [-INF,   2, INF, INF, INF, INF]
	//	dp_3[0..5] = [-INF,   2,   3, INF, INF, INF]
	//	dp_4[0..5] = [-INF,   2,   3, INF, INF, INF]
	//	dp_5[0..5] = [-INF,   1,   3, INF, INF, INF]

	rep(i, n) {
		// 右端が a[i] 以上であるような増加部分列の最小長さ j を得る．
		int j = lbpos(dp, a[i]);

		// 長さ j の増加部分列の右端を a[i] に置き換える．
		dp[j] = a[i];

		// これより短いものは右端を a[i] に置き換えても得しないので無視できる．
		// これより長いものはそもそも右端を a[i] に置き換えることができない．
	}

	// 右端の値が設定できている長さの最大値を求める．
	int res = 0;
	repir(j, n, 1) {
		if (dp[j] != T(INFL)) {
			res = j;
			break;
		}
	}

	return res;
}


//【最長増加部分列（広義）】O(n log n)
/*
* 数列 a[0..n) の広義最長増加部分列の長さを返す．
*
*（二分探索で高速化したインライン DP）
*/
template <class T>
int weakly_LIS(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/abc134/tasks/abc134_e

	int n = sz(a);

	// dp_i[j] : a[0..i) までで，長さが j である広義増加部分列の右端の値の最小値
	//	短い増加部分列はそれより長い増加部分列の部分列なので，広義単調増加性がある．
	vector<T> dp(n + 1, numeric_limits<T>::max());
	dp[0] = numeric_limits<T>::lowest();

	// a[0..5) = [4, 2, 3, 3, 1] のときの遷移例
	//	dp_0[0..5] = [-INF, INF, INF, INF, INF, INF]
	//	dp_1[0..5] = [-INF,   4, INF, INF, INF, INF]
	//	dp_2[0..5] = [-INF,   2, INF, INF, INF, INF]
	//	dp_3[0..5] = [-INF,   2,   3, INF, INF, INF]
	//	dp_4[0..5] = [-INF,   2,   3,   3, INF, INF]
	//	dp_5[0..5] = [-INF,   1,   3,   3, INF, INF]

	rep(i, n) {
		// 右端が a[i] より大きい増加部分列の最小長さ j を得る．
		int j = ubpos(dp, a[i]);

		// 長さ j の増加部分列の右端に a[i] に置き換える．
		// これより短いものは右端を a[i] に置き換えても得しないので無視できる．
		// これより長いものはそもそも右端を a[i] に置き換えることができない．
		dp[j] = a[i];
	}

	// 右端の値が設定できている長さの最大値を求める．
	int res = 0;
	repir(j, n, 1) {
		if (dp[j] != numeric_limits<T>::max()) {
			res = j;
			break;
		}
	}

	return res;
}


//【最長増加部分列（復元）】O(n log n)
/*
* 数列 a[0..n) の（狭義）最長増加部分列の長さを返す．またその一例の添字列を lis に構成する．
*
*（セグメント木で高速化したインライン DP）
*/
pii op_lis(pii a, pii b) { return max(a, b); }
pii e_lis() { return { 0, -1 }; } // max の単位元が -INF でなく 0 であることに注意
template <class T>
int LIS_val_to_length(const vector<T>& a, vi* lis = nullptr) {
	// verify : https://judge.yosupo.jp/problem/longest_increasing_subsequence

	int n = sz(a);

	// a を座標圧縮した結果を b に格納する．
	vector<T> a_uniqed(a); uniq(a_uniqed); int m = sz(a_uniqed);
	vi b(n); rep(i, n) b[i] = lbpos(a_uniqed, a[i]);

	// dp_i[j] : b[0..i] で右端の値が j であるような最長増加部分列の長さとそのときの右端位置
	segtree<pii, op_lis, e_lis> dp(m);

	// prv[j] : 右端が b[i] の最長増加部分列について，右端の 1 つ前の要素の位置（DP 復元用）
	//（インライン DP を行うので，これを持たずに DP テーブルから復元しようとすると失敗する．）
	vi prv(n, -1);

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
		auto [len, pos] = dp.prod(0, j);
		len++;

		// j を右端とするより長いものが作れれば更新する．
		// dp[j] 以外は更新されることはないので，更新は O(log n) で終わる．
		// この性質が dp テーブルのインライン化と相性が良い．
		if (len > dp.get(j).first) {
			dp.set(j, { len, i });
			prv[i] = pos;
		}
	}

	// 右端の値を任意としたときの最長増加部分列の長さを得る．
	auto [len, pos] = dp.prod(0, m);

	// DP 復元を行う．
	if (lis != nullptr) {
		lis->clear();
		while (pos != -1) {
			lis->emplace_back(pos);
			pos = prv[pos];
		}
		reverse(all(*lis));
	}

	return len;
}


//【最長増加部分列（二次元）】O(n (log n)^2)
/*
* 数列 a[0..n), b[0..n) について，添字の増加列 t_0 < ... < t_(k-1) で，
*		∀i∈[0..k-1), a[t_i] < a[t_(i+1)] かつ b[t_i] < b[t_(i+1)]
* を満たすものの長さ k の最大値を返す．
*
* 利用：【狭義単調な点列】，【めぐる式二分探索】
*/
template <class T>
int LIS_2D(const vector<T>& a, const vector<T>& b) {
	// 参考 : https://topcoder-g-hatena-ne-jp.jag-icpc.org/skyaozora/20141216.html
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/1341

	int n = sz(a);

	// ps[j] : その要素を末尾にもつ長さ j の増加部分列が存在するような点（ps[0] は使わない）
	vector<Monotonous_points<T>> ps(1);

	rep(i, n) {
		// j : (a[i], b[i]) を末尾に持つ増加部分列の最大長
		function<bool(int)> okQ = [&](int j) { return ps[j].find_LL(a[i], b[i]); };
		int j = meguru_search(0, sz(ps), okQ) + 1;

		// 点 (a[i], b[i]) を挿入する．
		if (j >= sz(ps)) ps.push_back(Monotonous_points<T>(false));
		if (!ps[j].find_LL(a[i], b[i], false)) ps[j].insert(a[i], b[i]);

		dump(i); dump(ps);
	}

	return sz(ps) - 1;
}


//【最大スコア増加部分列】O(n log n)
/*
* 非負スコア c[0..n) が与えられた数列 a[0..n) の増加部分列の最大スコアを返す．
*
*（セグメント木で高速化したインライン DP）
*/
ll op_mis(ll a, ll b) { return max(a, b); }
ll e_mis() { return 0; }
template <class T>
ll maxscore_increasing_subsequence(const vector<T>& a, const vl& c) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_q

	int n = sz(a);

	// a を座標圧縮した結果を b に格納する．
	vector<T> a_uniqed(a); uniq(a_uniqed); int m = sz(a_uniqed);
	vi b(n); rep(i, n) b[i] = lbpos(a_uniqed, a[i]);

	// dp[j] : 今まで見てきた中での，右端の値が j であるような増加部分列の最大スコア
	segtree<ll, op_mis, e_mis> dp(m);

	// j = b[i] を順に見ていく
	rep(i, n) {
		int j = b[i];

		// j を右端にもてるのは，それまでの右端が j 未満のもののみ．
		// よってその中での増加部分列の最大スコアを求め，それに c[i] を加える．
		ll score = dp.prod(0, j) + c[i];
		// ll score = dp.prod(0, j + 1) + c[i]; // 広義単調増加の場合

		// j を右端とするよりスコアの大きいものが作れれば更新する．
		// dp[j] 以外は更新されることはないので，更新は O(log n) で終わる．
		// この性質が dp テーブルのインライン化と相性が良い．
		if (score > dp.get(j)) dp.set(j, score);
	}

	// 右端の値を任意としたときの増加部分列の最大スコアを得る．
	ll score = dp.prod(0, m);

	return score;
}


//【最長増加部分列（区分的）】O(m n log n)
/*
* 数列 a[0..n) の，m 箇所以下の違反を認めた（狭義）最長増加部分列の長さを返す．
*
*（セグ木上の二分探索で高速化したインライン DP）
*/
ll op_lpis(ll a, ll b) { return min(a, b); }
ll e_lpis() { return INFL; }
int piecewise_LIS(const vl& a, int m) {
	// verify : https://atcoder.jp/contests/dwacon2018-final-open/tasks/dwacon2018_final_b

	int n = sz(a);

	if (m >= n - 1) return n;

	// dp_i[k][j] : a[0..i) までで，ルール違反が k 回，長さが j での右端の値の最小値
	using SEG = segtree<ll, op_lpis, e_lpis>;
	vector<SEG> dp(m + 1, SEG(n + 1));
	repi(k, 0, m) dp[k].set(0, -INFL);

	// r[k] : dp_i[k] のまだ INFL である最小の添字
	vi r(m + 1, 1);

	rep(i, n) repir(k, m, 0) { // インライン DP なので k は降順にしないとまずい．
		// ルール違反する場合
		if (k < m) {
			ll val = dp[k + 1].get(r[k]);
			chmin(val, a[i]); // 場合分けを含むので，より小さいものを採用する．
			dp[k + 1].set(r[k], val);
			chmax(r[k + 1], r[k] + 1);
		}

		// ルール違反しない場合
		auto g = [&](ll x) { return x >= a[i]; };
		int j = dp[k].min_left(n, g); // dp は単調ではないので，累積 min をとって単調にする．

		ll val = dp[k].get(j);
		chmin(val, a[i]); // 場合分けを含むので，より小さいものを採用する．
		dp[k].set(j, val);
		chmax(r[k], j + 1);
	}

	return r[m] - 1;
}


//【最長増加部分列問題の双対】
/*
* 狭義[広義] 最長増加部分列の長さ = 広義[狭義] 減少部分列への分割の最小個数
*
* 証明：数列 a[0..n) に対し，DAG G = (V, E) を
*	V = [0..n)，
*	e = i→j ∈ E ⇔ a[i] ≧ a[j]
* として定める．このとき
*	狭義最長増加部分列の長さ = G の最大半鎖の大きさ
*	広義減少部分列への分割の最小個数 = G の最小パス被覆の大きさ
* となるので，ディルワースの定理よりこれらは等しい．
*
* verify : https://atcoder.jp/contests/abc134/tasks/abc134_e
*/


//【最長凸部分列（単調増加，両端含む）】O(n log n)
/*
* 与えられた広義単調増加列 a[0..n) に対し，a[0] と a[n-1] を含む
* 下に凸（階差が広義単調増加）な部分列で，その長さを最大にするものの添字列を返す．
*/
template <class T>
vi longest_convex_subsequence(const vector<T>& a) {
	// verify : https://leetcode.com/problems/find-maximum-non-decreasing-array-length/description/

	int n = sz(a);

	// dp[i] : a[i] を最後の要素とする凸部分列についての最後の階差の最小値
	vector<T> dp(n, (T)INFL);
	dp[0] = 0;

	rep(i, n - 1) {
		// 最後の要素を a[i] から a[i+1] に変更する場合，a[i] が階差に上乗せされる．
		// a は単調増加なので，凸性は自動的にみたされる．
		chmin(dp[i + 1], dp[i] + a[i + 1] - a[i]);

		// ni : 凸性をみたせる中で最も手前の位置
		// a は単調増加なので凸性をみたせるか否かには単調性がある．
		int ni = lbpos(a, a[i] + dp[i]);
		// int ni = ubpos(a, a[i] + dp[i]); // 狭義凸ならこっち

		if (ni < n) chmin(dp[ni], a[ni] - a[i]);
	}

	// DP 復元を行う．
	vi sel; int i = n - 1;
	while (true) {
		sel.push_back(i);

		bool end_flag = true;
		repir(ni, i - 1, 0) {
			if (a[i] - a[ni] == dp[i]) {
				i = ni;
				end_flag = false;
				break;
			}
		}
		if (end_flag) break;
	}
	reverse(all(sel));

	return sel;
}


//【最長共通部分列】O(|s| |t|)
/*
* 2 つの文字列 s, t の最長共通部分列の長さを返す．またその一例を lcs に格納する．
*
*（二次元 DP）
*/
template <class T>
int longest_common_subsequence(const vector<T>& s, const vector<T>& t, vector<T>* lcs = nullptr) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_f

	// 文字列の長さ
	int n = sz(s), m = sz(t);

	// dp[i][j] : s[0..i) と t[0..j) の最長共通部分列の長さ
	vvi dp(n + 1, vi(m + 1));
	rep(i, n) {
		rep(j, m) {
			// s の i 文字目と t の j 文字目が等しい場合
			if (s[i] == t[j]) {
				// その文字は採用し，1 つ短い文字列に帰着する．
				dp[i + 1][j + 1] = dp[i][j] + 1;
			}
			// s の i 文字目と t の j 文字目が異なる場合
			else {
				// どちらかを 1 文字削った文字列に帰着する．
				dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
			}
		}
	}

	// DP 復元を行い最長共通部分列 lcs を求める．
	if (lcs != nullptr) {
		*lcs = vector<T>(dp[n][m]);
		int i = n - 1, j = m - 1, pt = dp[n][m] - 1;
		while (i >= 0 && j >= 0) {
			// s の i 文字目と t の j 文字目が等しい場合
			if (s[i] == t[j]) {
				// その文字は採用し，1 つ短い文字列に帰着する．
				(*lcs)[pt--] = s[i];
				i--;
				j--;
			}
			// s の i 文字目と t の j 文字目が異なる場合
			else {
				// どちらを 1 文字削る方が長い文字列が得られるかを調べて短い文字列に帰着する．
				if (dp[i][j + 1] > dp[i + 1][j]) {
					i--;
				}
				else {
					j--;
				}
			}
		}
	}

	return dp[n][m];
}


//【レーベンシュタイン距離】O(|s| |t|)
/*
* 文字列 s から文字列 t へのレーベンシュタイン距離を返す．
* また必要なら削除や挿入の位置を '-' で表した文字列を s2, t2 に格納する．
* dist = {fit, ins, del, sub} : 順に，一致，挿入，削除，置換 1 回あたりの距離
*
*（二次元 DP）
*/
template <class T>
ll levenshtein_distance(const vector<T>& s, const vector<T>& t,
	const tuple<ll, ll, ll, ll>& dist = { 0, 1, 1, 1 },
	vector<T>* s2 = nullptr, vector<T>* t2 = nullptr)
{
	// verify : https://atcoder.jp/contests/genocon2021/tasks/genocon2021_b

	int n = sz(s), m = sz(t);

	auto [fit, ins, del, sub] = dist;

	// dp[i][j] : s[0..i) から t[0..j) への距離
	vvl dp(n + 1, vl(m + 1, INFL));
	dp[0][0] = 0;

	// 貰う DP
	repi(i, 0, n) repi(j, 0, m) {
		// 以下の 3 通りのうちの最大のものを選ぶ：
		//   s[i - 1] と t[j - 1] を対にする．
		//   s[i - 1] を削除する．
		//   t[j - 1] を挿入する．
		if (i > 0 && j > 0) {
			chmin(dp[i][j], dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? fit : sub));
		}
		if (i > 0) {
			chmin(dp[i][j], dp[i - 1][j] + del);
		}
		if (j > 0) {
			chmin(dp[i][j], dp[i][j - 1] + ins);
		}
	}

	// DP 復元を行う
	if (s2 != nullptr) {
		s2->clear();
		t2->clear();

		int i = n, j = m;
		while (i > 0 || j > 0) {
			if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? fit : sub)) {
				s2->push_back(s[--i]);
				t2->push_back(t[--j]);
			}
			else if (i > 0 && dp[i][j] == dp[i - 1][j] + del) {
				s2->push_back(s[--i]);
				t2->push_back('-');
			}
			else {
				s2->push_back('-');
				t2->push_back(t[--j]);
			}
		}
		reverse(all(*s2));
		reverse(all(*t2));
	}

	return dp[n][m];
}


//【最長回文部分列】O(n^2)
/*
* s[0..n) の回文である部分列のうち最長のものの長さを返す．
*
*（区間 DP）
*/
int longest_palindrome_subsequence(const string& s) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_ct

	int n = sz(s);

	// dp[l][r] : s[l..r) の最長回文部分列長
	vvi dp(n + 1, vi(n + 1));

	// 1 文字の場合
	rep(i, n) dp[i][i + 1] = 1;

	// 貰う区間 DP
	repir(l, n - 1, 0) repi(r, l + 2, n) {
		// s[l] を使わない場合
		chmax(dp[l][r], dp[l + 1][r]);

		// s[r-1] を使わない場合
		chmax(dp[l][r], dp[l][r - 1]);

		// s[l] と s[r-1] を対応させる場合
		if (s[l] == s[r - 1]) chmax(dp[l][r], dp[l + 1][r - 1] + 2);
	}

	return dp[0][n];
}


