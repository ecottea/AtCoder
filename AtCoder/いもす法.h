#pragma once
#include "header.h"
// ■■■■■ いもす法 ■■■■■


//【いもす法】
/*
* [0, n) 内の半開区間に一定の値を加算する．
*
* Imos(n) : O(n)
*	半開区間 [0, n) を 0 で初期化する．
*
* set(l, r, val) : O(1)
*	半開区間 [l, r) に val を加算する準備を行う．
*
* sum() : O(n)
*	実際の加算を行う．
*
* v[i] : O(1)
*	加算後の位置 i の値を得る．
*/
template <class T> struct Imos {
	// 参考：https://imoz.jp/algorithms/imos_method.html

	int n;
	vector<T> v;

	// [0, n) 上の a を 0 で初期化する．
	Imos(int n_) : n(n_), v(n + 1)) {}

	// アクセス
	T const& operator[](int i) const { return v[i]; }
	T& operator[](int i) { return v[i]; }
	
	// 半開区間 [l, r) に val を加算する準備を行う．O(1)
	void set(int l, int r, T val) {
		v[l] += val;
		v[r] -= val;
	}

	// 実際の加算を行う．O(n)
	vector<T>& sum() {
		rep(i, n) {
			v[i + 1] += v[i];
		}

		// 不要な部分の削除
		v.pop_back();

		return v;
	}
};


//【二次元いもす法（長方形）】
/*
* [0, h) * [0, w) 内の長方形領域に一定の値を加算する．
*
* Imos2d(h, w) : O(h w)
*	[0, h) * [0, w) を 0 で初期化する．
*
* set_rect(x1, y1, x2, y2, val) : O(1)
*	[x1, x2] * [y1, y2] に val を加算する準備を行う．
*
* sum() : O(h w)
*	実際に加算を行う．
*
* v[i][j] : O(1)
*	加算後の位置 (i, j) の値を得る．
*/
template <class T> struct Imos2d {
	// 参考：https://imoz.jp/algorithms/imos_method.html

	int h, w;
	vector<vector<T>> v;

	// [0, h) * [0, w) を 0 で初期化する．
	Imos2d(int h_, int w_) : h(h_), w(w_), v(h + 1, vector<T>(w + 1)) {}

	// アクセス
	vector<T> const& operator[](int i) const { return v[i]; }
	vector<T>& operator[](int i) { return v[i]; }

	// [x1, x2) * [y1, y2) に val を加算する準備を行う．O(1)
	void set(int x1, int y1, int x2, int y2, T val) {
		v[x1][y1] += val;
		v[x1][y2] -= val;
		v[x2][y1] -= val;
		v[x2][y2] += val;
	}

	// 実際の加算を行う．O(h w)
	void sum() {
		repi(i, 1, h) {
			repi(j, 0, w) {
				v[i][j] += v[i - 1][j];
			}
		}
		repi(i, 0, h) {
			repi(j, 1, w) {
				v[i][j] += v[i][j - 1];
			}
		}
		dumpel(v);

		// 不要な部分の削除
		v.pop_back();
		rep(i, h) {
			v[i].pop_back();
		}
		dumpel(v);
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Imos2d& imos) {
		rep(i, sz(imos.v)) {
			rep(j, sz(imos.v[0])) os << imos[i][j] << " ";
			os << endl;
		}
		return os;
	}
};


//【二次元いもす法（長方形，三角形）】
/*
* [0, h) * [0, w) 内の長方形または三角形領域に一定の値を加算する．
*
* Imos2d(h, w) : O(h w)
*	[0, h) * [0, w) を 0 で初期化する．
*
* set_rect(x1, y1, x2, y2, val) : O(1)
*	[x1, x2] * [y1, y2] に val を加算する準備を行う．
*
* set_tri(x, y, d, val) : O(1)
*	[x, y] * [x + d, y + d] の対角線以下に val を加算する準備を行う．
*
* sum() : O(h w)
*	実際に加算を行う．
*
* v[i][j] : O(1)
*	加算後の位置 (i, j) の値を得る．
*/
template <class T> struct Imos2d_tri {
	// 参考：https://imoz.jp/algorithms/imos_method.html
	
	int h;
	int w;
	vector<vector<T>> v;

	// [0, h) * [0, w) を 0 で初期化する．
	Imos2d_tri(int h_, int w_) : h(h_), w(w_), v(h + 2, vector<T>(w + 2)) {}
	
	// アクセス
	vector<T> const& operator[](int i) const { return v[i]; }
	vector<T>& operator[](int i) { return v[i]; }

	// [x1, x2] * [y1, y2] に val を加算する準備を行う．O(1)
	void set_rect(int x1, int y1, int x2, int y2, T val) {
		// 左上
		v[x1][y1] += val;
		v[x1 + 1][y1 + 1] -= val;

		// 左下
		v[x2 + 2][y1 + 1] += val;
		v[x2 + 1][y1] -= val;

		// 右上
		v[x1 + 1][y2 + 2] += val;
		v[x1][y2 + 1] -= val;

		// 右下
		v[x2 + 1][y2 + 1] += val;
		v[x2 + 2][y2 + 2] -= val;
	}

	// [x, y] * [x + d, y + d] の対角線以下に val を加算する準備を行う．O(1)
	void set_tri(int x, int y, int d, T val) {
		// 左上
		v[x][y] += val;
		v[x][y + 1] -= val;

		// 左下
		v[x + d + 2][y + 1] += val;
		v[x + d + 1][y] -= val;

		// 右下
		v[x + d + 1][y + d + 2] += val;
		v[x + d + 2][y + d + 2] -= val;
	}

	// 実際の加算を行う．O(h w)
	vector<vector<T>>& sum() {
		// 縦方向の累積和
		repi(i, 1, h) {
			repi(j, 0, w) {
				v[i][j] += v[i - 1][j];
			}
		}

		// 横方向の累積和
		repi(i, 0, h) {
			repi(j, 1, w) {
				v[i][j] += v[i][j - 1];
			}
		}

		// 右下がり方向の累積和
		repi(i, 1, h) {
			repi(j, 1, w) {
				v[i][j] += v[i - 1][j - 1];
			}
		}

		// 不要な部分の削除
		v.pop_back();
		v.pop_back();
		rep(i, h) {
			v[i].pop_back();
			v[i].pop_back();
		}

		return v;
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Imos2d& imos) {
		rep(i, sz(imos.v)) {
			rep(j, sz(imos.v[0])) os << imos[i][j] << " ";
			os << endl;
		}
		return os;
	}
};


//【木上のいもす法】
/*
* r を根とする根付き木 rt の頂点の先祖や子孫に一定の値を加算する．
*
* Imos_tree(rt, r) : O(|V|)
*	r を根とする根付き木 rt を 0 で初期化する．
*
* set_anc(v, val) : O(1)
*	頂点 v とその先祖に val を加算する準備を行う．
*
* set_dsc(v, val) : O(1)
*	頂点 v とその子孫に val を加算する準備を行う．
*
* sum() : O(|V|)
*	実際の加算を行う．
*
* get(v) : O(1)
*	加算後の頂点 v の値を得る．
*/
template <class T> struct Imos_tree {
	RTree rt;
	vector<T> v_anc, v_dsc;

	// 根付き木 rt を 0 で初期化する．
	Imos_tree(const RTree& rt_) : rt(rt_), v_anc(rt_.n), v_dsc(rt_.n) {}


	// 頂点 v とその先祖に val を加算する準備を行う．
	void set_anc(int v, T val) {
		v_anc[v] += val;
	}

	// 頂点 v とその子孫に val を加算する準備を行う．
	void set_dsc(int v, T val) {
		v_dsc[v] += val;
	}

	// 実際の加算を行う．
	void sum() {
		sum_sub(rt.r, 0);
	}
	// 再帰用の関数
	T sum_sub(int s, T val) {
		v_dsc[s] += val;
		for (auto t : rt.v[s].child) {
			v_anc[s] += sum_sub(t, v_dsc[s]);
		}
		return v_anc[s];
	};

	// 加算後の頂点 v の値を得る．
	T get(int v) {
		return v_dsc[v] + v_anc[v];
	}
};


