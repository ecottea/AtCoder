#pragma once
#include "header.h"
// ■■■■■ 多倍長整数 ■■■■■


//【比較（文字列）】O(min(n, m))
/*
* b 進表記された非負整数 s[0..n), t[0..m) について，s[0..n) op t[0..m) かを返す．
* 比較演算子 op は，">", ">=", "=", "<=", "<" のいずれかとする．
*/
bool comp_bint(const string& s, const string& op, const string& t) {
	// verify : https://judge.yosupo.jp/problem/addition_of_big_integers

	int n = sz(s), m = sz(t);

	if (op[0] == '=') return s == t;

	if (op[0] == '<') {
		// 桁数の比較だけで大小関係が分かる場合（前 0 は無いものとする）
		if (n < m) return true;
		if (n > m) return false;

		// 桁数が同じときは単に辞書順比較したものと結果が一致する．
		return op == "<" ? (s < t) : (s <= t);
	}
	else {
		// 桁数の比較だけで大小関係が分かる場合（前 0 は無いものとする）
		if (n > m) return true;
		if (n < m) return false;

		// 桁数が同じときは単に辞書順比較したものと結果が一致する．
		return op == ">" ? (s > t) : (s >= t);
	}
}


//【加算（文字列）】O(max(n, m))
/*
* B 進表記された非負整数 s[0..n) と t[0..m) の和を返す．
*/
string add_bint(const string& s, const string& t, int B = 10) {
	// verify : https://judge.yosupo.jp/problem/addition_of_big_integers

	int i = sz(s) - 1, j = sz(t) - 1, c = 0;
	string res;
	res.reserve(max(i, j) + 2);

	while (i >= 0 || j >= 0 || c > 0) {
		int v = (i >= 0 ? (s[i] - '0') : 0) + (j >= 0 ? (t[j] - '0') : 0) + c;

		c = v / B;
		res.push_back('0' + (v % B));

		i--; j--;
	}

	reverse(all(res));

	return res;
}


//【減算（文字列）】O(max(n, m))
/*
* B 進表記された非負整数 s[0..n) から t[0..m) を引いた差を返す．
*
* 制約：s ≧ t
*/
string sub_bint(const string& s, const string& t, int B = 10) {
	// verify : https://judge.yosupo.jp/problem/addition_of_big_integers

	int i = sz(s) - 1, j = sz(t) - 1, c = 0;
	string res;
	res.reserve(max(i, j) + 1);

	while (i >= 0) {
		int vs = (int)(s[i] - '0') - c;
		int vt = j >= 0 ? (t[j] - '0') : 0;

		c = 0;
		if (vs < vt) {
			vs += B;
			c = 1;
		}

		res.push_back('0' + (vs - vt));

		i--; j--;
	}

	int k = sz(res) - 1;
	while (k >= 0 && res[k] == '0') {
		res.pop_back();
		k--;
	}

	if (k == -1) res.push_back('0');

	reverse(all(res));

	return res;
}


//【乗算（文字列）】O((n + m) log(n + m))
/*
* B 進表記で表された数 s[0..n) と t[0..m) の積を返す．
* 
* 制約：min(n,m)*(B-1)^2 < 998244353
*/
string mul_bint(const string& s, const string& t, int B = 10) {
	// verify : https://judge.yosupo.jp/problem/multiplication_of_big_integers

	if (s == "0" || t == "0") return "0";

	int n = sz(s), m = sz(t);

	vector<modint998244353> a(n), b(m);
	rep(i, n) a[i] = s[n - 1 - i] - '0';
	rep(j, m) b[j] = t[m - 1 - j] - '0';

	vector<modint998244353> cm = convolution(a, b);

	vi c(n + m - 1);
	rep(i, n + m - 1) c[i] = cm[i].val();

	int k = 0;
	for (; k < n + m - 2; k++) {
		c[k + 1] += c[k] / B;
		c[k] %= B;
	}
	while (c[k] >= B) {
		c.push_back(c[k] / B);
		c[k] %= B;
		k++;
	}

	string res;
	res.reserve(k);
	while (k >= 0) {
		res += (char)(c[k] + '0');
		k--;
	}

	return res;
}


//【除算（文字列）】O((n + m) log(n + m))
/*
* B 進表記された非負整数 s[0..n) を t[0..m) で割った商を返す．
*
* 利用：【加算（文字列）】,【減算（文字列）】,【乗算（文字列）】,【比較（文字列）】
*/
string div_bint(string s, string t, int B = 10) {
	// 参考 : https://qiita.com/square1001/items/1aa12e04934b6e749962
	// verify : https://judge.yosupo.jp/problem/division_of_big_integers

	Assert(t != "0");

	int n = sz(s), m = sz(t);

	// FPS の inv() を真似して書いてみた．ちゃんとは理解していない．
	string t_inv = "1"; int shift = m; string two = "2";
	for (int k = 1; k < 32 * (n + 3); k *= 2) { // 16 進のとき 32→64 にしないと WA した．
		int pshift = shift;
		int len = max(min(2 * k, n + 3), 1);
		string tmp;
		rep(i, min(len, m)) tmp += t[i];
		shift -= m - sz(tmp);
		tmp = mul_bint(tmp, t_inv, B);
		if (sz(tmp) > len) {
			shift -= sz(tmp) - len;
			tmp.resize(len);
		}
		while (sz(two) > shift + 1) two.pop_back();
		while (sz(two) < shift + 1) two += '0';
		tmp = sub_bint(two, tmp, B);
		t_inv = mul_bint(t_inv, tmp, B);
		shift += pshift;
		if (sz(t_inv) > len) {
			shift -= sz(t_inv) - len;
			t_inv.resize(len);
		}
	}

	string res = mul_bint(s, t_inv, B);

	res.resize(max(sz(res) - m - sz(t_inv) + 1, 0));
	if (res.empty()) res = "0";

	string res2 = add_bint(res, "1", B);
	if (comp_bint(mul_bint(res2, t, B), "<=", s)) res = move(res2);

	return res;
}


//【余り（文字列）】O((n + m) log(n + m))
/*
* B 進表記された非負整数 s[0..n) を t[0..m) で割った余りを返す．
*
* 利用：【減算（文字列）】，【除算（文字列）】
*/
string mod_bint(const string& s, const string& t, int B = 10) {
	// 参考 : https://qiita.com/square1001/items/1aa12e04934b6e749962
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/all/NTL_2_E

	Assert(t != "0");

	string q = div_bint(s, t, B);
	string r = sub_bint(s, mul_bint(q, t, B), B);

	return r;
}


//【インクリメント（文字列）】ならし O(1)
/*
* b 進表記された非負整数 s[0..n) に 1 を加える．
*/
void increment_bint(string& s, int b = 10) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_s

	int n = sz(s);

	repir(i, n - 1, 0) {
		if (s[i] != '0' + (b - 1)) {
			s[i]++;
			break;
		}

		s[i] = '0';
	}

	if (s[0] == '0') {
		s.insert(s.begin(), '1');
	}
}


//【デクリメント（文字列）】ならし O(1)
/*
* b 進表記された非負整数 s[0..n) から 1 を減じる．
* 
* 制約：s > 0
*/
void decrement_bint(string& s, int b = 10) {
	// verify : https://atcoder.jp/contests/joi2012yo/tasks/joi2012yo_f

	Assert(s[0] != '0');

	int n = sz(s);

	repir(i, n - 1, 0) {
		if (s[i] != '0') {
			s[i]--;
			break;
		}

		s[i] = '0' + (b - 1);
	}

	if (n >= 2 && s[0] == '0') {
		s.erase(s.begin());
	}
}


//【前 0 除去（文字列）】O(n)
/*
* b 進表記された非負整数 s[0..n) に不要な前 0 があればそれらを取り除く．
*/
void shrink_bint(string& s) {
	// verify : https://atcoder.jp/contests/arc057/tasks/arc057_c

	int i = 0;
	for (; i < sz(s); i++) {
		if (s[i] != '0') break;
	}

	if (i == sz(s)) s = "0";
	else s.erase(s.begin(), s.begin() + i);
}


//【除算（文字列）】O((n + m) (log(n + m))^2)（遅い）
/*
* B 進表記された非負整数 s[0..n) を t[0..m) で割った商を返す．
*
* 利用：【乗算（文字列）】
*/
string div_bint_slow(string s, string t, int base = 10) {
	// 参考 : https://qiita.com/square1001/items/1aa12e04934b6e749962
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/all/NTL_2_D

	Assert(t != "0");
	if (s == "0") return "0";
	if (s == t) return "1";

	int n = sz(s), m = sz(t), d = n - m;
	if (d < 0) return "0";

	// どれくらい余裕持たせれば良いのかわからない．
	// 30 にしたら Library Checker で WA した（そもそも TLE だが）
	int margin = 50;

	char nine = '0' + (base - 1);

	// ゴールドシュミットの割り算アルゴリズムを用いる．
	while (true) {
		bool all9 = true;
		rep(j, m) if (t[j] != nine) { all9 = false; break; }
		if (m == d + margin && all9) break;

		int jr = m - 1;
		while (t[jr] == '0') jr--;

		string t2 = "1";
		rep(j, jr) t2 += '0' + ((base - 1) - (t[j] - '0'));
		t2 += '0' + (base - (t[jr] - '0'));
		repi(j, jr + 1, m - 1) t2 += '0';

		s = mul_bint(s, t2, base);
		t = mul_bint(t, t2, base);

		while (sz(t) > d + margin) { s.pop_back(); t.pop_back(); }
		n = sz(s); m = sz(t);
	}

	if (n > m) s.resize(n - m);
	else s = "0";

	return s;
}


