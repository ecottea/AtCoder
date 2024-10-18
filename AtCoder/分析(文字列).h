#pragma once
#include "header.h"
// ■■■■■ 文字列の各種性質の分析 ■■■■■


//【suffix tree】
/*
* Suffix_tree<STR>(STR s) : O(n)
*	文字列 s[0..n) で初期化する．
*
* ll count_unique() : O(n)
*	s に含まれる非空な部分文字列の種類数を返す．
*
* pii search(STR p) : O(m log σ) ?
*   s[sa[i]..sa[i]+m) = p[0..m) なる i の範囲 i∈[il..ir) を返す（存在しなければ {-1, -1}）
*
* dfs_all(FUNC f) : O(n β)
*	辞書順で昇順に全ブロック [x1..x2)×[y1..y2) に対し f(x1, x2, y1, y2) を呼び出す．
*	ブロック [x1..x2)×[y1..y2) は sa[x1..x2) の [y1..y2) 文字目に対応する．
*
* build_dictionary() : O(n)
*	辞書機能を有効にする．
*
* pii get(ll k) : O(log n)
*	空文字列を除き辞書順で k 番目の部分文字列が s[l..r) であるとき {l, r} を返す．
*	なければ {-1, -1} を返す．build_dictionary() を先に呼び出すこと．
*
* pii get_unique(ll k) : O(log n)
*	空文字列を除き辞書順で k 番目のユニークな部分文字列が s[l..r) であるとき {l, r} を返す．
*	なければ {-1, -1} を返す．build_dictionary() を先に呼び出すこと．
*/
template <class STR>
class Suffix_tree {
	struct Node {
		int x1, x2; // sa[x1..x2) に対応するブロックであることを表す
		int y1, y2; // [y1..y2) 文字目に対応するブロックであることを表す
		vi ch; // 子の昇順リスト

		Node(int x1, int x2, int y1, int y2) : x1(x1), x2(x2), y1(y1), y2(y2) {}
		Node() : x1(-1), x2(-1), y1(-1), y2(-1) {};

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "x:[" << v.x1 << "," << v.x2 << ") "
				<< "y:[" << v.y1 << "," << v.y2 << ") "
				<< "ch:" << v.ch;
			return os;
		}
#endif
	};

	// n : 文字列の長さ
	int n;

	// s : 元の文字列
	STR s;

	// g : s の接尾辞木（0 はダミーの根）
	vector<Node> g;

	// id[i] : 辞書順で昇順 i 番目（ダミー含む）の頂点
	vi id;

	// acc[i] : 頂点 id[0..i) に対応する部分文字列の個数
	vl acc;

	// acc_uniq[i] : 頂点 id[0..i) に対応する部分文字列の種類数
	vl acc_uniq;

	// 部分木 i から辞書データを構築する．
	void build_dictionary_dfs(int i) {
		id.emplace_back(i);

		ll dx = g[i].x2 - g[i].x1;
		ll dy = g[i].y2 - g[i].y1;
		acc.emplace_back(acc.back() + dx * dy);
		acc_uniq.emplace_back(acc_uniq.back() + dy);

		repe(ni, g[i].ch) build_dictionary_dfs(ni);
	}

public:
	// sa[x] : s の接尾辞のうち辞書順 x 番目のものの先頭位置
	vi sa;

	// 文字列 s[0..n) で初期化する．
	Suffix_tree(const STR& s) : n(sz(s)), s(s) {
		// verify : https://atcoder.jp/contests/abc362/tasks/abc362_g

		// 自前で書くのは大変そうなので，ACL の SA と LCP を利用する．
		sa = suffix_array(s);

		auto lcp = lcp_array(s, sa);
		lcp.insert(lcp.begin(), 0);
		lcp.push_back(0);

		// ダミーの根
		g.emplace_back(0, n, 0, 0);

		// stk : 未完成のノードを溜めておくスタック
		stack<int> stk;
		stk.push(0);

		// デカルト木を組み上げるような感じで suffix tree を構築する．
		repi(x, 0, n) {
			int i = -1;
			while (!stk.empty()) {
				i = stk.top();

				if (g[i].y2 <= lcp[x]) break;

				if (g[i].y1 < lcp[x]) {
					g.emplace_back(g[i].x1, x, lcp[x], g[i].y2);
					g.back().ch = move(g[i].ch);
					g[i].ch.push_back(sz(g) - 1);
					g[i].y2 = lcp[x];
				}
				else {
					stk.pop();
					g[i].x2 = x;
				}
			}

			if (x < n) {
				g[i].ch.push_back(sz(g));
				stk.push(sz(g));
				g.emplace_back(x, -1, lcp[x], n - sa[x]);
			}
		}
	}

	// s に含まれる非空な部分文字列の種類数を返す．
	ll count_unique() const {
		// verify : https://judge.yosupo.jp/problem/number_of_substrings

		ll res = 0;
		repe(v, g) res += v.y2 - v.y1;
		return res;
	}

	// s[sa[i]..sa[i]+m) = p[0..m) なる i の範囲 i∈[il..ir) を返す（存在しなければ {-1, -1}）
	pii search(const STR& p) {
		// verify : https://codeforces.com/contest/1202/problem/E

		int m = sz(p);
		Assert(m > 0);

		int i = 0; int j = 0;

		// どの子に進めばいいかを毎回二分探索する．
		while (true) {
			int ok = 0, ng = sz(g[i].ch);
			if (ng == 0) return { -1, -1 };

			while (ng - ok > 1) {
				int mid = (ok + ng) / 2;

				int ic = g[i].ch[mid];
				if (p[j] >= s[sa[g[ic].x1] + g[ic].y1]) ok = mid;
				else ng = mid;
			}

			i = g[i].ch[ok];

			int ws = g[i].y2 - g[i].y1;
			int wp = m - j;
			rep(d, min(ws, wp)) if (s[sa[g[i].x1] + g[i].y1 + d] != p[j + d]) return { -1, -1 };

			if (wp <= ws) return { g[i].x1, g[i].x2 };

			j += ws;
		}
	}

	// 辞書順で昇順に全ブロック [x1..x2)×[y1..y2) に対し f(x1, x2, y1, y2) を呼び出す．
	template <class FUNC>
	void dfs_all(const FUNC& f, int i = 0) const {
		// verify : https://atcoder.jp/contests/abc280/tasks/abc280_h

		if (i != 0) f(g[i].x1, g[i].x2, g[i].y1, g[i].y2);

		repe(ni, g[i].ch) dfs_all(f, ni);
	}

	// 辞書機能を有効にする．
	void build_dictionary() {
		// verify : https://yukicoder.me/problems/no/2361

		id.reserve(sz(g));

		acc.reserve(sz(g) + 1);
		acc.emplace_back(0);

		acc_uniq.reserve(sz(g) + 1);
		acc_uniq.emplace_back(0);

		build_dictionary_dfs(0);
	}

	// 空文字列を除き辞書順で k 番目の部分文字列が s[l..r) であるとき {l, r} を返す（なければ {-1, -1} を返す）
	pii get(ll k) const {
		// verify : https://yukicoder.me/problems/no/2361

		Assert(!acc.empty());

		if (k < 0 || k >= acc.back()) return { -1, -1 };

		int i = ubpos(acc, k) - 1;
		int v = id[i];
		k -= acc[i];
		int dx = g[v].x2 - g[v].x1;
		return { sa[g[v].x1], sa[g[v].x1] + g[v].y1 + (int)k / dx + 1 };
	}

	// 空文字列を除き辞書順で k 番目のユニークな部分文字列が s[l..r) であるとき {l, r} を返す．（なければ {-1, -1} を返す）
	pii get_unique(ll k) const {
		// verify : https://atcoder.jp/contests/arc097/tasks/arc097_a

		Assert(!acc_uniq.empty());

		if (k < 0 || k >= acc_uniq.back()) return { -1, -1 };

		int i = ubpos(acc_uniq, k) - 1;
		int v = id[i];
		k -= acc_uniq[i];
		int dx = g[v].x2 - g[v].x1;
		return { sa[g[v].x1], sa[g[v].x1] + g[v].y1 + (int)k / dx + 1 };
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Suffix_tree& S) {
		os << "[suffixes]:" << endl;
		rep(i, S.n) {
			os << right << setw(2) << S.sa[i] << ": ";
			repi(j, S.sa[i], sz(S.s) - 1) os << S.s[j];
			os << endl;
		}

		os << "[blocks]:" << endl;
		auto f = [&](int x1, int x2, int y1, int y2) {
			os << "x:[" << x1 << "," << x2 << ") ";
			os << "y:[" << y1 << "," << y2 << ") ";
			repi(j, S.sa[x1] + y1, S.sa[x1] + y2 - 1) os << S.s[j];
			os << endl;
		};
		S.dfs_all(f);
		return os;
	}
#endif
};


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
* またそれぞれの開始位置を ls, lt に格納する．
*/
template <class STR>
int longest_common_substring(const STR& s, const STR& t, int* ls = nullptr, int* lt = nullptr) {
	// verify : https://judge.yosupo.jp/problem/longest_common_substring

	int n = sz(s), m = sz(t);

	if (ls != nullptr) *ls = *lt = 0;

	STR st(s);
	st.push_back('$');
	repe(c, t) st.push_back(c);

	auto sa = suffix_array(st);
	auto lcp = lcp_array(st, sa);

	int res = 0;
	rep(i, n + m) {
		if ((sa[i] < n) == (sa[i + 1] < n)) continue;

		if (chmax(res, lcp[i]) && ls != nullptr) {
			if (sa[i] < n) {
				*ls = sa[i];
				*lt = sa[i + 1] - n - 1;
			}
			else {
				*ls = sa[i + 1];
				*lt = sa[i] - n - 1;
			}
		}
	}

	return res;
}


//【最小周期】O(n)
/*
* 与えられた列 s[0..n) に対し，各 i∈[1..n] について s[0..i) の最小周期を格納したリストを返す．
*/
template <class STR>
vi cycle_length(const STR& s) {
	// 参考 : https://snuke.hatenablog.com/entry/2015/04/05/184819
	// verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_f

	//【方法】
	// Morris-Pratt で s[0..n) の最長共通接頭尾辞長 len(0..n] を求める．
	// s[0..i) の最短周期長は i-len[i] で得られる．

	//【例】s = "abababcaa"
	//	i:			0 1 2 3 4 5 6 7 8 9
	//	s[i-1]:		  a b a b a b c a a
	//	len[i]:		- 0 0 1 2 3 4 0 1 1
	//	i-len[i]:	- 1 2 2 2 2 2 7 7 8

	// Morris-Pratt で最長共通接頭尾辞長を求める．
	int n = sz(s);
	vi len(n + 1);
	len[0] = -1;

	int j = -1;
	rep(i, n) {
		while (j >= 0 && s[i] != s[j]) j = len[j];
		len[i + 1] = ++j;
	}

	// 最小周期長に書き換える．
	len[0] = 0;
	repi(i, 1, n) len[i] = i - len[i];

	return len;
}


//【Lyndon 分解】O(n)
/*
* s[0..n) の Lyndon 分解が
*	s[0..n) = s[x[0]..x[1]) + s[x[1]..x[2]) + ... + s[x[K-1]..x[K])
* である，すなわち
*	各 s[x[k]..x[k+1]) が Lyndon 文字列（任意の真の接尾辞より辞書順で小さい文字列）
*	s[x[0]..x[1]) ≧ s[x[1]..x[2]) ≧ ... ≧ s[x[K-1]..x[K])
* をみたすとし，区切り位置のリスト x[0..K] を返す．
*/
template <class STR>
vi lyndon_factorization(const STR& s) {
	// 参考 : https://qiita.com/nakashi18/items/66882bd6e0127174267a
	// verify : https://judge.yosupo.jp/problem/lyndon_factorization

	int n = sz(s);

	vi res;

	// s[l..r) : s[l..n) の最長 Lyndon 接頭辞
	int l = 0, r = 1;

	// s[i] : 次に見る文字
	//	s[l..i) は s[l..r) を周期的に繰り返した文字列になっている（余りも許す）
	int i = 1;

	while (i <= n) {
		dump(l, r);

		// c : 比較対象の文字
		auto c = s[l + (i - l) % (r - l)];

		// まだ周期的である場合
		if (i < n && s[i] == c) {
			// 最長 Lyndon 接頭辞は変わらない．
			i++;
		}
		// 辞書順で大きい文字によって周期的でなくなった場合
		else if (i < n && s[i] > c) {
			// s[l..i] が最長 Lyndon 接頭辞となる．
			r = i + 1;
			i++;
		}
		// 辞書順で小さい文字によって周期的でなくなった場合
		else {
			// s[l..n) の最長 Lyndon 接頭辞が s[l..r) に確定する．
			// 周期的な部分もまとめて確定できる．
			int w = r - l;
			while (r <= i) {
				res.push_back(l);
				l += w;
				r += w;
			}

			// 周期から漏れた部分は戻ってやり直し．
			r = l + 1;
			i = r;
		}
	}

	res.push_back(n);

	return res;
}


//【最長回文長（文字中心）】O(n)
/*
* s[0..n) の各 s[i] を中心とする最長回文の半径（(文字数+1)/2）を格納したリストを返す．
*/
template <class STR>
vi manacher(const STR& s) {
	// 参考 : https://snuke.hatenablog.com/entry/2014/12/02/235837
	// verify : https://judge.yosupo.jp/problem/enumerate_palindromes

	//【方法】
	// s[i] を中心とする最長回文の半径 j = r[i] が求まったとする．
	// すなわち s(i-j..i+j) が s[i] を中心とする最長回文である．
	// 
	// 各 k = [1..j) について，s[i-k] を中心とする最長回文 s(i-k-r[i-k]..i-k+r[i-k]) が
	// s(i-j+1..i+j-1) の部分文字列であれば，s[i±j] の影響を受けず
	// s[i] についての左右対称性より r[i+k] = r[i-k] と定まる．
	// その条件は，左端を比較して
	//		i - k - r[i-k] ≧ i - j + 1
	//		⇔ k + r[i-k] < j
	// である．このような結果の使い回しができる限り k を進め，次の i を i + k にする．
	// 
	// 使い回しができなくなったということは，s[i+k] を中心とする最長回文
	// s(i+k-r[i+k]..i+k+r[i+k]) が s(i-j+1..i+j-1) の部分文字列でないので，
	// 右端を比較することで
	//		i + k + r[i+k] > i + j - 1
	//		⇔ r[i+k] ≧ j - k
	// である．よって次の j は j - k にすればよい．

	int n = sz(s);
	vi r(n);

	// i : 中心位置，j : 暫定の半径
	int i = 0, j = 0;

	while (i < n) {
		// 回文である限り半径を大きくする．
		while (i - j >= 0 && i + j < n && s[i - j] == s[i + j]) j++;
		r[i] = j;

		// s[i] を中心とする回文に真に含まれている限り，左側での結果をコピーする．
		int di = 1;
		while (i - di >= 0 && di + r[i - di] < j) {
			r[i + di] = r[i - di];
			di++;
		}

		// 次は i + di から調べれば良い．半径についても j - di 以上は保証されている．
		i += di;
		j -= di;
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

	vi r = manacher(s_riffled);

	rep(i, n) lo[i] = r[2 * i + 1] - 1;
	rep(i, n - 1) le[i] = r[2 * (i + 1)] - 1;
}


//【最長回文接頭辞クエリ】
/*
* Longest_palindrome_suffix(string s) : O(n log n)
*	文字列 s[0..n) で初期化する．
*
* int get(int l, int r) : O(log(r-l))
*	s[l..r) の最長回文接頭辞が s[l..m) のとき m を返す．
*
* 利用：【最長回文長（文字中心）】
*/
int op_lps(int a, int b) { return max(a, b); }
int e_lps() { return -INF; }
class Longest_palindrome_suffix {
	int n;

	using SEG = segtree<int, op_lps, e_lps>;
	SEG seg;

public:
	// 文字列 s[0..n) で初期化する．
	Longest_palindrome_suffix(const string& s) : n(sz(s)) {
		// verify : https://yukicoder.me/problems/no/2606

		string s_riffled;
		s_riffled.resize(2 * n + 1);
		rep(i, n) s_riffled[2 * i + 1] = s[i];
		rep(i, n + 1) s_riffled[2 * i] = '$'; // '$' は s に含まれない文字

		vi rad = manacher(s_riffled);
		rep(i, 2 * n + 1) rad[i] -= i;

		seg = SEG(rad);
	}

	// s[l..r) の最長回文接頭辞が s[l..m) のとき m を返す．
	int get(int l0, int r0) {
		// verify : https://yukicoder.me/problems/no/2606

		int l = 2 * l0 + 1;
		int r = 2 * r0 + 1;
		int m = seg.min_left((l + r) / 2, [&](int x) {return x < 1 - l; });

		return l0 + (m - l);
	}
};


//【回文の種類数】
/*
* s[0..n) に部分文字列として現れる回文は高々 n 種類である．
* 
* verify : https://atcoder.jp/contests/abc237/tasks/abc237_h
*/


//【回文を連結した回文】
/*
* 回文 s, t を連結した s+t が回文であるとき，g = gcd(|s|, |t|) とおくと，
* s, t は共に周期 g をもち，その 1 周期も回文となる．
* 
* verify : https://atcoder.jp/contests/arc048/tasks/arc048_c
*/


//【回文を連結した周期的文字列】
/*
* 以下 0 文字以上の回文を単に回文，1 文字以上の回文を真の回文と呼ぶことにする．
* 
* 最小周期 T をもつ文字列 s[0..nT) が (回文)+(回文) の形で書けるとき，
*	s[0..T) も (回文)+(回文) の形で書ける
*	s[0..nT) を (回文)+(真の回文) の形に分ける方法は n 通り
* 
* verify : https://mofecoder.com/contests/yurufuwa_onsite_06/tasks/yurufuwa_onsite_06_g
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
* 文字列 s[0..n) について，s[0..i] と s の最長共通接尾辞の長さを z[i] に格納し z を返す．
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
* 任意文字とマッチする文字 Q および英小文字からなる文字列 s[0..n+m-1), p[0..m) について，
* s[i..i+m) = p[0..m) となる i を昇順に格納したリストを返す．
*
* 制約：m ≦ 1.48×10^6
*/
vi wildcard_matching(const string& s, const string& p, char Q = '?') {
	// 参考 : https://ei1333.hateblo.jp/entry/2021/01/02/000716
	// verify : https://judge.yosupo.jp/problem/wildcard_pattern_matching

	//【方法】
	// 文字列 s[0..n) を元に数列 sa[0..n), sb[0..n) を
	//		sa[i] = 0,              sb[i] = 0  (s[i]  = Q のとき)
	//		sa[i] = s[i] - 'a' + 1, sb[i] = 1  (s[i] != Q のとき)
	// と定め，p[0..m) についても同様に pa, pb を定める．
	//
	// このとき文字列の一致は
	//		s[i..i+m) = p[0..m)
	//		⇔ Σk∈[0..m) sb[i+k] pb[k] (sa[i+k] - pa[k])^2 = 0
	// と言い換えられる．左辺の値は大きくとも
	//		m * 1 * 1 * (26 - 0)^2 = m * 998255452 / 1.48×10^6
	// なので，m ≦ 1.48×10^6 であれば mint で計算しても問題ない．
	//
	// Σ の中身は
	//		sb[i+k] pb[k] (sa[i+k] - pa[k])^2
	//		= sb[i+k] pb[k] sa[i+k]^2 - 2 sb[i+k] pb[k] sa[i+k] pa[k] + sb[i+k] pb[k] pa[k]^2
	//		= (sa[i+k]^2 sb[i+k]) pb[k] - 2 (sa[i+k] sb[i+k])(pa[k] pb[k]) + sb[i+k] (pa[k]^2 pb[k])
	// と書き直すことができる．よって，
	//		sab[i] = sa[i] sb[i]
	//		saab[i] = sa[i]^2 sb[i]
	// などとおき，p をあらかじめ逆順にしていたことにすれば，middle product で一括計算することができる．

	using mint = modint998244353;
	using vm = vector<mint>;

	int m = sz(p), n = sz(s) - m + 1;

	int W = 1 << (msb(n + m - 2) + 1);

	vm sb(W), sab(W), saab(W);
	rep(i, n + m - 1) {
		mint a = (s[i] == Q ? 0 : s[i] - 'a' + 1);
		mint b = (s[i] == Q ? 0 : 1);

		sb[i] = b;
		sab[i] = a * b;
		saab[i] = a * sab[i];
	}

	vm pb(W), pab(W), paab(W);
	rep(j, m) {
		mint a = (p[m - 1 - j] == Q ? 0 : p[m - 1 - j] - 'a' + 1);
		mint b = (p[m - 1 - j] == Q ? 0 : 1);

		pb[j] = b;
		pab[j] = a * b;
		paab[j] = a * pab[j];
	}

	internal::butterfly(saab);
	internal::butterfly(pb);
	rep(i, W) saab[i] *= pb[i];
	internal::butterfly_inv(saab);

	internal::butterfly(sab);
	internal::butterfly(pab);
	rep(i, W) sab[i] *= pab[i];
	internal::butterfly_inv(sab);

	internal::butterfly(sb);
	internal::butterfly(paab);
	rep(i, W) sb[i] *= paab[i];
	internal::butterfly_inv(sb);

	vi pos;
	repi(i, m - 1, n + m - 2) {
		mint val = saab[i] - 2 * sab[i] + sb[i];
		if (val == 0) pos.push_back(i - m + 1);
	}

	return pos;
}


//【複数文字列検索】
/*
* Aho_corasick(vector<string> pats) : O(Σ|pats|)
*	パターン文字列の集合 pats を検索できるよう初期化する．
*
* find(string s, vb& ex) : O(|s| + Σ|pats|)
*	文字列 s 中に各 pats[i] が存在するかを ex[i] に格納する．
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

			// p を通過したことを記録しておく．
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


//【Suffix Array と LCP Array】
/*
* ACL の sa = suffix_array(s), lcp = lcp_array(s, sa) を使えば良い．
* 
* 使用例：
* s[0..11) = "mississippi"
*  sa[0]: "i"
* lcp[0]: 1-
*  sa[1]: "ippi"
* lcp[1]: 1-
*  sa[2]: "issippi"
* lcp[2]: 4----
*  sa[3]: "ississippi"
* lcp[3]: 0
*  sa[4]: "mississippi"
* lcp[4]: 0
*  sa[5]: "pi"
* lcp[5]: 1-
*  sa[6]: "ppi"
* lcp[6]: 0
*  sa[7]: "sippi"
* lcp[7]: 2--
*  sa[8]: "sissippi"
* lcp[8]: 1-
*  sa[9]: "ssippi"
* lcp[9]: 3---
* sa[10]: "ssissippi"
*/


//【部分文字列判定（複数回）】（旧）
/*
* String_search(string s) : O(|s|)
*   文字列 s に対する検索ができるように初期化する．
*
* int search(string p) : O(|p| log |s|)
*   s[i..i+m) = p[0..m) なる i を 1 つ返す（存在しなければ -1）
*
* int count(string p) : O(|p| log |s|)
*   s[i..i+m) = p[0..m) なる i の個数を返す．
*/
class String_search {
	int n;
	string s;
	vi sa; // s の接尾辞配列

public:
	// 文字列 s に対する検索ができるように初期化する．
	String_search(const string& s) : n(sz(s)), s(s) {
		sa = suffix_array(s);
	}

	// s[i..i+m) = p[0..m) なる i を返す（存在しなければ -1）
	int search(const string& p) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_14_D

		int m = sz(p);

		// s[sa[ok]..sa[ok]+m) ≦ p < s[sa[ng]..sa[ng]+m)
		int ok = 0, ng = n;

		// i_ok : p と s[sa[ok]] の接頭辞が最長何文字一致しているか
		// i_ng : p と s[sa[ng]] の接頭辞が最長何文字一致しているか
		int i_ok = 0, i_ng = 0;
		while (i_ok < m && sa[ok] + i_ok < n && s[sa[ok] + i_ok] == p[i_ok]) i_ok++;
		if (i_ok == m) return sa[ok];

		while (abs(ok - ng) > 1) {
			int mid = (ok + ng) / 2;

			// i_mid : p と s[sa[mid]] の接頭辞が最長何文字一致しているか
			int i_mid = min(i_ok, i_ng);
			while (i_mid < m && sa[mid] + i_mid < n && s[sa[mid] + i_mid] == p[i_mid]) i_mid++;

			// s[sa[mid]..sa[mid]+m) = p の場合
			if (i_mid == m) {
				ok = mid;
				i_ok = i_mid;
				break;
			}

			// s[sa[mid]..sa[mid]+m) < p の場合
			if (sa[mid] + i_mid == n || s[sa[mid] + i_mid] < p[i_mid]) {
				ok = mid;
				i_ok = i_mid;
			}
			// s[sa[mid]..sa[mid]+m) > p の場合
			else {
				ng = mid;
				i_ng = i_mid;
			}
		}

		if (i_ok == m) return sa[ok];
		else return -1;
	}

	// s[i..i+m) = p[0..m) なる i の個数を返す．
	int count(const string& p) {
		// verify : https://atcoder.jp/contests/abc362/tasks/abc362_g

		int m = sz(p);

		int cnt = 0;

		{
			// s[sa[ok]..sa[ok]+m) ≦ p < s[sa[ng]..sa[ng]+m)
			int ok = -1, ng = n;

			// i_ok : p と s[sa[ok]] の接頭辞が最長何文字一致しているか
			// i_ng : p と s[sa[ng]] の接頭辞が最長何文字一致しているか
			int i_ok = 0, i_ng = 0;

			while (abs(ok - ng) > 1) {
				int mid = (ok + ng) / 2;

				// i_mid : p と s[sa[mid]] の接頭辞が最長何文字一致しているか
				int i_mid = min(i_ok, i_ng);
				while (i_mid < m && sa[mid] + i_mid < n && s[sa[mid] + i_mid] == p[i_mid]) i_mid++;

				if (i_mid == m || sa[mid] + i_mid == n || s[sa[mid] + i_mid] < p[i_mid]) {
					ok = mid;
					i_ok = i_mid;
				}
				else {
					ng = mid;
					i_ng = i_mid;
				}
			}

			cnt += ng;
		}

		{
			// s[sa[ok]..sa[ok]+m) < p ≦ s[sa[ng]..sa[ng]+m)
			int ok = -1, ng = n;

			// i_ok : p と s[sa[ok]] の接頭辞が最長何文字一致しているか
			// i_ng : p と s[sa[ng]] の接頭辞が最長何文字一致しているか
			int i_ok = 0, i_ng = 0;

			while (abs(ok - ng) > 1) {
				int mid = (ok + ng) / 2;

				// i_mid : p と s[sa[mid]] の接頭辞が最長何文字一致しているか
				int i_mid = min(i_ok, i_ng);
				while (i_mid < m && sa[mid] + i_mid < n && s[sa[mid] + i_mid] == p[i_mid]) i_mid++;

				if (i_mid != m && (sa[mid] + i_mid == n || s[sa[mid] + i_mid] < p[i_mid])) {
					ok = mid;
					i_ok = i_mid;
				}
				else {
					ng = mid;
					i_ng = i_mid;
				}
			}

			cnt -= ng;
		}

		return cnt;
	}
};


