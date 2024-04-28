#pragma once
#include "avl_tree.h"
#include "func.h"
#include <gtest.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;
TEST(AVLTreeTest, Insert) {
    AVLTree<int, int> tree;

    tree.insert(5, 5);
    tree.insert(3, 3);
    tree.insert(7, 7);

    EXPECT_TRUE(tree.search(5));
    EXPECT_TRUE(tree.search(3));
    EXPECT_TRUE(tree.search(7));
}

TEST(AVLTreeTest, Remove) {
    AVLTree<int, int> tree;

    tree.insert(5, 5);
    tree.insert(3, 3);

    tree.remove(5);

    EXPECT_FALSE(tree.search(5));
    EXPECT_TRUE(tree.search(3));
}

TEST(AVLTreeTest, MinMax) {
    AVLTree<int, int> tree;

    tree.insert(5, 5);
    tree.insert(3, 3);
    tree.insert(7, 7);

    EXPECT_EQ(tree.min(), 3);
    EXPECT_EQ(tree.max(), 7);
}
TEST(AVLTreeTest, InsertAndSearch) {
    AVLTree<int, int> avlTree;

    avlTree.insert(5,5);
    ASSERT_TRUE(avlTree.search(5));

    ASSERT_FALSE(avlTree.search(10));
}

TEST(AVLTreeTest, RemoveandSearch) {
    AVLTree<int, int> avlTree;

    avlTree.insert(5,5);
    avlTree.insert(10, 10);

    ASSERT_TRUE(avlTree.search(5));
    avlTree.remove(5);
    ASSERT_FALSE(avlTree.search(5));
}
TEST(AVLTreeTest, Iterator) {
    AVLTree<int, int> tree;
    tree.insert(10,10);
    tree.insert(5,5);
    tree.insert(15, 15);
    AVLTreeIterator<int, int> it = tree.begin();
    EXPECT_EQ(*it, 5);

    ++it;
    EXPECT_EQ(*it, 10);

    ++it;
    EXPECT_EQ(*it, 15);
}
TEST(AVLTreeTest, BigTree) {
	const int N = 1000;
	AVLTree<int, int> tree;
	for (int i = 0; i < N; i++) {
		tree.insert(i, i);
	}
	for (auto it = tree.begin(); it != tree.end(); ++it) {
		it->data++;
	}
	for (int i = 0; i < N; i++) {
		EXPECT_EQ(i + 1, tree.find(i)->data);
	}
}
TEST(AVLTreeTest, BigTreeOperator) {
	const int N = 1000;
	AVLTree<int, int> tree;
	for (int i = 0; i < N; i++) {
		tree.insert(i, i);
	}
	for (int i = 0; i < N; i++) {
		tree[i]++;
	}
	for (int i = 0; i < N; i++) {
		EXPECT_EQ(i + 1, tree[i]);
	}
}
TEST(AVLTreeTest, BigRandomInsert) {
	const int N = 1000;
	vector<int> ar;
	AVLTree<int, int> tree;
	for (int i = 0; i < N; i++) {
		ar.push_back(i);
	}
	random_device rd;
	mt19937 g(rd());
	shuffle(ar.begin(), ar.end(), g);
	for (int i = 0; i < N; i++) {
		tree.insert(ar[i], ar[i]);
		EXPECT_TRUE(tree.checkbalance(tree.rootfind()));
	}
	for (int i = 0; i < N; i++) {
		EXPECT_EQ(i, tree.find(i)->key);
		EXPECT_TRUE(tree.checkbalance(tree.rootfind()));
	}
}

TEST(AVLTreeTest, BigRandomRemove) {
	const int N = 1000;
	vector<int> ar;
	AVLTree<int, int> tree;
	for (int i = 0; i < N; i++) {
		ar.push_back(i);
	}
	random_device rd;
	mt19937 g(rd());
	shuffle(ar.begin(), ar.end(), g);
	for (int i = 0; i < N; i++) {
		tree.insert(ar[i], ar[i]);
		EXPECT_TRUE(tree.checkbalance(tree.rootfind()));
	}
	shuffle(ar.begin(), ar.end(), g);
	for (int i = 0; i < N; i++) {
		EXPECT_TRUE(tree.remove(ar[i]));
		EXPECT_EQ(N - i - 1, tree.getsize());
		EXPECT_TRUE(tree.checkbalance(tree.rootfind()));
	}
	EXPECT_EQ(0, tree.getsize());
}
/*TEST(AVLTreeTest, BigRandomInsertAndRemove) {
	const int N = 1000;
	AVLTree<int, int> tree;
	vector<int> ar;
	for (int i = 0; i < N; i++) {
		ar.push_back(i);
	}
	random_device rd;
	mt19937 g(rd());
	shuffle(ar.begin(), ar.end(), g);
	for (int i = 0; i < N; i++) {
		tree.insert(ar[i], ar[i]);
	}
	bool balance = tree.checkbalance();
	vector<int> ar2;
	for (int i = 1000; i < 1100; i++) {
		ar2.push_back(i);
	}
	int j = 0;
	while (tree.rootfind()->data < 400 && balance != false) {
		shuffle(ar2.begin(), ar2.end(), g);
		for (int i = 0; i < 100; i++) {
			tree.insert(ar2[i], ar2[i]);
			balance = tree.checkbalance();
		}
		shuffle(ar2.begin(), ar2.end(), g);
		for (int i = 0; i < 100; i++) {
			tree.remove(ar2[i]);
			balance = tree.checkbalance();
		}
		j++;
	}
	if (balance == true) {
		cout << 1 <<" "<< j;
	}
	else {
		cout << 0 <<" "<< j;
	}
}*/
