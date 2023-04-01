#pragma once
#include "header.h"
// ■■■■■ ヒープ ■■■■■


//【併合可能ヒープ（降順）】
/*
* Skew_heap<T>() : O(1)
*	空で初期化する．T は比較可能な型で，ヒープからは降順に取り出される．
*
* bool empty() : O(1)
*	ヒープが空かを返す．
*
* int size() : O(1)
*	ヒープの大きさを返す．
*
* T top() : O(1)
*	ヒープ内の最大値を返す．
*
* push(T val) : O(log n)
*	ヒープに値 val を追加する．
*
* pop() : O(log n)
*	ヒープ内の最大値を削除する．
*
* merge(Skew_heap<T>& hp) : O(log n)
*	ヒープ hp を自身に併合する．
*/
template <typename T>
struct Skew_heap {
	// 参考 : https://kopricky.github.io/code/DataStructure_Advanced/skew_heap.html
	
	struct Node {
		Node* l, * r;
		T v;

		Node(T v_) : l(nullptr), r(nullptr), v(v_) {}

		friend Node* meld(Node* a, Node* b) {
			if (a == nullptr) return b;
			if (b == nullptr) return a;

			// a >= b となるよう並び替える
			if (a->v < b->v) swap(a, b);

			// b の方が小さいので，a の左の子とマージしておけば大小関係は保たれる．
			a->l = meld(a->l, b);

			// このままだと毎回左の子が成長していってまずいので，左右の子を交換する．
			swap(a->l, a->r);

			return a;
		}
	};

	Node* root;
	int n;

	// 空で初期化
	Skew_heap() : root(nullptr), n(0) {}

	// ヒープが空かを返す．
	bool empty() const { 
		// verify : https://atcoder.jp/contests/abc246/tasks/abc246_g
		
		return root == nullptr; 
	}

	// ヒープの大きさを返す．
	int size() const { return n; }

	// ヒープに値 val を追加する．
	void push(T val) {
		// verify : https://atcoder.jp/contests/abc246/tasks/abc246_g

		Node* p = new Node(val);
		root = meld(root, p);
		n++;
	}

	// ヒープ内の最大値を返す．
	T top() const {
		// verify : https://atcoder.jp/contests/abc246/tasks/abc246_g
		
		return root->v;
	}

	// ヒープ内の最大値を削除する．
	void pop() {
		// verify : https://atcoder.jp/contests/abc246/tasks/abc246_g

		Node* p = root;
		root = meld(root->r, root->l);
		delete p;
		n--;
	}

	// ヒープ hp を併合する．
	void merge(Skew_heap<T>& hp) {
		// verify : https://atcoder.jp/contests/abc246/tasks/abc246_g

		n += hp.n;
		root = meld(root, hp.root);
		hp.root = nullptr;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Skew_heap<T>& q) {
		q.print_rf(os, q.root);
		return os;
	}
	void print_rf(ostream& os, const Node* pt) const {
		if (pt == nullptr) return;
		os << pt->v << " "; print_rf(os, pt->l);  print_rf(os, pt->r);
	}
#endif
};


//【併合可能ヒープ（昇順）】
/*
* Skew_heap_rev<T>() : O(1)
*	空で初期化する．T は比較可能な型で，ヒープからは昇順に取り出される．
*
* bool empty() : O(1)
*	ヒープが空かを返す．
*
* int size() : O(1)
*	ヒープの大きさを返す．
*
* T top() : O(1)
*	ヒープ内の最小値を返す．
*
* push(T val) : O(log n)
*	ヒープに値 val を追加する．
*
* pop() : O(log n)
*	ヒープ内の最小値を削除する．
*
* merge(Skew_heap_rev& hp) : O(log n)
*	ヒープ hp を自身に併合する．
*/
template <typename T>
struct Skew_heap_rev {
	// 参考 : https://kopricky.github.io/code/DataStructure_Advanced/skew_heap.html
	
	struct Node {
		Node* l, * r;
		T v;

		Node(T v_) : l(nullptr), r(nullptr), v(v_) {}

		friend Node* meld(Node* a, Node* b) {
			if (a == nullptr) return b;
			if (b == nullptr) return a;

			// a <= b となるよう並び替える
			if (a->v > b->v) swap(a, b);

			// b の方が大きいので，a の左の子とマージしておけば大小関係は保たれる．
			a->l = meld(a->l, b);

			// このままだと毎回左の子が成長していってまずいので，左右の子を交換する．
			swap(a->l, a->r);

			return a;
		}
	};

	Node* root;
	int n;

	// 空で初期化
	Skew_heap_rev() : root(nullptr), n(0) {}

	// ヒープが空かを返す．
	bool empty() const {
		// verify : https://atcoder.jp/contests/atc002/tasks/atc002_c

		return root == nullptr;
	}

	// ヒープの大きさを返す．
	int size() const { return n; }

	// ヒープに値 val を追加する．
	void push(T val) {
		// verify : https://atcoder.jp/contests/atc002/tasks/atc002_c

		Node* p = new Node(val);
		root = meld(root, p);
		n++;
	}

	// ヒープ内の最小値を返す．
	T top() const {
		// verify : https://atcoder.jp/contests/atc002/tasks/atc002_c

		return root->v;
	}

	// ヒープ内の最小値を削除する．
	void pop() {
		// verify : https://atcoder.jp/contests/atc002/tasks/atc002_c

		Node* p = root;
		root = meld(root->r, root->l);
		delete p;
		n--;
	}

	// ヒープ hp を併合する．
	void merge(Skew_heap_rev<T>& hp) {
		// verify : https://atcoder.jp/contests/atc002/tasks/atc002_c
		
		n += hp.n;
		root = meld(root, hp.root);
		hp.root = nullptr;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Skew_heap_rev<T>& q) {
		q.print_rf(os, q.root);
		return os;
	}
	void print_rf(ostream& os, const Node* pt) const {
		if (pt == nullptr) return;
		os << pt->v << " "; print_rf(os, pt->l);  print_rf(os, pt->r);
	}
#endif
};


//【併合可能遅延ヒープ（モノイド作用付き全順序集合）】
/*
* Lazy_skew_heap<S, leq, inf, F, act, comp, id>() : O(1)
*	降順に取り出されるヒープを空で初期化する
*	要素はモノイド左作用付き全順序集合 (S, leq(≦), inf, F, act, comp, id) の元とする．
*
* bool empty() : O(1)
*	ヒープが空かを返す．
*
* int size() : O(1)
*	ヒープの大きさを返す．
*
* S top() : O(1)
*	ヒープ内の最大値を返す．
*
* push(S val) : O(log n)
*	ヒープに値 val を追加する．
*
* pop() : O(log n)
*	ヒープ内の最大値を削除する．
*
* merge(Lazy_skew_heap& hp) : O(log n)
*	ヒープ hp を自身に併合する．
*
* apply(F val) : O(1)
*	ヒープ内の全要素に val を作用させる．
*/
template <class S, bool(*leq)(S, S), S(*inf)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Lazy_skew_heap {
	// 参考 : https://joisino.hatenablog.com/entry/2017/01/11/230141

	struct Node {
		Node* l, * r;
		S v;
		F lazy;

		Node(S v_) : l(nullptr), r(nullptr), v(v_), lazy(id()) {}

		void eval() {
			if (l != nullptr) l->lazy = comp(lazy, l->lazy);
			if (r != nullptr) r->lazy = comp(lazy, r->lazy);
			v = act(lazy, v);
			lazy = id();
		}

		friend Node* meld(Node* a, Node* b) {
			if (a == nullptr) return b;
			if (b == nullptr) return a;

			// a >= b となるよう並び替える
			if (leq(act(a->lazy, a->v), act(b->lazy, b->v))) swap(a, b);

			// b の方が小さいので，a の左の子とマージしておけば大小関係は保たれる．
			a->eval();
			a->l = meld(a->l, b);

			// このままだと毎回左の子が成長していってまずいので，左右の子を交換する．
			swap(a->l, a->r);

			return a;
		}
	};

	Node* root;
	int n;

public:
	// 空で初期化
	Lazy_skew_heap() : root(nullptr), n(0) {}

	// ヒープが空かを返す．
	bool empty() const {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_B

		return root == nullptr;
	}

	// ヒープの大きさを返す．
	int size() const {
		return n;
	}

	// ヒープに値 val を追加する．
	void push(S val) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_B

		Node* p = new Node(val);
		root = meld(root, p);
		n++;
	}

	// ヒープ内の最大値を返す．
	S top() const {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_B

		if (root == nullptr) return inf();
		return act(root->lazy, root->v);
	}

	// ヒープ内の最大値を削除する．
	void pop() {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_B

		Assert(root != nullptr);
		Node* p = root;
		p->eval();
		root = meld(root->r, root->l);
		delete p;
		n--;
	}

	// ヒープ hp を併合する．
	void merge(Lazy_skew_heap& hp) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_B

		if (root == hp.root) return;
		n += sz(hp);
		root = meld(root, hp.root);
		hp.root = nullptr;
	}

	// ヒープ内の全要素に val を作用させる．
	void apply(F val) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_2_B

		if (root == nullptr) return;
		root->lazy = comp(val, root->lazy);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Lazy_skew_heap& q) {
		q.print_rf(os, q.root, id());
		return os;
	}
private:
	void print_rf(ostream& os, const Node* pt, F lazy) const {
		if (pt == nullptr) return;
		lazy = comp(lazy, pt->lazy);
		os << act(lazy, pt->v) << " ";
		print_rf(os, pt->l, lazy);
		print_rf(os, pt->r, lazy);
	}
#endif
};


//【多重集合（大小分離）】
/*
* Separated_multiset<T>() : O(1)
*	空で初期化する．
*
* bool empty_l(), empty_h() : O(1)
*	多重集合が空かを返す．
*
* int size_l(), size_h() : O(1)
*	多重集合の大きさを返す．
*
* void insert_l(T val), insert_h(T val) : O(log n)
*	多重集合に値 val を追加する．set_l <= set_h は自動的に保たれる．
*
* T max_l(), min_h() : O(log n)
*	多重集合内の最大値[最小値]を返す．
*
* bool erase_l(T val), erase_h(T val) : O(log n)
*	多重集合内の値 val を削除する（空なら何もせず false を返す）
*	どちらにも要素がなければ false を返す．
* 
* void decrease_l(), decrease_r() : O(log n)
*	多重集合の大きさを 1 減らす．逆側は 1 増える（空なら何もしない）
*/
template <class T>
struct Separated_multiset {
	int nl, nh;
	multiset<T> sl, sh;

	// 空で初期化
	Separated_multiset() : nl(0), nh(0) {}

	// 多重集合が空かを返す．
	bool empty_l() const { return nl == 0; }
	bool empty_h() const { return nh == 0; }

	// 多重集合の大きさを返す．
	int size_l() const { return nl; }
	int size_h() const { return nh; }

	// 多重集合に値 val を追加する．
	void insert_l(const T& val) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g
		
		if (nh == 0 || val <= *sh.begin()) {
			sl.insert(val);
		}
		else {
			sl.insert(*sh.begin());
			sh.erase(sh.begin());
			sh.insert(val);
		}
		nl++;
	}
	void insert_h(const T& val) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g
		
		if (nl == 0 || val >= *sl.rbegin()) {
			sh.insert(val);
		}
		else {
			sh.insert(*sl.rbegin());
			sl.erase(prev(sl.end()));
			sl.insert(val);
		}
		nh++;
	}

	// 多重集合内の最大値[最小値]を返す．
	T max_l() const { return nl > 0 ? *sl.rbegin() : numeric_limits<T>::lowest(); }
	T min_h() const { return nh > 0 ? *sh.begin() : numeric_limits<T>::max(); }

	// 多重集合内の値 val を削除する．
	bool erase_l(const T& val) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g
		
		if (nl == 0) return false;

		auto it_l = sl.lower_bound(val);
		if (it_l != sl.end() && *it_l == val) {
			sl.erase(it_l);
			nl--;
			return true;
		}

		auto it_h = sh.lower_bound(val);
		if (it_h != sh.end() && *it_h == val) {
			sh.erase(it_h);
			sh.insert(*sl.rbegin());
			sl.erase(prev(sl.end()));
			nl--;
			return true;
		}

		return false;
	}
	bool erase_h(const T& val) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g
		
		if (nh == 0) return false;

		auto it_h = sh.lower_bound(val);
		if (it_h != sh.end() && *it_h == val) {
			sh.erase(it_h);
			nh--;
			return true;
		}

		auto it_l = sl.lower_bound(val);
		if (it_l != sl.end() && *it_l == val) {
			sl.erase(it_l);
			sl.insert(*sh.begin());
			sh.erase(sh.begin());
			nh--;
			return true;
		}

		return false;
	}

	// 多重集合の大きさを 1 減らす．
	void decrese_l() {
		if (nl == 0) return;

		sh.insert(*sl.rbegin());
		sl.erase(prev(sl.end()));
		
		nl--; nh++;
	}
	void decrese_h() {
		if (nh == 0) return;

		sl.insert(*sh.begin());
		sh.erase(sh.begin());
		
		nh--; nl++;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Separated_multiset<T>& sms) {
		os << "l: " << sms.sl << endl;
		os << "h: " << sms.sh << endl;
		return os;
	}
#endif
};


//【多重集合の和（大小分離）】
/*
* Separated_multiset_sum<T>() : O(1)
*	空で初期化する．
*
* bool empty_l(), empty_h() : O(1)
*	多重集合が空かを返す．
*
* int size_l(), size_h() : O(1)
*	多重集合の大きさを返す．
*
* void insert_l(T val), insert_h(T val) : O(log n)
*	多重集合に値 val を 1 つ追加する．set_l <= set_h は自動的に保たれる．
*
* T max_l(), min_h() : O(log n)
*	多重集合内の最大値[最小値]を返す．
*
* bool erase_l(T val), erase_h(T val) : O(log n)
*	多重集合内の値 val を 1 つ削除する（空なら何もしない）
*	どちらにも要素がなければ false を返す．
*
* void decrease_l(), decrease_r() : O(log n)
*	多重集合の大きさを 1 減らす．逆側は 1 増える（空なら何もしない）
*
* T sum_l(), sum_h() : O(1)
*	多重集合内の和を返す．
*/
template <class T>
struct Separated_multiset_sum {
	int nl, nh;
	multiset<T> sl, sh;
	T suml, sumh;

	// 空で初期化
	Separated_multiset_sum() : nl(0), nh(0), suml(T(0)), sumh(T(0)) {}

	// 多重集合が空かを返す．
	bool empty_l() const { return nl == 0; }
	bool empty_h() const { return nh == 0; }

	// 多重集合の大きさを返す．
	int size_l() const { 
		// verify : https://atcoder.jp/contests/pakencamp-2022-day2/tasks/pakencamp_2022_day2_e

		return nl; 
	}
	int size_h() const {
		// verify : https://yukicoder.me/problems/no/2028

		return nh;
	}

	// 多重集合に値 val を追加する．
	void insert_l(const T& val) {
		// verify : https://atcoder.jp/contests/abc249/tasks/abc249_f

		if (nh == 0 || val <= *sh.begin()) {
			sl.insert(val);

			suml += val;
		}
		else {
			T v = *sh.begin();
			sl.insert(v);
			sh.erase(sh.begin());
			sh.insert(val);

			suml += v;
			sumh += val - v;
		}
		nl++;
	}
	void insert_h(const T& val) {
		// verify : https://yukicoder.me/problems/no/2028

		if (nl == 0 || val >= *sl.rbegin()) {
			sh.insert(val);

			sumh += val;
		}
		else {
			T v = *sl.rbegin();
			sh.insert(v);
			sl.erase(prev(sl.end()));
			sl.insert(val);

			sumh += v;
			suml += val - v;
		}
		nh++;
	}

	// 多重集合内の最大値[最小値]を返す．
	T max_l() const { return nl > 0 ? *sl.rbegin() : numeric_limits<T>::lowest(); }
	T min_h() const { return nh > 0 ? *sh.begin() : numeric_limits<T>::max(); }

	// 多重集合内の値 val を削除する．
	bool erase_l(const T& val) {
		// verify : https://atcoder.jp/contests/donuts-2015/tasks/donuts_2015_4

		if (nl == 0) return false;

		auto it_l = sl.lower_bound(val);
		if (it_l != sl.end() && *it_l == val) {
			sl.erase(it_l);

			suml -= val;

			nl--;

			return true;
		}

		auto it_h = sh.lower_bound(val);
		if (it_h != sh.end() && *it_h == val) {
			T v = *sl.rbegin();
			sh.erase(it_h);
			sh.insert(v);
			sl.erase(prev(sl.end()));

			suml -= v;
			sumh -= val - v;

			nl--;

			return true;
		}

		return false;
	}
	bool erase_h(const T& val) {
		// verify : https://atcoder.jp/contests/agc018/tasks/agc018_c

		if (nh == 0) return false;

		auto it_h = sh.lower_bound(val);
		if (it_h != sh.end() && *it_h == val) {
			sh.erase(it_h);

			sumh -= val;

			nh--;

			return true;
		}

		auto it_l = sl.lower_bound(val);
		if (it_l != sl.end() && *it_l == val) {
			T v = *sh.begin();
			sl.erase(it_l);
			sl.insert(v);
			sh.erase(sh.begin());

			sumh -= v;
			suml -= val - v;

			nh--;

			return true;
		}

		return false;
	}

	// 多重集合内の和を返す．
	T sum_l() const { 
		// verify : https://atcoder.jp/contests/donuts-2015/tasks/donuts_2015_4
		return suml;
	}
	T sum_h() const {
		// verify : https://yukicoder.me/problems/no/2028
		
		return sumh;
	}

	// 多重集合の大きさを 1 減らす．
	void decrese_l() {
		// verify : https://atcoder.jp/contests/abc249/tasks/abc249_f

		if (nl == 0) return;

		T v = *sl.rbegin();
		sl.erase(prev(sl.end()));
		sh.insert(v);

		suml -= v; sumh += v;

		nl--; nh++;
	}
	void decrese_h() {
		// verify : https://atcoder.jp/contests/abc281/tasks/abc281_e

		if (nh == 0) return;

		T v = *sh.begin();
		sh.erase(sh.begin());
		sl.insert(v);

		sumh -= v; suml += v;

		nh--; nl++;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Separated_multiset_sum<T>& sms) {
		os << "l: " << sms.sl << endl;
		os << "h: " << sms.sh << endl;
		os << "sum: " << sms.suml << " " << sms.sumh << endl;
		return os;
	}
#endif
};


//【二分ヒープ】
/*
* Binary_heap<T>() : O(1)
*	空ヒープで初期化する．
*	制約：T は比較可能な型
*
* Binary_heap<T>(vT a) : O(n log n)
*	配列 a[0..n) で初期化する．
*
* int size() : O(1)
*	ヒープ内の要素数を返す．
*
* bool empty() : O(1)
*	ヒープが空かを返す．
*
* void push(T val) : O(log n)
*	ヒープに値 val を追加する．
*
* T top() : O(1)
*	ヒープ内の最大要素を返す．
*
* void pop() : O(log n)
*	ヒープ内の最大要素を削除する．
*/
template <class T>
class Binary_heap {
	int n; // 格納されているデータの個数
	vector<T> v; // v[1] を根とする完全二分木（v[0] は使わない）

	void push_sub(int i) {
		if (i == 1) return;

		int p = i / 2;
		if (v[p] < v[i]) {
			swap(v[p], v[i]);
			push_sub(p);
		}
	}

	void pop_sub(int i) {
		int l = 2 * i, r = 2 * i + 1;

		int i_max = i;
		if (l <= n && v[l] > v[i_max]) i_max = l;
		if (r <= n && v[r] > v[i_max]) i_max = r;

		if (i_max != i) {
			swap(v[i], v[i_max]); num_swaps++;
			pop_sub(i_max);
		}
	}

public:
	// 空で初期化する．
	Binary_heap() : n(0), v(10) {}

	// 配列 a[0..n) で初期化する．
	Binary_heap(const vector<T>& a) : n(sz(a)), v(n + 10) {
		repi(i, 1, n) v[i] = a[i - 1];
		repir(i, n / 2, 1) pop_sub(i);
	}

	// ヒープ内の要素数を返す．
	int size() {
		return n;
	}

	// ヒープが空かを返す．
	bool empty() {
		return n == 0;
	}

	// ヒープに値 val を追加する．
	void push(T val) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_9_C

		if (sz(v) == ++n) v.resize((int)(1.1 * n));
		v[n] = val;
		push_sub(n);
	}

	// ヒープ内の最大要素を返す．
	T top() {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_9_C

		return v[1];
	}

	// ヒープ内の最大要素を削除する．
	void pop() {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_9_C

		v[1] = v[n--];
		pop_sub(1);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Binary_heap& q) {
		os << q.v << endl;
		return os;
	}
#endif
};


