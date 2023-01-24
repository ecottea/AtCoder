#pragma once
#include "header.h"
// ■■■■■ 桁 DP（組） ■■■■■
 
 
//【組の上から桁 DP，未満フラグ，数え上げ】O(max(n1, n2) b^2)
/*
* b=10 進数で n1 桁の非負整数 num1 と n2 桁の非負整数 num2 について，
* d1 ≦ num1, d2 ≦ num2 なる非負整数の組 (d1, d2) の個数を返す．
*/
mint count_pair(string num1, string num2, int b = 10) {
	int n = max(sz(num1), sz(num2));

	// 0 埋め
	ostringstream sout1, sout2;
	sout1 << setfill('0') << setw(n) << num1;
	sout2 << setfill('0') << setw(n) << num2;
	num1 = sout1.str();
	num2 = sout2.str();

	// dp[i][f] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d1[0..i), d2[0..i) まで決まっている．
	//	f : d1[0..i) < num1[0..i) なら 1，さもなくば 0（未満フラグ 1）
	//	    d2[0..i) < num2[0..i) なら 2，さもなくば 0（未満フラグ 2）
	//		f はこれら 2 つのフラグの OR をとったもの
	vvm dp(n + 1, vm(1LL << 2));
	dp[0][0 | 0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x1[x2] : num1[num2] の上から i 桁目の数
		int x1 = num1[i] - '0';
		int x2 = num2[i] - '0';

		repb(f, 2) {
			int smaller1 = (f >> 0) & 1;
			int smaller2 = (f >> 1) & 1;

			// d1_max[d2_max] : d1[i][ d2[i] ] のとれる値の最大値
			int d1_max = (smaller1 ? b - 1 : x1);
			int d2_max = (smaller2 ? b - 1 : x2);

			// d1[d2] : d1[i][ d2[i] ]
			repi(d1, 0, d1_max) {
				repi(d2, 0, d2_max) {
					int n_smaller1 = (int)(smaller1 || (d1 < d1_max));
					int n_smaller2 = (int)(smaller2 || (d2 < d2_max));
					int nf = (n_smaller1 << 0) | (n_smaller2 << 1);

					dp[i + 1][nf] += dp[i][f];
				}
			}
		}

		//dump(i + 1);
		//repb(f, 2) {
		//	dumps("(smaller2, smaller1) ="); dump(bitset<2>(f));
		//	dump(dp[i + 1][f]);
		//}
	}

	mint res = 0;
	repb(f, 2) res += dp[n][f];

	return res;
}


//【組の上から桁 DP，未満フラグ，比較フラグ，数え上げ】O(max(n1, n2) b^2)
/*
* b=10 進数で n 桁の非負整数 num について，
* d1 ≦ d2 ≦ num なる非負整数の組 (d1, d2) の個数を返す．
*/
mint count_unordered_pair(string num, int b = 10) {
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d1[0..i), d2[0..i) まで決まっている．
	//	f : d2[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//		d1[0..i) < d2[0..i) なら 2，さもなくば 0（比較フラグ）
	//		f はこれらのフラグの OR をとったもの
	vvm dp(n + 1, vm(1LL << 2));
	dp[0][0 | 0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 2) {
			int smaller = (f >> 0) & 1;
			int cmp = (f >> 1) & 1;

			// d1 : d1[i]
			repi(d1, 0, b - 1) {
				// d2 : d2[i]
				int d2_min = (cmp ? 0 : d1);
				int d2_max = (smaller ? b - 1 : x);
				repi(d2, d2_min, d2_max) {
					int n_smaller = (int)(smaller || (d2 < d2_max));
					int n_cmp = (int)(cmp || d1 < d2);
					int nf = (n_smaller << 0) | (n_cmp << 1);

					dp[i + 1][nf] += dp[i][f];
				}
			}
		}

		//dump("----", i + 1, "----");
		//repb(f, 2) dump("(cmp, smaller) =", bitset<2>(f), ":", dp[i + 1][f]);
	}

	mint res = 0;
	repb(f, 2) res += dp[n][f];

	return res;
}


//【組の上から桁 DP，超過フラグ，未満フラグ，比較フラグ，数え上げ】O(max(n1, n2) b^2)
/*
* b=10 進数で n1 桁の非負整数 num1 と n2 桁の非負整数 num2 について，
* num1 ≦ d1 ≦ d2 ≦ num2 なる非負整数の組 (d1, d2) の個数を返す．
*/
mint count_unordered_pair(string num1, string num2, int b = 10) {
	int n = max(sz(num1), sz(num2));

	// 0 埋め
	ostringstream sout1, sout2;
	sout1 << setfill('0') << setw(n) << num1;
	sout2 << setfill('0') << setw(n) << num2;
	num1 = sout1.str();
	num2 = sout2.str();

	// dp[i][f] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d1[0..i), d2[0..i) まで決まっている．
	//	f : d1[0..i) > num1[0..i) なら 1，さもなくば 0（超過フラグ）
	//	    d2[0..i) < num2[0..i) なら 2，さもなくば 0（未満フラグ）
	//		d1[0..i) < d2[0..i) なら 4，さもなくば 0（比較フラグ）
	//		f はこれらのフラグの OR をとったもの
	vvm dp(n + 1, vm(1LL << 3));
	dp[0][0 | 0 | 0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x1[x2] : num1[num2] の上から i 桁目の数
		int x1 = num1[i] - '0';
		int x2 = num2[i] - '0';

		repb(f, 3) {
			int greater1 = (f >> 0) & 1;
			int smaller2 = (f >> 1) & 1;
			int cmp = (f >> 2) & 1;

			// d1 : d1[i]
			int d1_min = (greater1 ? 0 : x1);
			repi(d1, d1_min, b - 1) {
				// d2 : d2[i]
				int d2_min = (cmp ? 0 : d1);
				int d2_max = (smaller2 ? b - 1 : x2);
				repi(d2, d2_min, d2_max) {
					int n_greater1 = (int)(greater1 || (d1 > d1_min));
					int n_smaller2 = (int)(smaller2 || (d2 < d2_max));
					int n_cmp = (int)(cmp || d1 < d2);
					int nf = (n_greater1 << 0) | (n_smaller2 << 1) | (n_cmp << 2);

					dp[i + 1][nf] += dp[i][f];
				}
			}
		}

		//dump(i + 1);
		//repb(f, 3) {
		//	dumps("(cmp, smaller2, greater1) ="); dump(bitset<3>(f));
		//	dump(dp[i + 1][f]);
		//}
	}

	mint res = 0;
	repb(f, 3) res += dp[n][f];

	return res;
}


//【組の下から桁 DP，以下フラグ，桁上げフラグ，数え上げ】O(n b^2)
/*
* b=10 進数で n 桁の数 num について，和が num 以下になる非負整数の組 (d1, d2) の個数を返す．
*/
mint count_pair_sum(const string& num, int b = 10) {
	// verify : https://atcoder.jp/contests/abc129/tasks/abc129_e

	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の個数：
	//	i : 下からの桁 d[i..n) まで決まっている（d = d1 + d2 とおく）
	//	f : d[i..n) <= num[i..n) なら 1，さもなくば 0（以下フラグ）
	//      d[i] から桁上げがあるなら 2，さもなくば 0（桁上げフラグ）
	//      f はこれら 2 つのフラグの OR をとったもの
	vvm dp(n + 1, vm(1LL << 2));
	dp[n][1 | 0] = 1;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		repb(f, 2) {
			int leq = (f >> 0) & 1;
			int carry = (f >> 1) & 1;

			// d1 : d1[i], d2 : d2[i]
			rep(d1, b) {
				rep(d2, b) {
					int d = d1 + d2 + carry;

					int n_leq = (d % b < x) || (d % b == x && leq);
					int n_carry = (d >= b);
					int nf = (n_leq << 0) | (n_carry << 1);

					dp[i][nf] += dp[i + 1][f];
				}
			}
		}

		//dump("----", i, "----");
		//repb(f, 2) dump("(carry, leq) =", bitset<2>(f), ":", dp[i][f]);
	}

	return dp[0][1 | 0];
}


//【組の下から桁 DP，以下フラグ，桁上げフラグ，比較フラグ，数え上げ】O(n b^2)
/*
* b=10 進数で n 桁の数 num について，和が num 以下になる非負整数の組 (d1, d2) で
* d1 ≦ d2 を満たすものの個数を返す．
*/
mint count_unordered_pair_sum(const string& num, int b = 10) {
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の個数：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//      ここで d = d1 + d2 である．
	//	f : d[i..n) <= num[i..n) なら 1，さもなくば 0（以下フラグ）
	//      d[i] から桁上げがあるなら 2，さもなくば 0（桁上げフラグ）
	//      d1[i..n) <= d2[i..n) なら 4，さもなくば 0（比較フラグ）
	//      f はこれら 3 つのフラグの OR をとったもの
	vvm dp(n + 1, vm(1LL << 3, 0));
	dp[n][1 | 0 | 4] = 1;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 8) {
			int leq = (f >> 0) & 1;
			int carry = (f >> 1) & 1;
			int cmp = (f >> 2) & 1;

			// d1 : d1[i], d2 : d2[i]
			rep(d1, b) {
				rep(d2, b) {
					int d = d1 + d2 + carry;

					int n_leq = (d % b < x) || (d % b == x && leq);
					int n_carry = (d >= b);
					int n_cmp = (d1 < d2) || (d1 == d2 && cmp);
					int nf = (n_leq << 0) | (n_carry << 1) | (n_cmp << 2);

					dp[i][nf] += dp[i + 1][f];
				}
			}
		}

		//dump("----", i, "----");
		//repb(f, 3) dump("(cmp, carry, leq) =", bitset<2>(f), ":", dp[i][f]);
	}

	return dp[0][1 | 0 | 4];
}


//【組の下から桁 DP，以下フラグ，桁上げフラグ，比較フラグ，スコア和】O(n b^2)
/*
* b 進数で n 桁の数 num について，和が num 以下になる非負整数の組 (d1, d2) で
* d1 ≦ d2 を満たすもの全てにおける繰り上がり回数の総和を返す．
*/
mint carry_sum_unordered_pair_sum(const string& num, int b = 10) {
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の繰り上がり回数：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//      ここで d = d1 + d2 である．
	//	f : d[i..n) <= num[i..n) なら 1，さもなくば 0（以下フラグ）
	//      d[i] から桁上げがあるなら 2，さもなくば 0（桁上げフラグ）
	//      d1[i..n) <= d2[i..n) なら 4，さもなくば 0（比較フラグ）
	//      f はこれら 3 つのフラグの OR をとったもの
	vvm dp(n + 1, vm(1LL << 3));
	vvm cnt(n + 1, vm(1LL << 3));
	cnt[n][1 | 0 | 4] = 1;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 8) {
			int leq = (f >> 0) & 1;
			int carry = (f >> 1) & 1;
			int cmp = (f >> 2) & 1;

			// d1 : d1[i], d2 : d2[i]
			rep(d1, b) {
				rep(d2, b) {
					int d = d1 + d2 + carry;

					int n_leq = (d % b < x) || (d % b == x && leq);
					int n_carry = (d >= b);
					int n_cmp = (d1 < d2) || (d1 == d2 && cmp);
					int nf = (n_leq << 0) | (n_carry << 1) | (n_cmp << 2);

					cnt[i][nf] += cnt[i + 1][f];
					dp[i][nf] += dp[i + 1][f] + n_carry * cnt[i + 1][f];
				}
			}
		}

		//dump("----", i, "----");
		//repb(f, 3) dump("(cmp, carry, leq) =", bitset<2>(f), ":", dp[i][f]);
	}

	return dp[0][1 | 0 | 4];
}


//【k 個組の下から桁 DP，以下フラグ，桁上げ状態，数え上げ】O(n k b^k)
/*
* b=10 進数で n 桁の数 num について，和が num 以下になる非負整数の k 個組 d[0..k) の個数を返す．
*/
mint count_tuple_sum(const string& num, int k, int b = 10) {
	int n = sz(num);

	// dp[i][f][c] : 以下の条件を満たす数の個数：
	//	i : 下からの桁 D[i..n) まで決まっている（D = Σd[0..k) とおく）
	//	f : D[i..n) ≦ num[i..n) なら 1，さもなくば 0（以下フラグ）
	//  c : D[i] からの桁上げの大きさ（k 未満）
	vvvm dp(n + 1, vvm(1LL << 1, vm(k)));
	dp[n][1][0] = 1;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		repb(f, 1) {
			int leq = (f >> 0) & 1;

			rep(carry, k) {
				// ds[0..k) : d[0..k) それぞれの i 桁目の数
				vi ds(k); int d_sum = 0; int pt = 0;

				while (true) {
					int d = d_sum + carry;

					int n_leq = (d % b < x) || (d % b == x && leq);
					int n_carry = d / b;
					int nf = (n_leq << 0);

					dp[i][nf][n_carry] += dp[i + 1][f][carry];

					while (pt < k && ds[pt] == b - 1) {
						d_sum -= b - 1;
						ds[pt++] = 0;
					}
					if (pt == k) break;

					ds[pt]++;
					d_sum++;
					pt = 0;
				}
			}
		}

		//dump("----", i, "----");
		//repb(f, 1) rep(carry, k) dump("(carry, leq) =", carry, bitset<1>(f), ":", dp[i][f][carry]);
	}

	return dp[0][1][0];
}


