#pragma once
#include "header.h"
// ■■■■■ 桁 DP（下の桁から） ■■■■■


//【下から状態桁 DP，以下フラグ，数え上げ】O(n m b)
/*
* b=10 進数で n 桁の数 num 以下の非負の整数で，数字和が m の倍数であるものの個数を返す．
*/
mint count_digit_sum(const string& num, int m, int b = 10) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_s

	int n = sz(num);

	// dp[i][f][j] : 以下の条件を満たす数の個数：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//	f : d[i..n) <= num[i..n) なら 1，さもなくば 0（以下フラグ）
	//	j : d[i..n) の数字和 (mod m)
	vvvm dp(n + 1, vvm(2, vm(m)));
	dp[n][1][0] = 1;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 2) {
			rep(j, m) {
				// d : d[i]
				rep(d, b) {
					int nf = (d < x) || (d == x && f);
					int nj = (j + d) % m;

					dp[i][nf][nj] += dp[i + 1][f][j];
				}
			}
		}

		//dump(i);
		//dump("!leq"); dump(dp[i][0]);
		//dump("leq"); dump(dp[i][1]);
	}

	return dp[0][1][0];
}


//【下から状態桁 DP，以上フラグ，数え上げ】O(n m b)
/*
* b=10 進数で n 桁の数 num 以上の n 桁の整数で，数字和が m の倍数であるものの個数を返す．
*/
mint count_digit_sum_greater(const string& num, int m, int b = 10) {
	int n = sz(num);

	// dp[i][f][j] : 以下の条件を満たす数の個数：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//	f : d[i..n) >= num[i..n) なら 1，さもなくば 0（以上フラグ）
	//	j : d[i..n) の数字和 (mod m)
	vvvm dp(n + 1, vvm(2, vm(m)));
	dp[n][1][0] = 1;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 2) {
			rep(j, m) {
				// d : d[i]
				rep(d, b) {
					int nf = (d > x) || (d == x && f);
					int nj = (j + d) % m;

					dp[i][nf][nj] += dp[i + 1][f][j];
				}
			}
		}

		//dump(i);
		//dump("!geq"); dump(dp[i][0]);
		//dump("geq"); dump(dp[i][1]);
	}

	return dp[0][1][0];
}


//【下から状態桁 DP，以下フラグ，前 0 フラグ，数え上げ】O(n m b)
/*
* b=10 進数で n 桁の数 num 以下の非負の整数で，桁の数字に 0 を含まず，
* 数字和が m の倍数であるものの個数を返す．
*/
mint count_digit_sum_avoid0(const string& num, int m, int b = 10) {
	int n = sz(num);

	// dp[i][f][j] : 以下の条件を満たす数の個数：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//	f : d[i..n) <= num[i..n) なら 1，さもなくば 0（以下フラグ）
	//	    d[i] = '0' なら 2，さもなくば 0（前 0 フラグ）
	//	j : d[i..n) の数字和 (mod m)
	//	k : d[i..n) に '0' が含まれるなら 1，さもなくば 0
	using vvvvm = vector<vvvm>;
	vvvvm dp(n + 1, vvvm(4, vvm(m, vm(2))));
	dp[n][0 | 1][0][0] = 1;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 4) {
			int leq = (f >> 0) & 1;
			int lz = (f >> 1) & 1;

			rep(j, m) {
				rep(k, 2) {
					// d : d[i]
					rep(d, b) {
						int n_leq = (d < x) || (d == x && leq);
						int n_lz = (d == 0);
						int nf = (n_leq << 0) | (n_lz << 1);

						int nj = (j + d) % m;
						int nk = k | (lz && (d != 0));

						dp[i][nf][nj][nk] += dp[i + 1][f][j][k];
					}
				}
			}
		}

		//dump(i);
		//rep(f, 4) {
		//	dumps("(lz, leq) =");
		//	dump(bitset<2>(f));
		//	dump(dp[i][f]);
		//}
	}

	mint res = dp[0][0 | 1][0][0] + dp[0][2 | 1][0][0];
	res--; // 0 を除外

	return res;
}


//【下から状態桁 DP，以下フラグ，スコア和】O(n m b)
/*
* b=10 進数で n 桁の数 num 以下の非負の整数で，数字和が m の倍数であるものの和を返す．
*/
mint sum_digit_sum(const string& num, int m, int b = 10) {
	int n = sz(num);

	// dp[i][f][j] : 以下の条件を満たす数の個数：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//	f : d[i..n) <= num[i..n) なら 1，さもなくば 0（以下フラグ）
	//	j : d[i..n) の数字和 (mod m)
	vvvm dp(n + 1, vvm(2, vm(m)));
	vvvm cnt(n + 1, vvm(2, vm(m)));
	cnt[n][1][0] = 1;

	// 下の桁から順に配る DP
	mint b_pow = 1;
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 2) {
			rep(j, m) {
				// d : d[i]
				rep(d, b) {
					int nf = (d < x) || (d == x && f);
					int nj = (j + d) % m;

					cnt[i][nf][nj] += cnt[i + 1][f][j];
					dp[i][nf][nj] += dp[i + 1][f][j] + cnt[i + 1][f][j] * d * b_pow;
				}
			}
		}
		b_pow *= b;

		//dump(i);
		//dump("!leq"); dump(dp[i][0]);
		//dump("leq"); dump(dp[i][1]);
	}

	return dp[0][1][0];
}


//【下から状態桁 DP，以下フラグ，前 0 フラグ，スコア和】O(n m b)
/*
* b=10 進数で n 桁の数 num 以下の非負の整数で，桁の数字に 0 を含まず，
* 数字和が m の倍数であるものの和を返す．
*/
mint sum_digit_sum_avoid0(const string& num, int m, int b = 10) {
	int n = sz(num);

	// dp[i][f][j] : 以下の条件を満たす数の個数：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//	f : d[i..n) <= num[i..n) なら 1，さもなくば 0（以下フラグ）
	//	    d[i] = '0' なら 2，さもなくば 0（前 0 フラグ）
	//	j : d[i..n) の数字和 (mod m)
	//	k : d[i..n) に '0' が含まれるなら 1，さもなくば 0
	using vvvvm = vector<vvvm>;
	vvvvm dp(n + 1, vvvm(4, vvm(m, vm(2))));
	vvvvm cnt(n + 1, vvvm(4, vvm(m, vm(2))));
	cnt[n][0 | 1][0][0] = 1;

	// 下の桁から順に配る DP
	mint b_pow = 1;
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 4) {
			int leq = (f >> 0) & 1;
			int lz = (f >> 1) & 1;

			rep(j, m) {
				rep(k, 2) {
					// d : d[i]
					rep(d, b) {
						int n_leq = (d < x) || (d == x && leq);
						int n_lz = (d == 0);
						int nf = (n_leq << 0) | (n_lz << 1);

						int nj = (j + d) % m;
						int nk = k | (lz && (d != 0));

						cnt[i][nf][nj][nk] += cnt[i + 1][f][j][k];
						dp[i][nf][nj][nk] += dp[i + 1][f][j][k] + cnt[i + 1][f][j][k] * d * b_pow;
					}
				}
			}
		}
		b_pow *= b;

		//dump(i);
		//rep(f, 4) {
		//	dumps("(lz, leq) =");
		//	dump(bitset<2>(f));
		//	dump(dp[i][f]);
		//}
	}

	mint res = dp[0][0 | 1][0][0] + dp[0][2 | 1][0][0];

	return res;
}


//【下から桁 DP，以下フラグ，桁上げフラグ，スコア最大化】O(n b)
/*
* b=10 進数で n 桁の数 num 以下の非負の整数 d 全てについて，
* d の数字和と num - d の数字和の合計の最大値を返す．
*/
ll maximize_pair_digit_sum(const string& num, int b = 10) {
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の最大スコア：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//	f : d[i..n) <= num[i..n) なら 1，さもなくば 0（以下フラグ）
	//      d[i] から桁上げがあるなら 2，さもなくば 0（桁上げフラグ）
	//      f はこれら 2 つのフラグの OR をとったもの
	vvl dp(n + 1, vl(4, -INFL));
	dp[n][0 | 1] = 0;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 4) {
			int leq = (f >> 0) & 1;
			int carry = (f >> 1) & 1;

			// d : d[i]
			rep(d, b) {
				int n_leq = (d < x) || (d == x && leq);
				int n_carry = d + carry > x;
				int nf = (n_leq << 0) | (n_carry << 1);

				// d2 : num - d の i 桁目
				int d2 = smod(x - (d + carry), b);
				chmax(dp[i][nf], dp[i + 1][f] + d + d2);
			}
		}

		//dump(i);
		//rep(f, 4) {
		//	dumps("(carry, leq) =");
		//	dump(bitset<2>(f));
		//	dump(dp[i][f]);
		//}
	}

	return dp[0][0 | 1];
}


//【下から桁 DP，以下フラグ，桁上げフラグ，スコア最小化】O(n b)
/*
* b=10 進数で n 桁の数 num 以下の非負の整数 d 全てについて，
* d の数字和と num + d の数字和の合計の最小値を返す．
*/
ll minimize_pair_digit_sum(string num, int b = 10) {
	num.insert(num.begin(), '0');
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の最小スコア：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//	f : d[i..n) <= num[i..n) なら 1，さもなくば 0（以下フラグ）
	//      d[i] から桁上げがあるなら 2，さもなくば 0（桁上げフラグ）
	//      f はこれら 2 つのフラグの OR をとったもの
	vvl dp(n + 1, vl(4, INFL));
	dp[n][1] = 0;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 4) {
			int leq = f & 1;
			int carry = (f >> 1) & 1;

			// d : d[i]
			rep(d, b) {
				int n_leq = (d < x) || (d == x && leq);
				int n_carry = x + d + carry >= b;
				int nf = n_leq | (n_carry << 1);

				// d2 : num + d の i 桁目
				int d2 = (x + d + carry) % b;
				chmin(dp[i][nf], dp[i + 1][f] + d + d2);
			}
		}

		//dump(i);
		//rep(f, 4) {
		//	dumps("(carry, leq) =");
		//	dump(bitset<2>(f));
		//	dump(dp[i][f]);
		//}
	}

	return dp[0][0 | 1];
}


//【下から桁 DP，桁上げフラグ，前 0 フラグ，スコア最大化】O(n b)
/*
* b=10 進数で n 桁の数 num について，全ての整数 d についての
* d, num, d + num の桁が揃う箇所の最大個数を返す．（前 0 は '0' と認めない）
*/
ll maximize_digit_triple(string num, int b = 10) {
	// verify : https://atcoder.jp/contests/ttpc2015/tasks/ttpc2015_f

	num.insert(num.begin(), '0');
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の最小スコア：
	//	i : 下からの桁 d[i..n) まで決まっている．
	//	f : d[i] から桁上げがあるなら 1，さもなくば 0（桁上げフラグ）
	//      d[i] = 0 なら 2，さもなくば 0（前 0 フラグ）
	//      f はこれら 2 つのフラグの OR をとったもの
	vvl dp(n + 1, vl(4, -INFL));
	dp[n][2 | 0] = 0;

	// 下の桁から順に配る DP
	repir(i, n - 1, 0) {
		int x = num[i] - '0';

		rep(f, 4) {
			int carry = (f >> 0) & 1;
			int lz = (f >> 1) & 1;

			// d : d[i]
			rep(d, b) {
				int n_carry = x + d + carry >= b;
				int n_lz = (d == 0);
				int nf = (n_carry << 0) | (n_lz << 1);

				int d2 = (x + d + carry) % b;
				chmax(dp[i][nf], dp[i + 1][f] + (d == d2 && d == x));
			}
		}

		//dump(i);
		//rep(f, 4) {
		//	dumps("(lz, carry) =");
		//	dump(bitset<2>(f));
		//	dump(dp[i][f]);
		//}
	}

	return dp[0][0 | 0];
}


