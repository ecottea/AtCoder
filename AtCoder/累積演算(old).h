#pragma once
#include "header.h"
// ■■■■■ 累積演算 ■■■■■


//【累積和】
/*
* Cumulative_sum(a) : O(n)
*	配列 a[0..n) で初期化する．
*
* sum(l, r) : O(1)
*	Σa[l..r) を返す．
*/
template <class T> struct Cumulative_sum {
	// verify : https://judge.yosupo.jp/problem/static_range_sum

	int n;

	// acc[i] : Σa[0..i)
	vector<T> acc;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_sum() : n(0) {}
	Cumulative_sum(const vector<T>& a) : n(sz(a)), acc(n + 1) {
		rep(i, n) acc[i + 1] = acc[i] + a[i];
	}

	// Σa[l..r) を返す．
	T sum(int l, int r) { return acc[r] - acc[l]; }
};


//【二次元累積和】
/*
* Cumulative_sum_2D(a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する．
*
* sum(x1, y1, x2, y2) : O(1)
*	Σa[x1..x2)[y1..y2) を返す．
*/
template <class T> struct Cumulative_sum_2D {
	// verify : https://atcoder.jp/contests/abc005/tasks/abc005_4

	int h, w;

	// acc[i][j] : Σa[0..i)[0..j)
	vector<vector<T>> acc;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_sum_2D() : h(0), w(0) {}
	Cumulative_sum_2D(const vector<vector<T>>& a)
		: h(sz(a)), w(sz(a[0])), acc(h + 1, vector<T>(w + 1)) {
		// 元データを仮格納する．
		rep(i, h) {
			rep(j, w) {
				acc[i + 1][j + 1] = a[i][j];
			}
		}

		// 縦方向に累積和をとる．
		repi(i, 1, h) {
			repi(j, 0, w) {
				acc[i][j] += acc[i - 1][j];
			}
		}

		// 横方向に累積和をとる．
		repi(i, 0, h) {
			repi(j, 1, w) {
				acc[i][j] += acc[i][j - 1];
			}
		}
	}

	// Σa[x1..x2)[y1..y2) を返す．
	T sum(int x1, int y1, int x2, int y2) {
		return acc[x2][y2] - acc[x1][y2] - acc[x2][y1] + acc[x1][y1];
	}
};


//【累積最大値】
/*
* Cumulative_max(a) : O(n)
*	配列 a[0..n) で初期化する
*
* left_max(r) : O(1)
*	max a[0..r] を返す．
*
* right_max(l) : O(1)
*	max a[l..n) を返す．
*
* without_max(i) : O(1)
*	max a[0..i)∪a(i..n) を返す．
*/
template <class T> struct Cumulative_max {
	int n;

	// acc_l[i] : max a[0..i)
	// acc_r[i] : max a[i..n)
	vector<T> acc_l, acc_r;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_max() : n(0) {}
	Cumulative_max(const vector<T>& a) : n(sz(a)), acc_l(n + 1), acc_r(n + 1) {
		acc_l[0] = acc_r[n] = numeric_limits<T>::min();
		rep(i, n) acc_l[i + 1] = max(acc_l[i], a[i]);
		repir(i, n - 1, 0) acc_r[i] = max(acc_r[i + 1], a[i]);
	}

	// max a[0..r] を返す．
	T left_max(int r) { return acc_l[r + 1]; }

	// max a[l..n) を返す．
	T right_max(int l) { return acc_r[l]; }

	// max a[0..i)∪a(i..n) を返す．
	T without_max(int i) { return max(acc_l[i], acc_r[i + 1]); }
};


//【累積最小値】
/*
* Cumulative_min(a, ninf) : O(n)
*	配列 a[0..n) で初期化する
*
* left_min(r) : O(1)
*	min a[0..r] を返す．
*
* right_min(l) : O(1)
*	min a[l..n) を返す．
*
* without_min(i) : O(1)
*	min a[0..i)∪a(i..n) を返す．
*/
template <class T> struct Cumulative_min {
	int n;

	// acc_l[i] : min a[0..i)
	// acc_r[i] : min a[i..n)
	vector<T> acc_l, acc_r;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_min() : n(0) {}
	Cumulative_min(const vector<T>& a) : n(sz(a)), acc_l(n + 1), acc_r(n + 1) {
		acc_l[0] = acc_r[n] = numeric_limits<T>::max();
		rep(i, n) acc_l[i + 1] = min(acc_l[i], a[i]);
		repir(i, n - 1, 0) acc_r[i] = min(acc_r[i + 1], a[i]);
	}

	// min a[0..r] を返す．
	T left_min(int r) { return acc_l[r + 1]; }

	// min a[l..n) を返す．
	T right_min(int l) { return acc_r[l]; }

	// min a[0..i)∪a(i..n) を返す．
	T without_min(int i) { return min(acc_l[i], acc_r[i + 1]); }
};


//【二次元累積最大値】
/*
* Cumulative_max_2d(a) : O(n)
*	二次元配列 a[0..h)[0..w) で初期化する
*
* ul_max(x, y) : O(1)
*	max a[0..x][0..y] を返す．
*
* ur_max(x, y) : O(1)
*	max a[0..x][y..w) を返す．
*
* dl_max(x, y) : O(1)
*	max a[x..h)[0..y] を返す．
*
* dr_max(x, y) : O(1)
*	max a[x..h)[y..w) を返す．
*
*/
template <class T> struct Cumulative_max_2d {
	int h, w;

	// acc_ul[i][j] : max a[0..i)[0..j)
	// acc_ur[i][j] : max a[0..i)[j..w)
	// acc_dl[i][j] : max a[i..h)[0..j)
	// acc_dr[i][j] : max a[i..h)[j..w)
	vector<vector<T>> acc_ul, acc_ur, acc_dl, acc_dr;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_max_2d() : h(0), w(0) {}
	Cumulative_max_2d(const vector<vector<T>>& a) : h(sz(a)), w(sz(a[0])) {
		acc_ul = acc_ur = acc_dl = acc_dr
			= vector<vector<T>>(h + 1, vector<T>(w + 1, numeric_limits<T>::min()));

		// 元データを仮格納する．
		rep(i, h) {
			rep(j, w) {
				acc_ul[i + 1][j + 1] = acc_ur[i + 1][j]
					= acc_dl[i][j + 1] = acc_dr[i][j] = a[i][j];
			}
		}

		// 下方向に累積 max をとる．
		repi(i, 1, h) {
			repi(j, 0, w) {
				chmax(acc_ul[i][j], acc_ul[i - 1][j]);
				chmax(acc_ur[i][j], acc_ur[i - 1][j]);
			}
		}

		// 上方向に累積 max をとる．
		repir(i, h - 1, 0) {
			repi(j, 0, w) {
				chmax(acc_dl[i][j], acc_dl[i + 1][j]);
				chmax(acc_dr[i][j], acc_dr[i + 1][j]);
			}
		}

		// 右方向に累積 max をとる．
		repi(i, 0, h) {
			repi(j, 1, w) {
				chmax(acc_ul[i][j], acc_ul[i][j - 1]);
				chmax(acc_dl[i][j], acc_dl[i][j - 1]);
			}
		}

		// 左方向に累積 max をとる．
		repi(i, 0, h) {
			repir(j, w - 1, 0) {
				chmax(acc_ur[i][j], acc_ur[i][j + 1]);
				chmax(acc_dr[i][j], acc_dr[i][j + 1]);
			}
		}

	}

	// max a[0..x][0..y] を返す．
	T ul_max(int x, int y) { return acc_ul[x + 1][y + 1]; }

	// max a[0..x][y..w) を返す．
	T ur_max(int x, int y) { return acc_ur[x + 1][y]; }

	// max a[x..h)[0..y] を返す．
	T dl_max(int x, int y) { return acc_dl[x][y + 1]; }

	// max a[x..h)[y..w) を返す．
	T dr_max(int x, int y) { return acc_dr[x][y]; }
};


//【二次元累積最小値】
/*
* Cumulative_min_2d(a) : O(n)
*	二次元配列 a[0..h)[0..w) で初期化する
*
* ul_min(x, y) : O(1)
*	min a[0..x][0..y] を返す．
*
* ur_min(x, y) : O(1)
*	min a[0..x][y..w) を返す．
*
* dl_min(x, y) : O(1)
*	min a[x..h)[0..y] を返す．
*
* dr_min(x, y) : O(1)
*	min a[x..h)[y..w) を返す．
*
*/
template <class T> struct Cumulative_min_2d {
	int h, w;

	// acc_ul[i][j] : min a[0..i)[0..j)
	// acc_ur[i][j] : min a[0..i)[j..w)
	// acc_dl[i][j] : min a[i..h)[0..j)
	// acc_dr[i][j] : min a[i..h)[j..w)
	vector<vector<T>> acc_ul, acc_ur, acc_dl, acc_dr;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_min_2d() : h(0), w(0) {}
	Cumulative_min_2d(const vector<vector<T>>& a) : h(sz(a)), w(sz(a[0])) {
		acc_ul = acc_ur = acc_dl = acc_dr
			= vector<vector<T>>(h + 1, vector<T>(w + 1, numeric_limits<T>::max()));

		// 元データを仮格納する．
		rep(i, h) {
			rep(j, w) {
				acc_ul[i + 1][j + 1] = acc_ur[i + 1][j]
					= acc_dl[i][j + 1] = acc_dr[i][j] = a[i][j];
			}
		}

		// 下方向に累積 min をとる．
		repi(i, 1, h) {
			repi(j, 0, w) {
				chmin(acc_ul[i][j], acc_ul[i - 1][j]);
				chmin(acc_ur[i][j], acc_ur[i - 1][j]);
			}
		}

		// 上方向に累積 min をとる．
		repir(i, h - 1, 0) {
			repi(j, 0, w) {
				chmin(acc_dl[i][j], acc_dl[i + 1][j]);
				chmin(acc_dr[i][j], acc_dr[i + 1][j]);
			}
		}

		// 右方向に累積 min をとる．
		repi(i, 0, h) {
			repi(j, 1, w) {
				chmin(acc_ul[i][j], acc_ul[i][j - 1]);
				chmin(acc_dl[i][j], acc_dl[i][j - 1]);
			}
		}

		// 左方向に累積 min をとる．
		repi(i, 0, h) {
			repir(j, w - 1, 0) {
				chmin(acc_ur[i][j], acc_ur[i][j + 1]);
				chmin(acc_dr[i][j], acc_dr[i][j + 1]);
			}
		}

	}

	// min a[0..x][0..y] を返す．
	T ul_min(int x, int y) { return acc_ul[x + 1][y + 1]; }

	// min a[0..x][y..w) を返す．
	T ur_min(int x, int y) { return acc_ur[x + 1][y]; }

	// min a[x..h)[0..y] を返す．
	T dl_min(int x, int y) { return acc_dl[x][y + 1]; }

	// min a[x..h)[y..w) を返す．
	T dr_min(int x, int y) { return acc_dr[x][y]; }
};


//【スライド最大値】O(n)
/*
* 配列 a[0..n) に対し max a[i..i+w) を a_max[i] に返す．
*/
template <class T>
void slide_maximum(const vector<T>& a, int w, vector<T>& a_max) {
	// 参考：https://qiita.com/kuuso1/items/318d42cd089a49eeb332

	int n = sz(a);
	a_max.resize(n + 1 - w);

	// 現在の最大値の位置と，今後最大値になりうる数の位置を入れておくデック
	deque<int> q;

	rep(i, n) {
		// 現在の最大値が注目区間の外に出たらデックの先頭から除去する．
		if (!q.empty() && q.front() <= i - w) {
			q.pop_front();
		}

		// 新しく区間に入る数より小さい数は最大値とはなりえないので
		// デックの末尾から除去する．
		while (!q.empty() && a[q.back()] < a[i]) {
			q.pop_back();
		}

		// 新しく区間に入る数は，今後最大値となる可能性があるので
		// デックの末尾に追加する．
		q.push_back(i);

		// 注目区間の幅が w になっていれば結果を記録する．
		if (i >= w - 1) {
			a_max[i - w + 1] = a[q.front()];
		}
	}
}


//【スライド最小値】O(n)
/*
* 配列 a[0..n) に対し min a[i..i+w) を a_min[i] に返す．
*/
template <class T>
void slide_minimum(const vector<T>& a, int w, vector<T>& a_min) {
	// 参考：https://qiita.com/kuuso1/items/318d42cd089a49eeb332

	int n = sz(a);
	a_min.resize(n + 1 - w);

	// 現在の最小値の位置と，今後最小値になりうる数の位置を入れておくデック
	deque<int> q;

	rep(i, n) {
		// 現在の最小値が注目区間の外に出たらデックの先頭から除去する．
		if (!q.empty() && q.front() <= i - w) {
			q.pop_front();
		}

		// 新しく区間に入る数より大きい数は最小値とはなりえないので
		// デックの末尾から除去する．
		while (!q.empty() && a[q.back()] > a[i]) {
			q.pop_back();
		}

		// 新しく区間に入る数は，今後最小値となる可能性があるので
		// デックの末尾に追加する．
		q.push_back(i);

		// 注目区間の幅が w になっていれば結果を記録する．
		if (i >= w - 1) {
			a_min[i - w + 1] = a[q.front()];
		}
	}
}


//【二次元スライド最大値】O(h w)
/*
* 二次元配列 a[0..h)[0..w) に対し max a[i..i+dh)[j..j+dw) を a_max[i][j] に返す．
*/
template <class T>
void slide_maximum_2d(const vector<vector<T>>& a, int dh, int dw, vector<vector<T>>& a_max) {
	// verify : https://atcoder.jp/contests/abc228/tasks/abc228_f

	int h = sz(a), w = sz(a[0]);
	vector<vector<T>> a_tmp(h - dh + 1, vector<T>(w));
	a_max = vector<vector<T>>(h - dh + 1, vector<T>(w - dw + 1));

	// 縦方向のスライド最小値を求め a_tmp に格納する．
	rep(j, w) {
		// 現在の最大値の位置と，今後最大値になりうる数の位置を入れておくデック
		deque<int> q;

		rep(i, h) {
			// 現在の最大値が注目区間の外に出たらデックの先頭から除去する．
			if (!q.empty() && q.front() <= i - dh) {
				q.pop_front();
			}

			// 新しく区間に入る数より小さい数は最大値とはなりえないので
			// デックの末尾から除去する．
			while (!q.empty() && a[q.back()][j] < a[i][j]) {
				q.pop_back();
			}

			// 新しく区間に入る数は，今後最大値となる可能性があるので
			// デックの末尾に追加する．
			q.push_back(i);

			// 注目区間の幅が dh になっていれば結果を記録する．
			if (i >= dh - 1) {
				a_tmp[i - dh + 1][j] = a[q.front()][j];
			}
		}
	}

	// 横方向のスライド最小値を求め a_max に格納する
	repi(i, 0, h - dh) {
		// 現在の最大値の位置と，今後最大値になりうる数の位置を入れておくデック
		deque<int> q;

		rep(j, w) {
			// 現在の最大値が注目区間の外に出たらデックの先頭から除去する．
			if (!q.empty() && q.front() <= j - dw) {
				q.pop_front();
			}

			// 新しく区間に入る数より小さい数は最大値とはなりえないので
			// デックの末尾から除去する．
			while (!q.empty() && a_tmp[i][q.back()] < a_tmp[i][j]) {
				q.pop_back();
			}

			// 新しく区間に入る数は，今後最大値となる可能性があるので
			// デックの末尾に追加する．
			q.push_back(j);

			// 注目区間の幅が dw になっていれば結果を記録する．
			if (j >= dw - 1) {
				a_max[i][j - dw + 1] = a_tmp[i][q.front()];
			}
		}
	}
}


//【二次元スライド最小値】O(h w)
/*
* 二次元配列 a[0..h)[0..w) に対し min a[i..i+dh)[j..j+dw) を a_min[i][j] に返す．
*/
template <class T>
void slide_minimum_2D(const vector<vector<T>>& a, int dh, int dw, vector<vector<T>>& a_min) {
	// verify : https://atcoder.jp/contests/abc228/tasks/abc228_f

	int h = sz(a), w = sz(a[0]);
	vector<vector<T>> a_tmp(h - dh + 1, vector<T>(w));
	a_min = vector<vector<T>>(h - dh + 1, vector<T>(w - dw + 1));

	// 縦方向のスライド最小値を求め a_tmp に格納する．
	rep(j, w) {
		// 現在の最小値の位置と，今後最小値になりうる数の位置を入れておくデック
		deque<int> q;

		rep(i, h) {
			// 現在の最小値が注目区間の外に出たらデックの先頭から除去する．
			if (!q.empty() && q.front() <= i - dh) {
				q.pop_front();
			}

			// 新しく区間に入る数より大きい数は最小値とはなりえないので
			// デックの末尾から除去する．
			while (!q.empty() && a[q.back()][j] > a[i][j]) {
				q.pop_back();
			}

			// 新しく区間に入る数は，今後最小値となる可能性があるので
			// デックの末尾に追加する．
			q.push_back(i);

			// 注目区間の幅が dh になっていれば結果を記録する．
			if (i >= dh - 1) {
				a_tmp[i - dh + 1][j] = a[q.front()][j];
			}
		}
	}

	// 横方向のスライド最小値を求め a_min に格納する
	repi(i, 0, h - dh) {
		// 現在の最小値の位置と，今後最小値になりうる数の位置を入れておくデック
		deque<int> q;

		rep(j, w) {
			// 現在の最小値が注目区間の外に出たらデックの先頭から除去する．
			if (!q.empty() && q.front() <= j - dw) {
				q.pop_front();
			}

			// 新しく区間に入る数より大きい数は最小値とはなりえないので
			// デックの末尾から除去する．
			while (!q.empty() && a_tmp[i][q.back()] > a_tmp[i][j]) {
				q.pop_back();
			}

			// 新しく区間に入る数は，今後最小値となる可能性があるので
			// デックの末尾に追加する．
			q.push_back(j);

			// 注目区間の幅が dw になっていれば結果を記録する．
			if (j >= dw - 1) {
				a_min[i][j - dw + 1] = a_tmp[i][q.front()];
			}
		}
	}
}


