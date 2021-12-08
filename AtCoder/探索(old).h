#pragma once


//y•À—ñ“ñ•ª’Tõz
template <typename T>
void parallel_binary_search(vector<T>& oks, vector<T>& ngs,
	function<void(unordered_map<T, vector<pair<int, bool>>>&)>& okQs)
{
	int q = sz(oks);

	while (true) {
		unordered_map<T, vector<pair<int, bool>>> mid_to_ib;

		bool update = false;
		rep(i, q) {
			if (abs(oks[i] - ngs[i]) == 1) continue;
			update = true;

			T mid = (oks[i] + ngs[i]) / 2;
			mid_to_ib[mid].push_back({ i, false });
		}
		if (!update) break;

		okQs(mid_to_ib);
		dumpel(mid_to_ib);

		repe(tmp, mid_to_ib) {
			T mid = tmp.first;

			repe(ib, tmp.second) {
				int i; bool b;
				tie(i, b) = ib;

				if (b) oks[i] = mid;
				else ngs[i] = mid;
			}
		}
	}
}


