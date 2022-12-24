#pragma once
#include "header.h"
// ■■■■■ 文字列の各種性質の分析 ■■■■■


//【最長共通接頭尾辞】O(n)
/*
* 文字列 s[0..n) について，s[0..i) の接頭辞と接尾辞が
* 最大何文字一致しているか（i 文字未満）を len[i] に格納し len を返す．
*/
template <class STR>
vi morris_pratt(const STR& s) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/01/235807
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_B

	//【方法】
	// j = len[i] まで分かっているときに len[i+1] を求めることを考える．
	// len[i] = j なので，
	//		s[0..j) = s[i-j..i)
	// であり，これが最長一致である．
	// 
	// case1: s[j] = s[i] のときは，
	//		s[0..j+1) = s[(i+1)-(j+1)..i+1)
	// となり len[i+1] = j+1 と求まる．
	// 
	// case2: s[j] != s[i] のときは，
	//		s[0..k) = s[i-k..i) かつ s[k] = s[i]
	// なる最大の k < j を見つけることができれば len[i] = k+1 と求まる．
	// 
	// s[0..k) は s[0..j) の接頭辞であり，
	// s[i-k..i) は s[i-j..i) = s[0..j) の接尾辞である．
	// よって len[j] の定め方より k <= len[j] が必要である．
	// 
	// もし s[len[j]] = s[i] なら k = len[j] と選べばよく，そうでなければ
	// j ← len[j] として同じ操作を繰り返せば良い．

	//【例】
	// i:		0 1 2 3 4 5 6 7 8 9
	// s[i-1] :   a b a b a b c a a
	// len[i] : - 0 0 1 2 3 4 0 1 1

	int n = sz(s);
	vi len(n + 1);
	len[0] = -1;

	int j = -1;
	rep(i, n) {
		while (j >= 0 && s[i] != s[j]) j = len[j];
		len[i + 1] = ++j;
	}

	return len;
}


//【部分文字列判定】O(n + m)
/*
* s[0..n) の部分文字列として w[0..m) が含まれているかどうか調べ，
* 見つかった場所の先頭位置を昇順に格納したリストを返す．
*
* 利用：【最長共通接頭尾辞】
*/
template <class STR>
vi knuth_morris_pratt(const STR& s, const STR& w) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_B

	int n = sz(s), m = sz(w);
	vi pos;

	// l[j] : w[0..j) の接頭辞と接尾辞が最大何文字一致しているか（j 文字未満）
	vi l = morris_pratt(w);

	int i = 0; // s[i..i+m) を走査中
	int j = 0; // s[i..i+j) = w[0..j) まで確定
	while (i + j < n) {
		// s[i+j] = w[j] の場合
		if (w[j] == s[i + j]) {
			// さらに 1 文字先を見に行く．
			j++;

			// もし w を走査し終えたなら連続部分列として w を発見できたので記録する．
			if (j == m) pos.push_back(i);
			else continue;
		}

		// s[i..i+j) までは走査したので，次は s[i+j..i+j+m) の走査といきたい.
		// しかし，w[0..j) の最大長 l[j] の接尾辞は w[0..j) の接頭辞に一致していた．
		// s[i..i+j) = w[0..j) だから，これは s[i..i+j) の接尾辞とも一致している．
		// よって s[i+j-l[j]..i+j-l[j]+m) = w[0..m) となる可能性は残っている．
		i = i + j - l[j];

		// 一方で，これは s[i+j-l[j]..i+j-l[j]+m) の長さ l[j] の接頭辞が
		// w[0..j) の接頭辞に一致していることを意味するので，
		// s[i+j-l[j]..i+j) = w[0..l[j]) までは確定していることになる． 
		if (j > 0) j = l[j];
	}

	return pos;
}


//【最長共通部分文字列】O(n + m)
/*
* 2 つの文字列 s[0..n), t[0..m) の最長共通部分文字列の長さを返す．
*/
template <class STR>
int longest_common_substring(const STR& s, const STR& t) {
	// verify : https://atcoder.jp/contests/arc151/tasks/arc151_e

	int n = sz(s), m = sz(t);

	STR st(s);
	st.push_back('$');
	repe(c, t) st.push_back(c);

	auto sa = suffix_array(st);
	auto lcp = lcp_array(st, sa);

	int res = 0;
	rep(i, n + m) {
		if ((sa[i] < n) == (sa[i + 1] < n)) continue;

		chmax(res, lcp[i]);
	}

	return res;
}


//【最短周期長】
/*
* s[0..n) の最長共通接頭尾辞長 len(0..n] が【最長共通接頭尾辞】で得られていれば，
* s[0..i) の最短周期長は i-len[i] で得られる．
* 
* 例：
*	i:			0 1 2 3 4 5 6 7 8 9
*	s[i-1]:		  a b a b a b c a a
*	len[i]:		- 0 0 1 2 3 4 0 1 1
*	i-len[i]:	- 1 2 2 2 2 2 7 7 8
* 
* 参考 : https://snuke.hatenablog.com/entry/2015/04/05/184819
*/


//【最長回文長（文字中心）】O(n)
/*
* s[0..n) の s[i] を中心とする最長回文の半径（(文字数 + 1) / 2）を r[i] に格納し r を返す．
* ここで回文の半径とは，(文字数 + 1) / 2 を意味する．
*/
template <class STR>
vi manacher(const STR& s) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/02/235837

	//【方法】
	// s[i] を中心とする最長回文の半径 j = r[i] が愚直に求まったとする．
	// すなわち s(i-j..i+j) が s[i] を中心とする最長回文である．
	// 
	// 各 k = [1..j) について，s[i-k] を中心とする最長回文 s(i-k-r[i-k]..i-k+r[i-k]) が
	// s(i-j+1..i+j-1) の部分文字列であれば，s[i±j] の影響を受けず
	// s[i] についての左右対称性より r[i+k] = r[i-k] と定まる．
	// その条件は，左端を比較して
	//		i - k - r[i-k] >= i - j + 1
	//		⇔ k + r[i-k] < j
	// である．このような結果の使い回しができる限り k を進め，次の i を i + k にする．
	// 
	// 使い回しができなくなったということは，s[i+k] を中心とする最長回文
	// s(i+k-r[i+k]..i+k+r[i+k]) が s(i-j+1..i+j-1) の部分文字列でないので，
	// 右端を比較することで
	//		i + k + r[i+k] > i + j - 1
	//		⇔ r[i+k] >= j - k
	// である．よって次の j は j - k にすればよい．

	int n = sz(s);
	vi r(n);

	int i = 0, j = 0;
	while (i < n) {
		while (i - j >= 0 && i + j < n && s[i - j] == s[i + j]) j++;
		r[i] = j;

		int k = 1;
		while (i - k >= 0 && k + r[i - k] < j) {
			r[i + k] = r[i - k];
			k++;
		}
		i += k;
		j -= k;
	}

	return r;
}


//【最長回文長】O(n)
/*
* s[0..n) の s[i] を中心とする最長回文の長さを lo[i] に格納し，
* s[i..i+1] を中心とする最長回文の長さを le[i] に格納する．
*
* 利用：【最長回文長（文字中心）】
*/
template <class STR>
void manacher(const STR& s, vi& lo, vi& le) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/02/235837
	// verify : https://judge.yosupo.jp/problem/enumerate_palindromes

	int n = sz(s);
	lo.resize(n);
	le.resize(n - 1);

	STR s_riffled;
	s_riffled.resize(2 * n + 1);
	rep(i, n) s_riffled[2 * i + 1] = s[i];
	rep(i, n + 1) s_riffled[2 * i] = '$'; // '$' は s に含まれない文字

	vi r;
	manacher(s_riffled, r);

	rep(i, n) lo[i] = r[2 * i + 1] - 1;
	rep(i, n - 1) le[i] = r[2 * (i + 1)] - 1;
}


//【回文を連結した回文】
/*
* 回文 s, t を連結した s+t が回文であるとき，g = gcd(|s|, |t|) とおくと，
* s, t は共に周期 g をもち，その 1 周期も回文となる．
* 
* verify : https://atcoder.jp/contests/arc048/tasks/arc048_c
*/


//【Z アルゴリズム】O(n)
/*
* 文字列 s[0..n) について，s[i..n) と s の最長共通接頭辞の長さを z[i] に格納し z を返す．
*/
template <class STR>
vi z_algo(const STR& s) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/03/214243
	// verify : https://judge.yosupo.jp/problem/zalgorithm

	//【方法】
	// j = z[i] > 0 が愚直に求まったとする．
	// すなわち s[0..j) = s[i..i+j) かつ s[j] != s[i+j] である．
	// 
	// 各 k = [1..j) について，s と s[k..n) の最長共通接頭辞 s[k..k+z[k]) が
	// s[0..j-1) の部分文字列であれば，s[j] と s[i+j] の影響を受けず
	// s[0..j) = s[i..i+j) より z[i+k] = z[k] と定まる．
	// その条件は，これらの右端を比較して
	//		k + z[k] <= j - 1
	//		⇔ k + z[k] < j
	// である．このような結果の使い回しができる限り k を進め，次の i を i + k にする．
	//  
	// 使い回しができなくなったということは，s[k..k+z[k]) が s[0..j-1) の
	// 部分文字列でないので，s[i+k..i+k+z[i+k]) も s[i..i+j-1) の
	// 部分文字列ではなく，これらの右端を比較することで
	//		i + k + z[i + k] > i + j - 1
	//		⇔ z[i + k] >= j - k
	// である．よって次の j は j - k にすればよい．

	//【例】
	// i:		0 1 2 3 4 5 6 7 8 9
	// s[i] :	a a a b a a a a b
	// z[i] :	9 2 1 0 3 4 2 1 0
	
	int n = sz(s);
	vi z(n);
	z[0] = n;

	int i = 1, j = 0;
	while (i < n) {
		while (i + j < n && s[j] == s[i + j]) j++;
		z[i] = j;

		if (j == 0) {
			i++;
			continue;
		}

		int k = 1;
		while (i + k < n && k + z[k] < j) {
			z[i + k] = z[k];
			k++;
		}
		i += k;
		j -= k;
	}

	return z;
}


//【Z アルゴリズム（接尾辞）】O(n)
/*
* 文字列 s[0..n) について，s[0..i] と s の最長共通接尾辞の長さを z[i] にまとめて z を返す．
*/
template <class STR>
vi z_algorithm_suffix(STR s) {
	// verify : https://atcoder.jp/contests/arc055/tasks/arc055_c

	reverse(all(s));
	vi zs = z_algorithm(s);
	reverse(all(zs));
	return zs;
}


//【ワイルドカード付き文字列検索】O((n + m) log(n + m))
/*
* 任意文字とマッチする文字 '?' および英小文字からなる文字列 s[0..n), p[0..m) について，
* s[i..i+m) = p[0..m) となる i を昇順に格納したリストを返す．
*/
vi wildcard_matching(const string& s, const string& p) {
	// 参考 : https://ei1333.hateblo.jp/entry/2021/01/02/000716
	// verify : https://atcoder.jp/contests/panasonic2020/tasks/panasonic2020_e

	//【方法】
	// 文字列 s[0..n) を元に数列 sa[0..n), sb[0..n) を
	//		sa[i] = 0, sb[i] = 0  (s[i] = '?' のとき)
	//		sa[i] = s[i] - 'a' + 1, sb[i] = 1  (s[i] != '?' のとき)
	// と定め，p[0..m) についても同様に pa, pb を定める．
	//
	// このとき文字列の一致は
	//		s[i..i+m) = p[0..m) ⇔ Σk∈[0..m) sb[i+k] pb[k] (sa[i+k] - pa[k])^2 = 0
	// と言い換えられる．
	//
	// Σ の中身は
	//		sb[i+k] pb[k] (sa[i+k] - pa[k])^2
	//		= sb[i+k] pb[k] sa[i+k]^2 - 2 sb[i+k] pb[k] sa[i+k] pa[k] + sb[i+k] pb[k] pa[k]^2
	//		= (sa[i+k]^2 sb[i+k]) pb[k] - 2 (sa[i+k] sb[i+k])(pa[k] pb[k]) + sb[i+k] (pa[k]^2 pb[k])
	// と書き直すことができる．よって，
	//		sab[i] = sa[i] sb[i]
	//		saab[i] = sa[i]^2 sb[i]
	// などとおき，p をあらかじめ逆順にしていたことにすれば，畳込みで一括計算することができる．

	using mint = modint998244353;
	using vm = vector<mint>;

	int n = sz(s), m = sz(p);
	vi pos;

	vm sb(n), sab(n), saab(n);
	rep(i, n) {
		mint a = (s[i] == '?' ? 0 : s[i] - 'a' + 1);
		mint b = (s[i] == '?' ? 0 : 1);

		sb[i] = b;
		sab[i] = a * b;
		saab[i] = a * a * b;
	}

	vm pb(m), pab(m), paab(m);
	rep(j, m) {
		mint a = (p[m - 1 - j] == '?' ? 0 : p[m - 1 - j] - 'a' + 1);
		mint b = (p[m - 1 - j] == '?' ? 0 : 1);

		pb[j] = b;
		pab[j] = a * b;
		paab[j] = a * a * b;
	}

	vm c1 = convolution(saab, pb);
	vm c2 = convolution(sab, pab);
	vm c3 = convolution(sb, paab);

	repi(i, m - 1, n - 1) {
		mint val = c1[i] - 2 * c2[i] + c3[i];
		if (val == 0) pos.push_back(i - m + 1);
	}

	return pos;
}


//【部分文字列判定（複数回）】
/*
* String_search(string s) : O(|s|)
*   文字列 s に対する検索ができるように初期化する．
*
* int search(string p) : O(|p| log |s|)
*   s[i..i+m) = p[0..m) なる i を返す（存在しなければ -1）
*/
class String_search {
	int n;
	string s;
	vi sa; // s の接尾辞配列

public:
	String_search(const string& s_) : n(sz(s_)), s(s_) {
		sa = suffix_array(s);
	}

	int search(const string& p) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_D

		int m = sz(p);

		int ok = 0, ng = n;
		int i_ok = 0, i_ng = 0;
		while (i_ok < m && sa[ok] + i_ok < n && s[sa[ok] + i_ok] == p[i_ok]) i_ok++;
		if (i_ok == m) return sa[ok];

		while (abs(ok - ng) > 1) {
			int mid = (ok + ng) / 2;
			int i_mid = min(i_ok, i_ng);

			while (i_mid < m && sa[mid] + i_mid < n && s[sa[mid] + i_mid] == p[i_mid]) i_mid++;

			if (i_mid == m) {
				ok = mid;
				i_ok = i_mid;
				break;
			}

			if (sa[mid] + i_mid == n || s[sa[mid] + i_mid] < p[i_mid]) {
				ok = mid;
				i_ok = i_mid;
			}
			else {
				ng = mid;
				i_ng = i_mid;
			}
		}

		if (i_ok == m) return sa[ok];
		else return -1;
	}
};


//【複数文字列検索】
/*
* Aho_corasick(vector<string> pats) : O(Σ|pats|)
*	パターン文字列の集合 pats を検索できるよう初期化する．
*
* find(string s, vb& ex) : O(|s| + Σ|pats|)
*	文字列 s 中に pats[i] が存在するかを ex[i] に格納する．
*	制約：1 度しか実行できない．
*/
class Aho_corasick {
	// 参考 : https://naoya-2.hatenadiary.org/entry/20090405/aho_corasick

	static const int C = 26; // 文字の種類数
	static const char A = 'a'; // 最初の文字

	struct Node {
		Node* parent; // 親へのポインタ
		vector<Node*> childs; // 子へのポインタ（C 分木）
		vi pat_ids; // この文字で終わるパターンの番号
		Node* suf; // 最長接尾辞へのポインタ
		bool seen; // 探索済か（find 用）

		Node(Node* parent_ = nullptr) : parent(parent_), childs(C), suf(nullptr), seen(false) {}
	};

	int n; // パターン数
	Node* root; // 根へのポインタ

	void create_trie_tree(const vector<string>& pats) {
		rep(i, n) {
			Node* p = root;

			// pats[i] の文字 c を先頭から順に見ていく
			repe(c, pats[i]) {
				// 未登録の文字だった場合は新たにノードを追加
				if (p->childs[c - A] == nullptr) {
					p->childs[c - A] = new Node(p);
				}

				p = p->childs[c - A];
			}
			p->pat_ids.push_back(i);
		}
	}

public:
	// パターン文字列の集合 pats を検索できるよう初期化する．
	Aho_corasick(const vector<string>& pats) : n(sz(pats)), root(new Node()) {
		// まずトライ木を構築する．
		create_trie_tree(pats);

		// BFS で最長接尾辞へのポインタを繋ぐ．
		queue<Node*> q;
		q.push(root);

		while (!q.empty()) {
			Node* p = q.front(); q.pop();

			// 各文字 c について
			rep(c, C) {
				// 子 c が居ない場合は何もしない．
				if (p->childs[c] == nullptr) continue;

				// p から最小回数の遷移失敗を繰り返してたどり着ける c 遷移が可能なノード pp を探す．
				Node* pp = p->suf;
				while (pp != nullptr && pp->childs[c] == nullptr) pp = pp->suf;

				// p から c で遷移し，次に遷移失敗した場合の行き先を定める．
				// pp が見つからなかったら空文字列まで戻る．
				if (pp == nullptr) p->childs[c]->suf = root;
				// さもなくば pp から c で遷移した先に移る．
				else p->childs[c]->suf = pp->childs[c];

				// 子についての処理を予約する．
				q.push(p->childs[c]);
			}
		}
	}

	// 文字列 s 中に pats[i] が存在するかを ex[i] に格納する（1 度しか実行できない）
	void find(const string& s, vb& ex) {
		ex.assign(n, false);

		// 通過した頂点を記録するキュー
		queue<Node*> q;

		// 対象文字列 s による遷移でトライ木上を移動する．
		Node* p = root;
		repe(c, s) {
			// c 遷移が可能なノード p まで戻る．
			while (p != nullptr && p->childs[c - A] == nullptr) p = p->suf;

			// c 遷移が不可能なら空文字列まで戻る．
			if (p == nullptr) p = root;
			// c 遷移が可能なら c で遷移した先に移る．
			else p = p->childs[c - A];

			// p 通過したことを記録しておく．
			q.push(p);
		}

		// 通過した頂点だけでなく，空文字列からそこに至るまでの頂点全てを調べる．
		while (!q.empty()) {
			Node* p = q.front(); q.pop();
			if (p == nullptr) continue;

			if (p->seen) continue;
			p->seen = true;

			repe(id, p->pat_ids) ex[id] = true;

			q.push(p->parent);
			q.push(p->suf);
		}
	}
};


//【複数文字列検索（文字が多種）】
/*
* Aho_corasick_large(vector<string> pats) : O(Σ|pats|)
*	パターン文字列の集合 pats を検索できるよう初期化する．
*
* find(string s, vb& ex) : O(|s| + Σ|pats|)
*	文字列 s 中に pats[i] が存在するかを ex[i] に格納する．
*	制約：1 度しか実行できない．
*/
class Aho_corasick_large {
	// 参考 : https://naoya-2.hatenadiary.org/entry/20090405/aho_corasick
	
	struct Node {
		Node* parent; // 親へのポインタ
		unordered_map<char, Node*> childs; // 子へのポインタ
		vi pat_ids; // この文字で終わるパターンの番号
		Node* suf; // 最長接尾辞へのポインタ
		bool seen; // 探索済か（find 用）

		Node(Node* parent_ = nullptr, int pat_id_ = -1, Node* suf_ = nullptr)
			: parent(parent_), suf(suf_), seen(false) {}
	};

	int n; // パターン数
	Node* root; // 根へのポインタ

	void create_trie_tree(const vector<string>& pats) {
		rep(i, n) {
			Node* p = root;

			// pats[i] の文字 c を先頭から順に見ていく
			repe(c, pats[i]) {
				// 未登録の文字だった場合は新たにノードを追加
				if (!p->childs.count(c)) {
					p->childs[c] = new Node(p);
				}

				p = p->childs[c];
			}
			p->pat_ids.push_back(i);
		}
	}

public:
	Aho_corasick_large(const vector<string>& pats) : n(sz(pats)), root(new Node()) {
		// まずトライ木を構築する．
		create_trie_tree(pats);

		// BFS で最長接尾辞へのポインタを繋ぐ．
		queue<Node*> q;
		q.push(root);

		while (!q.empty()) {
			Node* p = q.front(); q.pop();

			// 各文字 c について
			repe(ccp, p->childs) {
				char c; Node* cp;
				tie(c, cp) = ccp;

				Node* pp = p->suf;
				while (pp != nullptr && !pp->childs.count(c)) pp = pp->suf;

				if (pp == nullptr) cp->suf = root;
				else cp->suf = pp->childs[c];

				q.push(cp);
			}
		}
	}

	void find(const string& s, vb& ex) {
		ex.assign(n, false);

		queue<Node*> q;

		Node* p = root;
		repe(c, s) {
			while (p != nullptr && !p->childs.count(c)) p = p->suf;

			if (p == nullptr) p = root;
			else p = p->childs[c];

			q.push(p);
		}

		while (!q.empty()) {
			Node* p = q.front(); q.pop();
			if (p == nullptr) continue;

			if (p->seen) continue;
			p->seen = true;

			repe(id, p->pat_ids) ex[id] = true;

			q.push(p->parent);
			q.push(p->suf);
		}
	}
};


