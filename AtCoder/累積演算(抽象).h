#pragma once
#include "header.h"
// ■■■■■ 累積演算（抽象代数上） ■■■■■


//【累積積（群）】
/*
* Cumulative_prod<S, op, e, inv>(a) : O(n)
*	配列 a[0..n) で初期化する．
*	要素は群 <S, op, e, inv> の元とする．
*
* prod(l, r) : O(1)
*	Πa[l..r) を返す．（空なら e() を返す，範囲外の値は e() とみなす）
*/
template <class S, S(*mul)(S, S), S(*e)(), S(*inv_)(S)>
struct Cumulative_prod {
	// verify : https://judge.yosupo.jp/problem/static_range_sum

	int n;

	// acc[i]     : a[0] a[1] ... a[i - 1]
	// acc_inv[i] : a[i - 1]^(-1) ... a[1]^(-1) a[0]^(-1)
	vector<S> acc, acc_inv;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_prod() : n(0) {}
	Cumulative_prod(const vector<S>& a) : n(sz(a)), acc(n + 1), acc_inv(n + 1) {
		acc[0] = acc_inv[0] = e();
		rep(i, n) {
			acc[i + 1] = op(acc[i], a[i]);
			acc_inv[i + 1] = op(inv(a[i]), acc_inv[i]);
		}
	}

	// Πa[l..r) を返す．
	S prod(int l, int r) {
		if (l >= r) return e();

		// a[l] ... a[r - 1]
		// = (a[l - 1]^(-1) ... a[0]^(-1)) (a[0] ... a[l - 1] a[l] ... a[r - 1])
		return op(acc_inv[max(l, 0)], acc[min(r, n)]);
	}
};


//【二次元累積和（アーベル群）】
/*
* Cumulative_sum_2d<S, op, o, inv>(a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する．
*	要素はアーベル群 <S, op, o, inv> の元とする．
*
* sum(x1, y1, x2, y2) : O(1)
*	Σa[x1..x2)[y1..y2) を返す．（空なら o() を返す，範囲外の値は o() とみなす）
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
struct Cumulative_sum_2d {
	// verify : https://atcoder.jp/contests/abc005/tasks/abc005_4

	int h, w;

	// acc[i][j] : Σa[0..i)[0..j)
	vector<vector<S>> acc;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_sum_2d() : h(0), w(0) {}
	Cumulative_sum_2d(const vector<vector<S>>& a)
		: h(sz(a)), w(sz(a[0])), acc(h + 1, vector<S>(w + 1, o())) {
		// 元データを仮格納する．
		rep(i, h) {
			rep(j, w) {
				acc[i + 1][j + 1] = a[i][j];
			}
		}

		// 縦方向に累積和をとる．
		repi(i, 1, h) {
			repi(j, 0, w) {
				acc[i][j] = op(acc[i][j], acc[i - 1][j]);
			}
		}

		// 横方向に累積和をとる．
		repi(i, 0, h) {
			repi(j, 1, w) {
				acc[i][j] = op(acc[i][j], acc[i][j - 1]);
			}
		}
	}

	// Σa[x1..x2)[y1..y2) を返す．
	S sum(int x1, int y1, int x2, int y2) {
		chmax(x1, 0);
		chmax(y1, 0);
		chmin(x2, h);
		chmin(y2, w);

		S res = o();
		res = op(res, acc[x2][y2]);
		res = op(res, inv(acc[x1][y2]));
		res = op(res, inv(acc[x2][y1]));
		res = op(res, acc[x1][y1]);

		return res;
	}
};


//【累積非可逆積（モノイド）】
/*
* Cumulative_lossy_prod<S, op, e>(a) : O(n)
*	配列 a[0..n) で初期化する
*	要素はモノイド <S, op, e> の元とする．
*
* left_prod(r) : O(1)
*	Πa[0..r] を返す．
*
* right_prod(l) : O(1)
*	Πa[l..n) を返す．
*
* without_prod(i) : O(1)
*	Πa[0..i)a(i..n) を返す．
*/
template <class S, S(*op)(S, S), S(*e)()>
struct Cumulative_lossy_prod {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_1_D

	int n;

	// acc_l[i] : Πa[0..i)
	// acc_r[i] : Πa[i..n)
	vector<S> acc_l, acc_r;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_lossy_prod() : n(0) {}
	Cumulative_lossy_prod(const vector<S>& a) : n(sz(a)), acc_l(n + 1), acc_r(n + 1) {
		acc_l[0] = acc_r[n] = e();
		rep(i, n) acc_l[i + 1] = op(acc_l[i], a[i]);
		repir(i, n - 1, 0) acc_r[i] = op(a[i], acc_r[i + 1]);
	}

	// max a[0..r] を返す．
	S left_prod(int r) { return acc_l[r + 1]; }

	// max a[l..n) を返す．
	S right_prod(int l) { return acc_r[l]; }

	// max a[0..i)∪a(i..n) を返す．
	S without_prod(int i) { return op(acc_l[i], acc_r[i + 1]); }
};


//【二次元累積非可逆和（可換モノイド）】
/*
* Cumulative_lossy_sum_2d<S, op, o>(a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する
*	要素は可換モノイド <S, op, o> の元とする．
*
* ul_sum(x, y) : O(1)
*	Σa[0..x][0..y] を返す．
*
* ur_sum(x, y) : O(1)
*	Σa[0..x][y..w) を返す．
*
* dl_sum(x, y) : O(1)
*	Σa[x..h)[0..y] を返す．
*
* dr_sum(x, y) : O(1)
*	Σa[x..h)[y..w) を返す．
*/
template <class S, S(*op)(S, S), S(*o)()>
struct Cumulative_lossy_sum_2d {
	int h, w;

	// acc_ul[i][j] : Σa[0..i)[0..j)
	// acc_ur[i][j] : Σa[0..i)[j..w)
	// acc_dl[i][j] : Σa[i..h)[0..j)
	// acc_dr[i][j] : Σa[i..h)[j..w)
	vector<vector<S>> acc_ul, acc_ur, acc_dl, acc_dr;

	// コンストラクタ（初期化なし，配列で初期化）
	Cumulative_lossy_sum_2d() : h(0), w(0) {}
	Cumulative_lossy_sum_2d(const vector<vector<S>>& a) : h(sz(a)), w(sz(a[0])) {
		acc_ul = acc_ur = acc_dl = acc_dr
			= vector<vector<S>>(h + 1, vector<S>(w + 1, o()));

		// 元データを仮格納する．
		rep(i, h) {
			rep(j, w) {
				acc_ul[i + 1][j + 1] = acc_ur[i + 1][j]
					= acc_dl[i][j + 1] = acc_dr[i][j] = a[i][j];
			}
		}

		// 下方向に累積和をとる．
		repi(i, 1, h) {
			repi(j, 0, w) {
				acc_ul[i][j] = op(acc_ul[i][j], acc_ul[i - 1][j]);
				acc_ur[i][j] = op(acc_ur[i][j], acc_ur[i - 1][j]);
			}
		}

		// 上方向に累積和をとる．
		repir(i, h - 1, 0) {
			repi(j, 0, w) {
				acc_dl[i][j] = op(acc_dl[i][j], acc_dl[i + 1][j]);
				acc_dr[i][j] = op(acc_dr[i][j], acc_dr[i + 1][j]);
			}
		}

		// 右方向に累積和をとる．
		repi(i, 0, h) {
			repi(j, 1, w) {
				acc_ul[i][j] = op(acc_ul[i][j], acc_ul[i][j - 1]);
				acc_dl[i][j] = op(acc_dl[i][j], acc_dl[i][j - 1]);
			}
		}

		// 左方向に累積和をとる．
		repi(i, 0, h) {
			repir(j, w - 1, 0) {
				acc_ur[i][j] = op(acc_ur[i][j], acc_ur[i][j + 1]);
				acc_dr[i][j] = op(acc_dr[i][j], acc_dr[i][j + 1]);
			}
		}

	}

	// Σa[0..x][0..y] を返す．
	S ul_sum(int x, int y) { return acc_ul[x + 1][y + 1]; }

	// Σa[0..x][y..w) を返す．
	S ur_sum(int x, int y) { return acc_ur[x + 1][y]; }

	// Σa[x..h)[0..y] を返す．
	S dl_sum(int x, int y) { return acc_dl[x][y + 1]; }

	// Σa[x..h)[y..w) を返す．
	S dr_sum(int x, int y) { return acc_dr[x][y]; }
};


//【スライド最小値（整列集合）】O(n)
/*
* 配列 a[0..n) に対し min a[i..i+w) を a_min[i] に返す．
*/
template <class S, bool(*comp)(S, S), S(*inf)()>
void slide_minimum(const vector<S>& a, int w, vector<S>& a_min) {
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
		while (!q.empty() && comp(a[i], a[q.back()])) {
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


//【二次元スライド最小値（整列集合）】O(h w)
/*
* 二次元配列 a[0..h)[0..w) に対し min a[i..i+dh)[j..j+dw) を a_min[i][j] に返す．
*/
template <class S, bool(*comp)(S, S), S(*inf)()>
void slide_minimum_2d(const vector<vector<S>>& a, int dh, int dw, vector<vector<S>>& a_min) {
	// verify : https://atcoder.jp/contests/abc228/tasks/abc228_f

	int h = sz(a), w = sz(a[0]);
	vector<vector<S>> a_tmp(h - dh + 1, vector<S>(w));
	a_min = vector<vector<S>>(h - dh + 1, vector<S>(w - dw + 1));

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
			while (!q.empty() && comp(a[i][j], a[q.back()][j])) {
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
			while (!q.empty() && comp(a_tmp[i][j], a_tmp[i][q.back()])) {
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


//【Sparse Table（冪等可換モノイド）】
/*
* Sparse_table<S, op, o>(a) : O(n log n)
*	配列 a[0..n) で初期化する
*	要素は冪等可換モノイド <S, op, o> の元とする．
*
* sum(l, r) : O(1)
*	Σa[l..r) を返す．（空なら o() を返す）
*/
template <class S, S(*op)(S, S), S(*o)()>
struct Sparse_table {
	// 参考 : https://tookunn.hatenablog.com/entry/2016/07/13/211148
	// verify : https://codeforces.com/contest/689/problem/D

	int n, m;

	// acc[j][i] : Σa[i..i+2^j)
	vector<vector<S>> acc;

	// コンストラクタ（初期化なし，配列で初期化）
	Sparse_table() : n(0), m(0) {}
	Sparse_table(const vector<S>& a) : n(sz(a)), m(msb(n) + 1), acc(m, vector<S>(n)) {
		rep(i, n) acc[0][i] = a[i];

		repi(j, 1, m - 1) {
			int d = 1 << (j - 1);
			rep(i, n - d) {
				acc[j][i] = op(acc[j - 1][i], acc[j - 1][i + d]);
			}
		}
	}

	// Σa[l..r) を返す．
	S sum(int l, int r) {
		if (l >= r) return o();

		int j = msb(r - l);
		return op(acc[j][l], acc[j][r - (1 << j)]);
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Sparse_table& st) {
		rep(j, st.m) {
			rep(i, st.n) os << st.acc[j][i] << " ";
			os << "\n";
		}
		return os;
	}
};


//【二次元 Sparse Table（冪等可換モノイド）】
/*
* Sparse_table<S, op, o>(a) : O(n log n)
*	二次元配列 a[0..h)[0..w) で初期化する
*	要素は冪等可換モノイド <S, op, o> の元とする．
*
* sum(x1, y1, x2, y2) : O(1)
*	Σa[x1..x2)[y1..y2) を返す．（空なら o() を返す）
*/
template <class S, S(*op)(S, S), S(*o)()>
struct Sparse_table_2d {
	// 参考 : https://kopricky.github.io/code/DataStructure_Advanced/sparse_table_2D.html
	// verify : https://codeforces.com/problemset/problem/713/D

	int h, w, bh, bw;

	// acc[bx][by][x][y] : Σa[x..x+2^bx)[y..y+2^by)
	vector<vector<vector<vector<S>>>> acc;

	// コンストラクタ（初期化なし，二次元配列で初期化）
	Sparse_table_2d() : h(0), w(0), bh(0), bw(0) {}
	Sparse_table_2d(const vector<vector<S>>& a) : h(sz(a)), w(sz(a[0])), bh(msb(h) + 1), bw(msb(w) + 1),
		acc(bh, vector<vector<vector<S>>>(bw, vector<vector<S>>(h, vector<S>(w))))
	{
		rep(x, h) rep(y, w) acc[0][0][x][y] = a[x][y];

		repi(bx, 1, bh - 1) {
			int dx = 1 << (bx - 1);
			rep(x, h - dx) {
				rep(y, w) {
					acc[bx][0][x][y] = op(acc[bx - 1][0][x][y], acc[bx - 1][0][x + dx][y]);
				}
			}
		}

		rep(bx, bh) {
			int dx = (bx > 0 ? 1 << (bx - 1) : 0);
			repi(by, 1, bw - 1) {
				int dy = 1 << (by - 1);
				rep(x, h - dx) {
					rep(y, w - dy) {
						acc[bx][by][x][y]
							= op(acc[bx][by - 1][x][y], acc[bx][by - 1][x][y + dy]);
					}
				}
			}
		}
	}

	// Σa[x1..x2)[y1..y2) を返す．
	S sum(int x1, int y1, int x2, int y2) {
		if (x1 >= x2 || y1 >= y2) return o();

		int bx = msb(x2 - x1), by = msb(y2 - y1);

		S res = o();
		res = op(res, acc[bx][by][x1][y1]);
		res = op(res, acc[bx][by][x2 - (1 << bx)][y1]);
		res = op(res, acc[bx][by][x1][y2 - (1 << by)]);
		res = op(res, acc[bx][by][x2 - (1 << bx)][y2 - (1 << by)]);

		return res;
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Sparse_table_2d& st) {
		rep(bx, st.bh) {
			rep(by, st.bw) {
				rep(x, st.h) {
					rep(y, st.w) os << st.acc[bx][by][x][y] << " ";
					os << "\n";
				}
				os << "\n";
			}
		}
		return os;
	}
};


