#pragma once
#include "header.h"
// ■■■■■ フェニック木 ■■■■■


//【フェニック木（アーベル群）】
/*
* Fenwick_tree<S, op, o, inv>(int n) : O(n)
*	a[0..n) = o() で初期化する．要素はアーベル群 (S, op, o, inv) の元とする．
*
* Fenwick_tree<S, op, o, inv>(vS a) : O(n)
*	配列 a[0..n) で初期化する．
*
* set(int i, S x) : O(log n)
*	a[i] = x とする．
*
* S get(int i) : O(log n)
*	a[i] を返す．
*
* S sum(int l, int r) : O(log n)
*	Σa[l..r) を返す．空なら o() を返す．
*
* add(int i, S x) : O(log n)
*	a[i] += x とする．
*
* int max_right(function<bool(S)>& f) : O(log n)
*	f( Σa[0..r) ) = true となる最大の r を返す．
*   制約：f( o() ) = true，f は単調
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
class Fenwick_tree {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数（要素数 + 1）
	int n;

	// v[i] : Σa[*..i] の値（i:1-indexed，v[0] は不使用）
	vector<S> v;

	// Σa[1..r] を返す．空なら o() を返す．（r:1-indexed）
	S sum_sub(int r) const {
		S res = o();

		// 根に向かって累積 op() をとっていく．
		while (r > 0) {
			res = op(res, v[r]);

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

public:
	// a[0..n) = o() で初期化する．
	Fenwick_tree(int n_) : n(n_ + 1), v(n, o()) {
		// verify : https://judge.yosupo.jp/problem/range_kth_smallest
	}

	// 配列 a[0..n) で初期化する．
	Fenwick_tree(const vector<S>& a) : n(sz(a) + 1), v(n) {
		// verify : https://judge.yosupo.jp/problem/point_add_range_sum

		// 配列の値を仮登録する．
		rep(i, n - 1) v[i + 1] = a[i];

		// 正しい値になるよう根に向かって累積 op() をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[i] = op(v[i], v[i - pow2]);
			}
		}
	}
	Fenwick_tree() : n(0) {}

	// a[i] = x とする．（i : 0-indexed）
	void set(int i, S x) {
		Assert(0 <= i && i < n);

		// 差分を求める．
		S d = op(x, inv(get(i)));

		add(i, d);
	}

	// a[i] を返す．（i : 0-indexed）
	S get(int i) const {
		Assert(0 <= i && i < n);

		return sum(i, i + 1);
	}

	// Σa[l..r) を返す．空なら o() を返す．（l, r : 0-indexed）
	S sum(int l, int r) const {
		// verify : https://judge.yosupo.jp/problem/point_add_range_sum

		chmax(l, 0); chmin(r, n);
		if (l >= r) return o();

		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return op(sum_sub(r), inv(sum_sub(l)));
	}

	// a[i] += x とする．（i : 0-indexed）
	void add(int i, S x) {
		// verify : https://judge.yosupo.jp/problem/point_add_range_sum

		Assert(0 <= i && i < n);

		// i を 1-indexed に直す．
		i++;

		// 根に向かって値を op() していく．
		while (i < n) {
			v[i] = op(v[i], x);

			// i の最下位ビットに 1 を加算することで次の位置を得る．
			i += i & -i;
		}
	}

	// f( Σa[0..r) ) = true となる最大の r を返す．（r : 0-indexed）
	int max_right(const function<bool(S)>& f) const {
		// verify : https://www.spoj.com/problems/ALLIN1/

		S x = o();

		// 注目している閉区間は [l+1, r] で幅は len
		int l = 0;
		for (int len = 1 << msb(n - 1); len > 0; len = len >> 1) {
			int r = l + len;

			if (r < n && f(op(x, v[r]))) {
				x = op(x, v[r]);
				l = r;
			}
		}
		return l;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Fenwick_tree& ft) {
		rep(i, ft.n - 1) {
			os << ft.get(i) << " ";
		}
		return os;
	}
#endif
};


//【区間加算フェニック木（Z-加群）】
/*
* Fenwick_tree_range_add<S, op, o, inv, mul>(int n) : O(n)
*	要素数 n かつ初期値 o() で初期化する．
*	要素は Z-加群 (S, op, o, inv, mul) の元とする．
*
* Fenwick_tree_range_add<S, op, o, inv, mul>(vS v) : O(n)
*	v[0..n) で初期化する．
*
* set(int i, S x) : O(log n)
*	v[i] = x とする．
*
* S get(int i) : O(log n)
*	v[i] を返す．
*
* S sum(int l, int r) : O(log n)
*	Σv[l..r) を返す．空なら o() を返す．
*
* add(int i, S x) : O(log n)
*	v[i] += x とする．
*
* add(int l, int r, S x) : O(log n)
*	v[l..r) += x とする．
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S), S(*mul)(ll, S)>
struct Fenwick_tree_range_add {
	// 参考：https://algo-logic.info/binary-indexed-tree/
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_G

	// ノードの個数（要素数 + 1）
	int n;

	// Σv[1..i] を acc0[i] + i acc1[i] と分解する．
	// さらに accD[i] = ΣrawD[1..i] と表されるような rawD を導入する．
	// v[D][i] : ΣrawD[*..i] の値（i:1-indexed，v[D][0] は使わない）
	vector<vector<S>> v;
	
	// 要素数 n かつ初期値 o() で初期化
	Fenwick_tree_range_add(int n_) : n(n_ + 1), v(2, vector<S>(n, o())) {}

	// 配列 a で初期化
	Fenwick_tree_range_add(const vector<S>& v_) : n(sz(v_) + 1), v(2, vector<S>(n, o())) {
		// 配列の値を仮登録する．
		rep(i, n - 1) v[0][i + 1] = v_[i];

		// 正しい値になるよう根に向かって累積 op() をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[0][i] = op(v[0][i], v[0][i - pow2]);
			}
		}
	}
	Fenwick_tree_range_add() : n(0) {}

	// v[i] = x とする．（i : 0-indexed）
	void set(int i, S x) {
		// 差分を求める．
		S d = op(x, inv(get(i)));

		add(i, d);
	}

	// v[i] を返す．（i : 0-indexed）
	S get(int i) const {
		return sum(i, i + 1);
	}

	// Σv[l..r) を返す．空なら o() を返す．（l, r : 0-indexed）
	S sum(int l, int r) const {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return op(sum_sub(r), inv(sum_sub(l)));
	}

	// Σv[1..r] を返す．空なら o() を返す．（r : 1-indexed）
	S sum_sub(int r) const {
		return op(sum_sub(r, 0), mul((ll)r, sum_sub(r, 1)));
	}

	// Σv[d][1..r] を返す．空なら o() を返す．（r : 1-indexed）
	S sum_sub(int r, int d) const {
		S res = o();

		// 子に向かって累積 op() をとっていく．
		while (r > 0) {
			res = op(res, v[d][r]);

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// v[i] += x とする．（i : 0-indexed）
	void add(int i, S x) {
		// i を 1-indexed に直す．
		i++;

		add_sub(i, x, 0);
	}

	// v[l..r) += x とする．（l, r : 0-indexed） 
	void add(int l, int r, S x) {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		l++;

		// 区間の端の値を調整する．
		add_sub(l, mul((ll)(l - 1), inv(x)), 0);
		add_sub(r + 1, mul((ll)r, x), 0);

		add_sub(l, x, 1);
		add_sub(r + 1, inv(x), 1);
	}

	// v[d][i] += x とする．（i : 1-indexed）
	void add_sub(int i, S x, int d) {
		// 根に向かって値を op() していく．
		while (i < n) {
			v[d][i] = op(v[d][i], x);

			// i の最下位ビットに 1 を加算することで次の位置を得る．
			i += i & -i;
		}
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Fenwick_tree_range_add& ft) {
		rep(i, ft.n - 1) os << ft.get(i) << " ";
		return os;
	}
#endif
};


//【区間線型加算フェニック木】
/*
* Fenwick_tree_range_linear_add<S>(int n) : O(n)
*	v[0..n) = 0 で初期化する．
*	制約 : S の元は 2 で割れる．
*
* Fenwick_tree_range_linear_add<S>(vS v) : O(n)
*	v[0..n) で初期化する．
*
* set(int i, S b) : O(log n)
*	v[i] = b とする．
*
* S get(int i) : O(log n)
*	v[i] を返す．
*
* S sum(int l, int r) : O(log n)
*	Σv[l..r) を返す．空なら 0 を返す．
*
* add(int i, S b) : O(log n)
*	v[i] += b とする．
*
* add_const(int l, int r, S b) : O(log n)
*	v[l..r) += b とする．
*
* add_linear(int l, int r, S a, S b) : O(log n)
*	i∈[l..r) について，v[i] += a i + b とする．
*
* add_linear_right(ll l, ll r, S w0, S w1) : O(log n)
*	v[l..r) に昇順に等差数列 w0, w1, ... を加える．
*
* add_linear_left(ll r, ll l, S w0, S w1) : O(log n)
*	v(l..r] に降順に等差数列 w0, w1, ... を加える．
*/
template <class S>
class Fenwick_tree_range_linear_add {
	// ノードの個数（要素数 + 1）
	int n;

	// Σv[1..i] を acc0[i] + i acc1[i] + i^2 acc2[i] と分解する．
	// さらに accD[i] = (1/2)ΣrawD[1..i] と表されるような rawD を導入する．
	// v[D][i] : ΣrawD[*..i] の値（i:1-indexed，v[D][0] は使わない）
	vector<vector<S>> v;

	// Σv[1..r] を返す．空なら 0 を返す．（r : 1-indexed）
	S sum_sub(int r) const {
		return sum_sub(r, 0) + sum_sub(r, 1) * r + sum_sub(r, 2) * r * r;
	}

	// Σv[d][1..r] を返す．空なら 0 を返す．（r : 1-indexed）
	S sum_sub(int r, int d) const {
		S res = 0;

		// 子に向かって累積和をとっていく．
		while (r > 0) {
			res += v[d][r];

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}

		return res;
	}

	// v[d][i] += x とする．（i : 0-indexed）
	void add_sub(int i, S x, int d) {
		// 1-indexed に直す．
		i++;

		// 根に向かって値を加算していく．
		while (i < n) {
			v[d][i] += x;

			// i の最下位ビットに 1 を加算することで次の位置を得る．
			i += i & -i;
		}
	}

public:
	// v[0..n) = 0 で初期化する．
	Fenwick_tree_range_linear_add(int n_) : n(n_ + 1), v(3, vector<S>(n)) {
		// verify : https://yukicoder.me/problems/no/2662
	}

	// v[0..n) で初期化する．
	Fenwick_tree_range_linear_add(const vector<S>& v_) : n(sz(v_) + 1), v(3, vector<S>(n)) {
		// 配列の値を仮登録する．
		rep(i, n - 1) v[0][i + 1] = 2 * v_[i];

		// 正しい値になるよう根に向かって累積和をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[0][i] += v[0][i - pow2];
			}
		}
	}
	Fenwick_tree_range_linear_add() : n(0) {}

	// v[i] = b とする．（i : 0-indexed）
	void set(int i, S b) {
		add(i, b - get(i));
	}

	// v[i] を返す．（i : 0-indexed）
	S get(int i) const {
		// verify : https://yukicoder.me/problems/no/2662

		return sum(i, i + 1);
	}

	// Σv[l..r) を返す．空なら 0 を返す．（l, r : 0-indexed）
	S sum(int l, int r) const {
		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return (sum_sub(r) - sum_sub(l)) / 2;
	}

	// v[i] += b とする．（i : 0-indexed）
	void add(int i, S b) {
		if (i < 0 || n <= i) return;

		add_sub(i, 2 * b, 0);
	}

	// v[l..r) += b とする．（l, r : 0-indexed） 
	void add_const(int l, int r, S b) {
		chmax(l, 0); chmin(r, n);
		if (l >= r) return;

		add_sub(l, -2 * l * b, 0);
		add_sub(r, 2 * r * b, 0);
		add_sub(l, 2 * b, 1);
		add_sub(r, -2 * b, 1);
	}

	// i∈[l..r) について，v[i] += a i + b とする．
	void add_linear(int l, int r, S a, S b) {
		chmax(l, 0); chmin(r, n);
		if (l >= r) return;

		add_sub(l, -l * (a * (l - 1) + 2 * b), 0);
		add_sub(r, r * (a * (r - 1) + 2 * b), 0);
		add_sub(l, -a + 2 * b, 1);
		add_sub(r, a - 2 * b, 1);
		add_sub(l, a, 2);
		add_sub(r, -a, 2);
	}

	// v[l..r) に昇順に等差数列 w0, w1, ... を加える．
	void add_linear_right(ll l, ll r, S w0, S w1) {
		// verify : https://yukicoder.me/problems/no/2662

		// a l + b = w0, a(l+1) + b = w1 を解いて a, b を求める．
		ll a = w1 - w0;
		ll b = w0 - a * l;
		add_linear((int)max<ll>(l, 0), (int)min<ll>(r, n), a, b);
	}

	// v(l..r] に降順に等差数列 w0, w1, ... を加える．
	void add_linear_left(ll r, ll l, S w0, S w1) {
		// verify : https://yukicoder.me/problems/no/2662

		// a r + b = w0, a(r-1) + b = w1 を解いて a, b を求める．
		ll a = w0 - w1;
		ll b = w0 - a * r;
		add_linear((int)max<ll>(l + 1, 0), (int)min<ll>(r + 1, n), a, b);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Fenwick_tree_range_linear_add& ft) {
		rep(i, ft.n - 1) os << ft.get(i) << " ";
		return os;
	}
#endif
};


//【動的フェニック木（アーベル群）】（遅い）
/*
* Fenwick_tree<S, op, o, inv>(int n) : O(1)
*	要素数 n かつ初期値 o() で初期化する．
*	要素はアーベル群 (S, op, o, inv) の元とする．
*
* add(ll i, S x) : O(log n)
*	a[i] = op(a[i], x) とする．
*
* set(ll i, S x) : O(log n)
*	a[i] = x とする．
*
* S get(ll i) : O(log n)
*	a[i] を返す．
*
* S sum(ll r) : O(log n)
*	op( a[0..r) ) を返す．空なら o() を返す．
*
* S sum(ll l, ll r) : O(log n)
*	op( a[l..r) ) を返す．空なら o() を返す．
*
* ll max_right(function<bool(S)>& f) : O(log n)
*	f( op( a[0..r) ) ) = true となる最大の r を返す．
*   f : S → bool で f(o()) = true かつ単調とする．
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
struct Dynamic_fenwick_tree {
	ll n;
	unordered_map<ll, S> v;

	// 要素数 n かつ初期値 o() で初期化
	Dynamic_fenwick_tree(ll n) : n(n) {}
	Dynamic_fenwick_tree() {}

	// v[i] = x とする．（i : 0-indexed）
	void set(ll i, S x) {
		// 差分を求める．
		S d = op(x, inv(get(i)));

		add(i, d);
	}

	// v[i] を返す．（i : 0-indexed）
	S get(ll i) const {
		return sum(i, i + 1);
	}

	// op( v[0..r) ) を返す．空なら o() を返す．（l, r : 0-indexed）
	S sum(ll r) const {
		if (r <= 0) return o();

		// 0-indexed での半開区間 [0, r) は，
		// 1-indexed での閉区間 [1, r] に対応する．
		return sum_sub(r);
	}

	// op( v[l..r) ) を返す．空なら o() を返す．（l, r : 0-indexed）
	S sum(ll l, ll r) const {
		if (l >= r) return o();

		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return sum_sub(r) - sum_sub(l);
	}

	// op( v[1..r] ) を返す．空なら o() を返す．（r : 1-indexed）
	S sum_sub(ll r) const {
		S res = o();

		// 子に向かって累積 op() をとっていく．
		while (r > 0) {
			auto it = v.find(r);
			if (it != v.end()) res = op(res, it->second);

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// v[i] = op(v[i], x) とする．（i : 0-indexed）
	void add(ll i, S x) {
		// i を 1-indexed に直す．
		i++;

		// 根に向かって値を op() していく．
		while (i < n) {
			auto it = v.find(i);
			if (it != v.end()) it->second = op(it->second, x);
			else v[i] = x;

			// i の最下位ビットに 1 を加算することで次の位置を得る．
			i += i & -i;
		}
	}

	// f( op( v[0, r) ) ) = true となる最大の r を返す．（r : 0-indexed）
	ll max_right(const function<bool(S)>& f) const {
		S x = o();

		// 注目している閉区間は [l+1, r] で幅は len
		ll l = 0;
		for (ll len = 1LL << msb(n - 1); len > 0; len = len >> 1) {
			ll r = l + len;

			auto it = v.find(r);
			S nx = (it != v.end() ? op(x, it->second) : x);
			if (r < n && f(nx)) {
				x = nx;
				l = r;
			}
		}
		return l;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Dynamic_fenwick_tree& ft) {
		vector<pair<ll, S>> ivs;
		repe(iv, ft.v) ivs.push_back(iv);
		sort(all(ivs));

		for (auto& [i, v] : ivs) os << "(" << i << "," << v << ") ";
		return os;
	}
#endif
};


//【動的区間線型加算フェニック木】（遅い）
/*
* Fenwick_tree_range_linear_add<S>(int n) : O(n)
*	v[0..n) = 0 で初期化する．
*	制約 : S の元は 2 で割れる．
*
* Fenwick_tree_range_linear_add<S>(vS v) : O(n)
*	v[0..n) で初期化する．
*
* set(int i, S b) : O(log n)
*	v[i] = b とする．
*
* S get(int i) : O(log n)
*	v[i] を返す．
*
* S sum(int l, int r) : O(log n)
*	Σv[l..r) を返す．空なら 0 を返す．
*
* add(int i, S b) : O(log n)
*	v[i] += b とする．
*
* add_const(int l, int r, S b) : O(log n)
*	v[l..r) += b とする．
*
* add_linear(int l, int r, S a, S b) : O(log n)
*	i∈[l..r) について，v[i] += a i + b とする．
*
* add_linear_right(int l, int r, S w0, S w1) : O(log n)
*	v[l..r) に昇順に等差数列 w0, w1, ... を加える．
*
* add_linear_left(int r, int l, S w0, S w1) : O(log n)
*	v(l..r] に降順に等差数列 w0, w1, ... を加える．
*/
template <class S>
class Dynamic_Fenwick_tree_range_linear_add {
	// ノードの個数（要素数 + 1）
	ll n;

	// Σv[1..i] を acc0[i] + i acc1[i] + i^2 acc2[i] と分解する．
	// さらに accD[i] = (1/2)ΣrawD[1..i] と表されるような rawD を導入する．
	// v[D][i] : ΣrawD[*..i] の値（i:1-indexed，v[D][0] は使わない）
	unordered_map<ll, vector<S>> v;

	// Σv[1..r] を返す．空なら 0 を返す．（r : 1-indexed）
	S sum_sub(ll r) const {
		return sum_sub(r, 0) + sum_sub(r, 1) * r + sum_sub(r, 2) * r * r;
	}

	// Σv[d][1..r] を返す．空なら 0 を返す．（r : 1-indexed）
	S sum_sub(ll r, ll d) const {
		S res = 0;

		// 子に向かって累積和をとっていく．
		while (r > 0) {
			auto it = v.find(r);
			if (it != v.end()) res += it->second[d];

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}

		return res;
	}

	// v[d][i] += x とする．（i : 0-indexed）
	void add_sub(ll i, S x, int d) {
		// 1-indexed に直す．
		i++;

		// 根に向かって値を加算していく．
		while (i < n) {
			auto it = v.find(i);
			if (it != v.end()) it->second[d] += x;
			else {
				vl ini(3);
				ini[d] = x;
				v[i] = ini;
			}

			// i の最下位ビットに 1 を加算することで次の位置を得る．
			i += i & -i;
		}
	}

public:
	// v[0..n) = 0 で初期化する．
	Dynamic_Fenwick_tree_range_linear_add(ll n_) : n(n_ + 1) {}
	Dynamic_Fenwick_tree_range_linear_add() : n(0) {}

	// v[i] = b とする．（i : 0-indexed）
	void set(ll i, S b) {
		add(i, b - get(i));
	}

	// v[i] を返す．（i : 0-indexed）
	S get(ll i) const {
		// verify : https://www.codechef.com/problems/STREETTA

		return sum(i, i + 1);
	}

	// Σv[l..r) を返す．空なら 0 を返す．（l, r : 0-indexed）
	S sum(ll l, ll r) const {
		chmax(l, 0LL); chmin(r, n);
		if (l >= r) return 0;

		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return (sum_sub(r) - sum_sub(l)) / 2;
	}

	// v[i] += b とする．（i : 0-indexed）
	void add(ll i, S b) {
		if (i < 0 || n <= i) return;

		add_sub(i, 2 * b, 0);
	}

	// v[l..r) += b とする．（l, r : 0-indexed） 
	void add_const(ll l, ll r, S b) {
		chmax(l, 0LL); chmin(r, n);
		if (l >= r) return;

		add_sub(l, -2 * l * b, 0);
		add_sub(r, 2 * r * b, 0);
		add_sub(l, 2 * b, 1);
		add_sub(r, -2 * b, 1);
	}

	// i∈[l..r) について，v[i] += a i + b とする．
	void add_linear(ll l, ll r, S a, S b) {
		chmax(l, 0LL); chmin(r, n);
		if (l >= r) return;

		add_sub(l, -l * (a * (l - 1) + 2 * b), 0);
		add_sub(r, r * (a * (r - 1) + 2 * b), 0);
		add_sub(l, -a + 2 * b, 1);
		add_sub(r, a - 2 * b, 1);
		add_sub(l, a, 2);
		add_sub(r, -a, 2);
	}

	// v[l..r) に昇順に等差数列 w0, w1, ... を加える．
	void add_linear_right(ll l, ll r, S w0, S w1) {
		// verify : https://www.codechef.com/problems/STREETTA

		// a l + b = w0, a(l+1) + b = w1 を解いて a, b を求める．
		ll a = w1 - w0;
		ll b = w0 - a * l;
		add_linear(l, r, a, b);
	}

	// v(l..r] に降順に等差数列 w0, w1, ... を加える．
	void add_linear_left(ll r, ll l, S w0, S w1) {
		// a r + b = w0, a(r-1) + b = w1 を解いて a, b を求める．
		ll a = w0 - w1;
		ll b = w0 - a * r;
		add_linear(l + 1, r + 1, a, b);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Dynamic_Fenwick_tree_range_linear_add& ft) {
		rep(i, ft.n - 1) os << ft.get(i) << " ";
		return os;
	}
#endif
};


//【二次元フェニック木（アーベル群）】
/*
* Fenwick_tree_2D<S, op, o, inv>(int h, int w) : O(h w)
*	要素数 h * w かつ初期値 o で初期化する．
*	要素はアーベル群 (S, op, o, inv) の元とする．
*
* Fenwick_tree_2D<S, op, o, inv>(vvS a) : O(h w)
*	二次元配列 a で初期化する．
*
* apply(int x, int y, S val) : O(log h log w)
*	v[x][y] = op(v[x][y], val) とする．
*
* set(int x, int y, S val) : O(log h log w)
*	v[x][y] = val とする．
*
* S get(int x, int y) : O(log h log w)
*	v[x][y] を返す．
*
* S prod(int x1, int y1, int x2, int y2) : O(log h log w)
*	op( v[x1..x2)[y1..y2) ) を返す．空なら o() を返す．
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
struct Fenwick_tree_2D {
	// 参考：https://algo-logic.info/binary-indexed-tree/
	
	// ノードの個数（要素数 + 1）
	int h, w;

	// v[x][y] : op( [*..x][*..y] ) の値（x, y ： 1-indexed，v[0][*], v[*][0] は使わない）
	vector<vector<S>> v;

	// op( v[1..x][1..y] ) を返す．空なら o を返す．（x, y : 1-indexed）
	S prod_sub(int x, int y) const {
		S res = o();

		// 子に向かって累積 op() をとっていく．
		// i, j の最下位ビットから 1 を減算することで次の位置を得る．
		for (int i = x; i > 0; i -= i & -i) {
			for (int j = y; j > 0; j -= j & -j) {
				res = op(res, v[i][j]);
			}
		}

		return res;
	}

	// 要素数 h * w かつ初期値 o で初期化
	Fenwick_tree_2D(int h_, int w_) : h(h_ + 1), w(w_ + 1), v(h, vector<S>(w, o())) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2842
	}

	// 配列 a で初期化
	Fenwick_tree_2D(const vector<vector<S>>& v_) : h(sz(v_) + 1), w(sz(v_[0]) + 1),
		v(h, vector<S>(w, o())) {
		// 配列の値を仮登録する．
		rep(i, h - 1) {
			rep(j, w - 1) {
				v[i + 1][j + 1] = v_[i][j];
			}
		}

		// 正しい値になるよう根に向かって累積 op() をとっていく．
		// j 方向
		repi(i, 1, h - 1) {
			for (int pow2 = 1; 2 * pow2 < w; pow2 *= 2) {
				for (int j = 2 * pow2; j < w; j += 2 * pow2) {
					v[i][j] = op(v[i][j], v[i][j - pow2]);
				}
			}
		}
		// i 方向
		repi(j, 1, w - 1) {
			for (int pow2 = 1; 2 * pow2 < h; pow2 *= 2) {
				for (int i = 2 * pow2; i < h; i += 2 * pow2) {
					v[i][j] = op(v[i][j], v[i - pow2][j]);
				}
			}
		}
	}
	Fenwick_tree_2D() {} // ダミー

	// v[x][y] = val とする．（x, y : 0-indexed）
	void set(int x, int y, S val) {
		// 差分を求める．
		S d = op(val, inv(get(x, y)));

		apply(x, y, d);
	}

	// v[x][y] を返す．（x, y : 0-indexed）
	S get(int x, int y) const {
		return prod(x, y, x + 1, y + 1);
	}

	// op( v[x1..x2)[y1..y2) ) を返す．空なら o を返す．（x1, y1, x2, y2 : 0-indexed）
	S prod(int x1, int y1, int x2, int y2) const {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2842

		// 0-indexed での半開長方形 [x1..x2) * [y1..y2) は，
		// 1-indexed での閉長方形 [x1+1..x2] * [y1+1..y2] に対応する．
		S res = o();
		res = op(res, prod_sub(x2, y2));
		res = op(res, inv(prod_sub(x2, y1)));
		res = op(res, inv(prod_sub(x1, y2)));
		res = op(res, prod_sub(x1, y1));
		return res;
	}

	// v[x][y] = op(v[x][y], val) とする．（x, y : 0-indexed）
	void apply(int x, int y, S val) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2842

		// x, y を 1-indexed に直す．
		x++; y++;

		// 根に向かって値を op() していく．
		// i, j の最下位ビットに 1 を加算することで次の位置を得る．
		for (int i = x; i < h; i += i & -i) {
			for (int j = y; j < w; j += j & -j) {
				v[i][j] = op(v[i][j], val);
			}
		}
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Fenwick_tree_2D& ft) {
		rep(x, ft.h - 1) {
			rep(y, ft.w - 1) {
				os << ft.get(x, y) << " ";
			}
			cout << "\n";
		}
		return os;
	}
#endif
};


//【二次元領域加算フェニック木（Z-加群）】
/*
* Lazy_fenwick_tree_2D<S, op, o, inv, mul>(int h, int w) : O(h w)
*	要素数 h * w かつ初期値 o で初期化する．
*	要素は Z-加群 (S, op, o, inv, mul) の元とする．
*
* Lazy_fenwick_tree_2D<S, op, o, inv, mul>(vvS a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する．
*
* apply(int x, int y, S val) : O(log h log w)
*	v[x][y] = op(v[x][y], val) とする．
*
* apply(int x1, int y1, int x2, int y2, S val) : O(log h log w)
*	v[x1..x2)[y1..y2) = op(v[x1..x2)[y1..y2), val) とする．
*
* set(int x, int y, S val) : O(log h log w)
*	v[x][y] = val とする．
*
* S get(int x, int y) : O(log h log w)
*	v[x][y] を返す．
*
* S prod(int x1, int y1, int x2, int y2) : O(log h log w)
*	op( v[x1..x2)[y1..y2) ) を返す．空なら o() を返す．
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S), S(*mul)(ll, S)>
struct Lazy_fenwick_tree_2D {
	// verify : https://codeforces.com/contest/869/problem/E

	// ノードの個数（要素数 + 1）
	int h, w;

	// op( [1..i][1..j] ) を acc0[i][j] + i acc1[i][j] + j acc2[i][j] + i j acc3[i][j] と分解する．
	// さらに acc?[i][j] = Σraw?[1..i][1..j] と表されるような raw? を導入する．
	// v[?][i][j] : Σraw?[*..i][*..j] の値（i, j ： 1-indexed，v[*][0], v[0][*] は使わない）
	using vS = vector<S>; using vvS = vector<vS>; using vvvS = vector<vvS>;
	vvvS v;

	// 要素数 h * w かつ初期値 e で初期化
	Lazy_fenwick_tree_2D(int h_, int w_) : h(h_ + 1), w(w_ + 1), v(4, vvS(h, vS(w, o()))) {}

	// 配列 a で初期化
	Lazy_fenwick_tree_2D(const vector<vector<S>>& v_) : h(sz(v_) + 1), w(sz(v_[0]) + 1), v(4, vvS(h, vS(w, o()))) {
		// 配列の値を仮登録する．
		rep(i, h - 1) {
			rep(j, w - 1) {
				v[0][i + 1][j + 1] = v_[i][j];
			}
		}

		// 正しい値になるよう根に向かって累積 op() をとっていく．
		// j 方向
		repi(i, 1, h - 1) {
			for (int pow2 = 1; 2 * pow2 < w; pow2 *= 2) {
				for (int j = 2 * pow2; j < w; j += 2 * pow2) {
					v[0][i][j] = op(v[0][i][j], v[0][i][j - pow2]);
				}
			}
		}
		// i 方向
		repi(j, 1, w - 1) {
			for (int pow2 = 1; 2 * pow2 < h; pow2 *= 2) {
				for (int i = 2 * pow2; i < h; i += 2 * pow2) {
					v[0][i][j] = op(v[0][i][j], v[0][i - pow2][j]);
				}
			}
		}
	}

	// v[x][y] = val とする．（x, y : 0-indexed）
	void set(int x, int y, S val) {
		// 差分を求める．
		S d = op(val, inv(get(x, y)));

		apply(x, y, d);
	}

	// v[x][y] を返す．（x, y : 0-indexed）
	S get(int x, int y) const {
		return prod(x, y, x + 1, y + 1);
	}

	// op( v[x1..x2)[y1..y2) ) を返す．空なら o を返す．（x1, y1, x2, y2 : 0-indexed）
	S prod(int x1, int y1, int x2, int y2) const {
		// 0-indexed での半開長方形 [x1..x2) * [y1..y2) は，
		// 1-indexed での閉長方形 [x1+1..x2] * [y1+1..y2] に対応する．
		S res = o();
		res = op(res, prod_sub(x2, y2));
		res = op(res, inv(prod_sub(x2, y1)));
		res = op(res, inv(prod_sub(x1, y2)));
		res = op(res, prod_sub(x1, y1));
		return res;
	}

	// op( v[1..x][1..y] ) を返す．空なら o を返す．（x, y : 1-indexed）
	S prod_sub(int x, int y) const {
		S res = o();
		res = op(res, prod_sub(x, y, 0));
		res = op(res, mul(x, prod_sub(x, y, 1)));
		res = op(res, mul(y, prod_sub(x, y, 2)));
		res = op(res, mul((ll)x * y, prod_sub(x, y, 3)));
		return res;
	}

	// op( v[d][1..x][1..y] ) を返す．空なら o を返す．（x, y : 1-indexed）
	S prod_sub(int x, int y, int d) const {
		S res = o();

		// 子に向かって累積 op() をとっていく．
		// i, j の最下位ビットから 1 を減算することで次の位置を得る．
		for (int i = x; i > 0; i -= i & -i) {
			for (int j = y; j > 0; j -= j & -j) {
				res = op(res, v[d][i][j]);
			}
		}

		return res;
	}

	// v[x][y] = op(v[x][y], val) とする．（x, y : 0-indexed）
	void apply(int x, int y, S val) {
		// x, y を 1-indexed に直す．
		x++; y++;

		apply_sub(x, y, val, 0);
	}

	// v[x1..x2)[y1..y2) = op(v[x1..x2)[y1..y2), val) とする．（x, y : 0-indexed）
	void apply(int x1, int y1, int x2, int y2, S val) {
		// 0-indexed での半開長方形 [x1..x2) * [y1..y2) は，
		// 1-indexed での閉長方形 [x1+1..x2] * [y1+1..y2] に対応する．
		x1++; y1++;

		// 区間の端の値を調整する．
		apply_sub(x1, y1, mul((ll)(x1 - 1) * (y1 - 1), val), 0);
		apply_sub(x1, y2 + 1, mul((ll)(x1 - 1) * y2, inv(val)), 0);
		apply_sub(x2 + 1, y1, mul((ll)x2 * (y1 - 1), inv(val)), 0);
		apply_sub(x2 + 1, y2 + 1, mul((ll)x2 * y2, val), 0);

		apply_sub(x1, y1, mul(y1 - 1, inv(val)), 1);
		apply_sub(x1, y2 + 1, mul(y2, val), 1);
		apply_sub(x2 + 1, y1, mul(y1 - 1, val), 1);
		apply_sub(x2 + 1, y2 + 1, mul(y2, inv(val)), 1);

		apply_sub(x1, y1, mul(x1 - 1, inv(val)), 2);
		apply_sub(x1, y2 + 1, mul(x1 - 1, val), 2);
		apply_sub(x2 + 1, y1, mul(x2, val), 2);
		apply_sub(x2 + 1, y2 + 1, mul(x2, inv(val)), 2);

		apply_sub(x1, y1, val, 3);
		apply_sub(x1, y2 + 1, inv(val), 3);
		apply_sub(x2 + 1, y1, inv(val), 3);
		apply_sub(x2 + 1, y2 + 1, val, 3);
	}

	// v[d][x][y] = op(v[d][x][y], val) とする．（x, y : 1-indexed）
	void apply_sub(int x, int y, S val, int d) {
		// 根に向かって値を op() していく．
		// i, j の最下位ビットに 1 を加算することで次の位置を得る．
		for (int i = x; i < h; i += i & -i) {
			for (int j = y; j < w; j += j & -j) {
				v[d][i][j] = op(v[d][i][j], val);
			}
		}
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Lazy_fenwick_tree_2D& ft) {
		rep(x, ft.h - 1) {
			rep(y, ft.w - 1) {
				os << ft.get(x, y) << " ";
			}
			cout << "\n";
		}
		return os;
	}
#endif
};


//【動的二次元フェニック木（アーベル群）】（遅い）
/*
* Dynamic_fenwick_tree_2D<S, op, o, inv>(int h, int w) : O(h)
*	要素数 h * w かつ初期値 o() で初期化する．
*	要素はアーベル群 (S, op, o, inv) の元とする．
*
* add(int x, int y, S val) : O(log h log w)
*	v[x][y] += val とする．
*
* set(int x, int y, S val) : O(log h log w)
*	v[x][y] = val とする．
*
* S get(int x, int y) : O(log h log w)
*	v[x][y] を返す．
*
* S sum(int x, int y) : O(log h log w)
*	Σv[0..x)[0..y) を返す．空なら o() を返す．
*
* S sum(int x1, int y1, int x2, int y2) : O(log h log w)
*	Σv[x1..x2)[y1..y2) を返す．空なら o() を返す．
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
struct Dynamic_fenwick_tree_2D {
	// h : 行数 + 1, w : 列数 + 1
	int h, w;

	// v[x][y] : Σv[*..x][*..y] の値（x, y ： 1-indexed）
	vector<unordered_map<int, S>> v;

	// 要素数 h * w かつ初期値 o() で初期化
	Dynamic_fenwick_tree_2D(int h_, int w_) : h(h_ + 1), w(w_ + 1), v(h) {}

	// v[x][y] = val とする．（x, y : 0-indexed）
	void set(int x, int y, S val) {
		// 差分を求める．
		S d = op(val, inv(get(x, y)));

		add(x, y, d);
	}

	// v[x][y] を返す．（x, y : 0-indexed）
	S get(int x, int y) const {
		return sum(x, y, x + 1, y + 1);
	}

	// Σv[0..x)[0..y) を返す．空なら o() を返す．（x, y : 0-indexed）
	S sum(int x, int y) const {
		// verify : https://atcoder.jp/contests/abc266/tasks/abc266_h

		S res = o();

		// 子に向かって累積和をとっていく．
		// i, j の最下位ビットから 1 を減算することで次の位置を得る．
		for (int i = x; i > 0; i -= i & -i) {
			for (int j = y; j > 0; j -= j & -j) {
				const auto it = v[i].find(j);
				if (it != v[i].end()) res = op(res, it->second);
			}
		}

		return res;
	}

	// Σv[x1..x2)[y1..y2) を返す．空なら o() を返す．（x1, y1, x2, y2 : 0-indexed）
	S sum(int x1, int y1, int x2, int y2) const {
		S res = o();

		// sum(x, y) を 4 つ組合せてもいいが，定数倍改善のため場合わけで実装する．
		while (x1 != x2) {
			if (x1 < x2) {
				int j1 = y1, j2 = y2;
				while (j1 != j2) {
					if (j1 < j2) {
						const auto it = v[x2].find(j2);
						if (it != v[x2].end()) res = op(res, it->second);
						j2 -= j2 & -j2;
					}
					else {
						const auto it = v[x2].find(j1);
						if (it != v[x2].end()) res = op(res, inv(it->second));
						j1 -= j1 & -j1;
					}
				}
				x2 -= x2 & -x2;
			}
			else {
				int j1 = y1, j2 = y2;
				while (j1 != j2) {
					if (j1 < j2) {
						const auto it = v[x1].find(j2);
						if (it != v[x1].end()) res = op(res, inv(it->second));
						j2 -= j2 & -j2;
					}
					else {
						const auto it = v[x1].find(j1);
						if (it != v[x1].end()) res = op(res, it->second);
						j1 -= j1 & -j1;
					}
				}
				x1 -= x1 & -x1;
			}
		}

		return res;
	}

	// v[x][y] += val とする．（x, y : 0-indexed）
	void add(int x, int y, S val) {
		// verify : https://atcoder.jp/contests/abc266/tasks/abc266_h

		// x, y を 1-indexed に直す．
		x++; y++;

		// 根に向かって値を足していく．
		// i, j の最下位ビットに 1 を加算することで次の位置を得る．
		for (int i = x; i < h; i += i & -i) {
			for (int j = y; j < w; j += j & -j) {
				auto it = v[i].find(j);
				if (it != v[i].end()) it->second = op(it->second, val);
				else v[i][j] = val;
			}
		}
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Dynamic_fenwick_tree_2D ft) {
		rep(x, ft.h) {
			rep(y, ft.w) {
				os << ft.get(x, y) << " ";
			}
			cout << "\n";
		}
		return os;
	}
#endif
};


//【フェニック木（ACL 仕様）】
/*
* fenwick_tree(n) : O(n)
*	要素数 n かつ初期値 0 で初期化する．
*
* add(p, val) : O(log n)
*	v[p] に val を加算する．
*
* sum(l, r) : O(log n)
*	半開区間 [l, r) の要素の総和を返す．
*/
#ifndef _MSC_VER
template <class T>
struct fenwick_tree {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_6_A

	int n;
	vector<T> v;

	fenwick_tree() {}
	fenwick_tree(int n_) : n(n_ + 1), v(n) {}
	void add(int p, T val) {
		p++;
		while (p < n) {
			v[p] += val;
			p += p & -p;
		}
	}
	T sum_sub(int r) const {
		T res = 0;
		while (r > 0) {
			res += v[r];
			r -= r & -r;
		}
		return res;
	}
	T sum(int l, int r) const {
		return sum_sub(r) - sum_sub(l);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const fenwick_tree& ft) {
		rep(i, ft.n - 1) {
			os << ft.sum(i, i + 1) << " ";
		}
		return os;
	}
#endif
};
#endif


