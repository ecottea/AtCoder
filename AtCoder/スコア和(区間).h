#pragma once
#include "header.h"
#include "数え上げ(区間).h"
#include "前処理(文字列).h"
#include "累積演算.h"
#include "座標圧縮.h"
// ■■■■■ スコア和（区間） ■■■■■


//【区間の和の総和】O(n)
/*
* Σ_l<r Σa[l..r) の値を返す．
*/
template <class T>
T interval_sum_sum(const vector<T>& a) {
	//【方法】
	// 主客転倒して考えると，a[i] を含む区間の個数は (i+1)(n-i) 個なので，
	// Σi∈[0..n) (i+1)(n-i) a[i] を計算すればよい．

	int n = sz(a);

	T res = T(0);
	rep(i, n) res += a[i] * (i + 1) * (n - i);

	return res;
}


//【区間の和の総和（長さごと）】O(n)
/*
* 各 w∈[0..n] について，
*	Σ_r-l=w Σa[l..r)
* の値を格納したリストを返す．
*/
template <class T>
vector<T> interval_sum_every_length(const vector<T>& a) {
	//【方法】
	// a[0..n) の累積和を A[i] = Σa[0..i) とおくと，求める和は
	//		Σ_r-l=w Σa[l..r)
	//		= Σ_r-l=w (A[r] - A[l])
	//		= Σl∈[0..n-w] (A[l+w] - A[l])
	//		= ΣA[w..n] - ΣA[0..n-w]
	// と表される．これは A[0..n] の累積和を前計算すれば高速に求められる．

	int n = sz(a);
	vector<T> res(n + 1);

	vector<T> A(n + 1);
	rep(i, n) A[i + 1] = A[i] + a[i];

	vector<T> B(n + 2);
	rep(i, n + 1) B[i + 1] = B[i] + A[i];

	repi(w, 1, n) res[w] = (B[n + 1] - B[w]) - B[n - w + 1];

	return res;
}


//【区間の min の総和】O(n log n)
/*
* Σ_l<r min a[l..r) の値を返す．
*
* 利用：【最小値ごとの区間の数え上げ】
*/
template <class T>
T interval_min_sum(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc005/tasks/agc005_b

	auto cnt = count_min_intervals(a);

	T res = T(0);
	repe(p, cnt) res += p.first * p.second;

	return res;
}


//【区間の XOR の総和】O(n log maxA)
/*
* a[0..n) の累積 XOR をとり【組の XOR の総和】を用いて 2 で割れば良い．
* 
* verify : https://atcoder.jp/contests/abc365/tasks/abc365_e
*/


//【区間の OR の総和】O(n log A)
/*
* Σ_l<r OR a[l..r) の値を返す．
*
* 利用：【ランレングス符号】
*/
ll interval_OR_sum(const vi& a) {
	// verify : https://yukicoder.me/problems/no/2300

	int n = sz(a);

	ll res = 0;

	// 第 b ビットからの寄与を計算する．
	rep(b, 31) {
		// seq[i] : a[i] の第 b ビット
		vi seq(n);
		rep(i, n) seq[i] = getb(a[i], b);

		// ビット列をランレングス符号化する．
		auto rle = run_length_encoding(seq);

		// 全ての区間の個数を加算する．
		ll cnt = (ll)n * (n + 1) / 2;

		for (auto [v, c] : rle) {
			// 0 のみからなる区間の個数を減算する．
			if (v == 0) cnt -= (ll)c * (c + 1) / 2;
		}

		// ビット位置の分だけ平行移動したものが実際の寄与になる．
		res += cnt << b;
	}

	return res;
}


//【区間の平均の総和】O(n)
/*
* Σ_l<r 1/(r-l) Σa[l..r) の値を返す．
*
* 利用：【線形加重累積和】
*/
template <class T>
mint interval_mean_sum(const vector<T>& a) {
	// verify : https://mojacoder.app/users/yunipoke/problems/ave-sum-subarray

	//【方法】
	// 主客転倒して考える．
	// a[i] を含む長さ w の区間の個数を c(i,w) とすると，求める値は
	//		Σi∈[0..n) (Σw∈[1..n] c(i,w)/w) a[i]
	// と表される．
	// 
	// c(i,w) は，区間の左端の上下限に注目すると
	//		c(i,w) = min(i, n-w) - max(i+1-w, 0) + 1
	// と表される．
	//		x = min(i+1, n-i)
	// とおくと，c(i,w) は
	//		c(i,w) = w      (1 ≦ w ≦ x)
	//		c(i,w) = x      (x < w ≦ n-x)
	//		c(i,w) = n+1-w  (n-x < w ≦ n)
	// と w の区分線形関数で表されるから，先の係数は線形加重和で計算できる．
	
	int n = sz(a);

	vm w(n + 1);
	repi(i, 1, n) w[i] = mint(i).inv();

	Linear_weighted_cumulative_sum W(w);

	mint res = 0;

	rep(i, n) {
		int x = min(i + 1, n - i);

		mint c;
		c += W.sum_to_right(1, x, 1, 2);
		c += W.sum_to_right(x, n + 2 - x, x, x);
		c += W.sum_to_right(n + 2 - x, n + 1, x - 1, x - 2);

		res += c * a[i];
	}

	return res;
}


//【区間の転倒数の総和】O(n log n)
/*
* Σ_l<r (a[l..r) の転倒数) の値を返す．
*
* 利用：【座標圧縮】
*/
template <class T>
mint interval_inversion_number_sum(const vector<T>& a) {
	// verify : https://mojacoder.app/users/shinnshinn/problems/inv-query

	int n = sz(a);

	// b : a を座標圧縮した結果
	vi b;
	int m = coordinate_compression(a, b);

	// fen[i] : 今まで見てきた範囲に値 i が何個あったか
	fenwick_tree<mint> fen0(m);
	fenwick_tree<mint> fen1(m);

	mint res = 0;

	// 位置について昇順に見ていく．
	rep(i, n) {
		// 自身より大きい数が今までに何個あったか調べ，適切に重みづけて加算する．
		mint c = fen0.sum(b[i] + 1, m);
		mint s = fen1.sum(b[i] + 1, m);
		res += (c + s) * (n - i);

		fen0.add(b[i], 1);
		fen1.add(b[i], i);
	}

	return res;
}


//【区間の積の総和（長さ K 以下）】O(n log n)
/*
* Σ_1≦r-l≦K Πa[l..r) の値を返す．
*
*（分割統治法）
*/
template <class T>
mint interval_product_sum(const vector<T>& a, int K) {
	// verify : https://www.codechef.com/problems/KPRODSUM

	int n = sz(a);

	mint res = 0;

	function<void(int, int)> rf = [&](int l, int r) {
		// 区間の幅が 1 になったら終了．
		if (l + 1 == r) {
			res += a[l];
			return;
		}

		int m = (l + r) / 2;

		// mul_l[i] : [l..m) の右からの長さ i の区間の総積
		vm mul_l(m - l + 1, 1);
		repir(i, m - 1, l) mul_l[m - i] = mul_l[m - i - 1] * a[i];

		// mul_r[i] : [m..r) の左からの長さ i の区間の総積
		vm mul_r(r - m + 1, 1);
		repi(i, m + 1, r) mul_r[i - m] = mul_r[i - m - 1] * a[i - 1];

		// acc_r[i] : Σmul_r[1..i)
		vm acc_r(r - m + 1 + 1);
		repi(i, 1, r - m) acc_r[i + 1] = acc_r[i] + mul_r[i];

		// [l..m) と [m..r) それぞれから長さ 1 以上の区間を選ぶパターンについての寄与を求める．
		repi(i, 1, m - l) res += mul_l[i] * acc_r[max(min(K - i + 1, r - m + 1), 0)];

		// 左右の区間について再帰的に処理する．
		rf(l, m);
		rf(m, r);
	};
	rf(0, n);

	return res;
}


