#pragma once


//【ライブラリの読み込み】
#include <bits/stdc++.h>
using namespace std;


//【GCC のビルトイン関数との互換用】

// 全ビット中の 1 の個数
#define popcount (int)__popcnt
#define popcountll (int)__popcnt64

// 最下位ビットの位置（0-indexed）
inline int lsb(unsigned int n) {
	unsigned long i;
	_BitScanForward(&i, n);
	return i;
} 
inline int lsbll(unsigned long long n) {
	unsigned long i;
	_BitScanForward64(&i, n);
	return i;
}

// 最上位ビットの位置（0-indexed）
inline int msb(unsigned int n) {
	unsigned long i;
	_BitScanReverse(&i, n);
	return i;
} 
inline int msbll(unsigned long long n) {
	unsigned long i;
	_BitScanReverse64(&i, n);
	return i;
}

// 最大公約数
template <class T> T gcd(T a, T b) {
	a = abs(a);
	b = abs(b);

	while (b > 0) {
		a %= b;
		swap(a, b);
	}

	return a;
}


//【出力演算子 << のオーバーロード（デバッグ用）】
template <class T, class U>
inline ostream& operator<<(ostream& os, const pair<T, U>& p) {
	os << "(" << p.first << "," << p.second << ")";
	return os;
}

template <class T, class U, class V>
inline ostream& operator<<(ostream& os, const tuple<T, U, V>& t) {
	os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << ")";
	return os;
}

template <class T, class U, class V, class W>
inline ostream& operator<< (ostream& os, const tuple<T, U, V, W>& t) {
	os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << "," << get<3>(t) << ")";
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, const vector<T>& v) {
	repe(x, v) os << x << " ";
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, const list<T>& v) {
	repe(x, v) os << x << " ";
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, const set<T>& s) {
	repe(x, s) os << x << " ";
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, const set<T, greater<T>>& s) {
	repe(x, s) os << x << " ";
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, const multiset<T>& s) {
	repe(x, s) os << x << " ";
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, const multiset<T, greater<T>>& s) {
	repe(x, s) os << x << " ";
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, const unordered_set<T>& s) {
	repe(x, s) os << x << " ";
	return os;
}

template <class T, class U>
inline ostream& operator<< (ostream& os, const map<T, U>& m) {
	repe(p, m) os << p << " ";
	return os;
}

template <class T, class U>
inline ostream& operator<< (ostream& os, const map<T, U, greater<T>>& m) {
	repe(p, m) os << p << " ";
	return os;
}
template <class T, class U>
inline ostream& operator<< (ostream& os, const unordered_map<T, U>& m) {
	repe(p, m) os << p << " ";
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, stack<T> s) {
	while (!s.empty()) {
		os << s.top() << " ";
		s.pop();
	}
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, queue<T> q) {
	while (!q.empty()) {
		os << q.front() << " ";
		q.pop();
	}
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, deque<T> q) {
	while (!q.empty()) {
		os << q.front() << " ";
		q.pop_front();
	}
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, priority_queue<T> q) {
	while (!q.empty()) {
		os << q.top() << " ";
		q.pop();
	}
	return os;
}

template <class T>
inline ostream& operator<< (ostream& os, priority_queue<T, vector<T>, greater<T>> q) {
	while (!q.empty()) {
		os << q.top() << " ";
		q.pop();
	}
	return os;
}


//【ファイル入出力】
#define input_from_file(f) ifstream _is_TMP_(f); cin.rdbuf(_is_TMP_.rdbuf());
#define output_to_file(f) ofstream _os_TMP_(f); cout.rdbuf(_os_TMP_.rdbuf());


//【デバッグ出力関数】
// 参考 : https://marycore.jp/prog/cpp/variadic-function/
template <typename First>
void dump(First first) {
	cerr << "\033[1;36m" << first << "\033[0m" << endl;
}
template <typename First, typename... Rest>
void dump(First first, Rest... rest) {
	cerr << "\033[1;36m" << first << "\033[0m ";
	dump(rest...);
}

template <class T>
void dumpel(T a) {
	int i = 0; 
	cerr << "\033[1;36m";
	repe(x, a) {
		cerr << i++ << ": " << x << endl;
	}
	cerr << "\033[0m";
}
