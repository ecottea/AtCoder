#pragma once
#include "header.h"
// ■■■■■ ソート ■■■■■


//【swap によるスコア変化に基づくソート】
/*
* 2 要素 x, y を swap したときのスコアの大小に関する不等式を同値変形して f(x) < f(y) の形に
* できるとき，f を任意比較関数としてソートを行うことでスコアを最大化できる．
* 
* verify : https://atcoder.jp/contests/abc366/tasks/abc366_f
*/


//【任意比較関数によるソート】
/*
* sort() に任意比較関数 bool cmp(T a, T b) を与えてソートを行う場合，
* cmp は狭義の弱順序を示すことが要求される．すなわち，cmp(a, b) ⇔ a < b が
*	非反射：	∀a ∈ T，		!(a < a)
*	推移律：	∀a, b, c ∈ T，	a < b かつ b < c ⇒ a < c
*			∀a, b, c ∈ T，	a ≡ b かつ b ≡ c ⇒ a ≡ c
* を満たすことを要求する．ここで ≡ は < から誘導される同値関係であり，
*		a ≡ b ⇔ !(a < b) かつ !(b < a)　（等しい または 比較不能）
* で定義される．商集合 T/≡ は < から誘導される順序で狭義の全順序となる．
*/


//【不正な比較関数（有向グラフ上の移動可能性）】
/*
* G = (V, E) を有向グラフとし，T = V で，u < v ⇔ u→v ∈ E と定める．
* このとき，u→v かつ v→w でも u→w とは限らないので < に関する推移律を満たさない．
* 
* この場合に適用できるアルゴリズムとしてトポロジカルソートがある．
*/


//【不正な比較関数（1 元追加）】
/*
* T = Z∪{*} で，Z には通常の < を適用し，* は任意の T の元と比較不能とする．
* このとき，0 ≡ * かつ * ≡ 1 であるにもかかわらず 0 < 1 なので，≡ に関する推移律を満たさない．
*/


//【不正な比較関数（ベクトル）】
/*
* T = Z^d で，a[0..d) < b[0..d) ⇔ ∀i∈[0..d) a[i] < b[i] とする．
* このとき，[1,1] ≡ [3,0] かつ [3,0] ≡ [2,2] であるにもかかわらず [1,1] < [2,2] なので，
* ≡ に関する推移律を満たさない．
* 
* この場合，各成分ごとに満たすべき大小関係を有向グラフの辺として表し，
* 超頂点を導入して辺数を O(n d) に抑え，トポロジカルソートを用いればよい．
* 
* verify : https://atcoder.jp/contests/abc277/tasks/abc277_f
*/


//【バブルソート】O(n^2)
/*
* a[0..n) に対してバブルソートを行う．
*/
template <class T>
void bubble_sort(vector<T>& a) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_2_A

	int n = sz(a);

	while (true) {
		// 要素の交換を行ったかどうか
		bool swapped = false;

		repir(j, n - 1, 1) {
			// 隣接要素の大小関係が逆転していたら交換する．
			if (a[j - 1] > a[j]) {
				swap(a[j - 1], a[j]);
				swapped = true;
			}
		}

		// 要素の交換が行われなくなったら終了する．
		if (!swapped) {
			break;
		}
	}
}


//【バブルソートの交換回数】
/*
* 列 a に対するバブルソートの交換回数は，a の転倒数に等しい．
*/


//【挿入ソート】O(n^2)
/*
* 長さ n の配列 a に対して挿入ソートを行う．
*/
template <class T>
void insertion_sort(vector<T>& a) {
	int n = sz(a);

	// 未ソートの要素を昇順に見ていく．
	repi(i, 1, n - 1) {
		// 注目要素
		T v = a[i];

		// 注目要素が存在すべき位置を調べつつ，
		// 要素を 1 つずつずらして挿入のための隙間をつくる．
		int j = i - 1;
		while (j >= 0 && a[j] > v) {
			a[j + 1] = a[j];
			j--;
		}

		// 要素を挿入する．
		a[j + 1] = v;
	}
}


//【挿入ソートの挿入回数】
/*
* 数列 a[0..n) に対する最適な順序で行った挿入ソートの挿入（巡回シフト）の回数は，
* n - (a の広義最長増加部分列の長さ) に等しい．
* 
* 右巡回シフトしか認めない場合，最小回数は n - (a の左からの広義貪欲増加部分列の長さ) に等しい．
*/


//【コスト最小挿入ソート】O(n^2)
/*
* [0..n) の順列 p[0..n) に対し挿入を繰り返して昇順にソートするときの最小コストを返す．
* 一度の操作ではコスト lc[rc] を払っての 左[右] 巡回シフトが可能である．
*/
ll minimum_cost_insertion_sort(vi p, ll lc, ll rc) {
	// verify : https://atcoder.jp/contests/agc032/tasks/agc032_d

	//【方法】
	// 動かさない要素の全体は増加部分列を成し，残る要素を適切な位置に挿入していくと考える．
	// 動かさない要素を最善に決めれば，残る要素を左右どちらに動かすかは一意に決まる．
	//（決まらないようであれば，その要素を新たに動かさない要素として悪くはならない．）

	int n = sz(p);
	++p; // p を [1..n] の順列にする．

	// dp[i][j] : p[0..i) までの増加部分列で，直前に j を採用した場合
	vvl dp(n + 1, vl(n + 1, INFL));
	dp[0][0] = 0;

	rep(i, n) repi(j, 0, n) {
		if (dp[i][j] == INFL) continue;

		// p[i] を採用する場合
		if (j < p[i]) {
			chmin(dp[i + 1][p[i]], dp[i][j]);
		}

		// p[i] を採用しない場合
		if (j < p[i]) {
			// せっかく j より大きいのにスルーするということは，
			// この先で j より大きく p[i] 未満の要素を採用するつもりということ．
			//（そうでなければ p[i] を採用すべきなので，それより良くなることはないので構わない．）
			// よって p[i] は右に飛ばす，すなわち左シフトの対象になる．
			chmin(dp[i + 1][j], dp[i][j] + lc);
		}
		else if (j > p[i]) {
			// p[i] は j より小さいので左に飛ばす，すなわち右シフトしなければならない．
			chmin(dp[i + 1][j], dp[i][j] + rc);
		}
	}

	ll res = INFL;
	repi(j, 0, n) chmin(res, dp[n][j]);

	return res;
}


//【選択ソート】O(n^2)
/*
* a[0..n) に対して選択ソートを行う．
*/
template <class T>
void selection_sort(vector<T>& a) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_2_B

	int n = sz(a);

	// 左から順にそこに居るべき要素を決定していく．
	rep(i, n) {
		// 残る要素の中の最小のものの位置
		int min_j = i;

		// 残りの要素を順に調べる．
		repi(j, i + 1, n - 1) {
			// より小さい要素が見つかれば位置を記憶する．
			if (a[j] < a[min_j]) {
				min_j = j;
			}
		}

		// 注目要素と最小要素とを交換する．
		swap(a[i], a[min_j]);
	}
}


//【選択ソートの交換回数】
/*
* 順列 p[0..n) に対する最適な順序で行った選択ソートの交換回数は，
* p をサイクル分解したときの (サイクル長) - 1 の総和に等しい．
* 
* verify : https://yukicoder.me/problems/no/2289
*/


//【コスト最小選択ソート】O(n log n)
/*
* [0..n) の順列 p[0..n) に対し 2 つの要素の交換を繰り返して昇順にソートするときの最小コストを返す．
* 一度の操作ではコスト c[i] + c[j] を払っての i と j の交換が可能である．
*/
ll minimum_cost_selection_sort(const vi& p, const vl& c) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_6_D

	int n = sz(p);
	ll c_min = *min_element(all(c));

	ll res = 0;
	vb seen(n);

	// サイクルごとに独立に見ていく．
	rep(i, n) {
		if (seen[i] || p[i] == i) continue;
		seen[i] = true;

		// サイクルの長さ，合計値，最小値を得る．
		int l = 1;
		ll loop_sum = c[i], loop_min = INFL;
		for (int j = p[i]; j != i; j = p[j]) {
			seen[j] = true;

			l++;
			loop_sum += c[j];
			chmin(loop_min, c[j]);
		}

		// サイクル内で要素の交換をする場合と，
		// サイクル外の最小要素を利用して交換をする場合のうち，
		// コストの小さい方を総コストに加える．
		res += loop_sum + min((l - 2) * loop_min, loop_min + (l + 1) * c_min);
	}

	return res;
}


//【シェルソート】O(n^1.25)
/*
* a[0..n) に対してシェルソートを行う．
*/
template <class T>
void shell_sort(vector<T>& a) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_2_D

	int n = sz(a);

	// 適切な増分の列を得る．
	vi gap;
	int g = 1;
	do {
		gap.push_back(g);
		g = 3 * g + 1;
	} while (g < n);
	reverse(all(gap));
	int m = sz(gap);

	// 増分を g とした挿入ソート
	function<void(int)> isort = [&](int g) {
		repi(i, g, n - 1) {
			T v = a[i];

			int j = i - g;
			while (j >= 0 && a[j] > v) {
				a[j + g] = a[j];
				j -= g;
			}

			a[j + g] = v;
		}
	};

	// 増分を減らしながら挿入ソートを繰り返す．
	rep(i, m) {
		isort(gap[i]);
	}
}


//【マージソート】O(log n)
/*
* a[0..n) に対してマージソートを行う．
*/
template <class T>
void merge_sort(vector<T>& a) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_5_B

	// 型 T における最大値
	const T T_INF = numeric_limits<T>::max();

	int n = sz(a);

	// 二つの配列 a[l..m) と a[m..r) を統合する．
	function<void(int, int, int)> merge = [&](int l, int m, int r) {
		vector<T> a_lm{ a.begin() + l, a.begin() + m };
		vector<T> a_mr{ a.begin() + m, a.begin() + r };
		a_lm.push_back(T_INF);
		a_mr.push_back(T_INF);

		int i = 0, j = 0;
		repi(k, l, r - 1) {
			if (a_lm[i] <= a_mr[j]) {
				a[k] = a_lm[i++];
			}
			else {
				a[k] = a_mr[j++];
			}
		}
	};

	// 再帰用の関数
	function<void(int, int)> merge_sort_rf = [&](int l, int r) {
		if (r - l > 1) {
			int m = (l + r) / 2;
			merge_sort_rf(l, m);
			merge_sort_rf(m, r);
			merge(l, m, r);
		}
	};

	merge_sort_rf(0, n);
}


//【クイックソート】最悪 O(n^2)
/*
* a[0..n) に対してクイックソートを行う．
*/
template <class T>
void quick_sort(vector<T>& a) {
	// varify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_6_C

	int n = sz(a);

	// a[p, r] を a[r] をピボットとして分割する．
	// 計算量を良くしたいのならピボットは乱択で決めるべき．
	function<int(int, int)> partition = [&](int p, int r) {
		// 分割の基準となる数（ピボット）
		T x = a[r];

		// i : a[p, i] は x 以下の要素と確定
		int i = p - 1;

		// j : a(i, j] は x より大きい要素と確定
		repi(j, p, r - 1) {
			// j の位置に x 以下の要素がある場合
			if (a[j] <= x) {
				// 最も左の x より大きい要素と交換する．
				i++;
				swap(a[i], a[j]);
			}
		}

		// ピボットを仕切りとしての位置に移動する．
		swap(a[i + 1], a[r]);

		return i + 1;
	};

	// 再帰用の関数
	function<void(int, int)> quick_sort_rf = [&](int p, int r) {
		if (p >= r) {
			return;
		}

		// 右端の要素をピボットとして分割し，そのそれぞれをソートする．
		int q = partition(p, r);
		quick_sort_rf(p, q - 1);
		quick_sort_rf(q + 1, r);
	};

	quick_sort_rf(0, n - 1);
}


//【計数ソート】O(n + k)
/*
* 各要素が k 未満の非負整数である a[0..n) に対して計数ソートを行う．
*/
void counting_sort(vi& a, int k) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_6_A

	int n = sz(a);

	// 結果の格納用
	vi res(n);

	// 各要素の出現数の記録用
	vi cnt(k);

	// cnt[j] : 要素 j の出現数
	rep(i, n) cnt[a[i]]++;

	// cnt[j] : j 以下の要素の出現数
	rep(j, k - 1) cnt[j + 1] += cnt[j];

	// 安定ソートとなるように降順に正しい位置にはめていく．
	repir(i, n - 1, 0) {
		res[cnt[a[i]] - 1] = a[i];
		cnt[a[i]]--;
	}

	swap(a, res);
}


//【基数ソート（基数 2）】O(n log A)（A = max(a)）
/*
* 非負数列 a[0..n) に対して基数 2 の基数ソートを行う．
*/
template <class T>
void radix_sort(vector<T>& a) {
	// verify : https://atcoder.jp/contests/chokudai_s001/tasks/chokudai_S001_d

	int n = sz(a);
	int m = msb(*max_element(all(a))) + 1;

	// 第 j ビットについて安定ソートを行う．
	rep(j, m) {
		vector<T> na; queue<T> q;
		na.reserve(n);

		// 第 j ビットが 0 か 1 かで要素を振り分ける．
		rep(i, n) {
			if (getb(a[i], j)) q.push(a[i]);
			else na.push_back(a[i]);
		}

		// 振り分けた要素を 1 つにまとめる．
		while (!q.empty()) {
			na.push_back(q.front()); q.pop();
		}

		a = move(na);
	}
}


