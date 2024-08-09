#ifndef TREE_HPP
#define TREE_HPP

#include <functional>
#include <stdexcept>
#include <stack>
#include <queue>
#include <initializer_list>
#include "treeIter.hpp"
#include "constTreeIter.hpp"
#include "LNRIter.hpp"
#include "constLNRIter.hpp"

namespace isaychev
{
  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
    using Tree = BSTree< Key, Value, Compare >;
    using node_t = detail::TreeNode< Key, Value >;
    using const_iterator = ConstTreeIter< Key, Value, Compare >;
    using iterator = TreeIter< Key, Value, Compare >;
    using value_t = std::pair< Key, Value >;
    using lnr_iterator = LNRIter< Key, Value, Compare >;
    using const_lnr_iterator = ConstLNRIter< Key, Value, Compare >;

   public:
    BSTree();
    ~BSTree();
    BSTree(const Tree & rhs);
    BSTree(Tree && rhs);
    BSTree(std::initializer_list< value_t > l);
    template < class InputIt >
    BSTree(InputIt first, InputIt last);
    Tree & operator=(const Tree & rhs);
    Tree & operator=(Tree && rhs);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    lnr_iterator lnrbegin();
    lnr_iterator lnrend();
    const_lnr_iterator lnrbegin() const;
    const_lnr_iterator lnrend() const;
    const_lnr_iterator clnrbegin() const;
    const_lnr_iterator clnrend() const;

    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;
    void swap(Tree & other) noexcept;
    iterator find(const Key & key);
    const_iterator find(const Key & key) const;
    size_t count(const Key & key) const;
    std::pair< iterator, iterator > equal_range(const Key & key);
    std::pair< const_iterator, const_iterator > equal_range(const Key & key) const;
    iterator lower_bound(const Key & key);
    const_iterator lower_bound(const Key & key) const;
    iterator upper_bound(const Key & key);
    const_iterator upper_bound(const Key & key) const;

    Value & at(const Key & key);
    const Value & at(const Key & key) const;
    Value & operator[](const Key & key);
    std::pair< iterator, bool > insert(const value_t & value);
    template < class InputIt >
    void insert(InputIt first, InputIt last);
    iterator erase(iterator pos);
    const_iterator erase(const_iterator pos);
    size_t erase(const Key & key);
    iterator erase(iterator first, iterator last);
    iterator erase(const_iterator first, const_iterator last);

    template < class F >
    F traverse_lnr(F f) const;
    template < class F >
    F traverse_rnl(F f) const;
    template < class F >
    F traverse_breadth(F f) const;

   private:
    node_t * root_;
    Compare cmp_;
    size_t size_;

    void delete_tree(node_t * ptr) noexcept;
    void erase_el(node_t * curr);
    std::pair< node_t *, bool > insert_new(const value_t & value);
    node_t * go_down(node_t * curr, const Key & key) const;
  };

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
   root_(nullptr),
   cmp_(),
   size_(0)
  {}

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear();
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const Tree & rhs):
   BSTree(rhs.cbegin(), rhs.cend())
  {}

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(Tree && rhs):
   root_(rhs.root_),
   cmp_(rhs.cmp_),
   size_(rhs.size_)
  {
    rhs.root_ = nullptr;
    rhs.size_ = 0;
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(std::initializer_list< value_t > l):
   BSTree(l.begin(), l.end())
  {}

  template < class Key, class Value, class Compare >
  template < class InputIt >
  BSTree< Key, Value, Compare >::BSTree(InputIt first, InputIt last):
   root_(nullptr),
   cmp_(),
   size_(0)
  {
    for (; first != last; ++first)
    {
      insert(*first);
    }
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(const Tree & rhs)
  {
    if (this != std::addressof(rhs))
    {
      clear();
      cmp_ = rhs.cmp_;
      for (auto i = rhs.cbegin(); i != rhs.cend(); ++i)
      {
        (*this)[(*i).first] = (*i).second;
      }
    }
    return *this;
  }

  template < class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(Tree && rhs)
  {
    if (this != std::addressof(rhs))
    {
      root_ = rhs.root_;
      cmp_ = rhs.cmp_;
      size_ = rhs.size_;
      rhs.root_ = nullptr;
      rhs.size_ = 0;
    }
    return *this;
  }

  template < class Key, class Value, class Compare >
  TreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::begin() noexcept
  {
    return TreeIter< Key, Value, Compare >(detail::traverse_left(root_));
  }

  template < class Key, class Value, class Compare >
  TreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::end() noexcept
  {
    return TreeIter< Key, Value, Compare >();
  }

  template < class Key, class Value, class Compare >
  ConstTreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::begin() const noexcept
  {
    return ConstTreeIter< Key, Value, Compare >(detail::traverse_left(root_));
  }

  template < class Key, class Value, class Compare >
  ConstTreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::end() const noexcept
  {
    return ConstTreeIter< Key, Value, Compare >();
  }

  template < class Key, class Value, class Compare >
  ConstTreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::cbegin() const noexcept
  {
    return ConstTreeIter< Key, Value, Compare >(detail::traverse_left(root_));
  }

  template < class Key, class Value, class Compare >
  ConstTreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::cend() const noexcept
  {
    return ConstTreeIter< Key, Value, Compare >();
  }

  template < class Key, class Value, class Compare >
  LNRIter< Key, Value, Compare > BSTree< Key, Value, Compare >::lnrbegin()
  {
    std::stack< node_t * > s;
    auto curr = root_;
    while (curr && curr->left)
    {
      s.push(curr);
      curr = curr->left;
    }
    return lnr_iterator(curr, std::move(s));
  }

  template < class Key, class Value, class Compare >
  LNRIter< Key, Value, Compare > BSTree< Key, Value, Compare >::lnrend()
  {
    return lnr_iterator(nullptr, std::stack< node_t * >());
  }

  template < class Key, class Value, class Compare >
  ConstLNRIter< Key, Value, Compare > BSTree< Key, Value, Compare >::lnrbegin() const
  {
    std::stack< node_t * > s;
    auto curr = root_;
    while (curr && curr->left)
    {
      s.push(curr);
      curr = curr->left;
    }
    return const_lnr_iterator(curr, std::move(s));
  }

  template < class Key, class Value, class Compare >
  ConstLNRIter< Key, Value, Compare > BSTree< Key, Value, Compare >::lnrend() const
  {
    return const_lnr_iterator(nullptr, std::stack< node_t * >());
  }

  template < class Key, class Value, class Compare >
  ConstLNRIter< Key, Value, Compare > BSTree< Key, Value, Compare >::clnrbegin() const
  {
    std::stack< node_t * > s;
    auto curr = root_;
    while (curr && curr->left)
    {
      s.push(curr);
      curr = curr->left;
    }
    return const_lnr_iterator(curr, std::move(s));
  }

  template < class Key, class Value, class Compare >
  ConstLNRIter< Key, Value, Compare > BSTree< Key, Value, Compare >::clnrend() const
  {
    return const_lnr_iterator(nullptr, std::stack< node_t * >());
  }

  template < class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template < class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const noexcept
  {
    return root_ == nullptr;
  }

  template < class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear() noexcept
  {
    delete_tree(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template < class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(Tree & other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(cmp_, other.cmp_);
    std::swap(size_, other.size_);
  }

  template < class Key, class Value, class Compare >
  TreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::find(const Key & key)
  {
    node_t * current = root_;
    while (current)
    {
      if (key == current->data.first)
      {
        return TreeIter< Key, Value, Compare >(current);
      }
      current = go_down(current, key);
    }
    return end();
  }

  template < class Key, class Value, class Compare >
  ConstTreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::find(const Key & key) const
  {
    node_t * current = root_;
    while (current)
    {
      if (key == current->data.first)
      {
        return ConstTreeIter< Key, Value, Compare >(current);
      }
      current = go_down(current, key);
    }
    return cend();
  }

  template < class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::count(const Key & key) const
  {
    auto result = find(key);
    if (!result.current_)
    {
      return 0;
    }
    return 1;
  }

  template < class Key, class Value, class Compare >
  std::pair< TreeIter< Key, Value, Compare >, TreeIter< Key, Value, Compare > >
  BSTree< Key, Value, Compare >::equal_range(const Key & key)
  {
    iterator result = find(key);
    if (!result.current_)
    {
      return std::pair< iterator, iterator >(end(), end());
    }
    iterator lesser;
    if (result == (*this).begin())
    {
      return std::pair< iterator, iterator >(end(), ++result);
    }
    lesser = result++;
    --lesser;
    return std::pair< iterator, iterator >(lesser, result);
  }

  template < class Key, class Value, class Compare >
  std::pair< ConstTreeIter< Key, Value, Compare >, ConstTreeIter< Key, Value, Compare > >
  BSTree< Key, Value, Compare >::equal_range(const Key & key) const
  {
    const_iterator result = find(key);
    if (!result.current_)
    {
      return std::pair< const_iterator, const_iterator >(cend(), cend());
    }
    const_iterator lesser;
    if (result == (*this).cbegin())
    {
      return std::pair< const_iterator, const_iterator >(cend(), ++result);
    }
    lesser = result++;
    --lesser;
    return std::pair< const_iterator, const_iterator >(lesser, result);
  }

  template < class Key, class Value, class Compare >
  TreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::lower_bound(const Key & key)
  {
    for (auto i = begin(); i != end(); ++i)
    {
      if (!cmp_((*i).first), key)
      {
        return i;
      }
    }
    return end();
  }

  template < class Key, class Value, class Compare >
  ConstTreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::lower_bound(const Key & key) const
  {
    for (auto i = cbegin(); i != cend(); ++i)
    {
      if (!cmp_((*i).first), key)
      {
        return i;
      }
    }
    return cend();
  }

  template < class Key, class Value, class Compare >
  TreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::upper_bound(const Key & key)
  {
    for (auto i = begin(); i != end(); ++i)
    {
      if (cmp_(key, (*i).first))
      {
        return i;
      }
    }
    return end();
  }

  template < class Key, class Value, class Compare >
  ConstTreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::upper_bound(const Key & key) const
  {
    for (auto i = cbegin(); i != cend(); ++i)
    {
      if (cmp_(key, (*i).first))
      {
        return i;
      }
    }
    return cend();
  }

  template < class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::at(const Key & key)
  {
    iterator value_iter = find(key);
    if (!value_iter.current_)
    {
      throw std::out_of_range("no element with such key");
    }
    return (*value_iter).second;
  }

  template < class Key, class Value, class Compare >
  const Value & BSTree< Key, Value, Compare >::at(const Key & key) const
  {
    const_iterator value_iter = find(key);
    if (!value_iter.current_)
    {
      throw std::out_of_range("no element with such key");
    }
    return (*value_iter).second;
  }

  template < class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::operator[](const Key & key)
  {
    auto result = insert_new(std::make_pair(key, Value()));
    return result.first->data.second;
  }

  template < class Key, class Value, class Compare >
  std::pair< TreeIter< Key, Value, Compare >, bool > BSTree< Key, Value, Compare >::insert(const value_t & value)
  {
    auto result = insert_new(value);
    return std::pair< iterator, bool >(iterator(result.first), result.second);
  }

  template < class Key, class Value, class Compare >
  template < class InputIt >
  void BSTree< Key, Value, Compare >::insert(InputIt first, InputIt last) //func input_range(first, last) s class InputIt
  {
    for (; first != last; ++first)
    {
      insert(*first);
    }
  }

  template < class Key, class Value, class Compare >
  TreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::erase(iterator pos)
  {
    node_t * trash = pos.current_;
    iterator next = ++pos;
    erase_el(trash);
    return next;
  }

  template < class Key, class Value, class Compare >
  ConstTreeIter< Key, Value, Compare >  BSTree< Key, Value, Compare >::erase(const_iterator pos)
  {
    node_t * trash = pos.current_;
    const_iterator next = ++pos;
    erase_el(trash);
    return next;
  }

  template < class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::erase(const Key & key)
  {
    auto trash = find(key);
    if (!trash.current_)
    {
      return 0;
    }
    erase_el(trash.current_);
    return 1;
  }

  template < class Key, class Value, class Compare >
  TreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::erase(iterator first, iterator last)
  {
    while (first != last)
    {
      first = erase(first);
    }
  }

  template < class Key, class Value, class Compare >
  TreeIter< Key, Value, Compare > BSTree< Key, Value, Compare >::erase(const_iterator first, const_iterator last)
  {
    while (first != last)
    {
      first = erase(first);
    }
  }

  template < class Key, class Value, class Compare >
  template < class F >
  F BSTree< Key, Value, Compare >::traverse_lnr(F f) const
  {
    if (!root_)
    {
      return f;
    }
    node_t * current = root_;
    std::stack< node_t * > s;
    while (current || !s.empty())
    {
      while (current)
      {
        s.push(current);
        current = current->left;
      }
      current = s.top();
      s.pop();
      f(current->data);
      current = current->right;
    }
    return f;
  }

  template < class Key, class Value, class Compare >
  template < class F >
  F BSTree< Key, Value, Compare >::traverse_rnl(F f) const
  {
    if (!root_)
    {
      return f;
    }
    node_t * current = root_;
    std::stack< node_t * > s;
    while (current || !s.empty())
    {
      while (current)
      {
        s.push(current);
        current = current->right;
      }
      current = s.top();
      s.pop();
      f(current->data);
      current = current->left;
    }
    return f;
  }

  template < class Key, class Value, class Compare >
  template < class F >
  F BSTree< Key, Value, Compare >::traverse_breadth(F f) const
  {
    if (!root_)
    {
      return f;
    }
    node_t * current = root_;
    std::queue< node_t * > q;
    q.push(root_);
    while (!q.empty())
    {
      current = q.front();
      q.pop();
      if (current)
      {
        q.push(current->left);
        q.push(current->right);
        f(current->data);
      }
    }
    return f;
  }

  template < class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::delete_tree(node_t * root) noexcept
  {
    if (root)
    {
      delete_tree(root->left);
      delete_tree(root->right);
      delete root;
    }
  }

  template < class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::erase_el(node_t * curr) // perepisat
  {
    if (curr->left && !curr->right)
    {
      if (curr->parent && curr->parent->left == curr)
      {
        curr->parent->left = curr->left;
        curr->left->parent = curr->parent;
      }
      else if (curr->parent && curr->parent->right == curr)
      {
        curr->parent->right = curr->left;
        curr->left->parent = curr->parent;
      }
      else
      {
        root_ = curr->left;
      }
    }
    else if (!curr->left && curr->right)
    {
      if (curr->parent && curr->parent->left == curr)
      {
        curr->parent->left = curr->right;
        curr->right->parent = curr->parent;
      }
      else if (curr->parent && curr->parent->right == curr)
      {
        curr->parent->right = curr->right;
        curr->right->parent = curr->parent;
      }
      else
      {
        root_ = curr->right;
      }
    }
    else if (curr->left && curr->right)
    {
      node_t * min_right = detail::traverse_left(curr->right);
      min_right->left = curr->left;
      curr->left->parent = min_right;
      if (curr->parent && curr->parent->left == curr)
      {
        curr->parent->left = curr->right;
        curr->right->parent = curr->parent;
      }
      else if (curr->parent && curr->parent->right == curr)
      {
        curr->parent->right = curr->right;
        curr->right->parent = curr->parent;
      }
      else
      {
        root_ = curr->right;
      }
    }
    delete curr;
  }

  template < class Key, class Value, class Compare >
  std::pair< detail::TreeNode< Key, Value > *, bool > BSTree< Key, Value, Compare >::insert_new(const value_t & value)
  {
    if (!root_)
    {
      root_ = new node_t(value.first, value.second, nullptr);
      ++size_;
      return std::pair< node_t *, bool >(root_, true);
    }
    node_t * current = root_;
    while (current)
    {
      if (value.first == current->data.first)
      {
        return std::pair< node_t *, bool >(current, false);
      }
      if (cmp_(value.first, current->data.first))
      {
        if (!current->left)
        {
          current->left = new node_t(value.first, value.second, current);
          current = current->left;
          break;
        }
        current = current->left;
      }
      else
      {
        if (!current->right)
        {
          current->right = new node_t(value.first, value.second, current);
          current = current->right;
          break;
        }
        current = current->right;
      }
    }
    ++size_;
    return std::pair< node_t *, bool >(current, true);
  }

  template < class Key, class Value, class Compare >
  detail::TreeNode< Key, Value > * BSTree< Key, Value, Compare >::go_down(node_t * current, const Key & key) const
  {
    if (cmp_(key, current->data.first))
    {
      current = current->left;
    }
    else
    {
      current = current->right;
    }
    return current;
  }
}

#endif
