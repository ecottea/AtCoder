#pragma once
#include "header.h"
// ■■■■■ 列 ■■■■■


//【判定問題 → 数え上げ問題】
/*
*	(判定問題) 列 seq[0..n) が条件 P を満たすか判定せよ
* が，O(1) 個のパラメータ（変域 O(m)）を保持しながらの線形走査で解けるならば，
* そのパラメータの値を状態にもつ DP を行うことで，
*	(数え上げ問題) 条件 P を満たす列の総数を求めよ
* を O(n m) で解くことができる．
* 
* verify : https://atcoder.jp/contests/abc237/tasks/abc237_f
*/


//【隣接要素への加算に対する不変量】
/*
* 列 a に対して a[i..i+1] += x という操作だけが許される場合，
* a の交代和 Σi (-1)^i a[i] は不変量となる．
* 
* verify : https://atcoder.jp/contests/arc135/tasks/arc135_d
*/


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
*	a[i] = (i の p-1 進表示を p 進法表示だと解釈しなおしたもの)
* 
* これは 0 から順に条件を満たす限り昇順に数を追加していくという構成でも得られる．
* 
* verify : https://atcoder.jp/contests/monamieHB2021/tasks/monamieHB2021_b
*/


//【辞書順最小部分列（長さごと）】
/*
* 辞書順.h へ
*/


