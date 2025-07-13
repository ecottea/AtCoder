#pragma once
#include "header.h"
#include "探索.h"
#include "フェニック木.h"
// ■■■■■ 動的辞書 ■■■■■


//【binary trie】
/*
* Binary_trie<T = ll>(int B = 62) : O(1)
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
*	戻り値には mask との XOR はかかっていないので注意！
*
* T min_element(T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，最小要素を返す．
*
* T get(ll i, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，昇順で i 番目（0-indexed）の要素を返す（なければ -1）
*
* ll lower_bound(T val, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，
*   val 以上の最小の要素が昇順で何番目（0-indexed）の要素かを返す（なければ要素数）
*
* ll upper_bound(T val, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，
*   val より大きい最小の要素が昇順で何番目（0-indexed）の要素かを返す（なければ要素数）
*
* ll count(T val) : O(B)
*   要素 val の個数を返す．
*
* ll count(T l, T r, T mask = 0) : O(B)
*   全要素に対して mask と XOR をとったと仮定し，値 [l..r) をもつ要素の個数を返す．
*
* vector<pTl> get_all() : O(n)
*	全要素の (値, 個数) の組のリストを返す．
*/
template <class T = ll>
class Binary_trie {
	// 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/06/022654

	struct Node {
		ll cnt; // 部分木のもつ要素の個数
		Node* ch[2]; // 左右の子へのポインタ

		//// 参考 : https://qiita.com/tubo28/items/f058582e457f6870a800
		//static inline int node_count = 0;
		//// 静的に確保した配列から返す
		//void* operator new(std::size_t) {
		//	constexpr int MAX_N = (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536 + 131072 + 262144 + 500000 * 12);
		//	static Node pool[MAX_N];
		//	return pool + node_count++;
		//}

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

	void get_all_sub(Node* t, T val, int b, vector<pair<T, ll>>& res) const {
		if (t == nullptr) return;

		if (b < 0) {
			res.emplace_back(val, t->cnt);
			return;
		}

		get_all_sub(t->ch[0], val << 1, b - 1, res);
		get_all_sub(t->ch[1], (val << 1) | T(1), b - 1, res);
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
	Binary_trie(int B_ = 62) : root(nullptr), B(B_) {
		// verify : https://judge.yosupo.jp/problem/ordered_set
	}

	// メモリを開放する．： O(n B)
	~Binary_trie() {
		// verify : https://yukicoder.me/problems/no/2977
		//free_sub(root);
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
		// verify : https://judge.yosupo.jp/problem/set_xor_min

		root = insert_sub(root, val, cnt, B - 1);
	}

	// 値 val を cnt[=1] 個削除する． : O(B)
	void erase(T val, ll cnt = 1) {
		// verify : https://judge.yosupo.jp/problem/set_xor_min

		root = erase_sub(root, val, cnt, B - 1);
	}

	// mask[=0] との XOR をとったときの最大要素を返す． : O(B)
	T max_element(T mask = 0) const {
		// verify : https://judge.yosupo.jp/problem/double_ended_priority_queue

		return min_element_sub(root, ~mask, B - 1);
	}

	// mask[=0] との XOR をとったときの最小要素を返す． : O(B)
	T min_element(T mask = 0) const {
		// verify : https://judge.yosupo.jp/problem/set_xor_min

		return min_element_sub(root, mask, B - 1);
	}

	// mask[=0] との XOR をとったときの昇順で i 番目（0-indexed）の要素を返す（なければ -1）
	T get(ll i, T mask = 0) const {
		// verify : https://judge.yosupo.jp/problem/ordered_set

		if (i < 0 || size() <= i) return T(-1);

		return get_sub(root, mask, i, B - 1);
	}

	// mask[=0] との XOR をとったときの val 以上の最小の要素が昇順で何番目の要素かを返す．（0-indexed） : O(B)
	ll lower_bound(T val, T mask = 0) const {
		// verify : https://judge.yosupo.jp/problem/ordered_set

		if (val <= 0) return 0LL;
		if (val >= (T(1) << B)) return size();

		return lower_bound_sub(root, mask, val, B - 1);
	}

	// mask[=0] との XOR をとったときの val より大きい最小の要素が昇順で何番目の要素かを返す．（0-indexed） : O(B)
	ll upper_bound(T val, T mask = 0) const {
		// verify : https://judge.yosupo.jp/problem/ordered_set

		return lower_bound_sub(root, mask, val + 1, B - 1);
	}

	// 要素 val の個数を返す． : O(B)
	ll count(T val) const {
		// verify : https://judge.yosupo.jp/problem/associative_array

		return upper_bound(val) - lower_bound(val);
	}

	// mask[=0] との XOR をとったときの [l..r) に値をもつ要素の個数を返す． : O(B)
	ll count(T l, T r, T mask = 0) const {
		// verify : https://judge.yosupo.jp/problem/ordered_set

		return lower_bound(r, mask) - lower_bound(l, mask);
	}

	// 全要素の (値, 個数) の組のリストを返す．
	vector<pair<T, ll>> get_all() const {
		// verify : https://atcoder.jp/contests/joig2024-open/tasks/joig2024_f

		vector<pair<T, ll>> res;
		get_all_sub(root, T(0), B - 1, res);
		return res;
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
* Interval_set<T>(T L = -INFL, T R = INFL) : O(1)
*	定義域を [L..R) とし空で初期化する．
*
* int size() : O(1)
*	区間の数を返す．
*
* pTT get(T x) : O(log n)
*	x が含まれる区間 [l..r) を返す（なければ {-1, -1} を返す）
*
* pTT get_right(T x) : O(log n)
*	x < l なる最左区間 [l..r) を返す（なければ {R+1, R+1} を返す）
*
* pTT get_left(T x) : O(log n)
*	r ≦ x なる最右区間 [l..r) を返す（なければ {L-1, L-1} を返す）
*
* insert(T l, T r) : ならし O(log n)
*	区間 [l..r) を追加する．区間は自動的に結合される．
*
* erase(T l, T r) : ならし O(log n)
*	区間 [l..r) を削除する．空の区間は自動的に削除される．
*
* vector<pTT> get_all_intervals() : O(n)
*	全ての区間 [l..r) からなるリストを返す．
*/
template <class T>
class Interval_set {
	// L, R : 定義域が [L..R) であることを表す．
	T L, R;

	// l_to_r[l] : l を左端にもつ半開区間 [l..r) の右端 r
	map<T, T> l_to_r;

public:
	// 定義域を [L..R) とし空で初期化する．
	Interval_set(T L = -(T)INFL, T R = (T)INFL) : L(L), R(R) {
		// verify : https://yukicoder.me/problems/no/2292

		l_to_r[L - 1] = L - 1;
		l_to_r[R + 1] = R + 1; // 番兵
	}

	// 区間の数を返す．
	int size() const {
		// verify : https://atcoder.jp/contests/abc411/tasks/abc411_c

		return sz(l_to_r) - 2;
	}

	// x が含まれる区間 [l..r) を返す（なければ {-1, -1} を返す）
	pair<T, T> get(T x) const {
		// verify : https://yukicoder.me/problems/no/2292

		Assert(L <= x && x < R);

		auto it = prev(l_to_r.upper_bound(x));

		return it->second <= x ? make_pair(T(-1), T(-1)) : pair<T, T>(*it);
	}

	// x < l なる最左区間 [l..r) を返す（なければ {R+1, R+1} を返す）
	pair<T, T> get_right(T x) const {
		// verify : https://atcoder.jp/contests/code-festival-2015-qualb/tasks/codefestival_2015_qualB_d

		Assert(L <= x && x < R);

		auto it = l_to_r.upper_bound(x);

		return *it;
	}

	// r ≦ x なる最右区間 [l..r) を返す（なければ {L-1, L-1} を返す）
	pair<T, T> get_left(T x) const {
		Assert(L <= x && x < R);

		auto it = prev(l_to_r.lower_bound(x));

		return it->second <= x ? *it : *prev(it);
	}

	// 区間 [l..r) を追加する．
	void insert(T l, T r) {
		// verify : https://yukicoder.me/problems/no/2292

		chmax(l, L); chmin(r, R);
		if (l >= r) return;

		auto it = l_to_r.lower_bound(l);

		// [l..r) の左側と繋がる区間がある場合
		auto pit = prev(it);
		if (l <= pit->second) {
			//if (l < pit->second) { // 隣り合う区間を結合したくない場合はこっち
			l = pit->first;
			it = pit;
		}

		while (true) {
			if (r < it->first) break;
			//if (r <= it->first) break; // 隣り合う区間を結合したくない場合はこっち

			// [l..r) の右側と繋がる区間がある場合
			if (r <= it->second) {
				r = it->second;
				l_to_r.erase(it);
				break;
			}

			it = l_to_r.erase(it);
		}

		l_to_r[l] = r;
	}

	// 区間 [l..r) を削除する．
	void erase(T l, T r) {
		// verify : https://yukicoder.me/problems/no/2292

		chmax(l, L); chmin(r, R);
		if (l >= r) return;

		auto it = l_to_r.lower_bound(l);

		// [l..r) の左側で削られる区間がある場合
		auto pit = prev(it);
		if (l < pit->second) {
			// [l..r) を真に含む区間がある場合
			if (r < pit->second) l_to_r[r] = pit->second;

			pit->second = l;
		}

		while (true) {
			if (r <= it->first) break;

			// [l..r) の右側で削られる区間がある場合
			if (r < it->second) {
				T nr = it->second;
				l_to_r.erase(it);
				l_to_r[r] = nr;
				break;
			}

			it = l_to_r.erase(it);
		}
	}

	// 全ての区間 [l..r) からなるリストを返す．
	vector<pair<T, T>> get_all_intervals() const {
		// verify : https://atcoder.jp/contests/abc254/tasks/abc254_g
		
		vector<pair<T, T>> res;
		res.reserve(sz(l_to_r) - 2);
		repe(lr, l_to_r) {
			if (lr.first == L - 1 || lr.first == R + 1) continue;
			res.push_back(lr);
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Interval_set& IS) {
		repe(p, IS.l_to_r) {
			if (p.first == IS.L - 1 || p.first == IS.R + 1) continue;
			os << "[" << p.first << "," << p.second << ") ";
		}
		return os;
	}
#endif
};


//【区間からの写像】
/*
* Interval_map<S, T>(S L, S R, T v0) : O(1)
*	定義域を [L..R) とし，a[L..R) = v0 で初期化する．
*
* void set_interval(S l, S r, T v) : ならし O(log n)
*	a[l..r) = v とする．
*
* T get(S i) : O(log n)
*	a[i] を返す．
*
* tuple<S, S, T> get_interval(S i) : O(log n)
*	i を含む値の等しい極大区間が a[l..r)=v だとして，3 つ組 {l, r, v} を返す．
*
* vector<tuple<S, S, T>> get_all(S l, S r) : O(n)
*	a[l..r) を連長圧縮した結果を {左端, 右端, 値} のリストとして返す．
*	備考：これを呼んだ直後に set_interval(l, r, v) を呼ぶなら計算量はならし O(log n)
*/
template <class S, class T>
struct Interval_map {
	// L, R : 定義域が [L..R) であることを表す．
	S L, R;

	// l_to_v[l] : 区間 [l..r) に割り当てられた値（r は次の区間の l）
	map<S, T> l_to_v;

public:
	// 定義域を [L..R) とし，a[L..R) = v0 で初期化する．
	Interval_map(S L, S R, T v0) : L(L), R(R) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_ac

		// 番兵として絶対使わない値を置いておく
		l_to_v[L - 1] = T(INFL) + 1;
		l_to_v[L] = v0;
		l_to_v[R] = T(INFL) + 1;
	}
	Interval_map() : L(0), R(0) {}

	// a[l..r) = v とする．
	void set_interval(S l, S r, T v) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_ac

		chmax(l, L); chmin(r, R);
		if (l >= r) return;

		auto it = l_to_v.upper_bound(l);
		auto pit = prev(it);
		T vR = pit->second;

		if (pit->first == l) {
			pit = l_to_v.erase(pit);
			pit = prev(pit);
		}
		T vL = pit->second;

		// 丸ごと上書きされる区間は削除する．
		while (it != l_to_v.end()) {
			if (it->first > r) break;

			vR = it->second;
			it = l_to_v.erase(it);
		}

		if (v != vR) l_to_v[r] = vR;
		if (v != vL) l_to_v[l] = v;
	}

	// a[i] を返す．
	T get(S i) {
		// verify : https://codeforces.com/contest/1638/problem/E

		Assert(L <= i); Assert(i < R);
		return prev(l_to_v.upper_bound(i))->second;
	}

	// i を含む値の等しい極大区間が a[l..r)=v だとして，3 つ組 {l, r, v} を返す．
	tuple<S, S, T> get_interval(S i) {
		// verify : https://atcoder.jp/contests/abc380/tasks/abc380_e

		Assert(L <= i); Assert(i < R);

		auto it = l_to_v.upper_bound(i);
		auto pit = prev(it);
		return { pit->first, it->first, pit->second };
	}

	// a[l..r) を連長圧縮した結果を {左端, 右端, 値} のリストとして返す．
	vector<tuple<S, S, T>> get_all(S l, S r) {
		// verify : https://atcoder.jp/contests/typical90/tasks/typical90_ac

		chmax(l, L); chmin(r, R);
		if (l >= r) return vector<tuple<S, S, T>>();

		vector<tuple<S, S, T>> res;
		auto nit = l_to_v.upper_bound(l), it = prev(nit);
		S i = it->first;
		while (i < r) {
			S ni = nit->first;
			res.emplace_back(max(i, l), min(ni, r), it->second);
			i = ni;
			it = nit++;
		}

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Interval_map& IM) {
		auto it = next(IM.l_to_v.begin());
		while (it->first < IM.R) {
			os << "[" << it->first << "," << next(it)->first << "):" << it->second << " ";
			it++;
		}
		return os;
	}
#endif
};


//【トライ木（集合）】
/*
* Trie_tree_set() : O(1)
*   空で初期化する．
*
* int size() : O(1)
*   登録されている文字列の個数を返す．
*
* insert(string s) : O(|s|)
*   文字列 s を登録する．
*
* bool find(string s) : O(|s|)
*   文字列 s が登録されているかを返す．
*
* bool find_prefix(string s) : O(|s|)
*   文字列 s を接頭辞にもつ文字列が登録されているかを返す．
*/
struct Trie_tree_set {
	// 参考 : https://algo-logic.info/trie-tree/

	static constexpr int K = 26; // 文字数
	static constexpr char A = 'a'; // 最初の文字

	struct Node {
		char c; // 頂点に対応する文字
		bool end; // この頂点で終わる文字列があるか
		int cnt; // この頂点で終わる文字列を接頭辞にもつ文字列が何個あるか
		array<Node*, K> childs; // 子へのポインタ（K 分木）

		Node(char c, bool end, int cnt) : c(c), end(end), cnt(cnt), childs({ nullptr }) {}
	};

	Node* root; // 根へのポインタ

	bool find_sub(const string& str, bool prefix_flag) const {
		Node* p = root;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// 登録済みの文字だった場合
			if (p->childs[c - A]) {
				// そのノードへ移動
				p = p->childs[c - A];
			}
			// 未登録の文字だった場合
			else {
				return false;
			}
		}

		return p->end || prefix_flag;
	}

	// 空で初期化する．
	Trie_tree_set() {
		// verify : https://codeforces.com/contest/1629/problem/D

		root = new Node('^', false, 0);
	}

	// 登録されている文字列の個数を返す．
	int size() const {
		return root->cnt;
	}

	// 文字列 s を登録する．
	void insert(const string& str) {
		// verify : https://codeforces.com/contest/1629/problem/D

		Node* p = root;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// str はノード p を含む文字列なので個数に加算する．
			p->cnt++;

			// 未登録の文字だった場合は新たなノードを追加する．
			if (!p->childs[c - A]) p->childs[c - A] = new Node(c, false, 0);

			// 次のノードへ移動
			p = p->childs[c - A];
		}

		p->cnt++;
		p->end = true;
	}

	// 文字列 s が登録されているかを返す．
	bool find(const string& str) const {
		// verify : https://codeforces.com/contest/1629/problem/D

		return find_sub(str, false);
	}

	// 文字列 s を接頭辞にもつ文字列が登録されているかを返す．
	bool find_prefix(const string& str) const {
		// verify : https://codeforces.com/contest/1629/problem/D

		return find_sub(str, true);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Trie_tree_set& Trie) {
		string s;

		function<void(Node*)> dfs = [&](Node* p) {
			s.push_back(p->c);
			if (p->end) {
				os << s << endl;
			}
			rep(k, K) {
				if (!p->childs[k]) continue;
				dfs(p->childs[k]);
			}
			s.pop_back();
		};
		dfs(Trie.root);

		return os;
	}
#endif
};


//【トライ木（写像）】
/*
* Trie_tree_map<T>(T nil = lowest()) : O(26)
*   空で初期化する．
*
* int size() : O(1)
*   登録されている文字列の個数を返す．
*
* set(string s, T x) : O(|s| 26)
*   文字列 s に値 x を割り当てる．
*
* T get(string s) : O(|s|)
*   文字列 s に割り当てられている値を返す（無ければ nil）
*
* vpiT get_all(string s) : O(|s|)
*	文字列 s の接頭辞である登録済文字列の {長さ, 値} の組のリストを返す．
*
* int count_prefix(string s) : O(|s|)
*   文字列 s を接頭辞にもつ文字列が何個登録されているかを返す．
*
* vpiT erase_all(string s) : ならし O(|s| 26)
*	文字列 s を接頭辞にもつ登録済文字列を全削除し，{長さ, 値} の組のリストを返す．
*/
template <class T>
class Trie_tree_map {
	// 参考 : https://algo-logic.info/trie-tree/

	static constexpr int K = 26;   // 文字数
	static constexpr char A = 'a'; // 最初の文字

	struct Node {
		char c; // 頂点に対応する文字
		T val; // この頂点で終わる文字列に対応する値
		int cnt; // この頂点で終わる文字列を接頭辞にもつ文字列が何個あるか
		array<Node*, K> childs; // 子へのポインタ（K 分木）

		Node(char c, T val, int cnt) : c(c), val(val), cnt(cnt), childs({ nullptr }) {}
	};

	Node* root; // 根へのポインタ
	T nil; // 無効値

	Node* get_node(const string& str) const {
		Node* p = root;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// 登録済みの文字だった場合
			if (p->childs[c - A]) {
				// そのノードへ移動
				p = p->childs[c - A];
			}
			// 未登録の文字だった場合
			else {
				return nullptr;
			}
		}

		return p;
	}

public:
	// 空で初期化する．
	Trie_tree_map(T nil = numeric_limits<T>::lowest()) : nil(nil) {
		// verify : https://atcoder.jp/contests/abc403/tasks/abc403_e

		root = new Node('^', nil, 0);
	}

	// 登録されている文字列の個数を返す．
	int size() const {
		return root->cnt;
	}

	// 文字列 s に値 x を割り当てる．
	void set(const string& str, T x) {
		// verify : https://atcoder.jp/contests/abc403/tasks/abc403_e

		Node* p = root;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// str はノード p を含む文字列なので個数に加算する．
			p->cnt++;

			// 未登録の文字だった場合は新たなノードを追加する．
			if (!p->childs[c - A]) p->childs[c - A] = new Node(c, nil, 0);

			// 次のノードへ移動
			p = p->childs[c - A];
		}

		p->val = x;
		p->cnt++;
	}

	// 文字列 s に割り当てられている値を返す（無ければ nil）
	T get(const string& str) const {
		// verify : https://atcoder.jp/contests/abc403/tasks/abc403_e

		Node* p = get_node(str);

		return p ? p->val : nil;
	}

	// 文字列 s の接頭辞である登録済文字列の {長さ, 値} の組のリストを返す．
	vector<pair<int, T>> get_all(const string& str) const {
		// verify : https://atcoder.jp/contests/abc403/tasks/abc403_e

		vector<pair<int, T>> res;

		int l = 0;

		Node* p = root;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			if (p->val != nil) res.emplace_back(l, p->val);

			// 登録済みの文字だった場合
			if (p->childs[c - A]) {
				// そのノードへ移動
				p = p->childs[c - A];
				l++;
			}
			// 未登録の文字だった場合
			else {
				return res;
			}
		}

		if (p->val != nil) res.emplace_back(l, p->val);

		return res;
	}


	// 文字列 s を接頭辞にもつ文字列が何個登録されているかを返す．
	int count_prefix(const string& str) const {
		Node* p = get_node(str);

		return p ? p->cnt : 0;
	}

	// 文字列 s を接頭辞にもつ登録済文字列を全削除し，{長さ, 値} の組のリストを返す．
	vector<pair<int, T>> erase_all(const string& str) {
		// verify : https://atcoder.jp/contests/abc403/tasks/abc403_e

		vector<pair<int, T>> res;

		Node* p = get_node(str);
		if (!p) return res;

		int l = sz(str);

		// p の先にある登録済文字列を記録しつつ削除する．
		function<void(Node*)> dfs = [&](Node* p) {
			if (p->val != nil) {
				res.emplace_back(l, p->val);
			}

			l++;
			rep(k, K) {
				if (!p->childs[k]) continue;

				dfs(p->childs[k]);
			}
			l--;

			delete p;
		};
		dfs(p);

		p = root;
		int el_cnt = sz(res);

		// str の文字 c を先頭から順に見ていく
		rep(i, l - 1) {
			p->cnt -= el_cnt;
			p = p->childs[str[i] - A];
		}
		p->childs[str[l - 1] - A] = nullptr;

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Trie_tree_map& Trie) {
		string s;

		function<void(Node*)> dfs = [&](Node* p) {
			s.push_back(p->c);
			if (p->val != Trie.nil) {
				os << s << " : " << p->val << endl;
			}
			rep(k, K) {
				if (!p->childs[k]) continue;
				dfs(p->childs[k]);
			}
			s.pop_back();
		};
		dfs(Trie.root);

		return os;
	}
#endif
};


//【index 付き多重集合】
/*
* Multi_set<T>(int n) : O(n)
*	[0..n) を記録可能な辞書を空で初期化する．
*
* Multi_set<T>(int n, vi a) : O(n + |a|)
*	[0..n) を記録可能な辞書を多重集合 a で初期化する．
*
* T size() : O(log n)
*	要素の総数を返す．
*
* T count(int v) : O(log n)
*	要素 v の個数を返す．
*
* T count(int l, int r) : O(log n)
*	値 [l..r) をもつ要素の個数を返す．
*
* insert(int v, T k = 1) : O(log n)
*	要素 v を k 個挿入する．
*
* erase(int v, T k = 1) : O(log n)
*	要素 v を k 個削除する．個数は負数にもなる．
*
* int get(T i) : O(log n)
*	昇順で i 番目（0-indexed）の要素を返す．なければ n を返す．
*
* T lower_bound(int v) : O(log n)
*	v 以上の最小の要素が昇順で何番目（0-indexed）の要素かを返す．
*
* 利用：【フェニック木（アーベル群）】
*/
template <class T> T opdd(T x, T y) { return x + y; }
template <class T> T edd() { return 0; }
template <class T> T invdd(T x) { return -x; }
template <class T>
struct Multi_set {
	int n;

	// ft[v] : 要素 v の個数
	using RSQ = Fenwick_tree<T, opdd<T>, edd<T>, invdd<T>>;
	RSQ ft;

	// [0..n) を記録可能な辞書を空で初期化する．
	Multi_set(int n) : n(n), ft(n) {
		// verify : https://judge.yosupo.jp/problem/range_kth_smallest
	}

	// [0..n) を記録可能な辞書を多重集合 a で初期化する．
	Multi_set(int n, const vi& a) : n(n) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem

		vector<T> cnt(n);
		repe(v, a) cnt[v]++;
		ft = RSQ(cnt);
	}
	Multi_set() : n(0) {}

	// 要素の総数を返す．
	T size() { return ft.sum(0, n); }

	// 要素 v の個数を返す．
	T count(int v) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem

		return ft.get(v);
	}

	// 値 [l..r) をもつ要素の個数を返す．
	T count(int l, int r) { return ft.sum(l, r); }

	// 要素 v を k 個挿入する．
	void insert(int v, T k = 1) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem

		ft.add(v, k);
	}

	// 要素 v を k 個削除する．
	void erase(int v, T k = 1) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem

		ft.add(v, -k);
	}

	// 昇順で i 番目の要素を返す．
	int get(T i) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem

		auto f = [&](T x) { return x <= i; };
		return ft.max_right(f);
	}

	// v が昇順で何番目の要素かを返す．
	T lower_bound(int v) {
		// verify : https://judge.yosupo.jp/problem/predecessor_problem

		return ft.sum(0, v);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Multi_set& dd) {
		rep(v, dd.n) rep(hoge, dd.ft.get(v)) os << v << " ";
		return os;
	}
#endif
};


//【区間の集合（旧）】
/*
* Interval_set(bool marge = true) : O(1)
*	空で初期化する．ちょうど接する区間を併合するかを marge に渡す．
*
* int size() : O(1)
*	区間の数を返す．
*
* pll get(ll x) : O(log n)
*	x が含まれる区間 [l..r) を返す（なければ {-1, -1} を返す）
*
* pll get_right(ll x) : O(log n)
*	x の 1 つ右にある区間 [l..r) を返す（なければ {-1, -1} を返す）
*
* pll get_left(ll x) : O(log n)
*	x の 1 つ左にある区間 [l..r) を返す（なければ {-1, -1} を返す）
*
* insert(ll l, ll r) : ならし O(log n)
*	区間 [l..r) を追加する．
*
* erase(ll l, ll r) : ならし O(log n)
*	区間 [l..r) を削除する．
*
* vector<pll> get_all_intervals() : O(n)
*	全ての区間 [l..r) からなるリストを返す．
*/
class Interval_set_old {
	bool marge;

	// x が含まれる区間を指すイテレータを返す（なければ lr.end() を返す）
	typename set<pll>::iterator get_iter(ll x) const {
		auto it = lrs.lower_bound({ x, INFL });
		if (it == lrs.begin()) return lrs.end();
		it--;
		if (it->first <= x && x < it->second) return it;
		else return lrs.end();
	}

	// x の 1 つ右にある区間を指すイテレータを返す（なければ lr.end() を返す）
	typename set<pll>::iterator get_right_iter(ll x) const {
		return lrs.lower_bound({ x, INFL });
	}

	// x の 1 つ左にある区間を指すイテレータを返す（なければ lr.end() を返す）
	typename set<pll>::iterator get_left_iter(ll x) const {
		auto it = lrs.lower_bound({ x, INFL });
		if (it == lrs.begin()) return lrs.end();
		it--;
		if (it->first <= x && x < it->second) {
			if (it == lrs.begin()) return lrs.end();
			it--;
		}
		return it;
	}

	typename set<pll>::iterator begin() const { return lrs.begin(); }
	typename set<pll>::iterator end() const { return lrs.end(); }

public:
	set<pll> lrs; // 区間 [l[i], r[i]) の昇順列

	// コンストラクタ（空で初期化）
	Interval_set_old(bool marge_ = true) : marge(marge_) {}

	// 区間の数を返す．
	int size() const { return sz(lrs); }

	// x が含まれる区間 [l, r) を返す（なければ {-1, -1} を返す）
	pll get(ll x) const {
		// verify : https://atcoder.jp/contests/abc228/tasks/abc228_d

		auto it = get_iter(x);
		return it == lrs.end() ? make_pair(-1LL, -1LL) : *it;
	}

	// x の 1 つ右にある区間 [l, r) を返す（なければ {-1, -1} を返す）
	pll get_right(ll x) const {
		// verify : https://atcoder.jp/contests/code-festival-2015-qualb/tasks/codefestival_2015_qualB_d

		auto it = get_right_iter(x);
		return it == lrs.end() ? make_pair(-1LL, -1LL) : *it;
	}

	// x の 1 つ左にある区間 [l, r) を返す（なければ {-1, -1} を返す）
	pll get_left(ll x) const {
		auto it = get_left_iter(x);
		return it == lrs.end() ? make_pair(-1LL, -1LL) : *it;
	}

	// 区間 [l, r) を追加する．
	void insert(ll l, ll r) {
		// verify : https://atcoder.jp/contests/abc254/tasks/abc254_g

		if (l >= r) return;

		auto it_l = get_iter(l - (int)marge);
		if (it_l == lrs.end()) it_l = get_right_iter(l - (int)marge);

		auto it_r = get_iter(r - (int)(!marge));
		if (it_r == lrs.end()) it_r = get_left_iter(r - (int)(!marge));

		if (it_l != lrs.end() && it_r != lrs.end()) {
			chmin(l, it_l->first);
			chmax(r, it_r->second);
			lrs.erase(it_l, ++it_r);
		}
		lrs.insert({ l, r });
	}

	// 区間 [l, r) を削除する．
	void erase(ll l, ll r) {
		if (l >= r) return;

		ll l2 = l, r2 = r;

		auto it_l = get_iter(l);
		if (it_l != lrs.end()) l2 = it_l->first;
		else it_l = get_right_iter(l - 1);

		auto it_r = get_iter(r);
		if (it_r != lrs.end()) r2 = it_r->second;
		else it_r = get_left_iter(r);

		if (it_l != lrs.end() && it_r != lrs.end()) {
			lrs.erase(it_l, ++it_r);
		}

		if (l2 < l) lrs.insert({ l2, l });
		if (r2 > r) lrs.insert({ r, r2 });
	}

	// 全ての区間のリストを返す．
	vector<pll> get_all_intervals() {
		vector<pll> res;
		res.reserve(sz(lrs));
		repe(lr, lrs) res.push_back(lr);
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Interval_set_old& d) {
		repe(p, d.lrs) os << "[" << p.first << "," << p.second << ") ";
		return os;
	}
#endif
};


