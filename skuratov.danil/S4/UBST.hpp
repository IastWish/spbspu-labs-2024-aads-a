#ifndef UBST_HPP
#define UBST_HPP

#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include "treeNode.hpp"
#include "constIteratorTree.hpp"

namespace skuratov
{
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class UBST
  {
  public:
    UBST():
      root_(nullptr),
      cmp_(Compare()),
      size_(0)
    {}

    ~UBST()
    {
      clear();
    }

    UBST(const UBST& diff):
      root_(nullptr),
      cmp_(diff.cmp_),
      size_(0)
    {
      try
      {
        for (auto it = diff.cbegin(); it != diff.cend(); it++)
        {
          insert(it->first, it->second);
        }
      }
      catch (...)
      {
        clear();
        throw;
      }
    }

    UBST& operator=(const UBST& diff)
    {
      if (this != std::addressof(diff))
      {
        UBST tempNode(diff);
        swap(tempNode);
      }
      return *this;
    }

    UBST(UBST&& diff) noexcept:
      root_(diff.root_),
      cmp_(diff.cmp_),
      size_(diff.size_)
    {
      diff.root_ = nullptr;
      diff.size_ = 0;
    }

    UBST& operator=(UBST&& diff) noexcept
    {
      if (this != std::addressof(diff))
      {
        clear();
        swap(diff);
      }
      return *this;
    }

    ConstIteratorTree< Key, Value, Compare > cbegin() const noexcept
    {
      detail::TreeNode< Key, Value >* temp = root_;
      if (!temp)
      {
        return ConstIteratorTree< Key, Value, Compare > (nullptr);
      }
      while (temp->left_ != nullptr)
      {
        temp = temp->left_;
      }
      return ConstIteratorTree< Key, Value, Compare > (temp);
    }

    ConstIteratorTree< Key, Value, Compare > cend() const noexcept
    {
      return ConstIteratorTree< Key, Value, Compare > (nullptr);
    }

    size_t size() const noexcept
    {
      return size_;
    }

    bool empty() const noexcept
    {
      return (size_ == 0);
    }

    Value& at(const Key& key)
    {
      detail::TreeNode< Key, Value >* nodePointer = findNode(root_, key);

      if (!nodePointer)
      {
        throw std::out_of_range("Key not found");
      }
      return nodePointer->data_.second;
    }

    const Value& at(const Key& key) const
    {
      detail::TreeNode< Key, Value >* nodePointer = findNode(root_, key);

      if (!nodePointer)
      {
        throw std::out_of_range("Key not found");
      }
      return nodePointer->data_.second;
    }

    Value& operator[](const Key& key)
    {
      detail::TreeNode< Key, Value >* nodePointer = findNode(root_, key);

      if (nodePointer == nullptr)
      {
        nodePointer = insertNode(root_, key, Value());
      }
      return nodePointer->data_.second;
    }

    const Value& operator[](const Key& key) const
    {
      detail::TreeNode< Key, Value >* nodePointer = findNode(root_, key);

      if (nodePointer == nullptr)
      {
        nodePointer = insertNode(root_, key, Value());
      }
      return nodePointer->data_.second;
    }

    void clear() noexcept
    {
      removeNode(root_);
      root_ = nullptr;
      size_ = 0;
    }

    void swap(UBST& diff)
    {
      std::swap(root_, diff.root_);
      std::swap(cmp_, diff.cmp_);
      std::swap(size_, diff.size_);
    }

    ConstIteratorTree< Key, Value, Compare > find(const Key& key) const
    {
      return ConstIteratorTree< Key, Value, Compare > (findNode(root_, key));
    }

    void push(const Key& key, const Value& value)
    {
      root_ = insertNode(root_, key, value);
      ++size_;
    }

    void insert(const Key& key, const Value& value)
    {
      try
      {
        if (root_)
        {
          insertNode(root_, key, value);
          ++size_;
        }
        else
        {
          root_ = new detail::TreeNode< Key, Value >(key, value);
          ++size_;
        }
      }
      catch (...)
      {
        clear();
        throw;
      }
    }

  private:
    detail::TreeNode< Key, Value >* root_;
    Compare cmp_;
    size_t size_;

    void removeNode(detail::TreeNode< Key, Value >* nodePointer)
    {
      if (nodePointer == nullptr)
      {
        return;
      }
      removeNode(nodePointer->left_);
      removeNode(nodePointer->right_);
      delete nodePointer;
    }

    detail::TreeNode< Key, Value >* findNode(detail::TreeNode< Key, Value >* nodePointer, const Key& key) const
    {
      if (!nodePointer)
      {
        return nullptr;
      }

      if (cmp_(key, nodePointer->data_.first))
      {
        return findNode(nodePointer->left_, key);
      }
      else if (cmp_(nodePointer->data_.first, key))
      {
        return findNode(nodePointer->right_, key);
      }
      else
      {
        return nodePointer;
      }
    }

    detail::TreeNode< Key, Value >* insertNode(detail::TreeNode< Key, Value >* nodePointer, const Key& key, const Value& value)
    {
      if (!nodePointer)
      {
        nodePointer = new detail::TreeNode< Key, Value >(key, value);
        if (!root_)
        {
          root_ = nodePointer;
        }
        size_++;
        return nodePointer;
      }

      if (cmp_(key, nodePointer->data_.first))
      {
        nodePointer->left_ = insertNode(nodePointer->left_, key, value);
        nodePointer->left_->parent_ = nodePointer;
      }
      else if (cmp_(nodePointer->data_.first, key))
      {
        nodePointer->right_ = insertNode(nodePointer->right_, key, value);
        nodePointer->right_->parent_ = nodePointer;
      }
      else
      {
        nodePointer->data_.second = value;
      }
      return nodePointer;
    }
  };
}

#endif
