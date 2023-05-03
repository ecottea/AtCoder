#pragma once
#include "header.h"
// ■■■■■ ヤング図形 ■■■■■


//【ヤング図形】
/*
* 広義単調減少な正整数列 a[0..n) で，左から順に箱が a[0..n) 個並んだヤング図形を表す．
*/


//【ヤング図形の転置】O(n)
/*
* ヤング図形 a[0..n) を転置したヤング図形を返す．
*/
vi transpose_yd(const vi& a) {
	// verify : https://yukicoder.me/problems/no/2149

	if (a.empty()) return vi();

	int n = sz(a);

	vi at(a[0]);
	rep(i, n) at[a[i] - 1]++;
	repir(i, a[0] - 2, 0) at[i] += at[i + 1];

	return at;
}


//【ヤング図形のドミノ分割】O(n)
/*
* ヤング図形 a[0..n) を左上を 0 とする 0, 1 の市松模様に彩色する．
* a をドミノに分割し，各 k∈[0,1] に対し，右上が k であるようなドミノだけを抽出して作った
* 新たなヤング図形を b[k] に格納して b[0..1] を返す（分割不可能なら空配列を返す）
*/
vvi domino_division_yd(const vi& a) {
	// 参考 : https://mathlog.info/articles/2214
	// verify : https://yukicoder.me/problems/no/2149

	int n = sz(a);

	vvi b(2); int h = 0;

	// 左から順に縦にどんどん削っていき，余りが出たら右に階段状に削っていく．
	rep(i, n) {
		int p = (i + a[i] + 1) % 2;
		b[p].push_back((a[i] - h) / 2);

		if ((a[i] - h) & 1) {
			b[p].back() += ++h;
		}
		else {
			h = max(h - 1, 0);
		}
	}
	rep(k, 2) while (!b[k].empty() && b[k].back() == 0) b[k].pop_back();

	// 削りきれなかったならドミノ分割不可能．
	return h == 0 ? b : vvi();
}


//【標準タブローの数え上げ】O(Σa)
/*
* ヤング図形 a に対応する標準タブローの個数を返す．
* 戻り値は，ヤング図形であるという性質を保ちながら 1 マスずつ箱を配置する方法の数とも解釈できる．
*
* 利用：【ヤング図形の転置】
*/
mint hook_length_formula(const vi& a) {
	// 参考 : https://zenn.dev/koboshi/articles/306304c0381c1e
	// verify : https://yukicoder.me/problems/no/2149

	int n = sz(a);

	// a_sum : マスの総数（分割対象の自然数）
	int a_sum = accumulate(all(a), 0);

	// 分子は a_sum!
	mint num = 1;
	repi(i, 1, a_sum) num *= i;

	vi at = transpose_yd(a);

	// 分母は各マスにおけるフック長の積
	mint dnm = 1;
	rep(i, n) rep(j, a[i]) dnm *= (a[i] - j) + (at[j] - i) - 1;

	return num / dnm;
}


//【ヤング図形の列挙（包含指定）】
/*
* ヤング図形 a[0..n) に包含されるヤング図形を格納したリストを返す（空のヤング図形も含む）
*/
vvi enumerate_young_diagrams(const vi& a) {
	int n = sz(a);
	vi yng; vvi yngs;

	function<void(int)> rf = [&](int i) {
		// 幅が n になったら完成とする．
		if (i == n) {
			yngs.push_back(yng);
			return;
		}

		// h_max : i 列目に積める箱の数の最大値
		int h_max = a[i];
		if (i > 0) chmin(h_max, yng[i - 1]);

		// i 列目に箱を積む場合
		repir(h, h_max, 1) {
			yng.push_back(h);
			rf(i + 1);
			yng.pop_back();
		}

		// i 列目に箱を積まない場合は打ち切って完成とする．
		yngs.push_back(yng);
	};
	rf(0);

	return yngs;
}


//【ヤング図形の数え上げ（包含指定）】O(Σa)
/*
* ヤング図形 a[0..n) に包含されるヤング図形の個数を返す（空のヤング図形も含む）
*/
mint count_young_diagrams(const vi& a) {
	int n = sz(a);

	// dp_i[j] : a[0..i) に包含される，右端の高さが j であるヤング図形の個数
	vm dp(a[0] + 1);
	dp[a[0]] = 1;

	rep(i, n) {
		// acc[j] : Σdp_i[j..∞)
		vm acc(sz(dp) + 1);
		repir(j, sz(dp) - 1, 0) acc[j] = acc[j + 1] + dp[j];

		acc.resize(a[i] + 1);
		dp = move(acc);
	}

	return accumulate(all(dp), mint(0));
}


//【ヤング図形の数え上げ（包含指定）】O(n^2)
/*
* ヤング図形 a[0..n) に包含されるヤング図形の個数を返す（空のヤング図形も含む）
*
* 利用：【二項係数（一括，n が固定，r が小さい，法が大きな素数）】
*/
template<class T>
mint count_young_diagrams_ll(vector<T> a) {
	// verify : https://atcoder.jp/contests/arc104/tasks/arc104_e

	//【方法】
	// a[0..i) に包含される右端の高さが x であるヤング図形の個数を f(i,x) とおくと，
	//		f(i,x) = Σr∈[0..i) (-1)^r c[i-1-r] bin(x, r)
	// として x の i-1 次多項式で表される．f(i+1,x) は
	//		f(i+1,x)
	//		= Σy∈[x..a[i-1]] f(i,y)
	//		= Σy∈[x..a[i-1]] Σr∈[0..i) (-1)^r c[i-1-r] bin(y, r)
	//		= (Σy∈[0..a[i-1]+1)-Σy∈[0..x)) Σr∈[0..i) (-1)^r c[i-1-r] bin(y, r)
	//		= Σr∈[0..i) (-1)^r c[i-1-r] (bin(a[i-1]+1, r+1) - bin(x, r+1)) （ホッケースティック恒等式）
	// となるが，
	//		c[i] = Σr∈[0..i) (-1)^r c[i-1-r] bin(a[i-1]+1, r+1)  ...(*)
	// とおけば
	//		f(i+1,x)
	//		= c[i] - Σr∈[0..i) (-1)^r c[i-1-r] bin(x, r+1)
	//		= c[i] - Σr∈[1..i+1) (-1)^(r-1) c[i-r] bin(x, r)
	//		= c[i] + Σr∈[1..i+1) (-1)^r c[i-r] bin(x, r)
	//		= Σr∈[0..i+1) (-1)^r c[i-r] bin(x, r)
	// となり，係数 c[0..i) を使いまわした上で同じ形に表される．
	// 
	// c[0]=1 から始めて先の遷移式 (*) を用いた DP で c[0..n) を昇順に求めていけば良い．
	//（オンライン畳込みの形なので，二項係数用の前計算が可能な程度の a[0] なら高速化できる）

	//【備考】
	// 幅が同じところをまとめて遷移させているだけの最短格子路数の DP だとも思える．

	a.push_back(0); // 右端の高さが 0 でなければならないことにする．
	int n = sz(a);

	vm c(n);
	c[0] = 1;

	repi(i, 1, n - 1) {
		auto bin = binomial_fixed_n(a[i - 1] + 1, i);

		rep(r, i) c[i] += (r & 1 ? -1 : 1) * c[i - 1 - r] * bin[r + 1];
	}

	return c[n - 1];
}


