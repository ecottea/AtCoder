#pragma once
#include "header.h"
// ■■■■■ 座標圧縮 ■■■■■


//【座標圧縮】O(n log n)
/*
* 大きさ n の多重集合 a を 0 以上 |a| 未満の範囲に座標圧縮した結果を a_cp に格納し，その要素数を返す．
* また xs[j] に圧縮された座標 j に対応する元の座標を格納する．
*
* a に重複する要素がなければ，a_cp[i] は a[i] が昇順で何番目かを表し，
* xs[j] は昇順で j 番目の要素が何かを表す．
*/
template <typename T>
int coordinate_compression(const vector<T>& a, vi& a_cp, vector<T>* xs = nullptr) {
	// verify : https://atcoder.jp/contests/abc036/tasks/abc036_c

	int n = sz(a);
	if (xs == nullptr) xs = new vector<T>;

	// *xs : a の x 座標のユニークな昇順列
	*xs = a;
	uniq(*xs);

	// a[i] が xs において何番目かを求める．
	a_cp.resize(n);
	rep(i, n) a_cp[i] = lbpos(*xs, a[i]);

	return sz(*xs);
}


//【座標圧縮（区間）】O(n log n)
/*
* n 個の半開区間 [x1[i], x2[i]) を座標圧縮した結果を x1_cp[i], x2_cp[i] に格納する．
* また xs[i] に圧縮された座標 i に対応する元の座標を格納する．
* 戻り値として x 座標の数を返す．
*/
template <typename T>
int coordinate_compression_interval(const vector<T>& x1, const vector<T>& x2,
	vi& x1_cp, vi& x2_cp, vector<T>* xs = nullptr)
{
	// verify : https://atcoder.jp/contests/abc188/tasks/abc188_d

	int n = sz(x1);
	if (xs == nullptr) xs = new vector<T>;

	// x 座標だけを抜き出す．
	xs->clear();
	rep(i, n) {
		xs->push_back(x1[i]);
		xs->push_back(x2[i]);
	}

	// *xs : 区間端の x 座標のユニークな昇順列
	uniq(*xs);

	// 各区間の端の座標が xs において何番目かを求める．
	x1_cp.resize(n); x2_cp.resize(n);
	rep(i, n) {
		x1_cp[i] = lbpos(*xs, x1[i]);
		x2_cp[i] = lbpos(*xs, x2[i]);
	}

	return sz(*xs);
}


//【座標圧縮（長方形）】O(n log n)
/*
* n 個の半開長方形 [x1[i], x2[i]) * [y1[i], y2[i]) を座標圧縮した結果を
* x1_cp[i], y1_cp[i], x2_cp[i], y2_cp[i] に格納する．
* また xs[i], ys[j] に圧縮された座標 i, j に対応する元の座標を格納する．
* 戻り値として，(x 座標の数, y 座標の数) を返す．
*/
template <typename T>
pii coordinate_compression_rectangle(
	const vector<T>& x1, const vector<T>& y1, const vector<T>& x2, const vector<T>& y2,
	vi& x1_cp, vi& y1_cp, vi& x2_cp, vi& y2_cp,
	vector<T>* xs = nullptr, vector<T>* ys = nullptr)
{
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_4_A

	int n = sz(x1);
	if (xs == nullptr) xs = new vector<T>;
	if (ys == nullptr) ys = new vector<T>;

	// x, y それぞれの座標だけを抜き出す．
	xs->clear();
	ys->clear();
	rep(i, n) {
		xs->push_back(x1[i]);
		ys->push_back(y1[i]);
		xs->push_back(x2[i]);
		ys->push_back(y2[i]);
	}

	// *xs[*ys] : 区間端の x[y] 座標のユニークな昇順列
	uniq(*xs); uniq(*ys);

	// 各長方形の角の座標が xs, ys において何番目かを求める．
	x1_cp.resize(n); y1_cp.resize(n);
	x2_cp.resize(n); y2_cp.resize(n);
	rep(i, n) {
		x1_cp[i] = lbpos(*xs, x1[i]);
		y1_cp[i] = lbpos(*ys, y1[i]);
		x2_cp[i] = lbpos(*xs, x2[i]);
		y2_cp[i] = lbpos(*ys, y2[i]);
	}

	return { sz(*xs), sz(*ys) };
}


//【座標圧縮（直方体）】O(n log n)
/*
* n 個の半開直方体 [x1[i], x2[i]) * [y1[i], y2[i]) * [z1[i], z2[i]) を座標圧縮した結果を
* x1_cp[i], y1_cp[i], z1_cp[i], x2_cp[i], y2_cp[i], z2_cp[i] に格納する．
* また xs[i], ys[j], zs[k] に圧縮された座標 i, j, k に対応する元の座標を格納する．
* 戻り値として，(x 座標の数, y 座標の数, z 座標の数) を返す．
*/
template <typename T>
tuple<int, int, int> coordinate_compression_rectangular(
	const vector<T>& x1, const vector<T>& y1, const vector<T>& z1,
	const vector<T>& x2, const vector<T>& y2, const vector<T>& z2,
	vi& x1_cp, vi& y1_cp, vi& z1_cp, vi& x2_cp, vi& y2_cp, vi& z2_cp,
	vector<T>* xs = nullptr, vector<T>* ys = nullptr, vector<T>* zs = nullptr)
{
	// verify : https://atcoder.jp/contests/joi2013yo/tasks/joi2013yo_e

	int n = sz(x1);
	if (xs == nullptr) xs = new vector<T>;
	if (ys == nullptr) ys = new vector<T>;
	if (zs == nullptr) zs = new vector<T>;

	// x, y, z それぞれの座標だけを抜き出す．
	xs->clear();
	ys->clear();
	zs->clear();
	rep(i, n) {
		xs->push_back(x1[i]);
		ys->push_back(y1[i]);
		zs->push_back(z1[i]);
		xs->push_back(x2[i]);
		ys->push_back(y2[i]);
		zs->push_back(z2[i]);
	}

	// *xs[*ys, *zs] : 区間端の x[y, z] 座標のユニークな昇順列
	uniq(*xs); uniq(*ys); uniq(*zs);

	// 各直方体の角の座標が xs, ys, zs において何番目かを求める．
	x1_cp.resize(n); y1_cp.resize(n); z1_cp.resize(n);
	x2_cp.resize(n); y2_cp.resize(n); z2_cp.resize(n);
	rep(i, n) {
		x1_cp[i] = lbpos(*xs, x1[i]);
		y1_cp[i] = lbpos(*ys, y1[i]);
		z1_cp[i] = lbpos(*zs, z1[i]);
		x2_cp[i] = lbpos(*xs, x2[i]);
		y2_cp[i] = lbpos(*ys, y2[i]);
		z2_cp[i] = lbpos(*zs, z2[i]);
	}

	return { sz(*xs), sz(*ys), sz(*zs) };
}


//【座標圧縮（全順序集合）】O(n log n)
/*
* a[0..n) を全順序 cmp を用いて座標圧縮した結果を a_cp[0..m) に格納し，m を返す．
* また xs[j] に圧縮された座標 j に対応する元の座標を格納する．
*/
template <typename T>
int coordinate_compression(const vector<T>& a, vi& a_cp, function<bool(T, T)>& cmp, vector<T>* xs = nullptr) {
	// verify : https://atcoder.jp/contests/tenka1-2014-quala/tasks/tenka1_2014_qualA_d

	int n = sz(a);
	a_cp.resize(n);
	if (xs == nullptr) xs = new vector<T>;

	// *xs : a のユニークな昇順列
	*xs = a;
	sort(all(*xs), cmp);
	xs->erase(unique(all(*xs)), xs->end());

	// a[i] が xs において何番目かを求める．
	rep(i, n) a_cp[i] = (int)distance(xs->begin(), lower_bound(all(*xs), a[i], cmp));

	return sz(*xs);
}

