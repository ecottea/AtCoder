#pragma once
#include "header.h"
#include "フェニック木(抽象).h"
#include "探索.h"
// ■■■■■ 辞書 ■■■■■


//【動的辞書】
/*
* Dynamic_dictionary(n) : O(n)
*	[0..n) を記録可能な辞書を空で初期化する．
*
* Dynamic_dictionary(n, a) : O(n)
*	[0..n) を記録可能な辞書を多重集合 a で初期化する．
*
* size() : O(log n)
*	要素の総数を返す．
*
* count(v) : O(log n)
*	要素 v の個数を返す．
* 
* count(l, r) : O(log n)
*	値 [l..r) をもつ要素の個数を返す．
*
* insert(v) : O(log n)
*	要素 v を挿入する．
*
* erase(v) : O(log n)
*	要素 v を削除する．個数は負数にもなる．
*
* get(i) : O(log n)
*	昇順で i 番目の要素（i : 0-indexed）を返す．なければ n を返す．
*
* lower_bound(v) : O(log n)
*	v が（あるとすれば）昇順で何番目の要素かを返す．（0-indexed）
*
* 利用：【フェニック木】
*/
using S12 = int;
S12 op12(S12 x, S12 y) { return x + y; }
S12 e12() { return 0; }
S12 inv12(S12 x) { return -x; }
struct Dynamic_dictionary {
	int n;

	// ft[v] : 要素 v の個数
	using RSQ = Fenwick_tree<S12, op12, e12, inv12>;
	RSQ ft;

	// コンストラクタ（何もしない）
	Dynamic_dictionary() : n(0) {}

	// [0..n) を記録可能な辞書を空で初期化する．
	Dynamic_dictionary(int n_) : n(n_), ft(n) {}

	// [0..n) を記録可能な辞書を多重集合 a で初期化する．
	Dynamic_dictionary(int n_, const vi& a) : n(n_) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem

		vi cnt(n);
		repe(v, a) cnt[v]++;
		ft = RSQ(cnt);
	}

	// 要素の総数を返す．
	int size() { return ft.prod(0, n); }

	// 要素 v の個数を返す．
	int count(int v) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem

		return ft.get(v);
	}

	// 値[l..r) をもつ要素の個数を返す．
	int count(int l, int r) { return ft.prod(l, r); }

	// 要素 v を挿入する．
	void insert(int v) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem
	
		ft.apply(v, 1);
	}

	// 要素 v を削除する．
	void erase(int v) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem
		
		ft.apply(v, -1);
	}

	// 昇順で i 番目の要素を返す．
	int get(int i) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem

		auto f = [&](ll x) { return x <= i; };
		return ft.max_right(f);
	}

	// v が昇順で何番目の要素かを返す．
	int lower_bound(int v) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem
		
		return ft.prod(0, v);
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Dynamic_dictionary& dd) {
		rep(v, dd.n) rep(hoge, dd.ft.get(v)) os << v << " ";
		return os;
	}
};


//【区間の動的辞書】
/*
* Interval_dictionary() : O(1)
*	空で初期化する．
*
* size() : O(1)
*	区間の数を返す．
*
* get(x) : O(log n)
*	x が含まれる区間 [l, r) を返す（なければ {-1, -1} を返す）
*
* get_right(x) : O(log n)
*	x の 1 つ右にある区間 [l, r) を返す（なければ {-1, -1} を返す）
*
* get_left(x) : O(log n)
*	x の 1 つ左にある区間 [l, r) を返す（なければ {-1, -1} を返す）
*
* insert(l, r) : ならし O(log n)
*	区間 [l, r) を追加する．
*
* erase(l, r) : ならし O(log n)
*	区間 [l, r) を削除する．
*/
struct Interval_dictionary {
	set<pll> lr; // 区間 [l[i], r[i]) の昇順列

	// コンストラクタ（空で初期化）
	Interval_dictionary() {}

	// 区間の数を返す．
	int size() { return sz(lr); }

	// x が含まれる区間 [l, r) を返す（なければ {-1, -1} を返す）
	pll get(ll x) {
		// verify : https://atcoder.jp/contests/abc228/tasks/abc228_d

		auto it = get_iter(x);
		return it == lr.end() ? make_pair(-1LL, -1LL) : *it;
	}

	// x が含まれる区間を指すイテレータを返す（なければ lr.end() を返す）
	typename set<pll>::iterator get_iter(ll x) {
		auto it = lr.lower_bound({ x, INFL });
		if (it == lr.begin()) return lr.end();
		it--;
		if (it->first <= x && x < it->second) return it;
		else return lr.end();
	}

	// x の 1 つ右にある区間 [l, r) を返す（なければ {-1, -1} を返す）
	pll get_right(ll x) {
		auto it = get_right_iter(x);
		return it == lr.end() ? make_pair(-1LL, -1LL) : *it;
	}

	// x の 1 つ右にある区間を指すイテレータを返す（なければ lr.end() を返す）
	typename set<pll>::iterator get_right_iter(ll x) {
		return lr.lower_bound({ x, INFL });
	}

	// x の 1 つ左にある区間 [l, r) を返す（なければ {-1, -1} を返す）
	pll get_left(ll x) {
		auto it = get_left_iter(x);
		return it == lr.end() ? make_pair(-1LL, -1LL) : *it;
	}

	// x の 1 つ左にある区間を指すイテレータを返す（なければ lr.end() を返す）
	typename set<pll>::iterator get_left_iter(ll x) {
		auto it = lr.lower_bound({ x, INFL });
		if (it == lr.begin()) return lr.end();
		it--;
		if (it->first <= x && x < it->second) {
			if (it == lr.begin()) return lr.end();
			it--;
		}
		return it;
	}

	// 区間 [l, r) を追加する．
	void insert(ll l, ll r) {
		auto it_l = get_iter(l - 1);
		if (it_l == lr.end()) it_l = get_right_iter(l - 1);

		auto it_r = get_iter(r);
		if (it_r == lr.end()) it_r = get_left_iter(r);

		if (it_l != lr.end() && it_r != lr.end()) {
			chmin(l, it_l->first);
			chmax(r, it_r->second);
			lr.erase(it_l, ++it_r);
		}
		lr.insert({ l, r });
	}

	// 区間 [l, r) を削除する．
	void erase(ll l, ll r) {
		ll l2 = l, r2 = r;

		auto it_l = get_iter(l);
		if (it_l != lr.end()) l2 = it_l->first;
		else it_l = get_right_iter(l - 1);

		auto it_r = get_iter(r);
		if (it_r != lr.end()) r2 = it_r->second;
		else it_r = get_left_iter(r);

		if (it_l != lr.end() && it_r != lr.end()) {
			lr.erase(it_l, ++it_r);
		}

		if (l2 < l) lr.insert({ l2, l });
		if (r2 > r) lr.insert({ r, r2 });
	}

	typename set<pll>::iterator begin() { return lr.begin(); }
	typename set<pll>::iterator end() { return lr.end(); }

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Interval_dictionary& d) {
		repe(p, d.lr) os << p << " ";
		return os;
	}
};


//【トライ木】
/*
* Trie_tree() : O(1)
*   空文字列のみで初期化する．
*
* insert(str) : O(|str|)
*   文字列 str を登録する．
*
* find(str) : O(|str|)
*   文字列 str が登録されているかを返す．
*
* find_prefix(str) : O(|str|)
*   文字列 str を接頭辞にもつ文字列が登録されているかを返す．
*
* count() : O(1)
*   登録されている文字列の個数を返す．
*/
struct Trie_tree {
	// 参考 : https://algo-logic.info/trie-tree/

	const int K = 26; // 文字数

	int n; // g のノード数
	Graph g; // トライ木
	vc chars; // 頂点 g[i] に対応する文字
	vb end; // g[i] で終わる文字があるか
	vi cnt; // g[i] を含む文字列の個数

	Trie_tree() : n(1), g(1, vi(K, -1)), chars(1), end(1), cnt(1) {}

	void insert(const string& str) {
		int v = 0;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// str は頂点 v を含む文字列なので個数に加算する．
			cnt[v]++;

			// 登録済みの文字だった場合
			if (g[v][c - 'a'] != -1) {
				// そのノードへ移動
				v = g[v][c - 'a'];
			}
			// 未登録の文字だった場合
			else {
				// 新たにノード n を追加
				g.push_back(vi(K, -1));
				chars.push_back(c);
				end.push_back(false);
				cnt.push_back(0);

				// 新たなノードへのパスを追加
				g[v][c - 'a'] = n;

				// 新たなノードへ移動
				v = n++;
			}
		}

		end[v] = true;
	}

	bool find(const string& str) const {
		return find_sub(str, false);
	}

	bool find_prefix(const string& str) const {
		return find_sub(str, true);
	}

	bool find_sub(const string& str, bool prefix_flag) const {
		int v = 0;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// 登録済みの文字だった場合
			if (g[v][c - 'a'] != -1) {
				// そのノードへ移動
				v = g[v][c - 'a'];
			}
			// 未登録の文字だった場合
			else {
				return false;
			}
		}

		return end[v] | prefix_flag;
	}

	int count() const {
		return cnt[0];
	}
};


//【ウェーブレット行列】
/*
* Wavelet_matrix(a) : O(n)
*	辞書を非負整数列 a で初期化する．
*
* get(i) : O(log(max a))
*	昇順で i 番目の要素を返す．
*
* get(l, r, i) : O(log(max a))
*	a[l..r) の中で昇順で i 番目の要素を返す．
*
* count(l, r, v) : O(log(max a))
*	a[l..r) に v が何個あるかを返す．
*
* count(l, r, v0, v1) : O(log(max a))
*	a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
*
* position(v, c) : O(log(n) log(max a))
*	昇順で c 番目の v の位置を返す．
*
* frequency(l, r, c, freq) : O(min(r - l, max a) log(max a))
*	a[l..r) の中で出現頻度降順に最大 c 個の要素と頻度の組のリストを freq に格納する．
*
* sum(l, r) : O(1)
*	a[l..r) の和を返す．
*
* sum(l, r, v0, v1) : O(log(max a))
*	a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
*
* intersection(l1, r1, l2, r2, freq) : O(min((r1 - l1) + (r2 - l2), max a) log(max a))
*	a[l1..r1) と a[l2..r2) に共通する要素を求め，
*	その値とそれぞれにおける出現頻度の三つ組のリストを freq に格納する．
*/
struct Wavelet_matrix {
	// 参考 : https://miti-7.hatenablog.com/entry/2018/04/28/152259

	int n; // 要素数
	int k; // msb 以下の桁数（1-indexed）
	vvb bs; // bs[j][i] : 第 j + 1 ビットについての安定ソート後の a[i] の第 j ビット
	vvvi bs_acc; // bs[b] : のビット b = 0, 1 それぞれの個数の累積和
	vi num_zeros; // num_zeros[j] : bs[j] の 0 の個数
	unordered_map<ll, int> id; // 値 → 安定ソートが終わったときの最左位置
	vvl acc; // acc[j] : 第 j ビットについての安定ソート後の a の累積和

	// コンストラクタ（初期化なし，多重集合 t で初期化）
	Wavelet_matrix() : n(0) {}
	Wavelet_matrix(const vl& t)
		: n(sz(t)), k(msbll(*max_element(all(t))) + 1),
		bs(k, vb(n)), bs_acc(2, vvi(k, vi(n + 1))), num_zeros(k), acc(k + 1, vl(n + 1))
	{
		// verify : https://judge.yosupo.jp/problem/static_range_frequency

		// ビットと組にして安定ソートするためのリスト
		vector<pair<bool, ll>> bt(n);
		rep(i, n) {
			bt[i].second = t[i];
		}

		// j : 注目ビット位置（上位ビットから順に見ていく）
		repir(j, k - 1, 0) {
			rep(i, n) {
				// 注目ビットが 1 か
				bs[j][i] = bt[i].first = (bt[i].second & (1LL << j));

				// ビット 0, 1 それぞれの個数の累積和を求めておく．
				rep(b, 2) {
					bs_acc[b][j][i + 1] = bs_acc[b][j][i];
				}
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

	// 昇順で i 番目の要素を返す．
	ll get(int i) {
		ll res = 0;

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

		return res;
	}

	// a[l..r) に v が何個あるかを返す．
	int count(int l, int r, ll v) {
		// verify : https://judge.yosupo.jp/problem/static_range_frequency

		return count_sub(r, v) - count_sub(l, v);
	}

	// a[0..r) に v が何個あるかを返す．
	int count_sub(int r, ll v) {
		// 一つも無ければすぐに 0 を返す．
		if (!id.count(v)) return 0;

		// 最上位ビットから順に見ていく
		repir(j, k - 1, 0) {
			// 注目ビットに応じて次の位置を求めていく．
			if (v & (1LL << j)) {
				r = num_zeros[j] + bs_acc[1][j][r];
			}
			else {
				r = bs_acc[0][j][r];
			}
		}

		return r - id[v];
	}

	// 昇順で c 番目の v の位置を返す．
	int position(ll v, int c) {
		if (!id.count(v)) return -1;

		int i = id[v] + c;
		rep(j, k) {
			if (v & (1LL << j)) {
				auto it = upper_bound(all(bs_acc[1][j]), i - num_zeros[j]);
				i = distance(bs_acc[1][j].begin(), it) - 1;
			}
			else {
				auto it = upper_bound(all(bs_acc[0][j]), i);
				i = distance(bs_acc[0][j].begin(), it) - 1;
			}
		}

		return i;
	}

	// a[l..r) のうち昇順で i 番目の要素を返す．
	ll get(int l, int r, int i) {
		ll res = 0;

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

		return res;
	}

	// a[l..r) の中で出現頻度降順に最大 c 個の要素と頻度の組を返す．
	void frequency(int l, int r, int c, vector<pli>& freq) {
		freq.clear();

		priority_queue<tuple<int, int, int, int, ll>> q;
		q.push({ r - l, k - 1, l, r, 0 });

		while (!q.empty()) {
			int w, j;
			ll v;
			tie(w, j, l, r, v) = q.top();
			q.pop();

			if (j == -1) {
				freq.push_back({ v, w });
				if (--c == 0) return;
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
	}

	// a[l..r) の和を返す．
	ll sum(int l, int r) {
		return acc[k][r] - acc[k][l];
	}

	//// a[l..r) の和を返す． O(min(r - l, max a) log(max a))
	//ll sum(int l, int r) {
	//	vector<pli> freq;
	//	frequency(l, r, INF, freq);
	//	ll res = 0;
	//	repe(p, freq) {
	//		res += p.first * p.second;
	//	}
	//	return res;
	//}

	// a[l1..r1) と a[l2..r2) に共通する要素を求め，
	// その値とそれぞれにおける出現頻度の三つ組のリストを freq に格納する．
	void intersection(int l1, int r1, int l2, int r2, vector<tuple<ll, int, int>>& freq) {
		freq.clear();

		queue<tuple<int, int, int, int, int, ll>> q;
		q.push({ k - 1, l1, r1, l2, r2, 0 });

		while (!q.empty()) {
			int j;
			ll v;
			tie(j, l1, r1, l2, r2, v) = q.front();
			q.pop();

			if (l1 == r1 || l2 == r2) {
				continue;
			}

			if (j == -1) {
				freq.push_back({ v, r1 - l1, r2 - l2 });
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
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
	int count(int l, int r, ll v0, ll v1) {
		return count_rsub(l, r, v1) - count_rsub(l, r, v0);
	}

	// a[l..r) の中で [0..v) に値をもつ要素の個数を返す．
	int count_rsub(int l, int r, ll v) {
		if (msbll(v) >= k) return r - l;

		int cnt = 0;
		repir(j, k - 1, 0) {
			if (v & (1LL << j)) {
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

	// a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
	ll sum(int l, int r, ll v0, ll v1) {
		return sum_rsub(l, r, v1) - sum_rsub(l, r, v0);
	}

	// a[l..r) の中で [0..v) に値をもつ要素の和を返す．
	ll sum_rsub(int l, int r, ll v) {
		if (msbll(v) >= k) return acc[k][r] - acc[k][l];

		ll res = 0;
		repir(j, k - 1, 0) {
			if (v & (1LL << j)) {
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
};


//【部分文字列辞書】
/*
* Substring_dictionary(s) : O(|s|)
*	文字列 s の部分文字列（空文字列は除く）で初期化する．
*
* size() : O(1)
*	部分文字列の個数を返す．
*
* get(i) : O(|s|)
*	辞書順で i 番目の部分文字列を返す．（0-indexed, なければ "" を返す）
*/
struct Substring_dictionary {
	int n;
	string s;

	// sa[i] : s の接尾辞 s[j..n) のうち辞書順 i 番目のものの先頭位置 j（0-indexed）
	// la[i] : s[sa[i]..n) と s[sa[i+1]..n) の最長共通接頭辞の長さ
	vi sa, la;

	// cnt[i] : s の部分文字列のうち，s[sa[i]..n) 以下のものの個数
	vl cnt;

	// コンストラクタ
	Substring_dictionary() : n(0) {}
	Substring_dictionary(const string& s_) : n(sz(s_)), s(s_), cnt(n) {
		sa = suffix_array(s);
		la = lcp_array(s, sa);

		cnt[0] = n - sa[0];
		repi(i, 1, n - 1) {
			cnt[i] = cnt[i - 1] + (n - sa[i]) - la[i - 1];
		}
	}

	// 部分文字列の個数を返す．
	ll size() { return cnt[n - 1]; }

	// i 番目の部分文字列を返す．
	string get(ll i) {
		i++; // 1-indexed に直す

		// i 番目の部分文字列がどの接尾辞 s[sa[k]..n) の接頭辞かを探す．O(log n)
		auto it = lower_bound(all(cnt), i);
		if (it == cnt.end()) return "";
		int k = distance(cnt.begin(), it);

		// i から cnt[k] に足りない分だけ後ろの文字を削ったものが求める部分文字列．
		// c++ は s.substr(i, w) : s[i..i+w) なので注意．
		return s.substr(sa[k], n - (*it - i) - sa[k]);
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Substring_dictionary& sd) {
		cout << sd.n << endl << sd.s << endl << sd.sa << endl << sd.la << endl
			<< sd.cnt << endl;
		return os;
	}
};


//【組の和の辞書】
/*
* Outer_sum_dictionary(a, b) : O(n log n + m log m)
*	S = { a[i] + b[j] | i∈[0..n), j∈[0..m) } で初期化する．
*
* lower_bound(v) : O(n log m)
*	S の v 未満の要素の個数を返す．
*
* upper_bound(v) : O(n log m)
*	S の v 以下の要素の個数を返す．
*
* get(i) : O(n log m log(max(a+b) - min(a+b)))
*	S の i 番目の要素を返す．
*
* sum(i) : O(n log m log(max(a+b) - min(a+b)))
*	S の i 番目未満の要素の和を返す．
*
* 利用：【めぐる式二分探索】
*/
struct Outer_sum_dictionary {
	int n, m;
	vl a, b, acc_b;

	Outer_sum_dictionary() {}

	// S = { a[i] + b[j] | i∈[0..n), j∈[0..m) } で初期化する．
	Outer_sum_dictionary(const vl& a_, const vl& b_)
		: n(sz(a_)), m(sz(b_)), a(a_), b(b_) {
		sort(all(a));
		sort(all(b));

		acc_b = vl(m + 1);
		rep(j, m) {
			acc_b[j + 1] = acc_b[j] + b[j];
		}
	}

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
		// varify : https://algo-method.com/tasks/381

		ll cnt = 0;
		rep(i, n) {
			auto it = std::lower_bound(all(b), v - a[i]);
			cnt += distance(b.begin(), it);
		}
		return cnt;
	}

	// S の v 以下の要素の個数を返す．
	ll upper_bound(ll v) {
		ll cnt = 0;
		rep(i, n) {
			auto it = std::upper_bound(all(b), v - a[i]);
			cnt += distance(b.begin(), it);
		}
		return cnt;
	}

	// S の i 番目の要素を返す．
	ll get(ll i) {
		function<bool(ll)> okQ = [&](ll v) {
			return lower_bound(v) <= i;
		};
		return meguru_search(a[0] + b[0] - 1, a[n - 1] + b[m - 1] + 1, okQ);
	}

	// S の i 番目未満の要素の和を返す．
	ll sum(ll i) {
		// v : i 番目の要素 
		ll v = get(i);

		// sum : v 未満の要素の和
		// cnt : v 未満の要素の個数
		ll sum = 0, cnt = 0;
		rep(i, n) {
			auto it = std::lower_bound(all(b), v - a[i]);
			int d = (int)distance(b.begin(), it);
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
* Outer_mul_dictionary(a, b) : O(n log n + m log m)
*	S = { a[i] b[j] | i∈[0..n), j∈[0..m) } で初期化する．
*
* lower_bound(v) : O(n log m)
*	S の v 未満の要素の個数を返す．
*
* get(i) : O(n log m log(INFL))
*	S の i 番目の要素を返す．
*
* 利用：【めぐる式二分探索】
*/
struct Outer_mul_dictionary {
	// n, m : a, b の要素数
	// np, mp : a, b の正の要素数
	// nz, mz : a, b の 0 の要素数
	// nn, mn : a, b の負の要素数
	int n, np, nz, nn, m, mp, mz, mn;

	// ap, bp : a, b の正の要素を昇順に格納したリスト
	// an, bn : a, b の負の要素の 絶対値 を昇順に格納したリスト
	vl ap, an, bp, bn;

	Outer_mul_dictionary() {}

	// S = { a[i] b[j] | i∈[0..n), j∈[0..m) } で初期化する．
	Outer_mul_dictionary(const vl& a, const vl& b) {
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
		sort(all(ap));
		sort(all(an));
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
		sort(all(bp));
		sort(all(bn));
		m = mp + mz + mn;
	}

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
		ll cnt = 0;
		if (v > 0) {
			cnt += (ll)m * n - (ll)np * mp - (ll)nn * mn;
			repe(x, ap) {
				auto it = std::lower_bound(all(bp), (v + x - 1) / x);
				cnt += distance(bp.begin(), it);
			}
			repe(x, an) {
				auto it = std::lower_bound(all(bn), (v + x - 1) / x);
				cnt += distance(bn.begin(), it);
			}
		}
		else if (v < 0) {
			repe(x, ap) {
				auto it = std::upper_bound(all(bn), -v / x);
				cnt += distance(it, bn.end());
			}
			repe(x, an) {
				auto it = std::upper_bound(all(bp), -v / x);
				cnt += distance(it, bp.end());
			}
		}
		else {
			cnt += (ll)np * mn + (ll)nn * mp;
		}
		return cnt;
	}

	// S の i 番目の要素を返す．
	ll get(ll i) {
		function<bool(ll)> okQ = [&](ll v) {
			return lower_bound(v) <= i;
		};
		return meguru_search(-INFL, INFL, okQ);
	}
};


