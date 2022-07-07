#pragma once
#include "header.h"
// ■■■■■ 多倍長整数 ■■■■■


//【加算（文字列）】O(max(n, m))
/*
* b 進表記で表された数 s[0..n) と t[0..m) の和を返す．
*/
string add(const string& s, const string& t, int b = 10) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/all/NTL_2_A

	int i = sz(s) - 1, j = sz(t) - 1, c = 0;
	string res;

	while (i >= 0 || j >= 0 || c > 0) {
		int v = (i >= 0 ? s[i] - '0' : 0) + (j >= 0 ? t[j] - '0' : 0) + c;

		c = v / b;
		res.push_back('0' + v % b);

		i--; j--;
	}

	reverse(all(res));

	return res;
}


//【減算（文字列）】O(max(n, m))
/*
* b 進表記で表された数 s[0..n) から t[0..m) を引いた差を返す．
*
* 制約：s >= t
*/
string sub(const string& s, const string& t, int b = 10) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/all/NTL_2_A

	int i = sz(s) - 1, j = sz(t) - 1, c = 0;
	string res;

	while (i >= 0) {
		int vs = (s[i] - '0') - c;
		int vt = j >= 0 ? t[j] - '0' : 0;

		c = 0;
		if (vs < vt) {
			vs += b;
			c = 1;
		}

		res.push_back('0' + vs - vt);

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
* b 進表記で表された数 s[0..n) と t[0..m) の積を返す．
*/
string mul(const string& s, const string& t, int base = 10) {
	// verify : https://atcoder.jp/contests/arc057/tasks/arc057_c

	if (s == "0" || t == "0") return "0";

	int n = sz(s), m = sz(t);

	vl a(n), b(m);
	rep(i, n) a[i] = s[n - 1 - i] - '0';
	rep(j, m) b[j] = t[m - 1 - j] - '0';

	vl c = convolution_ll(a, b);

	int k = 0;
	for (; k < n + m - 2; k++) {
		c[k + 1] += c[k] / base;
		c[k] %= base;
	}
	while (c[k] >= base) {
		c.push_back(c[k] / base);
		c[k] %= base;
		k++;
	}

	string res;
	while (k >= 0) {
		res += (char)(c[k] + '0');
		k--;
	}

	return res;
}


//【比較（文字列）】O(min(n, m))
/*
* b 進表記で表された数 s[0..n), t[0..m) について，s[0..n) op t[0..m) かを返す．
* 比較演算子 op は，">", ">=", "=", "<=", "<" のいずれかとする．
*/
bool comp(const string& s, const string& op, const string& t) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/all/NTL_2_A

	Assert(op == ">" || op == ">=" || op == "=" || op == "<=" || op == "<");
	int n = sz(s), m = sz(t);

	if (op[0] == '=') return s == t;

	if (op[0] == '<') {
		// 桁数の比較だけで大小関係が分かる場合（前 0 は無いものとする）
		if (n < m) return true;
		if (n > m) return false;

		// 桁数が同じときは単に辞書順比較したものと結果が一致する．
		return op == "<" ? s < t : s <= t;
	}
	else {
		// 桁数の比較だけで大小関係が分かる場合（前 0 は無いものとする）
		if (n > m) return true;
		if (n < m) return false;

		// 桁数が同じときは単に辞書順比較したものと結果が一致する．
		return op == ">" ? s > t : s >= t;
	}
}


//【インクリメント（文字列）】ならし O(1)
/*
* b 進表記で表された数 s[0..n) に 1 を加える．
*/
void increment(string& s, int b = 10) {
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
* b 進表記で表された数 s[0..n) から 1 を減じる．
* 
* 制約：s > 0
*/
void decrement(string& s, int b = 10) {
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


//【前 0 除去】O(n)
/*
* 数 s[0..n) に不要な前 0 があればそれらを取り除く．
*/
void shrink(string& s) {
	// verify : https://atcoder.jp/contests/arc057/tasks/arc057_c

	int i = 0;
	for (; i < sz(s); i++) {
		if (s[i] != '0') break;
	}

	if (i == sz(s)) s = "0";
	else s.erase(s.begin(), s.begin() + i);
}


