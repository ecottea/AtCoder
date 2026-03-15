#pragma once
#include "header.h"
// ■■■■■ ヒープ ■■■■■


//【併合可能ヒープ】
/*
* Skew_heap<T, comp = less<T>>() : O(1)
*	空で初期化する．
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
* merge(Skew_heap& hp) : O(log n)
*	ヒープ hp を自身に併合する．
*/
template <class T, class comp = less<T>>
class Skew_heap {
	// 参考 : https://kopricky.github.io/code/DataStructure_Advanced/skew_heap.html

	struct Node {
		Node* l, * r;
		T v;

		Node(T v_) : l(nullptr), r(nullptr), v(v_) {}

		friend Node* meld(Node* a, Node* b) {
			if (a == nullptr) return b;
			if (b == nullptr) return a;

			// a ≧ b となるよう並び替える
			if (comp()(a->v, b->v)) swap(a, b);

			// b の方が小さいので，a の左の子とマージしておけば大小関係は保たれる．
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
	Skew_heap() : root(nullptr), n(0) {
		// verify : https://atcoder.jp/contests/abc246/tasks/abc246_g
	}

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
	void merge(Skew_heap<T, comp>& hp) {
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


//【併合可能遅延ヒープ（M-全順序集合）】
/*
* Lazy_skew_heap<S, leq, inf, F, act, comp, id>() : O(1)
*	降順に取り出されるヒープを空で初期化する
*	要素はモノイド左作用付き全順序集合 (S, leq, inf, F, act, comp, id) の元とする．
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
* apply(F f) : O(1)
*	ヒープ内の全要素に f を作用させる．
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


//【削除可能ヒープ】
/*
* Eraseable_heap<T, comp = less<T>>() : O(1)
*	最大値を管理するヒープを空で初期化する．
*
* bool empty() : O(1)
*	ヒープが空かを返す．
*
* int size() : O(1)
*	ヒープ内の要素数を返す．
*
* T top() : ならし O(log n)
*	ヒープ内の最大の要素を返す．
*	制約 : ヒープが空でない
*
* void push(T x) : O(log n)
*	ヒープに x を追加する．
*
* void pop() : ならし O(log n)
*	ヒープ内の最大の要素を削除する．
*	制約 : ヒープが空でない
*
* void erase(T x) : O(log n)
*	ヒープ内の要素 x を削除する．
*	制約 : ヒープ内に要素 x が存在する．
* 
* void rebuild() : O(n log n)
*	ヒープを再構築する．
*/
template <class T, class comp = less<T>>
class Eraseable_heap {
	// q : 順位キュー
	// qe : 削除された要素を覚えておく順位キュー
	priority_queue<T, vector<T>, comp> q, qe;

	void shrink() {
		// 最大要素が削除済みである限り実際に削除する．
		while (!q.empty() && !qe.empty()) {
			if (q.top() == qe.top()) {
				q.pop();
				qe.pop();
			}
			else break;
		}
	}

public:
	// 空で初期化する．
	Eraseable_heap() {
		// verify : https://atcoder.jp/contests/abc342/tasks/abc342_g
	}

	// 空かを返す．
	bool empty() {
		// verify : https://atcoder.jp/contests/abc342/tasks/abc342_g

		return sz(q) == sz(qe);
	}

	// 要素数を返す．
	int size() const {
		return sz(q) - sz(qe);
	}

	// 最大の要素を返す．
	T top() {
		// verify : https://atcoder.jp/contests/abc342/tasks/abc342_g

		shrink();
		Assert(!q.empty());
		return q.top();
	}

	// x を追加する．
	void push(T x) {
		// verify : https://atcoder.jp/contests/abc342/tasks/abc342_g

		q.push(x);
	}

	// 最大の要素を削除する．
	void pop() {
		shrink();
		Assert(!q.empty());
		q.pop();
	}

	// x を削除する．
	void erase(T x) {
		// verify : https://atcoder.jp/contests/abc342/tasks/abc342_g

		qe.push(x);

		// if (sz(q) < sz(qe)) rebuild(); // ヒープが肥大化しそうなときは使う
	}

	// 再構築する．
	void rebuild() {
		// verify : https://projecteuler.net/problem=870

		vector<T> tmp;
		tmp.reserve(size());
		while (!empty()) {
			tmp.push_back(top()); pop();
		}
		q = priority_queue<T, vector<T>, comp>(all(tmp));
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Eraseable_heap q) {
		while (!q.empty()) {
			os << q.top() << " ";
			q.pop();
		}
		return os;
	}
#endif
};


//【削除可能分離ヒープ】
/*
* Eraseable_separated_heap<T>() : O(1)
*	空のヒープで初期化する．
*
* bool empty_l(), empty_h() : O(1)
*	ヒープが空かを返す．
*
* int size_l(), size_h() : O(1)
*	ヒープの大きさを返す．
*
* void push_l(T x), push_h(T x) :ならし  O(log n)
*	ヒープに要素 x を追加する．set_l ≦ set_h は自動的に保たれる．
*
* T max_l(), min_h() : ならし O(log n)
*	ヒープ内の最大値[最小値]を返す．
*	制約 : ヒープが空でない．
*
* erase_l(T x), erase_h(T x) : ならし O(log n)
*	ヒープ内の要素 x を削除する．
*	制約 : どちらかのヒープ内に要素 x が存在する．
*
* void decrease_l(), decrease_r() : O(1)
*	ヒープの大きさを 1 減らす．逆側は 1 増える．
*	制約 : ヒープは空でない
*
* 利用：【削除可能ヒープ】
*/
template <class T>
class Eraseable_separated_heap {
	int nl, nh;
	Eraseable_heap<T> ql;
	Eraseable_heap<T, greater<T>> qh;

	// ql, qh の大きさが nl, nh に一致するよう調整する．
	void move() {
		while (sz(ql) < nl) {
			ql.push(qh.top());
			qh.pop();
		}
		while (sz(qh) < nh) {
			qh.push(ql.top());
			ql.pop();
		}
	}

public:
	// 空で初期化
	Eraseable_separated_heap() : nl(0), nh(0) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g
	}

	// ヒープが空かを返す．
	bool empty_l() const { return nl == 0; }
	bool empty_h() const { return nh == 0; }

	// ヒープの大きさを返す．
	int size_l() const { return nl; }
	int size_h() const { return nh; }

	// ヒープに要素 x を追加する．
	void push_l(const T& x) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g

		if (qh.empty() || x <= qh.top()) ql.push(x);
		else qh.push(x);
		++nl;
	}
	void push_h(const T& x) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g

		if (ql.empty() || x >= ql.top()) qh.push(x);
		else ql.push(x);
		++nh;
	}

	// ヒープ内の最大値[最小値]を返す．
	T max_l() {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g

		Assert(nl > 0);
		move();
		return ql.top();
	}
	T min_h() {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g

		Assert(nh > 0);
		move();
		return qh.top();
	}

	// ヒープ内の要素 x を削除する．
	void erase_l(const T& x) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g

		if (!ql.empty() && x <= ql.top()) ql.erase(x);
		else qh.erase(x);
		--nl;
	}
	void erase_h(const T& x) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g

		if (!qh.empty() && x >= qh.top()) qh.erase(x);
		else ql.erase(x);
		--nh;
	}

	// ヒープの大きさを 1 減らす．
	void decrese_l() {
		Assert(nl > 0);
		nl--; nh++;
	}
	void decrese_h() {
		Assert(nh > 0);
		nh--; nl++;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Eraseable_separated_heap<T> q) {
		q.move();
		os << "l: " << q.ql << endl;
		os << "h: " << q.qh << endl;
		return os;
	}
#endif
};


//【削除可能分離ヒープ（総和）】
/*
* Eraseable_separated_heap_sum<T> : O(1)
*	空のヒープで初期化する．
*
* bool empty_l(), empty_h() : O(1)
*	ヒープが空かを返す．
*
* int size_l(), size_h() : O(1)
*	ヒープの大きさを返す．
*
* void push_l(T x), push_h(T x) : ならし  O(log n)
*	ヒープに要素 x を追加する．set_l ≦ set_h は自動的に保たれる．
*
* T max_l(), min_h() : ならし O(log n)
*	ヒープ内の最大値[最小値]を返す．
*	制約 : ヒープが空でない．
*
* erase_l(T x), erase_h(T x) : ならし O(log n)
*	ヒープ内の要素 x を削除する．
*	制約 : どちらかのヒープ内に要素 x が存在する．
*
* void decrease_l(), decrease_h() : O(1)
*	ヒープの大きさを 1 減らす．逆側は 1 増える．
*	制約 : ヒープは空でない
*
* void sum_l(), sum_h() : ならし O(1)
*	ヒープ内の要素の総和を返す．
*
* 利用：【削除可能ヒープ】
*/
template <class T>
class Eraseable_separated_heap_sum {
	int nl, nh;
	Eraseable_heap<T> ql;
	Eraseable_heap<T, greater<T>> qh;
	T suml, sumh;

	// ql, qh の大きさが nl, nh に一致するよう調整する．
	void move() {
		while (sz(ql) < nl) {
			auto x = qh.top();
			ql.push(x);
			suml += x;
			qh.pop();
			sumh -= x;
		}
		while (sz(qh) < nh) {
			auto x = ql.top();
			qh.push(x);
			sumh += x;
			ql.pop();
			suml -= x;
		}
	}

public:
	// 空で初期化
	Eraseable_separated_heap_sum() : nl(0), nh(0), suml(T(0)), sumh(T(0)) {
		// verify : https://atcoder.jp/contests/abc306/tasks/abc306_e
	}

	// ヒープが空かを返す．
	bool empty_l() const { return nl == 0; }
	bool empty_h() const { return nh == 0; }

	// ヒープの大きさを返す．
	int size_l() const { return nl; }
	int size_h() const {
		// verify : https://atcoder.jp/contests/abc281/tasks/abc281_e

		return nh;
	}

	// ヒープに要素 x を追加する．
	void push_l(const T& x) {
		// verify : https://atcoder.jp/contests/abc306/tasks/abc306_e

		if (qh.empty() || x <= qh.top()) {
			ql.push(x);
			suml += x;
		}
		else {
			qh.push(x);
			sumh += x;
		}
		++nl;
	}
	void push_h(const T& x) {
		// verify : https://atcoder.jp/contests/abc306/tasks/abc306_e

		if (ql.empty() || x >= ql.top()) {
			qh.push(x);
			sumh += x;
		}
		else {
			ql.push(x);
			suml += x;
		}
		++nh;
	}

	// ヒープ内の最大値[最小値]を返す．
	T max_l() {
		Assert(nl > 0);
		move();
		return ql.top();
	}
	T min_h() {
		Assert(nh > 0);
		move();
		return qh.top();
	}

	// ヒープ内の要素 x を削除する．
	void erase_l(const T& x) {
		// verify : https://atcoder.jp/contests/donuts-2015/tasks/donuts_2015_4

		if (!ql.empty() && x <= ql.top()) {
			ql.erase(x);
			suml -= x;
		}
		else {
			qh.erase(x);
			sumh -= x;
		}
		--nl;
	}
	void erase_h(const T& x) {
		// verify : https://atcoder.jp/contests/abc306/tasks/abc306_e

		if (!qh.empty() && x >= qh.top()) {
			qh.erase(x);
			sumh -= x;
		}
		else {
			ql.erase(x);
			suml -= x;
		}
		--nh;
	}

	// ヒープの大きさを 1 減らす．
	void decrese_l() {
		// verify : https://atcoder.jp/contests/donuts-2015/tasks/donuts_2015_4

		Assert(nl > 0);
		nl--; nh++;
	}
	void decrese_h() {
		// verify : https://atcoder.jp/contests/abc281/tasks/abc281_e

		Assert(nh > 0);
		nh--; nl++;
	}

	// ヒープ内の和を返す．
	T sum_l() {
		// verify : https://atcoder.jp/contests/abc376/tasks/abc376_e

		move();
		return suml;
	}
	T sum_h() {
		// verify : https://atcoder.jp/contests/abc306/tasks/abc306_e

		move();
		return sumh;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Eraseable_separated_heap_sum<T> q) {
		q.move();
		os << "l: " << q.ql << endl;
		os << "h: " << q.qh << endl;
		os << "sum: " << q.suml << " " << q.sumh << endl;
		return os;
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
*	多重集合に値 val を 1 つ追加する．set_l ≦ set_h は自動的に保たれる．
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


