#include "AVL.h"

AVL::AVL() {
  this->root_ = nullptr;
}

AVL::AVL(std::vector <std::pair<double, double>> coordinates) : AVL(){
  for (std::pair<double,double> coordinate : coordinates){
      Apartment temp(coordinate);
      this->insert(temp);
    }
}

AVL::AVL(const AVL &other) : AVL(){
  for (const_iterator begin = other.begin(); begin != other.end(); ++begin){
      this->insert(*begin);
  }
}

AVL::~AVL(){
    level_order(root_);
}

void AVL::level_order(AVL::node *root) {
    if (root == nullptr) return;
    if (root->left_) AVL::level_order(root->left_);
    if (root->right_) AVL::level_order(root->right_);
    delete root;
    root = nullptr;
}

void AVL::insert(const Apartment &apartment) {
  if (root_ == nullptr){
      root_ = new AVL::node(apartment, nullptr, nullptr);
      return;
    }
  AVL::node* node_to_insert = new AVL::node(apartment,nullptr, nullptr);
  root_ = rec_insert(root_, node_to_insert);
}

void AVL::erase(const Apartment &apartment) {
  if (root_ == nullptr) return;
  iterator it = find (apartment);
  if (it.empty()) return;
  if (root_->right_ == nullptr && root_->left_ == nullptr){
      delete root_;
      root_ = nullptr;
      return;
  }
  root_ = rec_erase(root_, apartment);
}

AVL::node* AVL::rec_erase(AVL::node* root, const Apartment& apartment){
  if (root == nullptr) return root;

  if (apartment > root->get_data()){
      root->right_ = rec_erase(root->right_, apartment);
    }
  else if (apartment<root->get_data()){
      root->left_ = rec_erase(root->left_, apartment);
    }
  else{
      if (root->right_ == nullptr && root->left_ == nullptr){
          delete root;
          root = nullptr;
          return root;
        }
      if (root->right_ != nullptr && root->left_ == nullptr){
          root->data_ = root->right_->data_;
          delete root->right_;
          root->right_ = nullptr;
          return root;
        }
      if (root->left_ != nullptr && root->right_ == nullptr){
          root->data_ = root->left_->data_;
          delete root->left_;
          root->left_ = nullptr;
          return root;
        }
      if (root->left_ != nullptr && root->right_ != nullptr){
          AVL::node* successor = find_successor(root);
          root->data_ = successor->data_;
          root->right_ = rec_erase(root->right_, successor->get_data());
          return root;
        }
    }
  update(root);
  return re_balance(root);
}


AVL::node* AVL::rec_insert(AVL::node* root, AVL::node* curr){
  if (root == nullptr) return curr;

  if (curr->get_data() < root->get_data()){
      root->left_ = rec_insert(root->left_, curr);
    }
  else {
      root->right_ = rec_insert(root->right_, curr);
    }
  update(root);
  return re_balance(root);
}

void AVL::update(AVL::node* curr){
  size_t left_height = 0;
  size_t right_height = 0;
  if (curr->left_) left_height = curr->left_->height;
  if (curr->right_) right_height = curr->right_->height;

  size_t max_height = (left_height>right_height) ? left_height:right_height;
  curr->height = max_height + 1;
  curr->balance = right_height-left_height;
}

AVL::node *AVL::re_balance(AVL::node *curr) {
  if (curr->balance == -2){
      if (curr->left_->balance <= 0) return left_left(curr);
      else return left_right(curr);
  }
  else if (curr->balance == 2) {
      if (curr->right_->balance >= 0) return right_right(curr);
      else return right_left(curr);
  }
  return curr;
}

AVL::node *AVL::left_left(AVL::node *curr) {
  return right_rotate(curr);
}

AVL::node *AVL::left_right(AVL::node *curr) {
  curr->left_ = left_rotate(curr->left_);
  return left_left(curr);
}

AVL::node *AVL::right_right(AVL::node *curr) {
  return left_rotate(curr);
}

AVL::node *AVL::right_left(AVL::node *curr) {
  curr->right_ = right_rotate(curr->right_);
  return right_right(curr);
}

AVL::node *AVL::left_rotate(AVL::node *curr) {
  AVL::node* temp = curr->right_;
  curr->right_ = temp->left_;
  temp->left_ = curr;
  update(curr);
  update(temp);
  return temp;
}

AVL::node *AVL::right_rotate(AVL::node *curr) {
  AVL::node* temp = curr->left_;
  curr->left_ = temp->right_;
  temp->right_ = curr;
  update(curr);
  update(temp);
  return temp;
}

AVL::node *AVL::find_successor(AVL::node *curr) {
  AVL::node* res = nullptr;
  if (curr->right_ == nullptr){
      AVL::node* temp = root_;
      while (temp){
          if (curr->get_data() < temp->get_data()){
              res = temp;
              temp = temp->left_;
              continue;
            }
          if (curr->get_data() > temp->get_data()){
              temp = temp->right_;
              continue;
            }
          else break;
        }
    }
  else {
      AVL::node* temp = curr->right_;
      while(temp){
          res = temp;
          temp = temp->left_;
        }
    }
  return res;
}


AVL::Iterator::Iterator(AVL::node *curr) : curr(curr){
  this->curr = curr;
  traverse(curr);
}

void AVL::Iterator::traverse(AVL::node *temp) {
  if (temp == nullptr) return;
  tree.push_back(temp);
  if (temp->left_) traverse(temp->left_);
  if (temp->right_) traverse(temp->right_);
}

AVL::Iterator &AVL::Iterator::operator++() {
  if (!tree.empty()) tree.erase (tree.begin());
  if (tree.empty()){
      this->curr = nullptr;
      return *this;
    }
  AVL::node* next = tree.at(0);
  this->curr = next;
  return *this;
}

AVL::Iterator AVL::Iterator::operator++ (int)
{
  AVL::Iterator cpy = *this;
  ++(*this);
  return cpy;
}

AVL::Iterator::reference AVL::Iterator::operator*(){
  return curr->get_data();
}

AVL::Iterator::pointer AVL::Iterator::operator->() {
  if (this->curr == nullptr) return nullptr;
  return &(curr->get_data());
}

bool AVL::Iterator::operator!=(const Iterator &other) const {
  if (this->curr == nullptr && other.curr == nullptr) return false;
  if (this->curr == nullptr && other.curr != nullptr) return true;
  if (this->curr != nullptr && other.curr == nullptr) return true;
  if (curr->get_data() == other.curr->get_data()) return false;
  return true;
}

bool AVL::Iterator::operator==(const Iterator &other) const {
  if (this->curr == nullptr && other.curr == nullptr) return true;
  if (this->curr == nullptr && other.curr != nullptr) return false;
  if (this->curr != nullptr && other.curr == nullptr) return false;
  if (curr->get_data() == other.curr->get_data()) return true;
  return false;
}


AVL::ConstIterator::ConstIterator(node *curr) {
  this->curr = curr;
  traverse(curr);
}

AVL::ConstIterator::ConstIterator(const Iterator &it) {
  this->curr = it.curr;
  this->tree = it.tree;
}

void AVL::ConstIterator::traverse(AVL::node *temp) {
  if (temp == nullptr) return;
  tree.push_back(temp);
  if (temp->left_) traverse(temp->left_);
  if (temp->right_) traverse(temp->right_);
}

AVL::ConstIterator &AVL::ConstIterator::operator++() {
  tree.erase (tree.begin());
  if (tree.empty()){
      this->curr = nullptr;
      return *this;
    }
  AVL::node* next = tree.at(0);
  this->curr = next;
  return *this;
}

AVL::ConstIterator AVL::ConstIterator::operator++ (int)
{
  AVL::ConstIterator cpy = *this;
  ++(*this);
  return cpy;
}

AVL::ConstIterator::reference AVL::ConstIterator::operator*(){
  return curr->get_data();
}

AVL::ConstIterator::pointer AVL::ConstIterator::operator->(){
  return &(curr->get_data());
}

bool AVL::ConstIterator::operator!=(const ConstIterator &other) const {
  if (this->curr == nullptr && other.curr == nullptr) return false;
  if (this->curr == nullptr && other.curr != nullptr) return true;
  if (this->curr != nullptr && other.curr == nullptr) return true;
  if (curr->get_data() == other.curr->get_data()) return false;
  return true;
}

bool AVL::ConstIterator::operator==(const ConstIterator &other) const{
  if (this->curr == nullptr && other.curr == nullptr) return true;
  if (this->curr == nullptr && other.curr != nullptr) return false;
  if (this->curr != nullptr && other.curr == nullptr) return false;
  if (curr->get_data() == other.curr->get_data()) return true;
  return false;
}


AVL::node* AVL::node_find(const Apartment &curr) {
  AVL::node* temp = root_;
  while(temp){
      if (temp->get_data() == curr) break;
      if (temp->get_data() < curr){
          temp = temp->right_;
        }
      temp = temp->left_;
    }
  return temp;
}



AVL::iterator AVL::begin() {
  AVL::iterator it(root_);
  return it;
}

AVL::const_iterator AVL::begin() const {
  AVL::const_iterator it(root_);
  return it;
}

AVL::const_iterator AVL::cbegin() const {
  AVL::node* temp = root_;
  AVL::node* res = nullptr;
  while(temp){
      res = temp;
      temp = temp->left_;
    }
  AVL::const_iterator cit(res);
  return cit;
}

AVL::iterator AVL::end() {
  AVL::node* res = nullptr;
  AVL::iterator it(res);
  return it;
}

AVL::const_iterator AVL::end() const {
  AVL::node* res = nullptr;
  AVL::const_iterator it(res);
  return it;
}

AVL::const_iterator AVL::cend() const {
  AVL::node* res = nullptr;
  AVL::const_iterator it(res);
  return it;
}

AVL::iterator AVL::find(const Apartment &curr) {
  //if (begin() == nullptr) return null_it;
  for (AVL::iterator it = this->begin(); it != this->end(); it++){
      if (*it == curr) return it;
    }
  return this->end();
}

AVL::const_iterator AVL::find(const Apartment &curr) const {
  for (AVL::const_iterator it = this->begin(); it != this->end(); ++it){
      if (*it == curr) return it;
    }
  return this->end();
}

std::ostream &operator<< (std::ostream &os, const AVL &avl){
  for (AVL::const_iterator it = avl.cbegin(); it != avl.cend(); it++){
    os << *it;
  }
  return os;
}