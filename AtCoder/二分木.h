#pragma once
#include "header.h"
// ■■■■■ 二分木 ■■■■■


//【二分木のノード】
struct BTNode {
	int parent = -1; // 親（なければ -1）
	int left = -1; // 左の子（なければ -1）
	int right = -1; // 右の子（なければ -1）
	int depth = -1; // 深さ（根からのパスの長さ）
	int height = -1; // 高さ（最も遠い葉へのパスの長さ）

	// 出力
	friend ostream& operator<<(ostream& os, const BTNode& v) {
		os << '(' << v.parent << ',' << v.left << ',' << v.right << ',' << v.depth << ',' << v.height << ')';
		return os;
	}
};



//【二分木の深さ，高さ】O(|V|)
/*
* 二分木 rt の親と子の情報を元に，各ノードに深さと高さを設定する．
* また根の番号を返す．
*/
int decide_depth(vector<BTNode>& bt) {
	int n = (int)bt.size();

	// 親が設定されていないノードが根である．
	int r = -1;
	rep(i, n) {
		if (bt[i].parent == -1) {
			r = i;
			break;
		}
	}

	// 再帰用の関数
	// s : 注目ノード，d : s の深さ，戻り値：s の高さ
	function<int(int, int)> dfs = [&](int s, int d) {
		if (s == -1) {
			return -1;
		}

		// 行きがけに深さを求める．
		bt[s].depth = d;

		int hl = dfs(bt[s].left, d + 1);
		int hr = dfs(bt[s].right, d + 1);

		// 帰りがけに高さを求める．
		bt[s].height = max(hl, hr) + 1;

		return bt[s].height;
	};

	// 根 r を始点として再帰関数を呼び出す．
	dfs(r, 0);

	return r;
}


//【二分木の深さ優先探索】O(|V|)
/*
* 根を s とする二分木 bt を深さ優先でなぞりながら，
* 行きがけ順に pre(i)，通りがけ順に in(i)，帰りがけ順に post(i) を実行する．
*/
void traverse_dfs_binary_tree(vector<BTNode>& bt, int r,
	function<void(int)>& pre, function<void(int)>& in, function<void(int)>& post) {
	// 行きがけ順の処理
	pre(r);

	// 左の子があれば左の子をなぞりにいく．
	if (bt[r].left != -1) {
		traverse_dfs_binary_tree(bt, bt[r].left, pre, in, post);
	}

	// 通りがけ順の処理
	in(r);

	// 右の子があれば右の子をなぞりにいく．
	if (bt[r].right != -1) {
		traverse_dfs_binary_tree(bt, bt[r].right, pre, in, post);
	}

	// 帰りがけ順の処理
	post(r);
}


//【二分探索木】
/*
* binary_search_tree() : O(1)
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
template <class T>
struct binary_search_tree {
	// 二分探索木のノード
	struct bst_node {
		T key;
		bst_node* left;
		bst_node* right;
	};

	// 二分探索木のノード数
	int n;

	// 根へのポインタ
	bst_node* root;

	// コンストラクタ（空の二分探索木で初期化）：O(1)
	binary_search_tree() : n(0), root(nullptr) {}

	// 要素の挿入：平均 O(log n)／最悪 O(n)
	void insert(T& key) {
		// 挿入すべき位置を二分探索する．
		bst_node** p = &root;
		while (*p != nullptr) {
			if (key < (*p)->key) {
				p = &((*p)->left);
			}
			else {
				p = &((*p)->right);
			}
		}

		// 新しいノードの作成
		bst_node* node = new bst_node;
		node->key = key;
		node->left = nullptr;
		node->right = nullptr;
		*p = node;
		n++;
	}

	// 要素の探索：平均 O(log n)／最悪 O(n)
	bool find(T& key) {
		// key を二分探索する．
		bst_node* x = root;
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
		bst_node** p = &root;
		while (*p != nullptr) {
			// 見つかった場合
			if (key == (*p)->key) {
				// 見つかったノードを x とする．
				bst_node* x = *p;

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
	bst_node* delete_min(bst_node** p) {
		// 最小の要素は左の子をずっと辿った先にある．
		while ((*p)->left != nullptr) {
			p = &((*p)->left);
		}

		// 最小の要素には左の子は居ないので，
		// 親からのポインタに右の子を繋いで短絡しておく．
		bst_node* x = *p;
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
template <class T>
struct Treap {
	// 参考 : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_8_D

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


//【構文解析（逆ポーランド記法）】
/*
* 逆ポーランド記法の数式 str を構文解析して数式の木を bt に構築する．
* str の項は空白区切りになっており，順に term に格納される．
*
* 利用：【二分木のノード】
*/
int rpn_tree(const string& str, vector<BTNode>& bt, vector<string>& term) {
	stringstream ss{ str };
	bt.clear();
	term.clear();

	string s;
	int i = 0;
	stack<int> stk;
	while (getline(ss, s, ' ')) {
		bt.push_back(BTNode());
		term.push_back(s);

		// 演算子の場合
		if (!isdigit(s[0])) {
			bt[i].right = stk.top();
			stk.pop();
			bt[i].left = stk.top();
			stk.pop();
		}

		stk.push(i++);
	}

	return i;
}


//【構文解析（ポーランド記法）】
/*
* ポーランド記法の数式 str を構文解析して数式の木を bt に構築する．
* str の項は空白区切りになっており，順に term に格納される．
*
* 利用：【二分木のノード】
*/
int pn_tree(const string& str, vector<BTNode>& bt, vector<string>& term) {
	stringstream ss{ str };
	bt.clear();
	term.clear();

	string s;
	int i = -1;
	function<int()> rf = [&]() {
		getline(ss, s, ' ');
		bt.push_back(BTNode());
		term.push_back(s);
		i++;
		int pt = i;

		// 演算子の場合
		if (!isdigit(s[0])) {
			bt[pt].left = rf();
			bt[pt].right = rf();
		}

		return pt;
	};

	rf();

	return i;
}


