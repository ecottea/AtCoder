#pragma once
#include "header.h"
// ■■■■■ ヒープ ■■■■■


//【併合可能ヒープ（降順）】
/*
* Skew_heap<T>() : O(1)
*	空で初期化する．T は比較可能な型で，ヒープからは降順に取り出される．
*
* empty() : O(1)
*	ヒープが空かを返す．
*
* size() : O(1)
*	ヒープの大きさを返す．
*
* top() : O(1)
*	ヒープ内の最大値を返す．
*
* push(val) : O(log n)
*	ヒープに値 val を追加する．
*
* pop() : O(log n)
*	ヒープ内の最大値を削除する．
*
* merge(hp) : O(log n)
*	ヒープ hp を併合する．
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
* Skew_heap<T>() : O(1)
*	空で初期化する．T は比較可能な型で，ヒープからは昇順に取り出される．
*
* empty() : O(1)
*	ヒープが空かを返す．
*
* size() : O(1)
*	ヒープの大きさを返す．
*
* top() : O(1)
*	ヒープ内の最小値を返す．
*
* push(val) : O(log n)
*	ヒープに値 val を追加する．
*
* pop() : O(log n)
*	ヒープ内の最小値を削除する．
*
* merge(hp) : O(log n)
*	ヒープ hp を併合する．
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


