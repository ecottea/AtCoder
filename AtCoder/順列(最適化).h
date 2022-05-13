#pragma once
#include "header.h"
// ¡¡¡¡¡ ‡—ñ‚ÌÅ¬‰»[Å‘å‰»] ¡¡¡¡¡


//y—ÝÏ˜a‚ÌÅ‘å’l‚ÌÅ¬‰»zO(n log n)
/*
* —^‚¦‚ç‚ê‚½”ñ•‰®” a[0..n), b[0..n) ‚É‚Â‚¢‚ÄC[0..n) ‚Ì‡—ñ p[0..n) ‚Ì‚¤‚¿C
*	a[p[0]], -b[p[0]], a[p[1]], -b[p[1]], a[p[2]], -b[p[2]], ...
* ‚Ì—ÝÏ˜a‚ÌÅ‘å’l M ‚ðÅ¬‚Æ‚·‚é‚à‚Ì‚ð p ‚ÉŠi”[‚·‚éD‚Ü‚½‚»‚Ì‚Æ‚«‚Ì M ‚ð•Ô‚·D
*/
ll minimize_pair_accumulate(const vl& a, const vl& b, vi* p = nullptr) {
	//y•û–@z
	// a[i] - b[i] ‚Ì•„†‚É‚æ‚è‘g (a[i], b[i]) ‚ð 2 ‚Â‚É•ª‚¯‚éD
	// ‘S‘Ì‚ÅŒ©‚é‚ÆC•‰‚Ì‘g‚ðæC³‚Ì‘g‚ðŒã‚É•À‚×‚é‚Ì‚ªÅ‘P‚Å‚ ‚éD
	//
	// •‰‚Ì‘g‚Ì’†‚Å‚ÍCÅ‘å’l‚ðXV‚µ‚Ä‚µ‚Ü‚¢‚â‚·‚¢ a[i] ‚Ì‘å‚«‚¢‚à‚Ì‚ðŒã‚ë‚É‰ñ‚·‚Ì‚ªÅ‘PD
	// ³‚Ì‘g‚Ì’†‚Å‚ÍC‡˜‚ð‹t‚É‚µ‚Äl‚¦‚é‚ÆCb[i] ‚Ì‘å‚«‚¢‚à‚Ì‚ð‘O‚É‰ñ‚·‚Ì‚ªÅ‘PD

	int n = sz(a);
	if (p != nullptr) p->resize(n);

	vector<tuple<int, ll, int>> svi(n);
	rep(i, n) {
		if (a[i] - b[i] < 0) svi[i] = { -1, a[i], i };
		else svi[i] = { 1, -b[i], i };
	}
	sort(all(svi));

	ll res = 0, v = 0;
	rep(i, n) {
		int id = get<2>(svi[i]);

		if (p != nullptr) (*p)[i] = id;

		v += a[id];
		chmax(res, v);
		v -= b[id];
	}

	return res;
}


