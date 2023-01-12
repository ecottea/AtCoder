#pragma once
#include "header.h"
// ■■■■■ 関数 ■■■■■


//【Convex-Hull Trick（挿入単調，クエリ単調）】
/*
* Convex_hull_trick_monotonous() : O(1)
*	空で初期化する．
*
* insert(pll l) : ならし O(1)
*	l = {a, b} が表す直線 y = a x + b を追加する．
*	制約：呼び出す際の挿入する直線の傾き a は狭義降順でなくてはならない．
*
* ll min(ll x) : ならし O(1)
*	a x + b の最小値を返す．
*	制約：呼び出す際の x 座標は狭義昇順でなくてはならない．
*/
struct Convex_hull_trick_monotonous {
	// 参考 : https://satanic0258.hatenablog.com/entry/2016/08/16/181331
	// verify : https://atcoder.jp/contests/dp/tasks/dp_z

	int n; // 記録している直線の本数
	deque<pll> lines; // 直線を傾き狭義降順に記録したデック

	Convex_hull_trick_monotonous() : n(0) {}

	void insert(pll l) {
		ll a3, b3;
		tie(a3, b3) = l;

		while (n >= 2) {
			ll a1, b1, a2, b2;
			tie(a1, b1) = lines[n - 2];
			tie(a2, b2) = lines[n - 1];

			// y = a2 x + b2 が必要な直線なら消去をここまでにする．
			// 積をとってしまっているのでオーバーフローに注意．
			if ((a1 - a2) * (b3 - b2) > (a2 - a3) * (b2 - b1)) {
				break;
			}
			lines.pop_back();
			n--;
		}
		lines.push_back(l);
		n++;
	}

	ll min(ll x) {
		while (n >= 2) {
			ll a1, b1, a2, b2;
			tie(a1, b1) = lines[0];
			tie(a2, b2) = lines[1];

			if (a1 * x + b1 < a2 * x + b2) {
				break;
			}
			lines.pop_front();
			n--;
		}

		ll a, b;
		tie(a, b) = lines[0];
		return a * x + b;
	}
};


//【Convex-Hull Trick】
/*
* Convex_hull_trick<T>(bool min_flag = true) : O(1)
*	空で初期化する．min_flag = true[false] なら最小値[最大値] クエリに対応する．
*
* insert(T a, T b) : ならし O(log n)
*	直線 y = a x + b を追加する．
*
* T get(T x) : O(log n)
*	a x + b の最小値[最大値] を返す．
*	制約：直線集合は空でない
*/
template <class T>
class Convex_hull_trick {
	// 参考(理屈)：https://satanic0258.hatenablog.com/entry/2016/08/16/181331
	// 参考(実装)：https://github.com/satanic0258/Cpp_snippet/blob/master/src/technique/ConvexHullTrick.cpp

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

			if (l.a == 1) os << "x";
			else if (l.a == 0);
			else if (l.a == -1) os << "-x";
			else os << l.a << "x";

			if (l.a == 0 || l.b < 0) os << l.b;
			else if (l.b > 0) os << "+" << l.b;

			return os;
		}
#endif
	};

	set<Line> lines; // 直線を傾き狭義降順に記録した集合

	// 最小値クエリに対応する場合は true，最大値クエリに対応する場合は false
	bool min_flag;

public:
	// 空で初期化する．
	Convex_hull_trick(bool min_flag = true) : min_flag(min_flag) {}

	// 直線 l : y = a x + b を追加する．
	void insert(T a, T b) {
		// verify : https://judge.yosupo.jp/problem/line_add_get_min

		// 最大値クエリに対応する場合は -1 倍して上下反転し，最小値クエリとして扱う．
		if (!min_flag) {
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

		if (min_flag) return it->a * x + it->b;
		else return -(it->a * x + it->b); // 最大値クエリの場合は -1 倍していたので元に戻す．
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Convex_hull_trick& cht) {
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
* ll min(pll* lr) : O(1)
*	min f(x) を返し，必要ならそれを与える x の範囲 [l, r] を lr に格納する．
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
*/
struct Slope_trick {
	// 参考 : https://maspypy.com/slope-trick-1-%E8%A7%A3%E8%AA%AC%E7%B7%A8

	ll y_min; // 最小値
	priority_queue<ll> l; // 最小値より左の折れ点の x 座標を降順に取り出せるキュー
	priority_queue_rev<ll> r; // 最小値より右の折れ点の x 座標を昇順に取り出せるキュー
	ll add_l; // 最小値より左側の平行移動量
	ll add_r; // 最小値より右側の平行移動量

	// f(x) = 0 で初期化する．
	Slope_trick() : y_min(0), add_l(0), add_r(0) {
		// 番兵を挿入しておく．
		l.push(-INFL);
		r.push(INFL);
	};

	// min f(x) を返し，必要ならそれを与える x の範囲 [l, r] を lr に格納する．
	ll min(pll* lr = nullptr) {
		// verify : https://atcoder.jp/contests/abc127/tasks/abc127_f

		if (lr != nullptr) *lr = { l.top() + add_l, r.top() + add_r };
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

		y_min += max(0LL, (l.top() + add_l) - x0);
		l.push(x0 - add_l);
		r.push((l.top() + add_l) - add_r);
		l.pop();
	}

	// f(x) += min(x0 - x, 0) とする．（＼＿ の形を加算する．）
	void add_left(ll x0) {
		// verify : https://atcoder.jp/contests/abc217/tasks/abc217_h

		y_min += max(0LL, x0 - (r.top() + add_r));
		r.push(x0 - add_r);
		l.push((r.top() + add_r) - add_l);
		r.pop();
	}

	// f(x) += |x - x0| とする．（＼／ の形を加算する．）
	void add_abs(ll x0) {
		// verify : https://atcoder.jp/contests/abc127/tasks/abc127_f

		add_right(x0);
		add_left(x0);
	}

	// f(x) を左から累積最小値をとったものに置き換える．（＼＿ の形にする．）
	void acc_min_left() {
		r = priority_queue_rev<ll>();
		r.push(INFL);
	}

	// f(x) を右から累積最小値をとったものに置き換える．（＿／ の形にする．）
	void acc_min_right() {
		l = priority_queue<ll>();
		l.push(-INFL);
	}

	// f(x) を x0 だけ右に平行移動する．（f(x) ← f(x - x0)）
	void shift(ll x0) {
		add_l += x0;
		add_r += x0;
	}

	// f(x) を min f([x-dl, x+dr]) に置き換える．（＼＿＿／ の形にする．）
	void sliding_window_min(ll dl, ll dr) {
		// verify : https://atcoder.jp/contests/abc217/tasks/abc217_h

		add_l -= dl;
		add_r += dr;
	}
};


//【区分線形凸関数】
/*
* Convex_function() : O(1)
*	定数関数 f(x) = 0 で初期化する．
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


