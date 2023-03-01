#pragma once
#include "header.h"
// ■■■■■ monotone 行列 ■■■■■


//【monotone】
/*
* 行列 a[0..n)[0..m) が monotone 行列であるとは，任意の i1 < i2 について，
*	argmin_j a[i1][j] ≦ argmin_j a[i2][j]
* が成り立つことをいう（下の行ほど最小値が右にある）
*/


//【totally monotone】
/*
* 行列 a[0..n)[0..m) が totally monotone 行列であるとは，任意の i1 < i2, j1 < j2 について
*	[ a[i1][j1] > a[i1][j2] ]
*	[ a[i2][j1] < a[i2][j2] ] ではない
* を満たすことをいう（Monge のように 2x2 小行列を全チェックするだけではだめなので注意）
* 
* 性質：
* a が totally monotone ⇒ a は monotone
* 列ベクトルを 1 つの折れ線とみると，どの 2 つの折れ線も高々 1 回しか交差しない．
*/


//【Monge】
/*
* 行列 a[0..n)[0..m) が Monge 行列であるとは，任意の i1 < i2, j1 < j2 について
*	a[i1][j1] + a[i2][j2] ≦ a[i1][j2] + a[i2][j1]
* を満たすことをいう．
* 
* 性質：
* a が Monge ⇒ a は totally monotone
* 条件は ∀i,j, a[i][j] + a[i+1][j+1] ≦ a[i][j+1] + a[i+1][j] に弱められる．
* 
* 参考 : https://lorent-kyopro.hatenablog.com/entry/2021/04/04/133958
*/


//【monotone minima】O(n + m log n)
/*
* 与えられた monotone 行列 a[0..n)[0..m) について，各行 i∈[0..n) の argmin_j を並べたリストを返す．
* f は f(i, j) で呼び出すと a[i][j] を返す関数とする．
*/
template <class T = ll>
vi monotone_minima(int n, int m, const function<T(int i, int j)>& f) {
	// 参考 : https://future-architect.github.io/articles/20210707a/
	// verify : https://atcoder.jp/contests/colopl2018-final/tasks/colopl2018_final_c

	vi argmin(n);

	function<void(int, int, int, int)> rf = [&](int iL, int iR, int jL, int jR) {
		if (iR - iL <= 0) return;

		int iM = (iL + iR) / 2;
		T a_min = numeric_limits<T>::max(); int jM = -1;
		repi(j, jL, jR - 1) if (chmin(a_min, f(iM, j))) jM = j;

		argmin[iM] = jM;
		rf(iL, iM, jL, jM + 1);
		rf(iM + 1, iR, jM, jR);
	};
	rf(0, n, 0, m);

	return argmin;
};


//【Monge 性判定】O(n m)
/*
* 行列 a[0..n)[0..m) が Monge 行列かを返す
*/
template <class T>
bool mongeQ(const vector<vector<T>>& a) {
	// verify : https://atcoder.jp/contests/abc224/tasks/abc224_b

	if (a.empty()) return true;

	int n = sz(a), m = sz(a[0]);
	rep(i, n - 1) rep(j, m - 1) {
		if (a[i][j] + a[i + 1][j + 1] > a[i][j + 1] + a[i + 1][j]) return false;
	}
	return true;
}


