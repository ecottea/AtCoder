#pragma once
#include "header.h"
#include "構造(幾何).h"
#include "計量.h"
// ■■■■■ 判定 ■■■■■


//【点と有向線分の位置関係】O(1)
/*
* 点 p と有向線分 s = a → b の位置関係を返す．
*
* 戻り値：
*	 1 : p が s の左側にある場合（a → b → p が反時計回り）
*	-1 : p が s の右側にある場合（a → b → p が時計回り）
*	 2 : p が s の b より前にある場合（a < b < p 順）
*	-2 : p が s の a より後ろにある場合（p < a < b 順）
*	 0 : p が s 上にある場合（a <= p <= b 順）
*/
template <typename T> inline int ccw(const Point<T>& p, const Line<T>& s) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_1_C

	auto op = (s.second - s.first).cross(p - s.first);
	if (op > 0) {
		// p が s の左側にある
		return 1;
	}
	else if (op < 0) {
		// p が s の右側にある
		return -1;
	}
	else {
		if ((s.first - s.second).dot(p - s.second) < 0) {
			// p が s の前にある
			return 2;
		}
		else if ((s.second - s.first).dot(p - s.first) < 0) {
			// p が s の後ろにある
			return -2;
		}
		else {
			// p が s 上にある
			return 0;
		}
	}
}


//【共有判定（直線と直線）】O(1)
/*
* 直線 l1 と直線 l2 が共有点をもつなら true，さもなくば false を返す．
*/
template <typename T>
inline bool intersectQ_L_L(const Line<T>& l1, const Line<T>& l2) {
	// 共有点をもつ
	// ⇔ 平行でない，または，同一直線
	return (l1.second - l1.first).cross(l2.second - l2.first) != 0
		|| (l1.second - l1.first).cross(l2.second - l1.first) == 0;
}


//【共有判定（直線と閉線分）】O(1)
/*
* 直線 l と閉線分 s が共有点をもつなら true，さもなくば false を返す．
*/
template <typename T>
inline bool intersectQ_L_CS(const Line<T>& l, const Line<T>& s) {
	// 共有点をもつ
	// ⇔ s[0] と s[1] が l について逆側
	T op0 = (l.second - l.first).cross(s.first - l.first);
	T op1 = (l.second - l.first).cross(s.second - l.first);

	return (op0 >= 0 && op1 <= 0) || (op0 <= 0 && op1 >= 0);
}


//【共有判定（直線と半開有向線分）】O(1)
/*
* 直線 l と半開有向線分 s = [a → b) が共有点をもつなら true，さもなくば false を返す．
*/
template <typename T>
inline bool intersectQ_L_HS(const Line<T>& l, const Line<T>& s) {
	// 共有点をもつ
	// ⇔ a と b が l について逆側，または，a が l 上
	T op0 = (l.second - l.first).cross(s.first - l.first);
	T op1 = (l.second - l.first).cross(s.second - l.first);

	return (op0 > 0 && op1 < 0) || (op0 < 0 && op1 > 0) || (op0 == 0);
}


//【共有判定（直線と開線分）】O(1)
/*
* 直線 l と開線分 s が共有点をもつなら true，さもなくば false を返す．
*/
template <typename T>
inline bool intersectQ_L_OS(const Line<T>& l, const Line<T>& s) {
	// 共有点をもつ
	// ⇔ s[0] と s[1] が l について逆側，または，(直線 s) = l
	T op0 = (l.second - l.first).cross(s.first - l.first);
	T op1 = (l.second - l.first).cross(s.second - l.first);

	return (op0 > 0 && op1 < 0) || (op0 < 0 && op1 > 0)
		|| (op0 == 0 && op1 == 0);
}


//【共有判定（閉線分と閉線分）】O(1)
/*
* 閉線分 s1 と閉線分 s2 が共有点をもつなら true，さもなくば false を返す．
* 
* 利用：【点と有向線分の位置関係】
*/
template <typename T>
inline bool intersectQ_CS_CS(const Line<T>& s1, const Line<T>& s2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_2_B

	// 共有点をもつ
	// ⇔ (s1 の両端が s2 について逆側，かつ，s2 の両端が s1 について逆側)
	//    または (s1 の端点が s2 上) または (s2 の端点が s1 上)
	//
	// 端点が線分の逆側のとき ccw() の符号が異なり，
	// 端点が線分上のとき ccw() = 0 となるので，綺麗にまとめられる．
	return ccw(s2.first, s1) * ccw(s2.second, s1) <= 0 &&
		ccw(s1.first, s2) * ccw(s1.second, s2) <= 0;
}


//【共有判定（直線と点）】O(1)
/*
* 直線 l と点 p が共有点をもつなら true，さもなくば false を返す．
*/
template <typename T>
inline bool intersectQ_L_P(const Line<T>& l, const Point<T>& p) {
	return (l.second - l.first).cross(p - l.first) == 0;
}


//【共有判定（閉線分と点）】O(1)
/*
* 閉線分 s と点 p が共有点をもつなら true，さもなくば false を返す．
*/
template <typename T>
inline bool intersectQ_CS_P(const Line<T>& s, const Point<T>& p) {
	// ccw(p, s) == 0 としてもいいが，専用に用意しておく．
	if ((s.second - s.first).cross(p - s.first) != 0) return false;
	if ((s.second - s.first).dot(p - s.first) < 0) return false;
	if ((s.first - s.second).dot(p - s.second) < 0) return false;
	return true;
}


//【位置関係（円と円）】O(1)
/*
* 円 c1 と円 c2 の位置関係を調べる．
*
* 戻り値 : c1 と c2 の共通接線の数
*	0 : 内包する場合
*	1 : 内接する場合
*	2 : 交わる場合
*	3 : 外接する場合
*	4 : 離れている場合
*/
template <typename T>
inline int pos_relation_C_C(const Circle<T>& c1, const Circle<T>& c2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_A

	T r1 = c1.second;
	T r2 = c2.second;
	T d_sq = (c1.first - c2.first).sqnorm();

	int res;
	if (d_sq < pow(r1 - r2, 2)) {
		res = 0;
	}
	else if (d_sq == pow(r1 - r2, 2)) {
		res = 1;
	}
	else if (d_sq < pow(r1 + r2, 2)) {
		res = 2;
	}
	else if (d_sq == pow(r1 + r2, 2)) {
		res = 3;
	}
	else {
		res = 4;
	}

	return res;
}


//【点の内外判定（多角形）】O(n)
/*
* n 角形 poly と点 p の内外関係を判定する．
*
* 戻り値：
*	点 p が多角形 poly の外部にあれば -1
*	点 p が多角形 poly の境界にあれば 0
*	点 p が多角形 poly の内部にあれば 1
*/
template <class T>
int inner_polygon(const Polygon<T>& poly, Point<T>& p) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_3_C

	int n = sz(poly);

	int res = -1;

	// p から x 軸正の方向に半直線を伸ばし，多角形との交差回数の偶奇を調べる．
	rep(i, n) {
		// p から多角形の辺の両端までのベクトルを得る．
		Point<T> v1 = poly[i] - p;
		Point<T> v2 = poly[(i + 1) % n] - p;

		// v1 の方が下側になるようにする．
		if (v1.y > v2.y) {
			swap(v1, v2);
		}

		// 半直線と辺が交わっているか判定する．
		// ちょうど半直線が頂点を通っている場合に備えて，
		// 片方の y 座標は <= もう片方は < で判定する．
		if (v1.y <= 0 && v2.y > 0 && v1.cross(v2) < 0) {
			res *= -1;
		}

		// 辺上にあるか判定する．
		if (v1.cross(v2) == 0 && v1.dot(v2) <= 0) {
			res = 0;
			break;
		}
	}

	return res;
}


//【点の内外判定（円，直径指定）】O(1)
/*
* 2 点 a, b を直径の両端にもつ円 C と点 p の内外関係を判定する．
*
* 戻り値：
*	点 p が円 C の外部にあれば -1
*	点 p が円 C の境界にあれば 0
*	点 p が円 C の内部にあれば 1
*/
template <class T>
int inner_circle_by_diameter(const Point<T>& a, const Point<T>& b, const Point<T>& p) {
	// verify : https://atcoder.jp/contests/abc151/tasks/abc151_f

	//【方法】
	// タレスの定理の逆より，∠ a p b の大きさと 90°の大小を比較して判定できる．
	// 代わりに cos(∠ a p b) と cos(90°) = 0 の大小を比較して判定できる．
	// 代わりに内積 (p - a).(p - b) の符号を見て判定できる．

	T ip = (a - p).dot(b - p);

	if (ip > 0) return -1;
	if (ip < 0) return 1;
	return 0;
}


//【点の内外判定（円，3 点指定）】O(1)
/*
* 3 点 a, b, c を通る円 C と点 p の内外関係を判定する．
*
* 戻り値：
*	点 p が円 C の外部にあれば -1
*	点 p が円 C の境界にあれば 0
*	点 p が円 C の内部にあれば 1
*/
template <class T>
int inner_circle_by_3points(const Point<T>& a, const Point<T>& b, const Point<T>& c, const Point<T>& p) {
	// verify : https://atcoder.jp/contests/abc151/tasks/abc151_f

	//【方法】
	// a, b, c, p を複素平面上の点とみなせば，複比による共円判定条件より
	//		(b - c)(a - p) / (a - c)(b - p) ∈ R
	// なら 4 点 a, b, c, p は共円である．そうでないときは，p が無限遠点にあるときの
	//		(b - c) / (a - c)
	// と虚部の符号を比較して，等しいなら外部，異なるなら内部と判定できる．
	//
	// 複素数 num / dnm = (A + B i) / (C + D i) の虚部の符号の判定方法を考える．
	// 分母の実数化を行うと，
	//		(A + B i) / (C + D i)
	//		= (A + B i) (C - D i) / (C^2 + D^2)
	//		= ((A C + B D) + (- A D + B C) i) / (C^2 + D^2)
	// となる．分母は正の実数なので，- A D + B C の符号を見れば良い．

	T num_re = (b.x - c.x) * (a.x - p.x) - (b.y - c.y) * (a.y - p.y);
	T num_im = (b.x - c.x) * (a.y - p.y) + (b.y - c.y) * (a.x - p.x);
	T dnm_re = (a.x - c.x) * (b.x - p.x) - (a.y - c.y) * (b.y - p.y);
	T dnm_im = (a.x - c.x) * (b.y - p.y) + (a.y - c.y) * (b.x - p.x);
	T im = -num_re * dnm_im + num_im * dnm_re;

	if (im == 0) return 0;

	T num2_re = b.x - c.x;
	T num2_im = b.y - c.y;
	T dnm2_re = a.x - c.x;
	T dnm2_im = a.y - c.y;
	T im2 = -num2_re * dnm2_im + num2_im * dnm2_re;

	if ((im > 0) == (im2 > 0)) return -1;
	else return 1;
}


//【凸性判定】O(n)
/*
* n 角形 poly が凸多角形なら true，さもなくば false を返す．
*/
template <typename T>
bool convexQ(const Polygon<T>& poly) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_3_B

	int n = sz(poly);

	bool first_call = true;
	bool b, res = true;

	rep(i, n) {
		T op = (poly[(i + 1) % n] - poly[i]).cross(poly[(i + 2) % n] - poly[i]);
		if (op == 0) {
			continue;
		}

		if (first_call) {
			b = op > 0;
			first_call = false;
		}
		else {
			if (b != (op > 0)) {
				res = false;
				break;
			}
		}
	}

	return res;
}


//【多角形の頂点順】O(n)
/*
* 単純 n 角形 poly の頂点が反時計回りに並んでいたら true，さもなくば false を返す．
*/
template <typename T>
bool ccwQ(const Polygon<T>& poly) {
	int n = sz(poly);

	ll res = 0;
	rep(i, n) {
		res += Point<ll>(poly[i]).cross(Point<ll>(poly[(i + 1) % n]));
	}

	return res > 0;
}


