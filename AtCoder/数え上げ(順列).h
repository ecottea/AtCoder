#pragma once
#include "header.h"
#include "二項係数.h"
#include "数論変換.h"
// ■■■■■ 順列に関する数え上げ ■■■■■


//【挿入 DP】
/*
* あらかじめ昇順にソートする前処理を行っておけば，
* 次に使う数は今までのどの数よりも大きいことが保証される．
* これにより状態を減らして DP を行うことができる場合がある．
* 
* verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_string
*/


//【攪乱順列の数（モンモール数）】O(n)
/*
* 各 i∈[0..n] について，長さ i の攪乱順列の数を mon[i] に格納する．
*/
void montmort_number(int n, vm& mon) {
	// 参考 : https://ja.wikipedia.org/wiki/%E5%AE%8C%E5%85%A8%E9%A0%86%E5%88%97
	// verify : https://judge.yosupo.jp/problem/montmort_number_mod

	//【方法】
	// 長さ n の攪乱順列 p[0..n) を構成する方法を考える．
	// まず k = p[n-1] の選び方が n-1 通りある．
	// 次に p[k] で場合分けし，
	//	(i) p[k] = n-1 のとき，
	//		p[n-1], p[k] を除く残り n-2 個が撹乱順列になっていればいいので mon[i-2] 通り
	//	(ii) p[k] != n-1 のとき，
	//		p[n-1]=k と p[k] を入れ替えたとき，p[k] を除く残り n-1 個が攪乱順列になっている．
	//		これは一対一対応になるので mon[i-1] 通り
	// となる．以上より，漸化式
	//		mon[n] = (n-1)(mon[n-1] + mon[n-2])
	// を得る．

	mon.resize(n + 1);
	mon[0] = 1;

	repi(i, 2, n) mon[i] = (i - 1) * (mon[i - 1] + mon[i - 2]);
}


//【二重撹乱順列の数（p[i] ≠ i, p[i] ≠ i+1）】
/*
* 順列 p[0..n) で，任意の i∈[0..n) について
*		p[i] ≠ i かつ p[i] ≠ i+1 (i < n - 1)
* を満たすものの個数は
*		Σk=[0..n] (-1)^k bin(2n-k, k) (n-k)!
* である．
*
*（証明）
* 個数ごとの状態系包除原理を用いる．
* 固定された特定の k 箇所で条件に違反（他は自由）するような順列の数は，
* 固定の仕方 = パスグラフ P_2n の大きさ k のマッチングの個数 bin(2n-k, k) と，
* マッチングに属さない p[i] の選び方 (n-k)! との積に等しい．
*
* 参考 : http://oeis.org/A000271
* verify : https://atcoder.jp/contests/agc005/tasks/agc005_d
*/


//【メナージュ数（p[i] ≠ i, p[i] ≠ i+1 mod n）】
/*
* 順列 p[0..n) で，任意の i∈[0..n) について
*		p[i] ≠ i かつ p[i] ≠ i+1 (mod n)
* を満たすものの個数は
*		Σk=[0..n] (-1)^k 2n/(2n-k) bin(2n-k, k) (n-k)!
* である．
* 
*（証明）
* 個数ごとの状態系包除原理を用いる．
* 固定された特定の k 箇所で条件に違反（他は自由）するような順列の数は，
* 固定の仕方 = サイクルグラフ C_2n の大きさ k のマッチングの個数 2n/(2n-k) bin(2n-k, k) と，
* マッチングに属さない p[i] の選び方 (n-k)! との積に等しい．
* 
* 参考 : http://oeis.org/A000179
* verify : https://atcoder.jp/contests/abc214/tasks/abc214_g
*/


//【隣接大小関係の指定された順列の数え上げ（いもす法）】O(n^2)
/*
* '<', '>' からなる文字列 s[0..n-1) で指定される
* 隣接要素の間の大小関係を満たす [0..n) の順列の個数を返す．
*
*（いもす法で高速化した配る DP）
*/
mint count_permutations_adjacent_relation_imos(const string& s) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_t

	int n = sz(s) + 1;

	// dp[i][j] : p[0..i] まで出来ていて，p[i] 未満の数が j 個余っている列の個数
	vvm dp(n, vm(n));
	rep(j, n) dp[0][j] = 1;

	// 配る DP
	rep(i, n - 1) {
		// 直前より小さい数を使う場合
		if (s[i] == '>') {
			// dp[i+1][0..j-1] += dp[i][j] の種を蒔く．
			repi(j, 1, n - i - 1) dp[i + 1][j - 1] += dp[i][j];

			// 右から累積和をとる．
			repir(j, n - i - 2, 0) dp[i + 1][j] += dp[i + 1][j + 1];
		}
		// 直前より大きい数を使う場合
		else if (s[i] == '<') {
			// dp[i+1][j..n-i-2] += dp[i][j] の種を蒔く．
			repi(j, 0, n - i - 2) dp[i + 1][j] += dp[i][j];

			// 左から累積和をとる．
			repi(j, 1, n - i - 2) dp[i + 1][j] += dp[i + 1][j - 1];
		}
	}

	return dp[n - 1][0];
}


//【隣接大小関係の指定された順列の数え上げ（累積和）】O(n^2)
/*
* '<', '>' からなる文字列 s[0..n-1) で指定される
* 隣接要素の間の大小関係を満たす [0..n) の順列の個数を返す．
*
*（累積和で高速化した貰う DP）
*/
mint count_permutations_adjacent_relation_acc(const string& s) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_t

	int n = sz(s) + 1;

	// dp[i][j] : p[0..i] まで出来ていて，p[i] 未満の数が j 個余っている列の個数
	vvm dp(n, vm(n));
	rep(j, n) dp[0][j] = 1;

	// 貰う DP
	rep(i, n - 1) {
		// 直前より小さい数を使う場合
		if (s[i] == '>') {
			// acc[j] = Σdp[i][j..n-i-1]
			vm acc(n - i + 1);
			repir(j, n - i - 1, 0) acc[j] = acc[j + 1] + dp[i][j];

			// dp[i+1][j] = Σdp[i][j+1..n-i-1]
			rep(j, n - i - 1) dp[i + 1][j] = acc[j + 1];
		}
		// 直前より大きい数を使う場合
		else if (s[i] == '<') {
			// acc[j] = Σdp[i][0..j)
			vm acc(n - i + 1);
			repi(j, 0, n - i - 1) acc[j + 1] = acc[j] + dp[i][j];

			// dp[i+1][j] = Σdp[i][0..j]
			rep(j, n - i - 1) dp[i + 1][j] = acc[j + 1];
		}
	}

	return dp[n - 1][0];
}


//【隣接大小関係の指定された順列の数え上げ（挿入 DP）】O(n^2)
/*
* '<', '>' からなる文字列 s[0..n-1) で指定される
* 隣接要素の間の大小関係を満たす [0..n) の順列の個数を返す．
*
*（挿入 DP）
*/
mint count_permutations_adjacent_relation_insertDP(const string& s) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_t

	int n = sz(s) + 1;

	// dp[i][j] : s[0..i) で指定される大小関係を満たす [0..i] の順列 p で，p[i] = j であるものの数
	vvm dp(n, vm(n));
	dp[0][0] = 1;

	// 貰う DP
	rep(i, n - 1) {
		// 直前より小さい数を使う場合
		if (s[i] == '>') {
			// acc[j] = Σdp[i][j..i]
			vm acc(i + 2);
			repir(j, i, 0) acc[j] = acc[j + 1] + dp[i][j];

			// dp[i+1][j] = Σdp[i][j..i]
			repi(j, 0, i + 1) dp[i + 1][j] = acc[j];
		}
		// 直前より大きい数を使う場合
		else if (s[i] == '<') {
			// acc[j] = Σdp[i][0..j)
			vm acc(i + 2);
			repi(j, 0, i) acc[j + 1] = acc[j] + dp[i][j];

			// dp[i+1][j] = Σdp[i][0..j)
			repi(j, 0, i + 1) dp[i + 1][j] = acc[j];
		}
	}
	dumpel(dp);

	return accumulate(all(dp[n - 1]), mint(0));
}


//【2 つ以下の増加部分列に分割可能な順列の数え上げ】O(n^2)
/*
* 2 つ以下の増加部分列に分割可能な [0..n) の順列の個数を返す．
*
*（挿入 DP）
*/
mint count_permutations_2IS(int n) {
	// 参考 : https://degwer.hatenablog.com/entry/20171220

	//【方法】
	// [1..n] の順列 p[1..n] を 2 つの増加部分列に分割する方法は複数あり数えづらい．
	// そこで分割の方法が一意になるように制約を課して数えやすくすることを考える．
	//
	// [1..i] の順列を A, B の 2 つの列に分割するとき，
	//		A は i を含む
	//		B の末尾は可能な限り小さくする
	// の 2 つの制約を課すものとする．これで分割の方法は一意になった．
	//
	// これを踏まえて DP テーブルを
	//		dp[i][j] : A, B に分割できる [1..i] の順列で，B の末尾が j であるものの数
	// と定める．（B が空なら j = 0 とする）
	//
	// [1..i] の順列 p[1..i] に対し，x 以上の数を全てインクリメントした後 p[i+1] = x を追加すると，
	//		p[i+1] = i+1 とする場合：		j' = j
	//		j < p[i+1] < i+1 とする場合：	j' = p[i+1]
	// というように遷移する．

	//【備考】
	// 実は二項係数で書けそう．

	// dp[i][j] : A, B に分割できる [1..i] の順列で，B の末尾が j であるものの数
	vvm dp(n + 1, vm(n));
	dp[1][0] = 1;

	// 貰う DP
	rep(i, n) {
		// acc[j] : Σdp[i][0..i)
		vm acc(i + 2);
		repi(j, 0, i) acc[j + 1] = acc[j] + dp[i][j];

		// p[i+1] = i+1 とする場合
		repi(j, 0, i) dp[i + 1][j] += dp[i][j];

		// j < p[i+1] < i+1 とする場合
		repi(j, 0, i) dp[i + 1][j] += acc[j];
	}
	dumpel(dp);

	return accumulate(all(dp[n]), mint(0));
}


//【上下限の指定された順列の数え上げ】O(n^2)
/*
* [0..n) の順列 p[0..n) のうち，以下の条件を満たすものの個数を返す：
*	p[0..nu) <= ub[0..nu), p[nu..nu+nl) >= lb[0..nl)  (nu + nl = n)
*
*（箱根駅伝 DP）
*
* 利用：【階乗など（法が大きな素数）】
*/
mint count_permutations_single_bound(const vi& ub, const vi& lb) {
	// verify : https://yukicoder.me/problems/no/1001

	int nu = sz(ub), nl = sz(lb), n = nu + nl;

	// ub_cnt[i] : 上界が i であると指定されている p の要素の個数
	vi ub_cnt(n);
	rep(j, nu) {
		if (ub[j] < 0) return 0;
		ub_cnt[min(ub[j], n - 1)]++;
	}

	// lb_cnt[i] : 下界が i であると指定されている p の要素の個数
	vi lb_cnt(n);
	rep(k, nl) {
		if (lb[k] >= n) return 0;
		lb_cnt[max(lb[k], 0)]++;
	}

	Factorial_mint fm(nu);

	// dp_i[j] : [0..i) までの数について，上界が指定された要素のうちの
	//	いずれかに割り当てることだけが決まっている数が j 個ある場合の数
	vm dp(nu + 1);
	dp[0] = 1;

	// ub_acc : 上界が i 未満だと指定された要素の数
	// lb_acc : 下界が i 以下だと指定された要素の数
	int ub_acc = 0, lb_acc = 0;

	rep(i, n) {
		lb_acc += lb_cnt[i];

		vm dp2(nu + 1);
		repi(j, 0, nu) {
			// i を上界が指定された要素のいずれかに割り当てる予約をする場合
			if (j < nu) dp2[j + 1] += dp[j];

			// i を下界が指定された要素のいずれかに確定的に割り当てる場合
			// k : 下界が指定された要素の中の割り当て待ちのものの個数
			int k = lb_acc - (i - ub_acc - j);
			if (k > 0) dp2[j] += k * dp[j];
		}

		dp.assign(nu + 1, 0);
		repi(j, ub_cnt[i], nu) {
			// 上界が指定された要素に割り当てることだけが決まっていた未確定の j 個の数のうち，
			// 上界が i だと指定されている ub_cnt[i] 個だけ具体的な数を確定させる．
			dp[j - ub_cnt[i]] += dp2[j] * fm.perm(j, ub_cnt[i]);
		}

		ub_acc += ub_cnt[i];
	}

	return dp[0];
}


//【j 個の巡回置換の積で表される順列の数え上げ】O(n m)
/*
* 各 i∈[0..n], j∈[0..m] について，
* 順列 p[0..i) で j 個の巡回置換の積で表されるものの個数を S1[i][j] に格納し S1 を返す．
*/
vvm stirling_S1(int n, int m) {
	//【方法】
	// S1[i][j] は第 1 種スターリング数であり，これは上昇階乗冪の展開係数
	//		S1[i][j] = [x^j] x(x+1)(x+2)...(x+(i-1))
	// として求められる．ここから漸化式
	//		S1[i][j] = S1[i-1][j] * (i-1) + S1[i-1][j-1]
	// の成立が分かる．
	//
	// 組合せ論的に考えても，順列 p[0..i) で j 個の巡回置換の積で表されるものは，
	//		p[0..i-1) を j 個の巡回置換で表し，いずれかの要素の直後に i-1 を挿入したもの
	//		p[0..i-1) を j-1 個の巡回置換で表したものの積に単独の (i-1) を掛けたもの
	// に分けられるので，先の漸化式が成立することが分かる．

	// S1[i][j] : 順列 p[0..i) で j 個の巡回置換の積で表されるものの個数
	vvm S1(n + 1, vm(m + 1));
	S1[0][0] = 1;

	// 貰う DP
	repi(i, 1, n) repi(j, 1, m) S1[i][j] = S1[i - 1][j] * (i - 1) + S1[i - 1][j - 1];

	return S1;
}


//【貪欲増加部分列長が指定された順列の数え上げ】
/*
* [0..n) の順列 p[0..n) のうち貪欲増加部分列の長さが k であるものの個数は，
* 第一種スターリング数 S(n, k) に等しい．
* 
* 証明：順列 p[0..n) で貪欲増加部分列の長さが k であるものをとる．
* 貪欲増加部分列に選ばれた要素の左で区切って分割し，
* これを巡回置換表記とみなすことで k 個の巡回置換の積で表される順列と 1:1 に対応する．
* 
* 例えば n=4, k=2 のときの [1,0,3,2] は，巡回置換表記 (1 0)(3 2) に対応する．
*/


//【貪欲増加部分列長が指定された順列の数え上げ（両側）】O(n^3)
/*
* [0..n) の順列 p[0..n) のうち，左からの貪欲増加部分列長が l，
* 右からの貪欲増加部分列長が r であるものの個数を cnt[l][r] に格納する．
*
*（挿入 DP）
*/
void count_permutations_both_GIS(int n, vvm& cnt) {
	// dp_i[j][k] : [n-i..n) の順列で，左[右] からの貪欲増加部分列長が j[k] であるものの数
	vvm dp(n + 1, vm(n + 1));
	dp[1][1] = 1;

	repi(i, 1, n - 1) {
		vvm ndp(n + 1, vm(n + 1));

		repi(j, 1, i) repi(k, 1, i + 1 - j) {
			// 左端に配置した場合
			ndp[j + 1][k] += dp[j][k];

			// 右端に配置した場合
			ndp[j][k + 1] += dp[j][k];

			// その他の位置に配置した場合
			ndp[j][k] += (i - 1) * dp[j][k];
		}

		dp = move(ndp);
	}

	cnt = move(dp);
}


//【順列の数え上げ（同色隣接個数ごと）】O(n^2 max(cnt[i]))
/*
* [0..n) の中で色 i の与えられた数が cnt[i] 個含まれている（n = Σcnt）とき，
* 同色の隣接が j 箇所あるような [0..n) の順列の個数を res[j] に格納する．
*
* 制約：fm は n! まで計算可能であること
*
*（挿入 DP）
*
* 利用：【階乗など（法が大きな素数）】
*/
void count_adjacent_sequence(const vi& cnt, vm& res, Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_string

	// dp[i][j] : 色 [0..i) の中で同色の隣接が j 箇所ある列の個数
	vvm dp(1, vm({ 1, 0 }));

	int len = 2; // 列の長さ（両端の番兵含む）

	// i : 次に挿入する色，c : 色 i の数の個数
	int i = 0;
	repe(c, cnt) {
		if (c == 0) continue;
		dp.emplace_back(vm(len + c));

		// j : 同色の隣接箇所の個数
		rep(j, len - 1) {
			// k : 色 i の数をいくつの固まりに分けるか
			repi(k, 1, c) {
				// l : 同色の間にいくつ固まりを挿入するか
				repi(l, 0, min(k, j)) {
					int nj = j + (c - k) - l;

					mint add = dp[i][j];

					// c 個の数を順序込みでちょうど k 個に分ける方法の数
					//	数の区別を無視すれば，まず数を k 個減らしておき，重複組合せの考え方を用いて
					//	'o' を (c - k) 個と '|' を (k - 1) 個の並べ方を数えれば良い．
					//	実際には c 個の数には区別があるので，c! 倍する．
					add *= fm.bin(c - 1, k - 1) * fm.fact(c);

					// k 個の固まりをどこに挿入するか
					//	順序は先に定めたので，後は挿入位置だけを考えれば良い．
					//	同色の間が j 箇所中 l 箇所，異色の間が残り len - 1 - j 箇所中 k - l 箇所．
					add *= fm.bin(j, l) * fm.bin(len - 1 - j, k - l);

					dp[i + 1][nj] += add;
				}
			}
		}

		len += c;
		i++;
	}

	res = dp.back();
	res.resize(sz(res) - 2); // 番兵の分を削除
}


//【順列の数え上げ（同色隣接個数ごと，mod 998244353）】O(n (log n)^2)
/*
* [0..n) の中で色 i の与えられた数が cnt[i] 個含まれている（n = Σcnt）とき，
* 同色の隣接が j 箇所あるような [0..n) の順列の個数を res[j] に格納する．
*
* 制約：fm は n! まで計算可能であること
*
* 利用：【階乗など（法が大きな素数）】,【複数の数列の畳込み】
*/
void count_adjacent_sequence_fast(const vi& cnt, vm& res, Factorial_mint& fm) {
	// 参考 : https://atcoder.jp/contests/abc260/editorial/4434
	// verify : https://atcoder.jp/contests/abc260/tasks/abc260_h

	int m = sz(cnt), n = accumulate(all(cnt), 0);

	vvm g; mint fac = 1;
	rep(c, m) {
		if (cnt[c] == 0) continue;

		g.emplace_back(vm(cnt[c] + 1));
		repi(k, 0, cnt[c]) {
			g.back()[k] = fm.bin(cnt[c] - 1, k - 1) * fm.fact_inv(k);
		}

		fac *= fm.fact(cnt[c]);
	}

	vm q = multi_convoluion(g);

	rep(i, sz(q)) q[i] *= fm.fact(i);

	reverse(all(q));
	q.resize(n);

	rep(i, n) q[i] *= fm.fact(i);

	vm coef(n);
	rep(i, n) coef[i] = ((n - 1 - i) % 2 ? -1 : 1) * fm.fact_inv(n - 1 - i);

	res = convolution(coef, q);
	res.erase(res.begin(), res.begin() + n - 1);

	rep(i, n) res[i] *= fm.fact_inv(i) * fac;
}


//【列の数え上げ（差 1 以下禁止）】O(n m max(cnt[i])^7) ? 
/*
* [0..n) それぞれを cnt[0..n) 個ずつ含む長さ m の列で，差が 1 以下の数が隣り合わないものの個数を返す．
*
*（挿入 DP）
*
* 利用：【階乗など（法が大きな素数）】
*/
mint count_noncontinuous_sequence(const vi& cnt_) {
	// verify : https://atcoder.jp/contests/joi2019yo/tasks/joi2019_yo_f

	vi cnt;
	int m = 0; // 列の長さ
	int cnt_max = -INF;
	repe(c, cnt_) {
		// 0 個の文字は無視する．
		if (c > 0) cnt.push_back(c);

		m += c;
		chmax(cnt_max, c);
	}
	int n = sz(cnt);
	Factorial_mint fm(m);

	// unordered_map<ll, -> 用
	auto hash = [&](int j0, int j1, int J1) {
		// j0 の最大値は i がひとかたまりになった場合の cnt_max - 1
		// j1 の最大値は i と i-1 が交互に並んだ場合の 2 * cnt_max
		// これらを基数として j0, j1, J1 を混ぜ合わせる．
		return j0 + (ll)cnt_max * (j1 + (2LL * cnt_max + 1) * J1);
	};
	auto unhash = [&](ll v) {
		int j0 = v % cnt_max; v /= cnt_max;
		int j1 = v % (2 * cnt_max); v /= (2LL * cnt_max + 1);
		int J1 = (int)v;
		return make_tuple(j0, j1, J1);
	};

	// dp[i][{j0, j1, J1}] : [0..i) までで以下の条件を満たす列の個数：
	//		j0 : i-1 を含む差が 0 の隣接箇所（i-1 と i-1）の個数
	//		j1 : i-1 を含む差が 1 の隣接箇所（i-1 と i-2）の個数
	//		j2 : i-1 を含む差が 2 以上の隣接箇所の個数
	//		J1 : i-1 を含まない差が 1 以下の隣接箇所の個数
	//		J2 : i-1 を含まない差が 2 以上の隣接箇所の個数
	vector<unordered_map<ll, mint>> dp(n + 1);
	dp[0][hash(0, 0, 0)] = 1;
	dump(dp[0]);

	int len = 2; // 文字列の長さ（両端の番兵 -inf, inf を含む）

	// i : 次に挿入する数
	rep(i, n) {
		repe(tmp, dp[i]) {
			int j0, j1, j2, J1, J2;
			tie(j0, j1, J1) = unhash(tmp.first);
			j2 = 2 * (i > 0 ? cnt[i - 1] : 0) - (2 * j0 + j1);
			J2 = (len - 1) - (j0 + j1 + j2 + J1);

			// k : 数 i をいくつの固まりに分けるか
			// ここからのループがひどいが，定数倍 1/5! = 1/120 が掛かっている．
			repi(k, 1, min(cnt[i], len - 1)) {
				// ij* : 対応する j* 個ある隣接箇所にいくつ固まりを挿入するか
				repi(ij0, 0, min(k, j0)) {
					repi(ij1, 0, min(k - ij0, j1)) {
						repi(ij2, 0, min(k - (ij0 + ij1), j2)) {
							repi(iJ1, 0, min(k - (ij0 + ij1 + ij2), J1)) {
								int iJ2 = k - (ij0 + ij1 + ij2 + iJ1);
								if (iJ2 > J2) continue;

								// nj* : 対応する隣接箇所の個数がいくつになるか
								int nj0 = cnt[i] - k;
								int nj1 = 2 * ij0 + ij1 + ij2;
								int nJ1 = (J1 - iJ1) + (j0 - ij0) + (j1 - ij1);

								mint add = tmp.second;

								// cnt[i] 個の文字を順序込みで k 個に分ける方法の数
								//	まず文字を k 個減らしておき，重複組合せの考え方を用いて
								//	'o' cnt[i] - k 個と '|' k - 1 個の並べ方を数えれば良い．
								add *= fm.bin(cnt[i] - 1, k - 1);

								// k 個の固まりをどこに挿入するか
								//	順序は先に定めたので，後は挿入位置だけを考えれば良い．
								//	それぞれ j* 箇所中 ij* 箇所を選び順に挿入する．
								add *= fm.bin(j0, ij0);
								add *= fm.bin(j1, ij1);
								add *= fm.bin(j2, ij2);
								add *= fm.bin(J1, iJ1);
								add *= fm.bin(J2, iJ2);

								dp[i + 1][hash(nj0, nj1, nJ1)] += add;
							}
						}
					}
				}
			}
		}
		len += cnt[i];
	}

	return dp[n][hash(0, 0, 0)];
}


