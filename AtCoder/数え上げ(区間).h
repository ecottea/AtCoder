#pragma once
#include "header.h"
#include "前処理(列).h"
#include "辞書(動的).h"
// ■■■■■ 数え上げ（区間） ■■■■■


//【区間端範囲制約を満たす区間の数え上げ】O(n log n)
/*
* [0..n) の区間 [l..r] (l <= r) で，l_min[r] <= l <= l_max[r] かつ
* r_min[l] <= r <= r_max[l] を満たすものの個数を返す．
*/
ll count_intervals(const vi& l_min, const vi& l_max, const vi& r_min, const vi& r_max) {
	// 参考 : https://betrue12.hateblo.jp/entry/2020/03/28/112326
	// verify : https://yukicoder.me/problems/no/1031

	int n = sz(l_min);

	vvi r_min_to_ls(n), r_max_to_ls(n);
	rep(l, n) {
		// r_max[l] < l であるような条件は満たせないので無視する．
		if (r_max[l] < l) continue;

		// r_min[l] < l は r_min[l] = l だったことにして問題ない．
		r_min_to_ls[max(r_min[l], l)].push_back(l);
		r_max_to_ls[r_max[l]].push_back(l);
	}

	// ft_r[l] : 区間 [l..r] が許せるか
	fenwick_tree<int> ft(n);

	ll res = 0;

	// 区間の右端 r を昇順に見ていく．
	rep(r, n) {
		// 新たに r = r_min[l] なる l を許す．
		repe(l, r_min_to_ls[r]) ft.add(l, 1);

		// r を右端とする区間の個数を加算する．
		if (l_min[r] <= r) res += ft.sum(l_min[r], min(l_max[r], r) + 1);

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


//【最小値ごとの区間の数え上げ】O(n log n)
/*
* 数列 a[0..n) の最小値が m の区間 a[l..r) (l<r) の個数 c を {m, c} の形で格納したリストを返す．
*
* 利用：【自身より小さい数の次の位置】,【自身より小さい数の前の位置】
*/
template <class T>
vector<pair<T, ll>>  count_min_intervals(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc005/tasks/agc005_b

	int n = sz(a);
	unordered_map<T, ll> cnt;

	auto pl = prev_less_position(a);
	auto nl = next_less_position(a);
	auto pleq = prev_less_position(a, true);
	auto nleq = next_less_position(a, true);

	rep(i, n) {
		if (pl[i] == pleq[i]) {
			cnt[a[i]] += (ll)(nl[i] - pl[i] - 1) * (nl[i] - pl[i]) / 2;
			cnt[a[i]] -= (ll)(i - pleq[i] - 1) * (i - pleq[i]) / 2;
		}
		cnt[a[i]] -= (ll)(nleq[i] - i - 1) * (nleq[i] - i) / 2;
	}

	vector<pair<T, ll>> res;
	repe(tmp, cnt) res.push_back(tmp);

	return res;
}


//【最小値ごとの区間の数え上げ】O(n)
/*
* 数列 a[0..n) の最小値が m の区間 a[l..r) (l<r) の個数 c を {m, c} の形で格納したリストを返す．
*
* 利用：【デカルト木】
*/
template <class T>
vector<pair<T, ll>> count_min_intervals_ct(const vector<T>& a) {
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
* 与えられた [0..m) 上の n 個の区間 [l[i], r[i]) に対し，
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


//【隣接和をとる操作で得られる列の数え上げ】
/*
* a[0..n) に対し，隣接する 2 数をその和に置き換えるという操作を考える．
* 操作を 0 回以上の任意回行って得られる列が何通りあるかを返す．
*
*（前処理で高速化した DP）
*/
mint count_adjacent_sum_contraction_seq(const vl& a) {
	// verify : https://atcoder.jp/contests/abc230/tasks/abc230_f

	int n = sz(a);

	//【方法】
	// 列を決めるには，n-1 箇所の隙間それぞれに仕切りを入れるかどうかを決めれば良い．
	// ただし同じ列が得られる場合仕切りは右優先で入れていくものとする．
	// 例えば a[0..5) = (1, 2, -2, 3, -3) から (1, 0) を作るような仕切りの入れ方は
	// (1, 2, -2 | 3, -3) に限定する．
	// これで仕切りの入れ方とできる列との間に一対一の対応ができる．
	//
	// dp[i] を「a[0..i) に対する仕切りの入れ方の数」とし，dp[i+1] を考える．
	// 仕切りを入れて
	//		(... | a[j], ..., a[i-1] | a[i])
	// とすることは常に行える．一方仕切りを入れず
	//		(... | a[j], ..., a[i-1], a[i])
	// とすることは，右優先の規則より
	//		a[j] + ... + a[i-1] = 0
	// となるような j がある場合は行えない．よって遷移式は
	//		dp[i+1] = 2 * dp[i] 
	//			-「a[0..i) に対する仕切りの入れ方で末尾が 0 かつ長さ 2 以上の個数」
	// となる．
	// 
	// a[0..i) に対しても右優先の規則が働いているので，
	//		a[j] + ... + a[i-1] = 0
	// であるならば，
	//		a[j'] + ... + a[j-1] = 0
	// であるような
	//		(... | a[j'], ..., a[j-1] | a[j], ..., a[i-1])
	// という仕切りの入れ方は dp[i] に数えられていない．
	// よって
	//		k = k(i) = max_j(a[j] + ... + a[i-1] = 0)
	// とおけば，求める場合の数は a[0..k) への仕切りの入れ方の数，すなわち dp[k] である．
	// ただし k = 0 のときは長さが 1 の列に対応してしまい，これは引いてはならないので，
	// 便宜上 dp[0] = 0 と定める．（空列のとき 0 通りというのは不自然ではあるが）
	//
	// あとは前処理を行い，各 i に対して
	//		k(i) = max_j(a[j] + ... + a[i-1] = 0)
	// を求めておけば良い．これは，累積和
	//		acc[i] := Σa[0..i)
	// を導入すれば
	//		k(i) = max_j(acc[j] = acc[i])
	// と言い換えられるので，累積和の値 s ごとに
	//		Σa[0..j) = s
	// となるような最大の j を持ちながら k(i) を下から順に求めていくことができる．

	// dp[i] : a[0..i) に対する仕切りの入れ方の数
	vm dp(n + 1);
	dp[0] = 0; dp[1] = 1;

	// k[i] : max_j(acc[j] = acc[i])
	vi k(n + 1);

	// acc_to_k[v] = k は，k = max_j(acc[j] = v) であることを表す．
	unordered_map<ll, int> acc_to_k;

	ll s = 0; // 累積和
	rep(i, n) {
		// いま s = Σa[0..i) = acc[i] なので，
		// Σa[0..j) = acc となる最大の j を k[i] として記録しておく．
		// なお j が存在しない場合は j = 0 となり，dp[0] = 0 なので問題ない．
		k[i] = acc_to_k[s];

		// Σa[0..i) = s であることを記録する．
		acc_to_k[s] = i;

		// 累積和 s を更新する．
		s += a[i];
	}

	// 貰う DP
	repi(i, 1, n - 1) dp[i + 1] = 2 * dp[i] - dp[k[i]];

	return dp[n];
}


