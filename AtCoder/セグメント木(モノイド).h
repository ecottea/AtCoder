#pragma once
#include "header.h"
// ■■■■■ [遅延評価]セグメント木（抽象代数上） ■■■■■


//【セグメント木】
/*
* Segtree<T>(n) : O(n)
*	v[0..n) = e で初期化する．
*	要素はモノイド T = (S, op, e) の元とする．
*
* Segtree<T>(a) : O(n)
*	配列 a の要素で初期化する．
*
* set(i, x) : O(log n)
*	v[i] = x とする．
*
* get(i) : O(1)
*	v[i] を返す．
*
* prod(l, r) : O(log n)
*	Πv[l..r) を返す．空なら e を返す．
*
* max_right<f>(l) : O(log n)
*	f( Πv[l..r) ) = true となる最大の r を返す．
*   f : S → bool で f(e) = true かつ単調とする．
*
* min_left<f>(r) : O(log n)
*	f( Πv[l..r) ) = true となる最小の l を返す．
*/
template <class T> struct Segtree {
	// 参考：https://algo-logic.info/segment-tree/

	int n; // 完全二分木の葉の数（必ず 2 冪）
	int actual_n; // 実際の要素数

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，子は v[2 * i], v[2 * i + 1]．
	// 0-indexed での i 番目のデータは葉である v[i + n] に入っている．
	// v[0] は使用しない．
	vector<T> v;

	// コンストラクタ（初期化なし）
	Segtree() : n(0), actual_n(0) {}

	// コンストラクタ（e() で初期化）
	Segtree(int n_) : actual_n(n_) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_) pow2 *= 2;
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<T>(2 * n);
	}

	// コンストラクタ（配列で初期化）
	Segtree(vector<T>& v_) : Segtree(sz(v_)) {
		// verify : https://judge.yosupo.jp/problem/point_set_range_composite

		// 全ての葉にデータを設定する．
		rep(i, sz(v_)) v[i + n] = v_[i];

		// 全てのノードに正しい値を設定する．
		repir(i, n - 1, 1) v[i] = v[i * 2] * v[i * 2 + 1];
	}

	// v[i] = x とする．
	void set(int i, T x) {
		// verify : https://judge.yosupo.jp/problem/point_set_range_composite

		// 実際にデータを格納すべき葉の位置へ
		i += n;

		// 葉のデータを更新
		v[i] = x;

		// 親のデータも更新しておく
		while (i > 1) {
			i /= 2;
			v[i] = v[i * 2] * v[i * 2 + 1];
		}
	}

	// v[i] を返す．
	T get(int i) const { return v[i + n]; }

	// Πv[l..r) を返す．空なら e を返す．
	T prod(int l, int r) const {
		// verify : https://judge.yosupo.jp/problem/point_set_range_composite

		return prod_rf(l, r, 1, 0, n);
	}

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	T prod_rf(int l, int r, int k, int kl, int kr) const {
		// 範囲外なら単位元 e を返す．
		if (kr <= l || r <= kl) return T();

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) return v[k];

		// 一部の範囲のみを含むなら子を見に行く．
		T vl = prod_rf(l, r, k * 2, kl, (kl + kr) / 2);
		T vr = prod_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return vl * vr;
	}

	// Πv[0..n) を返す．
	T all_prod() const { return prod_rf(0, n, 1, 0, n); }

	// f( Πv[l, r) ) = true となる最大の r を返す．
	int max_right(int l, const function<bool(T)>& f) const {
		T x;
		return max_right_rf(l, actual_n, x, 1, 0, n, f);
	}

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	int max_right_rf(int l, int r, T& x, int k, int kl, int kr, const function<bool(T)>& f) const {
		// 範囲外の場合
		if (kr <= l || r <= kl) return r;

		// f( Πv[kl, kr) ) = true の場合
		if (f(x * v[k])) {
			x = x * v[k];
			return r;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) return k - n;

		// まず左の部分木を見に行き，見つかったならそれを返す．
		int pos = max_right_rf(l, r, x, k * 2, kl, (kl + kr) / 2, f);
		if (pos != r) return pos;

		// 見つからなかったなら右の部分木も見にいき，結果を返す．
		return max_right_rf(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, f);
	}

	// f( Πv[l, r) ) = true となる最小の l を返す．
	int min_left(int r, const function<bool(T)>& f) const {
		T x;
		return min_left_rf(0, r, x, 1, 0, n, f) + 1;
	}

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	int min_left_rf(int l, int r, T& x, int k, int kl, int kr, const function<bool(T)>& f) const {
		// 範囲外の場合
		if (kr <= l || r <= kl) return l - 1;

		// f( Πv[kl, kr) ) = true の場合
		if (f(v[k] * x)) {
			x = v[k] * x;
			return l - 1;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) return k - n;

		// まず右の部分木を見に行き，見つかったならそれを返す．
		int pos = min_left_rf(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, f);
		if (pos != l - 1) return pos;

		// 見つからなかったなら左の部分木も見にいき，結果を返す．
		return min_left_rf(l, r, x, k * 2, kl, (kl + kr) / 2, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Segtree seg) {
		rep(i, seg.actual_n) os << seg.get(i) << " ";
		return os;
	}
#endif
};


//【遅延評価セグメント木】
/*
* Lazy_segtree<T>(n) : O(n)
*	v[0..n) = e で初期化する．
*	要素はモノイド左作用付きモノイド T = (S, op, e, F, mapping, composition, id) の元とする．
*
* Lazy_segtree<T>(a) : O(n)
*	配列 a の要素で初期化する．
*
* set(i, x) : O(log n)
*	v[i] = x とする．
*
* get(i) : O(log n)
*	v[i] を返す．
*
* prod(l, r) : O(log n)
*	Πv[l..r) を返す．空なら e を返す．
*
* apply(i, f) : O(log n)
*	v[i] = f( v[i] ) とする．
*
* apply(l, r, f) : O(log n)
*	v[l..r) = f( v[l..r) ) とする．
*
* max_right<g>(l) : O(log n)
*	g( Πv[l..r) ) = true となる最大の r を返す．
*   g : S → bool で g(e) = true かつ単調とする．
*
* min_left<g>(r) : O(log n)
*	g( Πv[l..r) ) = true となる最小の l を返す．
*/
template <class T> struct Lazy_segtree {
	// 参考：https://algo-logic.info/segment-tree/

	// 完全二分木の葉の数（必ず 2 冪）
	int n;
	int actual_n; // 実際の要素数

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，左右の子は v[2 * i], v[2 * i + 1] である．
	// 0-indexed での i 番目のデータは，葉である v[i + n] に入っている．
	// v[0] は使用しない．
	vector<typename T::S> v;

	// 遅延評価用の完全二分木
	vector<typename T::F> lazy;

	// コンストラクタ（初期化なし）
	Lazy_segtree() : n(0), actual_n(0) {}

	// コンストラクタ（単位元で初期化）
	Lazy_segtree(int n_) : actual_n(n_) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_) pow2 *= 2;
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<typename T::S>(2 * n);
		lazy = vector<typename T::F>(2 * n);
	}

	// コンストラクタ（配列で初期化）
	Lazy_segtree(vector<typename T::S>& v_) : Lazy_segtree(sz(v_)) {
		// verify : https://judge.yosupo.jp/problem/range_affine_range_sum

		// 全ての葉にデータを設定する．
		rep(i, sz(v_)) v[i + n] = v_[i];

		// 全てのノードに正しい値を設定する．
		repir(i, n - 1, 1) v[i] = v[i * 2] * v[i * 2 + 1];
	}

	// 遅延させていた評価を行う．：O(1)
	void eval(int k) {
		// 遅延させていた評価がなければ何もしない．
		if (lazy[k] == T::id()) return;

		// 葉でなければ子に伝搬する．
		if (k < n) {
			// 左作用を考えているのでこの向きに合成する．
			lazy[k * 2] = lazy[k] * lazy[k * 2];
			lazy[k * 2 + 1] = lazy[k] * lazy[k * 2 + 1];
		}

		// 自身を評価する．
		v[k] = lazy[k] * v[k];
		lazy[k] = T::id();
	}

	// v[i] = x とする．
	void set(int i, typename T::S x) { set_rf(i, x, 1, 0, n); }

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	void set_rf(int i, typename T::S x, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら何もしない．
		if (kr <= i || i < kl) return;

		// 葉まで降りてきたら値を代入して帰る．
		if (kl == i && kr == i + 1) {
			v[k] = x;
			return;
		}

		// 左右の子を見に行く．
		set_rf(i, x, k * 2, kl, (kl + kr) / 2);
		set_rf(i, x, k * 2 + 1, (kl + kr) / 2, kr);
		v[k] = v[k * 2] * v[k * 2 + 1];
	}

	// v[i] を返す．
	typename T::S get(int i) { return prod(i, i + 1); }

	// op( v[l..r) ) を返す．空なら e() を返す．
	typename T::S prod(int l, int r) {
		// verify : https://judge.yosupo.jp/problem/range_affine_range_sum

		return prod_rf(l, r, 1, 0, n);
	}

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	typename T::S prod_rf(int l, int r, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら単位元 e() を返す．
		if (kr <= l || r <= kl) return T::e();

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) return v[k];

		// 一部の範囲のみを含むなら子を見に行く．
		typename T::S v_l = prod_rf(l, r, k * 2, kl, (kl + kr) / 2);
		typename T::S v_r = prod_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return v_l * v_r;
	}

	// v[i] = f( v[i] ) とする．
	void apply(int i, typename T::F f) { apply(i, i + 1, f); }

	// v[l..r) = f( v[l..r) ) とする．
	void apply(int l, int r, typename T::F f) {
		// verify : https://judge.yosupo.jp/problem/range_affine_range_sum
	
		apply_rf(l, r, f, 1, 0, n);
	}

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	void apply_rf(int l, int r, typename T::F f, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら何もしない．
		if (kr <= l || r <= kl) return;

		// 完全に範囲内なら自身の値を更新する．
		if (l <= kl && kr <= r) {
			// 左作用を考えているのでこの向きに合成する．
			lazy[k] = f * lazy[k];
			eval(k);

			return;
		}

		// 一部の範囲のみを含むなら子を見に行く．
		apply_rf(l, r, f, k * 2, kl, (kl + kr) / 2);
		apply_rf(l, r, f, k * 2 + 1, (kl + kr) / 2, kr);
		v[k] = v[k * 2] * v[k * 2 + 1];
	}

	// g( op( v[l, r) ) ) = true となる最大の r を返す．
	int max_right(int l, const function<bool(typename T::S)>& g) {
		typename T::S x;
		return max_right_rf(l, actual_n, x, 1, 0, n, g);
	}

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	int max_right_rf(int l, int r, typename T::S& x,
		int k, int kl, int kr, const function<bool(typename T::S)>& g) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外の場合
		if (kr <= l || r <= kl) return r;

		// g( op( v[kl, kr) ) ) = true の場合
		if (g(x * v[k])) {
			x = x * v[k];
			return r;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) return k - n;

		// まず左の部分木を見に行き，見つかったならそれを返す．
		int pos = max_right_rf(l, r, x, k * 2, kl, (kl + kr) / 2, g);
		if (pos != r) return pos;

		// 見つからなかったなら右の部分木も見にいき，結果を返す．
		return max_right_rf(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, g);
	}

	// g( op( v[l, r) ) ) = true となる最小の l を返す．
	int min_left(int r, const function<bool(typename T::S)>& g) {
		typename T::S x;
		return min_left_rf(0, r, x, 1, 0, n, g) + 1;
	}

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	int min_left_rf(int l, int r, typename T::S& x,
		int k, int kl, int kr, const function<bool(typename T::S)>& g) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外の場合
		if (kr <= l || r <= kl) return l - 1;

		// g( op( v[kl, kr) ) ) = true の場合
		if (g(v[k] * x)) {
			x = v[k] * x;
			return l - 1;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) return k - n;

		// まず右の部分木を見に行き，見つかったならそれを返す．
		int pos = min_left_rf(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, g);
		if (pos != l - 1) return pos;

		// 見つからなかったなら左の部分木も見にいき，結果を返す．
		return min_left_rf(l, r, x, k * 2, kl, (kl + kr) / 2, g);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Lazy_segtree seg) {
		rep(i, seg.actual_n) os << seg.get(i) << " ";
		return os;
	}
#endif
};


