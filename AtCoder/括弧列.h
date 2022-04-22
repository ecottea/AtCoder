#pragma once
#include "header.h"
// ■■■■■ 括弧列 ■■■■■


//【括弧列の正しさ判定】
/*
* 括弧列 s[0..2n) に対して，'(' を +1, ')' を -1 に置き換える操作を行い，
* さらに左から累積和をとったものを a[0..2n] とする．このとき，
*		s が正しい括弧列 ⇔ min(a) = a[2n] = 0
* 
* verify : https://atcoder.jp/contests/abc223/tasks/abc223_f
*/


//【括弧列の対応】
/*
* 括弧列 s において，s[i] = '(' と s[j] = ')' が対応する括弧であるとき i と j の偶奇は異なる．
*
* 証明：s(i..j) もまた括弧列であり，これの長さが偶数であることから直ちに従う．
* 
* verify : https://atcoder.jp/contests/agc048/tasks/agc048_b
*/


