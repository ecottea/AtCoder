#pragma once
#include "header.h"
// ■■■■■ 括弧列 ■■■■■


//【括弧列の数え上げ】
/*
* 長さ 2n の括弧列の総数はカタラン数 1/(n+1) bin(2n, n) である．
* 
* 証明：括弧列を通行禁止線をもった格子路に対応させ，反射原理を用いれば良い．
*/


//【括弧列の対応と偶奇】
/*
* 括弧列 s において，s[i]='(' と s[j]=')' が対応する括弧であるとき i と j の偶奇は異なる．
*
* 証明：s(i..j) もまた括弧列であり，これの長さが偶数であることから直ちに従う．
*
* verify : https://atcoder.jp/contests/agc048/tasks/agc048_b
*/


//【括弧列の重なり】
/*
* 括弧文字列 s+t+u について，
*	s+t と t+u が共に括弧列 ⇒ s, t, u は全て括弧列
*	s+t+u と t が共に括弧列 ⇒ s+u, t は共に括弧列
* 
* verify : https://yukicoder.me/problems/no/2133
*/


//【括弧列の正しさ判定】O(n)
/*
* 文字列 s[0..n) が正しい括弧列かを返す．
*/
bool parenthesis_sequenceQ(const string& s) {
	// verify : https://atcoder.jp/contests/arc141/tasks/arc141_c

	//【方法】
	// 括弧文字列 s[0..n) に対して，'(' を +1, ')' を -1 に置き換える操作を行い，
	// さらに左から累積和をとったものを acc[0..n] とする．このとき，
	//		s が正しい括弧列 ⇔ min(acc) = acc[n] = 0

	int n = sz(s);

	vi acc(n + 1);
	rep(i, n) {
		int val = 0;
		if (s[i] == '(') val = 1;
		if (s[i] == ')') val = -1;
		if (val == 0) return false;

		acc[i + 1] = acc[i] + val;
	}

	return *min_element(all(acc)) == 0 && acc[n] == 0;
}


//【括弧列との内積の最大化】O(n log n)
/*
* 与えられた数列 a[0..2n) について，括弧列 s[0..2n) の '(' を +1, ')' を -1 に
* 置き換えた列と a[0..2n) との内積の最大値を返す．
*/
ll maximize_parenthesis_inner_product(const vl& a) {
	// verify : https://atcoder.jp/contests/abc250/tasks/abc250_g

	//【方法】
	// 括弧列に課された条件より，
	//		s[0..0] には +1 が 1 個以上
	//		s[0..2] には +1 が 2 個以上
	//		s[0..4] には +1 が 3 個以上 ...
	// 存在する必要があり，逆にこれらを満たす括弧列は全て構成することができる．
	// 
	// よって先頭から順に優先度付きキューに a の要素を入れていき，
	// a[2i] まで読む度に，その時点での最大の要素を取り出し，
	// これを +1 と積をとる要素として採用すれば良い．
	//
	// 最終的にキューに残った要素は -1 と積をとることになる．

	int n2 = sz(a);

	priority_queue<ll> q;
	ll res = 0;

	rep(i, n2) {
		q.push(a[i]);

		if (i % 2 == 0) {
			res += q.top();	q.pop();
		}
	}
	while (!q.empty()) {
		res -= q.top(); q.pop();
	}

	return res;
}


//【括弧列 → 木】O(n)
/*
* 括弧列 s[0..2n) について，ネスト関係を表した 0 を根とする有向根付き木 g[0..n] を返す．
* i 番目の頂点は対応する括弧の組 s[ls[i]] = '(', s[rs[i]] = ')' に対応し，子ほどネストが深いものとする．
* ただし ls[0] = -1, rs[0] = 2n とする．
*/
Graph parenthesis_tree(const string& s, vi* ls = nullptr, vi* rs = nullptr) {
	// verify : https://atcoder.jp/contests/discovery2016-final/tasks/discovery_2016_final_c

	int n = sz(s) / 2;
	Graph g(n + 1);
	if (ls) ls->resize(n + 1);
	if (rs) rs->resize(n + 1);

	int id = 1;
	stack<pii> stk; // ('(' の位置, 木の頂点番号)
	stk.push({ -1, 0 });
	if (ls) (*ls)[0] = -1;
	if (rs) (*rs)[0] = 2 * n;

	rep(i, 2 * n) {
		if (s[i] == '(') {
			stk.push({ i, id++ });
		}
		else {
			auto [l, v] = stk.top(); stk.pop();

			g[stk.top().second].push_back(v);
			if (ls) (*ls)[v] = l;
			if (rs) (*rs)[v] = i;
		}
	}

	return g;
}


//【色付き括弧列】
/*
* [0..n) が各 2 回含まれる列 p[0..2n) を，各 i∈[0..n) について
* 左右の i を色 i の "()" に対応させることで色付き括弧列とみなす．
*/


//【色付き括弧列の正しさ判定】O(n)
/*
* 色付き括弧列 p[0..2n) が正しい色付き括弧列かどうかを返す．
*/
bool colored_parenthesis_sequenceQ(const vi& p) {
	// verify : https://atcoder.jp/contests/arc076/tasks/arc076_c

	int n = sz(p) / 2;
	if (n == 0) return true;

	// '(' を +1, ')' を -1 に置き換えつつ，'(' の位置を記憶する．
	vi seen(n, -1); vi a(2 * n);
	rep(i, 2 * n) {
		if (seen[p[i]] != -1) {
			a[i] = -1;
		}
		else {
			a[i] = 1;
			seen[p[i]] = i;
		}
	}

	// ±1 に直した列の累積和をとり，括弧のネスト数を得る．
	vi acc(2 * n + 1);
	rep(i, 2 * n) acc[i + 1] = acc[i] + a[i];

	// 各番号について対応する括弧の位置のネスト数が等しいかどうかをチェックする．
	rep(i, 2 * n) {
		int j = seen[p[i]];
		if (i == j) continue;

		// j < i
		if (acc[i + 1] != acc[j]) return false;
	}

	return true;
}


//【色付き括弧列の違反対の数え上げ】O(n log n)
/*
* 色付き括弧列 p[0..2n) に含まれる違反括弧対の個数を返す．
* (i, j) が違反括弧対であるとは，p[0..2n) が部分列 i, j, i, j をもつことをいう．
*/
ll count_illegal_colored_parenthesis_pair(vi p) {
	// verify : https://atcoder.jp/contests/abc263/tasks/abc263_h

	int n = sz(p) / 2;
	if (n == 0) return true;

	// '(' の色が昇順になるよう彩色しなおす．
	vi col(n, -1); int c = 0; vi is_left(2 * n);
	rep(i, 2 * n) {
		// '(' を読んだ場合
		if (col[p[i]] == -1) {
			col[p[i]] = c;
			p[i] = c++;
			is_left[i] = true;
		}
		// ')' を読んだ場合
		else {
			p[i] = col[p[i]];
		}
	}

	ll res = 0;

	// ft_i[j] : p[0..i) までで，色 j の括弧が左端だけ読んだ状態か
	fenwick_tree<int> ft(n);

	rep(i, 2 * n) {
		// '(' を読んだ場合
		if (is_left[i]) {
			ft.add(p[i], 1);
		}
		// ')' を読んだ場合
		else {
			ft.add(p[i], -1);
			res += ft.sum(p[i], n);
		}
	}

	return res;
}


