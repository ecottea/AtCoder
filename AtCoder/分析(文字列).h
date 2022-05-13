#pragma once
#include "header.h"
// ■■■■■ 文字列の各種性質の分析 ■■■■■


//【最長共通接頭尾辞】O(n)
/*
* 文字列 s[0..n) について，s[0..i) の接頭辞と接尾辞が
* 最大何文字一致しているか（i 文字未満）を len[i] に格納する．
*/
template <class STR> void morris_pratt(const STR& s, vi& len) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/01/235807
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_B

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


//【部分文字列判定】O(n + m)
/*
* s[0..n) の部分文字列として w[0..m) が含まれているかどうか調べ，
* 見つかった場所の先頭位置を昇順に pos に格納する．
*
* 利用：【最長共通接頭尾辞】
*/
template <class STR> void knuth_morris_pratt(const STR& s, const STR& w, vi& pos) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_B

	int n = sz(s), m = sz(w);
	pos.clear();

	// l[j] : w[0..j) の接頭辞と接尾辞が最大何文字一致しているか（j 文字未満）
	vi l;
	morris_pratt(w, l);

	int i = 0; // s[i..i+m) を走査中
	int j = 0; // s[i..i+j) = w[0..j) まで確定
	while (i + j < n) {
		// s[i+j] = w[j] の場合
		if (w[j] == s[i + j]) {
			// さらに 1 文字先を見に行く．
			j++;

			// もし w を走査し終えたなら連続部分列として w を発見できたので記録する．
			if (j == m) pos.push_back(i);
			else continue;
		}

		// s[i..i+j) までは走査したので，次は s[i+j..i+j+m) の走査といきたい.
		// しかし，w[0..j) の最大長 l[j] の接尾辞は w[0..j) の接頭辞に一致していた．
		// s[i..i+j) = w[0..j) だから，これは s[i..i+j) の接尾辞とも一致している．
		// よって s[i+j-l[j]..i+j-l[j]+m) = w[0..m) となる可能性は残っている．
		i = i + j - l[j];

		// 一方で，これは s[i+j-l[j]..i+j-l[j]+m) の長さ l[j] の接頭辞が
		// w[0..j) の接頭辞に一致していることを意味するので，
		// s[i+j-l[j]..i+j) = w[0..l[j]) までは確定していることになる． 
		if (j > 0) j = l[j];
	}
}


//【最短周期長】
/*
* s[0..n) の最長共通接頭尾辞長 len[0..n) が【最長共通接頭尾辞】で得られていれば，
* s[0..i) の最短周期長は i-len[i] で得られる．
* 
* 参考 : https://snuke.hatenablog.com/entry/2015/04/05/184819
*/


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
	rep(i, n + 1) s_riffled[2 * i] = '$'; // '$' は s に含まれない文字

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


