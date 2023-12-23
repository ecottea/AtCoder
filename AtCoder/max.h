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


//【min-max 包除】
/*
* a[0..n) の max[min] は，その部分集合の min[max] を用いて次のように表される：
* 	max a[0..n) = Σ_set⊂[0..n),set≠φ (-1)^(|set|-1) min a[set]
* 	min a[0..n) = Σ_set⊂[0..n),set≠φ (-1)^(|set|-1) max a[set]
*
* 2 数の場合：
*	max(a,b) = min(a) + min(b) - min(a,b)
*	min(a,b) = max(a) + max(b) - max(a,b)
*
* 3 数の場合：
*	max(a,b,c) = min(a) + min(b) + min(c) - min(a,b) - min(a,c) - min(b,c) + min(a,b,c)
*	min(a,b,c) = max(a) + max(b) + max(c) - max(a,b) - max(a,c) - max(b,c) + max(a,b,c)
* 
*（証明）第一式の成立を示す．
* max a[0..n) = a[i] とすると，任意の set⊂[0..n), set≠φ, i!∈set について，
*	min a[set] = min a[set∪{i}]
* である．両辺の集合の要素数の偶奇は異なるので，これらからの寄与は全て打ち消される．
* 
* 参考：https://rogi52.hatenablog.jp/entry/2023/12/03/183717
*/


//【和 → max, min】
/*
* a[0..n) の和は，その部分集合の min や max を用いて次のように表される：
*	Σa[0..n) = max a[0..n) + Σ_set⊂[0..n),|set|≧2 (-1)^|set| min a[set]
*	Σa[0..n) = min a[0..n) + Σ_set⊂[0..n),|set|≧2 (-1)^|set| max a[set]
*
* 2 数の場合：
*	a+b = max(a,b) + min(a,b)
* 
* 3 数の場合：
*	a+b+c = max(a,b,c) + min(a,b) + min(a,c) + min(b,c) - min(a,b,c)
*	a+b+c = min(a,b,c) + max(a,b) + max(a,c) + max(b,c) - max(a,b,c)
* 
*（証明）【min-max 包除】で Σ_|set|=1 の部分のみを左辺に移項すればよい．
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


