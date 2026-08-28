#pragma once
#ifndef SUBSCRIPTIONMANAGER_H
#define SUBSCRIPTIONMANAGER_H
#include "Subscription.h"
#include <iostream>
#include <vector>

using namespace std;

class SubscriptionManager {
private:
	vector<Sub> subs;
	string filePath;

	void sortData();
	string lowerStr(string s);
public:
	SubscriptionManager(string filename = "subscription.txt");
	bool loadData();
	bool saveData();

	void addSub(Sub newSub);
	bool editSub(int index, Sub editedSub);
	bool deleteSub(int index);
	bool contains(string s1, string s2);

	vector<Sub> searchSub(string searching);
	const vector<Sub>& getAllSubs();
};

#endif // !define SUBSCRIPTIONMANAGER.H
