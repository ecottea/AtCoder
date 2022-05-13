#pragma once
#include "header.h"
// ¡¡¡¡¡ ‘g‚ÌÅ¬‰»[Å‘å‰»] ¡¡¡¡¡


//y‘g‚Ì LCM ‚ÌÅ¬‰»zO(A log A)iA = max(a)j
/*
* a[0..n) > 0 ‚É‘Î‚µ‚Ä min_(i<j) LCM(a[i], a[j]) ‚ð•Ô‚·D
* ‚Ü‚½Å¬’l‚ð—^‚¦‚é (i, j) ‚ð ids ‚ÉŠi”[‚·‚éD
*/
ll minimize_pair_lcm(const vi& a, pii& ids) {
	// ŽQl : https://drken1215.hatenablog.com/entry/2019/04/17/193100
	// verify : https://codeforces.com/contest/1154/problem/G

	//y•û–@z
	// g = GCD(a[i], a[j]) ‚ðŒˆ‚ß‘Å‚¿‘S’Tõ‚·‚éD
	// ‚±‚Ì‚Æ‚« LCM(a[i], a[j]) ‚ÌÅ¬‰»‚ÍÏ a[i]a[j] ‚ÌÅ¬‰»‚Æ“¯’l‚Å‚ ‚éD
	// 
	// ‚»‚±‚ÅCg ‚Ì”{”‚Å‚ ‚é‚æ‚¤‚È a[0..n) ‚ð¸‡‚É 2 ‚Â‘I‚Ô‚±‚Æ‚É‚·‚éD
	// ‚±‚ê‚Å‚Í g | GCD(a[i], a[j]) ‚Æ‚È‚é‚±‚Æ‚µ‚©•ÛØ‚³‚ê‚È‚¢‚ªC
	// ‚æ‚è‘å‚«‚¢ g ‚ð’Tõ‚µ‚½‚Æ‚«‚É”­Œ©‚³‚ê‚é‚à‚Ì‚ðæ‚ÉŒ©‚Â‚¯‚½‚¾‚¯‚È‚Ì‚Å–â‘è‚È‚¢D

	int n = sz(a);

	int a_max = *max_element(all(a));
	vi cnt(a_max + 1);
	rep(i, n) cnt[a[i]]++;

	ll res = INFL, ai_min = -1, aj_min = -1;

	// g : GCD(a[i], a[j])
	repi(g, 1, a_max) {
		int ai = -1, aj = -1;

		for (int v = g; v <= a_max; v += g) {
			if (cnt[v] == 0) continue;

			if (cnt[v] == 1) {
				if (ai == -1) ai = v;
				else if (aj == -1) {
					aj = v;
					break;
				}
			}
			else {
				if (ai == -1) ai = v;
				if (aj == -1) aj = v;
				break;
			}
		}

		if (aj == -1) continue;

		if (chmin(res, (ll)ai * aj / g)) {
			ai_min = ai;
			aj_min = aj;
		}
	}

	rep(i, n) {
		if (a[i] == ai_min) {
			ids.first = i;
			break;
		}
	}
	rep(j, n) {
		if (j != ids.first && a[j] == aj_min) {
			ids.second = j;
			break;
		}
	}

	return res;
}


