#pragma once
#include "header.h"
// ■■■■■ 座標圧縮 ■■■■■


//【座標圧縮】O(n log n)
/*
* 大きさ n の集合 a を 0 以上 |a| 未満の範囲に座標圧縮した結果を a_cp に格納する．
* また x[j] に圧縮された座標 j に対応する元の座標を格納する．
* 戻り値として |a| を返す．
*
* a に重複する要素がなければ，a_cp[i] は a[i] が小さい方から何番目かを表し，
* x[j] は小さい方から j 番目の要素が何かを表す．
*/
template <typename T>
int coordinate_compression(const vector<T>& a, vi& a_cp, vector<T>* x = nullptr) {
	int n = sz(a);
	if (x == nullptr) x = new vector<T>;

	// unique() のためにまずソートを行う．
	*x = a;
	sort(all(*x));

	// 隣り合う重複する要素を末尾に送り，末尾を消去する．
	x->erase(unique(all(*x)), x->end());

	// a[i] が x において何番目かを求める．
	a_cp = vi(n);
	rep(i, n) {
		a_cp[i] = (int)distance(x->begin(), lower_bound(all(*x), a[i]));
	}

	return sz(*x);
}


//【座標圧縮（区間）】O(n log n)
/*
* n 個の半開区間 [l, r) の集合 a を座標圧縮した結果を a_cp に格納する．
* また x[i] に圧縮された座標 i に対応する元の座標を格納する．
* 戻り値として x 座標の数を返す．
*/
int coordinate_compression_interval(vector<pll>& a, vector<pii>& a_cp, vl* x = nullptr) {
	int n = sz(a);
	if (x == nullptr) x = new vl;

	// x 座標だけを抜き出す．
	x->clear();
	rep(i, n) {
		ll l, r;
		tie(l, r) = a[i];

		x->push_back(l);
		x->push_back(r);
	}

	// 左右の余白が消えてしまわないようにする．
	x->push_back(-INFL);
	x->push_back(INFL);

	// unique() のためにまずソートを行う．
	sort(all(*x));

	// 隣り合う重複する要素を末尾に送り，末尾を消去する．
	x->erase(unique(all(*x)), x->end());

	// a[i] の左右の座標が x において何番目かを求める．
	a_cp = vector<pii>(n);
	rep(i, n) {
		ll l, r;
		tie(l, r) = a[i];

		int l_cp = (int)distance(x->begin(), lower_bound(all(*x), l));
		int r_cp = (int)distance(x->begin(), lower_bound(all(*x), r));
		a_cp[i] = { l_cp, r_cp };
	}

	return sz(*x);
}


//【座標圧縮（長方形）】O(n log n)
/*
* n 個の半開長方形 [x1, x2) * [y1, y2) の集合 a を
* 座標圧縮した結果を a_cp に格納する．
* 各長方形 a[i] は，{x1, y1, x2, y2} を順に並べて表す．
* また x[i], y[j] に圧縮された座標 i, j に対応する元の座標を格納する．
* 戻り値として，(x 座標の数, y 座標の数) を返す．
*/
pii coordinate_compression_rectangle(vector<tuple<ll, ll, ll, ll>>& a,
	vector<tuple<int, int, int, int>>& a_cp, vl* x = nullptr, vl* y = nullptr) {
	int n = sz(a);
	if (x == nullptr) x = new vl;
	if (y == nullptr) y = new vl;

	// x, y それぞれの座標だけを抜き出す．
	x->clear();
	y->clear();
	rep(i, n) {
		ll x1, y1, x2, y2;
		tie(x1, y1, x2, y2) = a[i];

		x->push_back(x1);
		y->push_back(y1);
		x->push_back(x2);
		y->push_back(y2);
	}

	// 上下左右の余白が消えてしまわないようにする．
	x->push_back(-INFL);
	y->push_back(-INFL);
	x->push_back(INFL);
	y->push_back(INFL);

	// unique() のためにまずソートを行う．
	sort(all(*x));
	sort(all(*y));

	// 隣り合う重複する要素を末尾に送り，末尾を消去する．
	x->erase(unique(all(*x)), x->end());
	y->erase(unique(all(*y)), y->end());

	// a[i] の左上や右下の座標が x, y において何番目かを求める．
	a_cp = vector<tuple<int, int, int, int>>(n);
	rep(i, n) {
		ll x1, y1, x2, y2;
		tie(x1, y1, x2, y2) = a[i];

		int x1_cp = (int)distance(x->begin(), lower_bound(all(*x), x1));
		int y1_cp = (int)distance(y->begin(), lower_bound(all(*y), y1));
		int x2_cp = (int)distance(x->begin(), lower_bound(all(*x), x2));
		int y2_cp = (int)distance(y->begin(), lower_bound(all(*y), y2));
		a_cp[i] = { x1_cp, y1_cp, x2_cp, y2_cp };
	}

	return { sz(*x), sz(*y) };
}


