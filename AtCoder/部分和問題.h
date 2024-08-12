#pragma once
#include "header.h"
#include "二項係数.h"
#include "FPS(mint).h"
// ■■■■■ 部分和問題 ■■■■■


//【部分和問題（判定）】O(n v / 64)
/*
* 非負整数列 a[0..n) の部分和として v が作れるかを返す．
*
* 制約：N ≧ v + 1
*/
template <size_t N>
bool find_partial_sum(const vi& a, int v) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_r

	int n = sz(a);

	// dp_i[j] : a[0..i) の中で和をちょうど j にできるか
	bitset<N> dp;
	dp[0] = 1; // 空和は 0

	// インライン配る DP
	rep(i, n) dp |= dp << a[i];

	return dp[v];
}


//【部分和問題（数え上げ）】O(n v)
/*
* 非負整数列 a[0..n) の部分和として各 i∈[0..v] を作る方法が何通りあるかを格納したリストを返す．
*
*（和を状態にもつ状態 DP）
*/
template <class T>
vector<T> count_partial_sum(const vi& a, int v) {
	// 参考 : https://qiita.com/suisen_cp/items/794f24d31852b97d58a6
	// verify : https://yukicoder.me/problems/no/1043

	int n = sz(a);

	// dp[i][j] : a[0..i) の中で和がちょうど j になる組合せの数
	vector<vector<T>> dp(n + 1, vector<T>(v + 1));
	dp[0][0] = 1; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) repi(j, 0, v) {
		// a[i] を選ばない場合
		dp[i + 1][j] = dp[i][j];

		// a[i] を選ぶ場合
		if (j - a[i] >= 0) dp[i + 1][j] += dp[i][j - a[i]];
	}

	return dp[n];
}


//【部分和問題（数え上げ，mod 998244353）】O(n + v log v)
/*
* 非負整数列 a[0..n) の部分和として各 i∈[0..v] を作る方法が何通りあるかを格納したリストを返す．
*
* 制約：fm は 2(v+1)! まで計算可能
* 
* 利用：【形式的冪級数】,【指数関数】
*/
vm count_partial_sum_fps(const vi& a, int v, const Factorial_mint& fm) {
	// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a
	// verify : https://judge.yosupo.jp/problem/sharp_p_subset_sum

	//【方法】
	// 母関数は
	//		f(z) = Πi∈[0..n) (1 + z^a[i])
	// であるが，これは
	//		f(z) = exp(Σi=[0..n) log(1 + z^a[i]))
	// と書き直せる．対数関数のマクローリン展開の式
	//		log(1 + z) = Σk∈[1..∞) (-1)^(k-1) 1/k z^k
	// より
	//		log(1 + z^a[i]) = Σk∈[1..∞) (-1)^(k-1) 1/k z^(k a[i])
	// であり，これはスパースなので高速に和が計算できる．

	// cnt[v] : a[0..n) の中に v が何個含まれるか
	unordered_map<int, int> cnt;
	repe(x, a) cnt[x]++;

	MFPS f(0, v + 1);
	repe(p, cnt) {
		for (int k = 1; k * p.first <= v; k++) {
			f[k * p.first] += p.second * (k & 1 ? 1 : -1) * fm.inv(k);
		}
	}
	f = exp_fps(f, v + 1, fm);

	return f.c;
}


//【部分和問題（復元）】O(n v / 64)
/*
* 非負整数列 a[0..n) について，Σi∈S a[i] = v なる添字集合 S を is に格納する．
* S が存在しなければ false を返す．
*
* 制約：N は v + 1 以上の定数．
*/
template <size_t N>
bool construction_partial_sum_fast(const vi& a, int v, vi& is) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_cq

	if (v < 0) return false;

	int n = sz(a);
	is.clear();

	// dp[i][j] : a[0..i) の中で和をちょうど j にできるか
	vector<bitset<N>> dp(n + 1);
	dp[0][0] = 1;

	// 配る DP
	rep(i, n) dp[i + 1] = dp[i] | (dp[i] << a[i]);

	// 和が v になる部分集合が存在しない場合
	if (!dp[n][v]) return false;

	// DP 復元
	repir(i, n - 1, 0) {
		if (v - a[i] >= 0 && dp[i][v - a[i]]) {
			is.push_back(i);
			v -= a[i];
		}
	}
	reverse(all(is));

	return true;
}


//【部分和問題（列挙）】O(n M / 64)
/*
* 非負整数列 a[0..n) の部分和として i∈[0..M) が作れるかを格納したリストを返す．
*/
template <size_t M>
bitset<M> enumerate_partial_sum(const vi& a) {
	// verify : https://algo-method.com/tasks/337

	int n = sz(a);

	// dp_i[j] : a[0..i) の中で和をちょうど j にできるか
	bitset<M> dp;
	dp[0] = 1;

	// インライン配る DP
	rep(i, n) dp |= dp << a[i];

	return dp;
}


//【部分和問題（ペア，列挙）】O(n m / 64)
/*
* 与えられた非負整数列 a[0..n), b[0..n) について，各 i について a[i] と b[i] の
* いずれか一方を選んで得られる和として j が作れるかを格納したリストを返す．
*/
template <size_t N>
bitset<N> enumerate_pair_partial_sum(const vi& a, const vi& b) {
	// verify : https://atcoder.jp/contests/arc099/tasks/arc099_c

	int n = sz(a);

	// dp_i[j] : a[0..i), b[0..i) の中で和をちょうど j にできるか
	bitset<N> dp;
	dp[0] = 1;

	// インライン配る DP
	rep(i, n) dp = (dp << b[i]) | (dp << a[i]);

	return dp;
}


//【部分和問題（個数最小化）】O(n v)
/*
* 与えられた非負整数列 a[0..n) に対し，各 v∈[0..V] について，
* a[0..n) の部分和として v を作るのに必要な最小個数のリストを返す（不可能なら INF）
*
*（和を状態にもつインライン状態 DP）
*/
vi minimize_partial_sum(const vi& a, int V) {
	// verify : https://mojacoder.app/users/radix_sort/problems/wonder-wander

	int n = sz(a);

	// dp_i[j] : a[0..i) の中で和が j となる最小個数
	vi dp(V + 1, INF);
	dp[0] = 0; // 空和が 0 であることに対応

	// 配る DP
	rep(i, n) repir(j, V - a[i], 0) {
		// a[i] を選ばない場合はそのまま

		// a[i] を選ぶ場合
		chmin(dp[j + a[i]], dp[j] + 1);
	}

	return dp;
}


//【部分和問題（個数が小，数え上げ）】O(2^(n/2) n)
/*
* 長さ n の非負整数の列 a の部分和として v を作る方法が何通りあるかを返す．
*
*（半分全列挙）
*/
ll count_partial_sum(const vl& a, ll v) {
	int n = sz(a);

	// 前半と後半の要素数
	int n1 = n / 2, n2 = n - n1;

	// cnt[v] : 前半で部分和 v を作る方法の数
	unordered_map<ll, int> cnt;

	repb(set1, n1) {
		// 前半の部分和の計算
		ll sum1 = 0;
		rep(i, n1) if (set1 & (1 << i)) sum1 += a[i];

		cnt[sum1]++;
	}

	ll res = 0;
	repb(set2, n2) {
		// 後半の部分和の計算
		ll sum2 = 0;
		rep(i, n2) if (set2 & (1 << i)) sum2 += a[n1 + i];

		// 前半の部分和で v - sum2 になるものがあれば，合わせて部分和が v となる．
		res += cnt[v - sum2];
	}

	return res;
}


//【部分和問題（個数が小，存在判定）】O(2^(n/2))
/*
* 長さ n の非負整数の列 a の部分和として v が作れるかを返す．
*
*（半分全列挙）
*/
template <class T>
bool partial_sum(const vector<T>& a, T v) {
	// 参考 : https://qiita.com/keymoon/items/6cf46473b5421bfe1d48
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_cm

	int n = sz(a);

	// 前半の要素数
	int n1 = n / 2;

	// 前半の要素の部分和
	T sum1 = 0;

	// 前半の要素の部分和を記憶しておくリスト
	unordered_set<T> sum1s;

	// i = 0 に対応する処理
	sum1s.insert(0);

	// グレイコードを用いた差分更新を行うため，i = 1 からループを回す．
	repi(i, 1, (1 << n1) - 1) {
		// 差分更新が行われるのがどのビットか
		int change_index = lsb(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) sum1 += a[change_index];
		else sum1 -= a[change_index];

		// リストに追加
		sum1s.insert(sum1);
	}


	// 後半の要素数
	int n2 = n - n1;

	// 後半の要素の部分和
	T sum2 = 0;

	// i = 0 に対応する処理
	if (sum1s.count(v)) return true;

	// グレイコードを用いた差分更新を行うため，i = 1 からループを回す．
	repi(i, 1, (1 << n2) - 1) {
		// 差分更新が行われるのがどのビットか
		int change_index = lsb(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) sum2 += a[n1 + change_index];
		else sum2 -= a[n1 + change_index];

		// 前半のリストに v - sum2 があれば，合わせて部分和が v となる．
		if (sum1s.count(v - sum2)) return true;
	}

	// ここまで回ってきたなら部分和が v にならない．
	return false;
}


//【部分和問題（個数が小，復元）】O(2^(n/2))
/*
* 非負整数列 a[0..n) について，Σi∈S a[i] = v なる添字集合 S を is に格納する．
* S が存在しなければ false を返す．
*
*（半分全列挙）
*/
template <class T>
bool construction_partial_sum_large(const vector<T>& a, T v, vi& is) {
	// verify : https://yukicoder.me/problems/no/2081
	
	int n = sz(a);
	is.clear();

	// 前半の要素数
	int n1 = n / 2;

	// 前半の要素の部分和
	T sum1 = 0;

	// 前半の要素の部分和 → 添字集合の一例
	unordered_map<T, int> sum_to_set1;

	// i = 0 に対応する処理
	sum_to_set1[0] = 0;

	// グレイコードを用いた差分更新を行うため，i = 1 からループを回す．
	repi(i, 1, (1 << n1) - 1) {
		// 差分更新が行われるのがどのビットか
		int change_index = lsb(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) sum1 += a[change_index];
		else sum1 -= a[change_index];

		// リストに追加
		sum_to_set1[sum1] = gray_code;
	}

	// 後半の要素数
	int n2 = n - n1;

	// 後半の要素の部分和
	T sum2 = 0;

	// i = 0 に対応する処理
	if (sum_to_set1.count(v)) {
		int set1 = sum_to_set1[v];

		rep(i, n1) if (set1 & (1 << i)) is.emplace_back(i);

		return true;
	}

	// グレイコードを用いた差分更新を行うため，i = 1 からループを回す．
	repi(i, 1, (1 << n2) - 1) {
		// 差分更新が行われるのがどのビットか
		int change_index = lsb(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) sum2 += a[n1 + change_index];
		else sum2 -= a[n1 + change_index];

		// 前半のリストに v - sum2 があれば，合わせて部分和が v となる．
		if (sum_to_set1.count(v - sum2)) {
			int set1 = sum_to_set1[v - sum2];

			rep(i, n1) if (set1 & (1 << i)) is.emplace_back(i);
			rep(i, n2) if (gray_code & (1 << i)) is.emplace_back(n1 + i);

			return true;
		}
	}

	// ここまで回ってきたなら部分和が v にならない．
	return false;
}


//【部分和問題（倍数，存在判定）】O(n m)
/*
* 非負整数列 a[0..n) と正整数 m，r∈[0..m) について，
* a の部分列で和が m で割って r 余る数になるものが存在するかを返す．
*
*（mod m で和を状態にもつ状態 DP）
*/
template <class T>
bool multiple_partial_sum(const vector<T>& a, int m, int r) {
	// varify : https://algo-method.com/tasks/352

	int n = sz(a);

	// dp[i][j] : a[0..i) の中で和が j mod m にできるか
	vvb dp(n + 1, vb(m));
	dp[0][0] = true; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		rep(j, m) {
			// i 番目の数を選ばない場合
			dp[i + 1][j] = dp[i][j];

			// i 番目の数を選ぶ場合
			dp[i + 1][j] = dp[i + 1][j] | dp[i][smod<T>(j - a[i], m)];
		}
	}

	return dp[n][r];
}


//【部分和問題（倍数，数え上げ）】O(n m)
/*
* 非負整数列 a[0..n) と正整数 m について，a の部分列で和が m の倍数になるものの個数を返す．
*
*（mod m で和を状態にもつ状態 DP）
*/
template <class T>
mint count_multiple_partial_sum(const vector<T>& a, int m) {
	int n = sz(a);

	// dp[i][j] : a[0..i) の中で和が j mod m という状態をとる場合の数
	vvm dp(n + 1, vm(m));
	dp[0][0] = 1; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		rep(j, m) {
			// i 番目の数を選ばない場合
			dp[i + 1][j] = dp[i][j];

			// i 番目の数を選ぶ場合
			dp[i + 1][j] += dp[i][((j - a[i]) % m + m) % m];
		}
	}

	return dp[n][0];
}


//【部分和問題（無限個，数え上げ）】O(n v)
/*
* 正整数列 a[0..n) の部分和として各 i∈[0..v] を作る方法が何通りあるかを格納したリストを返す．
* 各 a[i] は [0..∞) 個用いることができる．
*/
vm count_unlimited_partial_sum(const vi& a, int v) {
	// verify : https://projecteuler.net/problem=495

	int n = sz(a);

	// dp_i[j] : a[0..i) の中で部分和が j になる組合せの数
	vm dp(v + 1);
	dp[0] = 1; // 空和が 0 であることに対応

	// インライン配る DP
	rep(i, n) {
		Assert(a[i] > 0);

		repi(j, 0, v) {
			// a[i] を選ばない場合はインライン DP なので何もしなくて良い．

			// a[i] を選ぶ場合（j のループを昇順にしているので何個でも選べることになる）
			if (j + a[i] <= v) dp[j + a[i]] += dp[j];
		}
	}

	return dp;
}


//【部分和問題（無限個，数え上げ，mod 998244353）】O(n + v log v)
/*
* 正整数列 a[0..n) の部分和として各 i∈[0..v] を作る方法が何通りあるかを格納したリストを返す．
* 各 a[i] は [0..∞) 個用いることができる．
*
* 制約：fm は 2(v+1)! まで計算可能
*
* 利用：【形式的冪級数】,【指数関数】
*/
vm count_unlimited_partial_sum_fps(const vi& a, int v, const Factorial_mint& fm) {
	// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a

	//【方法】
	// 母関数は
	//		f(z) = Πi∈[0..n) 1/(1 - z^a[i])
	// であるが，これは
	//		f(z) = exp(Σi=[0..n) -log(1 - z^a[i]))
	// と書き直せる．対数関数のマクローリン展開の式より
	//		-log(1 - z^a[i]) = Σk∈[1..∞) 1/k z^(k a[i])
	// であり，これはスパースなので高速に和が計算できる．

	// cnt[v] : a[0..n) の中に v が何個含まれるか
	unordered_map<int, int> cnt;
	repe(x, a) {
		Assert(x > 0);
		cnt[x]++;
	}

	MFPS f(0, v + 1);
	repe(p, cnt) {
		for (int k = 1; k * p.first <= v; k++) {
			f[k * p.first] += p.second * fm.inv(k);
		}
	}
	f = exp_fps(f, v + 1, fm);

	return f.c;
}


//【部分和問題（無限個，数え上げ）】O(A n log v) （A = Σa）
/*
* 正整数列 a[0..n) の部分和として v を作る方法が何通りあるかを返す．
* 各 a[i] は [0..∞) 個用いることができる．
*
* 利用：【展開係数（分母が二項式の積）】
*/
mint count_unlimited_partial_sum(const vi& a, ll v) {
	// verify : https://yukicoder.me/problems/no/137

	//【方法】
	// 母関数は
	//		f(z) = Πi=[0..n) 1/(1 - z^a[i])
	// であり，分母が二項式の積であることを利用してボスタン-森法で高速に第 v 項を計算できる．

	int n = sz(a);
	vector<pim> dcs(n);
	rep(i, n) {
		Assert(a[i] > 0);
		dcs[i] = { a[i], -1 };
	}

	return bostan_mori(vm{ 1 }, dcs, v);
}


//【部分和問題（無限個，個数最小化）】O(n V)
/*
* 与えられた非負整数列 a[0..n) に対し，各 v∈[0..V] について，
* a[0..n) の部分和（無限個使用可）として v を作るのに必要な最小個数のリストを返す（不可能なら INF）
*
*（和を状態にもつインライン状態 DP）
*/
vi minimize_unlimited_partial_sum(const vi& a, int V) {
	// verify : https://atcoder.jp/contests/arc178/tasks/arc178_c

	int n = sz(a);

	//【備考】
	// コイン両替問題と解釈できる．

	// dp_i[j] : a[0..i) の中で和が j となる最小個数
	vi dp(V + 1, INF);
	dp[0] = 0; // 空和が 0 であることに対応

	// 配る DP
	rep(i, n) repi(j, 0, V - a[i]) {
		// a[i] を選ばない場合はそのまま

		// a[i] を選ぶ場合
		chmin(dp[j + a[i]], dp[j] + 1);
	}

	return dp;
}


//【部分和問題（個数制限，存在判定）】O(n v)
/*
* 長さ n の正整数列 a の部分和として各 j∈[0..v] を作れるかを格納したリストを返す．
* 各 a[i] は [0..m[i]] 個用いることができる．
*/
vb limited_partial_sum(const vi& a, const vi& m, int v) {
	// 参考 : https://algo-method.com/tasks/313/editorial
	// verify : https://atcoder.jp/contests/abc286/tasks/abc286_d

	int n = sz(a);
	vb able(v + 1);

	// dp_i[j] : a[0..i) の中で和をちょうど j にするときの a[i-1] の個数の最小値
	vi dp(v + 1, INF);
	dp[0] = 0; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		// m[i] = 0 なら何もしない
		if (m[i] == 0) continue;

		vi ndp(v + 1, INF);

		repi(j, 0, v) {
			// i 番目の数を選ばない場合
			if (dp[j] != INF) chmin(ndp[j], 0);

			if (j >= a[i]) {
				// i 番目の数を新たに選ぶ場合
				if (dp[j - a[i]] != INF) chmin(ndp[j], 1);

				// i 番目の数を追加で選ぶ場合
				if (ndp[j - a[i]] < m[i]) chmin(ndp[j], ndp[j - a[i]] + 1);
			}
		}

		dp = move(ndp);
	}

	repi(j, 0, v) able[j] = (dp[j] != INF);

	return able;
}


//【部分和問題（個数制限，数え上げ）】O(n v)
/*
* 長さ n の正整数列 a の部分和として v を作る方法が何通りあるかを返す．
* 各 a[i] は [0..m[i]] 個用いることができる．
*
*（和を状態にもつ状態 DP）
*/
mint count_limited_partial_sum(const vi& a, const vi& m, int v) {
	// 参考 : https://betrue12.hateblo.jp/entry/2020/10/05/124052
	// verify : https://atcoder.jp/contests/abc286/tasks/abc286_d

	int n = sz(a);

	// dp[i][j] : a[0..i) の中で和がちょうど j という状態をとる場合の数
	vvm dp(n + 1, vm(v + 1));
	dp[0][0] = 1; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		repi(j, 0, v) {
			// i 番目の数を選ばない場合
			dp[i + 1][j] = dp[i][j];

			if (j >= a[i]) {
				// i 番目の数を選ぶ場合
				dp[i + 1][j] += dp[i + 1][j - a[i]];
			}

			if (j >= a[i] * (m[i] + 1)) {
				// i 番目の数を m[i] 個より多く選んでしまうケースも数えているので，
				// その影響を相殺するように負号を付けて足し込んでおく．
				dp[i + 1][j] -= dp[i][j - a[i] * (m[i] + 1)];
			}
		}
	}

	return dp[n][v];
}


//【部分和問題（負値可，数え上げ）】O(n Σ|a[i]|)
/*
* 整数列 a[0..n) の部分和として v を作る方法の数を cnt[v - MIN] に格納し，MIN を返す．
*
*（和を状態にもつインライン貰う DP）
*/
int count_signed_partial_sum(const vi& a, vm& cnt) {
	int n = sz(a);

	// MIN, MAX : 部分和の最小値および最大値
	int MIN = 0, MAX = 0;
	rep(i, n) {
		MIN += min(a[i], 0);
		MAX += max(a[i], 0);
	}

	// cnt_i[j - MIN] : a[0..i) の中で和がちょうど j になる部分集合の個数
	cnt.assign(MAX - MIN + 1, 0);
	cnt[0 - MIN] = 1; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		// a[i] の符号で走査方向を変える
		if (a[i] >= 0) {
			repir(j, MAX, MIN + a[i]) {
				// i 番目の数を選ぶ場合
				cnt[j - MIN] += cnt[j - a[i] - MIN];
			}
		}
		else if (a[i] < 0) {
			repi(j, MIN, MAX + a[i]) {
				// i 番目の数を選ぶ場合
				cnt[j - MIN] += cnt[j - a[i] - MIN];
			}
		}
	}

	return MIN;
}


//【部分和問題（負値可，列挙）】O(n M / 64)
/*
* 整数列 a[0..n) の部分和として v を作れるかを ex[v - min(a)] に格納し，min(a) を返す．
*/
template <size_t M>
int enumerate_signed_partial_sum(const vi& a, bitset<M>& ex) {
	int n = sz(a);
	int MIN = min(*min_element(all(a)), 0);

	// ex_i[j - MIN] : a[0..i) の中で和がちょうど j になる部分集合があるか
	ex.reset();
	ex[0 - MIN] = 1; // 空和が 0 であることに対応

	// インライン配る DP
	rep(i, n) {
		if (a[i] > 0)		ex |= ex << a[i];
		else if (a[i] < 0)	ex |= ex >> (-a[i]);
	}

	return MIN;
}


//【部分和問題（負値可，個数最小化）】O(n Σ|a[i]|)
/*
* 整数列 a[0..n) の部分和として v を作るのに必要な要素数の最小値を cnt[v - MIN] に格納し，MIN を返す．
*
*（和を状態にもつインライン貰う DP）
*/
int minimize_signed_partial_sum(const vi& a, vi& cnt) {
	int n = sz(a);

	// MIN, MAX : 部分和の最小値および最大値
	int MIN = 0, MAX = 0;
	rep(i, n) {
		MIN += min(a[i], 0);
		MAX += max(a[i], 0);
	}

	// cnt_i[j - MIN] : a[0..i) の中で和がちょうど j になる最小個数（なければ INF）
	cnt.assign(MAX - MIN + 1, INF);
	cnt[0 - MIN] = 0; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		// a[i] の符号で走査方向を変える
		if (a[i] > 0) {
			repir(j, MAX, MIN + a[i]) {
				// i 番目の数を選ぶ場合
				chmin(cnt[j - MIN], cnt[j - a[i] - MIN] + 1);
			}
		}
		else if (a[i] < 0) {
			repi(j, MIN, MAX + a[i]) {
				// i 番目の数を選ぶ場合
				chmin(cnt[j - MIN], cnt[j - a[i] - MIN] + 1);
			}
		}
	}

	return MIN;
}


//【部分和問題（負値可，個数制限，数え上げ）】O(n Σm[i]|a[i]|)
/*
* 長さ n の整数列 a の部分和として v を作る方法が何通りあるかを返す．
* 各 a[i] は [0..m[i]] 個用いることができる．
*
*（和を状態にもつ状態 DP）
*/
mint count_limited_signed_partial_sum(vi& a, const vi& m, int v) {
	// 参考 : https://betrue12.hateblo.jp/entry/2020/10/05/124052
	// verify : https://atcoder.jp/contests/arc104/tasks/arc104_d

	int n = sz(a);

	// MIN, MAX : 部分和の最小値および最大値
	int MIN = 0, MAX = 0;
	rep(i, n) {
		MIN += min(a[i], 0) * m[i];
		MAX += max(a[i], 0) * m[i];
	}

	// v が範囲外ならもちろん 0 通り
	if (v < MIN || MAX < v) {
		return 0;
	}

	// 適切にソートすることで考えるべき範囲を縮小する．
	if (-MIN < MAX) {
		// 昇順にならべれば，和が max(0, v) を超えたら枝刈りできる．
		sort(all(a));
		MAX = max(0, v);
	}
	else {
		// 降順にならべれば，和が min(0, v) より小さくなったら枝刈りできる．
		sort(all(a), greater<int>());
		MIN = min(0, v);
	}

	// dp[i][j - MIN] : a[0..i) の中で和がちょうど j という状態をとる場合の数
	vvm dp(n + 1, vm(MAX - MIN + 1));
	dp[0][0 - MIN] = 1; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		// a[i] の符号で場合分け
		if (a[i] > 0) {
			repi(j, MIN, MAX) {
				// i 番目の数を選ばない場合
				dp[i + 1][j - MIN] = dp[i][j - MIN];

				// i 番目の数を選ぶ場合
				if (j - a[i] >= MIN) {
					dp[i + 1][j - MIN] += dp[i + 1][j - a[i] - MIN];
				}

				// i 番目の数を m[i] 個より多く選んでしまうケースも数えているので，
				// その影響を相殺するように負号を付けて足し込んでおく．
				if (j - a[i] * (m[i] + 1) >= MIN) {
					dp[i + 1][j - MIN] -= dp[i][j - a[i] * (m[i] + 1) - MIN];
				}
			}
		}
		else if (a[i] < 0) {
			repir(j, MAX, MIN) {
				// i 番目の数を選ばない場合
				dp[i + 1][j - MIN] = dp[i][j - MIN];

				// i 番目の数を選ぶ場合
				if (j - a[i] <= MAX) {
					dp[i + 1][j - MIN] += dp[i + 1][j - a[i] - MIN];
				}

				// i 番目の数を m[i] 個より多く選んでしまうケースも数えているので，
				// その影響を相殺するように負号を付けて足し込んでおく．
				if (j - a[i] * (m[i] + 1) <= MAX) {
					dp[i + 1][j - MIN] -= dp[i][j - a[i] * (m[i] + 1) - MIN];
				}
			}
		}
		else {
			repi(j, MIN, MAX) {
				dp[i + 1][j - MIN] = dp[i][j - MIN] * (m[i] + 1);
			}
		}
	}

	return dp[n][v - MIN];
}


//【部分和問題（負値可，個数制限，個数最小化）】O(A√A + n)（A = Σm[i]|a[i]|）
/*
* 長さ n の整数列 a の部分和として v を作るのに必要な要素数の最小値を cnt[v - MIN] に格納し，MIN を返す．
* 各 a[i] は [0..m[i]] 個用いることができる．
*
*（和を状態にもつインライン貰う DP）
*/
int minimize_limited_signed_partial_sum(const vi& a_, const vi& m, vi& cnt) {
	// verify : https://atcoder.jp/contests/abc269/tasks/abc269_g

	// 要素を 1, 2, 4, 8, ... 個ずつセットにして 1 つの要素とみなす．
	// a[i], w[i] : i 番目のセットの値とセットにした要素の数
	vi a, w;
	rep(i, sz(a_)) {
		int mi = m[i], c = 1;
		while (c < mi) {
			a.emplace_back(a_[i] * c);
			w.emplace_back(c);

			mi -= c;
			c *= 2;
		}
		a.emplace_back(a_[i] * mi);
		w.emplace_back(mi);
	}
	dump(a); dump(w);

	int n = sz(a);

	// MIN, MAX : 部分和の最小値および最大値
	int MIN = 0, MAX = 0;
	rep(i, n) {
		MIN += min(a[i], 0);
		MAX += max(a[i], 0);
	}
	dump(MIN, MAX);

	// cnt_i[j - MIN] : a[0..i) の中で和がちょうど j になる最小個数（なければ INF）
	cnt.assign(MAX - MIN + 1, INF);
	cnt[0 - MIN] = 0; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		// a[i] の符号で走査方向を変える
		if (a[i] > 0) {
			repir(j, MAX, MIN + a[i]) {
				// i 番目の数を選ぶ場合
				chmin(cnt[j - MIN], cnt[j - a[i] - MIN] + w[i]);
			}
		}
		else if (a[i] < 0) {
			repi(j, MIN, MAX + a[i]) {
				// i 番目の数を選ぶ場合
				chmin(cnt[j - MIN], cnt[j - a[i] - MIN] + w[i]);
			}
		}
	}

	return MIN;
}


//【部分和問題（貪欲，数え上げ）】O(n v)
/*
* 長さ n の整数列 a の部分和として v 以下の数を作る方法が何通りあるかを返す．
* ただし，まだ加えられる a[i] が残った状態で加えるのをやめてはいけない．
*
*（和を状態にもつ状態 DP）
*/
mint count_greedy_partial_sum(vi a, int v) {
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/2333

	int n = sz(a);

	// 重さについて降順ソートしておく
	sort(all(a), greater<int>());

	// 全部足せる場合
	if (accumulate(all(a), 0) <= v) return 1;

	// dp[i][j] : a[0..i) の中から和が j 以下で運ぶ方法の数
	vvm dp(n + 1, vm(v + 1));
	repi(j, 0, v) dp[0][j] = 1;

	repi(i, 1, n) {
		repi(j, 0, v) {
			dp[i][j] += dp[i - 1][j];

			if (j - a[i - 1] >= 0) {
				dp[i][j] += dp[i - 1][j - a[i - 1]];
			}
		}
	}

	// a[n-1] を使わない場合は，a[0..n-1) の中から和が (v-a[n-1]..v] 内になるよう選び，
	// 以降は a[n-1] を入れる場合のみを数え上げる．
	// 同様のことを後ろから順番に行っていく．
	mint res = 0; int s = 0;
	repir(i, n - 1, 0) {
		if (v - s >= 0) res += dp[i][v - s];
		if (v - s - a[i] >= 0) res -= dp[i][v - s - a[i]];

		s += a[i];
	}

	return res;
}


