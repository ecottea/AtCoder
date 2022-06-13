#pragma once
#include "header.h"
// ■■■■■ max, min, 絶対値等 ■■■■■


//【max, min の別表現】
/*
* max(x, y), min(x, y) は |x - y| を用いて以下のように表される： 
*	max(x, y) = ((x + y) + |x - y|) / 2
*	min(x, y) = ((x + y) - |x - y|) / 2
* 
* 特に ReLU 系は以下のように表される： 
*	max(x, 0) = (x + |x|) / 2
*	min(x, 0) = (x - |x|) / 2
*/


//【差の絶対値の別表現】
/*
* |x - y| は max(x, y), min(x, y) を用いて以下のように表される： 
*	|x - y| = 2 max(x, y) - (x + y)
*	|x - y| = (x + y) - 2 min(x, y)
*/


//【絶対値の別表現】
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
*（証明）
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


