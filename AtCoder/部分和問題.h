#pragma once
#include "header.h"
#include "二項係数.h"
#include "FPS.h"
// ■■■■■ 部分和問題 ■■■■■



//【部分和問題（数え上げ）】O(n v)
/*
* 長さ n の非負整数の列 a の部分和として v を作る方法が何通りあるかを返す．
* 
*（和を状態にもつ状態 DP）
*/
mint count_partial_sum(const vi& a, int v) {
	// 参考 : https://qiita.com/suisen_cp/items/794f24d31852b97d58a6

	int n = sz(a);

	// dp[i][j] : a[0..i) の中で和がちょうど j という状態をとる場合の数
	vvm dp(n + 1, vm(v + 1));
	dp[0][0] = 1; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		repi(j, 0, v) {
			// i 番目の数を選ばない場合
			dp[i + 1][j] = dp[i][j];

			// i 番目の数が j より大きいと選べない．
			if (j < a[i]) {
				continue;
			}

			// i 番目の数を選ぶ場合を加算する．
			dp[i + 1][j] += dp[i][j - a[i]];
		}
	}

	return dp[n][v];
}


//【部分和問題（数え上げ）】O(n + v log v)
/*
* 各 j=[0..v] について，長さ n の正整数の列 a の部分和として j を作る方法が
* 何通りあるかを cnt[j] に格納する．
*
* 利用：【形式的冪級数】，【階乗と二項係数（mint利用）】
*/
void count_partial_sum(const vi& a, int v, vm& cnt) {
	// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a

	//【方法】
	// 母関数は
	//		f(x) = Πi=[0..n) (1 + x^a[i])
	// であるが，これは
	//		f(x) = exp(Σi=[0..n) log(1 + x^a[i]))
	// と書き直せる．対数関数のマクローリン展開の式より
	//		log(1 + x^a[i]) = Σk=[1..∞) (-1)^(k-1) 1/k x^(k * a[i])
	// であり，これはスパースなので高速に和が計算できる．

	factorial_mint fm(v);

	unordered_map<int, int> c;
	repe(x, a) c[x]++;

	FPS f(0, v + 1);
	repe(p, c) {
		for (int k = 1; k * p.first <= v; k++) {
			f[(ll)k * p.first] += p.second * (k & 1 ? 1 : -1) * fm.inv(k);
		}
	}
	f = exp(f, v + 1);
	cnt = f.c;
}


//【部分和問題（列挙）】O(n v)（v = Σa[i]）
/*
* 長さ n の非負整数の列 a の部分和として作れる数を列挙する．
*
* bitset<A> set : A は Σa[i] + 1 以上の定数．i が作れるなら res[i] = true．
* 
*（ビット演算で高速化した bool DP）
*/
template <size_t A>
void enumerate_partial_sum(const vi& a, bitset<A>& res) {
	int n = sz(a);

	// res_i[j] : i 番目までの数で，和をちょうど j にできるかどうか
	res.reset();
	res[0] = 1;

	// 配る DP
	rep(i, n) {
		res = res | (res << a[i]);
	}
}


//【部分和問題（個数最小化）】O(n v)
/*
* 長さ n の非負整数の列 a の部分和として v を作るために必要な要素の最小個数を返す．
* a の部分和として v が作れないなら INF を返す．
*
*（和を状態にもつ状態 DP）
*/
int minimize_partial_sum(const vi& a, int v) {
	int n = sz(a);

	// dp[i][j] : a[0..i) の中で和がちょうど j を実現できる最小個数
	vvi dp(n + 1, vi(v + 1, INF));
	dp[0][0] = 0; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		repi(j, 0, v) {
			// i 番目の数を選ばない場合
			dp[i + 1][j] = dp[i][j];

			// i 番目の数が j より大きいと選べない．
			if (j < a[i]) {
				continue;
			}

			// i 番目の数を選ぶ場合
			chmin(dp[i + 1][j], dp[i][j - a[i]] + 1);
		}
	}

	return dp[n][v];
}


//【部分和問題】O(2^(n/2))
/*
* 長さ n の非負整数の列 a の部分和として v が作れるかを返す．
*
*（半分全列挙）
*/
template <class T>
bool partial_sum(const vector<T>& a, T v) {
	// 参考：https://qiita.com/keymoon/items/6cf46473b5421bfe1d48

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
		int change_index = ctz(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) {
			sum1 += a[change_index];
		}
		else {
			sum1 -= a[change_index];
		}

		// リストに追加
		sum1s.insert(sum1);
	}
	dump(sum1s);


	// 後半の要素数
	int n2 = n - n1;

	// 後半の要素の部分和
	T sum2 = 0;

	// i = 0 に対応する処理
	if (sum1s.count(v)) {
		return true;
	}

	// グレイコードを用いた差分更新を行うため，i = 1 からループを回す．
	repi(i, 1, (1 << n2) - 1) {
		// 差分更新が行われるのがどのビットか
		int change_index = ctz(i);

		// i 番目のグレイコード
		int gray_code = i ^ (i >> 1);

		// グレイコードのビットを見て加算か減算かを判断
		if (gray_code & (1 << change_index)) {
			sum2 += a[n1 + change_index];
		}
		else {
			sum2 -= a[n1 + change_index];
		}

		// 前半のリストに v - sum2 があれば，合わせて部分和が v となる．
		if (sum1s.count(v - sum2)) {
			return true;
		}
	}

	// ここまで回ってきたなら部分和が v にならない．
	return false;
}


//【部分和問題（倍数，存在判定）】O(n m)
/*
* 長さ n の非負整数の列 a と正整数 m，r∈[0..m) について，
* a の部分列で和が m で割って r 余る数になるものが存在するかを返す．
*
*（mod m で和を状態にもつ状態 DP）
*/
template <class T>
bool multiple_partial_sum(const vector<T>& a, int m, int r) {
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
			dp[i + 1][j] = dp[i + 1][j] | dp[i][((j - a[i]) % m + m) % m];
		}
	}

	return dp[n][r];
}


//【部分和問題（倍数，数え上げ）】O(n m)
/*
* 長さ n の非負整数の列 a と正整数 m について，
* a の部分列で和が m の倍数になるものの個数を返す．
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


//【部分和問題（個数制限なし，数え上げ）】O(n v)
/*
* 長さ n の正整数の列 a の部分和として v を作る方法が何通りあるかを返す．
* 各 a[i] は 0 個以上の任意個用いることができる．
*
*（和を状態にもつ状態 DP）
*/
mint count_unlimited_partial_sum(const vi& a, int v) {
	// 参考 : https://qiita.com/suisen_cp/items/794f24d31852b97d58a6

	int n = sz(a);

	// dp[i][j] : a[0..i) の中で和がちょうど j という状態をとる場合の数
	vvm dp(n + 1, vm(v + 1));
	dp[0][0] = 1; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		repi(j, 0, v) {
			// i 番目の数を選ばない場合
			dp[i + 1][j] = dp[i][j];

			// i 番目の数が j より大きいと選べない．
			if (j < a[i]) {
				continue;
			}

			// i 番目の数を選ぶ場合
			dp[i + 1][j] += dp[i + 1][j - a[i]];
		}
	}

	return dp[n][v];
}


//【部分和問題（個数制限なし，個数最小化）】O(n v)
/*
* 長さ n の正整数列 a の部分和として v を作るために必要な最小要素数を返す．
* 各 a[i] は [0..∞) 個用いることができる．
*
* コイン両替問題としても知られる．
*
*（和を状態にもつ状態 DP）
*/
int minimize_unlimited_partial_sum(const vi& a, int v) {
	int n = sz(a);

	// dp[i][j] : a[0..i) の中で和が j となる最小個数
	vvi dp(n + 1, vi(v + 1, INF));
	dp[0][0] = 0; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		repi(j, 0, v) {
			// i 番目の数を選ばない場合
			dp[i + 1][j] = dp[i][j];

			// i 番目の数が j より大きいと選べない．
			if (j < a[i]) {
				continue;
			}

			// i 番目の数を選ぶ場合
			chmin(dp[i + 1][j], dp[i + 1][j - a[i]] + 1);
		}
	}

	return dp[n][v];
}


//【部分和問題（個数制限付き，存在判定）】O(n v)
/*
* 長さ n の正整数列 a の部分和として v を作れるかを返す．
* 各 a[i] は [0..m[i]] 個用いることができる．
*
*（和を状態にもつ状態 DP）
*/
bool limited_partial_sum(const vi& a, const vi& m, int v) {
	// 参考 : https://algo-method.com/tasks/313/editorial

	int n = sz(a);

	// dp[i][j] : a[0..i) の中で和をちょうど j にするときの a[i-1] の個数の最小値
	vvi dp(n + 1, vi(v + 1, INF));
	dp[0][0] = 0; // 空和が 0 であることに対応

	// 貰う DP
	rep(i, n) {
		repi(j, 0, v) {
			// i 番目の数を選ばない場合
			if (dp[i][j] != INF)
				chmin(dp[i + 1][j], 0);
			else
				chmin(dp[i + 1][j], INF);

			// i 番目の数を選べる場合
			if (j >= a[i]) {
				// i 番目の数を新たに選ぶ場合
				if (dp[i][j - a[i]] != INF)
					chmin(dp[i + 1][j], 1);
				else
					chmin(dp[i + 1][j], INF);

				// i 番目の数を追加で選ぶ場合
				if (dp[i + 1][j - a[i]] < m[i])
					chmin(dp[i + 1][j], dp[i + 1][j - a[i]] + 1);
				else
					chmin(dp[i + 1][j], INF);
			}
		}
	}

	return dp[n][v] != INF;
}


//【部分和問題（個数制限付き，数え上げ）】O(n v)
/*
* 長さ n の正整数列 a の部分和として v を作る方法が何通りあるかを返す．
* 各 a[i] は [0..m[i]] 個用いることができる．
*
*（和を状態にもつ状態 DP）
*/
mint count_limited_partial_sum(const vi& a, const vi& m, int v) {
	// 参考 : https://betrue12.hateblo.jp/entry/2020/10/05/124052

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


//【部分和問題（負値可，個数制限付き，数え上げ）】O(n Σ|a[i]m[i]|)
/*
* 長さ n の整数列 a の部分和として v を作る方法が何通りあるかを返す．
* 各 a[i] は [0..m[i]] 個用いることができる．
*
*（和を状態にもつ状態 DP）
*/
mint count_limited_signed_partial_sum(const vi& a, const vi& m, int v) {
	// 参考 : https://betrue12.hateblo.jp/entry/2020/10/05/124052

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


