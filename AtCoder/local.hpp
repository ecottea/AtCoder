#pragma once


//【ライブラリの読み込み】
#include <bits/stdc++.h>
using namespace std;

#define Assert assert


//【GCC のビルトイン関数との互換用】

// 全ビット中の 1 の個数
inline int popcount(int n) {
	return (int)__popcnt(n);
}
inline int popcount(ll n) {
	return (int)__popcnt64(n);
}

// 最下位ビットの位置（0-indexed）
inline int lsb(int n) {
	if (n == 0) return -1;
	unsigned long i;
	_BitScanForward(&i, (unsigned int)n);
	return i;
} 
inline int lsb(ll n) {
	if (n == 0) return -1;
	unsigned long i;
	_BitScanForward64(&i, (unsigned long long)n);
	return i;
}

// 最上位ビットの位置（0-indexed）
inline int msb(int n) {
	if (n == 0) return -1;
	unsigned long i;
	_BitScanReverse(&i, (unsigned int)n);
	return i;
} 
inline int msb(ll n) {
	if (n == 0) return -1;
	unsigned long i;
	_BitScanReverse64(&i, (unsigned long long)n);
	return i;
}

// 最大公約数
template <class T> T gcd(T a, T b) {
	// gcc の __gcd では負の数を入れるとバグる．
	Assert(a >= 0 && b >= 0);

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

template <class T, size_t N>
inline ostream& operator<< (ostream& os, const array<T, N>& v) {
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
inline ostream& operator<< (ostream& os, const multimap<T, U>& m) {
	repe(p, m) os << p << " ";
	return os;
}

template <class T, class U>
inline ostream& operator<< (ostream& os, const multimap<T, U, greater<T>>& m) {
	repe(p, m) os << p << " ";
	return os;
}

template <class Key, class T, class Hash, class Pred, class Allocater>
inline ostream& operator<< (ostream& os, const unordered_map<Key, T, Hash, Pred, Allocater>& um) {
	repe(p, um) os << p << " ";
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

template <class T, class Container, class Compare>
inline ostream& operator<< (ostream& os, priority_queue<T, Container, Compare> q) {
	// 参考 : https://qiita.com/hibit/items/8ca9a58ccd23014f3a54

	while (!q.empty()) {
		os << q.top() << " ";
		q.pop();
	}
	return os;
}

// tuple
// 参考 : https://www.delftstack.com/ja/howto/cpp/cpp-tuple-in-cpp/
template<class Tuple, size_t N>
struct TuplePrinter {
	static void print(ostream& os, const Tuple& t)
	{
		TuplePrinter<Tuple, N - 1>::print(os, t);
		os << "," << get<N - 1>(t);
	}
};

template<class Tuple>
struct TuplePrinter<Tuple, 1> {
	static void print(ostream& os, const Tuple& t)
	{
		os << get<0>(t);
	}
};

template<class... Args>
inline ostream& operator<< (ostream& os, const tuple<Args...>& t)
{
	os << "(";
	TuplePrinter<decltype(t), sizeof...(Args)>::print(os, t);
	os << ")";

	return os;
}


//【ファイル入出力】
#define input_from_file(f) ifstream _is_TMP_(f); cin.rdbuf(_is_TMP_.rdbuf());
#define output_to_file(f) ofstream _os_TMP_(f); cout.rdbuf(_os_TMP_.rdbuf());


//【デバッグ出力関数】
/* 参考: https://marycore.jp/prog/cpp/variadic-function/ */

bool mute_dump = false;

template <typename First>
void dump(First first) {
	if (mute_dump) return;

	cerr << "\033[1;32m" << first << "\033[0m" << endl;
}
template <typename First, typename... Rest>
void dump(First first, Rest... rest) {
	if (mute_dump) return;

	cerr << "\033[1;32m" << first << "\033[0m ";
	dump(rest...);
}

template <class T> void dumpel(T a) {
	if (mute_dump) return;

	int i = 0; 
	cerr << "\033[1;32m";
	repe(x, a) {
		cerr << right << setw(2) << i++ << ": " << x << endl;
	}
	cerr << "\033[0m";
}

// Mathematica の書式に合わせた出力
template <class T> void dump_list(vector<T> a) {
	if (mute_dump) return;

	cout << "{";
	rep(i, sz(a)) {
		cout << a[i] << (i < sz(a) - 1 ? ", " : "}\n");
	}
}

template <class T> void dump_mat(vector<vector<T>> a) {
	if (mute_dump) return;

	cout << "{";
	rep(i, sz(a)) {
		cout << "{";
		rep(j, sz(a[0])) cout << a[i][j] << (j < sz(a[0]) - 1 ? "," : "}");
		cout << (i < sz(a) - 1 ? ",\n" : "}\n");
	}
}
