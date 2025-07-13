#pragma once
#include "header.h"
#include "ディリクレ畳込み.h"
// ■■■■■ 数え上げ（数論） ■■■■■


//【素数計数関数】O(N^(3/4))
/*
* N 以下の素数の個数 π(N) を返す．
*
*（Lucy DP）
*/
ll prime_pi(ll N) {
	// 参考 : https://rsk0315.hatenablog.com/entry/2021/05/18/015511
	// verify : https://judge.yosupo.jp/problem/counting_primes

	//【方法】
	// j 番目（1-indexed）の素数を p[j](≦ √N) と表し，dp_j[i] を
	//		dp_j[i]
	//		= ([2..i] 内の "素数または p[j] 以下の素因数をもたない合成数" の個数)
	//		= (エラトステネスの篩において，[2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の個数)
	// とおく．dp_j[i] の求め方を考える．
	// 
	// p[j]^2 > i のときは，[2..i] 内には p[j] で新たに篩われる数は無いので
	//		dp_j[i] = dp_(j-1)[i]
	// である．
	// 
	// p[j]^2 ≦ i のときは，[2..i] 内の p[j] で新たに篩われる数は
	//		(i) p[j-1] 以下の素因数を持たない（まだ篩われていない）
	//		(ii) 2 p[j] 以上の p[j] の倍数（次に篩われる）
	// という条件を共に満たす数である．
	// 
	// [2..i] に条件 (i), (ii) を課す代わりに，全体を p[j] で割って，
	//		[2..i/p[j]] 内の p[j-1] 以下の素因数を持たない数
	// を数えても個数は変わらない．そのような数は，[2..i/p[j]] 内の
	//		(iii) p[j-1] 以下の素数で篩い終えた後残っている
	//		(iv) p[j-1] 以下の素数ではない
	// という条件を共に満たす数であり，!(iv) ⇒ (iii) に注意するとその個数は
	//		dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]
	// と表される．
	//
	// 以上をまとめると，DP の初期化は
	//		dp_0[i] = i - 1
	// で行い，遷移式は
	//		dp_j[i] = dp_(j-1)[i] （p[j]^2 > i のとき）
	//		dp_j[i] = dp_(j-1)[i] - (dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]) （p[j]^2 ≦ i のとき）
	// を用いれば良い．

	//【備考】
	// 途中で DP テーブルを参照すれば，K-rough number の数え上げにも対応できる．

	if (N <= 1) return 0;

	int m = (int)(sqrt(N) + 1e-9);

	// dp_j[i] : [2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の個数
	//	dp_l_j[i] : dp_j[i]
	//	dp_h_j[d] : dp_j[N/d]
	vl dp_l(m + 1), dp_h(m + 1);
	repi(i, 1, m) {
		dp_l[i] = i - 1;
		dp_h[i] = N / i - 1;
	}

	repi(p, 2, m) {
		// cnt_p1 : p-1 以下の素数の個数
		ll cnt_p1 = dp_l[p - 1];

		// p が素数でなければ次の p へ
		if (dp_l[p] == cnt_p1) continue;

		repi(d, 1, m) {
			// p^2 > N/d なら更新不要
			if (p > (N / d) / p) break;

			if (d <= m / p) dp_h[d] -= dp_h[d * p] - cnt_p1;
			else dp_h[d] -= dp_l[(N / d) / p] - cnt_p1;
		}

		repir(i, m, 1) {
			// p^2 > i なら更新不要
			if (p > i / p) break;

			dp_l[i] -= dp_l[i / p] - cnt_p1;
		}
	}

	return dp_h[1];
}


//【素数計数関数（一括）】O(N^(3/4))
/*
* i 以下の素数の個数を π(i) とし，π[N/d] のリストを返す．
*
*（Lucy DP）
*/
Floor_vector<ll> prime_pi_all(ll N) {
	// 参考 : https://rsk0315.hatenablog.com/entry/2021/05/18/015511
	// verify : https://atcoder.jp/contests/jsc2024-final/tasks/jsc2024_final_b

	//【方法】
	// j 番目（1-indexed）の素数を p[j](≦ √N) と表し，dp_j[i] を
	//		dp_j[i]
	//		= ([2..i] 内の "素数または p[j] 以下の素因数をもたない合成数" の個数)
	//		= (エラトステネスの篩において，[2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の個数)
	// とおく．dp_j[i] の求め方を考える．
	// 
	// p[j]^2 > i のときは，[2..i] 内には p[j] で新たに篩われる数は無いので
	//		dp_j[i] = dp_(j-1)[i]
	// である．
	// 
	// p[j]^2 ≦ i のときは，[2..i] 内の p[j] で新たに篩われる数は
	//		(i) p[j-1] 以下の素因数を持たない（まだ篩われていない）
	//		(ii) 2 p[j] 以上の p[j] の倍数（次に篩われる）
	// という条件を共に満たす数である．
	// 
	// [2..i] に条件 (i), (ii) を課す代わりに，全体を p[j] で割って，
	//		[2..i/p[j]] 内の p[j-1] 以下の素因数を持たない数
	// を数えても個数は変わらない．そのような数は，[2..i/p[j]] 内の
	//		(iii) p[j-1] 以下の素数で篩い終えた後残っている
	//		(iv) p[j-1] 以下の素数ではない
	// という条件を共に満たす数であり，!(iv) ⇒ (iii) に注意するとその個数は
	//		dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]
	// と表される．
	//
	// 以上をまとめると，DP の初期化は
	//		dp_0[i] = i - 1
	// で行い，遷移式は
	//		dp_j[i] = dp_(j-1)[i] （p[j]^2 > i のとき）
	//		dp_j[i] = dp_(j-1)[i] - (dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]) （p[j]^2 ≦ i のとき）
	// を用いれば良い．

	//【備考】
	// 途中で DP テーブルを参照すれば，K-rough number の数え上げにも対応できる．

	int nl = (int)(sqrt(N) + 1e-9);

	// dp_j[i] : [2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の個数
	Floor_vector<ll> dp(N, nl);
	int nh = dp.nh;

	repi(i, 1, nl) dp.set_l(i, i - 1);
	repi(d, 1, nh) dp.set_h(d, N / d - 1);

	// is_prime[i] : i が素数か
	vb is_prime(nl + 1, true);
	is_prime[0] = is_prime[1] = false;

	repi(p, 2, nl) {
		// p が素数でなければ次の p へ
		if (!is_prime[p]) continue;

		// p^2 以上の p の倍数は素数でないと確定する．
		for (ll j = (ll)p * p; j <= nl; j += p) is_prime[j] = false;

		// cnt_p1 : p-1 以下の素数の個数
		ll cnt_p1 = dp.get_l(p - 1);

		repi(d, 1, nh) {
			// p^2 > N/d なら更新不要
			if (p > (N / d) / p) break;

			dp[N / d] -= dp.get_h(d * p) - cnt_p1;
		}

		repir(i, nl, 1) {
			// p^2 > i なら更新不要
			if (p > i / p) break;

			dp[i] -= dp.get_l(i / p) - cnt_p1;
		}
	}

	return dp;
}


//【素数の数え上げ（mod 4）】O(N^(3/4))
/*
* N 以下の (4Z+1 型素数の個数, 4Z+3 型素数の個数) の組を返す．
*
*（Lucy DP）
*/
pll prime_pi_mod4(ll N) {
	if (N <= 2) return { 0, 0 };

	int sqrt_N = (int)(sqrt(N) + 1e-6);

	// S_t(i, p) : [2..i] 内の p 以下の素数で篩い終えた後残っている [t=0 ? 4Z+1 : 4Z+3] 型整数の個数
	//	dp_l_p[t][v] : S_t(i, p)
	//	dp_h_p[t][v] : S_t(N/i, p)
	vvl dp_l(2, vl(sqrt_N + 1)), dp_h(2, vl(sqrt_N + 1));
	repi(i, 1, sqrt_N) {
		// [2..i] 内の 4Z+1 型整数の個数
		dp_l[0][i] = (i - 1) / 4;
		
		// [2..i] 内の 4Z+3 型整数の個数
		dp_l[1][i] = (i + 1) / 4;
		
		// [2..N/i] 内の 4Z+1 型整数の個数
		dp_h[0][i] = (N / i - 1) / 4;
		
		// [2..N/i] 内の 4Z+3 型整数の個数
		dp_h[1][i] = (N / i + 1) / 4;
	}

	for (ll p = 3; p <= sqrt_N; p += 2) {
		// p が素数でなければ次の p へ
		if (dp_l[0][p] + dp_l[1][p] == dp_l[0][p - 1] + dp_l[1][p - 1]) continue;

		int tp = (p % 4 == 3);

		// dp_h[i] = S(N/i, p) の更新
		repi(i, 1, sqrt_N) {
			// p^2 > N/i なら更新不要
			if (p * p > N / i) break;

			if (i * p <= sqrt_N) {
				dp_h[0][i] -= dp_h[0 ^ tp][i * p] - dp_l[0 ^ tp][p - 1];
				dp_h[1][i] -= dp_h[1 ^ tp][i * p] - dp_l[1 ^ tp][p - 1];
			}
			else {
				dp_h[0][i] -= dp_l[0 ^ tp][N / (i * p)] - dp_l[0 ^ tp][p - 1];
				dp_h[1][i] -= dp_l[1 ^ tp][N / (i * p)] - dp_l[1 ^ tp][p - 1];
			}
		}

		// dp_l[i] = S(i, p) の更新
		repir(i, sqrt_N, 1) {
			// p^2 > i なら更新不要
			if (p * p > i) break;

			dp_l[0][i] -= dp_l[0 ^ tp][i / p] - dp_l[0 ^ tp][p - 1];
			dp_l[1][i] -= dp_l[1 ^ tp][i / p] - dp_l[1 ^ tp][p - 1];
		}
	}

	return { dp_h[0][1], dp_h[1][1] };
}


//【素数の数え上げ（mod 4，一括）】O(N^(3/4))
/*
* N 以下の 4Z+1 型素数の個数を π_1(i)，4Z+3 型素数の個数を π_3(i) とし，
* π_1[N/d] のリストと π_3[N/d] のリストの組を返す．
*
*（Lucy DP）
*/
pair<Floor_vector<ll>, Floor_vector<ll>> prime_pi_mod4_all(ll N) {
	int nl = (int)(sqrt(N) + 1e-9);

	// dp1_j[i] : [2..i] 内の p[j] 以下の素数で篩い終えた後残っている 4Z+1 型整数の個数
	// dp3_j[i] : [2..i] 内の p[j] 以下の素数で篩い終えた後残っている 4Z+3 型整数の個数
	Floor_vector<ll> dp1(N, nl), dp3(N, nl);
	int nh = dp1.nh;

	repi(i, 1, nl) {
		dp1.set_l(i, (i - 1) / 4);
		dp3.set_l(i, (i + 1) / 4);
	}
	repi(d, 1, nh) {
		dp1.set_h(d, (N / d - 1) / 4);
		dp3.set_h(d, (N / d + 1) / 4);
	}

	for (int p = 3; p <= nl; p += 2) {
		// p が素数でなければ次の p へ
		if (dp1.get_l(p) + dp3.get_l(p) == dp1.get_l(p - 1) + dp3.get_l(p - 1)) continue;

		// cnt1 : p-1 以下の 4Z+1 型素数の個数
		// cnt3 : p-1 以下の 4Z+3 型素数の個数
		ll cnt1 = dp1.get_l(p - 1);
		ll cnt3 = dp3.get_l(p - 1);

		if (p % 4 == 1) {
			repi(d, 1, nh) {
				// p^2 > N/d なら更新不要
				if ((ll)p * p > N / d) break;

				dp1[N / d] -= dp1.get_h(d * p) - cnt1;
				dp3[N / d] -= dp3.get_h(d * p) - cnt3;
			}

			repir(i, nl, 1) {
				// p^2 > i なら更新不要
				if ((ll)p * p > i) break;

				dp1[i] -= dp1.get_l(i / p) - cnt1;
				dp3[i] -= dp3.get_l(i / p) - cnt3;
			}
		}
		else {
			repi(d, 1, nh) {
				// p^2 > N/d なら更新不要
				if ((ll)p * p > N / d) break;

				dp1[N / d] -= dp3.get_h(d * p) - cnt3;
				dp3[N / d] -= dp1.get_h(d * p) - cnt1;
			}

			repir(i, nl, 1) {
				// p^2 > i なら更新不要
				if ((ll)p * p > i) break;

				dp1[i] -= dp3.get_l(i / p) - cnt3;
				dp3[i] -= dp1.get_l(i / p) - cnt1;
			}
		}
	}

	return { dp1, dp3 };
}


//【無平方数の数え上げ】O(N^(4/9))
/*
* N 以下の無平方数の個数を返す．
*
*（除原理）
*/
ll count_square_free(ll N) {
	// verify : https://judge.yosupo.jp/problem/counting_squarefrees

	int nl = (int)(pow(2. * N, 1. / 3) + 1e-9);
	int nh = (int)sqrt(N / (nl + 1) + 1);

	// isqrt[i] : floor(√i)
	vi isqrt(nl + 1);
	repi(i, 1, nl) isqrt[i] = (int)sqrtl((long double)i);

	// dp_l[i] : i 以下の無平方数の個数
	// dp_h[d] : N/d 以下の無平方数の個数
	vl dp_l(nl + 1), dp_h(nh + 1);

	// ちゃんとやれば O(nl log(log nl)) でできるが，全体の計算量は変わらない．
	repi(i, 1, nl) {
		dp_l[i] = i;

		int m = (int)(pow(2. * i, 2. / 3) + 1e-9);

		int j_max = isqrt[m];
		int k_max = i / (m + 1);

		repi(j, 2, j_max) {
			dp_l[i] -= dp_l[i / j / j];
		}

		repi(k, 1, k_max) {
			dp_l[i] -= (dp_l[k] - dp_l[k - 1]) * (isqrt[i / k] - j_max);
		}
	}

	repir(d, nh, 1) {
		ll i = N / d / d;

		dp_h[d] = i;

		ll m = (ll)(pow(2. * i, 2. / 3) + 1e-9);

		int j_max = (int)sqrtl((long double)m);
		int k_max = (int)(i / (m + 1));

		repi(j, 2, j_max) {
			if (i / j / j <= nl) {
				dp_h[d] -= dp_l[i / j / j];
			}
			else {
				dp_h[d] -= dp_h[d * j];
			}
		}

		repi(k, 1, k_max) {
			dp_h[d] -= (dp_l[k] - dp_l[k - 1]) * ((int)sqrtl((long double)(i / k)) - j_max);
		}
	}

	return dp_h[1];
}


//【整数の数え上げ（素因数分解の型ごと）】O(n^(3/4))
/*
* x = p^a q^b r^c ... のとき，多重集合 {a,b,c,...} を降順ソートしたものを x の素因数分解の型と呼ぶ．
* 各素因数分解の型 tp に対し，[1..n] 内の型 tp をもつ整数の個数を格納したリストを返す．
*/
map<vi, ll> count_by_prime_signature(ll n) {
	// verify : https://mojacoder.app/users/shobonvip/problems/shobonvip_pqrst/editorial

	int m = (int)(sqrt(n) + 1e-9);

	// 1 と素数の昇順リスト
	vl ps{ 1 };

	// cnt0_p[v] : [2..v] 内の p 以下の素数で篩い終えた後残っている数の個数
	// cnt1_p[v] : [2..n/v] 内の p 以下の素数で篩い終えた後残っている数の個数
	vl cnt0(m + 1), cnt1(m + 1);

	repi(v, 1, m) {
		cnt0[v] = v - 1;
		cnt1[v] = n / v - 1;
	}

	repi(p, 2, m) {
		ll c = cnt0[p - 1];

		// p が素数でなければ次の p へ
		if (cnt0[p] == c) continue;
		ps.push_back(p);

		// cnt1 の更新
		repi(v, 1, m) {
			// p^2 > n/v なら更新不要
			if (p > n / v / p) break;

			if (v <= m / p) {
				cnt1[v] -= cnt1[v * p] - c;
			}
			else {
				cnt1[v] -= cnt0[n / v / p] - c;
			}
		}

		// cnt0 の更新
		repir(v, m, 1) {
			// p^2 > v なら更新不要
			if (p > v / p) break;

			cnt0[v] -= cnt0[v / p] - c;
		}
	}

	map<vi, ll> cnt;
	cnt[{}]++;

	// s     : 注目頂点
	// i_gpf : s の最大素因数が何番目の素数か
	// tp    : 素因数分解の型（順序付き）
	function<void(ll, int, vi)> dfs = [&](ll s, int i_gpf, vi tp) {
		ll p = ps[i_gpf];

		// s の最小の子 s * p からの寄与を加算する．
		if (s != 1) {
			tp.back()++;
			cnt[tp]++;
			tp.back()--;
		}

		// その他の s の子からの寄与をまとめて加算する．
		if (s <= m) {
			tp.push_back(1);
			cnt[tp] += cnt1[s] - cnt0[p];
			tp.pop_back();
		}
		else {
			tp.push_back(1);
			cnt[tp] += cnt0[n / s] - cnt0[p];
			tp.pop_back();
		}

		// s の最小の子 s * p を探索する．
		if (s != 1 && s <= n / (p * p)) {
			tp.back()++;
			dfs(s * p, i_gpf, tp);
			tp.back()--;
		}

		// その他の s の子を探索する．
		tp.push_back(1);
		for (int i = i_gpf + 1; i < sz(ps) && s <= n / (ps[i] * ps[i]); i++) {
			dfs(s * ps[i], i, tp);
		}
	};
	dfs(1, 0, vi());

	// 素因数分解の型の順序を無視してまとめ直す．
	map<vi, ll> cnt2;
	repe(tmp, cnt) {
		if (tmp.second == 0) continue;

		auto es = tmp.first;
		sort(all(es), greater<int>());
		cnt2[es] += tmp.second;
	}

	return cnt2;
}


//【二元一次不定方程式の解の数え上げ】
/*
* GCD.h へ
*/
