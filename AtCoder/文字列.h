#pragma once
#include "header.h"
#include "二分木.h"
// ■■■■■ 文字列 ■■■■■



//【ローリングハッシュ】
/*
* 文字列 s の部分文字列 s[l, r) のハッシュ値を計算する．
* ハッシュ値は Σi=0..r-l (s[l + i] + SHIFT) * BASE ^ i (mod MOD)
*
* rolling_hash(s) : O(|s|)
*	文字列を s として初期化する．
*
* get(l, r) : O(1)
*	部分文字列 s[l, r) のハッシュ値を返す．
*/
template <typename STR, int MOD, int BASE, int SHIFT>
struct rolling_hash_sub {
	using mint = static_modint<MOD>;
	const mint B = BASE; // 適当な基数
	const mint invB = B.inv(); // 基数の逆数
	const mint S = SHIFT; // 適当なシフト

	// 文字列とその長さ
	STR s;
	int n;

	// v[i] : s[0, i) のハッシュ値
	vm v;

	// pow_invB[i] : B^(-i)
	vm pow_invB;


	// コンストラクタ（文字列 s で初期化）
	rolling_hash_sub(const STR& s_) : s(s_), n(sz(s_)), v(n + 1), pow_invB(n) {
		// ハッシュ値計算用の B の累乗
		pow_invB[0] = 1;
		rep(i, n - 1) {
			pow_invB[i + 1] = pow_invB[i] * invB;
		}

		// s[0, i) のハッシュ値の計算
		mint powB = 1;
		rep(i, n) {
			v[i + 1] = v[i] + (s[i] + S) * powB;
			powB *= B;
		}
	}


	// s[l, r) のハッシュ値の取得
	int get(int l, int r) {
		return ((v[r] - v[l]) * pow_invB[l]).val();
	}
};
template <typename STR> // STR は例えば string, vector<int>
struct rolling_hash {
	// 衝突の可能性を減らすため，二つのハッシュ値を統合する．
	rolling_hash_sub<STR, 1000000007, 100007, 17> rh1;
	rolling_hash_sub<STR, 998244353, 99991, 91> rh2;


	// コンストラクタ（文字列 s で初期化）
	rolling_hash(const STR& s) : rh1(s), rh2(s) {}


	// s[l, r) のハッシュ値の取得
	ll get(int l, int r) {
		return (ll(rh1.get(l, r)) << 32) + ll(rh2.get(l, r));
	}
};


//【ローリングハッシュ（二次元）】
/*
* 二次元配列 a の部分長方形領域 [x1, x2) * [y1, y2) のハッシュ値を計算する．
* ハッシュ値は次の式により計算する：
*   Σi=0..x2-x1 j=0..y2-y1 (a[x1 + i][y1 + j] + SHIFT) * BASE_X ^ i * BASE_Y ^ j (mod MOD)
*
* rolling_hash(a) : O(|w| |h|)
*	二次元配列を a として初期化する．
*
* get(x1, y1, x2, y2) : O(1)
*	部分長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
*/
template <typename T, int MOD, int BASE_X, int BASE_Y, int SHIFT>
struct rolling_hash_2d_sub {
	using mint = static_modint<MOD>;
	const mint BX = BASE_X; // 適当な基数
	const mint invBX = BX.inv(); // 基数の逆数
	const mint BY = BASE_Y;
	const mint invBY = BY.inv();
	const mint S = SHIFT; // 適当なシフト

	// 二次元配列とその大きさ
	vector<vector<T>> a;
	int h, w;

	// v[i][j] : 長方形領域 [0, i) * [0, j) のハッシュ値
	vvm v;

	// ハッシュ値計算用の B の累乗
	vm pow_BX, pow_BY, pow_invBX, pow_invBY;


	// コンストラクタ（文字列 s で初期化）
	rolling_hash_2d_sub(vector<vector<T>>& a_) :
		a(a_), h(sz(a)), w(sz(a[0])), v(h + 1, vector<mint>(w + 1)),
		pow_BX(h), pow_BY(w), pow_invBX(h), pow_invBY(w) {

		// ハッシュ値計算用の B の累乗の前計算
		pow_BX[0] = pow_BY[0] = pow_invBX[0] = pow_invBY[0] = 1;
		rep(i, h - 1) {
			pow_BX[i + 1] = pow_BX[i] * BX;
			pow_invBX[i + 1] = pow_invBX[i] * invBX;
		}
		rep(j, w - 1) {
			pow_BY[j + 1] = pow_BY[j] * BY;
			pow_invBY[j + 1] = pow_invBY[j] * invBY;
		}

		// 長方形領域 [0, i) * [0, j) のハッシュ値の計算
		rep(i, h) {
			rep(j, w) {
				v[i + 1][j + 1] = v[i + 1][j] + v[i][j + 1] - v[i][j]
					+ (a[i][j] + S) * pow_BX[i] * pow_BY[j];
			}
		}
	}


	// 長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
	int get(int x1, int y1, int x2, int y2) {
		return ((v[x2][y2] - v[x1][y2] - v[x2][y1] + v[x1][y1])
			* pow_invBX[x1] * pow_invBY[y1]).val();
	}
};
template <typename T> // T は例えば int, ll, char
struct rolling_hash_2d {
	// 衝突の可能性を減らすため，二つのハッシュ値を統合する．
	rolling_hash_2d_sub<T, 1000000007, 100007, 26627, 17> rh1;
	rolling_hash_2d_sub<T, 998244353, 99991, 54401, 91> rh2;


	// コンストラクタ（二次元配列 a で初期化）
	rolling_hash_2d(vector<vector<T>>& a) : rh1(a), rh2(a) {}


	// 長方形領域 [x1, x2) * [y1, y2) のハッシュ値を返す．
	ll get(int x1, int y1, int x2, int y2) {
		return (ll(rh1.get(x1, y1, x2, y2)) << 32) + ll(rh2.get(x1, y1, x2, y2));
	}
};


//【クヌース・モリス・プラット法】O(|s| + |w|)
/*
* s の連続部分列として w が含まれているかどうか調べ，
* 最初に見つかった場所の先頭位置を返す．（見つからなかったら -1 を返す．）
*/
template <typename STR> // STR は例えば string, vector<int>
int knuth_morris_pratt(const STR& s, const STR& w) {
	int n = sz(s);
	int m = sz(w);

	// 部分マッチテーブル tbl の作成
	// s[i] から照合を開始して s[i + j] != w[j] となった場合，
	// 次に照合を開始すべき位置が s[i + j - tbl[j]] であるように構築する．
	vi tbl(m);
	tbl[0] = -1;
	tbl[1] = 0;
	int i = 2; // いま tbl[i] を計算中であることを表す．
	int j = 0; // いま w[j] まで見ていることを表す．
	while (i < m) {
		// サブ文字列が w の先頭と一致し続けている場合
		if (w[i - 1] == w[j]) {
			// 一致した長さの分だけバックトラッキングしなければならない．
			tbl[i] = j + 1;

			// それぞれ 1 文字先を見に行く．
			i++;
			j++;
		}
		// サブ文字列と w の先頭との一致が終わった場合？
		else if (j > 0) {
			// 次のサブ文字列を走査するため j を戻す．？
			j = tbl[j];
		}
		// ？
		else {
			tbl[i] = 0;
			i++;
		}
	}

	// 連続部分列を探す．
	i = 0; // いま s[i] から始まる連続部分列を見ていることを表す．
	j = 0; // いま w[j] まで見ていることを表す．
	while (i + j < n) {
		// s で見ている文字 s[i + j] が w で見ている文字 w[j] に一致した場合．
		if (w[j] == s[i + j]) {
			// さらに 1 文字先を見に行く．
			j++;

			// もし w を走査し終えたなら連続部分列として w を発見．
			if (j == m) {
				return i;
			}
		}
		// s で見ている文字 s[i + j] が w で見ている文字 w[j] に一致しなかった場合．
		else {
			// 部分マッチテーブルに従い i, j を再設定する．
			// i = i + j としたいが tbl[j] だけのバックトラッキングが入る．
			i = i + j - tbl[j];

			// その代わり w との照合を tbl[j] だけ進んだところから始められる．
			if (j > 0) {
				j = tbl[j];
			}
		}
	}

	return -1;
}


//【レーベンシュタイン距離】O(|s| |t|)
/*
* 文字列 s から文字列 t へのレーベンシュタイン距離を返す．
* また必要なら削除や挿入の位置を '-' で表した文字列を s2, t2 に格納する．
*
* dist = {fit, ins, del, sub} :
*	順に，一致，挿入，削除，置換 1 回あたりの距離
*
*（二次元 DP）
*/
template <typename STR> // STR は例えば string, vector<int>
ll levenshtein_distance(const STR& s, const STR& t,
	const tuple<ll, ll, ll, ll>& dist = { 0, 1, 1, 1 },
	STR* s2 = nullptr, STR* t2 = nullptr) {

	int n = sz(s);
	int m = sz(t);

	ll fit, ins, del, sub;
	tie(fit, ins, del, sub) = dist;

	// dp[i][j] : s[0..i) から t[0..j) への距離
	vvl dp(n + 1, vl(m + 1, INF));
	dp[0][0] = 0;

	// 貰う DP
	repi(i, 0, n) {
		repi(j, 0, m) {
			// 以下の 3 通りのうちの最大のものを選ぶ：
			//   s[i - 1] と t[j - 1] を対にする．
			//   s[i - 1] を削除する．
			//   t[j - 1] を挿入する．
			if (i > 0 && j > 0) {
				chmin(dp[i][j], dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? fit : sub));
			}
			if (i > 0) {
				chmin(dp[i][j], dp[i - 1][j] + del);
			}
			if (j > 0) {
				chmin(dp[i][j], dp[i][j - 1] + ins);
			}
		}
	}

	// DP 復元を行う
	if (s2 != nullptr) {
		s2->clear();
		t2->clear();

		int i = n, j = m;
		while (i > 0 || j > 0) {
			if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? fit : sub)) {
				s2->push_back(s[--i]);
				t2->push_back(t[--j]);
			}
			else if (i > 0 && dp[i][j] == dp[i - 1][j] + del) {
				s2->push_back(s[--i]);
				t2->push_back('-');
			}
			else {
				s2->push_back('-');
				t2->push_back(t[--j]);
			}
		}
		reverse(all(*s2));
		reverse(all(*t2));
	}

	return dp[n][m];
}


//【ランレングス符号】
/*
* s をランレングス符号化し，結果を rlc に格納する．
* rlc[i] = {c, l} は前から i 番目の列が l 個の文字 c からなることを表す．
*/
template <class T>
void run_length_encodeing(vector<T>& a, vector<pair<T, int>>& rlc) {
	int n = sz(a);
	if (n == 0) {
		rlc = {};
		return;
	}

	rlc = { {a[0], 1} };

	// 今読んでいる文字の種類を記憶する．
	T c = a[0];

	repi(i, 1, n - 1) {
		// 記憶している文字と同じ文字の場合
		if (c == a[i]) {
			// 列の長さを増やす．
			rlc.rbegin()->second++;
		}
		// 記憶している文字と異なる文字の場合
		else {
			// 新しい文字を記憶しておく．
			c = a[i];

			// 新たな列を追加する．
			rlc.push_back({ c, 1 });
		}
	}
}


//【トライ木】
/*
* Trie_tree() : O(1)
*   空文字列のみで初期化する．
*
* insert(str) : O(|str|)
*   文字列 str を登録する．
*
* find(str) : O(|str|)
*   文字列 str が登録されているかを返す．
*
* find_prefix(str) : O(|str|)
*   文字列 str を接頭辞にもつ文字列が登録されているかを返す．
*
* count() : O(1)
*   登録されている文字列の個数を返す．
*/
struct Trie_tree {
	// 参考 : https://algo-logic.info/trie-tree/

	const int K = 26; // 文字数

	int n; // g のノード数
	Graph g; // トライ木
	vc chars; // 頂点 g[i] に対応する文字
	vb end; // g[i] で終わる文字があるか
	vi cnt; // g[i] を含む文字列の個数

	Trie_tree() : n(1), g(1, vi(K, -1)), chars(1), end(1), cnt(1) {}

	void insert(const string& str) {
		int v = 0;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// str は頂点 v を含む文字列なので個数に加算する．
			cnt[v]++;

			// 登録済みの文字だった場合
			if (g[v][c - 'a'] != -1) {
				// そのノードへ移動
				v = g[v][c - 'a'];
			}
			// 未登録の文字だった場合
			else {
				// 新たにノード n を追加
				g.push_back(vi(K, -1));
				chars.push_back(c);
				end.push_back(false);
				cnt.push_back(0);

				// 新たなノードへのパスを追加
				g[v][c - 'a'] = n;

				// 新たなノードへ移動
				v = n++;
			}
		}

		end[v] = true;
	}

	bool find(const string& str) const {
		return find_sub(str, false);
	}

	bool find_prefix(const string& str) const {
		return find_sub(str, true);
	}

	bool find_sub(const string& str, bool prefix_flag) const {
		int v = 0;

		// str の文字 c を先頭から順に見ていく
		repe(c, str) {
			// 登録済みの文字だった場合
			if (g[v][c - 'a'] != -1) {
				// そのノードへ移動
				v = g[v][c - 'a'];
			}
			// 未登録の文字だった場合
			else {
				return false;
			}
		}

		return end[v] | prefix_flag;
	}

	int count() const {
		return cnt[0];
	}
};


//【ハフマン符号】O(|s|)
/*
* 文字列 s をハフマン符号化したときの符号語の長さを返す．
*/
ll huffman_encoding(string& s) {
	// 文字の種類
	int K = 256;

	int n = sz(s);

	// 各文字の出現頻度を得る．
	vi cnt(K);
	rep(i, n) {
		cnt[(int)s[i]]++;
	}

	// 出現頻度の低い順に文字を取り出す優先度付きキュー
	priority_queue_rev<pii> q;

	// ハフマン木
	vector<BTNode> ht(2 * K - 1);

	// ハフマン木の葉となる文字たちをキューに追加する．
	rep(j, K) {
		ht[j] = { -1, -1, -1, 0, 0 };

		// 頻度 0 の文字は符号化の対象としない．
		if (cnt[j] > 0) {
			q.push({ cnt[j], j });
		}
		else {
			// 根として誤検出されないようにダミーを仕込んでおく．
			ht[j].parent = 12345;
		}
	}

	// 出現頻度の低い文字 2 つを組にして部分木を作り，
	// それらを合わせた分の出現頻度をもつ新たな文字とみなすことを繰り返す．
	int pt = K;
	while (sz(q) > 1) {
		// 出現頻度の低い文字 2 つを得る．
		int cnt1, cnt2, pos1, pos2;
		tie(cnt1, pos1) = q.top();
		q.pop();
		tie(cnt2, pos2) = q.top();
		q.pop();

		// それらを合わせた文字を表すノードを作り，キューに追加する．
		ht[pt] = { -1, pos1, pos2, -1, -1 };
		q.push({ cnt1 + cnt2, pt });

		// 子の情報を更新する．
		ht[pos1].parent = ht[pos2].parent = pt;

		pt++;
	}

	// ハフマン木の各ノードの深さを求める．
	decide_depth(ht);

	ll res = 0;
	rep(i, n) {
		// 葉の深さが対応する文字の符号語の長さである．
		// ただし文字が 1 種類のみのときは例外処理．
		int len = max(ht[(int)s[i]].depth, 1);
		res += len;
	}

	return res;
}


