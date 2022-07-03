#ifndef _AVL_H_
#define _AVL_H_
#include <vector>
#include "Apartment.h"

class AVL {
 public:
  /**
   * To manage the tree nodes, we use a nested struct. This struct contains the
   * apartment corresponding to the node, the left son and the right son of the
   * node, both of them node type themselves.
   */
  struct node {
      /**
       * Constructor - It can be expanded
       * @param data the corresponding apartment object
       * @param left child
       * @param right child
       */
      node (Apartment data, node *left, node *right)
          : data_ (data), left_ (left), right_ (right)
      {}
      /**
       *
       * @return the left child of this node
       */

      node *get_left () const
      {
        return left_;
      }
      /**
       *
       * @return the right child of this node
       */
      node *get_right () const
      {
        return right_;
      }
      /**
     *
     * @return the const reference apartment of this node
     */
      const Apartment &get_data () const
      {
        return data_;
      }

      size_t get_height() const{
        return height;
      }

      int get_balance() const{
        return balance;
      }

      Apartment data_;
      node *left_, *right_;
      size_t height = 1;
      int balance = 0;

  };
  /**
   * Constructor. Constructs an empty AVL tree
   */
  AVL ();
  /**
   * Copy constructor
   * @param other
   */
  AVL (const AVL &other);
  /**
   * A constructor that receives a vector of a pairs. Each such pair is an
   * apartment that will inserted to the tree. Insert can be used to insert the
   * apartments a vector of a pairs
   * @param coordinates
   */
  AVL (std::vector<std::pair<double, double>> coordinates);
  /**
   *
   * @return the root node of this tree
   */
  ~AVL();

  void level_order(node* root);

  node *get_root () const{
    return root_;
  }
  /**
   * The function inserts the new apartment into the tree so that it maintains
   * the legality of the tree.
   * @param apartment
   */
  void insert (const Apartment &apartment);
  /**
   * The function deletes the apartment from the tree
   * (if it is in that tree) so that it maintains
   * the legality of the tree.
   * @param apartment
   */
  void erase (const Apartment &apartment);

  node* rec_erase(node*, const Apartment&);

  node* rec_insert(node*, node*);

  void update(node*);

  node* re_balance(node*);

  node* left_left(node*);

  node* left_right(node*);

  node* right_right(node*);

  node* right_left(node*);

  node* right_rotate(node*);

  node* left_rotate(node*);

  node* find_successor(node*);

  node* node_find(const Apartment&);

  /**
   * The class should support forward iterator. Don't forget to define the
   * iterator traits and all the actions required to support a forward iterator
   * The iterator will move in preorder.
   */
  class ConstIterator;
  class Iterator {
   private:
    node* curr;
    friend class ConstIterator;
    std::vector<node*> tree;
    void traverse(AVL::node*);

   public:
    typedef Apartment value_type;
    typedef const Apartment& reference;
    typedef const Apartment* pointer;
    typedef std::forward_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;

    // constructor
    Iterator(node* curr);
    // next operator
    Iterator& operator++();

    Iterator operator++(int);
    // equality
    bool operator==(const Iterator &other) const;
    // non equality
    bool operator!=(const Iterator& other) const;
    // dereference
    reference operator*();
    // address
    pointer operator->();

    bool empty(){
      return curr == nullptr;
    }
  };


  class ConstIterator{
   private:
    node* curr;
    std::vector<node*> tree;
    void traverse(AVL::node*);

   public:
    typedef const Apartment value_type;
    typedef const Apartment& reference;
    typedef const Apartment* pointer;
    typedef std::forward_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;

    // constructor
    ConstIterator(node* curr);

    ConstIterator(const Iterator& it);
    // next operator
    ConstIterator& operator++();

    ConstIterator operator++(int);
    // equality
    bool operator==(const ConstIterator &other) const;
    // non equality
    bool operator!=(const ConstIterator& other) const;
    // dereference
    reference operator*();
    // address
    pointer operator->();

    bool empty(){
      return curr == nullptr;
    }
  };


  using iterator = Iterator;
  using const_iterator = ConstIterator;

  iterator begin ();
  const_iterator begin () const;
  const_iterator cbegin () const;
  iterator end ();
  const_iterator end () const;
  const_iterator cend () const;
  /**
   * The function returns an iterator to the item that corresponds to the item
   * we were looking for. If there is no such member, returns end ().
   * @param data apartment to search
   * @return iterator to the item that corresponds to the item
   * we were looking for. If there is no such member, returns end ().
   */
  iterator find (const Apartment &data);
  /**
   * The function returns an iterator to the item that corresponds to the item
   * we were looking for. If there is no such member, returns end ().
   * @param data apartment to search
   * @return iterator to the item that corresponds to the item
   * we were looking for. If there is no such member, returns end ().
   */
  const_iterator find (const Apartment &data) const;
  /**
   * Insertion operator, prints the apartment in the tree in
   * preorder traversal.
   * Each apartmnet will be printed in the format: (x,y)\n
   * @param os reference to std::ostream
   * @param avl tree
   * @return os reference to std::ostream
   */
  friend std::ostream &operator<< (std::ostream &os, const AVL &avl);

 private:
  node *root_;
};

#endif //_AVL_H_
