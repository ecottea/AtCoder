#pragma once
#include "header.h"
#include "二分木.h"
// ■■■■■ 区間 ■■■■■


//【実区間の重なり判定】
/*
* 実閉区間 [l1, r1] と [l2, r2] が共通部分をもつ ⇔ max(l1, l2) <= min(r1, r2)
* verify : https://codeforces.com/contest/1680/problem/A
* 
* 実半開区間 [l1, r1) と [l2, r2) が共通部分をもつ ⇔ max(l1, l2) < min(r1, r2)
*
* 実開区間 (l1, r1) と (l2, r2) が共通部分をもつ ⇔ max(l1, l2) < min(r1, r2)
* verify : https://atcoder.jp/contests/arc090/tasks/arc090_c
*/


//【実区間の重なりの長さ】
/*
* 実閉区間 [l1, r1] と [l2, r2] の共通部分の長さは以下の式で与えられる：
*	min(min(r1, r2) - max(l1, l2), 0)
*
* verify : https://atcoder.jp/contests/abc070/tasks/abc070_b
*/


//【ソートの仕方】
/*
* 左から貰う DP をする場合は右端で昇順ソート
* 左から配る DP をする場合は左端で昇順ソート
* 
* verify : https://atcoder.jp/contests/arc026/tasks/arc026_3
*/


//【区間の結合（左端でソート）】O(n log n)
/*
* n 個の半開区間 [l[i], r[i]) を結合した j 番目の半開区間を [l2[j], r2[j]) に格納する．
* また結合した後の半開区間の個数を返す．
*/
template <class T>
int interval_union_lsort(vector<T> l, vector<T> r, vector<T>& l2, vector<T>& r2) {
	// verify : https://atcoder.jp/contests/abc256/tasks/abc256_d

	int n = sz(l);
	l2.clear();	r2.clear();

	if (n == 0) return 0;

	// 左端の小さい順にソートする（空の区間は無視する）
	vector<pair<T, T>> lr;
	rep(i, n) if (l[i] < r[i]) lr.emplace_back(l[i], r[i]);
	sort(all(lr));

	n = sz(lr);
	rep(i, n) tie(l[i], r[i]) = lr[i];

	repi(i, 1, n - 1) {
		// i 番目の区間の左端が処理中の区間の右端より右だった場合
		if (l[i] > r2[m - 1]) {
			// 区間の結合は完了したので，i 番目の区間を処理中の区間として次に進む．
			l2.push_back(l[i]); r2.push_back(r[i]);
			m++;
		}
		// i 番目の区間の左端が処理中の区間の右端より左だった場合（ちょうどを含む）
		else {
			// i 番目の区間を処理中の区間に結合し，右端を更新する．
			chmax(r2[m - 1], r[i]);
		}
	}

	return m;
}


//【区間の結合（右端でソート）】O(n log n)
/*
* n 個の半開区間 [l[i], r[i]) を結合した j 番目の半開区間を [l2[j], r2[j]) に格納する．
* また結合した後の半開区間の個数を返す．
*/
template <class T>
int interval_union_rsort(vector<T> l, vector<T> r, vector<T>& l2, vector<T>& r2) {
	// verify : https://atcoder.jp/contests/abc256/tasks/abc256_d

	int n = sz(l);
	l2.clear();	r2.clear();

	if (n == 0) return 0;

	// 右端の小さい順にソートする．
	vector<pair<T, T>> rl(n);
	rep(i, n) rl[i] = { r[i], l[i] };
	sort(all(rl));
	rep(i, n) tie(r[i], l[i]) = rl[i];

	l2.push_back(l[0]);	r2.push_back(r[0]);

	repi(i, 1, n - 1) {
		// 区間を結合できるかぎり左端を伸ばしていく．
		while (!r2.empty() && l[i] <= r2.back()) {
			chmin(l[i], l2.back());
			l2.pop_back(); r2.pop_back();
		}

		// 暫定的に 1 つの区間としておく．
		l2.push_back(l[i]); r2.push_back(r[i]);
	}

	return sz(l2);
}


//【自身以上の要素からなる区間】O(n)
/*
* 列 a[0..n) の各要素 a[i] について，a[i] を含み a[i] 以上の要素のみからなる
* 最大区間が [l[i], r[i]) であることを l, r に格納する．
* greater = false とすると大小関係を逆転して計算する．
*
* 制約：a[0..n) は互いに異なる．
*
* 利用：【デカルト木】
*/
template <class T>
void greater_interval(const vector<T>& a, vi& l, vi& r, bool greater = true) {
	// verify : https://yukicoder.me/problems/no/1031

	int n = sz(a);
	l.resize(n); r.resize(n);

	Binary_Tree ct;
	cartesian_tree(a, ct, greater);

	function<void(int, int, int)> rf = [&](int s, int pl, int pr) {
		l[s] = pl; r[s] = pr;

		if (ct[s].left != -1) {
			rf(ct[s].left, pl, s);
		}

		if (ct[s].right != -1) {
			rf(ct[s].right, s + 1, pr);
		}
	};

	rf(ct.root, 0, n);
}


