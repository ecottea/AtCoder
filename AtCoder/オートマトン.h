#pragma once
#include "header.h"
// ■■■■■ オートマトン ■■■■■


//【非決定性有限オートマトン → 決定性有限オートマトン】
/*
* 非決定性有限オートマトン NFA の状態が n 個であるとき，NFA の集合を新たに状態とすることで
* 高々 2^n 個の状態をもつ決定性有限オートマトン DFA が得られる．
* 
* verify : https://atcoder.jp/contests/abc228/tasks/abc228_g
*/


//【決定性有限オートマトンの受理文字列の個数と経路数】
/*
* 決定性有限オートマトン DFA において，開始状態からいずれかの受理状態までを結ぶパスの本数と，
* DFA が受理する文字列の総数は等しい．
* 
* verify : https://atcoder.jp/contests/abc228/tasks/abc228_g
*/


