#pragma once
#include "header.h"
// ■■■■■ 二分木 ■■■■■


//【二分木】
/*
* Binary_Tree() : O(1)
*	空で初期化する．
*
* Binary_Tree(vi s, vi l, vi r) : O(n)
*	s[i] の左の子が l[i]，右の子が r[i] であるような二分木で初期化する．
*	存在しない場合は -1 を与える．
*/
struct Binary_Tree {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_7_B

	struct Node {
		int parent = -1; // 親（なければ -1）
		int left = -1; // 左の子（なければ -1）
		int right = -1; // 右の子（なければ -1）
		int depth = -1; // 深さ（根からのパスの長さ）
		int height = -1; // 高さ（最も遠い葉へのパスの長さ）
		int weight = -1; // 重さ（自身を根とする部分木の頂点の数）

		// 出力
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "(p:" << v.parent << ", l:" << v.left << ", r:" << v.right <<
				", d:" << v.depth << ", h:" << v.height << ", w:" << v.weight << ')';
			return os;
		}
	};

	int n; // 頂点の数
	int root; // 根
	vector<Node> v; // 頂点

	// コンストラクタ（初期化なし，子の情報で初期化）
	Binary_Tree() : n(0), root(-1) {}
	Binary_Tree(const vi& s, const vi& l, const vi& r) : n(sz(s)), v(n) {
		// 親子関係を設定する．
		rep(i, n) {
			v[s[i]].left = l[i];
			v[s[i]].right = r[i];
			if (l[i] != -1) v[l[i]].parent = s[i];
			if (r[i] != -1) v[r[i]].parent = s[i];
		}

		// 親が設定されていないノードが根である．
		rep(i, n) {
			if (v[i].parent == -1) {
				root = i;
				break;
			}
		}

		// 頂点の各種情報を決定する（s : 注目ノード，p : s の親）
		function<void(int)> dfs = [&](int s) {
			v[s].weight = 1;
			v[s].height = 0;

			int t = v[s].left;
			if (t != -1) {
				v[t].depth = v[s].depth + 1;
				dfs(t);
				v[s].weight += v[t].weight;
				chmax(v[s].height, v[t].height + 1);
			}

			t = v[s].right;
			if (t != -1) {
				v[t].depth = v[s].depth + 1;
				dfs(t);
				v[s].weight += v[t].weight;
				chmax(v[s].height, v[t].height + 1);
			}
		};

		// 根 root を始点として再帰関数を呼び出す．
		v[root].depth = 0;
		dfs(root);
	}

	// アクセス
	Node const& operator[](int i) const { return v[i]; }
	Node& operator[](int i) { return v[i]; }

	// 大きさ
	int size() const { return n; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Binary_Tree& rt) {
		rep(i, sz(rt)) os << rt[i] << endl;
		return os;
	}
#endif
};


//【二分木の入力】O(n)
/*
* (自身 左の子 右の子) を並べた入力を受け取り，n 頂点の二分木 bt を構築する．
* 非存在を表す入力を nval に与える．
*
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_binary_tree(int n, Binary_Tree& bt, bool one_indexed = true, int nval = -1) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_7_B

	vi s(n), l(n), r(n);
	rep(i, n) {
		cin >> s[i] >> l[i] >> r[i];

		if (s[i] == nval) s[i] = -1;
		if (l[i] == nval) l[i] = -1;
		if (r[i] == nval) r[i] = -1;

		if (one_indexed) {
			if (s[i] != -1) s[i]--;
			if (l[i] != -1) l[i]--;
			if (r[i] != -1) r[i]--;
		}
	}
	bt = Binary_Tree(s, l, r);
}


//【デカルト木】O(n)
/*
* a[0..n) の最小要素の位置を根とするデカルト木を ct に構築する．
* 根から順に小さい要素での区間の分割を表す（同じ要素は左のものほど小さいとする．）
* smaller = false とすると，大小関係を逆転して木の構築を行う．
*
* 利用：【二分木】
*/
template <class T> void cartesian_tree(const vector<T>& a, Binary_Tree& ct, bool smaller = true) {
	// verify : https://judge.yosupo.jp/problem/cartesian_tree

	int n = sz(a);

	// p[i] : i の親，l[i] : i の左の子，r[i] : i の右の子, rt : 根
	vi p(n, -1), l(n, -1), r(n, -1);
	int rt = 0;

	repi(i, 1, n - 1) {
		// pt : i - 1 の祖先で値が a[i] 以下であるもののうち最も深いもの（なければ -1）
		int pt = i - 1;
		while (pt != -1 && (smaller ? a[pt] > a[i] : a[pt] < a[i])) {
			pt = p[pt];
		}

		// pt の右の子を i，i の左の子を pt の元の右の子とする．
		if (pt != -1) {
			p[i] = pt;
			if (r[pt] != -1) p[r[pt]] = i;
			l[i] = r[pt];
			r[pt] = i;
		}
		else {
			l[i] = rt;
			p[rt] = i;
			rt = i;
		}
	}

	vi s(n);
	iota(all(s), 0);
	ct = Binary_Tree(s, l, r);
}


//【構文解析（逆ポーランド記法）】O(n)
/*
* 逆ポーランド記法の数式 str を構文解析して数式の木を bt に構築する．
* str の項は空白区切りになっており，順に term に格納される．
*
* 利用：【二分木】
*/
void reverse_polish_notation_tree(const string& str, Binary_Tree& bt, vector<string>& term) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_3_A

	stringstream ss{ str };
	term.clear();
	vi v, l, r; // v[i] の左の子が l[i], 右の子が r[i] であることを記録する．

	string s;
	int i = 0;
	stack<int> stk;
	while (getline(ss, s, ' ')) {
		term.push_back(s);
		v.push_back(i);

		// 演算子の場合
		if (!isdigit(s[0])) {
			r.push_back(stk.top()); stk.pop();
			l.push_back(stk.top()); stk.pop();
		}
		// 数値の場合
		else {
			r.push_back(-1);
			l.push_back(-1);
		}

		stk.push(i++);
	}

	bt = Binary_Tree(v, l, r);
}


//【構文解析（ポーランド記法）】O(n)
/*
* ポーランド記法の数式 str を構文解析して数式の木を bt に構築する．
* str の項は空白区切りになっており，順に term に格納される．
*
* 利用：【二分木】
*/
void polish_notation_tree(const string& str, Binary_Tree& bt, vector<string>& term) {
	stringstream ss{ str };
	term.clear();
	vi v, l, r; // v[i] の左の子が l[i], 右の子が r[i] であることを記録する．

	string s;
	int i = -1;
	function<int()> rf = [&]() {
		i++;
		getline(ss, s, ' ');

		term.push_back(s);
		v.push_back(i);
		l.push_back(-1);
		r.push_back(-1);

		// 演算子の場合
		int pt = i;
		if (!isdigit(s[0])) {
			l[pt] = rf();
			r[pt] = rf();
		}

		return pt;
	};

	rf();

	bt = Binary_Tree(v, l, r);
}


//【ハフマン符号木】O(n)
/*
* n 種類の文字 i の出現頻度が p[i] > 0 であるときのハフマン符号木を bt に構築する．
*
*（頻度が低い順に貪欲法）
*
* 利用：【二分木】
*/
void huffman_tree(const vi& p, Binary_Tree& bt) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_15_D

	int n = sz(p);

	// 出現頻度の低い順に文字を取り出す優先度付きキュー
	priority_queue_rev<pii> q;

	// s[i] の左の子が l[i]，右の子が r[i] であることの記録用
	vi s, l, r;

	// ハフマン木の葉となる文字たちをキューに追加する．
	rep(i, n) {
		q.push({ p[i], i });

		// 葉であることを記録する．
		s.push_back(i);
		l.push_back(-1);
		r.push_back(-1);
	}

	// 出現頻度の低い文字 2 つを組にして部分木を作り，
	// それらを合わせた分の出現頻度をもつ新たな文字とみなすことを繰り返す．
	int i = n;
	while (sz(q) > 1) {
		// 出現頻度の低い文字 2 つを得る．
		int p1, p2, i1, i2;
		tie(p1, i1) = q.top(); q.pop();
		tie(p2, i2) = q.top(); q.pop();

		// それらを合わせた文字を表すノードを作り，キューに追加する．
		q.push({ p1 + p2, i });

		// 親子関係を記録する．
		s.push_back(i);
		l.push_back(i1);
		r.push_back(i2);

		i++;
	}

	// ハフマン木を構築する．
	bt = Binary_Tree(s, l, r);
}


//【二分探索木】
/*
* Binary_search_tree() : O(1)
*	空の二分探索木で初期化する．
*
* insert(key) : 平均 O(log n)／最悪 O(n)
*	key を挿入する．
*
* find(key) : 平均 O(log n)／最悪 O(n)
*	key が存在すれば true, さもなくば false を返す．
*
* erase(key) : 平均 O(log n)／最悪 O(n)
*	key を削除する．
*/
template <class T> struct Binary_search_tree {
	verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_8_C

	// 二分探索木のノード
	struct Node {
		T key;
		Node* left;
		Node* right;
	};

	int n; // ノード数
	Node* root; // 根へのポインタ

	// コンストラクタ（空の二分探索木で初期化）：O(1)
	Binary_search_tree() : n(0), root(nullptr) {}

	// 要素の挿入：平均 O(log n)／最悪 O(n)
	void insert(T& key) {
		// 挿入すべき位置を二分探索する．
		Node** p = &root;
		while (*p != nullptr) {
			if (key < (*p)->key) {
				p = &((*p)->left);
			}
			else {
				p = &((*p)->right);
			}
		}

		// 新しいノードの作成
		Node* node = new Node;
		node->key = key;
		node->left = nullptr;
		node->right = nullptr;
		*p = node;
		n++;
	}

	// 要素の探索：平均 O(log n)／最悪 O(n)
	bool find(T& key) {
		// key を二分探索する．
		Node* x = root;
		while (x != nullptr) {
			if (key == x->key) {
				return true;
			}

			if (key < x->key) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		return false;
	}

	// 要素の削除：平均 O(log n)／最悪 O(n)
	void erase(T& key) {
		// 挿入すべき位置を二分探索する．
		Node** p = &root;
		while (*p != nullptr) {
			// 見つかった場合
			if (key == (*p)->key) {
				// 見つかったノードを x とする．
				Node* x = *p;

				// x が子をもたない場合
				if (x->left == nullptr && x->right == nullptr) {
					*p = nullptr;
				}
				// x が右の子のみをもつ場合
				else if (x->left == nullptr) {
					// 短絡する．
					*p = x->right;
				}
				// x が左の子のみをもつ場合
				else if (x->right == nullptr) {
					// 短絡する．
					*p = x->left;
				}
				// x が左右の子をもつ場合
				else {
					// x の右の子から最小の要素を削除し，
					// その要素へ親からのポインタを繋ぐ．
					*p = delete_min(&(x->right));

					// その要素の左右の子を x の左右の子とする．
					(*p)->right = x->right;
					(*p)->left = x->left;
				}

				delete x;
				n--;

				return;
			}

			// 見つからなかった場合は子を探しに行く．
			if (key < (*p)->key) {
				p = &((*p)->left);
			}
			else {
				p = &((*p)->right);
			}
		}
	}

	// *p が指す部分木の最小の要素を削除し，その要素へのポインタを返す．
	Node* delete_min(Node** p) {
		// 最小の要素は左の子をずっと辿った先にある．
		while ((*p)->left != nullptr) {
			p = &((*p)->left);
		}

		// 最小の要素には左の子は居ないので，
		// 親からのポインタに右の子を繋いで短絡しておく．
		Node* x = *p;
		*p = (*p)->right;

		return x;
	}
};


//【ツリープ】
/*
* treap() : O(1)
*	空のツリープで初期化する．
*
* insert(key) : O(log n)
*	key を挿入する．
*
* count(key) : O(log n)
*	key が存在すれば 1, さもなくば 0 を返す．
*
* erase(key) : O(log n)
*	key を削除する．
*/
template <class T> struct Treap {
	// varify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_8_D

	// ツリープのノード
	struct Node {
		T key;
		unsigned int priority;
		Node* left;
		Node* right;

		// コンストラクタ
		Node() {}
		Node(const T& k, unsigned int p)
			: key(k), priority(p), left(nullptr), right(nullptr) {}
	};

	int n; // ツリープのノード数
	Node* root; // 根へのポインタ
	mt19937 rnd; // 乱数生成器

	// コンストラクタ（空のツリープで初期化）
	Treap() : n(0), root(nullptr), rnd((int)time(0)) {}

	// 要素の挿入
	void insert(const T& key) { insert_rf(root, key, rnd()); n++; }
	void insert_rf(Node*& t, const T& key, unsigned int priority) {
		// 葉に到達したら新しいノードを作って繋ぐ．
		if (t == nullptr) {
			t = new Node(key, priority);
			return;
		}

		// 重複した key は無視する．
		if (key == t->key) {
			return;
		}

		// 左の子への移動
		if (key < t->key) {
			insert_rf(t->left, key, priority);
			if (t->priority < priority) {
				rotate_right(t);
			}
		}
		// 右の子への移動
		else {
			insert_rf(t->right, key, priority);
			if (t->priority < priority) {
				rotate_left(t);
			}
		}
	}

	// 木の右回転
	void rotate_right(Node*& t) {
		Node* s = t->left;
		t->left = s->right;
		s->right = t;
		t = s;
	}

	// 木の左回転
	void rotate_left(Node*& t) {
		Node* s = t->right;
		t->right = s->left;
		s->left = t;
		t = s;
	}

	// 要素の探索
	int count(const T& key) {
		Node** p = &root;
		p = move(p, key);
		return *p == nullptr ? 0 : 1;
	}
	Node** move(Node** p, const T& key) {
		// key を二分探索する．
		while (*p != nullptr) {
			if (key == (*p)->key) {
				return p;
			}

			if (key < (*p)->key) {
				p = &(*p)->left;
			}
			else {
				p = &(*p)->right;
			}
		}
		return p;
	}

	// 要素の削除
	void erase(const T& key) {
		// 削除対象の位置まで移動する．
		Node** p = &root;
		p = move(p, key);

		// 削除対象があれば削除する．
		if (*p != nullptr) {
			erase_rf(*p);
			n--;
		}
	}
	void erase_rf(Node*& t) {
		// 葉の場合
		if (t->left == nullptr && t->right == nullptr) {
			// 単に削除して終了．
			delete t;
			t = nullptr;
		}
		// 右の子のみもつ場合
		else if (t->left == nullptr) {
			// 短絡して終了．
			Node* s = t;
			t = t->right;
			delete s;
		}
		// 左の子のみもつ場合
		else if (t->right == nullptr) {
			// 短絡して終了．
			Node* s = t;
			t = t->left;
			delete s;
		}
		// 左右の子をもつ場合
		else {
			// 優先度の高い方が上にくるよう回転し，再帰する．
			if (t->left->priority < t->right->priority) {
				rotate_left(t);
				erase_rf(t->left);
			}
			else {
				rotate_right(t);
				erase_rf(t->right);
			}
		}
	}
};


