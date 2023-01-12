#pragma once
#include "header.h"
// ■■■■■ max, min, 絶対値等 ■■■■■


//【max, min → 絶対値】
/*
* max(x, y), min(x, y) は |x - y| を用いて以下のように表される： 
*	max(x, y) = ((x + y) + |x - y|) / 2
*	min(x, y) = ((x + y) - |x - y|) / 2
* 
* 特に ReLU は以下のように表される： 
*	max(x, 0) = (x + |x|) / 2
*	min(x, 0) = (x - |x|) / 2
*/


//【差の絶対値 → max, min】
/*
* |x - y| は max(x, y), min(x, y) を用いて以下のように表される： 
*	|x - y| = 2 max(x, y) - (x + y)
*	|x - y| = (x + y) - 2 min(x, y)
*/


//【絶対値 → max, min】
/*
* |x| は max, min を用いて以下のように表される：
*	|x| = max(x, -x)
*	|x| = -min(x, -x)
*/


//【マンハッタン距離の 45° 回転表現】
/*
* 点 (x1, y1) と点 (x2, y2) のマンハッタン距離は次のようにも表される：
*	|x2 - x1| + |y2 - y1| = max( |(x1 + y1) - (x2 + y2)|, |(x1 - y1) - (x2 - y2)| )
*
* 証明 : 左辺を計算すると次のように右辺に一致する：
*	|x2 - x1| + |y2 - y1|
*	= max(x1 - x2, x2 - x1) + max(y1 - y2, y2 - y1)
*	= max((x1 - x2) + (y1 - y2), (x1 - x2) + (y2 - y1),
*		  (x2 - x1) + (y1 - y2), (x2 - x1) + (y2 - y1)) （max-plus 分配法則）
*	= max( (x1 + y1) - (x2 + y2),  (x1 - y1) - (x2 - y2),
*		  -(x1 - y1) + (x2 - y2), -(x1 + y1) + (x2 + y2))
*	= max( |(x1 + y1) - (x2 + y2)|, |(x1 - y1) - (x2 - y2)| )
*
* verify : https://atcoder.jp/contests/abc178/tasks/abc178_e
*/


//【ReLU の max】
/*
* max(max(x+a1, b1), max(x+a2, b2)) = max(x + max(a1,a2), max(b1,b2))
* 
* 証明 : max-plus 代数で計算すると，
*	(a1 x + b1) + (a2 x + b2) = (a1 + a2)x + (b1 + b2)
* となることから直ちに従う．
*/


