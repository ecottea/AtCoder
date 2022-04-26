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
template <typename T> struct Skew_heap {
	// 参考 : https://kopricky.github.io/code/DataStructure_Advanced/skew_heap.html
	// verify : https://atcoder.jp/contests/abc246/tasks/abc246_g

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
	bool empty() const { return root == nullptr; }

	// ヒープの大きさを返す．
	int size() const { return n; }

	// ヒープに値 val を追加する．
	void push(T val) {
		Node* p = new Node(val);
		root = meld(root, p);
		n++;
	}

	// ヒープ内の最大値を返す．
	T top() const { return root->v; }

	// ヒープ内の最大値を削除する．
	void pop() {
		Node* p = root;
		root = meld(root->r, root->l);
		delete p;
		n--;
	}

	// ヒープ hp を併合する．
	void merge(Skew_heap<T>& hp) {
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
template <typename T> struct Skew_heap_rev {
	// 参考 : https://kopricky.github.io/code/DataStructure_Advanced/skew_heap.html
	// verify : https://atcoder.jp/contests/atc002/tasks/atc002_c

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
	bool empty() const { return root == nullptr; }

	// ヒープの大きさを返す．
	int size() const { return n; }

	// ヒープに値 val を追加する．
	void push(T val) {
		Node* p = new Node(val);
		root = meld(root, p);
		n++;
	}

	// ヒープ内の最小値を返す．
	T top() const { return root->v; }

	// ヒープ内の最小値を削除する．
	void pop() {
		Node* p = root;
		root = meld(root->r, root->l);
		delete p;
		n--;
	}

	// ヒープ hp を併合する．
	void merge(Skew_heap_rev<T>& hp) {
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


//【多重集合（大小分離）】
/*
* Separated_multiset() : O(1)
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
*	多重集合内の値 val を削除する．要素がなければ false を返す．
* 
* void decrease_l(), decrease_r() : O(log n)
*	多重集合の大きさを 1 減らす．逆側は 1 増える．
*/
template <class T> struct Separated_multiset {
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
	T max_l() const { return nl > 0 ? *sl.rbegin() : numeric_limits<T>::min(); }
	T min_h() const { return nh > 0 ? *sh.begin() : numeric_limits<T>::max(); }

	// 多重集合内の値 val を削除する．
	bool erase_l(const T& val) {
		// verify : https://atcoder.jp/contests/abc218/tasks/abc218_g
		
		assert(nl > 0);

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
		
		assert(nh > 0);

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
		assert(nl > 0);

		sh.insert(*sl.rbegin());
		sl.erase(prev(sl.end()));
		
		nl--; nh++;
	}
	void decrese_h() {
		assert(nh > 0);

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


//【多重集合（大小分離，和）】
/*
* Separated_multiset_sum() : O(1)
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
*	多重集合内の値 val を削除する．要素がなければ false を返す．
*
* void decrease_l(), decrease_r() : O(log n)
*	多重集合の大きさを 1 減らす．逆側は 1 増える．
*
* T sum_l(), sum_h() : O(1)
*	多重集合内の和を返す．
*/
template <class T> struct Separated_multiset_sum {
	int nl, nh;
	multiset<T> sl, sh;
	T suml, sumh;

	// 空で初期化
	Separated_multiset_sum() : nl(0), nh(0), suml(T(0)), sumh(T(0)) {}

	// 多重集合が空かを返す．
	bool empty_l() const { return nl == 0; }
	bool empty_h() const { return nh == 0; }

	// 多重集合の大きさを返す．
	int size_l() const { return nl; }
	int size_h() const { return nh; }

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
	T max_l() const { return nl > 0 ? *sl.rbegin() : numeric_limits<T>::min(); }
	T min_h() const { return nh > 0 ? *sh.begin() : numeric_limits<T>::max(); }

	// 多重集合内の値 val を削除する．
	bool erase_l(const T& val) {
		assert(nl > 0);

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
		assert(nh > 0);

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
	T sum_l() const { return suml; }
	T sum_h() const { return sumh; }

	// 多重集合の大きさを 1 減らす．
	void decrese_l() {
		// verify : https://atcoder.jp/contests/abc249/tasks/abc249_f

		assert(nl > 0);

		T v = *sl.rbegin();
		sl.erase(prev(sl.end()));
		sh.insert(v);

		suml -= v; sumh += v;

		nl--; nh++;
	}
	void decrese_h() {
		assert(nh > 0);

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


