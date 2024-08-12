#pragma once
#include "header.h"
#include "前処理(列).h"
#include "辞書(動的).h"
#include "FPS(mint).h"
// ■■■■■ 数え上げ（区間） ■■■■■


//【特定の index を含む区間の数え上げ（長さごと）】
/*
* [0..n) において i を含む長さ w の区間の個数を c(i,w) とおくと，
*	c(i,w) = min(i, n-w) - max(i+1-w, 0) + 1
* である．これは
*	x = min(i+1, n-i)
* とおくと，
*	c(i,w) = w      (1 ≦ w ≦ x)
*	c(i,w) = x      (x < w ≦ n-x)
*	c(i,w) = n+1-w  (n-x < w ≦ n)
* として w の区分線形関数で表される
* 
* verify : https://mojacoder.app/users/yunipoke/problems/ave-sum-subarray
*/


//【区間端範囲制約を満たす区間の数え上げ】O(n log n)
/*
* [0..n) の閉区間 [l..r] で，l_min[r] ≦ l ≦ l_max[r] かつ
* r_min[l] ≦ r ≦ r_max[l] を満たすものの個数を返す．
*/
ll count_intervals(const vi& l_min, const vi& l_max, const vi& r_min, const vi& r_max) {
	// 参考 : https://betrue12.hateblo.jp/entry/2020/03/28/112326
	// verify : https://yukicoder.me/problems/no/1031

	int n = sz(l_min);

	// r_min_to_ls[r] : r_min[l] = r となる l のリスト
	// r_max_to_ls[r] : r_max[l] = r となる l のリスト
	vvi r_min_to_ls(n), r_max_to_ls(n);

	rep(l, n) {
		int r_min2 = max(l, r_min[l]);

		// 明らかに左端に選べない l は無視する．
		if (r_max[l] < r_min2) continue;

		// r_min[l] < l は r_min[l] = l だったことにして問題ない．
		r_min_to_ls[r_min2].push_back(l);
		r_max_to_ls[r_max[l]].push_back(l);
	}

	// ft_r[l] : いまの r について，区間 [l..r] が許せるか
	fenwick_tree<int> ft(n);

	ll res = 0;

	// 区間の右端 r を昇順に見ていく．
	rep(r, n) {
		// 新たに r = r_min[l] なる l を許す．
		repe(l, r_min_to_ls[r]) ft.add(l, 1);

		// r を右端とする区間の個数を加算する．
		int l_max2 = min(l_max[r], r);
		if (l_min[r] <= l_max2) res += ft.sum(l_min[r], l_max2 + 1);

		// 以降は r = r_max[l] なる l を許さない．
		repe(l, r_max_to_ls[r]) ft.add(l, -1);
	}

	return res;
}


//【和が 0 の区間の数え上げ】O(n)
/*
* 数列 a[0..n) の空でない連続部分列で和が 0 であるものの個数を返す．
*/
template <class T>
ll count_zerosum_intervals(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc023/tasks/agc023_a

	//【方法】
	// 累積和 A[i] = Σa[0..i) の値ごとに個数を数え上げる．
	// Σa[l..r) = 0 ⇔ A[l] = A[r] なので求める個数は三角数の和になる．

	int n = sz(a);

	// acc[i] : Σa[0..i)
	vector<T> acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + a[i];

	// cnt[v] : Σa[0..i) = v となる i の個数（i = 0 を含む）
	unordered_map<T, int> cnt;
	repi(i, 0, n) cnt[acc[i]]++;

	ll res = 0;
	repe(tmp, cnt) {
		ll c = tmp.second;
		res += c * (c - 1) / 2;
	}

	return res;
}


//【和が s 以下の区間の数え上げ（要素が非負）】
/*
* 尺取法.h へ
*/


//【和が s 以上の区間の数え上げ】O(n log n)
/*
* 整数列 a[0..n)（負も可）で，Σa[l..r) ≧ s となる非空区間の個数を返す．
*
* 利用：【binary trie】
*/
ll count_sum_intervals_neg(const vl& a, ll s) {
	// verify : https://www.spoj.com/problems/MEANARR/

	//【方法】
	// 累積和を A[i] = Σa[0..i) とおくと，Σa[l..r) ≧ s ⇔ A[l] ≦ A[r] - s なので，
	// r を昇順走査し，A[r] - s 以下の要素の個数を足し込んでいけばいい．
	// A[0..n] に負の数があると困るので適当に下駄を履かせておく．

	int n = sz(a);

	vl A(n + 1);
	rep(i, n) A[i + 1] = A[i] + a[i];

	ll A_min = *min_element(all(A));
	repi(i, 0, n) A[i] -= A_min;

	Binary_trie<> bt;
	bt.insert(A[0]);

	ll res = 0;
	repi(r, 1, n) {
		res += bt.upper_bound(A[r] - s);
		bt.insert(A[r]);
	}

	return res;
}


//【最小値ごとの区間の数え上げ】O(n)
/*
* 数列 a[0..n) の最小値が m の区間 a[l..r) (l<r) の個数 c を {m, c} の形で格納したリストを返す．
*
* 利用：【デカルト木】
*/
template <class T>
vector<pair<T, ll>> count_min_intervals(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc005/tasks/agc005_b

	//【方法】
	// min(a[0..n)) = m で a[i] = m であるとする．a[i] を含むような区間 [l, r) は，
	// l∈[0..i] かつ r∈(i..n] であるような区間の全てであり，min(a[l..r)) = m である．
	// これと同様のことをデカルト木の各ノードについて行えば良い．

	int n = sz(a);
	unordered_map<T, ll> cnt;

	Cartesian_tree g(a);
	rep(s, n) cnt[g[s].val] += (ll)(s - g[s].l + 1) * (g[s].r - s);

	vector<pair<T, ll>> res;
	repe(tmp, cnt) res.push_back(tmp);

	return res;
}


//【区間被覆の数え上げ】O(n log n + m + n log m)
/*
* 与えられた [0..m) 上の n 個の区間 [l[i]..r[i]) に対し，
* 全区間 [0..m) を被覆するような区間の選び方が何通りあるかを返す．
*/
mint op_cic(mint x, mint y) { return x + y; }
mint e_cic() { return 0; }
mint count_interval_covering(int m, const vi& l, const vi& r) {
	int n = sz(l);

	// 右端の小さい順にソートする．
	vector<pii> rl(n);
	rep(i, n) rl[i] = { r[i], l[i] };
	sort(all(rl));

	// seg_i[j] : [0..i) 番目の区間で [0..j) を被覆する方法の数
	segtree<mint, op_cic, e_cic> seg(m + 1);
	seg.set(0, 1);

	// 貰う DP
	rep(i, n) {
		int r, l;
		tie(r, l) = rl[i];

		// cnt : 今までの区間で [0..l) を被覆する方法の数
		mint cnt = seg.prod(l, m + 1);

		// これに [l..r) を重ねれば [0..r) を被覆することができる．
		seg.set(r, seg.get(r) + cnt);
	}

	return seg.get(m);
}


//【全体を被覆する自由区間の数え上げ（mod 998244353）】O(N log N)
/*
* [1..2n]∩Z を端点にもつ互いに端点を共有しない n 個の閉区間で，
* 全区間 [1..2n] を被覆するものの個数を c[n] とする（c[0] = 0）．c[0..N] を返す．
*
* 利用：【形式的冪級数】
*/
vm count_connected_colored_parenthesis(int N) {
	// verify : https://mojacoder.app/users/googol_S0/problems/n-cut-n-eat

	//【方法】
	// 被覆条件を無視したときの個数を b[n] とすれば，2n 個の整数を n 個の 2 つ組に分ければよいので，
	//		b[n] = (2n)! / (2^n n!) = (2n-1)!!
	// である．被覆条件を満たしている区間に分割して考えることにより，
	//		b[n] = Boole[n=0] + c[n] + Σ_(i+j=n) c[i] c[j] + Σ_(i+j+k=n) c[i] c[j] c[k] + ...
	// なる等式を得る．これを母関数で表現すると，
	//		B(z) = 1 + C(z) + C(z)^2 + C(z)^3 + ...
	//			 = 1/(1-C(z))
	// となるので，C(z) について解くと
	//		C(z) = 1 - 1/B(z)
	// を得る．

	MFPS B(0, N + 1);
	B[0] = -1;
	repi(n, 1, N) B[n] = B[n - 1] * (2 * n - 1);

	MFPS C = B.inv(N + 1);
	C[0] = 0;

	return C.c;
}


