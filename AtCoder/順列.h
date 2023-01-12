#pragma once
#include "header.h"
#include "座標圧縮.h"
#include "二項係数.h"
#include "辞書(動的).h"
#include "前処理(列).h"
// ■■■■■ 順列，置換など ■■■■■


//【転倒数】O(n log n)
/*
* a[0..n) の転倒数を返す．
*/
template <class T>
ll inversion_number(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/arc075/tasks/arc075_c

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


//【転倒数】O(n log n)
/*
* a[0..n) の転倒数を返す．
*
* 利用：【座標圧縮】
*/
template <class T>
ll inversion_number_cc(const vector<T>& a) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_5_D

	int n = sz(a);

	// a を [0, |a|) に座標圧縮した結果を b に格納する．
	vi b;
	int m = coordinate_compression(a, b);

	// fw[i] : 今まで見てきた範囲に値 i が何個あったか（∈ {0, 1}）
	fenwick_tree<int> fw(m);
	ll res = 0;

	// 位置について昇順に見ていく．
	rep(i, n) {
		fw.add(b[i], 1);

		// 自身より大きい数が今までに何個あったか調べ，加算する．
		res += fw.sum(b[i] + 1, m);
	}
	return res;
}


//【転倒数】O(n (log n)^2)
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
	dump(ai); dump(bi);

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


//【置換のサイクル分解】O(n)
/*
* [0..n) の置換 p を巡回置換の積に分解して cycles に格納し cycles を返す．
* p は任意の i を p[i] に動かすような置換を表す．
*/
vvi permutation_decomposition(const vi& p) {
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


//【階乗進法 → 順列】O(n log n)
/*
* 階乗進法表記で上位桁から順に ds[0..n) が並んだ数を num とする．
* [0..n) の順列で辞書順で num 番目（0-indexed）の順列を返す．
*
* 利用：【多重集合の動的辞書】
*/
vi factorial_base_to_permutation(const vi& ds) {
	// verify : https://atcoder.jp/contests/arc047/tasks/arc047_c

	//【例】
	// 階乗進法表記で "1010" と表される数は，
	//		[1, 0, 1, 0].[3!, 2!, 1!, 0!] = 6 + 0 + 1 + 0 = 7
	// である．[0..4) の順列のうち辞書順で 7 番目のものは，
	//		0: [0, 1, 2, 3]
	//		1: [0, 1, 3, 2]
	//		2: [0, 2, 1, 3]
	//		3: [0, 2, 3, 1]
	//		4: [0, 3, 1, 2]
	//		5: [0, 3, 2, 1]
	//		6: [1, 0, 2, 3]
	//		7: [1, 0, 3, 2]
	// より p[0..4) = [1, 0, 3, 2] である．

	int n = sz(ds);
	vi p(n);

	vi ini(n);
	iota(all(ini), 0);

	// dd : [0..n) の中で残っている数を昇順に記録した辞書
	Dynamic_dictionary dd(n, ini);

	rep(i, n) {
		// [0..n) の中で残っている数のうち ds[i] 番目のものを選ぶ．
		p[i] = dd.get(ds[i]);

		// 選んだ数は消去しておく．
		dd.erase(p[i], 1);
	}

	return p;
}


