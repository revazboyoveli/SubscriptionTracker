#include "SubscriptionManager.h"
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

SubscriptionManager::SubscriptionManager(string filename) {
	filePath = filename;
	loadData();
}

void SubscriptionManager::sortData() {
	int n = subs.size();
	bool swapped;

	for (int i = 0; i < n - 1; i++) {
		swapped = false;
		for (int j = 0; j < n - i - 1; j++) {
			if (subs[j].billingDay > subs[j + 1].billingDay) {
				swap(subs[j], subs[j + 1]);
				swapped = true;
			}
		}
		if (!swapped) {
			break;
		}
	}
}
bool SubscriptionManager::loadData() {
	ifstream file(filePath);
	if (!file.is_open()) {
		return false;
	}
	subs.clear();
	Sub sub;
	while (file >> sub.name >> sub.price >> sub.currency >> sub.billingDay) {
		subs.push_back(sub);
	}
	file.close();
	sortData();
	return true;
}
string SubscriptionManager::lowerStr(string s) {
	for (char& ch : s) {
		ch = tolower(ch);
	}
	return s;
}

bool SubscriptionManager::saveData() {
	ofstream file(filePath);
	if (!file.is_open()) {
		return false;
	}

	for (auto& sub : subs) {
		file << sub.name << " " << sub.price << " " << sub.currency << " " << sub.billingDay << "\n";
	}
	file.close();
	return true;
}

void SubscriptionManager::addSub(Sub newSub) {
	subs.push_back(newSub);
	sortData();
	saveData();
}

bool SubscriptionManager::editSub(int index, Sub editedSub) {
	if (index < 0 || index >= (int)subs.size()) {
		return false;
	}
	subs[index] = editedSub;
	sortData();
	saveData();
	return true;
}

bool SubscriptionManager::deleteSub(int index) {
	if (index < 0 || index >= (int)subs.size()) {
		return false;
	}
	subs.erase(subs.begin() + index);
	saveData();
	return true;
}

vector<Sub> SubscriptionManager::searchSub(string searching) {
	vector<Sub> result;
	string search = lowerStr(searching);
	for (auto& sub : subs) {
		string name = lowerStr(sub.name);
		if (contains(search, name)) {
			result.push_back(sub);
		}
	}
	return result;
}

bool SubscriptionManager::contains(string s1, string s2) {
	if (s1.length() > s2.length()) {
		return false;
	}
	lowerStr(s1);
	lowerStr(s2);
	for (int i = 0; i < s1.length(); i++) {
		if (s1[i] != s2[i]) {
			return false;
		}
	}
	return true;
}

const vector<Sub>& SubscriptionManager::getAllSubs() {
	return subs;
}
