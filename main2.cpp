#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "BSTLEAF.h"
#include "BSTROOT.h"
#include "BSTRAND.h"
#include "AVL.h"
#include <iostream>
using namespace cop3530;
bool equal(const int& a, const int& b) {
	return a == b;
}
bool compare(const int& a, const int& b) {
	return a < b;
}

SCENARIO("INTERFACE TEST") {
	GIVEN("Binary search tree: key and values are integers") {

		cop3530::BSTLEAF<int, int, compare, equal> * tree = new cop3530::BSTLEAF<int, int, compare, equal>();
		cop3530::BSTROOT<int, int, compare, equal> * tree1 = new cop3530::BSTROOT<int, int, compare, equal>();

		cop3530::BSTRAND<int, int, compare, equal> * tree2 = new cop3530::BSTRAND<int, int, compare, equal>();
		cop3530::AVL<int, int, compare, equal> * tree3 = new cop3530::AVL<int, int, compare, equal>();


		tree->insert(10, 1);
		tree->insert(5, 100);
		tree->insert(12, 20);
		tree->insert(5, 12);
		tree->insert(8, 25);

		tree1->insert(10, 1);
		tree1->insert(5, 100);
		tree1->insert(12, 20);
		tree1->insert(5, 12);
		tree1->insert(8, 25);

		tree2->insert(10, 1);
		tree2->insert(5, 100);
		tree2->insert(12, 20);
		tree2->insert(5, 12);
		tree2->insert(8, 25);

		tree3->insert(10, 1);
		tree3->insert(5, 100);
		tree3->insert(12, 20);
		tree3->insert(5, 12);
		tree3->insert(8, 25);

		WHEN("Lookup is called") {
			int value = tree->lookup(10);
			int value1 = tree->lookup(10);  int value2 = tree->lookup(10); int value3 = tree->lookup(10);

			THEN("Value should be 1") {
				REQUIRE(value == 1);
				REQUIRE(value1 == 1);
				REQUIRE(value2 == 1); REQUIRE(value3 == 1);
			}
		}

		WHEN("Lookup is called") {
			int value = tree->lookup(5);
			int value1 = tree1->lookup(5); int value2 = tree2->lookup(5); int value3 = tree3->lookup(5);
			THEN("Value should be 12") {
				REQUIRE(value == 12);
				REQUIRE(value1 == 12); REQUIRE(value2 == 12); REQUIRE(value3 == 12);
			}
		}

		WHEN("Remove is called") {
			tree->remove(8); tree1->remove(8); tree2->remove(8); tree3->remove(8);
		}

		WHEN("lookup is called") {

			int value = tree->lookup(5);
			int value1 = tree1->lookup(5); int value2 = tree2->lookup(5); int value3 = tree3->lookup(5);

			THEN("Value should be 12") {

				REQUIRE(value == 12);
				REQUIRE(value1 == 12); REQUIRE(value2 == 12); REQUIRE(value3 == 12);

			}
		}

		delete tree; delete tree1; delete tree2; delete tree3;
	}
}
