#pragma once
#include "header.h"
// ■■■■■ 列 ■■■■■


//【隣接要素への加算に対する不変量】
/*
* 列 a に対して a[i..i+1] += x という操作だけが許される場合，
* a の交代和 Σi (-1)^i a[i] は不変量となる．
* 
* verify : https://atcoder.jp/contests/arc135/tasks/arc135_d
*/


//【下に凸判定（広義）】O(n)
/*
* a[0..n) が広義に下に凸（階差数列が広義単調増加）かを返す．
*/
template <class T>
bool convexQ(const vector<T>& a) {
	int n = sz(a);
	T diff = numeric_limits<T>::lowest();

	rep(i, n - 1) {
		T d = a[i + 1] - a[i];
		if (d < diff) return false;
		diff = d;
	}
	return true;
}


//【上に凸判定（広義）】O(n)
/*
* a[0..n) が広義に上に凸（階差数列が広義単調減少）かを返す．
*/
template <class T>
bool concaveQ(const vector<T>& a) {
	int n = sz(a);
	T diff = numeric_limits<T>::max();

	rep(i, n - 1) {
		T d = a[i + 1] - a[i];
		if (d > diff) return false;
		diff = d;
	}
	return true;
}


//【ヒストグラムの横切り】
/*
* ヒストグラム hist を横切りしたときの (高さ, 長さ) の組を hls に格納し hls を返す．
* ただし高さが狭義単調増加かつ長さ狭義単調減少となる部分だけを格納する．
*
* hls[i] = {h, l} : 低い方から i 番目の高さが h，長さが l であることを表す．
*/
vector<pli> cut_histogram_horizontal(vl hist) {
	int n = sz(hist);
	vector<pli> hls;

	// ヒストグラムをソートしておく．
	sort(all(hist));

	// 直前に見ていた高さ
	ll bh = -1;

	rep(i, n) {
		// 直前に見ていた高さより高い場合
		if (bh < hist[i]) {
			// 高さと長さの組を格納する．
			hls.push_back({ hist[i], n - i });

			bh = hist[i];
		}
	}

	return hls;
}


//【非等差数列】
/*
* どの p（素数）要素も等差数列をなさないような集合 a[0..n) は以下の方法で構築できる：
*	a[i] = (i の p-1 進表示を p 進表示だと解釈しなおしたもの)
* 
* これは 0 から順に条件を満たす限り昇順に数を追加していくという貪欲な構成でも得られる．
* 
* verify : https://atcoder.jp/contests/monamieHB2021/tasks/monamieHB2021_b
*/


//【辞書順最小部分列（長さごと）】
/*
* 辞書順.h へ
*/


