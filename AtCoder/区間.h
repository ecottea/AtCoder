#pragma once
#include "header.h"
// ■■■■■ 区間 ■■■■■


//【尺取り法】O(n β)
/*
* 与えられた列 a[0..n) と連続部分列に対する判定関数 is_ok について，
* 各 l∈[0..n] について is_ok(a[l..r))=true となる最大の r≦n を max_right[l] に，
* 各 r∈[0..n] について is_ok(a[l..r))=true となる最小の l≧0 を min_left[r] にそれぞれ格納する．
*
* 制約：is_ok(φ)=true，is_ok は単調，右追加と左削除が O(β) で可能
*/
template <class T>
void two_pointers(const vector<T>& a, vi& max_right, vi& min_left) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_ck

	int n = sz(a);
	max_right.resize(n + 1); min_left.resize(n + 1);

	// -------------- ここを実装する（auto の方が速い） ---------------

	// 必要なデータ構造を用意する．
	int m = *max_element(all(a)) + 1;
	fenwick_tree<int> ft(m);
	ll inv = 0;

	// 区間の右に a[i] を追加し，データ構造を更新する．
	auto insert_right = [&](int i) {
		inv += ft.sum(a[i] + 1, m);
		ft.add(a[i], 1);
	};

	// 区間の左から a[i] を削除し，データ構造を更新する．
	auto erase_left = [&](int i) {
		inv -= ft.sum(0, a[i]);
		ft.add(a[i], -1);
	};

	// データ構造を参照して ok かを返す．
	auto is_ok = [&]() {
		return inv <= 12345;
	};

	// --------------------------------------------------------------

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	while (true) {
		// is_ok( a[l..r) ) = true の場合
		if (is_ok()) {
			// いまの l は固定された r に対して最小の l となっている．
			min_left[r] = l;

			// 走査完了
			if (r == n) break;

			// 右を 1 つ進める．
			insert_right(r++);
		}
		// is_ok( a[l..r) ) = false の場合
		else {
			// いまの r は固定された l に対して最大の r より 1 だけ大きい．
			max_right[l] = r - 1;

			// 左を 1 つ進める．
			erase_left(l++);
		}
	}

	// いま is_ok( a[l..n) ) = true なので，l をより大きくしても true となる．
	for (; l <= n; l++) max_right[l] = n;
}


//【尺取り法（群）】O(n)
/*
* 群 (S, op, e, inv) の元を要素とする与えられた列 a[0..n) について，
* 各 l∈[0..n] について f(Πa[l..r)) = true となる最大の r≦n を max_right[l] に，
* 各 r∈[0..n] について f(Πa[l..r)) = true となる最小の l≧0 を min_left[r] にそれぞれ格納する．
*
* 制約：f(e()) = true，f は単調
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
void two_pointers(const vector<S>& a, const function<bool(S)>& f, vi& max_right, vi& min_left) {
	// verify : https://atcoder.jp/contests/abc130/tasks/abc130_d

	int n = sz(a);
	max_right.resize(n + 1); min_left.resize(n + 1);

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// val : Πa[l..r)
	S val = e();

	while (true) {
		// f( Πa[l..r) ) = true の場合
		if (f(val)) {
			// いまの l は固定された r に対して最小の l となっている．
			min_left[r] = l;

			// 走査完了
			if (r == n) break;

			// 右を 1 つ進める．
			val = op(val, a[r++]);
		}
		// f( Πa[l..r) ) = false の場合
		else {
			// いまの r は固定された l に対して最大の r より 1 だけ大きい．
			max_right[l] = r - 1;

			// 左を 1 つ進める．
			val = op(inv(a[l++]), val);
		}
	}

	// いま f( Πa[l..n) ) = true なので，l をより大きくしても true となる．
	for (; l <= n; l++) max_right[l] = n;
}


//【尺取り法（モノイド）】O(n)
/*
* モノイド (S, op, e) の元を要素とする与えられた列 a[0..n) について，
* 各 l∈[0..n] について f(Πa[l..r)) = true となる最大の r≦n を max_right[l] に，
* 各 r∈[0..n] について f(Πa[l..r)) = true となる最小の l≧0 を min_left[r] にそれぞれ格納する．
*
* 制約：f(e()) = true，f は単調
*
* 利用：【キュー（モノイド）】
*/
template <class S, S(*op)(S, S), S(*e)(), class FUNC>
void two_pointers(const vector<S>& a, const FUNC& f, vi& max_right, vi& min_left) {
	// verify : https://yukicoder.me/problems/no/1036

	int n = sz(a);
	max_right.resize(n + 1); min_left.resize(n + 1);

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// a[l..r) の要素を入れるキュー
	Queue_SWAG<S, op, e> q;

	while (true) {
		// f( Πa[l..r) ) = true の場合
		if (f(q.prod())) {
			// いまの l は固定された r に対して最小の l となっている．
			min_left[r] = l;

			// 走査完了
			if (r == n) break;

			// 右を 1 つ進める．
			q.push(a[r++]);
		}
		// f( Πa[l..r) ) = false の場合
		else {
			// いまの r は固定された l に対して最大の r より 1 だけ大きい．
			max_right[l] = r - 1;

			// 左を 1 つ進める．
			q.pop(); l++;
		}
	}

	// いま f( Πa[l..n) ) = true なので，l をより大きくしても true となる．
	for (; l <= n; l++) max_right[l] = n;

	/* f の定義の雛形
	using S = ll;
	auto f = [&](S x) {
		return true || false;
	};
	*/
}


//【区間の線形スコア → 差】
/*
* 区間のスコアに線形性があるとき，
*	[l..r) のスコア = [0..r) のスコア - [0..l) のスコア
* となるので，[0..r) のスコアを計算する関数を実装するだけで良い．
* 
* verify : https://atcoder.jp/contests/abc305/tasks/abc305_d
*/


//【連続区間の重なり判定】
/*
* 連続閉区間 [l1..r1] と [l2..r2] が共通部分をもつ ⇔ max(l1, l2) ≦ min(r1, r2)
* verify : https://codeforces.com/contest/1680/problem/A
* 
* 連続半開区間 [l1..r1) と [l2..r2) が共通部分をもつ ⇔ max(l1, l2) < min(r1, r2)
*
* 連続開区間 (l1..r1) と (l2..r2) が共通部分をもつ ⇔ max(l1, l2) < min(r1, r2)
* verify : https://atcoder.jp/contests/arc090/tasks/arc090_c
*/


//【連続区間の重なりの長さ】
/*
* 連続区間 [l1..r1] と [l2..r2] の共通部分の長さは以下の式で与えられる：
*	max(min(r1, r2) - max(l1, l2), 0)
*
* verify : https://atcoder.jp/contests/abc261/tasks/abc261_a
*/


//【区間端の交換】
/*
* a ≦ b ≦ c ≦ d であるとき以下が成り立つ：
*	|c - a| + |d - b| = |d - a| + |c - b|
* 
* verify : https://atcoder.jp/contests/arc120/tasks/arc120_d
*/


//【区間内判定】
/*
* 列 a[0..n) において値 k をもつ全ての要素が区間 [l..r) 内にあることは，
* 値 k をもつ要素の最左位置が l 以上かつ最右位置が r 未満であることと同値である．
* 
* verify : https://atcoder.jp/contests/abc285/tasks/abc285_f
*/


//【ソートの仕方】
/*
* 左から貰う DP をする場合は区間の右端で昇順ソート
* 左から配る DP をする場合は区間の左端で昇順ソート
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

	if (n == 0) return 0;

	// 左端の小さい順にソートする（空の区間は無視する）
	vector<pair<T, T>> lr;
	rep(i, n) if (l[i] < r[i]) lr.emplace_back(l[i], r[i]);
	sort(all(lr));

	n = sz(lr);
	rep(i, n) tie(l[i], r[i]) = lr[i];

	int m = 1;
	l2 = vector<T>{ l[0] };
	r2 = vector<T>{ r[0] };

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
		if (ct[s].left != -1) rf(ct[s].left, pl, s);
		if (ct[s].right != -1) rf(ct[s].right, s + 1, pr);
	};

	rf(ct.root, 0, n);
}


