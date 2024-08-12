#pragma once
#include "header.h"
#include "探索.h"
#include "前処理(列).h"
// ■■■■■ 静的辞書 ■■■■■


//【ウェーブレット行列】
/*
* Wavelet_matrix<T>(vT a) : O(n log n)
*	整数列 a[0..n) で初期化する．
*
* T get(int l, int r, int i) : O(log n)
*	a[l..r) の中で昇順で i 番目の要素を返す（なければ INFL）
*
* int count(int l, int r, T v) : O(log n)
*	a[l..r) に v が何個あるかを返す．
*
* int count(int l, int r, T v0, T v1) : O(log n)
*	a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
*
* ll sum(int l, int r) : O(1)
*	a[l..r) の和を返す．
*
* ll sum(int l, int r, T v0, T v1) : O(log n)
*	a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
*
* ll abs_sum(int l, int r, T v) : O(log n)
*	Σi∈[l..r) |a[i] - v| を返す．
*/
template <class T>
class Wavelet_matrix {
	// 参考 : https://miti-7.hatenablog.com/entry/2018/04/28/152259

	int n; // 要素数
	int m; // msb 以下の桁数
	vi bs; // bs[i][j] : 第 j+1 ビットについての安定ソート後の a[i] の第 j ビット
	array<vvi, 2> bs_acc; // bs_acc[b] : bs[*][b] のビット b=0,1 それぞれの個数の累積和
	vi num_zeros; // num_zeros[j] : bs[j] の 0 の個数
	vector<vector<T>> acc; // acc[j] : 第 j ビットについての安定ソート後の a の累積和
	vector<T> val; // 座圧前の値のユニークな昇順列

	// a[l..r) の中で [0..v) に値をもつ要素の個数を返す．
	int count_rsub(int l, int r, int v) {
		int cnt = 0;
		repir(j, m - 1, 0) {
			if (getb(v, j)) {
				cnt += bs_acc[0][j][r] - bs_acc[0][j][l];
				r = num_zeros[j] + bs_acc[1][j][r];
				l = num_zeros[j] + bs_acc[1][j][l];
			}
			else {
				r = bs_acc[0][j][r];
				l = bs_acc[0][j][l];
			}
		}

		return cnt;
	}

	// a[l..r) の中で [0..v) に値をもつ要素の和を返す．
	T sum_rsub(int l, int r, int v) {
		T res = 0;
		repir(j, m - 1, 0) {
			if (getb(v, j)) {
				res += acc[j][bs_acc[0][j][r]] - acc[j][bs_acc[0][j][l]];
				r = num_zeros[j] + bs_acc[1][j][r];
				l = num_zeros[j] + bs_acc[1][j][l];
			}
			else {
				r = bs_acc[0][j][r];
				l = bs_acc[0][j][l];
			}
		}

		return res;
	}

public:
	// 整数列 a[0..n) で初期化する．
	Wavelet_matrix(const vector<T>& a) : n(sz(a)) {
		// verify : https://judge.yosupo.jp/problem/static_range_frequency

		// a[0..n) を座標圧縮して t[0..n) にする．
		val = a;
		uniq(val);
		val.emplace_back((T)INFL + 1);
		vi t(n);
		rep(i, n) t[i] = lbpos(val, a[i]);

		m = msb(sz(val)) + 1;
		bs.resize(n);
		bs_acc[0] = bs_acc[1] = vvi(m, vi(n + 1));
		num_zeros.resize(m);
		acc.assign(m + 1, vector<T>(n + 1));

		// j : 注目ビット位置（上位ビットから順に見ていく）
		repir(j, m - 1, 0) {
			rep(i, n) {
				// 注目ビットが 1 か
				bs[i] |= t[i] & (1 << j);

				// ビット 0, 1 それぞれの個数の累積和を求めておく．
				rep(b, 2) bs_acc[b][j][i + 1] = bs_acc[b][j][i];
				int b = getb(t[i], j);
				bs_acc[b][j][i + 1]++;
				num_zeros[j] += 1 - b;

				// 要素の累積和の計算
				acc[j + 1][i + 1] = acc[j + 1][i] + val[t[i]];
			}

			// 注目ビットが 0 のものを左，1 のものを右に寄せる安定ソートを行う．
			vi nt0, nt1;
			nt0.reserve(num_zeros[j]);
			nt1.reserve(n - num_zeros[j]);

			rep(i, n) {
				if (getb(t[i], j)) nt1.push_back(t[i]);
				else nt0.push_back(t[i]);
			}
			t.clear();
			repe(x, nt0) t.push_back(x);
			repe(x, nt1) t.push_back(x);
		}

		// 要素の累積和の計算
		rep(i, n) acc[0][i + 1] = acc[0][i] + val[t[i]];
	}
	Wavelet_matrix() : n(0), m(0) {}

	// a[l..r) のうち昇順で i 番目の要素を返す．
	T get(int l, int r, int i) {
		// verify : https://judge.yosupo.jp/problem/range_kth_smallest

		chmax(l, 0); chmin(r, n);
		if (i >= r - l) return T(INFL);
		int ord = 0;

		repir(j, m - 1, 0) {
			ord <<= 1;

			int cnt0 = bs_acc[0][j][r] - bs_acc[0][j][l];
			if (i >= cnt0) {
				ord++;
				l = num_zeros[j] + bs_acc[1][j][l];
				r = num_zeros[j] + bs_acc[1][j][r];
				i -= cnt0;
			}
			else {
				l = bs_acc[0][j][l];
				r = bs_acc[0][j][r];
			}
		}

		return val[ord];
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
	int count(int l, int r, T v0, T v1) {
		// verify : https://atcoder.jp/contests/arc097/tasks/arc097_c

		chmax(l, 0); chmin(r, n);
		if (l >= r || v0 >= v1) return 0;

		int ord0 = lbpos(val, v0);
		int ord1 = lbpos(val, v1);

		return count_rsub(l, r, ord1) - count_rsub(l, r, ord0);
	}

	// a[l..r) の和を返す．
	T sum(int l, int r) {
		// verify : https://judge.yosupo.jp/problem/static_range_sum

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		return acc[m][r] - acc[m][l];
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
	T sum(int l, int r, T v0, T v1) {
		// verify : https://atcoder.jp/contests/abc339/tasks/abc339_g

		chmax(l, 0); chmin(r, n);;
		if (l >= r || v0 >= v1) return 0;

		int ord0 = lbpos(val, v0);
		int ord1 = lbpos(val, v1);

		return sum_rsub(l, r, ord1) - sum_rsub(l, r, ord0);
	}

	// Σi∈[l..r) |a[i] - v| を返す．
	T abs_sum(int l, int r, T v) {
		// verify : https://yukicoder.me/problems/no/2169

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		int ord = lbpos(val, v);

		T res = sum_rsub(l, r, (1 << m) - 1);
		res -= (r - l) * v;
		res -= 2 * sum_rsub(l, r, ord);
		res += 2 * count_rsub(l, r, ord) * v;

		return res;
	}
};


//【間引きウェーブレット行列】
/*
* Thinning_wavelet_matrix(vl a, int m) : O(n log n)
*	配列 a[0..n) と法 m で初期化する．
*
* ll get(int l, int r, int i, int k) : O(log n)
*	a[l..r) の中で昇順で i 番目の要素を返す．添字は ≡ k (mod m) の部分だけ見る．
*
* int count(int l, int r, ll v0, ll v1, int k) : O(log n)
*	a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．添字は ≡ k (mod m) の部分だけ見る．
*
* ll sum(int l, int r, int k) : O(1)
*	a[l..r) の和を返す．添字は ≡ k (mod m) の部分だけ見る．
*
* ll sum(int l, int r, ll v0, ll v1, int k) : O(log n)
*	a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．添字は ≡ k (mod m) の部分だけ見る．
*
* ll abs_sum(int l, int r, ll v, int k) : O(log n)
*	Σi∈[l..r) |a[i] - v| を返す．添字は ≡ k (mod m) の部分だけ見る．
* 
* 利用：【ウェーブレット行列】
*/
template <class T>
struct Thinning_wavelet_matrix {
	int m; // 法
	vector<Wavelet_matrix<T>> wms;

public:
	// 配列 a[0..n) と法 m で初期化する
	Thinning_wavelet_matrix(const vector<T>& a, int m) : m(m), wms(m) {
		// verify : https://codeforces.com/contest/1808/problem/D
		
		vector<vector<T>> a2(m);
		rep(i, sz(a)) a2[i % m].push_back(a[i]);
		rep(j, m) if (sz(a2[j]) > 0) wms[j] = Wavelet_matrix(a2[j]);
	}
	Thinning_wavelet_matrix() : m(1) {} // ダミー

	// a[l..r) のうち昇順で i 番目の要素を返す．
	T get(int l, int r, int i, int k) {
		return wms[k].get((l - k + m - 1) / m, (r - k + m - 1) / m, i);
	}

	// a[l..r) の和を返す．
	ll sum(int l, int r, int k) {
		return wms[k].sum((l - k + m - 1) / m, (r - k + m - 1) / m);
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
	int count(int l, int r, T v0, T v1, int k) {
		// verify : https://codeforces.com/contest/1808/problem/D

		return wms[k].count((l - k + m - 1) / m, (r - k + m - 1) / m, v0, v1);
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
	ll sum(int l, int r, T v0, T v1, int k) {
		return wms[k].sum((l - k + m - 1) / m, (r - k + m - 1) / m, v0, v1);
	}

	// Σi∈[l..r) |a[i] - v| を返す．
	ll abs_sum(int l, int r, T v, int k) {
		return wms[k].abs_sum((l - k + m - 1) / m, (r - k + m - 1) / m, v);
	}
};


//【部分文字列辞書（ユニーク）】
/*
* Unique_substring_dictionary(s) : O(n)
*	文字列 s[0..n) のユニークな部分文字列（空文字列は除く）で初期化する．
*
* ll size() : O(1)
*	ユニークな部分文字列の個数を返す．
*
* pii get(ll i) : O(log n)
*	辞書順で i 番目のユニークな部分文字列が s[l..r) であるとき {l, r} を返す．
*	（なければ {-1, -1} を返す）
*/
struct Unique_substring_dictionary {
	int n;

	// sa[i] : s の接尾辞のうち辞書順 i 番目のものの先頭位置
	vi sa;

	// acc[i] : s のユニークな部分文字列のうち辞書順で s[sa[i]..n) 以下のものの個数
	vl acc;

	// 文字列 s[0..n) の部分文字列（空文字列は除く）で初期化する．
	Unique_substring_dictionary(const string& s) : n(sz(s)), acc(n) {
		// verify : https://atcoder.jp/contests/arc097/tasks/arc097_a

		sa = suffix_array(s);
		auto la = lcp_array(s, sa);

		acc[0] = n - sa[0];
		repi(i, 1, n - 1) acc[i] = acc[i - 1] + (n - sa[i]) - la[i - 1];
	}
	Unique_substring_dictionary() : n(0) {}

	// 部分文字列の個数を返す．
	ll size() { return acc[n - 1]; }

	// 辞書順で i 番目の部分文字列が s[l..r) であるとき {l, r} を返す（なければ {-1, -1} を返す）
	pii get(ll i) {
		// verify : https://atcoder.jp/contests/arc097/tasks/arc097_a

		i++; // 1-indexed に直す

		// i 番目の部分文字列がどの接尾辞 s[sa[k]..n) の接頭辞かを探す．O(log n)
		auto it = lower_bound(all(acc), i);
		if (it == acc.end()) return { -1, -1 };
		int k = (int)distance(acc.begin(), it);

		// i から acc[k] に足りない分だけ後ろの文字を削ったものが求める部分文字列．
		return { sa[k], n - (int)(*it - i) };
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Unique_substring_dictionary& sd) {
		cout << sd.sa << endl << sd.acc << endl;
		return os;
	}
#endif
};


//【部分文字列辞書】
/*
* Substring_dictionary(s) : O(n)
*	文字列 s[0..n) の全ての部分文字列（空文字列は除く）で初期化する．
*
* pii get(ll i) : O(log n)
*	辞書順で i 番目の部分文字列が s[l..r) であるとき {l, r} を返す（なければ {-1, -1} を返す）
*
* 利用：【デカルト木】
*/
struct Substring_dictionary {
	int n; string s;

	// sa[i] : s の接尾辞のうち辞書順 i 番目のものの先頭位置
	vi sa;

	// id : 辞書順で何番目の接尾辞か，l : その何文字目からか，c : いくつの接尾辞にまたがるか
	vi id, l, c; vl acc;

	// 文字列 s[0..n) の部分文字列（空文字列は除く）で初期化する．
	Substring_dictionary(const string& s) : n(sz(s)), s(s), acc(1, 0) {
		// verify : https://yukicoder.me/problems/no/2361

		sa = suffix_array(s);

		if (n == 1) {
			id.push_back(0);
			l.push_back(0);
			c.push_back(1);
			acc.push_back(acc.back() + 1);
			return;
		}

		auto la = lcp_array(s, sa);

		// CT : 接尾辞木の代用品
		Cartesian_tree<int> CT(la);

		id.reserve(n); l.reserve(n); c.reserve(n); acc.reserve(n);
		function<void(int, int)> dfs = [&](int v, int len) {
			if (len < CT[v].val) {
				id.push_back(CT[v].l);
				l.push_back(len);
				c.push_back(CT[v].r - CT[v].l + 1);
				acc.push_back(acc.back() + (CT[v].val - len) * (CT[v].r - CT[v].l + 1));
			}

			if (CT[v].lc != -1) {
				dfs(CT[v].lc, CT[v].val);
			}
			else if ((n - sa[CT[v].l]) - CT[v].val > 0) {
				id.push_back(CT[v].l);
				l.push_back(CT[v].val);
				c.push_back(1);
				acc.push_back(acc.back() + ((n - sa[CT[v].l]) - CT[v].val));
			}

			if (CT[v].rc != -1) {
				dfs(CT[v].rc, CT[v].val);
			}
			else if ((n - sa[CT[v].r]) - CT[v].val > 0) {
				id.push_back(CT[v].r);
				l.push_back(CT[v].val);
				c.push_back(1);
				acc.push_back(acc.back() + ((n - sa[CT[v].r]) - CT[v].val));
			}
		};
		dfs(CT.rt, 0);
	}
	Substring_dictionary() : n(0) {}

	// 辞書順で i 番目の部分文字列が s[l..r) であるとき {l, r} を返す（なければ {-1, -1} を返す）
	pii get(ll i) const {
		// verify : https://yukicoder.me/problems/no/2361
		
		if (i < 0 || i >= (ll)n * (n + 1) / 2) return { -1, -1 };

		int k = ubpos(acc, i) - 1;
		return { sa[id[k]], sa[id[k]] + l[k] + (int)(i - acc[k]) / c[k] + 1 };
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Substring_dictionary& SD) {
		rep(i, SD.n * (SD.n + 1) / 2) {
			auto [l, r] = SD.get(i);
			os << "[" << l << "," << r << "] : " << SD.s.substr(l, r - l) << endl;
		}
		return os;
	}
#endif
};


//【組の和の辞書】
/*
* Outer_sum_dictionary(vl a, vl b) : O(n log n + m log m)
*	S = { a[i] + b[j] | i∈[0..n), j∈[0..m) } で初期化する．
*
* ll lower_bound(ll v) : O(n log m)
*	S の v 未満の要素の個数を返す．
*
* ll upper_bound(ll v) : O(n log m)
*	S の v 以下の要素の個数を返す．
*
* ll get(ll i) : O(n log m log(max(a+b) - min(a+b)))
*	S の i 番目の要素を返す．
*
* ll sum(ll i) : O(n log m log(max(a+b) - min(a+b)))
*	S の i 番目未満の要素の和を返す．
*/
struct Outer_sum_dictionary {
	int n, m;
	vl a, b, acc_b;

	// S = { a[i] + b[j] | i∈[0..n), j∈[0..m) } で初期化する．
	Outer_sum_dictionary(const vl& a_, const vl& b_) : n(sz(a_)), m(sz(b_)), a(a_), b(b_) {
		// verify : https://atcoder.jp/contests/abc149/tasks/abc149_e

		sort(all(a)); sort(all(b));

		acc_b = vl(m + 1);
		rep(j, m) acc_b[j + 1] = acc_b[j] + b[j];
	}
	Outer_sum_dictionary() : n(0), m(0) {}

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
		// verify : https://yukicoder.me/problems/no/989

		//【備考】
		// 尺取法でちゃんと書けば log を落とせる．

		ll cnt = 0;
		rep(i, n) cnt += lbpos(b, v - a[i]);
		return cnt;
	}

	// S の v 以下の要素の個数を返す．
	ll upper_bound(ll v) {
		ll cnt = 0;
		rep(i, n) cnt += ubpos(b, v - a[i]);
		return cnt;
	}

	// S の i 番目の要素を返す．
	ll get(ll i) {
		// verify : https://yukicoder.me/problems/no/2370

		ll ok = a[0] + b[0] - 1, ng = a[n - 1] + b[m - 1] + 1;
		while (abs(ok - ng) > 1) {
			ll mid = (ok + ng) / 2;

			if (lower_bound(mid) <= i) ok = mid;
			else ng = mid;
		}
		return ok;
	}

	// S の i 番目未満の要素の和を返す．
	ll sum(ll i) {
		// verify : https://atcoder.jp/contests/abc149/tasks/abc149_e

		// v : i 番目の要素 
		ll v = get(i);

		// sum : v 未満の要素の和, cnt : v 未満の要素の個数
		ll sum = 0, cnt = 0;
		rep(i, n) {
			int d = lbpos(b, v - a[i]);
			sum += a[i] * d + acc_b[d];
			cnt += d;
		}

		// 残り i - cnt 個の要素はちょうど v であるからその分を加算する．
		sum += v * (i - cnt);

		return sum;
	}
};


//【組の積の辞書】
/*
* Outer_mul_dictionary(vl a, vl b) : O(n log n + m log m)
*	S = { a[i] b[j] | i∈[0..n), j∈[0..m) } で初期化する．
*
* ll lower_bound(ll v) : O(n log m)
*	S の v 未満の要素の個数を返す．
*
* ll get(ll i) : O(n log m log(INFL))
*	S の i 番目の要素を返す．
*/
struct Outer_mul_dictionary {
	// n, m : a, b の要素数
	// np, mp : a, b の 正の要素数
	// nz, mz : a, b の 0 の要素数
	// nn, mn : a, b の 負の要素数
	int n, np, nz, nn, m, mp, mz, mn;

	// ap, bp : a, b の正の要素を昇順に格納したリスト
	// an, bn : a, b の負の要素の 絶対値 を昇順に格納したリスト
	vl ap, an, bp, bn;

	// S = { a[i] b[j] | i∈[0..n), j∈[0..m) } で初期化する．
	Outer_mul_dictionary(const vl& a, const vl& b) {
		// verify : https://yukicoder.me/problems/no/989

		np = nz = nn = 0;
		repe(x, a) {
			if (x > 0) {
				ap.push_back(x);
				np++;
			}
			else if (x < 0) {
				an.push_back(-x);
				nn++;
			}
			else {
				nz++;
			}
		}
		sort(all(ap)); sort(all(an));
		n = np + nz + nn;

		mp = mz = mn = 0;
		repe(x, b) {
			if (x > 0) {
				bp.push_back(x);
				mp++;
			}
			else if (x < 0) {
				bn.push_back(-x);
				mn++;
			}
			else {
				mz++;
			}
		}
		sort(all(bp)); sort(all(bn));
		m = mp + mz + mn;
	}
	Outer_mul_dictionary() : n(0), np(0), nz(0), nn(0), m(0), mp(0), mz(0), mn(0) {}

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
		// verify : https://yukicoder.me/problems/no/989

		//【備考】
		// 尺取法でちゃんと書けば log を落とせる．

		ll cnt = 0;
		if (v > 0) {
			cnt += (ll)m * n - (ll)np * mp - (ll)nn * mn;
			repe(x, ap) cnt += lbpos(bp, (v + x - 1) / x);
			repe(x, an) cnt += lbpos(bn, (v + x - 1) / x);
		}
		else if (v < 0) {
			repe(x, ap) {
				auto it = std::upper_bound(all(bn), -v / x);
				cnt += (ll)distance(it, bn.end());
			}
			repe(x, an) {
				auto it = std::upper_bound(all(bp), -v / x);
				cnt += (ll)distance(it, bp.end());
			}
		}
		else {
			cnt += (ll)np * mn + (ll)nn * mp;
		}
		return cnt;
	}

	// S の i 番目の要素を返す．
	ll get(ll i) {
		ll ok = -INFL, ng = INFL;
		while (abs(ok - ng) > 1) {
			ll mid = (ok + ng) / 2;

			if (lower_bound(mid) <= i) ok = mid;
			else ng = mid;
		}
		return ok;
	}
};


//【k-D 木（集合）】
/*
* KDTree<S>(vvS p) : O(n log n)
*	n 個の点 p[i] で初期化する．
*	制約 : 点の座標は互いに異なる．
*
* T search(vS p1, vS p2, vi& res) : O(n^(1 - 1/dim) + |res|)
*	半開直方体 R = [p1, p2) 内の点の番号を res に格納する．
*/
template <class S>
struct KDTree {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_C

	using vS = vector<S>; using vvS = vector<vS>; using vvSi = vector<pair<vS, int>>;
	const S S_MIN = numeric_limits<S>::lowest(), S_MAX = numeric_limits<S>::max();

	struct Node {
		vS p1, p2; // 半開直方体 R = [p1, p2) に対応するノードであることを表す
		int id; // R 内の点の注目座標に関する中央値に位置する点の番号
		Node* left, * right;

		Node() {}
		Node(vS p1_, vS p2_, int id_) : p1(p1_), p2(p2_), id(id_), left(nullptr), right(nullptr) {}
	};

	vvS p; // 点群
	Node* root; // 根へのポインタ
	int dim; // 次元

	// n 個の点 p[i] で初期化する．
	KDTree(vvS& p_) : p(p_), root(nullptr), dim(sz(p[0])) {
		int n = sz(p);

		vvSi pi(n);
		rep(i, n) pi[i] = { p[i], i };

		split(root, vS(dim, S_MIN), vS(dim, S_MAX), pi, 0, n, 0);
	}

	// ノード *t に点 p[i0..i1) を割り当て分割する．
	void split(Node*& t, vS p1, vS p2, vvSi& pi, int i0, int i1, int d) {
		// 空なら何もしない．
		if (i0 >= i1) return;

		// 要素が一つだけなら葉として格納して帰る．
		if (i0 + 1 == i1) {
			vS p0_inc = pi[i0].first;
			rep(i, dim) p0_inc[i]++;

			t = new Node(pi[i0].first, p0_inc, pi[i0].second);
			return;
		}

		// 中央値を得る．
		vector<pair<S, int>> cds;
		repi(i, i0, i1 - 1) cds.push_back({ pi[i].first[d], pi[i].second });
		sort(all(cds));
		S med = cds[sz(cds) / 2].first;

		// med を閾値として用い，それ未満のものを左，以上のものを右に移動する．
		int i = i0; // i : pi[i0, i) が med 未満の座標と確定
		repi(j, i0, i1 - 1) { // j : pi[i, j) が med 以上の座標と確定
			// j の位置に med 未満の座標がある場合
			if (pi[j].first[d] < med) {
				// 最も左の med 以上の座標と交換する．
				swap(pi[i], pi[j]);
				i++;
			}
		}

		// pi[i] に座標がちょうど med のものをもってくる．
		repi(j, i, i1 - 1) {
			if (pi[j].first[d] == med) {
				swap(pi[i], pi[j]);
				break;
			}
		}

		// med で分割して左右の子を作りに行く．
		t = new Node(p1, p2, 0);
		t->id = pi[i].second;
		vS p1m = p1, p2m = p2;
		p1m[d] = med; p2m[d] = med;
		split(t->left, p1, p2m, pi, i0, i, (d + 1) % dim);
		split(t->right, p1m, p2, pi, i + 1, i1, (d + 1) % dim);
	}

	// 半開直方体 R = [p1, p2) 内の点の番号を res に格納する．
	void search(const vS& p1, const vS& p2, vi& res) { return search_rf(root, p1, p2, 0, res); }

	void search_rf(Node* t, const vS& p1, const vS& p2, int d, vi& res) {
		// 木が空の場合
		if (t == nullptr) return;

		// 点が探索範囲に入っている場合は点を記録する
		bool in_flag = true;
		rep(i, dim) {
			if (p[t->id][i] < p1[i] || p2[i] <= p[t->id][i]) in_flag = false;
		}
		if (in_flag) res.push_back(t->id);

		// 探索範囲の左端が記録範囲の中央値以下なら左の子を調べにいく．
		if (p1[d] <= p[t->id][d]) {
			search_rf(t->left, p1, p2, (d + 1) % dim, res);
		}

		// 探索範囲の右端が記録範囲の中央値以上なら右の子を調べにいく．
		if (p2[d] >= p[t->id][d]) {
			search_rf(t->right, p1, p2, (d + 1) % dim, res);
		}
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const KDTree& kd) {
		kd.print_rf(os, kd.root);
		return os;
	}
	void print_rf(ostream& os, Node* t) const {
		if (t == nullptr) return;

		print_rf(os, t->left);
		os << "id:" << t->id << ", p1:" << t->p1 << ", p2:" << t->p2 << ", left:";
		t->left != nullptr ? os << "(" << t->left->p1 << "," << t->left->p2 << ")" : os << "-";
		os << ", right:";
		t->right != nullptr ? os << "(" << t->right->p1 << "," << t->right->p2 << ")" : os << "-";
		os << endl;
		print_rf(os, t->right);
	}
#endif
};


//【k-D トライ（写像）】
/*
* KDTrie(vvS p, vT val) : O(n log n)
*	n 個の点 p[i] に値 val[i] を持たせて初期化する．
*	制約 : 点の座標は互いに異なる．
*
* T sum(vS p1, vS p2) : O(n^(1 - 1/dim))
*	半開直方体 R = [p1, p2) 内の点の値の和を返す．
*/
template <class S, class T>
struct KDTrie {
	// verify : https://atcoder.jp/contests/abc075/tasks/abc075_d

	using vS = vector<S>; using vvS = vector<vS>; using vT = vector<T>;
	const S S_MIN = numeric_limits<S>::lowest(), S_MAX = numeric_limits<S>::max();

	struct Node {
		vS p1, p2; // 半開直方体 R = [p1, p2) に対応するノードであることを表す
		T val;
		Node *left, *right;

		Node() {}
		Node(vS p1_, vS p2_, T val_) : p1(p1_), p2(p2_), val(val_), left(nullptr), right(nullptr) {}
	};

	Node* root; // 根へのポインタ
	int dim; // 次元

	// n 個の点 p[i] に値 val[i] を持たせて初期化する．
	KDTrie(vvS& p, vT& val) : root(nullptr), dim(sz(p[0])) {
		int n = sz(p);
		split(root, vS(dim, S_MIN), vS(dim, S_MAX), p, val, 0, n, 0);
	}

	// ノード *t に点 p[i0..i1) と値 val[i0..i1) を割り当て分割する．
	T split(Node*& t, vS p1, vS p2, vvS& p, vT& val, int i0, int i1, int d) {
		// 空なら何もしない．
		if (i0 == i1) return 0;

		// 要素が一つだけなら葉として格納して帰る．
		if (i0 + 1 == i1) {
			vS p0_inc = p[i0];
			rep(i, dim) p0_inc[i]++;

			t = new Node(p[i0], p0_inc, val[i0]);
			return val[i0];
		}

		// 中央値を得る．
		vS cds;
		repi(i, i0, i1 - 1) cds.push_back(p[i][d]);
		uniq(cds);
		S med = cds[sz(cds) / 2];

		// med を閾値として用い，それ未満のものを左，以上のものを右に移動する．
		int i = i0; // i : p[i0, i) が med 未満の座標と確定
		repi(j, i0, i1 - 1) { // j : p[i, j) が med 以上の座標と確定
			// j の位置に med 未満の座標がある場合
			if (p[j][d] < med) {
				// 最も左の med 以上の座標と交換する．
				swap(p[i], p[j]); swap(val[i], val[j]);
				i++;
			}
		}

		// med で分割して左右の子を作りに行く．
		t = new Node(p1, p2, 0);
		vS p1m = p1, p2m = p2;
		p1m[d] = med; p2m[d] = med;
		t->val += split(t->left, p1, p2m, p, val, i0, i, (d + 1) % dim);
		t->val += split(t->right, p1m, p2, p, val, i, i1, (d + 1) % dim);

		return t->val;
	}

	// 半開直方体 R = [p1, p2) 内の点の値の和を返す．
	T sum(const vS& p1, const vS& p2) { return sum_rf(root, p1, p2, 0); }

	T sum_rf(Node* t, const vS& p1, const vS& p2, int d) {
		// 木が空の場合
		if (t == nullptr) return 0;

		// 一部も範囲に入っていない場合
		if (p2[d] <= t->p1[d] || t->p2[d] <= p1[d]) return 0;

		// 完全に範囲に入っている場合
		bool in_flag = true;
		rep(i, dim) {
			if (t->p1[i] < p1[i] || p2[i] < t->p2[i]) in_flag = false;
		}
		if (in_flag) return t->val;

		T val = 0;
		val += sum_rf(t->left, p1, p2, (d + 1) % dim);
		val += sum_rf(t->right, p1, p2, (d + 1) % dim);

		return val;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const KDTrie& kd) {
		kd.print_rf(os, kd.root);
		return os;
	}
	void print_rf(ostream& os, Node* t) const {
		if (t == nullptr) return;

		print_rf(os, t->left);
		os << "val:" << t->val << ", p1:" << t->p1 << ", p2:" << t->p2 << ", left:";
		t->left != nullptr ? os << "(" << t->left->p1 << "," << t->left->p2 << ")" : os << "-";
		os << ", right:";
		t->right != nullptr ? os << "(" << t->right->p1 << "," << t->right->p2 << ")" : os << "-";
		os << endl;
		print_rf(os, t->right);
	}
#endif
};


//【ウェーブレット行列】（旧）
/*
* Wavelet_matrix<T>(vT a) : O(n log n log A)
*	整数列 a[0..n) で初期化する．（A = max(|a[i]|) とおく．）
*
* T get(int i) : O(log A)
*	昇順で i 番目の要素を返す．
*
* T get(int l, int r, int i) : O(log A))
*	a[l..r) の中で昇順で i 番目の要素を返す．
*
* int count(int l, int r, T v) : O(log A)
*	a[l..r) に v が何個あるかを返す．
*
* int count(int l, int r, T v0, T v1) : O(log A)
*	a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
*	注意：点群 {(i, a[i])}_i に対し，矩形内に存在する点の個数を求めているとも解釈できる．
*
* int position(T v, int c) : O(log n log A)
*	昇順で c 番目の v の位置を返す．
*
* vector<pTi> frequency(int l, int r, int c) : O(min(r - l, A) log A)
*	a[l..r) の中で出現頻度降順に最大 c 個の要素と頻度の組のリストを返す．
*
* ll sum(int l, int r) : O(1)
*	a[l..r) の和を返す．
*
* ll sum(int l, int r, T v0, T v1) : O(log A)
*	a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
*
* vector<tTii> intersection(int l1, int r1, int l2, int r2) : O(min((r1 - l1) + (r2 - l2), A) log A)
*	a[l1..r1) と a[l2..r2) に共通する要素を求め，その値とそれぞれにおける出現頻度の三つ組のリストを返す．
*
* ll abs_sum(int l, int r, T v) : O(log A)
*	Σi∈[l..r) |a[i] - v| を返す．
*/
template <class T>
class Wavelet_matrix_old {
	// 参考 : https://miti-7.hatenablog.com/entry/2018/04/28/152259

	int n; // 要素数
	T shift; // 非負にするために履かせた下駄
	int k; // msb 以下の桁数（1-indexed）
	vvb bs; // bs[j][i] : 第 j + 1 ビットについての安定ソート後の a[i] の第 j ビット
	vvvi bs_acc; // bs[b] : のビット b = 0, 1 それぞれの個数の累積和
	vi num_zeros; // num_zeros[j] : bs[j] の 0 の個数
	unordered_map<T, int> id; // 値 → 安定ソートが終わったときの最左位置
	vvl acc; // acc[j] : 第 j ビットについての安定ソート後の a の累積和

	// a[0..r) に v が何個あるかを返す．
	int count_sub(int r, T v) {
		// 一つも無ければすぐに 0 を返す．
		if (!id.count(v)) return 0;

		// 最上位ビットから順に見ていく
		repir(j, k - 1, 0) {
			// 注目ビットに応じて次の位置を求めていく．
			if ((v >> j) & 1) {
				r = num_zeros[j] + bs_acc[1][j][r];
			}
			else {
				r = bs_acc[0][j][r];
			}
		}

		return r - id[v];
	}

	// a[l..r) の中で [0..v) に値をもつ要素の個数を返す．
	int count_rsub(int l, int r, T v) {
		if (msb(v) >= k) return r - l;

		int cnt = 0;
		repir(j, k - 1, 0) {
			if ((v >> j) & 1) {
				cnt += bs_acc[0][j][r] - bs_acc[0][j][l];
				r = num_zeros[j] + bs_acc[1][j][r];
				l = num_zeros[j] + bs_acc[1][j][l];
			}
			else {
				r = bs_acc[0][j][r];
				l = bs_acc[0][j][l];
			}
		}

		return cnt;
	}

	// a[l..r) の中で [0..v) に値をもつ要素の和を返す．
	ll sum_rsub(int l, int r, T v) {
		if (msb(v) >= k) return acc[k][r] - acc[k][l];

		ll res = 0;
		repir(j, k - 1, 0) {
			if ((v >> j) & 1) {
				res += acc[j][bs_acc[0][j][r]] - acc[j][bs_acc[0][j][l]];
				r = num_zeros[j] + bs_acc[1][j][r];
				l = num_zeros[j] + bs_acc[1][j][l];
			}
			else {
				r = bs_acc[0][j][r];
				l = bs_acc[0][j][l];
			}
		}

		return res;
	}

public:
	// 整数列 t で初期化する．
	Wavelet_matrix_old(const vector<T>& t)
		: n(sz(t)), shift(max(-*min_element(all(t)), T(0))), k(msb(*max_element(all(t)) + shift) + 1),
		bs(k, vb(n)), bs_acc(2, vvi(k, vi(n + 1))), num_zeros(k), acc(k + 1, vl(n + 1))
	{
		// verify : https://judge.yosupo.jp/problem/static_range_frequency

		// ビットと組にして安定ソートするためのリスト
		vector<pair<bool, T>> bt(n);
		rep(i, n) bt[i].second = t[i] + shift; // 下駄を履かせて非負にする．

		// j : 注目ビット位置（上位ビットから順に見ていく）
		repir(j, k - 1, 0) {
			rep(i, n) {
				// 注目ビットが 1 か
				bs[j][i] = bt[i].first = (bt[i].second & (T(1) << j));

				// ビット 0, 1 それぞれの個数の累積和を求めておく．
				rep(b, 2) bs_acc[b][j][i + 1] = bs_acc[b][j][i];
				if (bs[j][i]) {
					bs_acc[1][j][i + 1]++;
				}
				else {
					bs_acc[0][j][i + 1]++;
					num_zeros[j]++;
				}

				// 要素の累積和の計算
				acc[j + 1][i + 1] = acc[j + 1][i] + bt[i].second;
			}

			// 注目ビットが 0 のものを左，1 のものを右に寄せる安定ソートを行う．
			stable_sort(all(bt), [](auto const& lhs, auto const& rhs) {
				return lhs.first < rhs.first;
			});
		}

		rep(i, n) {
			// 値 → 安定ソートが終わったときの最左位置
			if (!id.count(bt[i].second)) id[bt[i].second] = i;

			// 要素の累積和の計算
			acc[0][i + 1] = acc[0][i] + bt[i].second;
		}
	}
	Wavelet_matrix_old() : n(0), shift(0), k(0) {}

	// 昇順で c 番目の v の位置を返す．
	int position(T v, int c) {
		v += shift;
		if (!id.count(v)) return -1;

		int i = id[v] + c;
		rep(j, k) {
			if ((v >> j) & 1) {
				i = ubpos(bs_acc[1][j], i - num_zeros[j]) - 1;
			}
			else {
				i = ubpos(bs_acc[0][j], i - num_zeros[j]) - 1;
			}
		}

		return i;
	}

	// 昇順で i 番目の要素を返す．
	T get(int i) {
		Assert(0 <= i && i < n);
		T res = 0;

		// 最上位ビットから順に見ていく
		repir(j, k - 1, 0) {
			res *= 2;

			// 注目ビットに応じて次の位置を求めつつ，値を更新していく．
			if (bs[j][i]) {
				res++;
				i = num_zeros[j] + bs_acc[1][j][i];
			}
			else {
				i = bs_acc[0][j][i];
			}
		}

		return res - shift;
	}

	// a[l..r) のうち昇順で i 番目の要素を返す．
	T get(int l, int r, int i) {
		// verify : https://judge.yosupo.jp/problem/range_kth_smallest

		chmax(l, 0); chmin(r, n);
		Assert(0 <= i && i < r - l);
		T res = 0;

		repir(j, k - 1, 0) {
			res *= 2;

			int cnt0 = bs_acc[0][j][r] - bs_acc[0][j][l];
			if (i >= cnt0) {
				res++;
				l = num_zeros[j] + bs_acc[1][j][l];
				r = num_zeros[j] + bs_acc[1][j][r];
				i -= cnt0;
			}
			else {
				l = bs_acc[0][j][l];
				r = bs_acc[0][j][r];
			}
		}

		return res - shift;
	}

	// a[l..r) に v が何個あるかを返す．
	int count(int l, int r, T v) {
		// verify : https://judge.yosupo.jp/problem/static_range_frequency

		chmax(l, 0); chmin(r, n); v += shift;
		if (l >= r) return 0;
		return count_sub(r, v) - count_sub(l, v);
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
	int count(int l, int r, T v0, T v1) {
		// verify : https://atcoder.jp/contests/arc097/tasks/arc097_c

		chmax(l, 0); chmin(r, n); v0 += shift; v1 += shift; chmax(v0, T(0));
		if (l >= r || v0 >= v1) return 0;

		return count_rsub(l, r, v1) - count_rsub(l, r, v0);
	}

	// a[l..r) の和を返す．
	ll sum(int l, int r) {
		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;
		return acc[k][r] - acc[k][l] - (ll)shift * (r - l);
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
	ll sum(int l, int r, T v0, T v1) {
		// verify : https://atcoder.jp/contests/abc276/tasks/abc276_f

		chmax(l, 0); chmin(r, n); v0 += shift; v1 += shift; chmax(v0, T(0));
		if (l >= r || v0 >= v1) return 0;

		ll res = sum_rsub(l, r, v1) - sum_rsub(l, r, v0);
		if (shift != 0) res -= (ll)shift * (count_rsub(l, r, v1) - count_rsub(l, r, v0));

		return res;
	}

	// Σi∈[l..r) |a[i] - v| を返す．
	ll abs_sum(int l, int r, T v) {
		// verify : https://yukicoder.me/problems/no/2169

		chmax(l, 0); chmin(r, n); v += shift;
		if (l >= r) return 0;
		if (v <= 0) return sum_rsub(l, r, T(INFL)) - (ll)v * (r - l);

		ll res = sum_rsub(l, r, T(INFL));
		res -= (ll)(r - l) * v;
		res -= 2 * sum_rsub(l, r, v);
		res += 2 * count_rsub(l, r, v) * v;

		return res;
	}

	// a[l..r) の中で出現頻度降順に最大 c 個の要素と頻度の組を返す．
	vector<pair<T, int>> frequency(int l, int r, int c) {
		chmax(l, 0); chmin(r, n);
		vector<pair<T, int>> freq;
		if (l >= r) return freq;

		priority_queue<tuple<int, int, int, int, T>> q;
		q.push({ r - l, k - 1, l, r, T(0) });

		// 出現頻度の高い値が多ければ c に応じて早めに打ち切られるが，
		// そうでなければ最悪 a[l..r) を調べ尽くしてしまう．
		while (!q.empty()) {
			int w, j; T v;
			tie(w, j, l, r, v) = q.top(); q.pop();

			if (j == -1) {
				freq.push_back({ v - shift, w });
				if (--c == 0) return freq;
			}
			else {
				int l1 = num_zeros[j] + bs_acc[1][j][l];
				int r1 = num_zeros[j] + bs_acc[1][j][r];
				int l0 = bs_acc[0][j][l];
				int r0 = bs_acc[0][j][r];

				q.push({ r1 - l1, j - 1, l1, r1, 2 * v + 1 });
				q.push({ r0 - l0, j - 1, l0, r0, 2 * v });
			}
		}

		return freq;
	}

	// a[l1..r1) と a[l2..r2) に共通する要素を求め，
	// その値とそれぞれにおける出現頻度の三つ組のリストを freq に格納する．
	vector<tuple<T, int, int>> intersection(int l1, int r1, int l2, int r2) {
		chmax(l1, 0); chmin(r1, n); chmax(l2, 0); chmin(r2, n);
		vector<tuple<T, int, int>> freq;
		if (l1 >= r1 || l2 >= r2) return freq;

		queue<tuple<int, int, int, int, int, T>> q;
		q.push({ k - 1, l1, r1, l2, r2, T(0) });

		while (!q.empty()) {
			int j; T v;
			tie(j, l1, r1, l2, r2, v) = q.front(); q.pop();

			if (l1 == r1 || l2 == r2) continue;

			if (j == -1) {
				freq.push_back({ v - shift, r1 - l1, r2 - l2 });
			}
			else {
				int l11 = num_zeros[j] + bs_acc[1][j][l1];
				int r11 = num_zeros[j] + bs_acc[1][j][r1];
				int l10 = bs_acc[0][j][l1];
				int r10 = bs_acc[0][j][r1];
				int l21 = num_zeros[j] + bs_acc[1][j][l2];
				int r21 = num_zeros[j] + bs_acc[1][j][r2];
				int l20 = bs_acc[0][j][l2];
				int r20 = bs_acc[0][j][r2];

				q.push({ j - 1, l11, r11, l21, r21, 2 * v + 1 });
				q.push({ j - 1, l10, r10, l20, r20, 2 * v });
			}
		}

		return freq;
	}
};


