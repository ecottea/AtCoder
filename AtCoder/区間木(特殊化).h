#pragma once
#include "header.h"
// ■■■■■ 特殊化したフェニック木，セグメント木 ■■■■■


//【フェニック木：一点加算／区間総和クエリ】
/*
* range_sum_query_ft(n) : O(n)
*	要素数 n かつ初期値 0 で初期化する．
*
* add(p, val) : O(log n)
*	v[p] に val を加算する．
*
* get(p) : O(log n)
*	v[p] の値を返す．
*
* sum(l, r) : O(log n)
*	半開区間 [l, r) の要素の総和を返す．
*/
template <class T>
struct range_sum_query_ft {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数
	int n;

	// v[i] : 右端が i であるような閉区間の総和
	// i : 1-indexed
	vector<T> v;


	// コンストラクタ（初期化なし）
	range_sum_query_ft() {}

	// コンストラクタ（要素数 n かつ初期値 0 で初期化）
	range_sum_query_ft(int n_) : n(n_ + 1), v(n) {}


	// v[p] に val を加算する．
	// p : 0-indexed
	void add(int p, T val) {
		// p を 1-indexed に直す．
		p++;

		// 根に向かって値を加算していく．
		while (p < n) {
			v[p] += val;

			// p の最下位ビットに 1 を加算することで次の位置を得る．
			p += p & -p;
		}
	}

	// 閉区間 [1, r] の要素の総和を返す．
	// r : 1-indexed
	T sum(int r) {
		T res = 0;

		// 子に向かって累積和をとっていく．
		while (r > 0) {
			res += v[r];

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// 半開区間 [l, r) の要素の総和を返す．
	// l, r : 0-indexed
	T sum(int l, int r) {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return sum(r) - sum(l);
	}

	// v[p] の値を返す．
	// p : 0-indexed
	T get(int p) {
		return sum(p, p + 1);
	}
};


//【フェニック木：一点 Xor／区間 Xor クエリ】
/*
* range_xor_query_ft(n) : O(n)
*	要素数 n かつ初期値 0 で初期化する．
*
* apply(p, val) : O(log n)
*	v[p] に val を Xor する．
*
* get(p) : O(log n)
*	v[p] の値を返す．
*
* prod(l, r) : O(log n)
*	半開区間 [l, r) の要素の Xor を返す．
*/
template <class T>
struct range_xor_query_ft {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数
	int n;

	// v[i] : 右端が i であるような閉区間の総和
	// i : 1-indexed（v[0] は使わない）
	vector<T> v;


	// コンストラクタ（初期化なし）
	range_xor_query_ft() {}

	// コンストラクタ（要素数 n かつ初期値 0 で初期化）
	range_xor_query_ft(int n_) : n(n_ + 1), v(n) {}

	// コンストラクタ（配列で初期化）
	range_xor_query_ft(const vector<T>& v_) : n(sz(v_) + 1), v(n) {
		// 配列の値を仮登録する．
		rep(i, n - 1) {
			v[i + 1] = v_[i];
		}

		// 正しい値になるよう根に向かって Xor をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[i] ^= v[i - pow2];
			}
		}
	}

	// 出力
	friend ostream& operator<<(ostream& os, range_xor_query_ft ft) {
		repi(i, 1, ft.n - 1) {
			os << ft.get(i) << " ";
		}
		return os;
	}


	// v[p] に val を Xor する．
	// p : 0-indexed
	void apply(int p, T val) {
		// p を 1-indexed に直す．
		p++;

		// 根に向かって値を Xor していく．
		while (p < n) {
			v[p] ^= val;

			// p の最下位ビットに 1 を加算することで次の位置を得る．
			p += p & -p;
		}
	}

	// 閉区間 [1, r] の要素の Xor を返す．
	// r : 1-indexed
	T prod(int r) {
		T res = 0;

		// 子に向かって累積 Xor をとっていく．
		while (r > 0) {
			res ^= v[r];

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// 半開区間 [l, r) の要素の Xor を返す．
	// l, r : 0-indexed
	T prod(int l, int r) {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総 Xor から閉区間 [1, l] の総 Xor を Xor すれば良い．
		return prod(r) ^ prod(l);
	}

	// v[p] の値を返す．
	// p : 0-indexed
	T get(int p) {
		return prod(p, p + 1);
	}
};


//【フェニック木：区間加算／区間総和クエリ】
/*
* range_add_sum_query_ft(n) : O(n)
*	要素数 n かつ初期値 0 で初期化する．
*
* add(p, val) : O(log n)
*	v[p] に val を加算する．
*
* add(l, r, val) : O(log n)
*	半開区間 [l, r) の要素に val を加算する．
*
* get(p) : O(log n)
*	v[p] の値を返す．
*
* sum(l, r) : O(log n)
*	半開区間 [l, r) の要素の総和を返す．
*/
template <class T>
struct range_add_sum_query_ft {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数（要素数 + 1）
	int n;
	vector<vector<T>> v;


	// コンストラクタ（初期化なし）
	range_add_sum_query_ft() {}

	// コンストラクタ（要素数 n かつ初期値 0 で初期化）
	range_add_sum_query_ft(int n_) : n(n_ + 1), v(2, vector<T>(n)) {}


	// 出力
	friend ostream& operator<<(ostream& os, range_add_sum_query_ft ft) {
		rep(i, ft.n - 1) {
			os << ft.get(i) << " ";
		}
		return os;
	}


	// add() の実現用．v[vi][p] に val を加算する．
	void add_sub(int vi, int p, T val) {
		// 計算の便宜上，位置を 1-indexed に直す．
		p++;

		// 根に向かって値を加算していく．
		while (p < n) {
			v[vi][p] += val;

			// p の最下位ビットに 1 を加算することで次の位置を得る．
			p += p & -p;
		}
	}

	// 半開区間 [l, r) の要素に val を加算する．
	void add(int l, int r, T val) {
		add_sub(0, l, -val * (l - 1));
		add_sub(0, r - 1, val * (r - 1));
		add_sub(1, l, val);
		add_sub(1, r - 1, -val);
	}

	// v[p] に val を加算する．
	void add(int p, T val) {
		add(p, p + 1, val);
	}

	// sum() の実現用．v[vi] の半開区間 [0, r) の要素の総和を返す．
	T sum_sub(int vi, int r) {
		T res = 0;

		// 子に向かって累積和をとっていく．
		while (r > 0) {
			res += v[vi][r];

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// 半開区間 [0, r) の要素の総和を返す．
	T sum(int r) {
		return sum_sub(0, r) + sum_sub(1, r) * (r - 1);
	}

	// 半開区間[l, r) の要素の総和を返す．
	T sum(int l, int r) {
		return sum(r) - sum(l);
	}

	// v[p] の値を返す．
	T get(int p) {
		return sum(p, p + 1);
	}
};


//【二次元フェニック木：一点加算／領域総和クエリ】
/*
* range_sum_query_ft2D(h, w) : O(h w)
*	要素数 h * w かつ初期値 0 で初期化する．
*
* add(x, y, val) : O(log h log w)
*	v[x][y] に val を加算する．
*
* get(x, y) : O(log h log w)
*	v[x][y] の値を返す．
*
* sum(x1, y1, x2, y2) : O(log h log w)
*	半開長方形 [x1, x2) * [y1, y2) の要素の総和を返す．
*/
template <class T>
struct range_sum_query_ft2D {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// 領域の大きさ
	int h, w;
	vector<vector<T>> v;


	// コンストラクタ（初期化なし）
	range_sum_query_ft2D() {}

	// コンストラクタ（要素数 h * w かつ初期値 0 で初期化）
	range_sum_query_ft2D(int h_, int w_) : h(h_), w(w_), v(h, vector<T>(w)) {}


	// v[x][y] に val を加算する．
	void add(int x, int y, T val) {
		// 計算の便宜上，位置 x, y を 1-indexed に直しておく．
		// 根に向かって値を加算していく．
		// 最下位ビットに 1 を加算することで次の位置を得る．
		for (int i = x + 1; i <= h; i += i & -i) {
			for (int j = y + 1; j <= w; j += j & -j) {
				// v にアクセスするときは 0-indexed に直す．
				v[i - 1][j - 1] += val;
			}
		}
	}

	// 半開長方形 [0, x) * [0, y) の要素の総和を返す．
	T sum(int x, int y) {
		T res = 0;

		// x, y は 1-indexed の閉長方形 [1, x] * [1, y] の端でもあるのでそのままで良い．
		// 子に向かって累積和をとっていく．
		// 最下位ビットから 1 を減算することで次の位置を得る．
		for (int i = x; i > 0; i -= i & -i) {
			for (int j = y; j > 0; j -= j & -j) {
				// v にアクセスするときは 0-indexed に直す．
				res += v[i - 1][j - 1];
			}
		}

		return res;
	}

	// 半開長方形 [x1, x2) * [y1, y2) の要素の総和を返す．
	T sum(int x1, int y1, int x2, int y2) {
		T res = 0;

		res += sum(x2, y2);
		res -= sum(x2, y1);
		res -= sum(x1, y2);
		res += sum(x1, y1);

		return res;
	}

	// v[x][y] の値を返す．
	T get(int x, int y) {
		return sum(x, y, x + 1, y + 1);
	}
};


//【セグメント木：一点変更／区間最小値クエリ】
/*
* range_minimum_query(n) : O(n)
*	要素数 n かつ初期値 INF で初期化する．
*
* set(i, val) : O(log n)
*	i 番目の要素を val に変更する．
*
* min(l, r) : O(log n)
*	半開区間 [l, r) の要素の最小値を返す．
*
* get(i) : O(1)
*	i 番目の要素を得る．
*/
template <class T>
struct range_minimum_query {
	// 参考：https://algo-logic.info/segment-tree/

	// 型 T における最大値
	const T T_INF = numeric_limits<T>::max();

	// 完全二分木の葉の数（必ず 2 冪）
	int n;

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，子は v[2 * i], v[2 * i + 1]．
	// 0-indexed での i 番目のデータは葉である v[i + n] に入っている．
	vector<T> v;

	// コンストラクタ（初期化なし）
	range_minimum_query() {}

	// コンストラクタ（最大値で初期化）：O(N)
	range_minimum_query(int n_tmp) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_tmp) {
			pow2 *= 2;
		}
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<T>(2 * n, T_INF);
	}

	// 一点更新：O(log N)
	// i 番目の要素を val に更新する．
	void set(int i, T val) {
		// 実際にデータを格納すべき葉の位置へ
		i += n;

		// 葉のデータを更新
		v[i] = val;

		// 親のデータも更新しておく
		while (i > 1) {
			i /= 2;
			v[i] = std::min(v[i * 2], v[i * 2 + 1]);
		}
	}

	// 区間最小値：O(log N)
	// 半開区間 [l, r) における最小の要素を返す．
	T min(int l, int r) {
		return min_rf(l, r, 1, 0, n);
	}
	// 実際の再帰関数
	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	T min_rf(int l, int r, int k, int kl, int kr) {
		// 範囲外なら最小値の計算に含めない．
		if (kr <= l || r <= kl) {
			return T_INF;
		}

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) {
			return v[k];
		}

		// 一部の範囲のみを含むなら子を見に行く．
		T min_l = min_rf(l, r, k * 2, kl, (kl + kr) / 2);
		T min_r = min_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return std::min(min_l, min_r);
	}

	// i 番目の要素を得る．
	T get(int i) {
		return v[i + n];
	}
};


//【セグメント木：一点変更／区間最大値クエリ】
/*
* range_maximum_query(n) : O(n)
*	要素数 n かつ初期値 -INF で初期化する．
*
* range_maximum_query(a) : O(n)
*	配列 a の要素で初期化する．
*
* set(i, val) : O(log n)
*	i 番目の要素を val に変更する．
*
* max(l, r) : O(log n)
*	半開区間 [l, r) の要素の最大値を返す．
*
* get(i) : O(1)
*	i 番目の要素を得る．
*
* find_left(l, r, val) : O(log n)
*	半開区間 [l, r) 内で val 以上の値をもつ最左位置を返す．
*   見つからない場合は r を返す．
*/
template <class T>
struct range_maximum_query {
	// 参考：https://algo-logic.info/segment-tree/

	// 型 T における最小値
	const T T_NINF = numeric_limits<T>::min();

	// 完全二分木の葉の数（必ず 2 冪）
	int n;

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，子は v[2 * i], v[2 * i + 1]．
	// 0-indexed での i 番目のデータは葉である v[i + n] に入っている．
	vector<T> v;


	// コンストラクタ（初期化なし）
	range_maximum_query() {}

	// コンストラクタ（最小値で初期化）：O(N)
	range_maximum_query(int n_tmp) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_tmp) {
			pow2 *= 2;
		}
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<T>(2 * n, T_NINF);
	}

	// コンストラクタ（配列で初期化）：O(N)
	range_maximum_query(vector<T>& a) : range_maximum_query(sz(a)) {
		// 全ての葉にデータを設定する．
		rep(i, sz(a)) {
			v[i + n] = a[i];
		}

		// 全てのノードに正しい値を設定する．
		repir(i, n - 1, 1) {
			v[i] = std::max(v[i * 2], v[i * 2 + 1]);
		}
	}


	// 一点更新：O(log N)
	// i 番目の要素を val に更新する．
	void set(int i, T val) {
		// 実際にデータを格納すべき葉の位置へ
		i += n;

		// 葉のデータを更新
		v[i] = val;

		// 親のデータも更新しておく
		while (i > 1) {
			i /= 2;
			v[i] = std::max(v[i * 2], v[i * 2 + 1]);
		}
	}

	// 区間最大値：O(log N)
	// 半開区間 [l, r) における最大の要素を返す．
	T max(int l, int r) {
		return max_rf(l, r, 1, 0, n);
	}

	// max() を実現する実際の再帰関数
	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	T max_rf(int l, int r, int k, int kl, int kr) {
		// 範囲外なら最大値の計算に含めない．
		if (kr <= l || r <= kl) {
			return T_NINF;
		}

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) {
			return v[k];
		}

		// 一部の範囲のみを含むなら子を見に行く．
		T max_l = max_rf(l, r, k * 2, kl, (kl + kr) / 2);
		T max_r = max_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return std::max(max_l, max_r);
	}

	// i 番目の要素を得る．
	T get(int i) {
		return v[i + n];
	}

	// 半開区間 [l, r) 内で val 以上の値をもつ最左位置を返す．
	int find_left(int l, int r, T val) {
		return find_left_rf(l, r, val, 1, 0, n);
	}

	// find_left() を実現する実際の再帰関数
	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	int find_left_rf(int l, int r, T val, int k, int kl, int kr) {
		// 自身の値が val 未満か範囲外なら右端を返す．
		if (v[k] < val || kr <= l || r <= kl) {
			return r;
		}

		// 自身が葉であればその位置を返す．
		if (k >= n) {
			return k - n;
		}

		// まず左の部分木を見に行き，見つかったならそれを返す．
		int pos = find_left_rf(l, r, val, k * 2, kl, (kl + kr) / 2);
		if (pos != r) {
			return pos;
		}

		// 見つからなかったなら右の部分木も見にいき，結果を返す．
		return find_left_rf(l, r, val, k * 2 + 1, (kl + kr) / 2, kr);
	}
};


//【セグメント木：一点加算／区間総和クエリ】
/*
* range_sum_query(n) : O(n)
*	要素数 n かつ初期値 0 で初期化する．
*
* add(i, val) : O(log n)
*	i 番目の要素に val を加算する．
*
* sum(l, r) : O(log n)
*	半開区間 [l, r) の要素の総和を返す．
*
* get(i) : O(1)
*	i 番目の要素を得る．
*/
template <class T>
struct range_sum_query {
	// 参考：https://algo-logic.info/segment-tree/

	// 完全二分木の葉の数（必ず 2 冪）
	int n;

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，子は v[2 * i], v[2 * i + 1]．
	// 0-indexed での i 番目のデータは葉である v[i + n] に入っている．
	vector<T> v;

	// コンストラクタ（初期化なし）
	range_sum_query() {}

	// コンストラクタ（0 で初期化）：O(N)
	range_sum_query(int n_tmp) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_tmp) {
			pow2 *= 2;
		}
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<T>(2 * n, 0);
	}

	// 一点更新：O(log N)
	// i 番目の要素に val を加算する．
	void add(int i, T val) {
		// 実際にデータを格納すべき葉の位置へ
		i += n;

		// 葉のデータを更新
		v[i] += val;

		// 親のデータも更新しておく
		while (i > 1) {
			i /= 2;
			v[i] = v[i * 2] + v[i * 2 + 1];
		}
	}

	// 区間総和：O(log N)
	// 半開区間 [l, r) 内の要素の総和を返す．
	T sum(int l, int r) {
		return sum_rf(l, r, 1, 0, n);
	}
	// 実際の再帰関数
	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	T sum_rf(int l, int r, int k, int kl, int kr) {
		// 範囲外なら総和の計算に含めない．
		if (kr <= l || r <= kl) {
			return 0;
		}

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) {
			return v[k];
		}

		// 一部の範囲のみを含むなら子を見に行く．
		T sum_l = sum_rf(l, r, k * 2, kl, (kl + kr) / 2);
		T sum_r = sum_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return sum_l + sum_r;
	}

	// i 番目の要素を得る．
	T get(int i) {
		return v[i + n];
	}
};


//【遅延評価セグメント木：区間変更／区間最小値クエリ】
/*
* range_update_mininum_query(n) : O(n)
*	要素数 n かつ初期値 INF で初期化する．
*
* set(i, val) : O(log n)
*	i 番目の要素を val に変更する．
*
* set(l, r, val) : O(log n)
*	半開区間 [l, r) の要素を val に変更する．
*
* get(i) : O(log n)
*	i 番目の要素を返す．
*
* min(l, r) : O(log n)
*	半開区間 [l, r) の要素の最小値を返す．
*/
template <class T>
struct range_update_minimum_query {
	// 参考：https://algo-logic.info/segment-tree/

	// 型 T における最大値
	const T T_INF = numeric_limits<T>::max();

	// 完全二分木の葉の数（必ず 2 冪）
	int n;

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，子は v[2 * i], v[2 * i + 1]．
	// 0-indexed での i 番目のデータは葉である v[i + n] に入っている．
	vector<T> v;

	// 遅延評価用の完全二分木
	vector<T> lazy;


	// コンストラクタ（初期化なし）
	range_update_minimum_query() {}

	// コンストラクタ（最大値で初期化）：O(N)
	range_update_minimum_query(int n_tmp) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_tmp) {
			pow2 *= 2;
		}
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<T>(2 * n, T_INF);
		lazy = vector<T>(2 * n, T_INF);
	}


	// 遅延させていた評価を行う．
	void eval(int k) {
		// 遅延させていた評価がなければ何もしない．
		if (lazy[k] == T_INF) {
			return;
		}

		// 葉でなければ子に伝搬する．
		if (k < n) {
			lazy[k * 2] = lazy[k * 2 + 1] = lazy[k];
		}

		// 自身を評価する．
		v[k] = lazy[k];
		lazy[k] = T_INF;
	}

	// 一点更新：O(log N)
	// i 番目の要素を val に更新する．
	void set(int i, T val) {
		set(i, i + 1, val);
	}

	// 区間更新：O(log N)
	// 半開区間 [l, r) の要素を val に更新する．
	void set(int l, int r, T val) {
		set_rf(l, r, val, 1, 0, n);
	}

	// set() を実現する実際の再帰関数
	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	void set_rf(int l, int r, T val, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら何もしない．
		if (kr <= l || r <= kl) {
			return;
		}

		// 完全に範囲内なら自身の値を更新する．
		if (l <= kl && kr <= r) {
			lazy[k] = val;
			eval(k);

			return;
		}

		// 一部の範囲のみを含むなら子を見に行く．
		set_rf(l, r, val, k * 2, kl, (kl + kr) / 2);
		set_rf(l, r, val, k * 2 + 1, (kl + kr) / 2, kr);
		v[k] = std::min(v[k * 2], v[k * 2 + 1]);
	}

	// 一点取得：O(log N)
	// i 番目の要素を返す．
	T get(int i) {
		return this->min(i, i + 1);
	}

	// 区間最小値：O(log N)
	// 半開区間 [l, r) における最小の要素を返す．
	T min(int l, int r) {
		return min_rf(l, r, 1, 0, n);
	}

	// min() を実現する実際の再帰関数
	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	T min_rf(int l, int r, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら最小値の計算に含めない．
		if (kr <= l || r <= kl) {
			return T_INF;
		}

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) {
			return v[k];
		}

		// 一部の範囲のみを含むなら子を見に行く．
		T min_l = min_rf(l, r, k * 2, kl, (kl + kr) / 2);
		T min_r = min_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return std::min(min_l, min_r);
	}
};


//【遅延評価セグメント木：区間加算／区間最小値クエリ】
/*
* range_add_minimum_query(n) : O(n)
*	要素数 n かつ初期値 INF で初期化する．
*
* range_add_minimum_query(a) : O(n)
*	配列 a で初期化する．
*
* add(i, val) : O(log n)
*	i 番目の要素に val を加算する．
*
* add(l, r, val) : O(log n)
*	半開区間 [l, r) の要素に val を加算する．
*
* get(i) : O(log n)
*	i 番目の要素を返す．
*
* min(l, r) : O(log n)
*	半開区間 [l, r) の要素の最小値を返す．
*/
template <class T>
struct range_add_minimum_query {
	// 参考：https://algo-logic.info/segment-tree/

	// 型 T における最大値
	const T T_INF = numeric_limits<T>::max();

	// 完全二分木の葉の数（必ず 2 冪）
	int n;

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，子は v[2 * i], v[2 * i + 1]．
	// 0-indexed での i 番目のデータは葉である v[i + n] に入っている．
	vector<T> v;

	// 遅延評価用の完全二分木
	vector<T> lazy;


	// コンストラクタ（初期化なし）
	range_add_minimum_query() {}

	// コンストラクタ（0 で初期化）：O(N)
	range_add_minimum_query(int n_tmp) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_tmp) {
			pow2 *= 2;
		}
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<T>(2 * n, T_INF);
		lazy = vector<T>(2 * n, 0);
	}

	// コンストラクタ（配列で初期化）：O(N)
	range_add_minimum_query(vector<T>& a) : range_add_minimum_query(sz(a)) {
		// 全ての葉にデータを設定する．
		rep(i, sz(a)) {
			v[i + n] = a[i];
		}

		// 全てのノードに正しい値を設定する．
		repir(i, n - 1, 1) {
			v[i] = std::min(v[i * 2], v[i * 2 + 1]);
		}
	}


	// 遅延させていた評価を行う．：O(1)
	void eval(int k) {
		// 遅延させていた評価がなければ何もしない．
		if (lazy[k] == 0) {
			return;
		}

		// 葉でなければ子に伝搬する．
		if (k < n) {
			lazy[k * 2] += lazy[k];
			lazy[k * 2 + 1] += lazy[k];
		}

		// 自身を評価する．（オーバーフローに注意）
		if (v[k] != T_INF) {
			v[k] += lazy[k];
		}
		lazy[k] = 0;
	}

	// 一点加算：O(log N)
	// i 番目の要素に val を加算する．
	void add(int i, T val) {
		add(i, i + 1, val);
	}

	// 区間加算：O(log N)
	// 半開区間 [l, r) の要素に val を加算する．
	void add(int l, int r, T val) {
		add_rf(l, r, val, 1, 0, n);
	}

	// add() を実現する実際の再帰関数
	// k : 注目ノード
	// [kl, kr) : ノード v[k] が表す区間
	void add_rf(int l, int r, T val, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら何もしない．
		if (kr <= l || r <= kl) {
			return;
		}

		// 完全に範囲内なら自身の値を更新する．
		if (l <= kl && kr <= r) {
			lazy[k] += val;
			eval(k);

			return;
		}

		// 一部の範囲のみを含むなら子を見に行く．
		add_rf(l, r, val, k * 2, kl, (kl + kr) / 2);
		add_rf(l, r, val, k * 2 + 1, (kl + kr) / 2, kr);
		v[k] = std::min(v[k * 2], v[k * 2 + 1]);
	}

	// 一点取得：O(log N)
	// i 番目の要素を返す．
	T get(int i) {
		return this->min(i, i + 1);
	}

	// 区間最小値：O(log N)
	// 半開区間 [l, r) の要素の最小値を返す．
	T min(int l, int r) {
		return min_rf(l, r, 1, 0, n);
	}

	// min() を実現する実際の再帰関数
	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	T min_rf(int l, int r, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら最小値の計算に含めない．
		if (kr <= l || r <= kl) {
			return T_INF;
		}

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) {
			return v[k];
		}

		// 一部の範囲のみを含むなら子を見に行く．
		T min_l = min_rf(l, r, k * 2, kl, (kl + kr) / 2);
		T min_r = min_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return std::min(min_l, min_r);
	}
};


//【遅延評価セグメント木：区間加算／区間総和クエリ】
/*
* range_add_sum_query(n) : O(n)
*	要素数 n かつ初期値 0 で初期化する．
*
* range_add_sum_query(a) : O(n)
*	配列 a で初期化する．
*
* add(i, val) : O(log n)
*	i 番目の要素に val を加算する．
*
* add(l, r, val) : O(log n)
*	半開区間 [l, r) の要素に val を加算する．
*
* get(i) : O(log n)
*	i 番目の要素を返す．
*
* sum(l, r) : O(log n)
*	半開区間 [l, r) の要素の総和を返す．
*/
template <class T>
struct range_add_sum_query {
	// 参考：https://algo-logic.info/segment-tree/

	// 完全二分木の葉の数（必ず 2 冪）
	int n;

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，子は v[2 * i], v[2 * i + 1]．
	// 0-indexed での i 番目のデータは葉である v[i + n] に入っている．
	vector<T> v;

	// 遅延評価用の完全二分木
	vector<T> lazy;


	// コンストラクタ（初期化なし）
	range_add_sum_query() {}

	// コンストラクタ（0 で初期化）
	range_add_sum_query(int n_) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_) {
			pow2 *= 2;
		}
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<T>(2 * n, 0);
		lazy = vector<T>(2 * n, 0);
	}

	// コンストラクタ（配列で初期化）
	range_add_sum_query(const vector<T>& a) {
		int n_ = sz(a);

		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_) {
			pow2 *= 2;
		}
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<T>(2 * n, 0);
		lazy = vector<T>(2 * n, 0);

		// 配列の値を直接書き込む．
		rep(i, n_) {
			v[i + n] = a[i];
		}

		// 総和をとりつつ根に向かって更新していく．
		repir(i, n - 1, 1) {
			v[i] = v[2 * i] + v[2 * i + 1];
		}
	}

	// 出力
	friend ostream& operator<<(ostream& os, range_add_sum_query rasq) {
		rep(i, rasq.n) {
			os << rasq.get(i) << " ";
		}
		return os;
	}


	// 遅延させていた評価を行う．
	void eval(int k) {
		// 遅延させていた評価がなければ何もしない．
		if (lazy[k] == 0) {
			return;
		}

		// 葉でなければ子に伝搬する．
		// 子の受ける影響は親の受ける影響の半分になる．
		if (k < n) {
			lazy[k * 2] += lazy[k] / 2;
			lazy[k * 2 + 1] += lazy[k] / 2;
		}

		// 自身を評価する．
		v[k] += lazy[k];
		lazy[k] = 0;
	}

	// 一点加算：O(log N)
	// i 番目の要素に val を加算する．
	void add(int i, T val) {
		add(i, i + 1, val);
	}

	// 区間加算：O(log N)
	// 半開区間 [l, r) の要素に val を加算する．
	void add(int l, int r, T val) {
		add_rf(l, r, val, 1, 0, n, n);
	}

	// add() を実現する実際の再帰関数
	// k : 注目ノード
	// [kl, kr) : ノード v[k] が表す区間
	// num_leaf : 部分木 k の葉の数
	void add_rf(int l, int r, T val, int k, int kl, int kr, int num_leaf) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら何もしない．
		if (kr <= l || r <= kl) {
			return;
		}

		// 完全に範囲内なら自身の値を更新する．
		// 葉の数の分だけ自身の値は影響を受ける．
		if (l <= kl && kr <= r) {
			lazy[k] += val * num_leaf;
			eval(k);

			return;
		}

		// 一部の範囲のみを含むなら子を見に行く．
		add_rf(l, r, val, k * 2, kl, (kl + kr) / 2, num_leaf / 2);
		add_rf(l, r, val, k * 2 + 1, (kl + kr) / 2, kr, num_leaf / 2);
		v[k] = v[k * 2] + v[k * 2 + 1];
	}

	// 一点取得：O(log N)
	// i 番目の要素を返す．
	T get(int i) {
		return sum(i, i + 1);
	}

	// 区間総和：O(log N)
	// 半開区間 [l, r) の要素の総和を返す．
	T sum(int l, int r) {
		return sum_rf(l, r, 1, 0, n);
	}

	// sum() を実現する実際の再帰関数
	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	T sum_rf(int l, int r, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら総和の計算に含めない．
		if (kr <= l || r <= kl) {
			return 0;
		}

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) {
			return v[k];
		}

		// 一部の範囲のみを含むなら子を見に行く．
		T sum_l = sum_rf(l, r, k * 2, kl, (kl + kr) / 2);
		T sum_r = sum_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return sum_l + sum_r;
	}
};


//【遅延評価セグメント木：区間変更／区間総和クエリ】
/*
* range_update_sum_query(n) : O(n)
*	要素数 n かつ初期値 0 で初期化する．
*
* set(i, val) : O(log n)
*	i 番目の要素を val に変更する．
*
* set(l, r, val) : O(log n)
*	半開区間 [l, r) の要素を val に変更する．
*
* get(i,) : O(log n)
*	i 番目の要素を返す．
*
* sum(l, r) : O(log n)
*	半開区間 [l, r) の要素の総和を返す．
*/
template <class T>
struct range_update_sum_query {
	// 参考：https://algo-logic.info/segment-tree/

	// 型 T における使用しない値（値の変更をしないことを表すために用いる）
	const T IGNORE = numeric_limits<T>::max();

	// 完全二分木の葉の数（必ず 2 冪）
	int n;

	// 完全二分木を実現する大きさ 2 * n の配列
	// 根は v[1] で，v[i] の親は v[i / 2]，子は v[2 * i], v[2 * i + 1]．
	// 0-indexed での i 番目のデータは葉である v[i + n] に入っている．
	vector<T> v;

	// 遅延評価用の完全二分木
	vector<T> lazy;


	// コンストラクタ（初期化なし）
	range_update_sum_query() {}

	// コンストラクタ（0 で初期化）：O(N)
	range_update_sum_query(int n_tmp) {
		// 要素数以上となる最小の 2 冪を求め，n とする．
		int pow2 = 1;
		while (pow2 < n_tmp) {
			pow2 *= 2;
		}
		n = pow2;

		// 完全二分木を実現する大きさ 2 * n の配列を確保する．
		v = vector<T>(2 * n, 0);
		lazy = vector<T>(2 * n, IGNORE);
	}


	// 遅延させていた評価を行う．
	void eval(int k) {
		// 遅延させていた評価がなければ何もしない．
		if (lazy[k] == IGNORE) {
			return;
		}

		// 葉でなければ子に伝搬する．
		// 子の受ける影響は親の受ける影響の半分になる．
		if (k < n) {
			lazy[k * 2] = lazy[k] / 2;
			lazy[k * 2 + 1] = lazy[k] / 2;
		}

		// 自身を評価する．
		v[k] = lazy[k];
		lazy[k] = IGNORE;
	}

	// 一点変更：O(log N)
	// i 番目の要素を val に変更する．
	void set(int i, T val) {
		set(i, i + 1, val);
	}

	// 区間変更：O(log N)
	// 半開区間 [l, r) の要素を val に変更する．
	void set(int l, int r, T val) {
		set_rf(l, r, val, 1, 0, n, n);
	}

	// set() を実現する実際の再帰関数
	// k : 注目ノード
	// [kl, kr) : ノード v[k] が表す区間
	// num_leaf : 部分木 k の葉の数
	void set_rf(int l, int r, T val, int k, int kl, int kr, int num_leaf) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら何もしない．
		if (kr <= l || r <= kl) {
			return;
		}

		// 完全に範囲内なら自身の値を更新する．
		// 葉の数の分だけ自身の値は影響を受ける．
		if (l <= kl && kr <= r) {
			lazy[k] = val * num_leaf;
			eval(k);

			return;
		}

		// 一部の範囲のみを含むなら子を見に行く．
		set_rf(l, r, val, k * 2, kl, (kl + kr) / 2, num_leaf / 2);
		set_rf(l, r, val, k * 2 + 1, (kl + kr) / 2, kr, num_leaf / 2);
		v[k] = v[k * 2] + v[k * 2 + 1];
	}

	// 一点取得：O(log N)
	// i 番目の要素を返す．
	T get(int i) {
		return sum(i, i + 1);
	}

	// 区間総和：O(log N)
	// 半開区間 [l, r) の要素の総和を返す．
	T sum(int l, int r) {
		return sum_rf(l, r, 1, 0, n);
	}

	// sum() を実現する実際の再帰関数
	// k : 注目ノード，[kl, kr) : ノード v[k] が表す区間
	T sum_rf(int l, int r, int k, int kl, int kr) {
		// まず自身の評価を行っておく．
		eval(k);

		// 範囲外なら総和の計算に含めない．
		if (kr <= l || r <= kl) {
			return 0;
		}

		// 完全に範囲内なら葉まで降りず自身の値を返す．
		if (l <= kl && kr <= r) {
			return v[k];
		}

		// 一部の範囲のみを含むなら子を見に行く．
		T sum_l = sum_rf(l, r, k * 2, kl, (kl + kr) / 2);
		T sum_r = sum_rf(l, r, k * 2 + 1, (kl + kr) / 2, kr);
		return sum_l + sum_r;
	}
};


