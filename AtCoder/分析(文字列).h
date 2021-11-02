#pragma once
#include "header.h"
// ■■■■■ 文字列の各種性質の分析 ■■■■■



//【ランレングス符号】O(|s|)
/*
* s をランレングス符号化し，結果を rlc に格納する．
* rlc[i] = {c, l} は前から i 番目の列が l 個の文字 c からなることを表す．
*/
template <class T>
void run_length_encodeing(vector<T>& a, vector<pair<T, int>>& rlc) {
	int n = sz(a);
	if (n == 0) {
		rlc = {};
		return;
	}

	rlc = { {a[0], 1} };

	// 今読んでいる文字の種類を記憶する．
	T c = a[0];

	repi(i, 1, n - 1) {
		// 記憶している文字と同じ文字の場合
		if (c == a[i]) {
			// 列の長さを増やす．
			rlc.rbegin()->second++;
		}
		// 記憶している文字と異なる文字の場合
		else {
			// 新しい文字を記憶しておく．
			c = a[i];

			// 新たな列を追加する．
			rlc.push_back({ c, 1 });
		}
	}
}


//【部分文字列の数え上げ】O(|s|)
/*
* 文字列 s の部分文字列の個数を返す（空文字列も s の部分文字列とみなす）
*/
ll count_substring(const string& s) {
	int n = sz(s);

	auto sa = suffix_array(s);
	auto la = lcp_array(s, sa);

	ll res = (ll)n - sa[0];
	repi(i, 1, n - 1) res += (ll)n - sa[i] - la[i - 1LL];

	return res;
}


//【最長共通接頭尾辞】O(|s|)
/*
* s[0..i) の接頭辞と接尾辞が最大何文字一致しているか（i 文字未満）を len[i] に格納する．
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
	len.resize(n + 1LL);
	len[0] = -1;

	int j = -1;
	rep(i, n) {
		while (j >= 0 && s[i] != s[j]) j = len[j];
		len[i + 1LL] = ++j;
	}
}


//【最長回文長（文字中心）】O(|s|)
/*
* s[i] を中心とする最長回文の半径（(文字数 + 1) / 2）を r[i] に格納する．
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
		while (i - j >= 0 && i + j < n && s[(ll)i - j] == s[(ll)i + j]) j++;
		r[i] = j;

		int k = 1;
		while (i - k >= 0 && k + r[(ll)i - k] < j) {
			r[(ll)i + k] = r[(ll)i - k];
			k++;
		}
		i += k;
		j -= k;
	}
}


//【最長回文長】O(|s|)
/*
* s[i] を中心とする最長回文の長さを lo[i] に格納し，
* s[i..i+1] を中心とする最長回文の長さを le[i] に格納する．
* 使用時には s に表れない文字を dmy に渡すこと．
*
* 利用：【最長回文長（文字中心）】
*/
template <class T> void manacher(const vector<T>& s, T dmy, vi& lo, vi& le) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/02/235837

	int n = sz(s);
	lo.resize(n);
	le.resize(n - 1LL);

	vector<T> s_riffled(2LL * n + 1);
	rep(i, n) s_riffled[2LL * i + 1] = s[i];
	rep(i, n + 1) s_riffled[2LL * i] = dmy;

	vi r;
	manacher(s_riffled, r);

	rep(i, n) lo[i] = r[2LL * i + 1] - 1;
	rep(i, n - 1) le[i] = r[2LL * (i + 1LL)] - 1;
}


//【Z アルゴリズム】O(|s|)
/*
* s = s[0..n) について，s[i..n) と s の最長共通接頭辞の長さを z[i] に格納する．
*/
template <class STR> void z_algorithm(const STR& s, vi& z) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/03/214243

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
		while (i + j < n && s[j] == s[(ll)i + j]) j++;
		z[i] = j;

		if (j == 0) {
			i++;
			continue;
		}

		int k = 1;
		while (i + k < n && k + z[k] < j) {
			z[(ll)i + k] = z[k];
			k++;
		}
		i += k;
		j -= k;
	}
}


