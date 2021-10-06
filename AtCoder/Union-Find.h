#pragma once
#include "header.h"
// ■■■■■ Union-Find ■■■■■



//【Union-Find】
/*
* ノードの統合と連結判定を行う．
*
* Union_find(n) : O(n)
*	非連結で大きさ n の Union-Find を構築する．
* 
* merge(a, b) : O(α(n))
*	ノード a とノード b を統合する．
* 
* sameQ(a, b) : O(α(N))
*	ノード a とノード b が同じ連結成分に属するかを返す．
* 
* root(a) : O(α(N))
*	ノード a の属する連結成分の親を返す．
* 
* size(a) : O(α(N))
*	ノード a の属する連結成分の大きさを返す．
*/
struct Union_find {
	// ノードの個数
	int n;

	// parent_or_size[i] : ノード i の親または集合の大きさ
	// ノード i が根でない場合は親の番号（非負）を，
	// 根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;


	// コンストラクタ（初期化なし）
	Union_find() : n(0) {}

	// コンストラクタ（大きさ n で初期化）
	Union_find(int n_) : n(n_), parent_or_size(n, -1) {}

	// ノード a, b を結合する．
	void merge(int a, int b) {
		// ノード a, b の属する連結成分の根 ra, rb を得る．
		int ra = root(a);
		int rb = root(b);

		// 根が同じであれば既に連結であるから何もしない．
		if (ra == rb) {
			return;
		}

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (-parent_or_size[ra] < -parent_or_size[rb]) {
			swap(ra, rb);
		}

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		parent_or_size[ra] += parent_or_size[rb];
		parent_or_size[rb] = ra;
	}

	// ノード a, b が同じ連結成分に属するかを返す．
	bool sameQ(int a, int b) {
		// 根が同じなら連結である．
		return root(a) == root(b);
	}

	// ノード a の属する連結成分の根を返す．
	int root(int a) {
		// a が根であれば自分自身を返す．
		int pa = parent_or_size[a];
		if (pa < 0) {
			return a;
		}

		// a が根でなければ，a の親 pa の根 ra を求める．
		int ra = root(pa);

		// a の親を ra に更新しつつ，a の根 ra を返す．
		parent_or_size[a] = ra;
		return ra;
	}

	// ノード a の属する連結成分の大きさを返す．
	int size(int a) {
		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size[root(a)];
	}
};


//【重み付き Union-Find】
/*
* ノード間の差の指定と差の計算を行う．
*
* Weighted_union_find(n) : O(n)
*	非連結で大きさ n の重み付き Union-Find を構築する．
* 
* set_diff(a, b, d) : O(α(n))
*	v[b] - v[a] = d という関係を追加する．失敗は false を返す．
* 
* sameQ(a, b) : O(α(n))
*	ノード a とノード b が同じ連結成分に属するかを返す．
*
* get_diff(a, b) : O(α(n))
*	v[b] - v[a] を返す．（差が未確定なら戻り値は未定義とする）
* 
* root(a) : O(α(n))
*	ノード a の属する連結成分の親を返す．
* 
* size(a) : O(α(n))
*	ノード a の属する連結成分の大きさを返す．
*/
template <class T>
struct Weighted_union_find {
	// ノードの個数
	int n;

	// parent_or_size[i] : ノード i の親または集合の大きさ
	// ノード i が根でない場合は親の番号（非負）を，
	// 根の場合は属する連結成分の大きさの -1 倍（負）を表す．
	vi parent_or_size;

	// diff[i] : 親からみたノード i との差
	vector<T> diff;

	// コンストラクタ（初期化なし）
	Weighted_union_find() {}

	// コンストラクタ（大きさ n で初期化）
	Weighted_union_find(int n_) : n(n_), parent_or_size(n, -1), diff(n) {}

	// ノード a, b 間の差 v[b] - v[a] を設定する．
	bool set_diff(int a, int b, T d) {
		// ノード a, b の属する連結成分の根 ra, rb を得る．
		int ra = root(a);
		int rb = root(b);

		// 根が同じであれば既に連結であるから何もしない．
		// 既にある関係と整合しているかを返す．
		if (ra == rb) {
			return diff[b] - diff[a] == d;
		}

		// 根が異なる場合，大きい連結成分の根を改めて ra，小さい方を rb とする．
		if (-parent_or_size[ra] < -parent_or_size[rb]) {
			swap(a, b);
			swap(ra, rb);
			d *= -1;
		}

		// 小さい方の連結成分を ra を根とする連結成分に統合する．
		parent_or_size[ra] += parent_or_size[rb];
		parent_or_size[rb] = ra;
		diff[rb] = diff[a] - diff[b] + d;

		return true;
	}

	// ノード a, b が同じ連結成分に属するかを返す．
	bool sameQ(int a, int b) {
		// 根が同じなら連結である．
		return root(a) == root(b);
	}

	// v[b] - v[a] を返す．
	T get_diff(int a, int b) {
		// 差が確定していると仮定し，根からの差の差として計算する．
		return diff[b] - diff[a];
	}

	// ノード a の属する連結成分の根を返す．
	int root(int a) {
		// a が根であれば自分自身を返す．
		int pa = parent_or_size[a];
		if (pa < 0) {
			return a;
		}

		// a が根でなければ，a の親 pa の根 ra を求める．
		// 再帰的な処理が回り，pa の親は ra になっていることに注意．
		int ra = root(pa);

		// a の親を ra に更新しつつ，a の根 ra を返す．
		parent_or_size[a] = ra;
		diff[a] += diff[pa];
		return ra;
	}

	// ノード a の属する連結成分の大きさを返す．
	int size(int a) {
		// a の根を調べ，そこに記録されている大きさの情報を返す．
		return -parent_or_size[root(a)];
	}
};

