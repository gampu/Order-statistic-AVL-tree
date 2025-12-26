#include "src/avl.h"
#include <gtest/gtest.h>

/* Custom data-type for testing purposes */
struct customNode {
  /* An integer */
  int val;

  /* A character */
  char c;

  /* Provide definition of < to help compare 2 'CustomNode's */
  bool operator<(const struct customNode &other) {
    if (val < other.val) {
      return true;
    }
    if (val == other.val) {
      return c < other.c;
    }
    return false;
  }

  /* Provide definition of == to help compare 2 'CustomNode' for equality */
  bool operator==(const struct customNode &other) {
    return (val == other.val && c == other.c);
  }

  /* Provide definition of > to help compare 2 'CustomNode's */
  bool operator>(const struct customNode &other) {
    return !(*this < other) && !(*this == other);
  }

  /* A friendly function to output CustomNode easily */
  friend std::ostream &operator<<(std::ostream &out,
                                  const struct customNode &obj) {
    out << obj.val << " " << obj.c;
    return out;
  }
};
/* A handy alias for struct customNode */
typedef struct customNode CustomNode;

TEST(AVLTEST1, IntegerSimpleCase) {
  /* First, lets' create an AVL tree of integers from 11-20 */
  AVL::Tree<int> t1;
  for (int i = 11; i <= 20; ++i) {
    EXPECT_TRUE(t1.insert(i)) << "Insertion failed.\n";
  }

  /* Size of t1 should be 10 */
  EXPECT_TRUE(t1.size() == 10);

  /* 15 should be present and 25 should not be */
  EXPECT_TRUE(t1.search(15));
  EXPECT_TRUE(!t1.search(25));

  /* Let's do a successful delete of 15 and an unsuccessful delete of 30 */
  EXPECT_TRUE(t1.findAndDeleteByCopying(15));
  EXPECT_TRUE(!t1.findAndDeleteByCopying(30));

  /* Let's do a forward inorder iteration */
  for (AVL::Iterator<int> fwd(t1.begin()); fwd != nullptr; ++fwd) {
    /* Print element value */
    std::cout << *fwd << "\n";
  }
  std::cout << "\n";

  /* Let's do a backward inorder iteration */
  for (AVL::Iterator<int> bwd(t1.rbegin()); bwd != nullptr; --bwd) {
    /* Print element value */
    std::cout << *bwd << "\n";
  }
  std::cout << "\n";

  /* Let's use getIndexGivenValue */
  /* 17 is the 6th element */
  EXPECT_TRUE(t1.getIndexGivenValue(17) == 6);
  /* 25 doesn't exist */
  EXPECT_TRUE(t1.getIndexGivenValue(25) == 0);

  /* Let's use getValueGivenIndex */
  /* 16 is at index 5 */
  EXPECT_TRUE(t1.getValueGivenIndex(5)->el == 16);
  /* t1.size() + 1 index is invalid */
  EXPECT_TRUE(t1.getValueGivenIndex(t1.size() + 1) == NULL);

  /* Let's use lowerbound */
  /* lowerbound of 14 is 14 */
  EXPECT_TRUE(t1.lowerbound(14)->el == 14);
  /* lowerbound of 15 is 16 */
  EXPECT_TRUE(t1.lowerbound(15)->el == 16);
  /* lowerbound of 9 is 11 */
  EXPECT_TRUE(t1.lowerbound(9)->el == 11);
  /* lowerbound of 21 doesn't exist in t1 */
  EXPECT_TRUE(t1.lowerbound(21) == NULL);

  /* Create t2 and t3 using copy constructor and assignment operator
     respectively */
  AVL::Tree<int> t2(t1), t3 = t2;

  /* Insert 50 in t2 and output it */
  t2.insert(50);
  for (AVL::Iterator<int> fwd(t2.begin()); fwd != nullptr; ++fwd) {
    /* Print element value */
    std::cout << *fwd << "\n";
  }
  std::cout << "\n";

  /* Clear t3 and output it */
  t3.clear();
  for (AVL::Iterator<int> fwd(t3.begin()); fwd != nullptr; ++fwd) {
    /* Print element value */
    std::cout << *fwd << "\n";
  }
  std::cout << "\n";
}

TEST(AVLTEST2, CustomNodeSimpleCase) {
  /* Let's try using some custom data-type */
  AVL::Tree<CustomNode> t4;
  for (int i = 10; i <= 20; ++i) {
    EXPECT_TRUE(t4.insert({i, 'a'}));
  }
  for (int i = 21; i <= 30; ++i) {
    EXPECT_TRUE(t4.insert({i, 'b'}));
  }

  /* Size of t4 should be 21 */
  EXPECT_TRUE(t4.size() == 21);

  /* Let's output it */
  /* Forward iteration */
  for (AVL::Iterator<CustomNode> it(t4.begin()); it != nullptr; ++it) {
    /* Print element value */
    std::cout << *it << "\n";
  }
  std::cout << "\n";

  /* Backward iteration */
  for (AVL::Iterator<CustomNode> it(t4.rbegin()); it != nullptr; --it) {
    /* Print element value */
    std::cout << *it << "\n";
  }
}

TEST(AVLTEST3, IntegerSimpleCase) {
  /* Let's do a heavy testing using 1 million integers */
  AVL::Tree<int> t5;
  for (int i = 1; i <= 1e6; ++i) {
    EXPECT_TRUE(t5.insert(i));
  }

  /* Iterate over t5 and test whether the ith value is equal to i */
  int i = 1;
  for (AVL::Iterator<int> fwd(t5.begin()); fwd != nullptr; ++fwd) {
    EXPECT_TRUE(*fwd == i);
    ++i;
  }

  /* Similarly, iterate backward and test whether the ith value is equal to i */
  --i;
  for (AVL::Iterator<int> bwd(t5.rbegin()); bwd != nullptr; --bwd) {
    EXPECT_TRUE(*bwd == i);
    --i;
  }
  /* Testing complete */
}
