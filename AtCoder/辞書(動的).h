#pragma once
#include "header.h"
#include "探索.h"
// ■■■■■ 動的辞書 ■■■■■


//【binary trie】
/*
* Binary_trie<T = ll>(int B = 63) : O(1)
*   型 T の B ビット非負整数を扱えるよう空で初期化する．
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
*	戻り値には mask との XOR はかかっていないので注意！！！
*
* T min_element(T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，最小要素を返す．
*
* T get(ll i, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，昇順で i 番目（0-indexed）の要素を返す．
*
* ll lower_bound(T val, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，
*   val 以上の最小の要素が昇順で何番目（0-indexed）の要素かを返す．
*
* ll upper_bound(T val, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，
*   val より大きい最小の要素が昇順で何番目（0-indexed）の要素かを返す．
*
* ll count(T val) : O(B)
*   要素 val の個数を返す．
*
* ll count(T l, T r, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，値 [l..r) をもつ要素の個数を返す．
*/
template <class T = ll>
class Binary_trie {
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
		Assert(t != nullptr && t->cnt >= cnt);

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
		Assert(t != nullptr);

		// 葉なら 0 を返す．
		if (b < 0) return 0;

		// 下位ビットに対応するノードの最小値を求めにいく．
		int g = (int)((mask >> b) & T(1));
		if (t->ch[g] == nullptr) g ^= 1;
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

	void free_sub(Node* t) {
		if (t == nullptr) return;

		free_sub(t->ch[0]);
		free_sub(t->ch[1]);

		delete t;
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

	// メモリを開放する．： O(n B)
	~Binary_trie() {
//		free_sub(root);
	}

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
		// verify(cnt=1) : https://judge.yosupo.jp/problem/set_xor_min

		root = insert_sub(root, val, cnt, B - 1);
	}

	// 値 val を cnt[=1] 個削除する． : O(B)
	void erase(T val, ll cnt = 1) {
		// verify(cnt=1) : https://judge.yosupo.jp/problem/set_xor_min

		root = erase_sub(root, val, cnt, B - 1);
	}

	// mask[=0] との XOR をとったときの最大要素を返す． : O(B)
	T max_element(T mask = 0) const {
		// verify(mask=0) : https://judge.yosupo.jp/problem/double_ended_priority_queue

		return min_element_sub(root, ~mask, B - 1);
	}

	// mask[=0] との XOR をとったときの最小要素を返す． : O(B)
	T min_element(T mask = 0) const {
		// verify : https://judge.yosupo.jp/problem/set_xor_min

		return min_element_sub(root, mask, B - 1);
	}

	// mask[=0] との XOR をとったときの昇順で i 番目（0-indexed）の要素を返す． : O(B)
	T get(ll i, T mask = 0) const {
		// verify : https://atcoder.jp/contests/arc147/tasks/arc147_e

		Assert(0 <= i && i < size());
		return get_sub(root, mask, i, B - 1);
	}

	// mask[=0] との XOR をとったときの val 以上の最小の要素が昇順で何番目の要素かを返す．（0-indexed） : O(B)
	ll lower_bound(T val, T mask = 0) const {
		// verify : https://www.spoj.com/problems/SUBXOR/

		if (val <= 0) return 0LL;

		return lower_bound_sub(root, mask, val, B - 1);
	}

	// mask[=0] との XOR をとったときの val より大きい最小の要素が昇順で何番目の要素かを返す．（0-indexed） : O(B)
	ll upper_bound(T val, T mask = 0) const {
		// verify : https://codeforces.com/contest/966/problem/C

		if (val < 0) return 0LL;

		// val + 1 が B ビット整数に収まらない場合の例外処理
		if (val == (T(1) << B) - 1) return size();

		return lower_bound_sub(root, mask, val + 1, B - 1);
	}

	// 要素 val の個数を返す． : O(B)
	ll count(T val) const {
		// verify : https://judge.yosupo.jp/problem/set_xor_min

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
		// verify : https://atcoder.jp/contests/code-festival-2015-qualb/tasks/codefestival_2015_qualB_d

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
template <class S, class T>
class Interval_map {
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
	
	static const int K = 26; // 文字数

	int n;		// g のノード数
	Graph g;	// トライ木
	vc chars;	// chars[i] : 頂点 g[i] に対応する文字
	vb end;		// end[i] : g[i] で終わる文字列があるか
	vi cnt;		// cnt[i] : g[i] を含む文字列の個数

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

		cnt[v]++;
		end[v] = true;
	}

	bool find(const string& str) const {
		// verify : https://codeforces.com/contest/1629/problem/D

		return find_sub(str, false);
	}

	bool find_prefix(const string& str) const {
		// verify : https://codeforces.com/contest/1629/problem/D

		return find_sub(str, true);
	}

	int count() const { return cnt[0]; }
};


//【トライ木（写像）】
/*
* Trie_tree_set(T nil = lowest()) : O(1)
*   空で初期化する．nil は T の値域に属さない値とする．
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
template <class T>
class Trie_tree_map {
	// 参考 : https://algo-logic.info/trie-tree/

	const int K = 26; // 文字数
	const char A = 'a'; // 開始文字

	int n;			// g のノード数
	Graph g;		// トライ木（K 分木）
	vc chars;		// chars[i] : 頂点 g[i] に対応する文字
	vi cnt;			// cnt[i] : g[i] を含む文字列の個数
	vector<T> vals;	// vals[i] : g[i] で終わる文字列に対応する値（無ければ nil）
	T nil;

public:
	Trie_tree_map() : n(1), g(1, vi(K, -1)), chars(1), cnt(1), vals(1), nil(numeric_limits<T>::lowest()) {}
	Trie_tree_map(T nil_) : n(1), g(1, vi(K, -1)), chars(1), cnt(1), vals(1), nil(nil_) {}

	void set(const string& str, T val) {
		// verify : https://judge.yosupo.jp/problem/associative_array

		int v = 0;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// str は頂点 v を含む文字列なので個数に加算する．
			cnt[v]++;

			// 登録済みの文字だった場合
			if (g[v][c - A] != -1) {
				// そのノードへ移動
				v = g[v][c - A];
			}
			// 未登録の文字だった場合
			else {
				// 新たにノード n を追加
				g.push_back(vi(K, -1));
				chars.push_back(c);
				cnt.push_back(0);
				vals.push_back(nil);

				// 新たなノードへのパスを追加
				g[v][c - A] = n;

				// 新たなノードへ移動
				v = n++;
			}
		}

		vals[v] = val;
	}

	T get(const string& str) const {
		// verify : https://judge.yosupo.jp/problem/associative_array

		int v = 0;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// 登録済みの文字だった場合
			if (g[v][c - A] != -1) {
				// そのノードへ移動
				v = g[v][c - A];
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
		// verify : https://atcoder.jp/contests/agc047/tasks/agc047_b

		int v = 0, l = 0;
		len.clear(); val.clear();

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			if (vals[v] != nil) {
				len.push_back(l);
				val.push_back(vals[v]);
			}

			// 登録済みの文字だった場合
			if (g[v][c - A] != -1) {
				// そのノードへ移動
				v = g[v][c - A];
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
			if (g[v][c - A] != -1) {
				// そのノードへ移動
				v = g[v][c - A];
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


