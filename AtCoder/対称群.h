#pragma once
#include "header.h"
#include "座標圧縮.h"
// ■■■■■ 対称群（置換） ■■■■■



//【転倒数】O(n log n)
/*
* 長さ n の配列 a の転倒数を返す．
*
* 利用：【座標圧縮】
*/
template <class T>
ll inversion_number(vector<T>& a) {
	int n = sz(a);

	// a を [0, |a|) に座標圧縮した結果を a_cp に格納する．
	vi b;
	vector<T> tmp;
	int m = coordinate_compression(a, b, tmp);

	// fw[i] : 今まで見てきた範囲に値 i が何個あったか．
	fenwick_tree<int> fw(m);
	ll res = 0;
	rep(i, n) {
		fw.add(b[i], 1);

		// 自身より大きい数が今までに何個あったか調べ，加算する．
		res += fw.sum(b[i] + 1, m);
	}
	return res;
}


//【置換の分解】O(n)
/*
* [0..n) の置換 p を巡回置換の積に分解して cycles に格納する．
* p は任意の i を p[i] に動かすような置換を表す．
*/
int permutation_decomposition(const vi& p, vvi& cycles) {
	int n = sz(p);

	int m = 0;
	vb seen(n);

	rep(i, n) {
		// 抽出済のサイクルに含まれるなら次へ
		if (seen[i]) continue;

		// 新しいサイクルを発見
		cycles.push_back(vi());
		m++;

		// サイクルを順に格納していく．
		int s = i;
		do {
			cycles[m - 1].push_back(s);
			seen[s] = true;
			s = p[s];
		} while (s != i);
	}

	return m;
}


