#pragma once
#include <AVLTree.h>
#include <func.h>
#include <iostream>
using namespace std;
int main() {
	AVLTree<int> avl;
	for (int i = 0; i < 9; i++) {
		avl.insert(i);
	}
	for (auto it = avl.begin(); it != avl.end(); ++it) {
		cout << *it;
	}
}