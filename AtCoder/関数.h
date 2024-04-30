#pragma once
#include "header.h"
// ■■■■■ 関数，写像 ■■■■■


//【写像の合成】
/*
* Map_composite(vi f, ll M) : O(n log M)
*	[0..n) 上の写像 f : i → f[i] で初期化する．f^M まで計算可能とする．
*
* int apply(int x, ll m) : O(log m)
*	f^m(x) を返す．
*
* ll max_right(int x, function<bool(int)>& okQ) : O(log M)
*	okQ(f^m(x)) = true かつ okQ(f^(m+1)(x)) = false なる m を返す．
*
*（ダブリング）
*/
class Map_composite {
	int n, K;

	// nxt[k][i] : f^(2^k)[i]
	vvi nxt;

public:
	// [0..n) 上の写像 i → f[i] で初期化する．f^M まで計算可能とする．
	Map_composite(const vi& f, ll M) : n(sz(f)), K(msb(max(M, 1LL)) + 1), nxt(K, vi(n)) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_be

		// f^(2^0)[i] = f[i]
		rep(i, n) nxt[0][i] = f[i];

		// f^(2^(k+1))[i] = f^(2^k)[ f^(2^k)[i] ]
		rep(k, K - 1) rep(i, n) nxt[k + 1][i] = nxt[k][nxt[k][i]];
	}
	Map_composite() : n(0), K(0) {}

	// f^m(x) を返す．
	int apply(int x, ll m) const {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_be

		Assert(0 <= x && x < n);

		int k = 0;
		while (m > 0) {
			if (m & 1) x = nxt[k][x];
			m /= 2;
			k++;
		}
		return x;
	}

	// okQ(f^m(x)) = true かつ okQ(f^(m+1)(x)) = false なる m を返す．
	ll max_right(int x, const function<bool(int)>& okQ) const {
		// verify : https://atcoder.jp/contests/arc060/tasks/arc060_c

		Assert(0 <= x && x < n);
		if (!okQ(x)) return -1;

		ll m = 0;
		repir(k, K - 1, 0) {
			m <<= 1;
			if (okQ(nxt[k][x])) {
				m++;
				x = nxt[k][x];
			}
		}
		return m;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Map_composite& mc) {
		rep(k, mc.K) {
			os << (1LL << k) << ":" << endl;
			rep(i, mc.n) os << mc.nxt[k][i] << " "; os << endl;
		}
		return os;
	}
#endif

	/* okQ の定義の雛形
	auto okQ = [&](int i) {
		return true || false;
	};
	*/
};


//【写像の合成（モノイド）】
/*
* Map_accumulate<S, op, e>(vi f, vS a, ll I) : O(n log I)
*	[0..n) 上の写像 f : x → f[x] と列 a[0..n) で初期化する．f^I まで計算可能とする．
*
* int apply(int x, ll i) : O(log i)
*	f^i(x) を返す．
*
* S accumulate(int x, ll r) : O(log r)
*	Πa[f^[0..r)(x)] を返す．
*
* ll max_right(int x, function<bool(int, S)>& okQ) : O(log I)
*	okQ(f^i(x), Πa[f^[0..i)(x)]) = true かつ okQ(f^(i+1)(x), Πa[f^[0..i+1)(x)]) = false なる i を返す．
*
*（ダブリング）
*/
template <class S, S(*op)(S, S), S(*e)()>
class Map_accumulate {
	int n, K;

	// nxt[k][x] : f^(2^k)[x]
	vvi nxt;

	// acc[k][x] : Σi∈[0..2^k) a[f^i[x]]
	using vS = vector<S>; using vvS = vector<vS>;
	vvS acc;

public:
	// [0..n) 上の写像 f : x → f[x] と数列 a[0..n) で初期化する．f^I まで計算可能とする．
	Map_accumulate(const vi& f, const vS& a, ll I)
		: n(sz(f)), K(msb(max(I, 1LL)) + 1), nxt(K, vi(n)), acc(K, vS(n))
	{
		// verify : https://atcoder.jp/contests/abc241/tasks/abc241_e

		// f^(2^0)[x] = f[x]
		rep(x, n) nxt[0][x] = f[x];

		// Σi=[0..2^0) a[f^i[x]] = a[x]
		rep(x, n) acc[0][x] = a[x];

		rep(k, K - 1) {
			rep(x, n) {
				// 例：
				// f^8[x] = f^4[ f^4[x] ]
				nxt[k + 1][x] = nxt[k][nxt[k][x]];

				// 例：
				// a[f^0[x]] + a[f^1[x]] + a[f^2[x]] + a[f^3[x]]
				//	+ a[f^4[x]] + a[f^5[x]] + a[f^6[x]] + a[f^7[x]]
				// = a[f^0[x]] + a[f^1[x]] + a[f^2[x]] + a[f^3[x]]
				//	+ a[f^0[ f^4[x] ]] + a[f^1[ f^4[x] ]] + a[f^2[ f^4[x] ]] + a[f^3[ f^4[x] ]]
				acc[k + 1][x] = op(acc[k][x], acc[k][nxt[k][x]]);
			}
		}
	}

	// f^i(x) を返す．
	int apply(int x, ll i) const {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_be

		Assert(0 <= x && x < n);

		int k = 0;
		while (i > 0) {
			if (i & 1) x = nxt[k][x];
			i >>= 1;
			k++;
		}
		return x;
	}

	// Πi=[0..r) a[ f^i(x) ] を返す．
	S accumulate(S x, ll r) const {
		// verify : https://atcoder.jp/contests/abc241/tasks/abc241_e

		Assert(0 <= x && x < n);

		int k = 0; S val = e();
		while (r > 0) {
			if (r & 1) {
				val = op(val, acc[k][x]);
				x = nxt[k][x];
			}
			r >>= 1;
			k++;
		}

		return val;
	}

	// okQ(f^i(x), Πa[f^[0..i)(x)]) = true かつ okQ(f^(i+1)(x), Πa[f^[0..i+1)(x)]) = false なる i を返す．
	ll max_right(int x, const function<bool(int, S)>& okQ) const {
		// verify : https://atcoder.jp/contests/arc169/tasks/arc169_b

		Assert(0 <= x && x < n);
		if (!okQ(x, e())) return -1;

		ll i = 0; S val = e();
		repir(k, K - 1, 0) {
			i <<= 1;
			auto nval = op(val, acc[k][x]);
			if (okQ(nxt[k][x], nval)) {
				i++;
				val = nval;
				x = nxt[k][x];
			}
		}
		return i;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Map_accumulate& ma) {
		rep(k, ma.K) {
			os << (1LL << k) << ":" << endl;
			rep(x, ma.n) os << ma.nxt[k][x] << " "; os << endl;
			rep(x, ma.n) os << ma.acc[k][x] << " "; os << endl;
		}
		return os;
	}
#endif

	/* okQ の定義の雛形
	using S = ll;
	auto okQ = [&](int x, S acc) {
		return true || false;
	};
	*/
};


//【Convex-Hull Trick（挿入単調，クエリ単調）】（オーバーフロー注意）
/*
* Convex_hull_trick_monotonous<T>(bool max_flag = false) : O(1)
*	空で初期化する．max_flag = false[true] なら最小値[最大値] クエリに対応する．
*
* insert(T a, T b) : ならし O(1)
*	直線 y = a x + b を追加する．
*	制約：呼び出す際の挿入する直線の傾き a は狭義降順[昇順] でなくてはならない．
*
* T get(T x) : ならし O(1)
*	a x + b の最小値[最大値] を返す（直線がなければ INFL[-INFL]）
*	制約：呼び出す際の x 座標は狭義昇順でなくてはならない．
*/
template <class T = ll>
class Convex_hull_trick_monotonous {
	// 参考 : https://satanic0258.hatenablog.com/entry/2016/08/16/181331

	int n; // 記録している直線の本数
	bool max_flag;
	deque<pair<T, T>> lines; // 直線を傾き狭義降順に記録したリスト

public:
	Convex_hull_trick_monotonous(bool max_flag = false) : n(0), max_flag(max_flag) {
		// verify : https://atcoder.jp/contests/dp/tasks/dp_z
	}

	// 直線 y = a x + b を追加する．
	void insert(T a, T b) {
		// verify : https://atcoder.jp/contests/dp/tasks/dp_z

		if (max_flag) { a *= -1; b *= -1; }

		while (n >= 2) {
			const auto& [a1, b1] = lines[n - 2];
			const auto& [a2, b2] = lines[n - 1];

			// y = a2 x + b2 が必要な直線なら消去をここまでにする．
			if ((a1 - a2) * (b - b2) > (a2 - a) * (b2 - b1)) break;

			lines.pop_back();
			n--;
		}
		lines.emplace_back(a, b);
		n++;
	}

	// a x + b の最小値[最大値] を返す．
	T get(T x) {
		// verify : https://atcoder.jp/contests/dp/tasks/dp_z

		while (n >= 2) {
			const auto& [a1, b1] = lines[0];
			const auto& [a2, b2] = lines[1];

			if (a1 * x + b1 < a2 * x + b2) break;
			lines.pop_front();
			n--;
		}

		T val(INFL);
		if (!lines.empty()) {
			const auto& [a, b] = lines[0];
			val = a * x + b;
		}
		if (max_flag) val *= -1;
		return val;
	}
};


//【Convex-Hull Trick】（オーバーフロー注意）
/*
* Convex_hull_trick<T>(bool max_flag = false) : O(1)
*	空で初期化する．max_flag = false[true] なら最小値[最大値] クエリに対応する．
*
* insert(T a, T b) : ならし O(log n)
*	直線 y = a x + b を追加する．
*
* T get(T x) : O(log n)
*	a x + b の最小値[最大値] を返す．
*	制約：直線集合は空でない
*/
template <class T = ll>
class Convex_hull_trick {
	// 参考 : https://satanic0258.hatenablog.com/entry/2016/08/16/181331

	// 1 本の直線を表す構造体
	struct Line {
		// 直線の式が y = a x + b であることを表す．
		T a, b;

		// 直線であるか（さもなくば最小値クエリ）
		bool is_line;

		// 次の直線へのポインタを返す関数 (クエリとの比較で)
		mutable function<const Line* ()> getSuc;

		Line(T a_, T b_, bool is_line = true) : a(a_), b(b_), is_line(is_line) {}

		bool operator<(const Line& rhs) const {
			// set は lower_bound のように任意の比較関数を引数にとることはできないので，
			// 比較演算子内で取得クエリか否かで場合分けすることにより無理やり二分探索を実現する．
			//（set を使わず自前で平衡二分探索木を書くなら，左右の子を参照して下っていくだけでいい）

			// 直線と最小値クエリの比較
			if (!rhs.is_line) {
				const Line* suc = getSuc();
				if (suc == nullptr) return false;

				const T& x = rhs.a;
				return (suc->a - a) * x + (suc->b - b) < T(0);
			}

			// 最小値クエリと直線の比較
			if (!is_line) {
				const Line* suc = rhs.getSuc();
				if (suc == nullptr) return true;

				const T& x = a;
				return (suc->a - rhs.a) * x + (suc->b - rhs.b) > T(0);
			}

			// 直線と直線の比較
			return a > rhs.a;
		}
	};

	set<Line> lines; // 直線を傾き狭義降順に記録した集合

	// 最小値クエリに対応する場合は false，最大値クエリに対応する場合は true
	bool max_flag;

public:
	// 空で初期化する．
	Convex_hull_trick(bool max_flag = false) : max_flag(max_flag) {}

	// 直線 l : y = a x + b を追加する．
	void insert(T a, T b) {
		// verify : https://judge.yosupo.jp/problem/line_add_get_min

		// 最大値クエリに対応する場合は -1 倍して上下反転し，最小値クエリとして扱う．
		if (max_flag) {
			a = -a;
			b = -b;
		}

		// nit : l の次に傾きが小さい直線（無いなら lines.end()）
		auto nit = lines.lower_bound({ a, b });

		// pit : l の次に傾きが大きい直線（無いなら lines.end()）
		auto pit = (nit != lines.begin() ? prev(nit) : lines.end());

		// pit と l の傾きが等しい場合
		if (pit != lines.end() && pit->a == a) {
			// pit の方が低い位置にあるなら l は不要
			if (pit->b <= b) return;

			// l の方が低い位置にあるなら pit は不要
			lines.erase(pit);
		}
		// l と nit の傾きが等しい場合
		else if (nit != lines.end() && a == nit->a) {
			// nit の方が低い位置にあるなら l は不要
			if (nit->b <= b) return;

			// l の方が低い位置にあるなら nit は不要
			lines.erase(nit);
		}
		// pit, l, nit の傾きが全て異なる場合
		else if (pit != lines.end() && nit != lines.end()) {
			// x1 = x1_num / x1_dnm : pit と l の交点の x 座標
			//（y = pit->a x + pit->b と y = a x + b を連立する）
			T x1_num = b - pit->b, x1_dnm = pit->a - a;

			// x2 = x2_num / x2_dnm : l と nit の交点の x 座標
			//（y = a x + b と y = nit->a x + nit->b を連立する）
			T x2_num = nit->b - b, x2_dnm = a - nit->a;

			// x1 >= x2 となっているなら l は不要な直線なので追加せず終わる．
			if (x1_num * x2_dnm >= x2_num * x1_dnm) return;
		}

		// 直線 l を追加する．
		auto it = lines.insert({ a, b }).first;
		it->getSuc = [=] { return (next(it) == lines.end() ? nullptr : &*next(it)); };

		// l より傾きが大きい直線のうち，l のせいで不必要になったものを削除する．
		if (it != lines.begin()) {
			auto pit = prev(it);
			while (pit != lines.begin()) {
				// pit : l の次に傾きが大きい直線
				// ppit : l の次の次に傾きが大きい直線
				auto ppit = prev(pit);

				// x1 = x1_num / x1_dnm : ppit と pit の交点の x 座標
				//（y = ppit->a x + ppit->b と y = pit->a x + pit->b を連立する）
				T x1_num = pit->b - ppit->b, x1_dnm = ppit->a - pit->a;

				// x2 = x2_num / x2_dnm : pit と l の交点の x 座標
				//（y = pit->a x + pit->b と y = a x + b を連立する）
				T x2_num = b - pit->b, x2_dnm = pit->a - a;

				// x1 <= x2 となっているなら pit は必要な直線なので削除せず終わる．
				if (x1_num * x2_dnm <= x2_num * x1_dnm) break;

				// さもなくば pit は不必要な直線なので削除する．
				pit = prev(lines.erase(pit));
			}
		}

		// l より傾きが小さい直線のうち，l のせいで不必要になったものを削除する．
		if (next(it) != lines.end()) {
			auto nit = next(it);
			while (next(nit) != lines.end()) {
				// nit : l の次に傾きが小さい直線
				// nnit : l の次の次に傾きが小さい直線
				auto nnit = next(nit);

				// x1 = x1_num / x1_dnm : l と nit の交点の x 座標
				//（y = a x + b と y = nit->a x + nit->b を連立する）
				T x1_num = nit->b - b, x1_dnm = a - nit->a;

				// x2 = x2_num / x2_dnm : nit と nnit の交点の x 座標
				//（y = nit->a x + nit->b と y = nnit->a x + nnit->b を連立する）
				T x2_num = nnit->b - nit->b, x2_dnm = nit->a - nnit->a;

				// x1 <= x2 となっているなら nit は必要な直線なので削除せず終わる．
				if (x1_num * x2_dnm <= x2_num * x1_dnm) break;

				// さもなくば nit は不必要な直線なので削除する．
				nit = lines.erase(nit);
			}
		}
	}

	// a x + b の最小値[最大値] を返す．
	T get(T x) {
		// verify : https://judge.yosupo.jp/problem/line_add_get_min

		Assert(!lines.empty());

		auto it = lines.lower_bound(Line{ x, x, false });

		if (!max_flag) return it->a * x + it->b;
		else return -(it->a * x + it->b); // 最大値クエリの場合は -1 倍していたので元に戻す．
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Convex_hull_trick& cht) {
		for (auto it = cht.lines.begin(); it != cht.lines.end(); it++) {
			auto a = it->a;
			auto b = it->b;

			if (cht.max_flag) {
				a *= T(-1);
				b *= T(-1);
			}

			os << "y=";

			if (a == T(1)) os << "x";
			else if (a == T(0));
			else if (a == T(-1)) os << "-x";
			else os << a << "x";

			if (a == T(0) || b < T(0)) os << b;
			else if (b > T(0)) os << "+" << b;

			os << (next(it) != cht.lines.end() ? "," : "");
		}
		return os;
	}
#endif
};


//【Convex-Hull Trick（整数）】
/*
* Convex_hull_trick<T>(bool max_flag = false) : O(1)
*	空で初期化する．max_flag = false[true] なら最小値[最大値] クエリに対応する．
*	制約：T は整数型
*
* insert(T a, T b) : ならし O(log n)
*	直線 y = a x + b を追加する．
*
* T get(T x) : O(log n)
*	a x + b の最小値[最大値] を返す．
*	制約：直線集合は空でない
*/
template <class T = ll>
class Convex_hull_trick_integer {
	// 参考 : https://noshi91.hatenablog.com/entry/2021/03/23/200810

	// 1 本の直線を表す構造体
	struct Line {
		// 直線の式が y = a x + b であることを表す．
		T a, b;

		// 直線であるか（さもなくば最小値クエリ）
		bool is_line;

		// 次の直線へのポインタを返す関数 (クエリとの比較で)
		mutable function<const Line* ()> getSuc;

		Line(T a_, T b_, bool is_line = true) : a(a_), b(b_), is_line(is_line) {}

		bool operator<(const Line& rhs) const {
			// set は lower_bound のように任意の比較関数を引数にとることはできないので，
			// 比較演算子内で取得クエリか否かで場合分けすることにより無理やり二分探索を実現する．
			//（set を使わず自前で平衡二分探索木を書くなら，左右の子を参照して下っていくだけでいい）

			// 直線と最小値クエリの比較
			if (!rhs.is_line) {
				const Line* suc = getSuc();
				if (suc == nullptr) return false;

				const T& x = rhs.a;
				return (suc->a - a) * x + (suc->b - b) < T(0);
			}

			// 最小値クエリと直線の比較
			if (!is_line) {
				const Line* suc = rhs.getSuc();
				if (suc == nullptr) return true;

				const T& x = a;
				return (suc->a - rhs.a) * x + (suc->b - rhs.b) > T(0);
			}

			// 直線と直線の比較
			return a > rhs.a;
		}

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Line& l) {
			os << "y=";

			if (l.a == T(1)) os << "x";
			else if (l.a == T(0));
			else if (l.a == T(-1)) os << "-x";
			else os << l.a << "x";

			if (l.a == T(0) || l.b < T(0)) os << l.b;
			else if (l.b > T(0)) os << "+" << l.b;

			return os;
		}
#endif
	};

	set<Line> lines; // 直線を傾き狭義降順に記録した集合

	// 最小値クエリに対応する場合は false，最大値クエリに対応する場合は true
	bool max_flag;

public:
	// 空で初期化する．
	Convex_hull_trick_integer(bool max_flag = false) : max_flag(max_flag) {
		// verify : https://judge.yosupo.jp/problem/line_add_get_min
	}

	// 直線 l : y = a x + b を追加する．
	void insert(T a, T b) {
		// verify : https://judge.yosupo.jp/problem/line_add_get_min

		// 最大値クエリに対応する場合は -1 倍して上下反転し，最小値クエリとして扱う．
		if (max_flag) {
			a = -a;
			b = -b;
		}

		// nit : l の次に傾きが小さい直線（無いなら lines.end()）
		auto nit = lines.lower_bound({ a, b });

		// pit : l の次に傾きが大きい直線（無いなら lines.end()）
		auto pit = (nit != lines.begin() ? prev(nit) : lines.end());

		// pit と l の傾きが等しい場合
		if (pit != lines.end() && pit->a == a) {
			// pit の方が低い位置にあるなら l は不要
			if (pit->b <= b) return;

			// l の方が低い位置にあるなら pit は不要
			lines.erase(pit);
		}
		// l と nit の傾きが等しい場合
		else if (nit != lines.end() && a == nit->a) {
			// nit の方が低い位置にあるなら l は不要
			if (nit->b <= b) return;

			// l の方が低い位置にあるなら nit は不要
			lines.erase(nit);
		}
		// pit, l, nit の傾きが全て異なる場合
		else if (pit != lines.end() && nit != lines.end()) {
			// l が不要な直線なら追加せず終わる．
			if ((b - pit->b) / (pit->a - a) >= (nit->b - b) / (a - nit->a)) return;
		}

		// 直線 l を追加する．
		auto it = lines.insert({ a, b }).first;
		it->getSuc = [=] { return (next(it) == lines.end() ? nullptr : &*next(it)); };

		// l より傾きが大きい直線のうち，l のせいで不必要になったものを削除する．
		if (it != lines.begin()) {
			auto pit = prev(it);
			while (pit != lines.begin()) {
				// pit : l の次に傾きが大きい直線
				// ppit : l の次の次に傾きが大きい直線
				auto ppit = prev(pit);

				// pit が必要な直線なら削除せず終わる．
				if ((pit->b - ppit->b) / (ppit->a - pit->a) < (b - pit->b) / (pit->a - a)) break;

				// さもなくば pit は不必要な直線なので削除する．
				pit = prev(lines.erase(pit));
			}
		}

		// l より傾きが小さい直線のうち，l のせいで不必要になったものを削除する．
		if (next(it) != lines.end()) {
			auto nit = next(it);
			while (next(nit) != lines.end()) {
				// nit : l の次に傾きが小さい直線
				// nnit : l の次の次に傾きが小さい直線
				auto nnit = next(nit);

				// nit が必要な直線なら削除せず終わる．
				if ((nit->b - b) / (a - nit->a) < (nnit->b - nit->b) / (nit->a - nnit->a)) break;

				// さもなくば nit は不必要な直線なので削除する．
				nit = lines.erase(nit);
			}
		}
	}

	// a x + b の最小値[最大値] を返す．
	T get(T x) {
		// verify : https://judge.yosupo.jp/problem/line_add_get_min

		Assert(!lines.empty());

		auto it = lines.lower_bound(Line{ x, x, false });

		if (!max_flag) return it->a * x + it->b;
		else return -(it->a * x + it->b); // 最大値クエリの場合は -1 倍していたので元に戻す．
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Convex_hull_trick_integer& cht) {
		for (auto it = cht.lines.begin(); it != cht.lines.end(); it++) {
			os << *it << (next(it) != cht.lines.end() ? "," : "");
		}
		return os;
	}
#endif
};


//【Slope Trick】
/*
* Slope_trick() : O(1)
*	定数関数 f(x) = 0 で初期化する．
*
* ll min(ll* l = nullptr, ll* r = nullptr) : O(1)
*	min f(x) を返し，必要ならそれを与える x の範囲 [l, r) を格納する．
*
* add_const(ll x0) : O(1)
*	f(x) += y0 とする．
*
* add_right(ll x0) : O(log n)
*	f(x) += max(x - x0, 0) とする．（＿／ の形を加算する．）
*
* add_left(ll x0) : O(log n)
*	f(x) += max(x0 - x, 0) とする．（＼＿ の形を加算する．）
*
* add_abs(ll x0) : O(log n)
*	f(x) += |x - x0| とする．（＼／ の形を加算する．）
*
* acc_min_left() : O(1)
*	f(x) を左から累積最小値をとったものに置き換える．（＼＿ の形にする．）
*
* acc_min_right() : O(1)
*	f(x) を右から累積最小値をとったものに置き換える．（＿／ の形にする．）
*
* shift(ll x0) : O(1)
*	f(x) を右に x0 だけ平行移動する．（f(x) ← f(x - x0)）
*
* sliding_window_min(ll dl, ll dr) : O(1)
*	f(x) を min f([x-dl, x+dr]) に置き換える．（＼＿＿／ の形にする．）
* 
* vector<pll> get_points(ll l, ll r) : O(n)
*	[l..r] 内の f(x) の折れ点（両端含む）を x 座標昇順に格納したリストを返す．
*/
struct Slope_trick {
	// 参考 : https://maspypy.com/slope-trick-1-%E8%A7%A3%E8%AA%AC%E7%B7%A8

	ll y_min; // 最小値
	priority_queue<ll> qL; // 最小値より左の折れ点の x 座標を降順に取り出せるキュー
	priority_queue_rev<ll> qR; // 最小値より右の折れ点の x 座標を昇順に取り出せるキュー
	ll addL; // 最小値より左側の平行移動量
	ll addR; // 最小値より右側の平行移動量

	// f(x) = 0 で初期化する．
	Slope_trick() : y_min(0), addL(0), addR(0) {
		// verify : https://atcoder.jp/contests/abc127/tasks/abc127_f

		// 番兵を挿入しておく．
		qL.push(-INFL);
		qR.push(INFL);
	};

	// min f(x) を返し，必要ならそれを与える x の範囲 [l, r) を格納する．
	ll min(ll* l = nullptr, ll* r = nullptr) {
		// verify : https://atcoder.jp/contests/abc127/tasks/abc127_f

		if (r != nullptr) {
			*l = qL.top() + addL;
			*r = qR.top() + addR + 1;
		}
		return y_min;
	}

	// f(x) += y0 とする．
	void add_const(ll y0) {
		// verify : https://atcoder.jp/contests/abc127/tasks/abc127_f

		y_min += y0;
	}

	// f(x) += min(x - x0, 0) とする．（＿／ の形を加算する．）
	void add_right(ll x0) {
		// verify : https://atcoder.jp/contests/abc217/tasks/abc217_h

		y_min += max(0LL, (qL.top() + addL) - x0);
		qL.push(x0 - addL);
		qR.push((qL.top() + addL) - addR);
		qL.pop();
	}

	// f(x) += min(x0 - x, 0) とする．（＼＿ の形を加算する．）
	void add_left(ll x0) {
		// verify : https://atcoder.jp/contests/abc217/tasks/abc217_h

		y_min += max(0LL, x0 - (qR.top() + addR));
		qR.push(x0 - addR);
		qL.push((qR.top() + addR) - addL);
		qR.pop();
	}

	// f(x) += |x - x0| とする．（＼／ の形を加算する．）
	void add_abs(ll x0) {
		// verify : https://atcoder.jp/contests/abc127/tasks/abc127_f

		add_right(x0);
		add_left(x0);
	}

	// f(x) を左から累積最小値をとったものに置き換える．（＼＿ の形にする．）
	void acc_min_left() {
		qR = priority_queue_rev<ll>();
		qR.push(INFL);
	}

	// f(x) を右から累積最小値をとったものに置き換える．（＿／ の形にする．）
	void acc_min_right() {
		qL = priority_queue<ll>();
		qL.push(-INFL);
	}

	// f(x) を x0 だけ右に平行移動する．（f(x) ← f(x - x0)）
	void shift(ll x0) {
		addL += x0;
		addR += x0;
	}

	// f(x) を min f([x-dl, x+dr]) に置き換える．（＼＿＿／ の形にする．）
	void sliding_window_min(ll dl, ll dr) {
		// verify : https://atcoder.jp/contests/abc217/tasks/abc217_h

		addL -= dl;
		addR += dr;
	}

	// [l..r] 内の f(x) の折れ点（両端含む）を x 座標昇順に返す．
	vector<pll> get_points(ll l, ll r) const {
		// verify : https://www.codechef.com/problems/UNRCOST

		auto qL2(qL); auto qR2(qR);

		vector<pll> res; ll a = 0, y = y_min;
		while (!qL2.empty()) {
			ll x = qL2.top(); qL2.pop();
			if (x + addL < l) x = l - addL;
			if (!res.empty()) y -= (res.back().first - (x + addL)) * a;
			if (res.empty() || res.back().first != x + addL) res.emplace_back(x + addL, y);
			if (x + addL == l) break;
			a--;
		}
		reverse(all(res));

		a = 0; y = y_min;
		while (!qR2.empty()) {
			ll x = qR2.top(); qR2.pop();
			if (x + addR > r) x = r - addR;
			if (!res.empty()) y += ((x + addR) - res.back().first) * a;
			if (res.empty() || res.back().first != x + addR) res.emplace_back(x + addR, y);
			if (x + addR == r) break;
			a++;
		}

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Slope_trick& st) {
		os << st.get_points(-100, 100);
		return os;
	}
#endif
};


//【狭義単調な点列】
/*
* Monotonous_points<T>(bool y_smaller = false, T inf = max(T)/2) : O(1)
*	空で初期化する．x 座標は狭義単調増加で，y 座標は y_smaller=false[true] なら狭義単調増加[減少]とする．
*
* void insert(T x, T y) : ならし O(log n)
*	点 (x, y) を挿入し，それにより単調性に違反する点は全て削除する．
*	x 座標や y 座標がちょうど等しい点も消してしまうので注意！
*
* bool find_LL(T x, T y, bool eq = false) : O(log n)
*	x' < x かつ y' < y なる点 (x', y') が存在するかを返す（eq=true なら等号も許す）
*
* bool find_LG(T x, T y, bool eq = false) : O(log n)
*	x' < x かつ y' > y なる点 (x', y') が存在するかを返す（eq=true なら等号も許す）
*
* bool find_GL(T x, T y, bool eq = false) : O(log n)
*	x' > x かつ y' < y なる点 (x', y') が存在するかを返す（eq=true なら等号も許す）
*
* bool find_GG(T x, T y, bool eq = false) : O(log n)
*	x' > x かつ y' > y なる点 (x', y') が存在するかを返す（eq=true なら等号も許す）
*
* pTT lower_bound(T x) : O(log n)
*	x' ≧ x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
*
* pTT upper_bound(T x) : O(log n)
*	x' > x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
*
* pTT lower_bound_rev(T x) : O(log n)
*	x' ≦ x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
*
* pTT upper_bound_rev(T x) : O(log n)
*	x' < x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
*
* pTT get_all_points() : O(n)
*	全ての点を x 座標昇順に並べたリストを返す．
*/
template <class T>
struct Monotonous_points {
	// 参考 : https://topcoder-g-hatena-ne-jp.jag-icpc.org/skyaozora/20141216.html

	bool y_smaller; // y 座標について狭義単調減少か
	T inf; // 無限大

	// x 座標は狭義単調増加で，y 座標は y_greater=true[false] なら狭義単調増加[減少] な点列
	// ただし番兵として (-inf, -inf[inf]) と (inf, inf[-inf]) を含む．
	map<T, T> x_to_y;

	// 空で初期化する．x 座標は狭義単調増加で，y 座標は y_greater=true[false] なら狭義単調増加[減少]とする．
	Monotonous_points(bool y_smaller = false, T inf_ = -1) : y_smaller(y_smaller) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		inf = (inf_ == -1 ? numeric_limits<T>::max() / 2 : inf_);

		// 番兵を挿入しておく．
		if (!y_smaller) { x_to_y[-inf] = -inf; x_to_y[inf] = inf; }
		else { x_to_y[-inf] = inf; x_to_y[inf] = -inf; }
	}

	// 点 (x, y) を挿入し，単調性に違反する点は全て削除する．
	void insert(T x, T y) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		// x <= x' なる最小の x' を指すイテレータを得る．
		auto it = x_to_y.lower_bound(x);

		// x' から昇順に，y' ≦ y[ y' ≧ y ] である限り要素を削除する．
		if (!y_smaller) {
			while (true) {
				if (it->second > y) break;
				it = x_to_y.erase(it);
			}
		}
		else {
			while (true) {
				if (it->second < y) break;
				it = x_to_y.erase(it);
			}
		}

		// x' から降順に，y' ≧ y[ y' ≦ y ] である限り要素を削除する．
		if (!y_smaller) {
			while (true) {
				it = prev(it);
				if (it->second < y) break;
				it = x_to_y.erase(it);
			}
		}
		else {
			while (true) {
				it = prev(it);
				if (it->second > y) break;
				it = x_to_y.erase(it);
			}
		}

		// 点 (x, y) を挿入する．
		x_to_y[x] = y;
	}

	// x' < x かつ y' < y なる点が存在するかを返す（eq=true なら等号も許す）
	bool find_LL(T x, T y, bool eq = false) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/1341

		if (!eq) {
			T y2 = prev(x_to_y.lower_bound(x))->second;
			return y2 != -inf && y2 < y;
		}
		else {
			T y2 = prev(x_to_y.upper_bound(x))->second;
			return y2 != -inf && y2 <= y;
		}
	}

	// x' < x かつ y' > y なる点が存在するかを返す（eq=true なら等号も許す）
	bool find_LG(T x, T y, bool eq = false) {
		if (!eq) {
			T y2 = prev(x_to_y.lower_bound(x))->second;
			return y2 != inf && y2 > y;
		}
		else {
			T y2 = prev(x_to_y.upper_bound(x))->second;
			return y2 != inf && y2 >= y;
		}
	}

	// x' > x かつ y' < y なる点が存在するかを返す（eq=true なら等号も許す）
	bool find_GL(T x, T y, bool eq = false) {
		if (!eq) {
			T y2 = x_to_y.upper_bound(x)->second;
			return y2 != -inf && y2 < y;
		}
		else {
			T y2 = x_to_y.lower_bound(x)->second;
			return y2 != -inf && y2 <= y;
		}
	}

	// x' > x かつ y' > y なる点が存在するかを返す（eq=true なら等号も許す）
	bool find_GG(T x, T y, bool eq = false) {
		if (!eq) {
			T y2 = x_to_y.upper_bound(x)->second;
			return y2 != inf && y2 > y;
		}
		else {
			T y2 = x_to_y.lower_bound(x)->second;
			return y2 != inf && y2 >= y;
		}
	}

	// x' ≧ x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
	pair<T, T> lower_bound(T x) {
		// verify : https://codeforces.com/gym/100633/problem/D

		return *x_to_y.lower_bound(x);
	}

	// x' > x なる x 座標が最小の点 (x', y') を返す（なければ (inf, inf[-inf])）
	pair<T, T> upper_bound(T x) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		return *x_to_y.upper_bound(x);
	}

	// x' ≦ x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
	pair<T, T> lower_bound_rev(T x) {
		return *prev(x_to_y.upper_bound(x));
	}

	// x' < x なる x 座標が最大の点 (x', y') を返す（なければ (-inf, -inf[inf])）
	pair<T, T> upper_bound_rev(T x) {
		// verify : https://atcoder.jp/contests/abc283/tasks/abc283_f

		return *prev(x_to_y.lower_bound(x));
	}

	// 全ての点を x 座標昇順に並べたリストを返す．
	vector<pair<T, T>> get_all_points() {
		vector<pair<T, T>> res;
		res.reserve(sz(x_to_y));
		repe(tmp, x_to_y) res.push_back(tmp);
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Monotonous_points& mp) {
		repe(p, mp.x_to_y) if (abs(p.first) != mp.inf) os << p << " ";
		return os;
	}
#endif
};


//【区分線形凸関数】
/*
* Convex_function() : O(1)
*	n=1 本の線分からなる定数関数 f(x) = 0 で初期化する．
*
* add_const(ll c) : O(1)
*	f(x) ← f(x) + c とする．（上に c 平行移動する）
*
* shift(ll d) : O(n)
*	f(x) ← f(x - d) とする．（右に d 平行移動する）
*
* add_right(ll x0) : O(n)
*	f(x) ← f(x) + max(x - x0, 0) とする．（＿／ の形を加算する．）
*
* add_left(ll x0) : O(n)
*	f(x) ← f(x) + max(x0 - x, 0) とする．（＼＿ の形を加算する．）
*
* max(Convex_function g) : O(n)
*	f(x) ← max(f(x), g(x)) とする．（g(x) との各点 max をとる）
*
* ll get(ll x0) : O(n)
*	f(x0) を返す．
*/
class Convex_function_PL {
	// 参考 : https://atcoder.jp/contests/abc219/editorial/2669

	//【方法】
	// 下に凸な区分線形関数を，n 個の一次関数の max として表現する．

	// a_min[a_max] : 傾きの最小値[最大値]
	int a_min, a_max;

	// bs : 直線を傾き昇順に並べたときの切片のリスト
	deque<ll> bs;

public:
	Convex_function_PL() : a_min(0), a_max(0), bs{ 0 } {
		// verify : https://atcoder.jp/contests/abc219/tasks/abc219_h
	}

	// f(x) ← f(x) + c とする．
	void add_const(ll c) {
		bs[0 - a_min] += c;
	}

	// f(x) ← f(x - d) とする．（右に d 平行移動する）
	void shift(ll d) {
		// verify : https://atcoder.jp/contests/abc219/tasks/abc219_h

		repi(a, a_min, a_max) bs[a - a_min] -= a * d;
	}

	// f(x) ← f(x) + max(x - x0, 0) とする．（＿／ の形を加算する．）
	void add_right(ll x0) {
		ll y_max = -INFL; int a0 = -INF;
		repi(a, a_min, a_max) {
			ll y = a * x0 + bs[a - a_min];
			if (chmax(y_max, y)) a0 = a;
		}

		bs.push_back(-INFL); a_max++;

		repir(a, a_max, a0 + 1) bs[a - a_min] = bs[a - 1 - a_min] - x0;
	}

	// f(x) ← f(x) + max(x0 - x, 0) とする．（＼＿ の形を加算する．）
	void add_left(ll x0) {
		// verify : https://atcoder.jp/contests/abc219/tasks/abc219_h

		ll y_max = -INFL; int a0 = -INF;
		repi(a, a_min, a_max) {
			ll y = a * x0 + bs[a - a_min];
			if (chmax(y_max, y)) a0 = a;
		}

		bs.push_front(-INFL); a_min--;

		repi(a, a_min, a0 - 1) bs[a - a_min] = bs[a + 1 - a_min] + x0;
	}

	// f(x) ← max(f(x), g(x)) とする．
	void max(const Convex_function_PL& g) {
		// verify : https://atcoder.jp/contests/abc219/tasks/abc219_h

		if (a_min > g.a_min) {
			rep(hoge, a_min - g.a_min) bs.push_front(-INFL);
			a_min = g.a_min;
		}

		if (a_max < g.a_max) {
			rep(hoge, g.a_max - a_max) bs.push_back(-INFL);
			a_max = g.a_max;
		}

		repi(a, g.a_min, g.a_max) chmax(bs[a - a_min], g.bs[a - g.a_min]);
	}

	// f(x0) を返す．
	ll get(ll x0) {
		// verify : https://atcoder.jp/contests/abc219/tasks/abc219_h

		ll y_max = -INFL;
		repi(a, a_min, a_max) {
			ll y = a * x0 + bs[a - a_min];
			chmax(y_max, y);
		}

		return y_max;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Convex_function_PL& f) {
		repi(a, f.a_min, f.a_max) {
			os << a << " x + " << f.bs[a - f.a_min];
			if (a < f.a_max) os << ", ";
		}
		return os;
	}
#endif
};


//【Li Chao Tree】
/*
* Li_Chao_tree(int n, bool max_flag = false) : O(n)
*	直線 y = 0 x + INFL[-INFL] (x∈[0..n)) で初期化する．
*	max_flag = false[true] なら最小値[最大値] クエリに対応する．
*
* add_line(ll a, ll b) : O(log n)
*	直線 a x + b (x∈[0..n)) を追加する．
*
* add_segment(int l, int r, ll a, ll b) : O((log n)^2)
*	線分 a x + b (x∈[l..r)) を追加する．
*
* add_segment_to_right(int l, int r, ll y0, ll y1) : O((log n)^2)
*	y[l] = y0, y[l+1] = y1 なる線分を区間 x∈[l..r) に追加する．
*
* add_segment_to_left(int r, int l, ll y0, ll y1) : O((log n)^2)
*	y[r] = y0, y[r-1] = y1 なる線分を区間 x∈(l..r] に追加する．
*
* ll get(int x) : O(log n)
*	x を定義域に含む線分 a x + b らの最小値[最大値] を返す．
*/
class Li_Chao_tree {
	// 参考 : https://smijake3.hatenablog.com/entry/2018/06/16/144548

	// 完全二分木の葉の数（必ず 2 冪）
	int n;
	int actual_n; // 実際の要素数
	bool max_flag;

	// 対応する区間全体で最小となる直線の a : 傾き，b : 切片（なければ a=0, b=∞）
	// 完全二分木を実現する大きさ 2n の配列
	// v[0] は使用せず，根は v[1] で，v[i] の親は v[i/2]，子は v[2i], v[2i+1]．
	// 0-indexed での i 番目のデータは葉である v[i+n] に入っている．
	vl as, bs;

	// 区間 [L..R) に対応する部分木 i に線分 a x + b (x∈[l..r)) を追加する．
	void add_segment(int i, int L, int R, int l, int r, ll a, ll b) {
		if (i >= 2 * n) return;

		// [L..R) が [l..r) と共通部分をもたない場合，何もせず終了．
		if (r <= L || R <= l) return;

		// [L..R) の中央
		int M = (L + R) / 2;

		// [L..R) が [l..r) に包含されていない場合
		if (L < l || r < R) {
			// 左右の区間それぞれに対して再帰的に処理を行う．
			add_segment(2 * i, L, M, l, r, a, b);
			add_segment(2 * i + 1, M, R, l, r, a, b);
			return;
		}

		// [L..R) が [l..r) に包含されている場合

		// 記録されている直線 L0 の L, R での値
		ll yL0 = as[i] * L + bs[i], yR0 = as[i] * R + bs[i];

		// 追加しようとしている直線 L1 の L, R での値
		ll yL1 = a * L + b, yR1 = a * R + b;

		// L1 が L0 の上側にある場合，L1 は追加する意味がないので何もせず終了．
		if (yL1 >= yL0 && yR1 >= yR0) return;

		// L1 が L0 の下側にある場合，L0 を捨てて L1 に取り替え終了．
		if (yL1 <= yL0 && yR1 <= yR0) {
			as[i] = a; bs[i] = b;
			return;
		}

		// 記録されている直線 L0 の M での値
		ll yM0 = as[i] * M + bs[i];

		// 追加しようとしている直線 L1 の M での値
		ll yM1 = a * M + b;

		// [M..R) で L1 が L0 の上側にある場合，[L..M) の探索のみを進める．
		if (yM1 >= yM0 && yR1 >= yR0) {
			add_segment(2 * i, L, M, l, r, a, b);
			return;
		}

		// [L..M) で L1 が L0 の上側にある場合，[M..R) の探索のみを進める．
		if (yL1 >= yL0 && yM1 >= yM0) {
			add_segment(2 * i + 1, M, R, l, r, a, b);
			return;
		}

		// [M..R) で L1 が L0 の下側にある場合，L1 と L0 を交換して [L..M) の探索のみを進める．
		if (yM1 <= yM0 && yR1 <= yR0) {
			swap(as[i], a); swap(bs[i], b);
			add_segment(2 * i, L, M, l, r, a, b);
			return;
		}

		// [L..M) で L1 が L0 の下側にある場合，L1 と L0 を交換して [M..R) の探索のみを進める．
		if (yL1 <= yL0 && yM1 <= yM0) {
			swap(as[i], a); swap(bs[i], b);
			add_segment(2 * i + 1, M, R, l, r, a, b);
			return;
		}
	}

	// 区間 [L..R) に対応する部分木 i を定義域に含む線分 a x + b らの最小値を返す．
	ll get(int i, int L, int R, int x) const {
		if (i >= 2 * n) return INFL;

		// [L..R) の中央
		int M = (L + R) / 2;

		ll y = as[i] * x + bs[i];
		if (x < M) chmin(y, get(2 * i, L, M, x));
		else chmin(y, get(2 * i + 1, M, R, x));

		return y;
	}

public:
	// 直線 y = 0 x + INFL (x∈[0..n)) で初期化する．
	Li_Chao_tree(int n_, bool max_flag = false) : actual_n(n_), n(1 << (msb(n_ - 1) + 1)),
		max_flag(max_flag), as(2 * n, 0), bs(2 * n, INFL)
	{
		// verify : https://yukicoder.me/problems/no/704
	}
	Li_Chao_tree() : n(0), actual_n(0), max_flag(0) {}

	// 直線 a x + b (x∈[0..n)) を追加する．
	void add_line(ll a, ll b) {
		if (max_flag) { a *= -1; b *= -1; }
		add_segment(1, 0, n, 0, n, a, b);
	}

	// 線分 a x + b (x∈[l..r)) を追加する．
	void add_segment(int l, int r, ll a, ll b) {
		// verfiy : https://yukicoder.me/problems/no/704

		chmax(l, 0); chmin(r, n);
		if (l >= r) return;

		if (max_flag) { a *= -1; b *= -1; }
		add_segment(1, 0, n, l, r, a, b);
	}

	// y[l] = y0, y[l+1] = y1 なる線分を区間 x∈[l..r) に追加する．
	void add_segment_to_right(int l, int r, ll y0, ll y1) {
		// a l + b = y0, a(l+1) + b = y1 を解いて a, b を求める．
		ll a = y1 - y0;
		ll b = y0 - a * l;
		add_segment(l, r, a, b);
	}

	// y[r] = y0, y[r-1] = y1 なる線分を区間 x∈(l..r] に追加する．
	void add_segment_to_left(int r, int l, ll y0, ll y1) {
		// a r + b = w0, a(r-1) + b = w1 を解いて a, b を求める．
		ll a = y0 - y1;
		ll b = y0 - a * r;
		add_segment(l + 1, r + 1, a, b);
	}

	// x を定義域に含む線分 a x + b らの最小値[最大値] を返す．
	ll get(int x) const {
		// verfiy : https://yukicoder.me/problems/no/704

		ll y = get(1, 0, n, x);
		if (max_flag) y *= -1;
		return y;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Li_Chao_tree seg) {
		rep(i, seg.actual_n) {
			os << seg.get(i) << " ";
		}
		return os;
	}
#endif
};


//【動的 Li Chao Tree】
/*
* Dynamic_Li_Chao_tree<T>(ll x_min, ll x_max, bool max_flag = false) : O(1)
*	直線 y = 0 x + INFL[-INFL] (x∈[x_min..x_max]) で初期化する．
*	max_flag = false[true] なら最小値[最大値] クエリに対応する．
*
* add_line(T a, T b) : O(log n)
*	直線 a x + b (x∈[0..n)) を追加する．
*
* add_segment(ll l, ll r, T a, T b) : O((log n)^2)
*	線分 a x + b (x∈[l..r)) を追加する．
*
* add_segment_right(ll l, ll r, T w0, T w1) : O((log n)^2)
*	x∈[l..r) に (x, y) = (l, w0), (l+1, w1) を通る線分を追加する．
*
* add_segment_left(ll r, ll l, T w0, T w1) : O((log n)^2)
*	x∈(l..r] に (x, y) = (r, w0), (r-1, w1) を通る線分を追加する．
*
* T get(ll x) : O(log n)
*	x を定義域に含む線分 a x + b らの最小値[最大値] を返す．
*/
template <class T = ll>
class Dynamic_Li_Chao_tree {
	// 参考 : https://smijake3.hatenablog.com/entry/2018/06/16/144548

	struct Node {
		T a, b; // 線分の傾きと切片
		Node* l, * r;

		Node(T a, T b) : a(a), b(b), l(nullptr), r(nullptr) {}
	};

	ll x_min, x_max;
	bool max_flag;
	Node* root;

	// 区間 [L..R) に対応する部分木 t に線分 a x + b (x∈[l..r)) を追加する．
	void add_segment(Node*& t, ll L, ll R, ll l, ll r, T a, T b) {
		if (L == R) return;

		// [L..R) が [l..r) と共通部分をもたない場合，何もせず終了．
		if (r <= L || R <= l) return;

		// ノードが存在しなかった場合は新たに作成する（短絡していないので遅い）
		if (!t) t = new Node(0, T(INFL));

		// [L..R) の中央
		ll M = (L + R) / 2;

		// [L..R) が [l..r) に包含されていない場合
		if (L < l || r < R) {
			// 左右の区間それぞれに対して再帰的に処理を行う．
			add_segment(t->l, L, M, l, r, a, b);
			add_segment(t->r, M, R, l, r, a, b);
			return;
		}

		// [L..R) が [l..r) に包含されている場合

		// 記録されている直線 L0 の L, R での値
		T yL0 = t->a * L + t->b, yR0 = t->a * R + t->b;

		// 追加しようとしている直線 L1 の L, R での値
		T yL1 = a * L + b, yR1 = a * R + b;

		// L1 が L0 の上側にある場合，L1 は追加する意味がないので何もせず終了．
		if (yL1 >= yL0 && yR1 >= yR0) return;

		// L1 が L0 の下側にある場合，L0 を捨てて L1 に取り替え終了．
		if (yL1 <= yL0 && yR1 <= yR0) {
			t->a = a; t->b = b;
			return;
		}

		// 記録されている直線 L0 の M での値
		T yM0 = t->a * M + t->b;

		// 追加しようとしている直線 L1 の M での値
		T yM1 = a * M + b;

		// [M..R) で L1 が L0 の上側にある場合，[L..M) の探索のみを進める．
		if (yM1 >= yM0 && yR1 >= yR0) {
			add_segment(t->l, L, M, l, r, a, b);
			return;
		}

		// [L..M) で L1 が L0 の上側にある場合，[M..R) の探索のみを進める．
		if (yL1 >= yL0 && yM1 >= yM0) {
			add_segment(t->r, M, R, l, r, a, b);
			return;
		}

		// [M..R) で L1 が L0 の下側にある場合，L1 と L0 を交換して [L..M) の探索のみを進める．
		if (yM1 <= yM0 && yR1 <= yR0) {
			swap(t->a, a); swap(t->b, b);
			add_segment(t->l, L, M, l, r, a, b);
			return;
		}

		// [L..M) で L1 が L0 の下側にある場合，L1 と L0 を交換して [M..R) の探索のみを進める．
		if (yL1 <= yL0 && yM1 <= yM0) {
			swap(t->a, a); swap(t->b, b);
			add_segment(t->r, M, R, l, r, a, b);
			return;
		}
	}

	// 区間 [L..R) に対応する部分木 i を定義域に含む線分 a x + b らの最小値を返す．
	T get(Node* t, ll L, ll R, ll x) const {
		if (!t) return T(INFL);

		// [L..R) の中央
		ll M = (L + R) / 2;

		T y = t->a * x + t->b;
		if (x < M) chmin(y, get(t->l, L, M, x));
		else chmin(y, get(t->r, M, R, x));

		return y;
	}

public:
	// 直線 y = 0 x + INFL (x∈[0..n)) で初期化する．
	Dynamic_Li_Chao_tree(ll x_min, ll x_max, bool max_flag = false)
		: x_min(x_min), x_max(x_max), max_flag(max_flag), root(nullptr) {
		// verify : https://judge.yosupo.jp/problem/segment_add_get_min
	}
	Dynamic_Li_Chao_tree() : x_min(-INF), x_max(INF), max_flag(false), root(nullptr) {}

	// 直線 a x + b (x∈[0..n)) を追加する．
	void add_line(T a, T b) {
		if (max_flag) { a *= -1; b *= -1; }
		add_segment(root, x_min, x_max + 1, x_min, x_max + 1, a, b);
	}

	// 線分 a x + b (x∈[l..r)) を追加する．
	void add_segment(ll l, ll r, T a, T b) {
		// verify : https://judge.yosupo.jp/problem/segment_add_get_min

		chmax(l, x_min); chmin(r, x_max + 1);
		if (l >= r) return;

		if (max_flag) { a *= -1; b *= -1; }
		add_segment(root, x_min, x_max + 1, l, r, a, b);
	}

	// x∈[l..r) に (x, y) = (l, w0), (l+1, w1) を通る線分を追加する．
	void add_segment_right(ll l, ll r, T w0, T w1) {
		// verify : https://www.codechef.com/problems/STREETTA

		// a l + b = w0, a(l+1) + b = w1 を解いて a, b を求める．
		T a = w1 - w0;
		T b = w0 - a * l;
		add_segment(l, r, a, b);
	}

	// x∈(l..r] に (x, y) = (r, w0), (r-1, w1) を通る線分を追加する．
	void add_segment_left(ll r, ll l, T w0, T w1) {
		// verify : https://www.codechef.com/problems/ARCTR

		// a r + b = w0, a(r-1) + b = w1 を解いて a, b を求める．
		T a = w0 - w1;
		T b = w0 - a * r;
		add_segment(l + 1, r + 1, a, b);
	}

	// x を定義域に含む線分 a x + b らの最小値[最大値] を返す．
	T get(ll x) const {
		// verify : https://judge.yosupo.jp/problem/segment_add_get_min

		T y = get(root, x_min, x_max + 1, x);
		if (max_flag) y *= -1;
		return y;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Dynamic_Li_Chao_tree seg) {
		repi(x, seg.x_min, seg.x_max) {
			os << seg.get(x) << " ";
		}
		return os;
	}
#endif
};


//【Li Chao Tree（1 交差関数群）】
/*
* Li_Chao_tree_1cross_func<P, T>(int n, function<T(P p, int x)> f, P p) : O(n)
*	関数 y = f(p; x) (x∈[0..n)) のみで初期化する．
*	関数はパラメータ p で表し，x における値は f(p, x) で与えられる．
*	注意：座圧する場合ははみ出しに注意．余分な座標を持って 2 倍に延長しておくと良い．
*
* add_function(P p) : O(log n)
*	関数 y = f(p; x) (x∈[0..n)) を追加する．
*	制約：他の関数との交差は高々 1 回
*
* add_function(int l, int r, P p) : O((log n)^2)
*	部分関数 y = f(p; x) (x∈[l..r)) を追加する．
*	制約：他の関数との交差は高々 1 回
*
* T get(int x) : O(log n)
*	x を定義域に含む関数 y = f(p; x) らの最小値を返す．
*/
template <class P, class T = ll>
class Li_Chao_tree_1cross_func {
	// 参考 : https://smijake3.hatenablog.com/entry/2018/06/16/144548

	//【備考】
	// f(p; 0) 昇順に関数を並べ替え，x∈[0..n) における関数値を列ベクトルにもつ行列を作ると，
	// その行列は totally monotone 性をもつ．逆もしかり．
	//
	// 行列 a[0..n)[0..m) が totally monotone であるとは，任意の i1 < i2, j1 < j2 について
	//		[ a[i1][j1] > a[i1][j2] ] 
	//		[ a[i2][j1] < a[i2][j2] ] ではない
	// を満たすことをいう（Monge のように 2x2 小行列を全チェックするだけではだめなので注意）

	int n; // 完全二分木の葉の数（必ず 2 冪）
	int actual_n; // 実際の要素数

	// パラメータ p を元に x における関数値 f(p; x) を計算する．
	function<T(P p, int x)> f;

	// ps[i] : ノード i に対応する区間全体で最小となる関数を表すパラメータ
	vector<P> ps;

	// 区間 [L..R) に対応するノード i とその部分木に関数 y = f(p; x) (x∈[l..r)) を追加する．
	void add_function(int i, int L, int R, int l, int r, const P& p) {
		if (i >= 2 * n) return;

		// [L..R) が [l..r) と共通部分をもたない場合，何もせず終了．
		if (r <= L || R <= l) return;

		// [L..R) の中央
		int M = (L + R) / 2;

		// [L..R) が [l..r) に包含されていない場合
		if (L < l || r < R) {
			// 左右の区間それぞれに対して再帰的に処理を行う．
			add_function(2 * i, L, M, l, r, p);
			add_function(2 * i + 1, M, R, l, r, p);
			return;
		}

		// [L..R) が [l..r) に包含されている場合

		// 記録されている関数 F0 の L, R での値
		T yL0 = f(ps[i], L), yR0 = f(ps[i], R);

		// 追加しようとしている関数 F1 の L, R での値
		T yL1 = f(p, L), yR1 = f(p, R);

		// F1 が F0 の上側にある場合，F1 は追加する意味がないので何もせず終了．
		if (yL1 >= yL0 && yR1 >= yR0) return;

		// F1 が F0 の下側にある場合，F0 を捨てて F1 に取り替え終了．
		if (yL1 <= yL0 && yR1 <= yR0) { ps[i] = p; return; }

		// 記録されている関数 F0 の M での値
		T yM0 = f(ps[i], M);

		// 追加しようとしている関数 F1 の M での値
		T yM1 = f(p, M);

		// [M..R) で F1 が F0 の上側にある場合，[L..M) の探索のみを進める．
		if (yM1 >= yM0 && yR1 >= yR0) {
			add_function(2 * i, L, M, l, r, p);
			return;
		}

		// [L..M) で F1 が F0 の上側にある場合，[M..R) の探索のみを進める．
		if (yL1 >= yL0 && yM1 >= yM0) {
			add_function(2 * i + 1, M, R, l, r, p);
			return;
		}

		// [M..R) で F1 が F0 の下側にある場合，F1 と F0 を交換して [L..M) の探索のみを進める．
		if (yM1 <= yM0 && yR1 <= yR0) {
			auto pi(ps[i]); ps[i] = p;
			add_function(2 * i, L, M, l, r, pi);
			return;
		}

		// [L..M) で F1 が F0 の下側にある場合，F1 と F0 を交換して [M..R) の探索のみを進める．
		if (yL1 <= yL0 && yM1 <= yM0) {
			auto pi(ps[i]); ps[i] = p;
			add_function(2 * i + 1, M, R, l, r, pi);
			return;
		}
	}

	// 区間 [L..R) に対応するノード i とその部分木に記録されている関数 y = f(x) らの最小値を返す．
	T get(int i, int L, int R, int x) const {
		if (i >= 2 * n) return INFL;

		// [L..R) の中央
		int M = (L + R) / 2;

		T y = f(ps[i], x);
		if (x < M) chmin(y, get(2 * i, L, M, x));
		else chmin(y, get(2 * i + 1, M, R, x));

		return y;
	}

public:
	// 関数 y = f(p; x) (x∈[0..n)) のみで初期化する．
	Li_Chao_tree_1cross_func(int n_, const function<T(P p, int x)>& f, const P& p)
		: actual_n(n_), n(1 << (msb(n_ - 1) + 1)), f(f), ps(2 * n, p)
	{
		// verify : https://yukicoder.me/problems/no/705
	}
	Li_Chao_tree_1cross_func() : n(0), actual_n(0) {}

	// 関数 y = f(p; x) (x∈[0..n)) を追加する．
	void add_function(const P& p) {
		// verify : https://yukicoder.me/problems/no/705

		add_function(1, 0, n, 0, n, p);
	}

	// 部分関数 y = f(p; x) (x∈[l..r)) を追加する．
	void add_function(int l, int r, const P& p) {
		chmax(l, 0); chmin(r, n);
		if (l >= r) return;

		add_function(1, 0, n, l, r, p);
	}

	// x を定義域に含む関数 y = f(p; x) らの最小値を返す．
	T get(int x) const {
		// verify : https://yukicoder.me/problems/no/705

		return get(1, 0, n, x);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Li_Chao_tree_1cross_func seg) {
		rep(i, seg.actual_n) {
			os << seg.get(i) << " ";
		}
		return os;
	}
#endif
};


//【動的 Li Chao Tree（1 交差関数群）】
/*
* Dynamic_Li_Chao_tree_1cross_func<P, T>(ll x_min, ll x_max, function<T(P p, ll x)> f, P p) : O(1)
*	関数 y = f(p; x) (x∈[x_min..x_max]) のみで初期化する．
*	関数はパラメータ p で表し，x における値は f(p, x) で与えられる．
*
* add_function(P p) : O(log n)
*	関数 y = f(p; x) (x∈[x_min..x_max]) を追加する．
*	制約：他の関数との交差は高々 1 回
*
* add_function(ll l, ll r, P p) : O((log n)^2)
*	部分関数 y = f(p; x) (x∈[l..r)) を追加する．
*	制約：他の関数との交差は高々 1 回
*
* T get(ll x) : O(log n)
*	x を定義域に含む関数 y = f(p; x) らの最小値を返す．
*/
template <class P, class T = ll>
class Dynamic_Li_Chao_tree_1cross_func {
	// 参考 : https://smijake3.hatenablog.com/entry/2018/06/16/144548

	struct Node {
		P p;
		Node* l, * r;

		Node(P p) : p(p), l(nullptr), r(nullptr) {}
	};

	ll x_min, x_max;
	Node* root;

	// パラメータ p を元に x における関数値 f(p; x) を計算する．
	function<T(P p, ll x)> f;

	P p_ini;

	// 区間 [L..R) に対応する部分木 t に関数 f(p; x) (x∈[l..r)) を追加する．
	void add_function(Node*& t, ll L, ll R, ll l, ll r, P p) {
		if (L == R) return;

		// [L..R) が [l..r) と共通部分をもたない場合，何もせず終了．
		if (r <= L || R <= l) return;

		// ノードが存在しなかった場合は新たに作成する（短絡していないので遅い）
		if (!t) t = new Node(p_ini);

		// [L..R) の中央
		ll M = (L + R) / 2;

		// [L..R) が [l..r) に包含されていない場合
		if (L < l || r < R) {
			// 左右の区間それぞれに対して再帰的に処理を行う．
			add_function(t->l, L, M, l, r, p);
			add_function(t->r, M, R, l, r, p);
			return;
		}

		// [L..R) が [l..r) に包含されている場合

		// 記録されている関数 F0 の L, R での値
		T yL0 = f(t->p, L), yR0 = f(t->p, R);

		// 追加しようとしている関数 F1 の L, R での値
		T yL1 = f(p, L), yR1 = f(p, R);

		// F1 が F0 の上側にある場合，F1 は追加する意味がないので何もせず終了．
		if (yL1 >= yL0 && yR1 >= yR0) return;

		// F1 が F0 の下側にある場合，F0 を捨てて F1 に取り替え終了．
		if (yL1 <= yL0 && yR1 <= yR0) {
			t->p = p;
			return;
		}

		// 記録されている関数 F0 の M での値
		T yM0 = f(t->p, M);

		// 追加しようとしている直線 F1 の M での値
		T yM1 = f(p, M);

		// [M..R) で F1 が F0 の上側にある場合，[L..M) の探索のみを進める．
		if (yM1 >= yM0 && yR1 >= yR0) {
			add_function(t->l, L, M, l, r, p);
			return;
		}

		// [L..M) で F1 が F0 の上側にある場合，[M..R) の探索のみを進める．
		if (yL1 >= yL0 && yM1 >= yM0) {
			add_function(t->r, M, R, l, r, p);
			return;
		}

		// [M..R) で F1 が F0 の下側にある場合，F1 と F0 を交換して [L..M) の探索のみを進める．
		if (yM1 <= yM0 && yR1 <= yR0) {
			swap(t->p, p);
			add_function(t->l, L, M, l, r, p);
			return;
		}

		// [L..M) で F1 が F0 の下側にある場合，F1 と F0 を交換して [M..R) の探索のみを進める．
		if (yL1 <= yL0 && yM1 <= yM0) {
			swap(t->p, p);
			add_function(t->r, M, R, l, r, p);
			return;
		}
	}

	// 区間 [L..R) に対応する部分木 i を定義域に含む関数 y = f(p; x) らの最小値を返す．
	T get(Node* t, ll L, ll R, ll x) const {
		// [L..R) の中央
		ll M = (L + R) / 2;

		T y = f(t->p, x);
		if (x < M) {
			if (t->l) chmin(y, get(t->l, L, M, x));
		}
		else {
			if (t->r) chmin(y, get(t->r, M, R, x));
		}

		return y;
	}

public:
	//  関数 y = f(p; x) (x∈[x_min..x_max])) のみで初期化する．
	Dynamic_Li_Chao_tree_1cross_func(ll x_min, ll x_max, const function<T(P p, ll x)>& f, const P& p)
		: x_min(x_min), x_max(x_max), f(f), p_ini(p), root(nullptr) {
		// verify : https://yukicoder.me/problems/no/2698
	}
	Dynamic_Li_Chao_tree_1cross_func() : x_min(-INFL), x_max(INFL), root(nullptr) {}

	// 関数 y = f(p; x) (x∈[x_min..x_max]) を追加する．
	void add_function(const P& p) {
		// verify : https://yukicoder.me/problems/no/2698

		add_function(root, x_min, x_max + 1, x_min, x_max + 1, p);
	}

	// 部分関数 y = f(p; x) (x∈[l..r)) を追加する．
	void add_function(ll l, ll r, const P& p) {
		chmax(l, x_min); chmin(r, x_max + 1);
		if (l >= r) return;

		add_function(root, x_min, x_max + 1, l, r, p);
	}

	// x を定義域に含む関数 y = f(p; x) らの最小値を返す．
	T get(ll x) const {
		// verify : https://yukicoder.me/problems/no/2698

		if (!root) return f(p_ini, x);
		T y = get(root, x_min, x_max + 1, x);
		return y;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Dynamic_Li_Chao_tree_1cross_func seg) {
		repi(x, seg.x_min, seg.x_max) {
			os << seg.get(x) << " ";
		}
		return os;
	}
#endif
};


//【線分群の最小値（傾きの種類が少ない）】
/*
* Segment_min(int n, int a_min, int a_max, bool max_flag = false) : O(n (a_max - a_min))
*	v[0..n) = INFL で初期化する．set クエリで与える a の範囲は [a_min..a_max] とする．
*	max_flag = false[true] なら最小値[最大値] クエリに対応する．
*
* Segment_min(vl v, int a_min, int a_max, bool max_flag = false) : O(n (a_max - a_min))
*	配列 v[0..n) で初期化する．
*
* add_line(ll a, ll b) : O(log n)
*	直線 a x + b (x∈[0..n)) を追加する．
*
* add_segment(int a, ll b, int l, int r) : O(log n)
*	線分 a x + b (x∈[l..r)) を追加する．
*
* ll get(int x) : O(log n (a_max - a_min))
*	x を定義域に含む線分 a x + b らの最小値[最大値] を返す．
*/
ll op_rmq(ll x, ll y) { return min(x, y); }
ll e_rmq() { return INFL; }
ll mapping_rmq(ll f, ll x) { return min(f, x); }
ll composition_rmq(ll f, ll g) { return min(f, g); }
ll id_rmq() { return INFL; }
struct Segment_min {
	// 内部では値 v[i] を一次の項の係数 a で分けて
	//		min(a[1] i + b[1], a[2] i + b[2], ...)
	// の形で保持する．a が同じであれば単に b の minに帰着できる．

	int n;
	int a_min, a_max;
	bool max_flag;

	using rmq = lazy_segtree<ll, op_rmq, e_rmq, ll, mapping_rmq, composition_rmq, id_rmq>;
	vector<rmq> segs;

	// v[0..n) = INFL で初期化する．set クエリで与える a の範囲は [a_min..a_max] とする．
	Segment_min(int n, int a_min_, int a_max_, bool max_flag = false)
		: n(n), max_flag(max_flag), segs(a_max_ - a_min_ + 1, rmq(n))
	{
		// verify : https://atcoder.jp/contests/abc216/tasks/abc216_g

		if (!max_flag) { a_min = a_min_; a_max = a_max_; }
		else { a_min = -a_max_; a_max = -a_min_; }
	}

	// 配列 v[0..n) で初期化する．
	Segment_min(const vl& v, int a_min_, int a_max_, bool max_flag = false)
		: n(sz(v)), max_flag(max_flag), segs(a_max_ - a_min_ + 1, rmq(n))
	{
		if (!max_flag) { a_min = a_min_; a_max = a_max_; }
		else { a_min = -a_max_; a_max = -a_min_; }

		vl b(n);
		if (!max_flag) rep(i, n) b[i] = v[i] - a_min * i;
		else rep(i, n) b[i] = -v[i] - a_min * i;
		segs[a_min - a_min] = rmq(b);
	}

	// 線分 a x + b (x∈[l..r)) を追加する．
	void add_segment(int a, ll b, int l, int r) {
		// verify : https://atcoder.jp/contests/abc216/tasks/abc216_g

		chmax(l, 0); chmin(r, n);
		if (l >= r) return;

		if (max_flag) { a *= -1; b *= -1; }
		Assert(a_min <= a && a <= a_max);

		segs[a - a_min].apply(l, r, b);
	}

	// 直線 a x + b (x∈[0..n)) を追加する．
	void add_line(int a, ll b) {
		add_segment(a, b, 0, n);
	}

	// x を定義域に含む線分 a x + b らの最小値[最大値] を返す．
	ll get(int x) {
		// verify : https://atcoder.jp/contests/abc216/tasks/abc216_g

		ll y = INFL;
		repi(a, a_min, a_max) {
			ll b = segs[a - a_min].get(x);
			if (b != INFL) chmin(y, (ll)a * x + b);
		}
		if (max_flag) y *= -1;
		return y;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Segment_min seg) {
		rep(i, seg.n) {
			os << seg.get(i) << " ";
		}
		return os;
	}
#endif
};


