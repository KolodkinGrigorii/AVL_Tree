#pragma once
#include <AVLTree.h>
#include <func.h>
#include <iostream>
using namespace std;
int main() {
	AVLTree<int> avl;
	avl.insert(2);
	avl.insert(3);
	avl.insert(4);
	auto it=avl.begin();
	cout << *it;
	++it;
}