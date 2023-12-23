#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 確率分布 ■■■■■


//【多項分布】
/*
* 値 [0..n) をとる確率がそれぞれ p[0..n) である試行を独立に K 回行ったときの
* それぞれの値の出現回数 X[0..n) はパラメータ (K, p[0..n)) の多項分布に従い，
*	P(X[0..n)=x[0..n)) = mul(K, x[0..n)) Π_i p[i]^x[i] （mul は多項係数）
*	E(X[i]) = K p[i]
* である．
*/


//【多項分布（m 種得る確率）】O(n^2 K^2)
/*
* X[0..n) をパラメータ (K, p[0..n)) の多項分布に従う確率ベクトルとする．
* 各 m∈[0..n] について，Pr( #{i | X[i] > 0} = m ) を pr[m] に格納する．
* 
* 利用：【階乗など（法が大きな素数）】
*/
void multinomial_distribution_kind_probability(int K, const vm& p, vm& pr) {
	// verify : https://atcoder.jp/contests/abc243/tasks/abc243_f

	//【方法】
	// dp[i][j][k] を「p[0..i) の中から j 個選んだ和全てについての k 乗和」とおく．
	// 例えば
	//	dp[4][3][k] = (p[0]+p[1]+p[2])^k + (p[0]+p[1]+p[3])^k + (p[0]+p[2]+p[3])^k + (p[1]+p[2]+p[3])^k
	// である．ただし便宜上 0 個の 0 乗和は 1 とする．
	//
	// i >= 1 かつ j >= 1 のとき，二項定理より漸化式
	//		dp[i][j][k] = dp[i-1][j][k] + Σt=[0..k] bin(k,t) p[i-1]^(k-t) dp[i-1][j-1][t]
	// が成り立つ．例えば
	//	dp[4][3][k]
	//	= (p[0]+p[1]+p[2])^k + (p[0]+p[1]+p[3])^k + (p[0]+p[2]+p[3])^k + (p[1]+p[2]+p[3])^k
	//	= (p[0]+p[1]+p[2])^k + Σt=[0..k] bin(k,t) p[3]^(k-t) ((p[0]+p[1])^t + (p[0]+p[2])^t + (p[1]+p[2])^t)
	//	= dp[3][3][k] + Σt=[0..k] bin(k,t) p[3]^(k-t) dp[2][2][t]
	// となる．よって dp[i][j][k] は DP で求めることができる．
	//
	// dp[i][j][k] を用いれば，包除原理より求める確率は
	//		Pr( #{i | X[i] > 0} = m ) = Σj=[0..m](-1)^(m-j) bin(n-j, m-j) dp[n][j][K]
	// で与えられる．例えば，n = 4 のとき
	//	Pr( #{i | X[i] > 0} = 3 )
	//	= (p[0]+p[1]+p[2])^k - ((p[0]+p[1])^k + (p[0]+p[2])^k + (p[1]+p[2])^k) + (p[0]^k + p[1]^k + p[2]^k) - 0
	//	+ (p[0]+p[1]+p[3])^k - ((p[0]+p[1])^k + (p[0]+p[3])^k + (p[1]+p[3])^k) + (p[0]^k + p[1]^k + p[3]^k) - 0
	//	+ (p[0]+p[2]+p[3])^k - ((p[0]+p[2])^k + (p[0]+p[3])^k + (p[2]+p[3])^k) + (p[0]^k + p[2]^k + p[3]^k) - 0
	//	+ (p[1]+p[2]+p[3])^k - ((p[1]+p[2])^k + (p[1]+p[3])^k + (p[2]+p[3])^k) + (p[1]^k + p[2]^k + p[3]^k) - 0
	// となる．そして，
	// (p[0]+p[1])^k が引かれる回数は，他の項 p[2], p[3] の中から残り 1 項を選ぶ場合の数 bin(4-2, 3-2) 回である．
	// p[0]^k が足される回数は，他の項 p[1], p[2], p[3] の中から残り 2 項を選ぶ場合の数 bin(4-1, 3-1) 回である．

	int n = sz(p);
	pr = vm(n + 1);

	Factorial_mint fm(max(n, K));

	// p_pow[i][t] : p[i]^t
	vvm p_pow(n, vm(K + 1));
	rep(i, n) {
		p_pow[i][0] = 1;
		repi(t, 1, K) p_pow[i][t] = p_pow[i][t - 1] * p[i];
	}

	// dp[i][j][k] : p[0..i) の中から j 個選んだ和全てについての k 乗和
	vvvm dp(n + 1, vvm(n + 1, vm(K + 1)));
	dp[0][0][0] = 1;

	// 貰う DP
	repi(i, 1, n) {
		dp[i][0][0] = 1;
		repi(j, 1, i) {
			repi(k, 0, K) {
				dp[i][j][k] = dp[i - 1][j][k];
				repi(t, 0, k) {
					dp[i][j][k] += fm.bin(k, t) * p_pow[i - 1][k - t] * dp[i - 1][j - 1][t];
				}
			}
		}
	}

	// 包除原理で求める確率を得る．
	repi(m, 0, min(n, K)) {
		repi(j, 0, m) {
			pr[m] += ((m - j) % 2 == 0 ? 1 : -1) * fm.bin(n - j, m - j) * dp[n][j][K];
		}
	}
}


//【多項分布（パラメータが一様，積の期待値）】
/*
* X[0..n) をパラメータ (K, (1/n,...,1/n)) の多項分布に従う確率ベクトルとするとき，
*		E[ ΠX[0..i) ] = perm(K, i) / n^i
* 
* (証明)
* n = 3, i = 3 の場合を例とする．反復試行を考え，
*	Y[i][j] : j 回目の試行で i が選ばれたなら 1，さもなくば 0
* という確率変数を導入する．求める期待値は
*	E[ X[0] X[1] X[2] ]
*	= E[ ΣY[0][0..K) ΣY[1][0..K) ΣY[2][0..K) ]
*	= E[ ΣΣΣj0,j1,j2=[0..K) Y[0][j0] Y[1][j1] Y[2][j2] ]
*	= ΣΣΣj0,j1,j2=[0..K) E[ Y[0][j0] Y[1][j1] Y[2][j2] ]
* となる．ここで
*	Y[a][j] Y[b][j] = 0 (a != b)
* および，j != j' のとき Y[i][j] と Y[i][j'] が独立であることを用いると，
*	= ΣΣΣj0!=j1!=j2!=j0 E[ Y[0][j0] ] E[ Y[1][j1] ] E[ Y[2][j2] ]
*	= ΣΣΣj0!=j1!=j2!=j0 (1/3)^3
*	= K(K-1)(K-2) / 3^3
* となる．
* 
* verify : https://atcoder.jp/contests/abc231/tasks/abc231_g
*/


//【超幾何分布】
/*
* 壺の中に N 個の玉がありそのうち K 個が赤玉であるとする．
* 壺から n 個の玉をランダムに取り出したときの含まれる赤玉の個数 X は超幾何分布に従い，
*	P(X = k) = bin(K, k) bin(N-K, n-k) / bin(N, n)
*	E[X] = n K / N
* である．
*/


//【連続一様分布のモーメント】
/*
* X を [0..1] 上の連続一様分布に従う確率変数とするとき，
*	E[X^i] = 1/(i+1)
* 
* verify : https://yukicoder.me/problems/no/2582
*/


//【順序統計量の分布】
/*
* X[0..n) が同一分布（分布関数 F(x)，密度関数 f(x)）に従う互いに独立な連続確率変数であるとき
* X[0..n) のうち昇順で k 番目（1-indexed）の値 Y(k) の分布関数 F_k(x)，密度関数 f_k(x) は
*	F_k(x) = Σj∈[k..n] bin(n,j) F(x)^j (1-F(x))^(n-j)
*	f_k(x) = n bin(n-1,k-1) F(x)^(k-1) (1-F(x))^(n-k) f(x)
* で与えられる．
*/


//【順序統計量の分布（連続一様分布）】
/*
* X[0..n) が [0..1] 上の連続一様分布に従う互いに独立な確率変数であるとき，
* X[0..n) のうち昇順で k 番目（1-indexed）の値 Y(k) はベータ分布 B(k, n+1-k) に従う．
* 特に E[Y(k)] = k/(n+1) である．
* 
* verify : https://atcoder.jp/contests/abc226/tasks/abc226_h
*/


//【順序統計量の分布（離散一様分布）】
/*
* Order_statistic_of_disuniform(int n, int m, Factorial_mint fm) : O(n m)
*	X[1..n] が [1..m] 上の離散一様分布に従う互いに独立な確率変数であるとして初期化する．
*	制約：fm は n! まで計算可能
*
* mint CDF(int k, int j) : O(1)
*	X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値が j 以下である確率を返す．
*
* mint PMF(int k, int j) : O(1)
*	X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値が j である確率を返す．
*
* mint ex(int k) : O(1)
*	X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値の期待値を返す．
*/
class Order_statistic_of_disuniform {
	int n, m;

	// P[k][j] : X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値が j 以下である確率
	vvm P;

	// E[k] : X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値の期待値
	vm E;

public:
	Order_statistic_of_disuniform(int n, int m, const Factorial_mint& fm) : n(n), m(m) {
		// verify : https://www.codechef.com/problems/SUMOVERALL

		mint dnm_inv = mint(m).pow(n).inv();

		// P[k][j] : X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値が j 以下である確率
		P = vvm(n + 2, vm(m + 1));
		repi(j, 1, m) {
			mint powj = mint(j).pow(n), j_inv = mint(j).inv();
			mint powxj = 1;

			repir(k, n, 1) {
				P[k][j] = P[k + 1][j] + fm.bin(n, k) * powj * powxj * dnm_inv;
				powj *= j_inv;
				powxj *= m - j;
			}
		}

		// E[k] : X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値の期待値
		E.resize(n + 1);
		repi(k, 1, n) repi(j, 1, m) E[k] += j * (P[k][j] - P[k][j - 1]);
	}

	// X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値が j 以下である確率を返す．
	mint CDF(int k, int j) const {
		if (k <= 0) return 1;
		if (k > n) return 0;

		chmax(j, 0); chmin(j, m);

		return P[k][j];
	}

	// X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値が j である確率を返す．
	mint PMF(int k, int j) const {
		if (k <= 0 || k > n || j <= 0 || j > m) return 0;

		return P[k][j] - P[k][j - 1];
	}

	// X[1..n] をソートしたときの昇順 k 番目（1-indexed）の値の期待値を返す．
	mint ex(int k) const {
		// verify : https://www.codechef.com/problems/SUMOVERALL

		Assert(1 <= k && k <= n);

		return E[k];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Order_statistic_of_disuniform& D) {
		repi(k, 1, D.n) {
			os << "k=" << k << ":";
			repi(j, 1, D.m) {
				mint x = D.PMF(k, j);
				string s = to_string(x.val());

				constexpr int v_max = (int)1e5;
				repi(dnm, 1, v_max) {
					int num = (x * dnm).val();
					if (num <= v_max) {
						s = to_string(num) + "/" + to_string(dnm);
						break;
					}
				}

				os << " " << s;
			}
			os << endl;
		}
		return os;
	}
#endif
};


