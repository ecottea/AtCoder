#pragma once
#include "header.h"
// ■■■■■ 尺取り法 ■■■■■


//【最大区間長】O(n)
/*
* 非負整数列 a[0..n) で，Σa[l..r) <= s となる連続部分列の長さの最大値 r - l を返す．
*
*（尺取り法）
*/
int maximize_interval_length(const vl& a, ll s) {
	int n = sz(a);

	// 空和は 0 とする．
	int res = 0;

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// sum : Σa[l..r)
	ll sum = 0;

	while (true) {
		// Σa[l..r) <= s の場合
		if (sum <= s) {
			// r で場合分けして考える．
			// いまの l は固定された r に対して最小の l となっているので，
			// いまの r を右端にもつ最長区間の長さは r - l である．
			chmax(res, r - l);

			// 走査完了
			if (r == n) break;

			// 右を 1 つ進める．
			sum += a[r];
			r++;

		}
		// Σa[l..r) > s の場合
		else {
			// 左を 1 つ進める．
			sum -= a[l];
			l++;
		}
	}

	return res;
}


//【最大区間長（先読み）】O(n)
/*
* 非負整数列 a[0..n) で，Σa[l..r) <= s となる連続部分列の長さの最大値 r - l を返す．
*
*（尺取り法）
*/
int maximize_interval_length_la(const vl& a, ll s) {
	int n = sz(a);

	// 空和は 0 とする．
	int res = 0;

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// sum : Σa[l..r)
	ll sum = 0;

	// 常に条件を満たした状態であるよう，先読みを行いながら走査する．
	while (r < n) {
		// Σa[l..r) + a[r] <= s の場合
		if (sum + a[r] <= s) {
			// 右を 1 つ進める．
			sum += a[r];
			r++;

			// r で場合分けして考える．
			// いまの l は固定された r に対して最小の l となっているので，
			// いまの r を右端にもつ最長区間の長さは r - l である．
			chmax(res, r - l);

		}
		// Σa[l..r) + a[r] > s の場合
		else {
			// 左を 1 つ進める．
			sum -= a[l];
			l++;
		}
	}

	return res;
}


//【最小区間長】O(n)
/*
* 非負整数列 a[0..n) で，Σa[l..r) >= s となる連続部分列の長さの最小値 r - l を返す．
* 存在しないなら INF を返す．
*
*（尺取り法）
*/
int minimize_interval_length(const vl& a, ll s) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_3_A

	// 空和は 0 とする．
	if (s == 0) return 0;

	int n = sz(a);

	int res = INF;

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// sum : Σa[l..r)
	ll sum = 0;

	while (true) {
		// Σa[l..r) >= s の場合
		if (sum >= s) {
			// 条件を満たしているので，最小区間長を更新する．
			chmin(res, r - l);

			// 左を 1 つ進める．
			sum -= a[l];
			l++;

		}
		// Σa[l..r) < s の場合
		else {
			// 走査完了
			if (r == n) break;

			// 右を 1 つ進める．
			sum += a[r];
			r++;
		}
	}

	return res;
}


//【区間の数え上げ】O(n)
/*
* 非負整数列 a[0..n) で，Σa[l..r) <= s となる連続部分列 l < r の個数を返す．
*
*（尺取り法）
*/
ll count_intervals(const vl& a, ll s) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_3_C

	int n = sz(a);

	ll res = 0;

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// sum : Σa[l..r)
	ll sum = 0;

	while (true) {
		// Σa[l..r) <= s の場合
		if (sum <= s) {
			// r で場合分けして数え上げる．
			// いまの l は固定された r に対して最小の l となっているので，
			// いまの r を右端にもつ区間は r - l 個存在する．
			res += r - l;

			// 走査完了
			if (r == n) break;

			// 右を 1 つ進める．
			sum += a[r];
			r++;

		}
		// Σa[l..r) > s の場合
		else {
			// 左を 1 つ進める．
			sum -= a[l];
			l++;
		}
	}

	return res;
}


//【区間の数え上げ（先読み）】O(n)
/*
* 非負整数列 a[0..n) で，Σa[l..r) <= s となる連続部分列 l < r の個数を返す．
*
*（尺取り法）
*/
ll count_interval_la(const vl& a, ll s) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_3_C

	int n = sz(a);

	ll res = 0;

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// sum : Σa[l..r)
	ll sum = 0;

	// 常に条件を満たした状態であるよう，先読みを行いながら走査する．
	while (r < n) {
		// Σa[l..r) + a[r] <= s の場合
		if (sum + a[r] <= s) {
			// 右を 1 つ進める．
			sum += a[r];
			r++;

			// r で場合分けして数え上げる．
			// いまの l は固定された r に対して最小の l となっているので，
			// いまの r を右端にもつ区間は r - l 個存在する．
			res += r - l;

		}
		// Σa[l..r) + a[r] > s の場合
		else {
			// 左を 1 つ進める．
			sum -= a[l];
			l++;
		}
	}

	return res;
}


//【最大区間長（先読み，連想配列）】O(n)
/*
* 非負整数列 a[0..n) で，要素に重複のない連続部分列の長さの最大値 r - l を返す．
*
*（尺取り法）
*/
int maximize_interval_length_la_aa(const vi& a) {
	int n = sz(a);

	// 空和は 0 とする．
	int res = 0;

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// elem : a[l..r) に含まれる要素の集合
	unordered_set<int> elem;

	// 常に条件を満たした状態であるよう，先読みを行いながら走査する．
	while (r < n) {
		// a[l..r) に a[r] が含まれない場合
		if (!elem.count(a[r])) {
			// 右を 1 つ進める．
			elem.insert(a[r]);
			r++;

			// r で場合分けして考える．
			// いまの l は固定された r に対して最小の l となっているので，
			// いまの r を右端にもつ最長区間の長さは r - l である．
			chmax(res, r - l);
		}
		// a[l..r) に a[r] が含まれる場合
		else {
			// 左を 1 つ進める．
			elem.erase(a[l]);
			l++;
		}
	}

	return res;
}


//【最小区間長（連想配列）】O(n)
/*
* 非負整数列 a[0..n) で，[0..k) を全て含む連続部分列の長さの最小値 r - l を返す．
* 存在しないなら INF を返す．
*
*（尺取り法）
*/
int minimize_interval_length_aa(const vi& a, int k) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_3_B

	int n = sz(a);

	int res = INF;

	// l, r : a[l..r) を走査中であることを表す．
	int l = 0, r = 0;

	// cnt[i] : a[l..r) に含まれる i の個数
	unordered_map<int, int> cnt;

	while (true) {
		// a[l..r) に [0..k) が全て含まれる場合
		if (sz(cnt) == k) {
			// 条件を満たしているので，最小区間長を更新する．
			chmin(res, r - l);

			// 左を 1 つ進める．
			if (a[l] <= k) {
				cnt[a[l]]--;
				if (cnt[a[l]] == 0) cnt.erase(a[l]);
			}
			l++;

		}
		// a[l..r) に [0..k) の少なくとも 1 つの要素が含まれない場合
		else {
			// 走査完了
			if (r == n) break;

			// 右を 1 つ進める．
			if (a[r] <= k) {
				cnt[a[r]]++;
			}
			r++;
		}
	}

	return res;
}


