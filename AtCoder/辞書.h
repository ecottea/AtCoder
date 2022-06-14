#pragma once
#include "header.h"
#include "フェニック木(抽象).h"
#include "探索.h"
// ■■■■■ 辞書 ■■■■■


//【binary trie】
/*
* Binary_trie<T>(int B = 63) : O(1)
*   型 T の B ビット整数を扱えるよう空で初期化する．
*
* ll size() : O(1)
*   要素数を返す．
*
* bool empty() : O(1)
*   要素が 0 個かを返す．
*
* insert(T val, ll cnt = 1) : O(B)
*   値 val を cnt 個追加する．
*
* erase(T val, ll cnt = 1) : O(B)
*   値 val を cnt 個削除する．
*
* T max_element(T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，最大要素を返す．
*
* T min_element(T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，最小要素を返す．
*
* T get(ll i, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，昇順で i 番目（0-indexed）の要素を返す．
*
* ll lower_bound(T val, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，
*   val 以上の最小の要素が昇順で何番目の要素かを返す．（0-indexed）
*
* ll upper_bound(T val, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，
*   val より大きい最小の要素が昇順で何番目の要素かを返す．（0-indexed）
*
* ll count(T val) : O(B)
*   要素 val の個数を返す．
*
* ll count(T l, T r, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，値 [l..r) をもつ要素の個数を返す．
*/
template <class T = ll> class Binary_trie {
	// 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/06/022654

	struct Node {
		ll cnt; // 部分木のもつ要素の個数
		Node* ch[2]; // 左右の子へのポインタ

		Node() : cnt(0), ch{ nullptr, nullptr } {}
	};

	Node* root; // 根へのポインタ
	int B; // 何ビット整数を扱うか

	Node* insert_sub(Node* t, T val, ll cnt, int b) {
		// まだノードがなければ作成する．
		if (t == nullptr) t = new Node;

		// 個数を増やす．
		t->cnt += cnt;

		// 自身が葉ならすぐに帰る．
		if (b < 0) return t;

		// 下位ビットに対応するノードに加算しにいく．
		T f = (val >> b) & T(1);
		t->ch[f] = insert_sub(t->ch[f], val, cnt, b - 1);

		// 自身へのポインタを親に返す．
		return t;
	}

	Node* erase_sub(Node* t, T val, ll cnt, int b) {
		// 存在しない要素を削除しようとすればエラーを返す．
		assert(t != nullptr && t->cnt >= cnt);

		// 個数を減らす．
		t->cnt -= cnt;

		// 要素が 0 個になったならノードを削除する．
		if (t->cnt == 0) {
			delete t;
			return nullptr;
		}

		// 自身が葉ならすぐに帰る．
		if (b < 0) return t;

		// 下位ビットに対応するノードに減算しにいく．
		T f = (val >> b) & T(1);
		t->ch[f] = erase_sub(t->ch[f], val, cnt, b - 1);

		// 自身へのポインタを親に返す．
		return t;
	}

	T min_element_sub(Node* t, T mask, int b) const {
		assert(t != nullptr);

		// 葉なら 0 を返す．
		if (b < 0) return 0;

		// 下位ビットに対応するノードの最小値を求めにいく．
		T g = (mask >> b) & T(1);
		if (t->ch[g] == nullptr) g ^= T(1);
		T val = min_element_sub(t->ch[g], mask, b - 1);

		// 自身のビットを設定する．
		val |= g << b;

		return val;
	}

	T get_sub(Node* t, T mask, ll k, int b) const {
		// 葉なら 0 を返す．
		if (b < 0) return 0;

		// 左の部分木に含まれる要素の個数をみて適切な子に探索しにいく．
		T g = (mask >> b) & T(1);
		ll lk = (t->ch[g] != nullptr ? t->ch[g]->cnt : 0);
		T val;
		if (k < lk) val = get_sub(t->ch[g], mask, k, b - 1) | (g << b);
		else val = get_sub(t->ch[g ^ T(1)], mask, k - lk, b - 1) | ((g ^ T(1)) << b);

		return val;
	}

	ll lower_bound_sub(Node* t, T mask, T val, int b) const {
		// 葉であるかまたはノードがなければ 0 を返す．
		if (t == nullptr || b < 0) return 0;

		// val の第 b ビットをみて適切な子に探索しにいく．
		T f = (val >> b) & T(1);
		T g = (mask >> b) & T(1);
		ll res = 0;
		if (f == T(1) && t->ch[g] != nullptr) res += t->ch[g]->cnt;
		res += lower_bound_sub(t->ch[f ^ g], mask, val, b - 1);

		return res;
	}

	void print_sub(Node* t, T val, int b, ostream& os) const {
		if (t == nullptr) return;

		if (b < 0) {
			os << "(" << val << "," << t->cnt << ") ";
			return;
		}

		print_sub(t->ch[0], val << 1, b - 1, os);
		print_sub(t->ch[1], (val << 1) | T(1), b - 1, os);
	}

public:
	// 空で初期化する． : O(1)
	Binary_trie(int B_ = 63) : root(nullptr), B(B_) {}

	// 要素数を返す． : O(1)
	ll size() const {
		return root != nullptr ? root->cnt : 0;
	}

	// 要素が 0 個かを返す． : O(1)
	bool empty() const {
		return root == nullptr;
	}

	// 値 val を cnt[=1] 個追加する． : O(B)
	void insert(T val, ll cnt = 1) {
		// verify : https://codeforces.com/contest/947/problem/C

		root = insert_sub(root, val, cnt, B - 1);
	}

	// 値 val を cnt[=1] 個削除する． : O(B)
	void erase(T val, ll cnt = 1) {
		// verify : https://codeforces.com/contest/947/problem/C

		root = erase_sub(root, val, cnt, B - 1);
	}

	// mask[=0] との XOR をとったときの最大要素を返す． : O(B)
	T max_element(T mask = 0) const {
		return min_element_sub(root, ~mask, B - 1);
	}

	// mask[=0] との XOR をとったときの最小要素を返す． : O(B)
	T min_element(T mask = 0) const {
		// verify : https://codeforces.com/contest/947/problem/C

		return min_element_sub(root, mask, B - 1);
	}

	// mask[=0] との XOR をとったときの昇順で i 番目（0-indexed）の要素を返す． : O(B)
	T get(ll i, T mask = 0) const {
		assert(0 <= i && i < size());
		return get_sub(root, mask, i, B - 1);
	}

	// mask[=0] との XOR をとったときの val 以上の最小の要素が昇順で何番目の要素かを返す．（0-indexed） : O(B)
	ll lower_bound(T val, T mask = 0) const {
		// verify : https://www.spoj.com/problems/SUBXOR/

		return lower_bound_sub(root, mask, val, B - 1);
	}

	// mask[=0] との XOR をとったときの val より大きい最小の要素が昇順で何番目の要素かを返す．（0-indexed） : O(B)
	ll upper_bound(T val, T mask = 0) const {
		// verify : https://codeforces.com/contest/966/problem/C

		// val + 1 が B ビット整数に収まらない場合の例外処理
		if (val == (T(1) << B) - 1) return size();

		return lower_bound_sub(root, mask, val + 1, B - 1);
	}

	// 要素 val の個数を返す． : O(B)
	ll count(T val) const {
		return upper_bound(val) - lower_bound(val);
	}

	// mask[=0] との XOR をとったときの [l..r) に値をもつ要素の個数を返す． : O(B)
	ll count(T l, T r, T mask = 0) const {
		// verify : https://www.spoj.com/problems/SUBXOR/

		return lower_bound(r, mask) - lower_bound(l, mask);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Binary_trie& bt) {
		bt.print_sub(bt.root, T(0), bt.B - 1, os);
		return os;
	}
#endif
};


//【区間の集合】
/*
* Interval_set(bool marge = true) : O(1)
*	空で初期化する．ちょうど接する区間を併合するかを marge に渡す．
*
* int size() : O(1)
*	区間の数を返す．
*
* pll get(ll x) : O(log n)
*	x が含まれる区間 [l, r) を返す（なければ {-1, -1} を返す）
*
* pll get_right(ll x) : O(log n)
*	x の 1 つ右にある区間 [l, r) を返す（なければ {-1, -1} を返す）
*
* pll get_left(ll x) : O(log n)
*	x の 1 つ左にある区間 [l, r) を返す（なければ {-1, -1} を返す）
*
* insert(ll l, ll r) : ならし O(log n)
*	区間 [l, r) を追加する．
*
* erase(ll l, ll r) : ならし O(log n)
*	区間 [l, r) を削除する．
*/
class Interval_set {
	bool marge;

	// x が含まれる区間を指すイテレータを返す（なければ lr.end() を返す）
	typename set<pll>::iterator get_iter(ll x) const {
		auto it = lr.lower_bound({ x, INFL });
		if (it == lr.begin()) return lr.end();
		it--;
		if (it->first <= x && x < it->second) return it;
		else return lr.end();
	}

	// x の 1 つ右にある区間を指すイテレータを返す（なければ lr.end() を返す）
	typename set<pll>::iterator get_right_iter(ll x) const {
		return lr.lower_bound({ x, INFL });
	}

	// x の 1 つ左にある区間を指すイテレータを返す（なければ lr.end() を返す）
	typename set<pll>::iterator get_left_iter(ll x) const {
		auto it = lr.lower_bound({ x, INFL });
		if (it == lr.begin()) return lr.end();
		it--;
		if (it->first <= x && x < it->second) {
			if (it == lr.begin()) return lr.end();
			it--;
		}
		return it;
	}

	typename set<pll>::iterator begin() const { return lr.begin(); }
	typename set<pll>::iterator end() const { return lr.end(); }

public:
	set<pll> lr; // 区間 [l[i], r[i]) の昇順列

	// コンストラクタ（空で初期化）
	Interval_set(bool marge_ = true) : marge(marge_) {}

	// 区間の数を返す．
	int size() const { return sz(lr); }

	// x が含まれる区間 [l, r) を返す（なければ {-1, -1} を返す）
	pll get(ll x) const {
		// verify : https://atcoder.jp/contests/abc228/tasks/abc228_d

		auto it = get_iter(x);
		return it == lr.end() ? make_pair(-1LL, -1LL) : *it;
	}

	// x の 1 つ右にある区間 [l, r) を返す（なければ {-1, -1} を返す）
	pll get_right(ll x) const {
		auto it = get_right_iter(x);
		return it == lr.end() ? make_pair(-1LL, -1LL) : *it;
	}

	// x の 1 つ左にある区間 [l, r) を返す（なければ {-1, -1} を返す）
	pll get_left(ll x) const {
		auto it = get_left_iter(x);
		return it == lr.end() ? make_pair(-1LL, -1LL) : *it;
	}

	// 区間 [l, r) を追加する．
	void insert(ll l, ll r) {
		// verify : https://atcoder.jp/contests/abc254/tasks/abc254_g

		if (l >= r) return;

		auto it_l = get_iter(l - (int)marge);
		if (it_l == lr.end()) it_l = get_right_iter(l - (int)marge);

		auto it_r = get_iter(r - (int)(!marge));
		if (it_r == lr.end()) it_r = get_left_iter(r - (int)(!marge));

		if (it_l != lr.end() && it_r != lr.end()) {
			chmin(l, it_l->first);
			chmax(r, it_r->second);
			lr.erase(it_l, ++it_r);
		}
		lr.insert({ l, r });
	}

	// 区間 [l, r) を削除する．
	void erase(ll l, ll r) {
		if (l >= r) return;

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

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Interval_set& d) {
		repe(p, d.lr) os << p << " ";
		return os;
	}
#endif
};


//【区間からの写像】
/*
* Interval_map<S, T>(T nil) : O(1)
*	S の全ての値に nil を割り当てる．
*
* void set(S l, S r, T v) : ならし O(log n)
*	区間 [l..r) に値 v を割り当てる．
*
* void set(S l, S r, T v, vS* ls, vS* rs, vT* vs) : ならし O(log n)
*	区間 [l..r) に含まれる区間の左端[右端, 値] を ls[rs, vs] に格納する．
*	その後区間 [l..r) に値 v を割り当てる．
*
* T get(S x) : O(log n)
*	x に割り当てられた値を返す．
*/
template <class S, class T> class Interval_map {
	map<pair<S, S>, T> lr_to_v; // 区間 [l[i], r[i]) → v[i]
	T nil;

	// x が含まれる区間を指すイテレータを返す（なければ lr_to_v.end() を返す）
	typename map<pair<S, S>, T>::iterator get_iter(S x) {
		auto it = lr_to_v.lower_bound({ x, numeric_limits<S>::max() });
		if (it == lr_to_v.begin()) return lr_to_v.end();
		it--;
		if (it->first.first <= x && x < it->first.second) return it;
		else return lr_to_v.end();
	}

	// x の 1 つ右にある区間を指すイテレータを返す（なければ lr_to_v.end() を返す）
	typename map<pair<S, S>, T>::iterator get_right_iter(S x) {
		return lr_to_v.lower_bound({ x, numeric_limits<S>::max() });
	}

	// x の 1 つ左にある区間を指すイテレータを返す（なければ lr_to_v.end() を返す）
	typename map<pair<S, S>, T>::iterator get_left_iter(S x) {
		auto it = lr_to_v.lower_bound({ x, numeric_limits<S>::max() });
		if (it == lr_to_v.begin()) return lr_to_v.end();
		it--;
		if (it->first.first <= x && x < it->first.second) {
			if (it == lr_to_v.begin()) return lr_to_v.end();
			it--;
		}
		return it;
	}

public:
	// 全ての値に nil を割り当てる．
	Interval_map(T nil_) : nil(nil_) {}

	// x に割り当てられた値を返す．
	T get(S x) {
		// verify : https://codeforces.com/contest/1638/problem/E

		auto it = get_iter(x);
		return it == lr_to_v.end() ? nil : it->second;
	}

	// 区間 [l..r) に値 v を割り当てる．
	// また [l..r) に含まれていた区間の情報を ls, rs, vs に格納する．
	void set(S l, S r, T v, vector<S>* ls = nullptr, vector<S>* rs = nullptr, vector<T>* vs = nullptr) {
		// verify : https://atcoder.jp/contests/abc255/tasks/abc255_h

		// 左端 l がぶつかる区間を調べる．
		bool n_l_flag = false; S nl_l, nr_l; T nv_l;
		auto it_l = get_iter(l);
		if (it_l != lr_to_v.end()) {
			n_l_flag = true;
			nl_l = it_l->first.first;
			nr_l = it_l->first.second;
			nv_l = it_l->second;
		}
		else {
			it_l = get_right_iter(l);
		}

		// 右端 r がぶつかる区間を調べる．
		bool n_r_flag = false; S nl_r, nr_r; T nv_r;
		auto it_r = get_iter(r - 1);
		if (it_r != lr_to_v.end()) {
			n_r_flag = true;
			nl_r = it_r->first.first;
			nr_r = it_r->first.second;
			nv_r = it_r->second;
		}
		else {
			it_r = get_left_iter(r - 1);
		}

		if (ls != nullptr) {
			ls->clear(); rs->clear(); vs->clear();
		}

		if (ls != nullptr && n_l_flag) {
			if (n_r_flag && it_l == it_r) {
				ls->push_back(l); rs->push_back(r); vs->push_back(nv_l);
			}
			else {
				ls->push_back(l); rs->push_back(nr_l); vs->push_back(nv_l);
			}
		}

		if (it_l != lr_to_v.end() && it_r != lr_to_v.end()) {
			if (ls != nullptr) {
				for (auto it = it_l; it != next(it_r); it++) {
					if (n_l_flag && it == it_l) continue;
					if (n_r_flag && it == it_r) continue;

					ls->push_back(it->first.first);
					rs->push_back(it->first.second);
					vs->push_back(it->second);
				}
			}
			lr_to_v.erase(it_l, next(it_r));
		}

		if (ls != nullptr && n_r_flag) {
			if (!(n_l_flag && it_l == it_r)) {
				ls->push_back(nl_r); rs->push_back(r); vs->push_back(nv_r);
			}
		}

		lr_to_v[{l, r}] = v;
		if (n_l_flag) lr_to_v[{nl_l, l}] = nv_l;
		if (n_r_flag) lr_to_v[{r, nr_r}] = nv_r;
	}

	typename map<pair<S, S>, T>::iterator begin() { return lr_to_v.begin(); }
	typename map<pair<S, S>, T>::iterator end() { return lr_to_v.end(); }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Interval_map& d) {
		repe(p, d.lr_to_v) os << p << " ";
		return os;
	}
#endif
};


//【トライ木（集合）】
/*
* Trie_tree_set() : O(1)
*   空で初期化する．
*
* insert(string s) : O(|s|)
*   文字列 s を登録する．
*
* bool find(string s) : O(|s|)
*   文字列 s が登録されているかを返す．
*
* bool find_prefix(string s) : O(|s|)
*   文字列 s を接頭辞にもつ文字列が登録されているかを返す．
*
* int count() : O(1)
*   登録されている文字列の個数を返す．
*/
struct Trie_tree_set {
	// 参考 : https://algo-logic.info/trie-tree/
	// verify : https://codeforces.com/contest/1629/problem/D

	static const int K = 26; // 文字数

	int n;		// g のノード数
	Graph g;	// トライ木
	vc chars;	// chars[i] : 頂点 g[i] に対応する文字
	vb end;		// end[i] : g[i] で終わる文字列があるか
	vi cnt;		// cnt[i] : g[i] を含む文字列の個数

	Trie_tree_set() : n(1), g(1, vi(K, -1)), chars(1), end(1), cnt(1) {}

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

		return end[v] || prefix_flag;
	}

	int count() const { return cnt[0]; }
};


//【トライ木（写像）】
/*
* Trie_tree_set(T nil) : O(1)
*   空で初期化する．nil は今後割り当てることのない値とする．
*
* set(string s, T v) : O(|s|)
*   s に値 v を割り当てる．
*
* T get(string s) : O(|s|)
*   s に割り当てられている値を返す（無ければ nil）
*
* void get_prefix(string s, vi& len, vT& val) : O(|s|)
*	s の接頭辞である i 番目の登録済文字列の長さ[値]を len[i][ val[i] ] に格納する．
*
* int count() : O(1)
*   登録されている文字列の個数を返す．
*
* int count_prefix(string s) : O(|s|)
*   s を接頭辞にもつ文字列が何個登録されているかを返す．
*/
template <class T> struct Trie_tree_map {
	// 参考 : https://algo-logic.info/trie-tree/
	// verify : https://atcoder.jp/contests/agc047/tasks/agc047_b

	const int K = 26; // 文字数

	int n;			// g のノード数
	Graph g;		// トライ木（K 分木）
	vc chars;		// chars[i] : 頂点 g[i] に対応する文字
	vi cnt;			// cnt[i] : g[i] を含む文字列の個数
	vector<T> vals;	// vals[i] : g[i] で終わる文字列に対応する値（無ければ nil）
	T nil;

	Trie_tree_map() : n(1), g(1, vi(K, -1)), chars(1), cnt(1), vals(1), nil(numeric_limits<T>::min()) {}
	Trie_tree_map(T nil_) : n(1), g(1, vi(K, -1)), chars(1), cnt(1), vals(1), nil(nil_) {}

	void set(const string& str, T val) {
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
				cnt.push_back(0);
				vals.push_back(nil);

				// 新たなノードへのパスを追加
				g[v][c - 'a'] = n;

				// 新たなノードへ移動
				v = n++;
			}
		}

		vals[v] = val;
	}

	T get(const string& str) const {
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
				return nil;
			}
		}

		return vals[v];
	}

	// s の接頭辞である i 番目の登録済文字列の長さ[値] を len[i][ val[i] ] に格納する．
	void get_prefix(const string& str, vi& len, vector<T>& val) {
		int v = 0, l = 0;
		len.clear(); val.clear();

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			if (vals[v] != nil) {
				len.push_back(l);
				val.push_back(vals[v]);
			}

			// 登録済みの文字だった場合
			if (g[v][c - 'a'] != -1) {
				// そのノードへ移動
				v = g[v][c - 'a'];
				l++;
			}
			// 未登録の文字だった場合
			else {
				return;
			}
		}

		if (vals[v] != nil) {
			len.push_back(l);
			val.push_back(vals[v]);
		}
	}

	int count() const { return cnt[0]; }

	int count_prefix(const string& str) const {
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
				return 0;
			}
		}

		return cnt[v];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Trie_tree_map& trie) {
		string s;

		function<void(int)> dfs = [&](int v) {
			s.push_back(trie.chars[v]);
			if (trie.vals[v] != trie.nil) {
				os << s << "->" << trie.vals[v] << endl;
			}
			rep(c, trie.K) {
				if (trie.g[v][c] == -1) continue;
				dfs(trie.g[v][c]);
			}
			s.pop_back();
		};
		dfs(0);

		return os;
	}
#endif
};


//【ウェーブレット行列】
/*
* Wavelet_matrix(vl a) : O(n log n log A)
*	非負整数列 a で初期化する．（A = max(a) とおく．）
*
* ll get(int i) : O(log(max a))
*	昇順で i 番目の要素を返す．
*
* ll get(int l, int r, int i) : O(log A))
*	a[l..r) の中で昇順で i 番目の要素を返す．
*
* int count(int l, int r, ll v) : O(log A)
*	a[l..r) に v が何個あるかを返す．
*
* int count(int l, int r, ll v0, ll v1) : O(log A)
*	a[l..r) の中で [v0..v1) に値をもつ要素の個数を返す．
*
* int position(ll v, int c) : O(log n log A)
*	昇順で c 番目の v の位置を返す．
*
* frequency(int l, int r, int c, vector<pli>& freq) : O(min(r - l, A) log A)
*	a[l..r) の中で出現頻度降順に最大 c 個の要素と頻度の組のリストを freq に格納する．
*
* ll sum(int l, int r) : O(1)
*	a[l..r) の和を返す．
*
* ll sum(int l, int r, ll v0, ll v1) : O(log A)
*	a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
*
* intersection(int l1, int r1, int l2, int r2, vector<tuple<ll, int, int>>& freq) : O(min((r1 - l1) + (r2 - l2), A) log A)
*	a[l1..r1) と a[l2..r2) に共通する要素を求め，
*	その値とそれぞれにおける出現頻度の三つ組のリストを freq に格納する．
*/
class Wavelet_matrix {
	// 参考 : https://miti-7.hatenablog.com/entry/2018/04/28/152259

	int n; // 要素数
	int k; // msb 以下の桁数（1-indexed）
	vvb bs; // bs[j][i] : 第 j + 1 ビットについての安定ソート後の a[i] の第 j ビット
	vvvi bs_acc; // bs[b] : のビット b = 0, 1 それぞれの個数の累積和
	vi num_zeros; // num_zeros[j] : bs[j] の 0 の個数
	unordered_map<ll, int> id; // 値 → 安定ソートが終わったときの最左位置
	vvl acc; // acc[j] : 第 j ビットについての安定ソート後の a の累積和

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

	// a[l..r) の中で [0..v) に値をもつ要素の個数を返す．
	int count_rsub(int l, int r, ll v) {
		if (msb(v) >= k) return r - l;

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

	// a[l..r) の中で [0..v) に値をもつ要素の和を返す．
	ll sum_rsub(int l, int r, ll v) {
		if (msb(v) >= k) return acc[k][r] - acc[k][l];

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

public:
	// 非負整数列 t で初期化する．
	Wavelet_matrix(const vl& t)
		: n(sz(t)), k(msb(*max_element(all(t))) + 1),
		bs(k, vb(n)), bs_acc(2, vvi(k, vi(n + 1))), num_zeros(k), acc(k + 1, vl(n + 1))
	{
		// verify : https://judge.yosupo.jp/problem/static_range_frequency

		// ビットと組にして安定ソートするためのリスト
		vector<pair<bool, ll>> bt(n);
		rep(i, n) bt[i].second = t[i];

		// j : 注目ビット位置（上位ビットから順に見ていく）
		repir(j, k - 1, 0) {
			rep(i, n) {
				// 注目ビットが 1 か
				bs[j][i] = bt[i].first = (bt[i].second & (1LL << j));

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

	Wavelet_matrix() : n(0), k(0) {} // ダミー

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

	// 昇順で c 番目の v の位置を返す．
	int position(ll v, int c) {
		if (!id.count(v)) return -1;

		int i = id[v] + c;
		rep(j, k) {
			if (v & (1LL << j)) {
				i = ubpos(bs_acc[1][j], i - num_zeros[j]) - 1;
			}
			else {
				i = ubpos(bs_acc[0][j], i - num_zeros[j]) - 1;
			}
		}

		return i;
	}

	// a[l..r) のうち昇順で i 番目の要素を返す．
	ll get(int l, int r, int i) {
		// verify : https://judge.yosupo.jp/problem/range_kth_smallest

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

		// 出現頻度の高い値が多ければ c に応じて早めに打ち切られるが，
		// そうでなければ最悪 a[l..r) を調べ尽くしてしまう．
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

	// a[l1..r1) と a[l2..r2) に共通する要素を求め，
	// その値とそれぞれにおける出現頻度の三つ組のリストを freq に格納する．
	void intersection(int l1, int r1, int l2, int r2, vector<tuple<ll, int, int>>& freq) {
		freq.clear();

		queue<tuple<int, int, int, int, int, ll>> q;
		q.push({ k - 1, l1, r1, l2, r2, 0 });

		while (!q.empty()) {
			int j; ll v;
			tie(j, l1, r1, l2, r2, v) = q.front(); q.pop();

			if (l1 == r1 || l2 == r2) continue;

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
		// verify : https://atcoder.jp/contests/arc097/tasks/arc097_c

		chmax(v0, 0LL); chmin(v1, (1LL << k) - 1);
		if (v0 >= v1) return 0;

		return count_rsub(l, r, v1) - count_rsub(l, r, v0);
	}

	// a[l..r) の中で [v0..v1) に値をもつ要素の和を返す．
	ll sum(int l, int r, ll v0, ll v1) {
		// verify : https://yukicoder.me/problems/no/924

		chmax(v0, 0LL); chmin(v1, (1LL << k) - 1);
		if (v0 >= v1) return 0;

		return sum_rsub(l, r, v1) - sum_rsub(l, r, v0);
	}
};


//【部分文字列辞書】
/*
* Substring_dictionary(s) : O(n)
*	文字列 s[0..n) の部分文字列（空文字列は除く）で初期化する．
*
* ll size() : O(1)
*	部分文字列の個数を返す．
*
* string get(ll i) : O(|sub| + log n)
*	辞書順で i 番目の部分文字列 sub を返す．（0-indexed, なければ "" を返す）
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
		int k = (int)distance(cnt.begin(), it);

		// i から cnt[k] に足りない分だけ後ろの文字を削ったものが求める部分文字列．
		// c++ は s.substr(i, w) : s[i..i+w) なので注意．
		return s.substr(sa[k], n - (*it - i) - sa[k]);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Substring_dictionary& sd) {
		cout << sd.n << endl << sd.s << endl << sd.sa << endl << sd.la << endl
			<< sd.cnt << endl;
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
*
* 利用：【めぐる式二分探索】
*/
struct Outer_sum_dictionary {
	// verify : https://atcoder.jp/contests/abc149/tasks/abc149_e

	int n, m;
	vl a, b, acc_b;

	Outer_sum_dictionary() : n(0), m(0) {}

	// S = { a[i] + b[j] | i∈[0..n), j∈[0..m) } で初期化する．
	Outer_sum_dictionary(const vl& a_, const vl& b_)
		: n(sz(a_)), m(sz(b_)), a(a_), b(b_)
	{
		sort(all(a)); sort(all(b));

		acc_b = vl(m + 1);
		rep(j, m) {
			acc_b[j + 1] = acc_b[j] + b[j];
		}
	}

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
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
		function<bool(ll)> okQ = [&](ll v) {
			return lower_bound(v) <= i;
		};
		return meguru_search(a[0] + b[0] - 1, a[n - 1] + b[m - 1] + 1, okQ);
	}

	// S の i 番目未満の要素の和を返す．
	ll sum(ll i) {
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
*
* 利用：【めぐる式二分探索】
*/
struct Outer_mul_dictionary {
	// verify : https://atcoder.jp/contests/arc037/tasks/arc037_c

	// n, m : a, b の要素数
	// np, mp : a, b の 正の要素数
	// nz, mz : a, b の 0 の要素数
	// nn, mn : a, b の 負の要素数
	int n, np, nz, nn, m, mp, mz, mn;

	// ap, bp : a, b の正の要素を昇順に格納したリスト
	// an, bn : a, b の負の要素の 絶対値 を昇順に格納したリスト
	vl ap, an, bp, bn;

	Outer_mul_dictionary() : n(0), np(0), nz(0), nn(0), m(0), mp(0), mz(0), mn(0) {}

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

	// S の v 未満の要素の個数を返す．
	ll lower_bound(ll v) {
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
		function<bool(ll)> okQ = [&](ll v) {
			return lower_bound(v) <= i;
		};
		return meguru_search(-INFL, INFL, okQ);
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
template <class S> struct KDTree {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_C

	using vS = vector<S>; using vvS = vector<vS>; using vvSi = vector<pair<vS, int>>;
	const S S_MIN = numeric_limits<S>::min(), S_MAX = numeric_limits<S>::max();

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
template <class S, class T> struct KDTrie {
	// verify : https://atcoder.jp/contests/abc075/tasks/abc075_d

	using vS = vector<S>; using vvS = vector<vS>; using vT = vector<T>;
	const S S_MIN = numeric_limits<S>::min(), S_MAX = numeric_limits<S>::max();

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


//【多重集合の動的辞書】
/*
* Dynamic_dictionary(int n) : O(n)
*	[0..n) を記録可能な辞書を空で初期化する．
*
* Dynamic_dictionary(int n, vi a) : O(n)
*	[0..n) を記録可能な辞書を多重集合 a で初期化する．
*
* ll size() : O(log n)
*	要素の総数を返す．
*
* ll count(int v) : O(log n)
*	要素 v の個数を返す．
*
* ll count(int l, int r) : O(log n)
*	値 [l..r) をもつ要素の個数を返す．
*
* insert(int v), insert(int v, ll k) : O(log n)
*	要素 v を 1 個 [k 個] 追加する．
*
* erase(int v), erase(int v, ll k) : O(log n)
*	要素 v を 1 個 [k 個] 削除する．個数は負数にもなる．
*
* int get(ll i) : O(log n)
*	昇順で i 番目（0-indexed）の要素を返す．なければ n を返す．
*
* ll lower_bound(int v) : O(log n)
*	v 以上の最小の要素が昇順で何番目の要素かを返す．（0-indexed）
*
* 利用：【フェニック木（アーベル群）】
*/
ll opdd(ll x, ll y) { return x + y; }
ll edd() { return 0; }
ll invdd(ll x) { return -x; }
struct Dynamic_dictionary {
	// verify : https://judge.yosupo.jp/problem/predecessor_problem
	// verify : https://atcoder.jp/contests/abc061/tasks/abc061_c

	int n;

	// ft[v] : 要素 v の個数
	using RSQ = Fenwick_tree<ll, opdd, edd, invdd>;
	RSQ ft;

	// コンストラクタ（何もしない）
	Dynamic_dictionary() : n(0) {}

	// [0..n) を記録可能な辞書を空で初期化する．
	Dynamic_dictionary(int n_) : n(n_), ft(n) {}

	// [0..n) を記録可能な辞書を多重集合 a で初期化する．
	Dynamic_dictionary(int n_, const vi& a) : n(n_) {
		vl cnt(n);
		repe(v, a) cnt[v]++;
		ft = RSQ(cnt);
	}

	// 要素の総数を返す．
	ll size() { return ft.prod(0, n); }

	// 要素 v の個数を返す．
	ll count(int v) { return ft.get(v); }

	// 値 [l..r) をもつ要素の個数を返す．
	ll count(int l, int r) { return ft.prod(l, r); }

	// 要素 v を挿入する．
	void insert(int v) { ft.apply(v, 1); }
	void insert(int v, ll k) { ft.apply(v, k); }

	// 要素 v を削除する．
	void erase(int v) { ft.apply(v, -1); }
	void erase(int v, ll k) { ft.apply(v, -k); }

	// 昇順で i 番目の要素を返す．
	int get(ll i) {
		auto f = [&](ll x) { return x <= i; };
		return ft.max_right(f);
	}

	// v が昇順で何番目の要素かを返す．
	ll lower_bound(int v) { return ft.prod(0, v); }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Dynamic_dictionary& dd) {
		rep(v, dd.n) rep(hoge, dd.ft.get(v)) os << v << " ";
		return os;
	}
#endif
};


