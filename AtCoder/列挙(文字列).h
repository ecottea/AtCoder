#pragma once
#include "header.h"
#include "辞書順.h"
// ■■■■■ 文字列の列挙 ■■■■■


//【任意文字列の列挙】O(n |cs|^n)
/*
* 文字集合 cs の要素からなる長さ n の文字列全てを格納したリストを返す．
*/
vector<string> enumerate_all_strings(int n, const string& cs) {
	// verify : https://yukicoder.me/problems/no/3015

	vector<string> strs;
	string s;

	// l : 長さ
	function<void(int)> rf = [&](int l) {
		// 長さが n の場合は記録
		if (l == n) {
			strs.push_back(s);
			return;
		}

		// c : s[l]
		repe(c, cs) {
			s.push_back(c);
			rf(l + 1);
			s.pop_back();
		}
	};
	rf(0);

	return strs;
}


//【任意文字列の列挙（置換）】O(n |cs|^n)
/*
* s[0..n) に含まれる '?' それぞれを cs の要素のいずれかに置き換えて
* 得られる文字列全てを格納したリストを返す．
*/
vector<string> enumerate_all_strings(string s, const string& cs) {
	int n = sz(s);
	vector<string> strs;

	function<void(int)> rf = [&](int i) {
		if (i == n) {
			strs.push_back(s);
			return;
		}

		if (s[i] == '?') {
			char c0 = s[i];
			repe(c, cs) {
				s[i] = c;
				rf(i + 1);
			}
			s[i] = c0;
		}
		else {
			rf(i + 1);
		}
	};
	rf(0);

	return strs;
}


//【de Bruijn 列】O(K^L)
/*
* [0..K) からなる周期境界列 a[0..K^L) で，各 i∈[0..K^L) について a[i..i+L) が全て互いに異なるものを返す．
*/
vi de_bruijn(int K, int L) {
	// 参考 : https://en.wikipedia.org/wiki/De_Bruijn_sequence
	// verify : https://yukicoder.me/problems/no/2893

	vi res;
	res.reserve(powi(K, L));

	vi seq(K * L);

	function<void(int, int)> rf = [&](int t, int p) {
		if (t > L) {
			if (L % p == 0) {
				copy(seq.begin() + 1, seq.begin() + p + 1, back_inserter(res));
			}
			return;
		}

		seq[t] = seq[t - p];
		rf(t + 1, p);
		repi(j, seq[t - p] + 1, K - 1) {
			seq[t] = j;
			rf(t + 1, t);
		}
	};
	rf(1, 1);

	return res;
}


//【周期的連の列挙】O(n log n)
/*
* s[0..n) の周期 t をもつ極大な周期的連 s[l..r) を 3 つ組 {l, r, t} で表しそのリストを返す．
* s[l..r) が s の周期 t の極大な周期的連であるとは，以下を満たすことをいう：
*	s[l..r) の最小周期は t（余りも許す）で，r-l ≧ 2t
*	s[l-1..r), s[l..r+1) の最小周期は t より大きい
*
* 利用：【部分文字列の辞書順比較】
*
*（分割統治法）
*/
template <class STR>
vector<tuple<int, int, int>> enumerate_cyclic_run(const STR& s) {
	// 参考 : https://pazzle1230.hatenablog.com/entry/2019/11/27/234632
	// verify : https://judge.yosupo.jp/problem/runenumerate

	//【方法】
	// ある地点 m を跨ぐ極大連 s[l..r) を O(|s|) で列挙できれば分割統治法が使える．
	// r-l ≧ 2t の条件より，s[l..m), s[m..r) の一方は 1 周期を丸ごと含む．
	// 一般性を失わず 1 周期が m を左右いずれかの境界に持つとしてよいので，周期の候補を O(|s|) 個に減らせた．
	// 連をどこまで伸ばせるかについては LCP を見れば分かる．

	//【備考】
	// 全ての周期的連をわたる 1			の和は n 以下
	// 全ての周期的連をわたる (r-l)/t		の和は 3n 以下
	// 全ての周期的連をわたる (r-l+1-2t)	の和は O(n log n)

	int n = sz(s);

	STR sR(s);
	reverse(all(sR));

	Substring_compare S(s), SR(sR);

	// lr_to_t[l*(n+1)+r] : s[l..r) の最小周期
	unordered_map<ll, int> lr_to_t;

	function<void(int, int)> rf = [&](int L, int R) {
		if (R - L <= 1) return;

		int M = (L + R) / 2;

		// [l..M) を周期にもつ極大連を探す．
		repi(l, L, M - 1) {
			int t = M - l;

			int r2 = M + S.lcp(l, n, M, n);
			int l2 = l - SR.lcp(n - M, n, n - l, n);
			if (r2 - l2 < 2 * t) continue;

			ll h = l2 * (n + 1LL) + r2;
			auto it = lr_to_t.find(h);
			if (it != lr_to_t.end()) chmin(it->second, t);
			else lr_to_t[h] = t;
		}

		// [M..r) を周期にもつ極大連を探す．
		repi(r, M + 1, R) {
			int t = r - M;

			int r2 = r + S.lcp(M, n, r, n);
			int l2 = M - SR.lcp(n - r, n, n - M, n);
			if (r2 - l2 < 2 * t) continue;

			ll h = l2 * (n + 1LL) + r2;
			auto it = lr_to_t.find(h);
			if (it != lr_to_t.end()) chmin(it->second, t);
			else lr_to_t[h] = t;
		}

		rf(L, M);
		rf(M, R);
	};
	rf(0, n);

	vector<tuple<int, int, int>> res;

	for (auto [lr, t] : lr_to_t) {
		int l = (int)(lr / (n + 1));
		int r = (int)(lr % (n + 1));
		res.emplace_back(l, r, t);
	}

	return res;
}


