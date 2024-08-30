#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 桁 DP（上の桁から） ■■■■■


//【上から桁 DP の状態の内訳】
/*
* 上から smaller フラグを持って桁 DP を行うとき，smaller = false となる状態をとるのは，
* 最上位からここまでの全ての桁が等しいものに限る．
* これを利用して他の状態を減らすことができる場合がある．
* 
* verify : https://atcoder.jp/contests/abc194/tasks/abc194_f
*/


//【上から桁 DP，未満フラグ，数え上げ】O(n b)
/*
* b 進数で n 桁の数 num 以下の非負の整数の個数を返す．
*/
mint count_digit(const string& num, int b = 10) {
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の個数：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	vvm dp(n + 1, vm(1LL << 1));
	dp[0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 1) {
			int smaller = (f >> 0) & 1;

			// d_max : d[i] のとれる値の最大値
			int d_max = (smaller ? b - 1 : x);

			// d : d[i]
			repi(d, 0, d_max) {
				int n_smaller = (int)(smaller || (d < d_max));

				int nf = (n_smaller << 0);

				dp[i + 1][nf] += dp[i][f];
			}
		}

		//dump("----", i + 1, "----");
		//repb(f, 1) dump("(smaller) =", bitset<1>(f), ":", dp[i + 1][f]);
	}

	mint res = 0;
	repb(f, 1) res += dp[n][f];

	return res;
}


//【上から状態桁 DP，未満フラグ，数え上げ】O(n b m)
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
	vvvm dp(n + 1, vvm(1LL << 1, vm(m)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 1) {
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

	mint res = 0;
	repb(f, 1) res += dp[n][f][0];

	return res;
}


//【上から状態桁 DP，超過フラグ，数え上げ】O(n b m)
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
	vvvm dp(n + 1, vvm(1LL << 1, vm(m)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 1) {
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

	mint res = 0;
	repb(f, 1) res += dp[n][f][0];

	return res;
}


//【上から状態桁 DP，未満フラグ，前 0 フラグ，数え上げ】O(n b m)
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
	vvvm dp(n + 1, vvm(1LL << 2, vm(m)));
	dp[0][0 | 2][0] = 1;

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

		//dump("----", i + 1, "----");
		//repb(f, 2) dump("(lz, smaller) =", bitset<2>(f), ":", dp[i + 1][f]);
	}

	return dp[n][0 | 0][0] + dp[n][1 | 0][0];
}


//【上から桁 DP，未満フラグ，和】O(n b)
/*
* b 進数で n 桁の数 num 以下の非負の整数の和を返す．
*/
mint sum_digit_downward(const string& num, int b = 10) {
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の和（cnt は個数）：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	vvm dp(n + 1, vm(1LL << 1));
	vvm cnt(n + 1, vm(1LL << 1));
	cnt[0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 1) {
			// d_max : d[i] のとれる値の最大値
			int d_max = (f ? b - 1 : x);

			// d : d[i]
			repi(d, 0, d_max) {
				int nf = (int)(f || (d < d_max));

				// 個数は単に増えるだけ．
				cnt[i + 1][nf] += cnt[i][f];

				// 総和については，いままでの数全てが桁上げのため一律 b 倍され，
				// 新たに 1 の位に d が cnt 個増える．
				dp[i + 1][nf] += dp[i][f] * b + cnt[i][f] * d;
			}
		}

		//dump(i + 1);
		//dump("!smaller"); dump(dp[i + 1][0]);
		//dump("smaller"); dump(dp[i + 1][1]);
	}

	return dp[n][0] + dp[n][1];
}


//【上から桁 DP，未満フラグ，累乗和】O(n m^2 b)
/*
* b 進数で n 桁の数 num 以下の非負の整数の m 乗和を返す．
*
* 利用：【階乗など（法が大きな素数）】
*/
mint power_sum_digit_downward(const string& num, int m, int b = 10) {
	int n = sz(num);

	Factorial_mint fm(m);

	vvm d_pow(b + 1, vm(m + 1, 1));
	repi(d, 0, b) rep(j, m) d_pow[d][j + 1] = d_pow[d][j] * d;

	// dp[i][f][j] : 以下の条件を満たす数の j 乗和：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	vvvm dp(n + 1, vvm(1LL << 1, vm(m + 1)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 1) {
			// d_max : d[i] のとれる値の最大値
			int d_max = (f ? b - 1 : x);

			// d : d[i]
			repi(d, 0, d_max) {
				int nf = (int)(f || (d < d_max));

				// 例えば
				//		17^m + 27^m + 37^m
				//		= Σj∈[0..m] bin(m,j) (10^j + 20^j + 30^j) 7^(m-j)
				//		= Σj∈[0..m] bin(m,j) (1^j + 2^j + 3^j) 10^j 7^(m-j)
				// となるように，
				//		dp[i + 1][nf][nj] = Σj∈[0..nj] bin(nj, j) dp[i][f][j] b^j d^(nj - j)
				// である．
				// 
				// これは指数型母関数を導入すれば畳込みで一括計算できる．
				repi(nj, 0, m) repi(j, 0, nj) {
					dp[i + 1][nf][nj] += fm.bin(nj, j) * (dp[i][f][j] * d_pow[b][j]) * d_pow[d][nj - j];
				}
			}
		}

		//dump(i + 1);
		//dump("!smaller"); dump(dp[i + 1][0]);
		//dump("smaller"); dump(dp[i + 1][1]);
	}

	return dp[n][0][m] + dp[n][1][m];
}


//【上から状態桁 DP，未満フラグ，スコア和】O(n b m)
/*
* b 進数で n 桁の数 num 以下の非負の整数で，数字和が m の倍数であるものの和を返す．
*/
mint sum_digit_sum(const string& num, int m, int b = 10) {
	int n = sz(num);
	
	// dp[i][f][j] : 以下の条件を満たす数の和（cnt は個数）：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//	j : d[0..i) の数字和 (mod m)
	vvvm dp(n + 1, vvm(1LL << 1, vm(m)));
	vvvm cnt(n + 1, vvm(1LL << 1, vm(m)));
	cnt[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 1) {
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


//【上から状態桁 DP，未満フラグ，前 0 フラグ，スコア和】O(n b m)
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
	vvvm dp(n + 1, vvm(1LL << 2, vm(m)));
	vvvm cnt(n + 1, vvm(1LL << 2, vm(m)));
	cnt[0][0 | 2][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 2) {
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

		//dump(i + 1, ":");
		//rep(f, 4) {
		//	dump("(lz, smaller) =", bitset<2>(f));
		//	dump("dp :", dp[i + 1][f]);
		//	dump("cnt:", cnt[i + 1][f]);
		//}
	}

	return dp[n][0 | 0][0] + dp[n][1 | 0][0];
}


//【上から桁 DP，未満フラグ，桁上げフラグ，スコア最大化】O(n b)
/*
* b 進数で n 桁の数 num 以下の非負の整数 d 全てについて，
* d の数字和と num - d の数字和の合計の最大値を返す．
*/
ll maximize_pair_digit_sum(const string& num, int b = 10) {
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の最大スコア：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//	    d[i] から桁上げがあるなら 2，さもなくば 0（桁上げフラグ）
	//      f はこれら 2 つのフラグの OR をとったもの
	vvl dp(n + 1, vl(1LL << 2, -INFL));
	dp[0][0 | 0] = 0;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 2) {
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

		//dump("----", i + 1, "----");
		//repb(f, 2) dump("(carry, smaller) =", bitset<2>(f), ":", dp[i + 1][f]);
	}

	return max(dp[n][0 | 0], dp[n][1 | 0]);
}


//【上から桁 DP，未満フラグ，桁上げフラグ，コスト最小化】O(n b)
/*
* b 進数で n 桁の数 num 以下の非負の整数 d 全てについて，
* d の数字和と num + d の数字和の合計の最小値を返す．
*/
ll minimize_pair_digit_sum(string num, int b = 10) {
	// verify : https://atcoder.jp/contests/abc155/tasks/abc155_e

	num.insert(num.begin(), '0');
	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の最小コスト：
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	f : d[0..i) < num[0..i) なら 1，さもなくば 0（未満フラグ）
	//	    d[i] から桁上げがあるなら 2，さもなくば 0（桁上げフラグ）
	//		f はこれら 2 つのフラグの OR をとったもの
	vvl dp(n + 1, vl(1LL << 2, INFL));
	dp[0][0 | 0] = 0;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		repb(f, 2) {
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

		//dump("----", i + 1, "----");
		//repb(f, 2) dump("(carry, smaller) =", bitset<2>(f), ":", dp[i + 1][f]);
	}

	return min(dp[n][0 | 0], dp[n][1 | 0]);
}


//【上から桁 DP，桁上げ状態，コスト最小化】O(n D B)
/*
* B 進数で D 桁の非負整数列 a[0..n) について，min_x Σi∈[0..n) digit_sum(a[i] + x) を返す．
*/
ll minimize_digit_sums(const vl& a, int D = 18, int B = 10) {
	// verify : https://atcoder.jp/contests/arc153/tasks/arc153_d

	int n = sz(a);

	vl powB(D + 1);
	powB[0] = 1;
	rep(d, D) powB[d + 1] = powB[d] * B;

	// dp[d][j] : 最上位から d 桁目までについて，d 桁目より下でソートしたときの
	//	降順に j 個の数が下の桁からの繰り上がりを貰っている場合の桁和の最小値
	vvl dp(D + 1, vl(n + 1, INFL));
	repi(i, 0, n) dp[D][i] = i;

	// 上の桁から順に貰う DP
	repir(d, D - 1, 0) {
		// suf_dig : {d 桁目より下, d 桁目の数字} を昇順ソートしたリスト
		vector<pll> suf_dig(n);
		rep(i, n) suf_dig[i] = { a[i] % powB[d], (a[i] / powB[d]) % B };
		sort(all(suf_dig));

		// cnt[b] : d 桁目にある数字 b の個数
		vi cnt(B);
		rep(i, n) cnt[suf_dig[i].second]++;

		// ds : d 桁目の数字和
		ll ds = 0;
		rep(b, B) ds += b * cnt[b];

		// acc0 : d 桁目に 0 を足した場合の d+1 桁目への繰り上がり回数
		int acc0 = 0;

		// d-1 桁目から n-i 個の繰り上がりを貰っている場合
		repir(i, n, 0) {
			if (i < n) {
				// d-1 桁目からの繰り上がりにより d 桁目の数字の個数分布が変わる．
				cnt[suf_dig[i].second]--;
				ds -= suf_dig[i].second;
				cnt[(suf_dig[i].second + 1) % B]++;
				ds += (suf_dig[i].second + 1) % B;

				// d+1 桁目への繰り上がりがあれば記録しておく．
				if (suf_dig[i].second + 1 == B) acc0++;
			}

			// acc : d+1 桁目への累積の繰り上がり回数
			int acc = 0;

			rep(b, B) {
				// b と B-b を足すと繰り上がるので，累積繰り上がり回数に B-b の個数を加算する．
				if (b > 0) acc += cnt[B - b];

				// 元々の d 桁目の数字和に b を加えた分と繰り上がった分の補正を加え，d+1 桁目の結果と合わせる．
				ll add = ds + b * n - B * acc;
				chmin(dp[d][n - i], dp[d + 1][acc + acc0] + add);
			}
		}
	}

	return dp[0][0];
}


