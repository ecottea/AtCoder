#pragma once
#include "header.h"
#include "二分木.h"
// ■■■■■ 構文解析 ■■■■■


//【数式の木（逆ポーランド記法）】O(n)
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


//【数式の木（ポーランド記法）】O(n)
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


//【ランレングス符号木】
/*
* Run_length_tree(string s) : O(n)
*	再帰的にランレングス符号化された文字列 s を構文解析し構文木を構築する．
*	(例) s = "2(2u2lt4d)3(rb)pa" を復号すると s' = "uulltdddduulltddddrbrbrbpa"
*/
struct Run_length_tree {
	struct Node {
		char c = 0; // 対応する文字（葉のみ）
		ll mul = 1; // 反復回数（根では 1）
		ll l = -1, r = -1; // 復号後の文字列 s'[l..r) に対応するノードであることを表す（最左優先）
		int parent = -1; // 親（なければ -1）
		vi childs; // 子

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "[" << v.l << "," << v.r << "):" << v.mul << v.c
				<< ", p:" << v.parent << ", cs:" << v.childs;
			return os;
		}
#endif
	};

	int n; // ノードの数
	int rt; // 根
	vector<Node> v; // 頂点

	// 文字列 s[0..n) で初期化する．
	Run_length_tree(const string& s) : n(0), rt(0), v(1) {
		// verify : https://paiza.jp/career/challenges/43/retry
		
		v[0].l = 0;
		stack<int> stk;
		stk.push(0);

		stack<ll> muls; // 文字列の反復回数
		ll len = 0; // 復号後の文字列の長さ
		int i = 0; // s の何文字目を読んでいるか
		while (i < sz(s)) {
			if (isdigit(s[i])) {
				v.push_back(Node()); n++;
				int p = stk.top();
				v[n].parent = p;
				v[n].l = len;
				v[p].childs.push_back(n);

				ll mul = 0;
				while (isdigit(s[i])) mul = mul * 10 + (s[i++] - '0');
				v[n].mul = mul;

				if (s[i] == '(') {
					stk.push(n);
				}
				else {
					v[n].c = s[i];
					v[n].r = len + 1;
					len += mul;
				}
			}
			else if (s[i] == ')') {
				int p = stk.top(); stk.pop();
				v[p].r = len;
				len = v[p].l + (v[p].r - v[p].l) * v[p].mul;
			}
			else {
				v.push_back(Node()); n++;
				int p = stk.top();
				v[n].parent = p;
				v[n].l = len;
				v[p].childs.push_back(n);

				v[n].c = s[i];
				len += 1;
				v[n].r = len;
			}

			i++;
		}

		v[0].r = len;
		n++;
	}
	Run_length_tree() : n(0), rt(-1) {} // ダミー

	// アクセス
	Node const& operator[](int i) const { return v[i]; }
	Node& operator[](int i) { return v[i]; }

	// 大きさ
	int size() const { return n; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Run_length_tree& rlt) {
		rep(i, sz(rlt)) os << i << ": " << rlt[i] << endl;
		return os;
	}
#endif
};


