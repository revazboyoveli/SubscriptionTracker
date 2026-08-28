#pragma once
#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H
#include <iostream>

using namespace std;

struct Sub {
	string name;
	double price;
	string currency;
	int billingDay;

	Sub() {
		name = "";
		price = 0;
		currency = "";
		billingDay = 0;
	}

	Sub(string n, double p, string c, int bd) {
		name = n;
		price = p;
		currency = c;
		billingDay = bd;
	}
};
#endif // !SUBSCRIPTION.H
