#pragma once
#include "header.h"
// ¡¡¡¡¡ ”‚Ì•\¦ ¡¡¡¡¡



//yŒ…‚Ì”‚Ìæ“¾zO(log n)
/*
* n ‚ğ b i•\‹L‚µ‚½‚Æ‚«‚ÌŒ…‚Ì”š‚ğ‰ºˆÊŒ…‚©‚ç‡‚É•À‚×‚½‚à‚Ì‚ğ digits ‚ÉŠi”[‚·‚éD
*/
void integer_digits(ll n, int b, vi& digits) {
	digits.clear();
	while (n > 0) {
		digits.push_back(n % b);
		n /= b;
	}
}


//yŒ…‚Ì”‚©‚ç‚Ì•œŒ³zO(|digits|)
/*
* b i•\‹L‚Å‰ºˆÊŒ…‚©‚ç‡‚É digits ‚ª•À‚ñ‚¾”‚Ì’l‚ğ•Ô‚·D
*/
ll from_digits(vi& digits, int b = 10) {
	ll res = 0;
	ll powb = 1;
	repe(d, digits) {
		res += d * powb;
		powb *= b;
	}
	return res;
}


//y”š˜azO(d)
/*
* d Œ…‚Ì” n ‚ğ b i•\‹L‚µ‚½‚Æ‚«‚ÌŒ…‚Ì”š‚Ì˜a‚ğ•Ô‚·D
*/
int digit_sum(ll n, int b = 10) {
	int sum = 0;
	while (n > 0) {
		sum += n % b;
		n /= b;
	}
	return sum;
}


//y”šªzO(d log(log d))?
/*
* d Œ…‚Ì” n ‚ğ b i•\‹L‚µ‚½‚Æ‚«‚Ì”šª‚ğ•Ô‚·D
*/
int digit_root(ll n, int b) {
	while (n >= b) {
		ll sum = 0;
		while (n > 0) {
			sum += n % b;
			n /= b;
		}
		n = sum;
	}
	return (int)n;
}

