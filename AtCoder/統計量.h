#pragma once
#include "header.h"
// ¡¡¡¡¡ “Œv—Ê ¡¡¡¡¡


//y’†‰›’l‚Ì 2 ”{zO(n log n)
/*
* a[0..n) ‚Ì’†‰›’l‚Ì“ñ”{‚ğ•Ô‚·D
*/
template <class T> T doubled_median(vector<T> a) {
	int n = sz(a);
	sort(all(a));
	return a[(n - 1) / 2] + a[n / 2];
}


//yl•ªˆÊ”‚Ì 2 ”{zO(n log n)
/*
* a[0..n) ‚Ìl•ªˆÊ”‚Ì“ñ”{‚ğ•Ô‚·D
*/
template <class T> tuple<T, T, T> doubled_quartile(vector<T> a) {
	// verify : https://algo-method.com/tasks/709

	int n = sz(a);
	sort(all(a));
	T dq1 = a[n / 2 / 2] + a[(n / 2 - 1) / 2];
	T dq2 = a[n / 2] + a[(n - 1) / 2];
	T dq3 = a[n - 1 - n / 2 / 2] + a[n - 1 - (n / 2 - 1) / 2];
	return make_tuple(dq1, dq2, dq3);
}


