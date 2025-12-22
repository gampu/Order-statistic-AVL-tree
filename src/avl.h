#ifndef AVLNAMESPACE_H
#define AVLNAMESPACE_H

/* Basic input/output */
#include <iostream>

/* Set data structure */
#include <set>

/* assert */
#include <cassert>

/* One stop header-only namespace for AVL tree*/
namespace AVL {
/* Individual element/node of an AVL tree */
template <typename T> class Node {
public:
  /* Value stored */
  T el;

  /* Parent of current node */
  Node<T> *par;

  /* Left child of current node */
  Node<T> *left;

  /* Right child of current node */
  Node<T> *right;

  /* Size of the subtree rooted at current node */
  int sz;

  /* Height of the subtree rooted at current node */
  int height;

  /* Balance factor of the subtree rooted at current node */
  int bf;

  /* Constructor for a Node object */
  Node(T value) {
    el = value;
    par = 0;
    left = 0;
    right = 0;
    sz = 1;
    height = 1;
    bf = 0;
  }
}; /* End class Node */

/* Very basic iterator for iterating over the nodes of an AVL tree */
template <typename T> class Iterator {
public:
  /* Pointer to a Node object */
  Node<T> *ptr = 0;

  /* Constructor for an Iterator object */
  Iterator(Node<T> *p) { ptr = p; }

  /* Set member function useful for using previously defined 'Iterator' object
   */
  void set(Node<T> *p) { ptr = p; }
}; /* End class Iterator */

/* AVL tree class */
template <typename T> class Tree {
public:
  /* Basic constructor for an AVL tree */
  Tree(void) {
    root = 0;
    cnt = 0;
  }

  /* Copy constructor */
  Tree(const Tree &other) {
    root = 0;
    cnt = 0;
    copyAndInsert(other.root);
  }

  /* Assignment operator */
  const Tree &operator=(const Tree &right) {
    /* Avoid self-assignment */
    if (&right != this) {
      /* Clear current tree */
      clear();
      /* Recreate by copying 'right' */
      copyAndInsert(right.root);
    }
    return *this;
  }

  /* Returns size of an AVL tree */
  int size(void) { return cnt; }

  /* Insert a value 'el' into an AVL tree
     Returns true on insertion, false if not */
  bool insert(T el) {
    Node<T> *pre = 0, *p = root, *node = new Node<T>(el);
    if (!p) {
      root = node;
      ++cnt;
      return true;
    }
    while (p != 0) {
      pre = p;
      if (el == p->el) {
        delete node;
        return false;
      } else if (el < p->el) {
        p = p->left;
      } else {
        p = p->right;
      }
    }
    if (el < pre->el) {
      pre->left = node;
    } else {
      pre->right = node;
    }
    node->par = pre;
    ++cnt;
    balance(pre);
    return true;
  }

  /* Search for 'el' in an AVL tree
     Returns true if found, false if not */
  bool search(T el) {
    Node<T> *p = root;
    while (p != 0) {
      if (el == p->el) {
        return true;
      } else if (el < p->el) {
        p = p->left;
      } else {
        p = p->right;
      }
    }
    return false;
  }

  /* Delete 'el'(if found) from an AVL tree
     Returns true if deleted, false if not found */
  bool findAndDeleteByCopying(T el) {
    if (!search(el)) {
      return false;
    }
    Node<T> *p = root, *pre = 0;
    while (p != 0) {
      if (p->el == el) {
        break;
      }
      pre = p;
      if (el < p->el) {
        p = p->left;
      } else {
        p = p->right;
      }
    }
    if (pre == 0) {
      deleteByCopying(root, pre);
    } else if (pre->left == p) {
      deleteByCopying(pre->left, pre);
    } else {
      deleteByCopying(pre->right, pre);
    }
    return true;
  }

  /* Given a 'val' returns its 1-based index in the ascending
     sequence of all the elements in the tree
     Returns 0 if not found, non-zero and less than or equal to 'cnt'
     if found. */
  int getIndexGivenValue(T val) {
    if (!search(val)) {
      return 0;
    }
    return getIndexGivenValue(root, val);
  }

  /* Given a 1-based index 'pos', returns the node pointer at
     index 'pos' in the ascending sequence of all the elements in
     the tree
     If pos is invalid, returns null */
  Node<T> *getValueGivenIndex(int pos) {
    if (!(pos >= 1 && pos <= cnt)) {
      return 0;
    }
    return getValueGivenIndex(root, pos);
  }

  /* Returns a pointer to smallest 'Node< T >' whose 'el' is atleast
     'val' */
  Node<T> *lowerbound(T val) {
    Node<T> *ans = 0, *ptr = root;
    while (ptr) {
      if (ptr->el < val) {
        ptr = ptr->right;
      } else {
        ans = ptr;
        ptr = ptr->left;
      }
    }
    return ans;
  }

  /* Returns begin pointer which points to the smallest node in the
     tree */
  Node<T> *begin(void) {
    Node<T> *p = root;
    while (p && p->left) {
      p = p->left;
    }
    return p;
  }

  /* Returns reverse begin pointer which points to the largest node
     in the tree */
  Node<T> *rbegin(void) {
    Node<T> *p = root;
    while (p && p->right) {
      p = p->right;
    }
    return p;
  }

  /* Implements forward iterator functionality
     Given an Iterator object 'it', updates 'it.ptr' to point to
     the next greater element in the inorder traversal */
  void nextNode(Iterator<T> &it) {
    Node<T> *&p = it.ptr;
    if (!p) {
      return;
    }
    if (p->right != 0) {
      p = p->right;
      while (p->left != 0) {
        p = p->left;
      }
      return;
    }
    Node<T> *q = p->par;
    while (q && q->el < p->el) {
      q = q->par;
    }
    p = q;
  }

  /* Implements backward iterator functionality
     Given an Iterator object 'it', updates 'it.ptr' to point to
     the next smaller element in the inorder traversal */
  void prevNode(Iterator<T> &it) {
    Node<T> *&p = it.ptr;
    if (!p) {
      return;
    }
    if (p->left != 0) {
      p = p->left;
      while (p->right != 0) {
        p = p->right;
      }
      return;
    }
    Node<T> *q = p->par;
    while (q && q->el > p->el) {
      q = q->par;
    }
    p = q;
  }

  /* Clears an AVL tree */
  void clear(void) {
    deleteSubtree(root);
    root = 0;
    cnt = 0;
  }

  /* Destructor for an AVL tree */
  ~Tree(void) { clear(); }

private:
  /* Private data members and member-functions
     Not intended for public usage */

  /* Root of an AVL tree */
  Node<T> *root;

  /* No. of nodes currently present in the tree */
  int cnt;

  /* Helper function for copying a subtree rooted at 'ptr' */
  void copyAndInsert(Node<T> *ptr) {
    if (!ptr) {
      return;
    }
    insert(ptr->el);
    copyAndInsert(ptr->left);
    copyAndInsert(ptr->right);
  }

  /* Update subtree's size, height & balance-factor pointed to by
     'ptr' */
  void update(Node<T> *ptr) {
    ptr->sz = sz(ptr->left) + sz(ptr->right) + 1;
    ptr->height = std::max(hgt(ptr->left), hgt(ptr->right)) + 1;
    ptr->bf = hgt(ptr->right) - hgt(ptr->left);
  }

  /* Left-rotate balancing technique */
  void LR(Node<T> *gr, Node<T> *par, Node<T> *ch) {
    gr->right = par->left;
    if (gr->right) {
      gr->right->par = gr;
    }
    par->left = gr;
    par->par = gr->par;
    gr->par = par;
    if (par->par == 0) {
      root = par;
    } else {
      if (par->par->left == gr) {
        par->par->left = par;
      } else {
        par->par->right = par;
      }
    }
    update(gr);
    update(par);
  }

  /* Right rotate balancing technique */
  void RR(Node<T> *gr, Node<T> *par, Node<T> *ch) {
    gr->left = par->right;
    if (gr->left) {
      gr->left->par = gr;
    }
    par->right = gr;
    par->par = gr->par;
    gr->par = par;
    if (par->par == 0) {
      root = par;
    } else {
      if (par->par->left == gr) {
        par->par->left = par;
      } else {
        par->par->right = par;
      }
    }
    update(gr);
    update(par);
  }

  /* Balance subtree rooted at 'ptr' */
  void balance(Node<T> *ptr) {
    while (ptr != 0) {
      update(ptr);
      if (ptr->bf == 2 || ptr->bf == -2) {
        if (ptr->bf == 2) {
          if (ptr->right->bf == -1) {
            RR(ptr->right, ptr->right->left, ptr->right->left->left);
          }
          LR(ptr, ptr->right, ptr->right->right);
        } else {
          if (ptr->left->bf == 1) {
            LR(ptr->left, ptr->left->right, ptr->left->right->right);
          }
          RR(ptr, ptr->left, ptr->left->left);
        }
        ptr = ptr->par;
      }
      ptr = ptr->par;
    }
  }

  /* Returns size of the subtree rooted at 'ptr' */
  int sz(Node<T> *ptr) { return (ptr != 0 ? ptr->sz : 0); }

  /* Returns height of the subtree rooted at 'ptr' */
  int hgt(Node<T> *ptr) { return (ptr != 0 ? ptr->height : 0); }

  /* Helper function for 'findAndDeleteByCopying' */
  void deleteByCopying(Node<T> *&ptr, Node<T> *par) {
    Node<T> *del = ptr, *pre = 0, *p = 0;
    if (ptr->right == 0) {
      ptr = ptr->left;
      if (ptr) {
        ptr->par = par;
      }
      balance(par);
    } else if (ptr->left == 0) {
      ptr = ptr->right;
      if (ptr) {
        ptr->par = par;
      }
      balance(par);
    } else {
      pre = ptr;
      p = ptr->left;
      while (p->right) {
        pre = p;
        p = p->right;
      }
      ptr->el = p->el;
      if (pre->left == p) {
        pre->left = p->left;
        if (pre->left) {
          pre->left->par = pre;
        }
      } else {
        pre->right = p->left;
        if (pre->right) {
          pre->right->par = pre;
        }
      }
      balance(pre);
      del = p;
    }
    delete del;
    --cnt;
  }

  /* Helper function for 'getIndexGivenValue' */
  int getIndexGivenValue(Node<T> *ptr, T val) {
    if (ptr->el == val) {
      return sz(ptr->left) + 1;
    } else if (ptr->el > val) {
      return getIndexGivenValue(ptr->left, val);
    } else {
      return sz(ptr->left) + 1 + getIndexGivenValue(ptr->right, val);
    }
  }

  /* Helper function for 'getValueGivenIndex' */
  Node<T> *getValueGivenIndex(Node<T> *ptr, int ind) {
    int res = sz(ptr->left) + 1;
    if (res == ind) {
      return ptr;
    } else if (res > ind) {
      return getValueGivenIndex(ptr->left, ind);
    } else {
      return getValueGivenIndex(ptr->right, ind - res);
    }
  }

  /* Helper function for the tree destructor */
  void deleteSubtree(Node<T> *p) {
    if (!p) {
      return;
    }
    deleteSubtree(p->left);
    deleteSubtree(p->right);
    delete p;
  }
}; /* End class Tree */
} // namespace AVL
/* End AVL namespace */
#endif
