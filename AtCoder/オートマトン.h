#pragma once
#include "header.h"
// ■■■■■ オートマトン ■■■■■


//【非決定性有限オートマトン → 決定性有限オートマトン】
/*
* 非決定性有限オートマトン NFA の状態が n 個であるとき，NFA の状態の集合を新たに状態とすることで
* 高々 2^n 個の状態をもつ決定性有限オートマトン DFA が得られる．
* 
* verify : https://atcoder.jp/contests/abc228/tasks/abc228_g
*/


//【決定性有限オートマトンの受理文字列の個数と経路数】
/*
* 決定性有限オートマトン DFA において，開始状態からいずれかの受理状態までを結ぶパスの本数と，
* DFA が受理する文字列の総数は等しい．
* 
* verify : https://atcoder.jp/contests/abc228/tasks/abc228_g
*/


//【文字列オートマトン】O(n K)
/*
* Σ = {A, ..., A+K-1} 上の文字列 s[0..n) を受理するオートマトンを構築する．
* 状態 i を s[0..i) と一致している状態とし，各 i∈[0..n), k∈[0..K) について，
* 状態 i で k 番目の文字を読んだときの遷移先を格納した二次元リストを返す．
*/
vvi string_automaton(const string& s, int K = 26, char A = 'a') {
	// verify : https://yukicoder.me/problems/no/2905

	int n = sz(s);

	// mp[i] : s[0..i) の接頭辞と接尾辞が最大何文字一致しているか（i 文字未満）
	vi mp(n + 1);
	mp[0] = -1;

	// モーリスプラット
	int j = -1;
	rep(i, n) {
		while (j >= 0 && s[i] != s[j]) j = mp[j];
		mp[i + 1] = ++j;
	}

	mp[0] = 0;

	// nxt[i][k] : 状態 i で k 番目の文字を読んだときの遷移先
	vvi nxt(n, vi(K));

	rep(i, n) {
		// len : s[0..len) = s[i-len..i) となる最大の len（ただし i > 0 なら len < i）
		int len = mp[i];

		rep(k, K) {
			// 次の文字と一致する文字を読んだ場合（成功）
			if (k == s[i] - A) {
				// 次の状態へと遷移する．
				nxt[i][k] = i + 1;
			}
			// 次の文字と一致しない文字を読んだ場合（失敗）
			else {
				// s[0..i) までは一致していたので，直前は s[i-len..i) と一致していた．
				// len の定め方より s[0..len) とも一致しているので，状態 len で k を読んだ場合に合流する．
				nxt[i][k] = nxt[len][k];
			}
		}
	}

	return nxt;
}


