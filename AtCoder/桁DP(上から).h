#pragma once
#include "header.h"
// ■■■■■ 桁 DP（上の桁から） ■■■■■


//【上から桁 DP，未満フラグ，数え上げ】O(n b)
/*
* b 進数で n 桁の数 num 以下の非負の整数の個数を返す．
*/
mint count_digit_sum(const string& num, int b = 10) {
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	vvm dp(n + 1, vm(1 << 1));
	dp[0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		rep(f, 2) {
			// d_max : d[i] のとれる値の最大値
			int d_max = (f ? b - 1 : x);

			// d : d[i]
			repi(d, 0, d_max) {
				int nf = (int)(f || (d < d_max));

				dp[i + 1][nf] += dp[i][f];
			}
		}

		//dump(i + 1);
		//dump("!smaller"); dump(dp[i + 1][0]);
		//dump("smaller"); dump(dp[i + 1][1]);
	}

	return dp[n][0] + dp[n][1];
}


//【上から桁 DP，未満フラグ，数え上げ】O(n b m)
/*
* b 進数で n 桁の数 num 以下の非負の整数で，数字和が m の倍数であるものの個数を返す．
*/
mint count_digit_sum(const string& num, int m, int b = 10) {
	// 参考 : https://ferin-tech.hatenablog.com/entry/2019/11/10/%E6%A1%81DP%E3%81%AE%E5%AE%9F%E8%A3%85
	// verify : https://atcoder.jp/contests/dp/tasks/dp_s

	int n = sz(num);

	// dp[i][f][j] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//	j : d[0..i) の数字和 (mod m)
	vvvm dp(n + 1, vvm(2, vm(m)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		rep(f, 2) {
			// d_max : d[i] のとれる値の最大値
			int d_max = (f ? b - 1 : x);

			rep(j, m) {
				// d : d[i]
				repi(d, 0, d_max) {
					int nf = (int)(f || (d < d_max));
					int nj = (j + d) % m;

					dp[i + 1][nf][nj] += dp[i][f][j];
				}
			}
		}

		//dump(i + 1);
		//dump("!smaller"); dump(dp[i + 1][0]);
		//dump("smaller"); dump(dp[i + 1][1]);
	}

	return dp[n][0][0] + dp[n][1][0];
}


//【上から桁 DP，超過フラグ，数え上げ】O(n b m)
/*
* b 進数で n 桁の数 num 以上の n 桁の整数で，数字和が m の倍数であるものの個数を返す．
*/
mint count_digit_sum_greater(const string& num, int m, int b = 10) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_s

	int n = sz(num);
	
	// dp[i][f][j] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) > num[0..i) なら 1，さもなくば 0（超過フラグ）
	//	j : d[0..i) の数字和 (mod m)
	vvvm dp(n + 1, vvm(2, vm(m)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		rep(f, 2) {
			// d_min : d[i] のとれる値の最小値
			int d_min = (f ? 0 : x);

			rep(j, m) {
				// d : d[i]
				repi(d, d_min, b - 1) {
					int nf = (int)(f || (d > d_min));
					int nj = (j + d) % m;

					dp[i + 1][nf][nj] += dp[i][f][j];
				}
			}
		}

		//dump(i + 1);
		//dump("!greater"); dump(dp[i + 1][0]);
		//dump("greater"); dump(dp[i + 1][1]);
	}

	return dp[n][0][0] + dp[n][1][0];
}


//【上から桁 DP，未満フラグ，前 0 フラグ，数え上げ】O(n b m)
/*
* b 進数で n 桁の数 num 以下の非負の整数で，桁の数字に 0 を含まず，
* 数字和が m の倍数であるものの個数を返す．
*/
mint count_digit_sum_avoid0(const string& num, int m, int b = 10) {
	int n = sz(num);

	// dp[i][f][j] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//      d[0..i) の全てが '0' なら 2，さもなくば 0（前 0 フラグ）
	//      f はこれら 2 つのフラグの OR をとったもの
	//	j : d[0..i) の数字和 (mod m)
	vvvm dp(n + 1, vvm(1 << 2, vm(m)));
	dp[0][2 | 0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 2) {
			int smaller = (f >> 0) & 1;
			int leading0 = (f >> 1) & 1;

			// d_max : d[i] のとれる値の最大値
			int d_max = (smaller ? b - 1 : x);

			rep(j, m) {
				// d : d[i]
				repi(d, 0, d_max) {
					int n_smaller = (int)(smaller || (d < d_max));
					int n_leading0 = (int)(leading0 && (d == 0));
					int nf = (n_smaller << 0) | (n_leading0 << 1);

					int nj = (j + d) % m;

					// 前 0 でない限り 0 は禁止
					if (leading0 || d != 0) {
						dp[i + 1][nf][nj] += dp[i][f][j];
					}
				}
			}
		}

		//dump(i + 1);
		//repb(f, 2) {
		//	dumps("(lz, smaller) =");
		//	dump(bitset<2>(f));
		//	dump(dp[i + 1][f]);
		//}
	}

	return dp[n][0 | 0][0] + dp[n][0 | 1][0];
}


//【上から桁 DP，未満フラグ，スコア和】O(n b m)
/*
* b 進数で n 桁の数 num 以下の非負の整数で，数字和が m の倍数であるものの和を返す．
*/
mint sum_digit_sum(const string& num, int m, int b = 10) {
	int n = sz(num);
	
	// dp[i][f][j] : 以下の条件を満たす数の (個数, 和)：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//	j : d[0..i) の数字和 (mod m)
	vvvm dp(n + 1, vvm(2, vm(m)));
	vvvm cnt(n + 1, vvm(2, vm(m)));
	cnt[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		rep(f, 2) {
			// d_max : d[i] のとれる値の最大値
			int d_max = (f ? b - 1 : x);

			rep(j, m) {
				// d : d[i]
				repi(d, 0, d_max) {
					int nf = (int)(f || (d < d_max));
					int nj = (j + d) % m;

					cnt[i + 1][nf][nj] += cnt[i][f][j];
					dp[i + 1][nf][nj] += dp[i][f][j] * b + cnt[i][f][j] * d;
				}
			}
		}

		//dump(i);
		//dump("!smaller"); dump(dp[i + 1][0]);
		//dump("smaller"); dump(dp[i + 1][1]);
	}

	return dp[n][0][0] + dp[n][1][0];
}


//【上から桁 DP，未満フラグ，前 0 フラグ，スコア和】O(n b m)
/*
* b 進数で n 桁の数 num 以下の非負の整数で，桁の数字に 0 を含まず，
* 数字和が m の倍数であるものの和を返す．
*/
mint sum_digit_sum_avoid0(const string& num, int m, int b = 10) {
	int n = sz(num);

	// dp[i][f][j] : 以下の条件を満たす数の和：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//      d[0..i) の全てが '0' なら 2，さもなくば 0（前 0 フラグ）
	//      f はこれら 2 つのフラグの OR をとったもの
	//	j : d[0..i) の数字和 (mod m)
	vvvm dp(n + 1, vvm(4, vm(m)));
	vvvm cnt(n + 1, vvm(4, vm(m)));
	cnt[0][0 | 2][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		rep(f, 4) {
			int smaller = f & 1;
			int leading0 = (f >> 1) & 1;

			// d_max : d[i] のとれる値の最大値
			int d_max = (smaller ? b - 1 : x);

			rep(j, m) {
				// d : d[i]
				repi(d, 0, d_max) {
					int n_smaller = (int)(smaller || (d < d_max));
					int n_leading0 = (int)(leading0 && (d == 0));
					int nf = n_smaller | (n_leading0 << 1);

					int nj = (j + d) % m;

					// 前 0 でない限り 0 は禁止
					if (leading0 || d != 0) {
						cnt[i + 1][nf][nj] += cnt[i][f][j];
						dp[i + 1][nf][nj] += dp[i][f][j] * b + cnt[i][f][j] * d;
					}
				}
			}
		}

		//dump(i + 1);
		//rep(f, 4) {
		//	dumps("(lz, smaller) =");
		//	dump(bitset<2>(f));
		//	dump(dp[i + 1][f]);
		//}
	}

	return dp[n][0][0] + dp[n][1][0];
}


//【上から桁 DP，未満フラグ，桁上げフラグ，スコア最大化】O(n b)
/*
* b 進数で n 桁の数 num 以下の非負の整数 d 全てについて，
* d の数字和と num - d の数字和の合計の最大値を返す．
*/
mint maximize_pair_digit_sum(const string& num, int b = 10) {
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//	    d[i] から桁上げがあるなら 2，さもなくば 0（桁上げフラグ）
	//      f はこれら 2 つのフラグの OR をとったもの
	vvl dp(n + 1, vl(4, -INFL));
	dp[0][0 | 0] = 0;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		rep(f, 4) {
			int smaller = (f >> 0) & 1;
			int carry = (f >> 1) & 1;

			// d_max : d[i] のとれる値の最大値
			int d_max = (smaller ? b - 1 : x);

			// d : d[i]
			repi(d, 0, d_max) {
				int n_smaller = (int)(smaller || (d < d_max));

				rep(n_carry, 2) {
					int nf = (n_smaller << 0) | (n_carry << 1);

					// d2 : num - d の i 桁目
					int d2 = smod(x - (d + n_carry), b);

					if (carry == (d + d2 + n_carry >= b)) {
						chmax(dp[i + 1][nf], dp[i][f] + d + d2);
					}
				}
			}
		}

		//dump(i + 1);
		//rep(f, 4) {
		//	dumps("(carry, smaller) =");
		//	dump(bitset<2>(f));
		//	dump(dp[i + 1][f]);
		//}
	}

	return max(dp[n][0 | 0], dp[n][0 | 1]);
}


//【上から桁 DP，未満フラグ，桁上げフラグ，スコア最小化】O(n b)
/*
* b 進数で n 桁の数 num 以下の非負の整数 d 全てについて，
* d の数字和と num + d の数字和の合計の最小値を返す．
*/
mint minimize_pair_digit_sum(string num, int b = 10) {
	num.insert(num.begin(), '0');
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//	    d[i] から桁上げがあるなら 2，さもなくば 0（桁上げフラグ）
	//		f はこれら 2 つのフラグの OR をとったもの
	vvl dp(n + 1, vl(4, INFL));
	dp[0][0 | 0] = 0;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		rep(f, 4) {
			int smaller = (f >> 0) & 1;
			int carry = (f >> 1) & 1;

			// d_max : d[i] のとれる値の最大値
			int d_max = (smaller ? b - 1 : x);

			// d : d[i]
			repi(d, 0, d_max) {
				int n_smaller = (int)(smaller || (d < d_max));

				rep(n_carry, 2) {
					int nf = (n_smaller << 0) | (n_carry << 1);

					// d2 : num + d の i 桁目
					int d2 = (x + d + n_carry) % b;

					if (carry == (d + x + n_carry >= b)) {
						chmin(dp[i + 1][nf], dp[i][f] + d + d2);
					}
				}
			}
		}

		//dump(i + 1);
		//rep(f, 4) {
		//	dumps("(carry, smaller) =");
		//	dump(bitset<2>(f));
		//	dump(dp[i + 1][f]);
		//}
	}

	return min(dp[n][0 | 0], dp[n][0 | 1]);
}


//【組の上から桁 DP，未満フラグ，数え上げ】O(max(n1, n2) b^2)
/*
* b 進数で n1[n2] 桁の数 num1[num2] について，num1[num2] 以下の非負整数 d1[d2] の組の個数を返す．
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
	vvm dp(n + 1, vm(4, 0));
	dp[0][0 | 0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x1[x2] : num1[num2] の上から i 桁目の数
		int x1 = num1[i] - '0';
		int x2 = num2[i] - '0';

		rep(f, 4) {
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
		//rep(f, 4) {
		//	dumps("(smaller2, smaller1) ="); dump(bitset<2>(f));
		//	dump(dp[i + 1][f]);
		//}
	}

	mint res = 0;
	rep(f, 4) res += dp[n][f];

	return res;
}


//【桁の数字の分布】O(n b)
/*
* b 進数で n 桁の数 num 以下の正の整数すべてについて，
* 桁の数字に現れる数字 t の個数を cnt[t] に格納する．
*
*（桁 DP，smaller フラグ，leading-zero フラグ）
*/
void digits_distribution(const string& num, vm& cnt, int b = 10) {
	// verify : https://atcoder.jp/contests/abc029/tasks/abc029_d

	int n = sz(num);
	cnt.resize(b);

	// dp[i][sml][lz][t] : 以下の条件を満たす数の桁の数字に含まれる t の個数
	//                     ただし t = b のときは条件を満たす数の個数を表すものとする．
	//	i   : 上からの桁 d[0..i) まで決まっている．
	//	sml : d[0..i) だけで num より小さいことが確定していれば 1，さもなくば 0
	//	lz  : d[0..i) 全てが 0 ならば 1，さもなくば 0
	vector<vvvm> dp(n + 1, vvvm(2, vvm(2, vm(b + 1))));
	dp[0][0][1][b] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数（0-indexed）
		int x = num[i] - '0';

		// smaller = true, leading zero = true の場合
		dp[i + 1][1][1][b] += dp[i][1][1][b];
		repi(t, 1, b - 1) {
			dp[i + 1][1][0][t] += dp[i][1][1][b];
			dp[i + 1][1][0][b] += dp[i][1][1][b];
		}

		// smaller = true, leading zero = false の場合
		rep(t, b) {
			dp[i + 1][1][0][t] += dp[i][1][0][t] * b;
			dp[i + 1][1][0][t] += dp[i][1][0][b];
			dp[i + 1][1][0][b] += dp[i][1][0][b];
		}

		// smaller = false, leading zero = true の場合
		dp[i + 1][1][1][b] += dp[i][0][1][b];
		repi(t, 1, x - 1) {
			dp[i + 1][1][0][t] += dp[i][0][1][b];
			dp[i + 1][1][0][b] += dp[i][0][1][b];
		}
		dp[i + 1][0][0][x] += dp[i][0][1][b];
		dp[i + 1][0][0][b] += dp[i][0][1][b];

		// smaller = false, leading zero = false の場合
		rep(t, b) {
			dp[i + 1][1][0][t] += dp[i][0][0][t] * x;
			if (t < x) {
				dp[i + 1][1][0][t] += dp[i][0][0][b];
				dp[i + 1][1][0][b] += dp[i][0][0][b];
			}

			dp[i + 1][0][0][t] += dp[i][0][0][t];
			if (t == x) {
				dp[i + 1][0][0][t] += dp[i][0][0][b];
				dp[i + 1][0][0][b] += dp[i][0][0][b];
			}
		}

		//dump(i + 1);
		//dump("smaller && leading zero:");
		//dump(dp[i + 1][1][1]);
		//dump("smaller && !leading zero:");
		//dump(dp[i + 1][1][0]);
		//dump("!smaller && leading zero:");
		//dump(dp[i + 1][0][1]);
		//dump("!smaller && !leading zero:");
		//dump(dp[i + 1][0][0]);
	}

	rep(t, b) {
		cnt[t] = dp[n][0][0][t] + dp[n][1][0][t];
	}
}


