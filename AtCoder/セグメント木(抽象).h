#pragma once
#include "header.h"
// ■■■■■ [遅延評価]セグメント木（抽象代数上） ■■■■■


//【セグメント木（モノイド）】
/*
* Segtree<S, op, e>(int n) : O(n)
*	v[0..n) = e() で初期化する．
*	要素はモノイド (S, op, e) の元とする．
*
* Segtree<S, op, e>(vS v) : O(n)
*	配列 v[0..n) の要素で初期化する．
*
* set(int i, S x) : O(log n)
*	v[i] = x とする．
*
* S get(int i) : O(1)
*	v[i] を返す．
*
* S prod(int l, int r) : O(log n)
*	op( v[l..r) ) を返す．空なら e() を返す．
*
* S all_prod() : O(1)
*	op( v[0..n) ) を返す．
*
* int max_right(int l, function<bool(S)> f) : O(log n)
*	f( op( v[l..r) ) ) = true となる最大の r を返す．
*   制約：f(e()) = true，f は単調
*
* int min_left(int r, function<bool(S)> f) : O(log n)
*	f( op( v[l..r) ) ) = true となる最小の l を返す．
*	制約：f(e()) = true，f は単調
*/
template <class S, S(*op)(S, S), S(*e)()>
struct Segtree {
	// 参考：https://algo-logic.info/segment-tree/

	// 完全二分木の葉の数（必ず 2 冪）
	int n;
	int actual_n; // 実際の要素数

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，子は v[2 * i], v[2 * i + 1]．
	// 0-indexed での i 番目のデータは葉である v[i + n] に入っている．
	// v[0] は使用しない．
	vector<S> v;

	// コンストラクタ（初期化なし）
	Segtree() : n(0), actual_n(0) {}

	// コンストラクタ（e() で初期化）
	Segtree(int n_) : actual_n(n_) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_) {
			pow2 *= 2;
		}
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<S>(2 * n, e());
	}

	// コンストラクタ（配列で初期化）
	Segtree(vector<S>& v_) : Segtree(sz(v_)) {
		// verify : https://judge.yosupo.jp/problem/point_set_range_composite

		// 全ての葉にデータを設定する．
		rep(i, sz(v_)) {
			v[i + n] = v_[i];
		}

		// 全てのノードに正しい値を設定する．
		repir(i, n - 1, 1) {
			v[i] = op(v[i * 2], v[i * 2 + 1]);
		}
	}

	// v[i] = x とする．
	void set(int i, S x) {
		// verify : https://judge.yosupo.jp/problem/point_set_range_composite

		// 実際にデータを格納すべき葉の位置へ
		i += n;

		// 葉のデータを更新
		v[i] = x;

		// 親のデータも更新しておく
		while (i > 1) {
			i /= 2;
			v[i] = op(v[i * 2], v[i * 2 + 1]);
		}
	}

	// v[i] を返す．
	S get(int i) const { return v[i + n]; }

	// op( v[l..r) ) を返す．空なら e() を返す．
	S prod(int l, int r) const {
		// verify : https://judge.yosupo.jp/problem/point_set_range_composite

		return prod_rf(l, r, 1, 0, n);
	}

	// k : 注目ノード，[kl.kr) : ノード v[k] が表す区間
	S prod_rf(int l, int r, int k, int kl, int kr) const {
		// 範囲外なら単位元 e() を返す．
		if (kr <= l || r <= kl) {
			return e();
		}

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) {
			return v[k];
		}

		// 一部の範囲のみを含むなら子を見に行く．
		S vl = prod_rf(l, r, k * 2, kl, (kl + kr) / 2);
		S vr = prod_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return op(vl, vr);
	}

	// op( v[0..n) ) を返す．
	S all_prod() const { return prod_rf(0, n, 1, 0, n); }

	// f( op( v[l..r) ) ) = true となる最大の r を返す．
	int max_right(int l, const function<bool(S)>& f) const {
		S x = e();
		return max_right_rf(l, actual_n, x, 1, 0, n, f);
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	int max_right_rf(int l, int r, S& x, int k, int kl, int kr, const function<bool(S)>& f) const {
		// 範囲外の場合
		if (kr <= l || r <= kl) {
			return r;
		}

		// f( op( v[kl, kr) ) ) = true の場合
		if (f(op(x, v[k]))) {
			x = op(x, v[k]);
			return r;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) {
			return k - n;
		}

		// まず左の部分木を見に行き，見つかったならそれを返す．
		int pos = max_right_rf(l, r, x, k * 2, kl, (kl + kr) / 2, f);
		if (pos != r) {
			return pos;
		}

		// 見つからなかったなら右の部分木も見にいき，結果を返す．
		return max_right_rf(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, f);
	}

	// f( op( v[l..r) ) ) = true となる最小の l を返す．
	int min_left(int r, const function<bool(S)>& f) const {
		S x = e();
		return min_left_rf(0, r, x, 1, 0, n, f) + 1;
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	int min_left_rf(int l, int r, S& x, int k, int kl, int kr, const function<bool(S)>& f) const {
		// 範囲外の場合
		if (kr <= l || r <= kl) {
			return l - 1;
		}

		// f( op( v[kl, kr) ) ) = true の場合
		if (f(op(v[k], x))) {
			x = op(v[k], x);
			return l - 1;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) {
			return k - n;
		}

		// まず右の部分木を見に行き，見つかったならそれを返す．
		int pos = min_left_rf(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, f);
		if (pos != l - 1) {
			return pos;
		}

		// 見つからなかったなら左の部分木も見にいき，結果を返す．
		return min_left_rf(l, r, x, k * 2, kl, (kl + kr) / 2, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Segtree seg) {
		rep(i, seg.actual_n) {
			os << seg.get(i) << " ";
		}
		return os;
	}
#endif
};


//【遅延評価セグメント木（M-モノイド）】
/*
* Lazy_segtree<S, op, e, F, act, comp, id>(int n) : O(n)
*	v[0..n) = e() で初期化する．
*	要素は左作用付きモノイド (S, op, e, F, act, comp, id) の元とする．
*
* Lazy_segtree<S, op, e, F, act, comp, id>(vS v) : O(n)
*	配列 v[0..n) の要素で初期化する．
*
* set(int i, S x) : O(log n)
*	v[i] = x とする．
*
* S get(int i) : O(log n)
*	v[i] を返す．
*
* S prod(int l, int r) : O(log n)
*	Πv[l..r) を返す．空なら e() を返す．
*
* apply(int i, F f) : O(log n)
*	v[i] = f( v[i] ) とする．
*
* apply(int l, int r, F f) : O(log n)
*	v[l..r) = f( v[l..r) ) とする．
*
* int max_right(int l, function<bool(S)> g) : O(log n)
*	g( Πv[l..r) ) = true となる最大の r を返す．
*   制約：g( e() ) = true かつ g は単調
*
* int min_left(int r, function<bool(S)> g) : O(log n)
*	g( Πv[l..r) ) = true となる最小の l を返す．
*	制約：g( e() ) = true かつ g は単調
*/
template <class S, S(*op)(S, S), S(*e)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Lazy_segtree {
	// 参考：https://algo-logic.info/segment-tree/

	int n; // 完全二分木の葉の数（必ず 2 冪）
	int actual_n; // 実際の要素数

	// 完全二分木を実現する大きさ 2 * n の配列（ v[0] は使用しない．）
	// 根は v[1] で，v[i] の親は v[i / 2]，左右の子は v[2 * i], v[2 * i + 1] である．
	// 0-indexed での i 番目のデータは，葉である v[i + n] に入っている．
	vector<S> v;

	// 遅延評価用の完全二分木
	vector<F> lazy;

	// 遅延させていた評価を行う．：O(1)
	void eval(int k) {
		// 遅延させていた評価がなければ何もしない．
		if (lazy[k] == id()) return;

		// 葉でなければ子に伝搬する．
		if (k < n) {
			// 左作用を考えているのでこの向きに合成する．
			lazy[k * 2] = comp(lazy[k], lazy[k * 2]);
			lazy[k * 2 + 1] = comp(lazy[k], lazy[k * 2 + 1]);
		}

		// 自身を評価する．
		v[k] = act(lazy[k], v[k]);
		lazy[k] = id();
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	void set_sub(int i, S x, int k, int kl, int kr) {
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
		set_sub(i, x, k * 2, kl, (kl + kr) / 2);
		set_sub(i, x, k * 2 + 1, (kl + kr) / 2, kr);
		v[k] = op(v[k * 2], v[k * 2 + 1]);
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	S prod_sub(int l, int r, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら単位元 e() を返す．
		if (kr <= l || r <= kl) return e();

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) return v[k];

		// 一部の範囲のみを含むなら子を見に行く．
		S vl = prod_sub(l, r, k * 2, kl, (kl + kr) / 2);
		S vr = prod_sub(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return op(vl, vr);
	}

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	void apply_sub(int l, int r, F f, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら何もしない．
		if (kr <= l || r <= kl) return;

		// 完全に範囲内なら自身の値を更新する．
		if (l <= kl && kr <= r) {
			// 左作用を考えているのでこの向きに合成する．
			lazy[k] = comp(f, lazy[k]);

			// return 直後に親から v[k] を参照される可能性があるので eval() が必要．
			eval(k);

			return;
		}

		// 一部の範囲のみを含むなら子を見に行く．
		apply_sub(l, r, f, k * 2, kl, (kl + kr) / 2);
		apply_sub(l, r, f, k * 2 + 1, (kl + kr) / 2, kr);
		v[k] = op(v[k * 2], v[k * 2 + 1]);
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	int max_right_sub(int l, int r, S& x, int k, int kl, int kr, const function<bool(S)>& g) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外の場合
		if (kr <= l || r <= kl) return r;

		// g( op( v[l, kr) ) ) = true の場合
		if (g(op(x, v[k]))) {
			x = op(x, v[k]);
			return r;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) return k - n;

		// まず左の部分木を見に行き，見つかったならそれを返す．
		int pos = max_right_sub(l, r, x, k * 2, kl, (kl + kr) / 2, g);
		if (pos != r) return pos;

		// 見つからなかったなら右の部分木も見にいき，結果を返す．
		return max_right_sub(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, g);
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	int min_left_sub(int l, int r, S& x, int k, int kl, int kr, const function<bool(S)>& g) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外の場合
		if (kr <= l || r <= kl) return l - 1;

		// g( op( v[kl, r) ) ) = true の場合
		if (g(op(v[k], x))) {
			x = op(v[k], x);
			return l - 1;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) return k - n;

		// まず右の部分木を見に行き，見つかったならそれを返す．
		int pos = min_left_sub(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, g);
		if (pos != l - 1) return pos;

		// 見つからなかったなら左の部分木も見にいき，結果を返す．
		return min_left_sub(l, r, x, k * 2, kl, (kl + kr) / 2, g);
	}

public:
	// v[0..n) = e() で初期化する．
	Lazy_segtree(int n_) : actual_n(n_) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_) pow2 *= 2;
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<S>(2 * n, e());
		lazy = vector<F>(2 * n, id());
	}

	// 配列 v[0..n) の要素で初期化する．
	Lazy_segtree(vector<S>& v_) : Lazy_segtree(sz(v_)) {
		// verify : https://judge.yosupo.jp/problem/range_affine_range_sum

		// 全ての葉にデータを設定する．
		rep(i, sz(v_)) v[i + n] = v_[i];

		// 全てのノードに正しい値を設定する．
		repir(i, n - 1, 1) v[i] = op(v[i * 2], v[i * 2 + 1]);
	}

	Lazy_segtree() : n(0), actual_n(0) {} // ダミー

	// v[i] = x とする．
	void set(int i, S x) {
		set_sub(i, x, 1, 0, n);
	}

	// v[i] を返す．
	S get(int i) {
		return prod(i, i + 1);
	}

	// op( v[l..r) ) を返す．空なら e() を返す．
	S prod(int l, int r) {
		// verify : https://judge.yosupo.jp/problem/range_affine_range_sum

		return prod_sub(l, r, 1, 0, n);
	}

	// v[i] = f( v[i] ) とする．
	void apply(int i, F f) {
		apply(i, i + 1, f);
	}

	// v[l..r) = f( v[l..r) ) とする．
	void apply(int l, int r, F f) {
		// verify : https://judge.yosupo.jp/problem/range_affine_range_sum

		apply_sub(l, r, f, 1, 0, n);
	}

	// g( op( v[l..r) ) ) = true となる最大の r を返す．
	int max_right(int l, const function<bool(S)>& g) {
		S x = e();
		return max_right_sub(l, actual_n, x, 1, 0, n, g);
	}

	// g( op( v[l..r) ) ) = true となる最小の l を返す．
	int min_left(int r, const function<bool(S)>& g) {
		S x = e();
		return min_left_sub(0, r, x, 1, 0, n, g) + 1;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Lazy_segtree seg) {
		rep(i, seg.actual_n) os << seg.get(i) << " ";
		return os;
	}
#endif
};


//【間引きセグメント木（モノイド）】
/*
* Thinning_segtree<S, op, e>(int n, int m) : O(n)
*	v[0..n) = e() と法 m で初期化する．
*	要素はモノイド (S, op, e) の元とする．
*
* Thinning_segtree<S, op, e>(vS v) : O(n)
*	配列 v[0..n) と法 m で初期化する．
*
* set(int i, S x) : O(log n)
*	v[i] = x とする．
*
* S get(int i) : O(1)
*	v[i] を返す．
*
* S prod(int l, int r, int k) : O(log n)
*	set = {i∈[l..r) | i=k (mod m)} とし，Πv[set] を返す．空なら e() を返す．
*
* S all_prod(int k) : O(1)
*	set = {i∈[0..n) | i=k (mod m)} とし，Πv[set] を返す．空なら e() を返す．
*
* int max_right(int l, function<bool(S)> f, int k) : O(log n)
*	set = {i∈[l..r) | i=k (mod m)} とし，f( Πv[set] ) = true となる最大の r(=k (mod m)) を返す．
*   制約：f(e()) = true，f は単調
*
* int min_left(int r, function<bool(S)> f, int k) : O(log n)
*	set = {i∈[l..r) | i=k (mod m)} とし，f( Πv[set] ) = true となる最小の l(=k (mod m)) を返す．
*	制約：f(e()) = true，f は単調
*/
template <class S, S(*op)(S, S), S(*e)()>
class Thinning_segtree {
	int n, m;
	vector<segtree<S, op, e>> segs;

public:
	// v[0..n) = e() と法 m で初期化する．
	Thinning_segtree(int n_, int m_) : n(n_), m(m_), segs(m) {
		// verify : https://atcoder.jp/contests/arc092/tasks/arc092_c

		rep(j, m) segs[j] = segtree<S, op, e>((n + m - 1 - j) / m);
	}

	// 配列 v[0..n) と法 m で初期化する．
	Thinning_segtree(vector<S>& v, int m_) : n(sz(v)), m(m_), segs(m) {
		vector<vector<S>> v2(m);
		rep(i, n) v2[i % m].push_back(v[i]);
		rep(j, m) segs[j] = segtree<S, op, e>(v2[j]);
	}

	// v[i] = x とする．
	void set(int i, S x) {
		// verify : https://atcoder.jp/contests/arc092/tasks/arc092_c

		segs[i % m].set(i / m, x);
	}

	// v[i] を返す．
	S get(int i) {
		// verify : https://atcoder.jp/contests/arc092/tasks/arc092_c

		return segs[i % m].get(i / m);
	}

	// set = {i∈[l..r) | i=k (mod m)} とし，op(v[set]) を返す．空なら e() を返す．
	S prod(int l, int r, int k) {
		// verify : https://atcoder.jp/contests/arc092/tasks/arc092_c

		return segs[k % m].prod((l - k + m - 1) / m, (r - k + m - 1) / m);
	}

	// set = {i∈[0..n) | i=k (mod m)} とし，op(v[set]) を返す．空なら e() を返す．
	S all_prod(int k) {
		return prod(0, n, k);
	}

	// set = {i∈[l..r) | i=k (mod m)} とし，f( op(v[set]) ) = true となる最大の r(=k (mod m)) を返す．
	int max_right(int l, const function<bool(S)>& f, int k) {
		k %= m;
		return segs[k].max_right((l - k + m - 1) / m, f) * m + k;
	}

	// set = {i∈[l..r) | i=k (mod m)} とし，f( op(v[set]) ) = true となる最小の l(=k (mod m)) を返す．
	int min_left(int r, const function<bool(S)>& f, int k) {
		k %= m;
		return segs[k].min_left((r - k + m - 1) / m, f) * m + k;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Thinning_segtree segm) {
		rep(i, segm.n) os << segm.get(i) << " ";
		return os;
	}
#endif
};


//【動的セグメント木（モノイド）】
/*
* Dynamic_segtree<T, S, op, e>(T n) : O(1)
*	a[0..n) = e() で初期化する．
*	位置の型は T，要素はモノイド (S, op, e) の元とする．
*
* set(T i, S x) : O(log n)
*	a[i] = x とする．
*
* apply_left(T i, S x) : O(log n)
*	a[i] = op(x, a[i]) とする．
*
* apply_right(T i, S x) : O(log n)
*	a[i] = op(a[i], x) とする．
*
* S get(T i) : O(log n)
*	a[i] を返す（なければ e() を返す）
*
* S prod(T l, T r) : O(log n)
*	op( a[l..r) ) を返す．空なら e() を返す．
*
* S all_prod() : O(1)
*	op( a[0..n) ) を返す．
*
* T max_right(T l, function<bool(S)> f) : O(log n)
*	f( op( a[l..r) ) ) = true となる最大の r を返す．
*   制約：f( e() ) = true，f は単調
*
* T min_left(T r, function<bool(S)> f) : O(log n)
*	f( op( a[l..r) ) ) = true となる最小の l を返す．
*	制約：f( e() ) = true，f は単調
*/
template <class T, class S, S(*op)(S, S), S(*e)()>
class Dynamic_segtree {
	// 参考 : https://lorent-kyopro.hatenablog.com/entry/2021/03/12/025644

	struct Node {
		T pos; // ノードの位置
		S val; // ノードの値
		S acc; // 部分木の値
		Node* l, * r;

		Node(T pos, S val) : pos(pos), val(val), acc(val), l(nullptr), r(nullptr) {}

		// acc を正しい値にする．
		void update() {
			acc = val;
			if (l) acc = op(l->acc, acc);
			if (r) acc = op(acc, r->acc);
		}
	};

	const int SET = 0, APL = 1, APR = 2;

	T n;
	Node* root;

	// 部分木 t の位置 pos を値 val にする（部分木 t は区間 [il, ir) に対応する）
	void set(Node*& t, T il, T ir, T pos, S val, int q_type) const {
		// ノードが存在しなかった場合は新たに作成する．
		if (!t) {
			t = new Node(pos, val);
			return;
		}

		// ちょうど pos に対応するノードだった場合はそこに val を書き込む．
		if (t->pos == pos) {
			if (q_type == SET) t->val = val;
			else if (q_type = APL) t->val = op(val, t->val);
			else t->val = op(t->val, val);

			t->update();
			return;
		}

		// 区間の中央
		T im = (il + ir) / 2;

		// 区間の左側に対象位置 pos がある場合
		if (pos < im) {
			// pos < t->pos であるようにする．
			if (pos > t->pos) {
				swap(pos, t->pos);
				swap(val, t->val);
			}

			set(t->l, il, im, pos, val, q_type);
		}
		// 区間の右側に対象位置 pos がある場合
		else {
			// t->pos < pos であるようにする．
			if (t->pos > pos) {
				swap(pos, t->pos);
				swap(val, t->val);
			}

			set(t->r, im, ir, pos, val, q_type);
		}

		t->update();
	}

	S get(Node* t, T il, T ir, T pos) const {
		// ノードが存在しなかった場合は単位元を返す．
		if (!t) return e();

		// ちょうど pos に対応するノードだった場合はそこの val を返す．
		if (t->pos == pos) return t->val;

		// 区間の中央
		T im = (il + ir) / 2;

		if (pos < im) return get(t->l, il, im, pos);
		else return get(t->r, im, ir, pos);
	}

	S prod(Node* t, T il, T ir, T l, T r) const {
		// ノードが存在しなかった場合や完全に [il, ir) の範囲外になった場合は単位元を返す．
		if (!t || ir <= l || r <= il) return e();

		// 完全に [il, ir) の範囲内だった場合はそこの acc を返す．
		if (l <= il && ir <= r) return t->acc;

		// 区間の中央
		T im = (il + ir) / 2;

		S res = prod(t->l, il, im, l, r);
		if (l <= t->pos && t->pos < r) res = op(res, t->val);
		res = op(res, prod(t->r, im, ir, l, r));

		return res;
	}

	T max_right(Node* t, T il, T ir, T l, const function<bool(S)>& f, S& acc) const {
		if (!t || ir <= l) return n;
		if (f(op(acc, t->acc))) {
			acc = op(acc, t->acc);
			return n;
		}
		T im = (il + ir) / 2;
		T res = max_right(t->l, il, im, l, f, acc);
		if (res != n) return res;
		if (l <= t->pos) {
			acc = op(acc, t->val);
			if (!f(acc)) return t->pos;
		}
		return max_right(t->r, im, ir, l, f, acc);
	}

	T min_left(Node* t, T il, T ir, T r, const function<bool(S)>& f, S& acc) const {
		if (!t || r <= il) return T(0);
		if (f(op(t->acc, acc))) {
			acc = op(t->acc, acc);
			return T(0);
		}
		T im = (il + ir) / 2;
		T res = min_left(t->r, im, ir, r, f, acc);
		if (res != 0) return res;
		if (t->pos < r) {
			acc = op(t->val, acc);
			if (!f(acc)) return t->pos + T(1);
		}
		return min_left(t->l, il, im, r, f, acc);
	}

	void print(Node* t, ostream& os) const {
		if (!t) return;

		print(t->l, os);
		os << "(" << t->pos << "," << t->val << ") ";
		print(t->r, os);
	}

public:
	// a[0..n) = e() で初期化する．
	Dynamic_segtree(T n) : n(n), root(nullptr) {
		// verify : https://www.spoj.com/problems/ADAAPHID/
	}
	Dynamic_segtree() : n(T(0)), root(nullptr) {}

	// a[i] = x とする．
	void set(T i, S x) {
		// verify : https://www.spoj.com/problems/ADAAPHID/

		assert(T(0) <= i && i < n);

		set(root, T(0), n, i, x, SET);
	}

	// a[i] = op(x, a[i]) とする．
	void apply_left(T i, S x) {
		// verify : https://www.spoj.com/problems/ADAAPHID/

		assert(T(0) <= i && i < n);

		set(root, T(0), n, i, x, APL);
	}

	// a[i] = op(a[i], x) とする．
	void apply_right(T i, S x) {
		assert(T(0) <= i && i < n);

		set(root, T(0), n, i, x, APR);
	}

	// a[i] を返す．
	S get(T i) const {
		// verify : https://www.spoj.com/problems/ADAAPHID/

		assert(T(0) <= i && i < n);

		return get(root, T(0), n, i);
	}

	// op( a[l..r) ) を返す．空なら e() を返す．
	S prod(T l, T r) const {
		// verify : https://www.spoj.com/problems/ADAAPHID/

		chmax(l, T(0)); chmin(r, n);
		if (l >= r) return e();

		return prod(root, T(0), n, l, r);
	}

	// op( a[0..n) ) を返す．
	S all_prod() const {
		return root ? root->acc : e();
	}

	// f( op( a[l..r) ) ) = true となる最大の r を返す．
	T max_right(T l, const function<bool(S)>& f) const {
		// verify : https://www.spoj.com/problems/COUNT1IT/

		chmax(l, T(0));

		S acc = e();
		assert(f(acc));
		return max_right(root, T(0), n, l, f, acc);
	}

	// f( op( a[l..r) ) ) = true となる最小の l を返す．
	T min_left(T r, const function<bool(S)>& f) const {
		chmin(r, n);

		S acc = e();
		assert(f(acc));
		return min_left(root, T(0), n, r, f, acc);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Dynamic_segtree seg) {
		seg.print(seg.root, os);
		return os;
	}
#endif
};


//【Segment tree beats!（不完全 M-モノイド）】
/*
* Lazy_segtree<S, op, e, F, act, comp, id, fail>(int n) : O(n)
*	v[0..n) = e() で初期化する．
*	要素は不完全左作用付きモノイド (S, op, e, F, act, comp, id, fail) の元とする．
*
* Lazy_segtree<S, op, e, F, act, comp, id, fail>(vS v) : O(n)
*	配列 v[0..n) の要素で初期化する．
*
* set(int i, S x) : O(α log n)
*	v[i] = x とする．
*
* S get(int i) : O(α log n)
*	v[i] を返す．
*
* S prod(int l, int r) : O(α log n)
*	Πv[l..r) を返す．空なら e() を返す．
*
* apply(int i, F f) : O(α log n)
*	v[i] = f( v[i] ) とする．
*
* apply(int l, int r, F f) : O(α log n)
*	v[l..r) = f( v[l..r) ) とする．
*
* int max_right(int l, function<bool(S)> g) : O(α log n)
*	g( Πv[l..r) ) = true となる最大の r を返す．
*   制約：g( e() ) = true かつ g は単調
*
* int min_left(int r, function<bool(S)> g) : O(α log n)
*	g( Πv[l..r) ) = true となる最小の l を返す．
*	制約：g( e() ) = true かつ g は単調
*/
template <class S, S(*op)(S, S), S(*e)(),
	class F, S(*act)(F, S), F(*comp)(F, F), F(*id)(), S(*fail)()>
class Segtree_beats {
	// 参考 : https://rsm9.hatenablog.com/entry/2021/02/01/220408

	int n; // 完全二分木の葉の数（必ず 2 冪）
	int actual_n; // 実際の要素数

	// 完全二分木を実現する大きさ 2 * n の配列（ v[0] は使用しない．）
	// 根は v[1] で，v[i] の親は v[i / 2]，左右の子は v[2 * i], v[2 * i + 1] である．
	// 0-indexed での i 番目のデータは，葉である v[i + n] に入っている．
	vector<S> v;

	// 遅延評価用の完全二分木
	vector<F> lazy;

	// 遅延させていた評価を行う．：O(1)
	void eval(int k) {
		// 遅延させていた評価がなければ何もしない．
		if (lazy[k] == id()) return;

		// 葉でなければ子に伝搬する．
		if (k < n) {
			// 左作用を考えているのでこの向きに合成する．
			lazy[k * 2] = comp(lazy[k], lazy[k * 2]);
			lazy[k * 2 + 1] = comp(lazy[k], lazy[k * 2 + 1]);
		}

		// 自身を評価する．
		v[k] = act(lazy[k], v[k]);
		lazy[k] = id();

		// 評価に失敗した場合は子ノードの値から再計算する．
		if (v[k] == fail()) {
			eval(k * 2);
			eval(k * 2 + 1);
			v[k] = op(v[k * 2], v[k * 2 + 1]);
		}
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	void set_sub(int i, S x, int k, int kl, int kr) {
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
		set_sub(i, x, k * 2, kl, (kl + kr) / 2);
		set_sub(i, x, k * 2 + 1, (kl + kr) / 2, kr);
		v[k] = op(v[k * 2], v[k * 2 + 1]);
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	S prod_sub(int l, int r, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら単位元 e() を返す．
		if (kr <= l || r <= kl) return e();

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) return v[k];

		// 一部の範囲のみを含むなら子を見に行く．
		S vl = prod_sub(l, r, k * 2, kl, (kl + kr) / 2);
		S vr = prod_sub(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return op(vl, vr);
	}

	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	void apply_sub(int l, int r, F f, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら何もしない．
		if (kr <= l || r <= kl) return;

		// 完全に範囲内なら自身の値を更新する．
		if (l <= kl && kr <= r) {
			// 左作用を考えているのでこの向きに合成する．
			lazy[k] = comp(f, lazy[k]);

			// return 直後に親から v[k] を参照される可能性があるので eval() が必要．
			eval(k);

			return;
		}

		// 一部の範囲のみを含むなら子を見に行く．
		apply_sub(l, r, f, k * 2, kl, (kl + kr) / 2);
		apply_sub(l, r, f, k * 2 + 1, (kl + kr) / 2, kr);
		v[k] = op(v[k * 2], v[k * 2 + 1]);
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	int max_right_sub(int l, int r, S& x, int k, int kl, int kr, const function<bool(S)>& g) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外の場合
		if (kr <= l || r <= kl) return r;

		// g( op( v[kl, kr) ) ) = true の場合
		if (g(op(x, v[k]))) {
			x = op(x, v[k]);
			return r;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) return k - n;

		// まず左の部分木を見に行き，見つかったならそれを返す．
		int pos = max_right_sub(l, r, x, k * 2, kl, (kl + kr) / 2, g);
		if (pos != r) return pos;

		// 見つからなかったなら右の部分木も見にいき，結果を返す．
		return max_right_sub(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, g);
	}

	// k : 注目ノード，[kl..kr) : ノード v[k] が表す区間
	int min_left_sub(int l, int r, S& x, int k, int kl, int kr, const function<bool(S)>& g) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外の場合
		if (kr <= l || r <= kl) return l - 1;

		// g( op( v[kl, kr) ) ) = true の場合
		if (g(op(v[k], x))) {
			x = op(v[k], x);
			return l - 1;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) return k - n;

		// まず右の部分木を見に行き，見つかったならそれを返す．
		int pos = min_left_sub(l, r, x, k * 2 + 1, (kl + kr) / 2, kr, g);
		if (pos != l - 1) return pos;

		// 見つからなかったなら左の部分木も見にいき，結果を返す．
		return min_left_sub(l, r, x, k * 2, kl, (kl + kr) / 2, g);
	}

public:
	// v[0..n) = e() で初期化する．
	Segtree_beats(int n_) : actual_n(n_) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_) pow2 *= 2;
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<S>(2 * n, e());
		lazy = vector<F>(2 * n, id());
	}

	// 配列 v[0..n) の要素で初期化する．
	Segtree_beats(vector<S>& v_) : Segtree_beats(sz(v_)) {
		// verify : https://atcoder.jp/contests/abc256/tasks/abc256_h

		// 全ての葉にデータを設定する．
		rep(i, sz(v_)) v[i + n] = v_[i];

		// 全てのノードに正しい値を設定する．
		repir(i, n - 1, 1) v[i] = op(v[i * 2], v[i * 2 + 1]);
	}

	Segtree_beats() : n(0), actual_n(0) {} // ダミー

	// v[i] = x とする．
	void set(int i, S x) {
		set_sub(i, x, 1, 0, n);
	}

	// v[i] を返す．
	S get(int i) {
		return prod(i, i + 1);
	}

	// op( v[l..r) ) を返す．空なら e() を返す．
	S prod(int l, int r) {
		// verify : https://atcoder.jp/contests/abc256/tasks/abc256_h

		return prod_sub(l, r, 1, 0, n);
	}

	// v[i] = f( v[i] ) とする．
	void apply(int i, F f) {
		apply(i, i + 1, f);
	}

	// v[l..r) = f( v[l..r) ) とする．
	void apply(int l, int r, F f) {
		// verify : https://atcoder.jp/contests/abc256/tasks/abc256_h

		apply_sub(l, r, f, 1, 0, n);
	}

	// g( op( v[l..r) ) ) = true となる最大の r を返す．
	int max_right(int l, const function<bool(S)>& g) {
		S x = e();
		return max_right_sub(l, actual_n, x, 1, 0, n, g);
	}

	// g( op( v[l..r) ) ) = true となる最小の l を返す．
	int min_left(int r, const function<bool(S)>& g) {
		S x = e();
		return min_left_sub(0, r, x, 1, 0, n, g) + 1;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Segtree_beats seg) {
		rep(i, seg.actual_n) os << seg.get(i) << " ";
		return os;
	}
#endif
};


//【永続セグメント木（モノイド）】
/*
* 永続データ構造.h へ
*/


//【連想セグメント木（モノイド）】（遅い）
/*
* Segtree_map<T, lb, ub, S, op, e>() : O(1)
*	空のセグメント木で初期化する．
*	要素はモノイド (S, op, e) の元とする．
*	key は比較可能な型 T で (lb..ub) の範囲の値をとる．
*
* set(key, x) : O(log n)
*	v[key] = x とする．
*
* get(key) : O(log n)
*	v[key] を返す．なければ e() を返す．
*
* erase(key) : O(log n)
*	v[key] を削除する．
*
* prod(l, r) : O(log n)
*	key が [l, r] の範囲の v の要素全ての op() の結果を返す．
*
* max_right<f>(l) : O(log n)
*	f( op( v[l..r) ) ) = true となる最大の r を返す．
*   f : S → bool で f(e()) = true かつ単調とする．
*
* min_left<f>(r) : O(log n)
*	f( op( v(l..r] ) ) = true となる最小の l を返す．
*/
template <class T, T(*lb)(), T(*ub)(), class S, S(*op)(S, S), S(*e)()>
struct Segtree_map {
	// セグメント木のノード
	struct Node {
		pair<T, T> key;
		S val;
		int priority; // 優先度（大きいほど根に近い，葉は固定で -INF）
		Node* left;
		Node* right;

		// コンストラクタ
		Node() {}
		Node(const pair<T, T>& key_, const S& val_, int priority_) :
			key(key_), val(val_), priority(priority_),
			left(nullptr), right(nullptr) {}
	};

	int n; // 要素数
	Node* root; // 根へのポインタ
	mt19937 rnd; // 乱数生成器
	uniform_int_distribution<int> unirnd; // 一様乱数生成器


	// コンストラクタ（空で初期化）
	Segtree_map() : n(0), root(nullptr), rnd((int)time(0)), unirnd(-INF + 1, INF) {}

	// ノードの更新
	void update(Node& t) {
		if (t.left == nullptr) {
			return;
		}

		t.key.first = t.left->key.first;
		t.key.second = t.right->key.second;
		t.val = op(t.left->val, t.right->val);
	}

	// 木の右回転
	void rotate_right(Node*& t) {
		Node* s = t->left;
		t->left = s->right;
		s->right = t;

		update(*t);
		update(*s);
		t = s;
	}

	// 木の左回転
	void rotate_left(Node*& t) {
		Node* s = t->right;
		t->right = s->left;
		s->left = t;

		update(*t);
		update(*s);
		t = s;
	}

	// ヒープ条件を満たすよう木を葉の方向に回転していく．
	void rotate_to_leaf(Node*& t) {
		// ヒープ条件を満たしていたら何もしない．
		if (t->priority > t->left->priority && t->priority > t->right->priority) {
			return;
		}

		// 右だけ条件を満たしていない場合
		if (t->priority > t->left->priority) {
			rotate_left(t);
		}
		// 左だけ条件を満たしていない場合
		else if (t->priority > t->right->priority) {
			rotate_right(t);
		}
		// 両方とも条件を満たしていない場合
		else {
			// 左右の子のうち優先度の大きい方が上にくるよう回転して再帰を回す．
			if (t->left->priority < t->right->priority) {
				rotate_left(t);
				rotate_to_leaf(t->left);
			}
			else {
				rotate_right(t);
				rotate_to_leaf(t->right);
			}
		}
	}

	// v[key] = x とする．
	void set(const T& key, const S& x) {
		// verify : https://atcoder.jp/contests/jsc2021/tasks/jsc2021_f

		if (root == nullptr) {
			root = new Node({ key, key }, x, -INF);
			n++;
		}
		else {
			set_rf(root, key, x);
		}
	}

	void set_rf(Node*& t, const T& key, const S& x) {
		// 区間の左端より左だった場合
		if (key < t->key.first) {
			// 区間を広げた新しいノードを作り，その左の子になる．
			Node* s = t;
			t = new Node({ key, t->key.second }, op(t->val, x), unirnd(rnd));
			t->right = s;
			t->left = new Node({ key, key }, x, -INF);
			n++;

			// ヒープ条件を満たすよう木を葉の方向に回転していく．
			rotate_to_leaf(t);
			return;
		}

		// 区間の右端より右だった場合
		if (key > t->key.second) {
			// 区間を広げた新しいノードを作り，その右の子になる．
			Node* s = t;
			t = new Node({ t->key.first, key }, op(t->val, x), unirnd(rnd));
			t->left = s;
			t->right = new Node({ key, key }, x, -INF);
			n++;

			// ヒープ条件を満たすよう木を葉の方向に回転していく．
			rotate_to_leaf(t);
			return;
		}

		// 既に登録されている key なら値を更新する．
		if (t->key.first == t->key.second) {
			t->val = x;
			return;
		}

		// 左の子への移動
		if (key <= t->left->key.second) {
			set_rf(t->left, key, x);
			update(*t);
			if (t->priority < t->left->priority) {
				rotate_right(t);
			}
		}
		// 右の子への移動
		else {
			set_rf(t->right, key, x);
			update(*t);
			if (t->priority < t->right->priority) {
				rotate_left(t);
			}
		}
	}

	// v[key] を返す．なければ e() を返す．
	S get(const T& key) {
		// verify : https://atcoder.jp/contests/jsc2021/tasks/jsc2021_f

		return prod(key, key);
	}

	// v[key] を削除する．
	void erase(const T& key) {
		if (root == nullptr) return;

		erase_rf(root, key);
	}

	bool erase_rf(Node*& t, const T& key) {
		// 要素が存在しなかった場合
		if (key < t->key.first || t->key.second < key) {
			// 何もしない
			return false;
		}

		// 登録されている key なら削除する．
		if (t->key.first == t->key.second) {
			delete t;
			t = nullptr;
			n--;

			return true;
		}

		// 左の子への移動
		if (key <= t->left->key.second) {
			if (erase_rf(t->left, key)) {
				Node* s = t;
				t = s->right;
				delete s;
			}
		}
		// 右の子への移動
		else {
			if (erase_rf(t->right, key)) {
				Node* s = t;
				t = s->left;
				delete s;
			}
		}
		update(*t);

		return false;
	}

	// key が [l, r] の範囲の v の要素全ての op() の結果を返す．
	S prod(const T& l, const T& r) {
		// verify : https://atcoder.jp/contests/jsc2021/tasks/jsc2021_f

		// 木が空なら単位元を返す．
		if (root == nullptr) {
			return e();
		}
		return prod_rf(root, l, r);
	}

	S prod_rf(Node* t, const T& l, const T& r) {
		// 範囲外なら単位元を返す．
		if (r < t->key.first || t->key.second < l) {
			return e();
		}

		// 完全に範囲内なら自身に記録されている値を返す．
		if (l <= t->key.first && t->key.second <= r) {
			return t->val;
		}

		// 範囲が一部かぶる場合
		S vl = prod_rf(t->left, l, r);
		S vr = prod_rf(t->right, l, r);

		return op(vl, vr);
	}

	// f( op( v[l, r) ) ) = true となる最大の r を返す．
	T max_right(const T& l, const function<bool(S)>& f) const {
		// 木が空の場合
		if (root == nullptr) {
			return ub();
		}

		S x = e();
		return max_right_rf(root, x, l, f);
	}

	T max_right_rf(Node* t, S& x, const T& l, const function<bool(S)>& f) const {
		// 範囲外の場合
		if (t->key.second < l) {
			return ub();
		}

		// f( op( v[t->key] ) ) = true の場合
		if (f(op(x, t->val))) {
			x = op(x, t->val);
			return ub();
		}

		// 自身が葉であればその位置を返す．
		if (t->left == nullptr) {
			return t->key.first;
		}

		// まず左の部分木を見に行き，見つかったならそれを返す．
		T key = max_right_rf(t->left, x, l, f);
		if (key != ub()) {
			return key;
		}

		// 見つからなかったなら右の部分木も見にいき，結果を返す．
		return max_right_rf(t->right, x, l, f);
	}

	// f( op( v(l, r] ) ) = true となる最小の l を返す．
	T min_left(const T& r, const function<bool(S)>& f) const {
		// 木が空の場合
		if (root == nullptr) {
			return lb();
		}

		S x = e();
		return min_left_rf(root, x, r, f);
	}

	T min_left_rf(Node* t, S& x, const T& r, const function<bool(S)>& f) const {
		// 範囲外の場合
		if (r < t->key.first) {
			return lb();
		}

		// f( op( v[t->key] ) ) = true の場合
		if (f(op(t->val, x))) {
			x = op(t->val, x);
			return lb();
		}

		// 自身が葉であればその位置を返す．
		if (t->left == nullptr) {
			return t->key.first;
		}

		// まず右の部分木を見に行き，見つかったならそれを返す．
		T key = min_left_rf(t->right, x, r, f);
		if (key != lb()) {
			return key;
		}

		// 見つからなかったなら左の部分木も見にいき，結果を返す．
		return min_left_rf(t->left, x, r, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Segtree_map& seg) {
		seg.print_rf(os, seg.root);
		return os;
	}
	void print_rf(ostream& os, Node* t) const {
		if (t == nullptr) return;

		print_rf(os, t->left);
		os << "key:" << t->key << ", val:" << t->val << ", left:";
		t->left != nullptr ? os << t->left->key : os << "-";
		os << ", right:";
		t->right != nullptr ? os << t->right->key : os << "-";
		os << endl;
		print_rf(os, t->right);
	}
#endif
};


//【連想遅延評価セグメント木（M-モノイド）】（遅い）
/*
* Lazy_segtree_map<T, lb, ub, S, op, e, F, act, comp, id>() : O(1)
*	空のセグメント木で初期化する．
*	要素は作用付きモノイド (S, op, e, F, act, comp, id) の元とする．
*	key は比較可能な型 T で (lb..ub) の範囲の値をとる．
*
* set(key, x) : O(log n)
*	v[key] = x とする．
*
* get(key) : O(log n)
*	v[key] を返す．なければ e を返す．
*
* erase(key) : O(log n)
*	v[key] を削除する．
*
* prod(l, r) : O(log n)
*	key が [l, r] の範囲の v の要素全ての op の結果を返す．
*
* apply(key, f) : O(log n)
*	v[key] = f( v[key] ) とする．
*
* apply(l, r, f) : O(log n)
*	v[l..r] = f( v[l..r] ) とする．
*
* max_right<f>(l) : O(log n)
*	f( op( v[l..r) ) ) = true となる最大の r を返す．
*   f : S → bool で f(e()) = true かつ単調とする．
*
* min_left<f>(r) : O(log n)
*	f( op( v(l..r] ) ) = true となる最小の l を返す．
*/
template <class T, T(*lb)(), T(*ub)(),
	class S, S(*op)(S, S), S(*e)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
struct Lazy_segtree_map {
	// 参考 : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_8_D
	// 参考 : https://algo-logic.info/segment-tree/

	// セグメント木のノード
	struct Node {
		pair<T, T> key;
		int size;
		S val;
		F lazy;
		int priority; // 優先度（大きいほど根に近い，葉は固定で -INF）
		Node* left;
		Node* right;

		// コンストラクタ
		Node() {}
		Node(const pair<T, T>& key_, int size_, const S& val_, const F& lazy_, int priority_) :
			key(key_), size(size_), val(val_), lazy(lazy_), priority(priority_),
			left(nullptr), right(nullptr) {}
	};

	Node* root; // 根へのポインタ
	mt19937 rnd; // 乱数生成器
	uniform_int_distribution<int> unirnd; // 一様乱数生成器

	// コンストラクタ（空で初期化）
	Lazy_segtree_map() : root(nullptr), rnd((int)time(0)), unirnd(-INF + 1, INF) {}

	// 遅延させていた評価を行う．：O(1)
	void eval(Node& t) {
		// 遅延させていた評価がなければ何もしない．
		if (t.lazy == id()) {
			return;
		}

		// 葉でなければ子に伝搬する．
		if (t.left != nullptr) {
			t.left->lazy = comp(t.lazy, t.left->lazy);
			t.right->lazy = comp(t.lazy, t.right->lazy);
		}

		// 自身を評価する．
		t.val = act(t.lazy, t.val);
		t.lazy = id();
	}

	// ノードの更新
	void update(Node& t) {
		if (t.left == nullptr) {
			return;
		}

		t.key.first = t.left->key.first;
		t.key.second = t.right->key.second;
		t.size = t.left->size + t.right->size;
		t.val = op(t.left->val, t.right->val);
	}

	// 木の右回転
	void rotate_right(Node*& t) {
		Node* s = t->left;
		eval(*t);
		eval(*s);

		t->left = s->right;
		s->right = t;

		update(*t);
		update(*s);
		t = s;
	}

	// 木の左回転
	void rotate_left(Node*& t) {
		Node* s = t->right;
		eval(*t);
		eval(*s);

		t->right = s->left;
		s->left = t;

		update(*t);
		update(*s);
		t = s;
	}

	// ヒープ条件を満たすよう木を葉の方向に回転していく．
	void rotate_to_leaf(Node*& t) {
		// ヒープ条件を満たしていたら何もしない．
		if (t->priority > t->left->priority && t->priority > t->right->priority) {
			return;
		}

		// 右だけ条件を満たしていない場合
		if (t->priority > t->left->priority) {
			rotate_left(t);
			rotate_to_leaf(t->left);
		}
		// 左だけ条件を満たしていない場合
		else if (t->priority > t->right->priority) {
			rotate_right(t);
			rotate_to_leaf(t->right);
		}
		// 両方とも条件を満たしていない場合
		else {
			// 左右の子のうち優先度の大きい方が上にくるよう回転して再帰を回す．
			if (t->left->priority < t->right->priority) {
				rotate_left(t);
				rotate_to_leaf(t->left);
			}
			else {
				rotate_right(t);
				rotate_to_leaf(t->right);
			}
		}
	}

	// v[key] = x とする．
	void set(const T& key, const S& x) {
		// 木が空の場合
		if (root == nullptr) {
			root = new Node({ key, key }, 1, x, id(), -INF);
		}
		else {
			set_rf(root, key, x);
		}
	}

	void set_rf(Node*& t, const T& key, const S& x) {
		eval(*t);

		// 区間の左端より左だった場合
		if (key < t->key.first) {
			// 区間を広げた新しいノードを作り，その左の子になる．
			Node* s = t;
			t = new Node({ key, t->key.second }, 1 + t->size, op(x, t->val), id(), unirnd(rnd));
			t->right = s;
			t->left = new Node({ key, key }, 1, x, id(), -INF);

			// ヒープ条件を満たすよう木を葉の方向に回転していく．
			rotate_to_leaf(t);
			return;
		}

		// 区間の右端より右だった場合
		if (key > t->key.second) {
			// 区間を広げた新しいノードを作り，その右の子になる．
			Node* s = t;
			t = new Node({ t->key.first, key }, t->size + 1, op(t->val, x), id(), unirnd(rnd));
			t->left = s;
			t->right = new Node({ key, key }, 1, x, id(), -INF);

			// ヒープ条件を満たすよう木を葉の方向に回転していく．
			rotate_to_leaf(t);
			return;
		}

		// 既に登録されている key なら値を更新する．
		if (t->key.first == t->key.second) {
			t->val = x;
			return;
		}

		// 左の子への移動
		if (key <= t->left->key.second) {
			set_rf(t->left, key, x);
			update(*t);
			if (t->priority < t->left->priority) {
				rotate_right(t);
			}
		}
		// 右の子への移動
		else {
			set_rf(t->right, key, x);
			update(*t);
			if (t->priority < t->right->priority) {
				rotate_left(t);
			}
		}
	}

	// v[key] を返す．なければ e() を返す．
	S get(const T& key) {
		return prod(key, key);
	}

	// v[key] を削除する．
	void erase(const T& key) {
		// 木が空の場合
		if (root == nullptr) return;

		erase_rf(root, key);
	}

	bool erase_rf(Node*& t, const T& key) {
		eval(*t);

		// 要素が存在しなかった場合
		if (key < t->key.first || t->key.second < key) {
			// 何もしない
			return false;
		}

		// 登録されている key なら削除する．
		if (t->key.first == t->key.second) {
			delete t;
			t = nullptr;

			return true;
		}

		// 左の子への移動
		if (key <= t->left->key.second) {
			if (erase_rf(t->left, key)) {
				t->size--;
				Node* s = t;
				t = s->right;
				delete s;
			}
		}
		// 右の子への移動
		else {
			if (erase_rf(t->right, key)) {
				t->size--;
				Node* s = t;
				t = s->left;
				delete s;
			}
		}
		update(*t);

		return false;
	}

	// key が [l, r] の範囲の v の要素全ての op() の結果を返す．
	S prod(const T& l, const T& r) {
		// 木が空の場合．
		if (root == nullptr) {
			return e();
		}
		return prod_rf(root, l, r);
	}

	S prod_rf(Node* t, const T& l, const T& r) {
		eval(*t);

		// 範囲外なら単位元を返す．
		if (r < t->key.first || t->key.second < l) {
			return e();
		}

		// 完全に範囲内なら自身に記録されている値を返す．
		if (l <= t->key.first && t->key.second <= r) {
			return t->val;
		}

		// 範囲が一部かぶる場合
		S vl = prod_rf(t->left, l, r);
		S vr = prod_rf(t->right, l, r);

		return op(vl, vr);
	}

	// v[key] = f(v[key]) とする．
	void apply(const T& key, const F& f) {
		apply(key, key, f);
	}

	// v[l..r] = f(v[l..r]) とする．
	void apply(const T& l, const T& r, const F& f) {
		// 木が空の場合
		if (root == nullptr) {
			return;
		}
		return apply_rf(root, l, r, f);
	}

	void apply_rf(Node* t, const T& l, const T& r, const F& f) {
		eval(*t);

		// 範囲外なら何もしない．
		if (r < t->key.first || t->key.second < l) {
			return;
		}

		// 完全に範囲内なら自身の値を更新する．
		if (l <= t->key.first && t->key.second <= r) {
			t->lazy = comp(f, t->lazy);
			eval(*t);

			return;
		}

		// 範囲が一部かぶる場合
		apply_rf(t->left, l, r, f);
		apply_rf(t->right, l, r, f);
		t->val = op(t->left->val, t->right->val);
	}

	// f( op( v[l, r) ) ) = true となる最大の r を返す．
	T max_right(const T& l, const function<bool(S)>& f) {
		// 木が空の場合
		if (root == nullptr) {
			return ub();
		}

		S x = e();
		return max_right_rf(root, x, l, f);
	}

	T max_right_rf(Node* t, S& x, const T& l, const function<bool(S)>& f) {
		eval(*t);

		// 範囲外の場合
		if (t->key.second < l) {
			return ub();
		}

		// f( op( v[t->key] ) ) = true の場合
		if (f(op(x, t->val))) {
			x = op(x, t->val);
			return ub();
		}

		// 自身が葉であればその位置を返す．
		if (t->left == nullptr) {
			return t->key.first;
		}

		// まず左の部分木を見に行き，見つかったならそれを返す．
		T key = max_right_rf(t->left, x, l, f);
		if (key != ub()) {
			return key;
		}

		// 見つからなかったなら右の部分木も見にいき，結果を返す．
		return max_right_rf(t->right, x, l, f);
	}

	// f( op( v(l, r] ) ) = true となる最小の l を返す．
	T min_left(const T& r, const function<bool(S)>& f) {
		// 木が空の場合
		if (root == nullptr) {
			return lb();
		}

		S x = e();
		return min_left_rf(root, x, r, f);
	}

	T min_left_rf(Node* t, S& x, const T& r, const function<bool(S)>& f) {
		eval(*t);

		// 範囲外の場合
		if (r < t->key.first) {
			return lb();
		}

		// f( op( v[t->key] ) ) = true の場合
		if (f(op(t->val, x))) {
			x = op(t->val, x);
			return lb();
		}

		// 自身が葉であればその位置を返す．
		if (t->left == nullptr) {
			return t->key.first;
		}

		// まず右の部分木を見に行き，見つかったならそれを返す．
		T key = min_left_rf(t->right, x, r, f);
		if (key != lb()) {
			return key;
		}

		// 見つからなかったなら左の部分木も見にいき，結果を返す．
		return min_left_rf(t->left, x, r, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Lazy_segtree_map& seg) {
		seg.print_rf(os, seg.root);
		return os;
	}
	void print_rf(ostream& os, Node* t) const {
		if (t == nullptr) return;

		print_rf(os, t->left);
		os << "key:" << t->key << ", size:" << t->size << ", val:" << t->val
			<< ", lazy:" << t->lazy << ", priority:" << t->priority << ", left:";
		t->left != nullptr ? os << t->left->key : os << "-";
		os << ", right:";
		t->right != nullptr ? os << t->right->key : os << "-";
		os << endl;
		print_rf(os, t->right);
	}
#endif
};


//【平行移動可能連想遅延評価セグメント木（M-モノイド）】（遅い）
/*
* Lazy_segtree_map_shiftable<T, lb, ub, add, zero, S, op, e, F, act, comp, id>() : O(1)
*	空のセグメント木で初期化する．
*	要素は作用付きモノイド (S, op, e, F, act, comp, id) の元とする．
*	キーは比較可能な型 T で (lb..ub) の範囲の値をとり，add で平行移動（零元は zero）できる．
*
* set(key, x) : O(log n)
*	v[key] = x とする．
*
* get(key) : O(log n)
*	v[key] を返す．なければ e を返す．
*
* erase(key) : O(log n)
*	v[key] を削除する．
*
* prod(l, r) : O(log n)
*	キーが [l, r] の範囲の v の要素全ての op の結果を返す．
*
* apply(key, f) : O(log n)
*	v[key] = f( v[key] ) とする．
*
* apply(l, r, f) : O(log n)
*	v[l..r] = f( v[l..r] ) とする．
*
* shift(key, s) : O(log n)
*	v[key] を v[key + s] に改める．
*	ただし木の形が変わるような変更は認めない．
*
* shift(l, r, s) : O(log n)
*	v[l..r] を v[l+s..r+s] に改める．
*	ただし木の形が変わるような変更は認めない．
*
* max() : O(log n)
*	最大のキーを返す．なければ lb を返す．
*
* min() : O(log n)
*	最小のキーを返す．なければ ub を返す．
*
* lower_bound(key) : O(log n)
*	key 以上の最小のキーを返す．なければ ub を返す．
*
* upper_bound(key) : O(log n)
*	key より大きい最小のキーを返す．なければ ub を返す．
*
* rlower_bound(key) : O(log n)
*	key 以下の最大のキーを返す．なければ lb を返す．
*
* rupper_bound(key) : O(log n)
*	key 未満の最大のキーを返す．なければ lb を返す．
*
* max_right<f>(l) : O(log n)
*	f( op( v[l..r) ) ) = true となる最大の r を返す．
*   f : S → bool で f(e()) = true かつ単調とする．
*
* min_left<f>(r) : O(log n)
*	f( op( v(l..r] ) ) = true となる最小の l を返す．
*/
template <class T, T(*lb)(), T(*ub)(), T(*add)(T, T), T(*zero)(),
	class S, S(*op)(S, S), S(*e)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
struct Lazy_segtree_map_shiftable {
	// 参考 : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_8_D
	// 参考 : https://algo-logic.info/segment-tree/

	const int LB = 0, UB = 1, RLB = 2, RUB = 3;

	// セグメント木のノード
	struct Node {
		int size;
		pair<T, T> key;
		T sft;
		S val;
		F lazy;
		int priority; // 優先度（大きいほど根に近い，葉は固定で -INF）
		Node* left;
		Node* right;

		// コンストラクタ
		Node() {}
		Node(const pair<T, T>& key_, int size_, const S& val_, const F& lazy_,
			const T& sft_, int priority_) :
			size(size_), key(key_), sft(sft_), val(val_), lazy(lazy_),
			priority(priority_), left(nullptr), right(nullptr) {}
	};

	Node* root; // 根へのポインタ
	mt19937 rnd; // 乱数生成器
	uniform_int_distribution<int> unirnd; // 一様乱数生成器


	// コンストラクタ（空で初期化）
	Lazy_segtree_map_shiftable() : root(nullptr), rnd((int)time(0)), unirnd(-INF + 1, INF) {}

	// 遅延させていた評価を行う．：O(1)
	void eval(Node& t) {
		// 遅延させていた評価がある場合．
		if (t.lazy != id()) {
			// 葉でなければ子に伝搬する．
			if (t.left != nullptr) {
				t.left->lazy = comp(t.lazy, t.left->lazy);
				t.right->lazy = comp(t.lazy, t.right->lazy);
			}

			// 自身を評価する．
			t.val = act(t.lazy, t.val);
			t.lazy = id();
		}

		// 遅延させていた平行移動がある場合
		if (t.sft != zero()) {
			// 葉でなければ子に伝搬する．
			if (t.left != nullptr) {
				t.left->sft = add(t.sft, t.left->sft);
				t.right->sft = add(t.sft, t.right->sft);
			}

			// 自身を評価する．
			t.key.first = add(t.key.first, t.sft);
			t.key.second = add(t.key.second, t.sft);
			t.sft = zero();
		}
	}

	// ノードの更新
	void update(Node& t) {
		if (t.left == nullptr) {
			return;
		}

		eval(*t.left);
		eval(*t.right);

		t.key.first = t.left->key.first;
		t.key.second = t.right->key.second;
		t.size = t.left->size + t.right->size;
		t.val = op(t.left->val, t.right->val);
	}

	// 木の右回転
	void rotate_right(Node*& t) {
		Node* s = t->left;
		eval(*t);
		eval(*s);

		t->left = s->right;
		s->right = t;

		update(*t);
		update(*s);
		t = s;
	}

	// 木の左回転
	void rotate_left(Node*& t) {
		Node* s = t->right;
		eval(*t);
		eval(*s);

		t->right = s->left;
		s->left = t;

		update(*t);
		update(*s);
		t = s;
	}

	// ヒープ条件を満たすよう木を葉の方向に回転していく．
	void rotate_to_leaf(Node*& t) {
		// ヒープ条件を満たしていたら何もしない．
		if (t->priority > t->left->priority && t->priority > t->right->priority) {
			return;
		}

		// 右だけ条件を満たしていない場合
		if (t->priority > t->left->priority) {
			rotate_left(t);
			rotate_to_leaf(t->left);
		}
		// 左だけ条件を満たしていない場合
		else if (t->priority > t->right->priority) {
			rotate_right(t);
			rotate_to_leaf(t->right);
		}
		// 両方とも条件を満たしていない場合
		else {
			// 左右の子のうち優先度の大きい方が上にくるよう回転して再帰を回す．
			if (t->left->priority < t->right->priority) {
				rotate_left(t);
				rotate_to_leaf(t->left);
			}
			else {
				rotate_right(t);
				rotate_to_leaf(t->right);
			}
		}
	}

	// v[key] = x とする．
	void set(const T& key, const S& x) {
		// 木が空の場合
		if (root == nullptr) {
			root = new Node({ key, key }, 1, x, id(), zero(), -INF);
		}
		else {
			set_rf(root, key, x);
		}
	}

	void set_rf(Node*& t, const T& key, const S& x) {
		eval(*t);

		// 区間の左端より左だった場合
		if (key < t->key.first) {
			// 区間を広げた新しいノードを作り，その左の子になる．
			Node* s = t;
			t = new Node({ key, t->key.second }, 1 + t->size, op(x, t->val),
				id(), zero(), unirnd(rnd));
			t->right = s;
			t->left = new Node({ key, key }, 1, x, id(), zero(), -INF);

			// ヒープ条件を満たすよう木を葉の方向に回転していく．
			rotate_to_leaf(t);
			return;
		}

		// 区間の右端より右だった場合
		if (key > t->key.second) {
			// 区間を広げた新しいノードを作り，その右の子になる．
			Node* s = t;
			t = new Node({ t->key.first, key }, t->size + 1, op(t->val, x),
				id(), zero(), unirnd(rnd));
			t->left = s;
			t->right = new Node({ key, key }, 1, x, id(), zero(), -INF);

			// ヒープ条件を満たすよう木を葉の方向に回転していく．
			rotate_to_leaf(t);
			return;
		}

		// 既に登録されている key なら値を更新する．
		if (t->key.first == t->key.second) {
			t->val = x;
			return;
		}

		// 左の子への移動
		if (key <= t->left->key.second) {
			set_rf(t->left, key, x);
			update(*t);
			if (t->priority < t->left->priority) {
				rotate_right(t);
			}
		}
		// 右の子への移動
		else {
			set_rf(t->right, key, x);
			update(*t);
			if (t->priority < t->right->priority) {
				rotate_left(t);
			}
		}
	}

	// v[key] を返す．なければ e() を返す．
	S get(const T& key) {
		return prod(key, key);
	}

	// v[key] を削除する．
	void erase(const T& key) {
		// 木が空の場合
		if (root == nullptr) return;

		erase_rf(root, key);
	}

	bool erase_rf(Node*& t, const T& key) {
		eval(*t);

		// 要素が存在しなかった場合
		if (key < t->key.first || t->key.second < key) {
			// 何もしない
			return false;
		}

		// 登録されている key なら削除する．
		if (t->key.first == t->key.second) {
			delete t;
			t = nullptr;

			return true;
		}

		// 左の子への移動
		if (key <= t->left->key.second) {
			if (erase_rf(t->left, key)) {
				t->size--;
				Node* s = t;
				t = s->right;
				delete s;
			}
		}
		// 右の子への移動
		else {
			if (erase_rf(t->right, key)) {
				t->size--;
				Node* s = t;
				t = s->left;
				delete s;
			}
		}
		update(*t);

		return false;
	}

	// key が [l, r] の範囲の v の要素全ての op() の結果を返す．
	S prod(const T& l, const T& r) {
		// 木が空の場合．
		if (root == nullptr) {
			return e();
		}
		return prod_rf(root, l, r);
	}

	S prod_rf(Node* t, const T& l, const T& r) {
		eval(*t);

		// 範囲外なら単位元を返す．
		if (r < t->key.first || t->key.second < l) {
			return e();
		}

		// 完全に範囲内なら自身に記録されている値を返す．
		if (l <= t->key.first && t->key.second <= r) {
			return t->val;
		}

		// 範囲が一部かぶる場合
		S vl = prod_rf(t->left, l, r);
		S vr = prod_rf(t->right, l, r);

		return op(vl, vr);
	}

	// v[key] = f(v[key]) とする．
	void apply(const T& key, const F& f) {
		apply(key, key, f);
	}

	// v[l..r] = f(v[l..r]) とする．
	void apply(const T& l, const T& r, const F& f) {
		// 木が空の場合
		if (root == nullptr) {
			return;
		}
		return apply_rf(root, l, r, f);
	}

	void apply_rf(Node* t, const T& l, const T& r, const F& f) {
		eval(*t);

		// 範囲外なら何もしない．
		if (r < t->key.first || t->key.second < l) {
			return;
		}

		// 完全に範囲内なら自身の値を更新する．
		if (l <= t->key.first && t->key.second <= r) {
			t->lazy = comp(f, t->lazy);
			eval(*t);

			return;
		}

		// 範囲が一部かぶる場合
		apply_rf(t->left, l, r, f);
		apply_rf(t->right, l, r, f);
		t->val = op(t->left->val, t->right->val);
	}

	// v[key] を v[key + s] に改める．
	void shift(const T& key, const T& s) {
		shift(key, key, s);
	}

	// v[l..r] を v[l+s..r+s] に改める．
	void shift(const T& l, const T& r, const T& s) {
		// 木が空の場合
		if (root == nullptr) {
			return;
		}
		return shift_rf(root, l, r, s);
	}

	void shift_rf(Node* t, const T& l, const T& r, const T& s) {
		eval(*t);

		// 範囲外なら何もしない．
		if (r < t->key.first || t->key.second < l) {
			return;
		}

		// 完全に範囲内なら自身の値を更新する．
		if (l <= t->key.first && t->key.second <= r) {
			t->sft = add(s, t->sft);
			eval(*t);

			return;
		}

		// 範囲が一部かぶる場合
		shift_rf(t->left, l, r, s);
		shift_rf(t->right, l, r, s);
		update(*t);
	}

	// 最大のキーを返す．なければ lb を返す．
	T max() { return rlower_bound(ub()); }

	// 最小のキーを返す．なければ ub を返す．
	T min() { return lower_bound(lb()); }

	// key 以上の最小のキーを返す．なければ ub を返す．
	T lower_bound(const T& key) { return bound(key, LB); }

	// key より大きい最小のキーを返す．なければ ub を返す．
	T upper_bound(const T& key) { return bound(key, UB); }

	// key 以下の最大のキーを返す．なければ lb を返す．
	T rlower_bound(const T& key) { return bound(key, RLB); }

	// key 未満の最大のキーを返す．なければ lb を返す．
	T rupper_bound(const T& key) { return bound(key, RUB); }

	T bound(const T& key, int type) {
		bool end_flag = true;

		// 木は空でないが該当するキーが存在しない場合
		if (root != nullptr) {
			eval(*root);

			if (type == LB) end_flag = key > root->key.second;
			else if (type == UB) end_flag = key >= root->key.second;
			else if (type == RLB) end_flag = key < root->key.first;
			else if (type == RUB) end_flag = key <= root->key.first;
		}

		// 木が空である場合とまとめて該当するキーがないことを返す．
		if (end_flag) {
			if (type == LB || type == UB) return ub();
			else return lb();
		}

		// 探索タイプに応じて子を辿っていく．
		Node* t = root;
		while (t->left != nullptr) {
			eval(*t->left);
			eval(*t->right);

			bool left_flag;
			if (type == LB) left_flag = key <= t->left->key.second;
			else if (type == UB) left_flag = key < t->left->key.second;
			else if (type == RLB) left_flag = key < t->right->key.first;
			else if (type == RUB) left_flag = key <= t->right->key.first;

			if (left_flag) t = t->left;
			else t = t->right;
		}

		return t->key.first;
	}

	// f( op( v[l, r) ) ) = true となる最大の r を返す．
	T max_right(const T& l, const function<bool(S)>& f) {
		// 木が空の場合
		if (root == nullptr) {
			return ub();
		}

		S x = e();
		return max_right_rf(root, x, l, f);
	}

	T max_right_rf(Node* t, S& x, const T& l, const function<bool(S)>& f) {
		eval(*t);

		// 範囲外の場合
		if (t->key.second < l) {
			return ub();
		}

		// f( op( v[t->key] ) ) = true の場合
		if (f(op(x, t->val))) {
			x = op(x, t->val);
			return ub();
		}

		// 自身が葉であればその位置を返す．
		if (t->left == nullptr) {
			return t->key.first;
		}

		// まず左の部分木を見に行き，見つかったならそれを返す．
		T key = max_right_rf(t->left, x, l, f);
		if (key != ub()) {
			return key;
		}

		// 見つからなかったなら右の部分木も見にいき，結果を返す．
		return max_right_rf(t->right, x, l, f);
	}

	// f( op( v(l, r] ) ) = true となる最小の l を返す．
	T min_left(const T& r, const function<bool(S)>& f) {
		// 木が空の場合
		if (root == nullptr) {
			return lb();
		}

		S x = e();
		return min_left_rf(root, x, r, f);
	}

	T min_left_rf(Node* t, S& x, const T& r, const function<bool(S)>& f) {
		eval(*t);

		// 範囲外の場合
		if (r < t->key.first) {
			return lb();
		}

		// f( op( v[t->key] ) ) = true の場合
		if (f(op(t->val, x))) {
			x = op(t->val, x);
			return lb();
		}

		// 自身が葉であればその位置を返す．
		if (t->left == nullptr) {
			return t->key.first;
		}

		// まず右の部分木を見に行き，見つかったならそれを返す．
		T key = min_left_rf(t->right, x, r, f);
		if (key != lb()) {
			return key;
		}

		// 見つからなかったなら左の部分木も見にいき，結果を返す．
		return min_left_rf(t->left, x, r, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Lazy_segtree_map_shiftable& seg) {
		seg.cout_rf(os, seg.root);
		return os;
	}
	void cout_rf(ostream& os, Node* t) const {
		if (t == nullptr) return;

		cout_rf(os, t->left);

		os << "size:" << t->size;
		os << ", key:" << t->key;
		os << ", sft:" << t->sft;
		os << ", val:" << t->val;
		os << ", lazy:" << t->lazy;
		os << ", priority:" << t->priority;
		os << ", left:";
		t->left != nullptr ? os << t->left->key : os << "-";
		os << ", right:";
		t->right != nullptr ? os << t->right->key : os << "-";
		os << endl;

		cout_rf(os, t->right);
	}

	void print() const {
		print_rf(root);
	}
	void print_rf(Node* t) const {
		if (t == nullptr) return;

		print_rf(t->left);

		if (t->left == nullptr) {
			pair<T, T> key_(add(t->key.first, t->sft), add(t->key.second, t->sft));
			S val_(act(t->lazy, t->val));
			cout << "key:" << key_ << ", val:" << val_ << endl;
		}

		print_rf(t->right);
	}
#endif
};


