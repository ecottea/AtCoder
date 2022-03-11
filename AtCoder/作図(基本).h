#pragma once
#include "header.h"
#include "\‘¢(Šô‰½).h"
// ¡¡¡¡¡ ì} ¡¡¡¡¡


//y‰ñ“]zO(1)
/*
* “_ p ‚ğ“_ c ‚ğ’†S‚É th[rad] ‚¾‚¯‰ñ“]‚µ‚½“_‚ğ•Ô‚·D
*/
inline Point<double> rotate(const Point<double>& p, const Point<double>& c, double th) {
	Point<double> q;
	q.x = cos(th) * (p.x - c.x) - sin(th) * (p.y - c.y) + c.x;
	q.y = sin(th) * (p.x - c.x) + cos(th) * (p.y - c.y) + c.y;
	return q;
}


//y90‹‰ñ“]zO(1)
/*
* “_ p ‚ğ“_ c ‚ğ’†S‚É 90‹~ i ‚¾‚¯‰ñ“]‚µ‚½“_‚ğ•Ô‚·D
*/
template <class T>
inline Point<T> rotate90(const Point<T>& p, const Point<T>& c, int i) {
	Point<T> q;
	switch (smod(i, 4)) {
	case 0:
		q.x = 1 * (p.x - c.x) - 0 * (p.y - c.y) + c.x;
		q.y = 0 * (p.x - c.x) + 1 * (p.y - c.y) + c.y;
		break;
	case 1:
		q.x = 0 * (p.x - c.x) - 1 * (p.y - c.y) + c.x;
		q.y = 1 * (p.x - c.x) + 0 * (p.y - c.y) + c.y;
		break;
	case 2:
		q.x = -1 * (p.x - c.x) - 0 * (p.y - c.y) + c.x;
		q.y = 0 * (p.x - c.x) + (-1) * (p.y - c.y) + c.y;
		break;
	case 3:
		q.x = 0 * (p.x - c.x) - (-1) * (p.y - c.y) + c.x;
		q.y = -1 * (p.x - c.x) + 0 * (p.y - c.y) + c.y;
		break;
	default:;
	}
	return q;
}


//yŠp‚Ì“ñ“™•ªüzO(1)
/*
* Ú a o b ‚Ì“ñ“™•ªü‚ğ•Ô‚·D
*/
template <typename T>
inline Line<double> corner_bisector(const Point<T>& a, const Point<T>& o, const Point<T>& b) {
	Point<double> p1 = o;
	Point<double> p2 = p1 + (a - o).normalize() + (b - o).normalize();
	return { p1, p2 };
}


//y‚’¼“ñ“™•ªüzO(1)
/*
* ü•ª l ‚Ì‚’¼“ñ“™•ªü‚ğ•Ô‚·D
*/
template <typename T> inline Line<double> vertical_bisector(const Line<T>& l) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_C

	Point<double> p1 = (l.first + l.second) / 2.;
	Point<T> d = l.second - l.first;
	Point<T> n(-d.y, d.x);
	Point<double> p2 = p1 + n;
	return { p1, p2 };
}


//y‚ü‚Ì‘«zO(1)
/*
* “_ p ‚©‚ç’¼ü l ‚Ö~‚ë‚µ‚½‚ü‚Ì‘«‚ğ•Ô‚·D
*/
inline Point<double> foot_of_perpendicular(const Point<double>& p, const Line<double>& l) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_1_A

	auto d = (l.second - l.first).normalize();
	return l.first + (p - l.first).dot(d) * d;
}


//y‘ÎÌˆÚ“®zO(1)
/*
* ’¼ü l ‚ÉŠÖ‚µ‚Ä“_ p ‚ğ‘ÎÌˆÚ“®‚µ‚½“_‚ğ•Ô‚·D
*/
inline Point<double> symmetrical_move(const Point<double>& p, const Line<double>& l) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_1_B

	auto d = (l.second - l.first).normalize();
	auto v = p - l.first;
	return l.first - v + v.dot(d) * d * 2.0;
}


//y2 ’¼ü‚ÌŒğ“_zO(1)
/*
* 2 ’¼ü l1, l2 ‚ÌŒğ“_‚ğ•Ô‚·D
*/
template <typename T>
Point<double> intersection_L_L(const Line<T>& l1, const Line<T>& l2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_2_C

	double x1 = (double)l1.first.x;
	double y1 = (double)l1.first.y;
	double x2 = (double)l1.second.x;
	double y2 = (double)l1.second.y;
	double x3 = (double)l2.first.x;
	double y3 = (double)l2.first.y;
	double x4 = (double)l2.second.x;
	double y4 = (double)l2.second.y;

	double x_num = x2 * x3 * y1 - x2 * x4 * y1 - x1 * x3 * y2 + x1 * x4 * y2
		- x1 * x4 * y3 + x2 * x4 * y3 + x1 * x3 * y4 - x2 * x3 * y4;
	double y_num = x2 * y1 * y3 - x4 * y1 * y3 - x1 * y2 * y3 + x4 * y2 * y3
		- x2 * y1 * y4 + x3 * y1 * y4 + x1 * y2 * y4 - x3 * y2 * y4;
	double dnm = x3 * y1 - x4 * y1 - x3 * y2 + x4 * y2
		- x1 * y3 + x2 * y3 + x1 * y4 - x2 * y4;

	return { x_num / dnm, y_num / dnm };
}


//y‰~‚Æ’¼ü‚ÌŒğ“_zO(1)
/*
* ‰~ c ‚Æ’¼ü l ‚Ì‹¤—L“_‚ÌŒÂ”‚ğ•Ô‚·D‚Ü‚½‹¤—L“_‚ª‚ ‚ê‚Î‚»‚ÌÀ•W‚ğ p1, p2 ‚ÉŠi”[‚·‚éD
*/
int intersection_C_L(const Circle<ll>& c, const Line<ll>& l, Point<double>& p1, Point<double>& p2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_D
	
	// ‰~ c ‚Ì’†S‚ªŒ´“_‚É‚­‚é‚æ‚¤•½sˆÚ“®
	Point<ll> o = c.first;
	Point<ll> a = l.first - o;
	Point<ll> b = l.second - o;

	// ’¼ü l ‚Ì•ûŒüƒxƒNƒgƒ‹
	Point<ll> d = b - a;

	// (0, 0) ‚Æ l ‚Æ‚Ì•„†•t‚«‹——£‚Ì•ªqC•ª•ê‚Ì“ñæC‰~ c ‚Ì”¼Œa
	ll num = a.cross(b);
	ll dnm_sq = d.sqnorm();
	ll r = c.second;

	// (0, 0) ‚Æ l ‚Æ‚Ì‹——£‚ª‰~‚Ì”¼Œa‚æ‚è‘å‚«‚¢ê‡ ¨ ‹¤—L“_ 0 ŒÂ
	if (dnm_sq != 0 && (num * num - 1) / dnm_sq >= r * r) {
		return 0;
	}

	// (0, 0) ‚Æ l ‚Æ‚Ì•„†•t‚«‹——£
	double dist = num / sqrt(dnm_sq);

	// ‰~ c ‚Ì’†S‚©‚çŒ·‚Ì’†“_‚Ü‚Å‚ÌƒxƒNƒgƒ‹
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (-dist / d.norm());

	// (0, 0) ‚Æ l ‚Æ‚Ì‹——£‚ª‰~‚Ì”¼Œa‚É“™‚µ‚¢ê‡ ¨ Ú“_ 1 ŒÂ
	if (dnm_sq != 0 && num * num / dnm_sq >= r * r) {
		p1 = p2 = Point<double>(o) + nn;
		return 1;
	}

	// Œ·‚Ì’†“_‚©‚çˆê•û‚ÌŒğ“_‚Ü‚Å‚ÌƒxƒNƒgƒ‹
	Point<double> nd = Point<double>(d) * (sqrt(r * r - dist * dist) / d.norm());

	// (0, 0) ‚Æ l ‚Æ‚Ì‹——£‚ª‰~‚Ì”¼Œa‚æ‚è¬‚³‚¢ê‡ ¨ Œğ“_ 2 ŒÂ
	p1 = Point<double>(o) + nn + nd;
	p2 = Point<double>(o) + nn - nd;
	return 2;
}


//y‰~‚Æ‰~‚ÌŒğ“_zO(1)
/*
* 2 ‰~ c1, c2 ‚Ì‹¤—L“_‚ÌŒÂ”‚ğ•Ô‚·D‚Ü‚½‹¤—L“_‚ª‚ ‚ê‚Î‚»‚ÌÀ•W‚ğ p1, p2 ‚ÉŠi”[‚·‚éD
*/
int intersection_C_C(const Circle<ll>& c1, const Circle<ll>& c2, Point<double>& p1, Point<double>& p2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_E
	
	// ‰~ c1, c2 ‚Ì’†S‚Æ”¼Œa
	Point<ll> o1 = c1.first, o2 = c2.first;
	ll r1 = c1.second, r2 = c2.second;

	// o1 ‚©‚ç o2 ‚Ö‚ÌƒxƒNƒgƒ‹C”¼Œa‚Ì˜a‚Æ·
	Point<ll> d = o2 - o1;
	ll r_sum = r1 + r2, r_dif = abs(r1 - r2);

	// ’†SŠÔ‹——£‚ª‰~‚Ì”¼Œa‚Ì˜a‚æ‚è‘å‚«‚¢ê‡ ¨ ‹¤—L“_ 0 ŒÂ
	if (d.sqnorm() > r_sum * r_sum) {
		return 0;
	}

	// ’†SŠÔ‹——£‚ª‰~‚Ì”¼Œa‚Ì˜a‚É“™‚µ‚¢ê‡ ¨ ŠOÚ“_ 1 ŒÂ
	if (d.sqnorm() == r_sum * r_sum) {
		p1 = p2 = Point<double>(o1) + Point<double>(d) * (r1 / d.norm());
		return 1;
	}

	// ’†SŠÔ‹——£‚ª‰~‚Ì”¼Œa‚Ì·‚æ‚è¬‚³‚¢ê‡ ¨ ‹¤—L“_ 0 ŒÂ
	if (d.sqnorm() < r_dif * r_dif) {
		return 0;
	}

	// ’†SŠÔ‹——£‚ª‰~‚Ì”¼Œa‚Ì·‚É“™‚µ‚¢ê‡ ¨ “àÚ“_ 1 ŒÂ
	if (d.sqnorm() == r_dif * r_dif) {
		if (r1 > r2) {
			p1 = p2 = Point<double>(o1) + Point<double>(d) * (r1 / d.norm());
		}
		else {
			p1 = p2 = Point<double>(o1) - Point<double>(d) * (r1 / d.norm());
		}
		return 1;
	}

	// ‚»‚Ì‘¼‚Ìê‡ ¨ Œğ“_ 2 ŒÂ
	double x = (r1 * r1 - r2 * r2 + d.sqnorm()) / (2 * d.norm());
	double h = sqrt(r1 * r1 - x * x);
	Point<double> nd = Point<double>(d) * (x / d.norm());
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (h / d.norm());
	p1 = Point<double>(o1) + nd + nn;
	p2 = Point<double>(o1) + nd - nn;
	return 2;
}


//y‰~‚ÌÚüzO(1)
/*
* “_ p ‚ğ’Ê‚é‰~ c ‚ÌÚü‚Ì–{”‚ğ•Ô‚·D‚Ü‚½Ú“_‚ª‚ ‚ê‚Î‚»‚ÌÀ•W‚ğ t1, t2 ‚ÉŠi”[‚·‚éD
*/
int tangent_to_circle(const Point<ll>& p, const Circle<ll>& c, Point<double>& t1, Point<double>& t2) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_F
	
	// ‰~ c ‚Ì’†S‚Æ”¼Œa
	Point<ll> o = c.first; ll r = c.second;

	// o ‚©‚ç p ‚Ö‚ÌƒxƒNƒgƒ‹C”¼Œa‚Ì˜a‚Æ·
	Point<ll> d = p - o;

	// p ‚Æ o ‚Ì‹——£‚ª‰~‚Ì”¼Œa‚æ‚è¬‚³‚¢ê‡ ¨ Úü 0 –{
	if (d.sqnorm() < r * r) {
		return 0;
	}

	// p ‚Æ o ‚Ì‹——£‚ª‰~‚Ì”¼Œa‚É“™‚µ‚¢ê‡ ¨ Úü 1 –{
	if (d.sqnorm() == r * r) {
		t1 = t2 = p;
		return 1;
	}

	// p ‚Æ o ‚Ì‹——£‚ª‰~‚Ì”¼Œa‚æ‚è‘å‚«‚¢ê‡ ¨ Úü 2 –{
	double x = r * r / d.norm();
	double y = r * sqrt(1 - (double)r * r / d.sqnorm());
	Point<double> nd = Point<double>(d) * (x / d.norm());
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (y / d.norm());
	t1 = Point<double>(o) + nd + nn;
	t2 = Point<double>(o) + nd - nn;
	return 2;
}


//y“ñ‰~‚Ì‹¤’ÊÚüzO(1)
/*
* 2 ‰~ c1, c2 ‚Ì‹¤’ÊÚü‚Ì–{”‚ğ•Ô‚·D‚Ü‚½ c1 ‚Æ‚ÌÚ“_‚ª‚ ‚ê‚Î‚»‚ÌÀ•W‚ğ ts ‚ÉŠi”[‚·‚éD
* c1 = c2 ‚Ì‚Æ‚«‚Í -1 ‚ğ•Ô‚·D
*/
int common_tangent(const Circle<ll>& c1, const Circle<ll>& c2, vector<Point<double>>& ts) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_G
	
	ts.clear();

	if (c1 == c2) return -1;

	// ‰~ c1, c2 ‚Ì’†S‚Æ”¼Œa
	Point<ll> o1 = c1.first; ll r1 = c1.second;
	Point<ll> o2 = c2.first; ll r2 = c2.second;

	// o1 ‚©‚ç o2 ‚Ö‚ÌƒxƒNƒgƒ‹C”¼Œa‚Ì˜a‚Æ·
	Point<ll> d = o2 - o1;
	ll r_sum = r1 + r2, r_dif = abs(r1 - r2);

	// ’†SŠÔ‹——£‚ª‰~‚Ì”¼Œa‚Ì·‚æ‚è¬‚³‚¢ê‡ ¨ ‹¤’ÊÚü 0 –{
	if (d.sqnorm() < r_dif * r_dif) {
		return 0;
	}

	// ’†SŠÔ‹——£‚ª‰~‚Ì”¼Œa‚Ì·‚É“™‚µ‚¢ê‡ ¨ ‹¤’ÊÚü 1 –{
	if (d.sqnorm() == r_dif * r_dif) {
		ts.push_back(Point<double>(o1) + (r1 > r2 ? 1 : -1) * Point<double>(d) * (r1 / d.norm()));
		return 1;
	}

	// ‹¤’ÊŠOÚü‚ğ’Ç‰Á
	double x = r1 * r_dif / d.norm();
	double y = r1 * sqrt(1 - (double)r_dif * r_dif / d.sqnorm());
	Point<double> nd = Point<double>(d) * (x / d.norm());
	Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (y / d.norm());
	ts.push_back(Point<double>(o1) + (r1 > r2 ? 1 : -1) * nd + nn);
	ts.push_back(Point<double>(o1) + (r1 > r2 ? 1 : -1) * nd - nn);

	// ’†SŠÔ‹——£‚ª‰~‚Ì”¼Œa‚Ì˜a‚æ‚è‘å‚«‚¢ê‡ ¨ ‹¤’ÊÚü 4 –{
	if (d.sqnorm() > r_sum * r_sum) {
		// ‹¤’Ê“àÚü‚ğ’Ç‰Á
		double x = r1 * r_sum / d.norm();
		double y = r1 * sqrt(1 - (double)r_sum * r_sum / d.sqnorm());
		Point<double> nd = Point<double>(d) * (x / d.norm());
		Point<double> nn = Point<double>(-(double)d.y, (double)d.x) * (y / d.norm());
		ts.push_back(Point<double>(o1) + nd + nn);
		ts.push_back(Point<double>(o1) + nd - nn);

		return 4;
	}

	// ’†SŠÔ‹——£‚ª‰~‚Ì”¼Œa‚Ì˜a‚É“™‚µ‚¢ê‡ ¨ ‹¤’ÊÚü 3 –{
	if (d.sqnorm() == r_sum * r_sum) {
		ts.push_back(Point<double>(o1) + Point<double>(d) * (r1 / d.norm()));
		return 3;
	}

	// ‚»‚Ì‘¼‚Ìê‡ ¨ ‹¤’ÊÚü 2 –{
	return 2;
}


//y“àÚ‰~zO(1)
/*
* OŠpŒ` a b c ‚Ì“àÚ‰~‚ğ•Ô‚·D
*/
template <typename T>
Circle<double> incircle(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_B

	auto len_a = (b - c).norm();
	auto len_b = (c - a).norm();
	auto len_c = (a - b).norm();
	Point<double> da = a;
	Point<double> db = b;
	Point<double> dc = c;
	auto i = (len_a * da + len_b * db + len_c * dc) / (len_a + len_b + len_c);

	Point<double> d = (b - a).normalize();
	Point<double> n = { -d.y, d.x };
	Point<double> p2 = i - a;
	auto r = abs(p2.dot(n));

	return { i, r };
}


//yŠOÚ‰~zO(1)
/*
* OŠpŒ` a b c ‚ÌŠOÚ‰~‚ğ•Ô‚·D
*
* —˜—pFy‚’¼“ñ“™•ªüzCy2 ’¼ü‚ÌŒğ“_z
*/
template <typename T>
Circle<double> circircle(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all/CGL_7_C

	auto lb = vertical_bisector(Line<double>{ a, b });
	auto lc = vertical_bisector(Line<double>{ a, c });
	auto o = intersection_L_L(lb, lc);
	auto r = (a - o).norm();
	return { o, r };

	/* ”Šw“I‚É‚Í‡‚Á‚Ä‚é‚ªC‹¤ü‚É‹ß‚¢“İŠpOŠpŒ`‚Ì‚Æ‚«‚ÉŒë·‚ª‘å‚«‚­‚È‚éD
	double len_a = (b - c).norm();
	double len_b = (c - a).norm();
	double len_c = (a - b).norm();
	Point<double> da = a;
	Point<double> db = b;
	Point<double> dc = c;
	double w_a = len_a * len_a * (len_b * len_b + len_c * len_c - len_a * len_a);
	double w_b = len_b * len_b * (len_c * len_c + len_a * len_a - len_b * len_b);
	double w_c = len_c * len_c * (len_a * len_a + len_b * len_b - len_c * len_c);
	Point<double> o = (w_a * da + w_b * db + w_c * dc) / (w_a + w_b + w_c);
	double r = (o - a).norm();
	return { o, r };
	*/
}


