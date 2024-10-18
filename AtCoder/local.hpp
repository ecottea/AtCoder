#pragma once


//【ライブラリの読み込み】
#include <bits/stdc++.h>
using namespace std;


#define __int128 ll
//#include <boost/multiprecision/cpp_int.hpp> // warning STL4038 がうざい
//#define __int128 boost::multiprecision::int128_t // gcc の 10 倍くらい時間がかかる


#define Assert assert


//【gcc のビルトイン関数との互換用】

// 全ビット中の 1 の個数
inline int popcount(int n) {
	return (int)__popcnt(n);
}
inline int popcount(ll n) {
	return (int)__popcnt64(n);
}

// 最下位ビットの位置（0-indexed）
inline int lsb(int n) {
	if (n == 0) return 32;
	unsigned long i;
	_BitScanForward(&i, (unsigned int)n);
	return i;
} 
inline int lsb(ll n) {
	if (n == 0) return 64;
	unsigned long i;
	_BitScanForward64(&i, (ull)n);
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
	_BitScanReverse64(&i, (ull)n);
	return i;
}


//【出力演算子 << のオーバーロード（デバッグ用）】
template <class T1, class T2>
inline ostream& operator<<(ostream& os, const pair<T1, T2>& p) {
	os << "(" << p.first << "," << p.second << ")";
	return os;
}

template <class T, class Allocator>
inline ostream& operator<< (ostream& os, const vector<T, Allocator>& v) {
	repe(x, v) os << x << " ";
	return os;
}

template <class T, class Allocator>
inline ostream& operator<< (ostream& os, const list<T, Allocator>& v) {
	repe(x, v) os << x << " ";
	return os;
}

template <class T, size_t N>
inline ostream& operator<< (ostream& os, const array<T, N>& v) {
	repe(x, v) os << x << " ";
	return os;
}

template <class Key, class Compare, class Allocator>
inline ostream& operator<< (ostream& os, const set<Key, Compare, Allocator>& s) {
	repe(x, s) os << x << " ";
	return os;
}

template <class Key, class Compare, class Allocator>
inline ostream& operator<< (ostream& os, const multiset<Key, Compare, Allocator>& s) {
	repe(x, s) os << x << " ";
	return os;
}

template <class Key, class Hash, class Pred, class Allocator>
inline ostream& operator<< (ostream& os, const unordered_set<Key, Hash, Pred, Allocator>& s) {
	repe(x, s) os << x << " ";
	return os;
}

template <class Key, class Hash, class Pred, class Allocator>
inline ostream& operator<< (ostream& os, const unordered_multiset<Key, Hash, Pred, Allocator>& s) {
	repe(x, s) os << x << " ";
	return os;
}

template <class Key, class T, class Compare, class Allocator>
inline ostream& operator<< (ostream& os, const map<Key, T, Compare, Allocator>& m) {
	repe(p, m) os << p << " ";
	return os;
}

template <class Key, class T, class Compare, class Allocator>
inline ostream& operator<< (ostream& os, const multimap<Key, T, Compare, Allocator>& m) {
	repe(p, m) os << p << " ";
	return os;
}

template <class Key, class T, class Hash, class Pred, class Allocater>
inline ostream& operator<< (ostream& os, const unordered_map<Key, T, Hash, Pred, Allocater>& um) {
	repe(p, um) os << p << " ";
	return os;
}

template <class T, class Container>
inline ostream& operator<< (ostream& os, stack<T, Container> s) {
	while (!s.empty()) {
		os << s.top() << " ";
		s.pop();
	}
	return os;
}

template <class T, class Container>
inline ostream& operator<< (ostream& os, queue<T, Container> q) {
	while (!q.empty()) {
		os << q.front() << " ";
		q.pop();
	}
	return os;
}

template <class T, class Allocator >
inline ostream& operator<< (ostream& os, deque<T, Allocator> q) {
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
		rep(j, sz(a[i])) cout << a[i][j] << (j < sz(a[i]) - 1 ? "," : "}");
		cout << (i < sz(a) - 1 ? ",\n" : "}\n");
	}
}

