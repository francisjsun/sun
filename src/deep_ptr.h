/* Copyright (C) 2020 Francis Sun, all rights reserved. */

#ifndef FS_SUN_DEEP_PTR_H
#define FS_SUN_DEEP_PTR_H

#include <memory>
#include <string>

#include "ns.h"
#include "string.h"

FS_SUN_NS_BEGIN

template <typename T>
class DeepPtr {
  friend void swap(DeepPtr &a, DeepPtr &b) {
    using std::swap;
    swap(a.ptr_, b.ptr_);
  }

  friend std::string to_string(const DeepPtr &ptr) {
    if (ptr != nullptr)
      return string::ToString(*ptr);
    else
      return "nullptr";
  }

  friend bool operator!=(const DeepPtr &rhs, std::nullptr_t) {
    return rhs.ptr_ != nullptr;
  }

  friend bool operator!=(std::nullptr_t, const DeepPtr &lhs) {
    return nullptr != lhs.ptr_;
  }

 public:
  template <typename... TArgs>
  DeepPtr(TArgs &&...args) : ptr_{new T{std::forward<TArgs>(args)...}} {}

 public:
  DeepPtr(const DeepPtr &ptr) : ptr_{ptr != nullptr ? new T(*ptr) : nullptr} {}

  DeepPtr(DeepPtr &&ptr) : ptr_{std::move(ptr.ptr_)} {}

  DeepPtr &operator=(DeepPtr rhs) {
    swap(*this, rhs);
    return *this;
  }

  const T &operator*() const { return *ptr_; }

  const std::unique_ptr<T> &operator->() const { return ptr_; }
  std::unique_ptr<T> &operator->() { return ptr_; }

 private:
  std::unique_ptr<T> ptr_{nullptr};
};

FS_SUN_NS_END

#endif  // FS_SUN_DEEP_PTR_H
