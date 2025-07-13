#pragma once
#include "header.h"
#include "分析(グラフ).h"
// ■■■■■ 論理 ■■■■■


//【2-SAT】
/*
* Two_sat(int n) : O(n)
*	n 変数で初期化する．
*
* void add_clause(int i, bool bi, int j, bool bj) : O(1)
*	条件 (x[i] = bi) OR (x[j] = bj) を追加する．
*
* void add_imply(int i, bool bi, int j, bool bj) : O(1)
*	条件 (x[i] = bi) ⇒ (x[j] = bj) を追加する．
*
* void add_literal(int i, bool bi) : O(1)
*	条件 x[i] = bi を追加する．
*
* bool satisfiable() : O(n + m)（m は制約の数）
*	全ての条件を AND したものが充足可能かを返す．
*
* vb answer() : O(n)
*	真理値解を返す．satisfiable() の後に呼び出すこと．
*
* 利用：【強連結成分分解】
*/
class Two_sat {
	// 参考：https://tjkendev.github.io/procon-library/python/graph/2-sat.html

	int n; // 変数の数
	int FALSE, TRUE;
	Graph g; // 頂点は g[2*i] : !x[i], g[2*i+1] : x[i], g[2*n] : false, g[2*n+1] : true に対応
	vvi scc; // g の強連結成分分解結果
	vb sol; // 真理値解

public:
	// n 変数で初期化する．
	Two_sat(int n_) : n(n_), FALSE(2 * n), TRUE(FALSE + 1), g(TRUE + 1), sol(n) {
		// verify : https://judge.yosupo.jp/problem/two_sat

		rep(i, 2 * n) g[FALSE].push_back(i), g[i].push_back(TRUE);
		g[FALSE].push_back(TRUE);
	}

	// 条件 (x[i] = bi) OR (x[j] = bj) を追加する．
	void add_clause(int i, bool bi, int j, bool bj) {
		// verify : https://judge.yosupo.jp/problem/two_sat

		// a OR b を (!a ⇒ b AND !b ⇒ a) と考え辺を張る．
		// 頂点を倍化したので条件式も倍化している．
		g[2 * i + (int)(!bi)].push_back(2 * j + (int)(bj));
		g[2 * j + (int)(!bj)].push_back(2 * i + (int)(bi));
	}

	// 条件 (x[i] = bi) ⇒ (x[j] = bj) を追加する．
	void add_imply(int i, bool bi, int j, bool bj) {
		// verify : https://atcoder.jp/contests/abc277/tasks/abc277_h

		// a ⇒ b とその対偶 !b ⇒ !a と合わせて辺を張る．
		// 頂点を倍化したので条件式も倍化している．
		g[2 * i + (int)(bi)].push_back(2 * j + (int)(bj));
		g[2 * j + (int)(!bi)].push_back(2 * i + (int)(!bj));
	}

	// 条件 x[i] = bi を追加する．
	void add_literal(int i, bool bi) {
		// verify : https://atcoder.jp/contests/abc277/tasks/abc277_h

		// a を !a ⇒ FALSE AND TRUE ⇒ a と考え辺を張る．
		// 頂点を倍化したので条件式も倍化している．
		g[2 * i + (int)(!bi)].push_back(FALSE);
		g[TRUE].push_back(2 * i + (int)(bi));
	}

	// 全ての条件を AND したものが充足可能かを返す．
	bool satisfiable() {
		// verify : https://judge.yosupo.jp/problem/two_sat

		// g を強連結成分分解する．
		scc = strongly_connected_component(g);

		// 変数 → 何番目の強連結成分に属するか
		vi x_to_c(n + 1, -1);

		rep(i, sz(scc)) repe(v, scc[i]) {
			int x = v / 2; bool b = (bool)(v % 2);

			// x ⇒ !x と !x ⇒ x が共にあれば充足不可能
			if (x_to_c[x] == i) return false;

			// x ⇒ !x がありそうなら x = false，!x ⇒ x がありそうなら x = true とする．
			if (x_to_c[x] == -1) {
				if (x < n) sol[x] = !b;
				x_to_c[x] = i;
			}
		}

		return true;
	}

	// 真理値解を返す．
	vb answer() {
		// verify : https://judge.yosupo.jp/problem/two_sat

		return sol;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Two_sat ts) {
		rep(s, ts.n) {
			os << "!" << s << " => ";
			repe(t, ts.g[2 * s]) {
				int x = t / 2; bool b = (bool)(t % 2);
				if (x < ts.n) os << (b ? "" : "!") << x << " ";
				else os << (b ? "T" : "F") << " ";
			}
			os << endl;

			os << " " << s << " => ";
			repe(t, ts.g[2 * s + 1]) {
				int x = t / 2; bool b = (bool)(t % 2);
				if (x < ts.n) os << (b ? "" : "!") << x << " ";
				else os << (b ? "T" : "F") << " ";
			}
			os << endl;
		}
		os << "T => ";
		repe(t, ts.g[2 * ts.n + 1]) {
			int x = t / 2; bool b = (bool)(t % 2);
			if (x < ts.n) os << (b ? "" : "!") << x << " ";
			else os << (b ? "T" : "F") << " ";
		}
		os << endl;
		return os;
	}
#endif
};


//【3 つのうちの過半数が真】
/*
* x, y, z のうち過半数が真という条件は，以下の標準和積形で表される：
*	(x OR y) AND (y OR z) AND (z OR x)
* 
* verify : https://atcoder.jp/contests/arc161/tasks/arc161_e
*/


//【高々 1 個が真】
/*
* x[0..n) のうち真であるものは高々 1 個であるという条件は，
*	AND_{0≦i<j<n} (!x[i] OR !x[j])
* なる標準和積形で表される．また，新たな変数 y[0..n) を
*	y[i] = OR_i∈[0..j] x[j]
* なるものとみなして導入することで
*	    AND_i∈[0..n) (!x[i] OR y[i])		(x[i] ⇒ y[i])
*	AND AND_i∈[0..n-1) (!y[i] OR y[i+1])	(y[i] ⇒ y[i+1])
*	AND AND_i∈[0..n-1) (!y[i] OR !x[i+1])	(y[i] ⇒ !x[i+1])
* とも表される．
* 
* 参考 : https://drken1215.hatenablog.com/entry/2023/08/04/021846
*/


//【2-SAT の解の数え上げ】
/*
* 2-SAT の解の数え上げを多項式時間で行えるアルゴリズムは見つかっていない．
*/


//【フローへの帰着】
/*
* x[0..n) が論理変数のとき，x[i] と !x[i] のちょうど一方が真である．
* これを ST → X[i], X[i] → x[i], X[i] → !x[i] という流量 1 の 3 辺に対応させる．
* その他の条件もグラフで表されれば，「フローが n 流れる ⇔ 充足可能」となる．
*
* verify : https://atcoder.jp/contests/abc241/tasks/abc241_g
*/


