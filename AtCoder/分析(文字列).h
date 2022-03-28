#pragma once
#include "header.h"
// ■■■■■ 文字列の各種性質の分析 ■■■■■


//【部分文字列の数え上げ】O(n)
/*
* 文字列 s[0..n) の部分文字列の個数を返す（空文字列も s の部分文字列とみなす）
*/
ll count_substring(const string& s) {
	int n = sz(s);

	auto sa = suffix_array(s);
	auto la = lcp_array(s, sa);

	ll res = n - sa[0];
	repi(i, 1, n - 1) res += n - sa[i] - la[i - 1];

	return res;
}


//【最長共通接頭尾辞】O(n)
/*
* 文字列 s[0..n) について，s[0..i) の接頭辞と接尾辞が
* 最大何文字一致しているか（i 文字未満）を len[i] に格納する．
*/
template <class STR> void morris_pratt(const STR& s, vi& len) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/01/235807

	//【方法】
	// j = len[i] まで分かっているときに len[i+1] を求めることを考える．
	// len[i] = j なので，
	//		s[0..j) = s[i-j..i)
	// であり，これが最長一致である．
	// 
	// case1: s[j] = s[i] のときは，
	//		s[0..j+1) = s[(i+1)-(j+1)..i+1)
	// となり len[i+1] = j+1 と求まる．
	// 
	// case2: s[j] != s[i] のときは，
	//		s[0..k) = s[i-k..i) かつ s[k] = s[i]
	// なる最大の k < j を見つけることができれば len[i] = k+1 と求まる．
	// 
	// s[0..k) は s[0..j) の接頭辞であり，
	// s[i-k..i) は s[i-j..i) = s[0..j) の接尾辞である．
	// よって len[j] の定め方より k <= len[j] が必要である．
	// 
	// もし s[len[j]] = s[i] なら k = len[j] と選べばよく，そうでなければ
	// j ← len[j] として同じ操作を繰り返せば良い．

	int n = sz(s);
	len.resize(n + 1);
	len[0] = -1;

	int j = -1;
	rep(i, n) {
		while (j >= 0 && s[i] != s[j]) j = len[j];
		len[i + 1] = ++j;
	}
}


//【最長回文長（文字中心）】O(n)
/*
* s[0..n) の s[i] を中心とする最長回文の半径（(文字数 + 1) / 2）を r[i] に格納する．
* ここで回文の半径とは，(文字数 + 1) / 2 を意味する．
*/
template <class STR> void manacher(const STR& s, vi& r) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/02/235837

	//【方法】
	// s[i] を中心とする最長回文の半径 j = r[i] が愚直に求まったとする．
	// すなわち s(i-j..i+j) が s[i] を中心とする最長回文である．
	// 
	// 各 k = [1..j) について，s[i-k] を中心とする最長回文 s(i-k-r[i-k]..i-k+r[i-k]) が
	// s(i-j+1..i+j-1) の部分文字列であれば，s[i±j] の影響を受けず
	// s[i] についての左右対称性より r[i+k] = r[i-k] と定まる．
	// その条件は，左端を比較して
	//		i - k - r[i-k] >= i - j + 1
	//		⇔ k + r[i-k] < j
	// である．このような結果の使い回しができる限り k を進め，次の i を i + k にする．
	// 
	// 使い回しができなくなったということは，s[i+k] を中心とする最長回文
	// s(i+k-r[i+k]..i+k+r[i+k]) が s(i-j+1..i+j-1) の部分文字列でないので，
	// 右端を比較することで
	//		i + k + r[i+k] > i + j - 1
	//		⇔ r[i+k] >= j - k
	// である．よって次の j は j - k にすればよい．

	int n = sz(s);
	r.resize(n);

	int i = 0, j = 0;
	while (i < n) {
		while (i - j >= 0 && i + j < n && s[i - j] == s[i + j]) j++;
		r[i] = j;

		int k = 1;
		while (i - k >= 0 && k + r[i - k] < j) {
			r[i + k] = r[i - k];
			k++;
		}
		i += k;
		j -= k;
	}
}


//【最長回文長】O(n)
/*
* s[0..n) の s[i] を中心とする最長回文の長さを lo[i] に格納し，
* s[i..i+1] を中心とする最長回文の長さを le[i] に格納する．
*
* 利用：【最長回文長（文字中心）】
*/
template <class STR> void manacher(const STR& s, vi& lo, vi& le) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/02/235837
	// verify : https://judge.yosupo.jp/problem/enumerate_palindromes

	int n = sz(s);
	lo.resize(n);
	le.resize(n - 1);

	STR s_riffled;
	s_riffled.resize(2 * n + 1);
	rep(i, n) s_riffled[2 * i + 1] = s[i];
	rep(i, n + 1) s_riffled[2 * i] = '_'; // '_' は s に含まれない文字

	vi r;
	manacher(s_riffled, r);

	rep(i, n) lo[i] = r[2 * i + 1] - 1;
	rep(i, n - 1) le[i] = r[2 * (i + 1)] - 1;
}


//【Z アルゴリズム】O(n)
/*
* 文字列 s[0..n) について，s[i..n) と s の最長共通接頭辞の長さを z[i] に格納する．
*/
template <class STR> void z_algorithm(const STR& s, vi& z) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/03/214243
	// verify : https://judge.yosupo.jp/problem/zalgorithm

	//【方法】
	// j = z[i] > 0 が愚直に求まったとする．
	// すなわち s[0..j) = s[i..i+j) かつ s[j] != s[i+j] である．
	// 
	// 各 k = [1..j) について，s と s[k..n) の最長共通接頭辞 s[k..k+z[k]) が
	// s[0..j-1) の部分文字列であれば，s[j] と s[i+j] の影響を受けず
	// s[0..j) = s[i..i+j) より z[i+k] = z[k] と定まる．
	// その条件は，これらの右端を比較して
	//		k + z[k] <= j - 1
	//		⇔ k + z[k] < j
	// である．このような結果の使い回しができる限り k を進め，次の i を i + k にする．
	//  
	// 使い回しができなくなったということは，s[k..k+z[k]) が s[0..j-1) の
	// 部分文字列でないので，s[i+k..i+k+z[i+k]) も s[i..i+j-1) の
	// 部分文字列ではなく，これらの右端を比較することで
	//		i + k + z[i + k] > i + j - 1
	//		⇔ z[i + k] >= j - k
	// である．よって次の j は j - k にすればよい．

	int n = sz(s);
	z.resize(n);
	z[0] = n;

	int i = 1, j = 0;
	while (i < n) {
		while (i + j < n && s[j] == s[i + j]) j++;
		z[i] = j;

		if (j == 0) {
			i++;
			continue;
		}

		int k = 1;
		while (i + k < n && k + z[k] < j) {
			z[i + k] = z[k];
			k++;
		}
		i += k;
		j -= k;
	}
}


//【部分文字列判定／クヌース－モリス－プラット法】O(n + m)
/*
* s[0..n) の部分文字列として w[0..m) が含まれているかどうか調べ，
* 見つかった場所の先頭位置を昇順に pos に格納する．
*/
template <class STR> void knuth_morris_pratt(const STR& s, const STR& w, vi& pos) {
	//  参考 : https://ja.wikipedia.org/wiki/%E3%82%AF%E3%83%8C%E3%83%BC%E3%82%B9%E2%80%93%E3%83%A2%E3%83%AA%E3%82%B9%E2%80%93%E3%83%97%E3%83%A9%E3%83%83%E3%83%88%E6%B3%95
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_B

	int n = sz(s), m = sz(w);
	pos.clear();

	// 部分マッチテーブル tbl の作成
	// s[i] から照合を開始して s[i + j] != w[j] となった場合，
	// 次に照合を開始すべき位置が s[i + j - tbl[j]] であるように構築する．
	vi tbl(m + 1);
	tbl[0] = -1; tbl[1] = 0;
	int i = 2; // いま tbl[i] を計算中であることを表す．
	int j = 0; // いま w[j] まで見ていることを表す．
	while (i <= m) {
		// サブ文字列が w の先頭と一致し続けている場合
		if (w[i - 1] == w[j]) {
			// 一致した長さの分だけバックトラッキングしなければならない．
			tbl[i] = j + 1;

			// それぞれ 1 文字先を見に行く．
			i++; j++;
		}
		// サブ文字列と w の先頭との一致が終わった場合？
		else if (j > 0) {
			// 次のサブ文字列を走査するため j を戻す．？
			j = tbl[j];
		}
		// ？
		else {
			tbl[i] = 0;
			i++;
		}
	}

	// 連続部分列を探す．
	i = 0; // いま s[i] から始まる連続部分列を見ていることを表す．
	j = 0; // いま w[j] まで見ていることを表す．
	while (i + j < n) {
		// s で見ている文字 s[i + j] が w で見ている文字 w[j] に一致した場合
		if (w[j] == s[i + j]) {
			// さらに 1 文字先を見に行く．
			j++;

			// もし w を走査し終えたなら連続部分列として w を発見．
			if (j == m) {
				pos.push_back(i);

				// 部分マッチテーブルに従い i, j を再設定する．
				// i = i + j としたいが tbl[j] だけのバックトラッキングが入る．
				i = i + j - tbl[j];

				// その代わり w との照合を tbl[j] だけ進んだところから始められる．
				if (j > 0) {
					j = tbl[j];
				}
			}
		}
		// s で見ている文字 s[i + j] が w で見ている文字 w[j] に一致しなかった場合
		else {
			// 部分マッチテーブルに従い i, j を再設定する．
			// i = i + j としたいが tbl[j] だけのバックトラッキングが入る．
			i = i + j - tbl[j];

			// その代わり w との照合を tbl[j] だけ進んだところから始められる．
			if (j > 0) {
				j = tbl[j];
			}
		}
	}
}


