#pragma once
#include "header.h"
#include "座標圧縮.h"
#include "二項係数.h"
#include "辞書(動的).h"
#include "前処理(列).h"
// ■■■■■ 順列，置換など ■■■■■


//【逆引き順列】
/*
* Permutation_rev(vi p) : O(n)
*	[0..n) の順列 p[0..n) で初期化する．
*
* int get(int i) : O(1)
*	p[i] の値を返す．
*
* int pos(int x) : O(1)
*	値 x の位置を返す．
*
* swap_PP(int i, int j) : O(1)
*	p[i] の値と p[j] の値を入れ替える．
*
* swap_PV(int i, int y) : O(1)
*	p[i] の値と値 y を入れ替える．
*
* swap_VV(int x, int y) : O(1)
*	値 x と値 y を入れ替える．
*/
class Permutation_rev {
	int n;
	vi p, p_inv;

public:
	Permutation_rev(const vi& p) : n(sz(p)), p(p), p_inv(n) {
		// verify : https://atcoder.jp/contests/abc350/tasks/abc350_c

		rep(i, n) p_inv[p[i]] = i;
	}
	Permutation_rev() : n(0) {}

	// p[i] の値を返す．
	int get(int i) {
		// verify : https://atcoder.jp/contests/abc350/tasks/abc350_c

		return p[i];
	}

	// x の位置を返す．
	int pos(int x) {
		// verify : https://atcoder.jp/contests/abc350/tasks/abc350_c

		return p_inv[x];
	}

	// p[i] の値と p[j] の値を入れ替える．
	void swap_PP(int i, int j) {
		// verify : https://atcoder.jp/contests/abc350/tasks/abc350_c

		if (i == j) return;

		int x = p[i], y = p[j];
		p[i] = y;
		p[j] = x;
		p_inv[y] = i;
		p_inv[x] = j;
	}

	// p[i] の値と値 y を入れ替える．
	void swap_PV(int i, int y) {
		// verify : https://atcoder.jp/contests/abc350/tasks/abc350_c

		if (p[i] == y) return;

		int x = p[i], j = p_inv[y];
		p[i] = y;
		p[j] = x;
		p_inv[y] = i;
		p_inv[x] = j;
	}

	// 値 x と値 y を入れ替える．
	void swap_VV(int x, int y) {
		// verify : https://atcoder.jp/contests/abc350/tasks/abc350_c

		if (x == y) return;

		int i = p_inv[x], j = p_inv[y];
		p[i] = y;
		p[j] = x;
		p_inv[y] = i;
		p_inv[x] = j;
	}
};


//【転倒数】O(n log n)
/*
* a[0..n) の転倒数を返す．
*/
template <class T>
ll inversion_number(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_ef

	int n = sz(a);

	// 値 a[i] と位置 i を組にしソートする．
	vector<pair<T, int>> ai(n);
	rep(i, n) ai[i] = { a[i], i };
	sort(all(ai));

	ll res = 0;

	// ft[i] : いままでに位置 i の要素が現れたか
	fenwick_tree<int> ft(n);

	// 値について昇順に見ていく．
	rep(j, n) {
		// pos : 昇順で j 番目の値の位置
		int pos = ai[j].second;

		// pos より右に j 未満の要素が今までに何個あったかを加算する．
		res += ft.sum(pos + 1, n);

		// 位置 pos の要素の出現を記録する．
		ft.add(pos, 1);
	}

	return res;
}


//【転倒数（座圧）】O(n log n)
/*
* a[0..n) の転倒数を返す．
*/
template <class T>
ll inversion_number_cc(const vector<T>& a) {
	// verify : https://onlinejudge.u-aizu.ac.jp/services/room.html#ACPC2021Day3/problems/F

	int n = sz(a);

	vector<T> a_uniq(a);
	uniq(a_uniq);

	int m = sz(a_uniq);

	// fw[i] : 今まで見てきた範囲に値 i が何個あったか
	fenwick_tree<int> fw(m);
	ll res = 0;

	// 位置について昇順に見ていく．
	rep(i, n) {
		int b = lbpos(a_uniq, a[i]);

		fw.add(b, 1);

		// 自身より大きい数が今までに何個あったか調べ，加算する．
		res += fw.sum(b + 1, m);
	}

	return res;
}


//【逆順列の転倒数】
/*
* 順列 p[0..n) の逆順列が q[0..n) であるとき，inv(p) = inv(q) である．
*/


//【最小 swap 回数】O(n log n)
/*
* 数列 a[0..n) に対し，隣接要素の swap を繰り返すことで b[0..n) に変化させるときの
* swap の最小回数を返す（不可能なら INFL）
*
* 利用：【転倒数】
*/
template <class T>
ll swap_distance(const vector<T>& a, const vector<T>& b) {
	// verify : https://atcoder.jp/contests/arc120/tasks/arc120_c

	int n = sz(a);

	// 長さが異なるならもちろん不可能
	if (sz(b) != n) return INFL;

	vector<pair<T, int>> ai(n), bi(n);
	rep(i, n) {
		ai[i] = { a[i], i };
		bi[i] = { b[i], i };
	}

	// 安定ソートになるので，左同士，右同士を対応付けてくれる．
	sort(all(ai));
	sort(all(bi));

	// p[ia] = ib : a[ia] を b[ib] と対応させることを表す．
	vi p(n);
	rep(i, n) {
		auto& [a, ia] = ai[i];
		auto& [b, ib] = bi[i];
		if (a != b) return INFL;

		p[ia] = ib;
	}

	// 順列 p[0..n) の転倒数が答え．
	return inversion_number(p);
}


//【置換 → 巡回置換の積】O(n)
/*
* [0..n) の置換 i → p[i] を巡回置換の積に分解し，巡回置換表記のリストを返す．
*/
vvi cycle_decomposition(const vi& p) {
	// verify : https://atcoder.jp/contests/abc175/tasks/abc175_d

	int n = sz(p);

	vvi cycles;
	vb seen(n);

	rep(i, n) {
		// 抽出済のサイクルに含まれるなら次へ
		if (seen[i]) continue;

		// 新しいサイクルを発見
		cycles.push_back(vi());

		// サイクルを順に格納していく．
		int s = i;
		do {
			cycles.rbegin()->push_back(s);
			seen[s] = true;
			s = p[s];
		} while (s != i);
	}
	
	return cycles;
}


//【置換 → 隣接互換の積】O(n^2)
/*
* [0..n) の置換 i → p[i] を隣接互換 (x, x+1) の積に分解し，x のリストを返す．
*/
vi adjacent_transposition_decomposition(vi p) {
	int n = sz(p);
	vi res;

	while (true) {
		// 要素の交換を行ったかどうか
		bool swapped = false;

		repir(j, n - 2, 0) {
			// 隣接要素の大小関係が逆転していたら交換する．
			if (p[j] > p[j + 1]) {
				swap(p[j], p[j + 1]);
				res.emplace_back(j);
				swapped = true;
			}
		}

		// 要素の交換が行われなくなったら終了する．
		if (!swapped) break;
	}
	reverse(all(res));

	return res;
}


//【順列全探索 → bitDP】O(2^n n)
/*
* a[0..n) に対して，1 回の操作でコスト x で a[i]++, a[i]--，コスト y で swap(a[i], a[i+1])
* が行えるとき，a[0..n) を b[0..n) に一致させるための最小コストを返す．
*/
ll minimize_inc_dec_swap_cost(const vl& a, const vl& b, ll x, ll y) {
	// verify : https://atcoder.jp/contests/abc232/tasks/abc232_f

	int n = sz(a);

	// a のどの要素を b のどの要素に対応させるかで決め打ち順列全探索を行えば，
	// コストが (要素の差の和) x + (順列の転倒数) y であることは容易に分かる．
	// これで間に合わない場合でも，b に対応させていく順番を固定することで bitDP を用いて高速化できる．

	// dp[set] : a[set] を b[0..|set|) に一致させるための最小コスト
	vl dp(1LL << n, INFL);
	dp[0] = 0;

	repb(set, n) {
		if (set == 0) continue;

		// set_size : 添字集合 set の大きさ
		int set_size = popcount(set);

		ll swap_cost = (set_size - 1) * y;

		// 添字集合 set の要素 i を昇順に走査する．
		rep(i, n) {
			if (set & (1 << i)) {
				int n_set = set - (1 << i);
				ll inc_dec_cost = abs(a[i] - b[set_size - 1]) * x;

				chmin(dp[set], dp[n_set] + inc_dec_cost + swap_cost);

				swap_cost -= y;
			}
		}
	}

	return dp[(1 << n) - 1];
}


//【階乗進法】
/*
* 混合基数.h へ
*/


//【転倒数（分割統治）】O(n (log n)^2)
/*
* a[0..n) の転倒数を返す．
*
* 利用：【中央値で部分列に分解】
*/
template <class T>
ll inversion_number_01sep(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/chokudai_s001/tasks/chokudai_S001_j

	//【方法】
	// 中央値で分割して 01 列にし，01 列としての転倒数を数える．
	// 0,1 それぞれの部分列の転倒数は再帰的に求める．

	ll res = 0;

	function<void(const vector<T>&)> rf = [&](const vector<T>& a) {
		int n = sz(a);

		vi la, ua, lp, up;
		if (!median_separate(a, la, ua, lp, up)) return;

		vi lu(n);
		repe(x, up) lu[x] = 1;

		ll cnt = 0;
		rep(i, n) {
			if (lu[i] == 1) cnt++;
			else res += cnt;
		}

		rf(la);
		rf(ua);
	};

	rf(a);

	return res;
}


