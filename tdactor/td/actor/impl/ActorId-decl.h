//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2022
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/utils/ObjectPool.h"
#include "td/utils/Slice.h"

#include <type_traits>

namespace td {

class Actor;
class ActorInfo;

template <class ActorType = Actor>
class ActorId {
 public:
  using ActorT = ActorType;
  explicit ActorId(ObjectPool<ActorInfo>::WeakPtr ptr) : ptr_(ptr) {
  }
  ActorId() = default;
  ActorId(const ActorId &other) = default;
  ActorId &operator=(const ActorId &other) = default;
  ActorId(ActorId &&other) noexcept : ptr_(other.ptr_) {
    other.ptr_.clear();
  }
  ActorId &operator=(ActorId &&other) noexcept {
    if (&other == this) {
      return *this;
    }
    ptr_ = other.ptr_;
    other.clear();
    return *this;
  }
  ~ActorId() = default;

  bool empty() const {
    return ptr_.empty();
  }
  void clear() {
    ptr_.clear();
  }

  bool is_alive() const {
    return ptr_.is_alive_unsafe();
  }

  ActorInfo *get_actor_info() const;
  ActorType *get_actor_unsafe() const;

  // returns pointer to actor if it is on current thread. nullptr otherwise
  ActorType *try_get_actor() const;

  Slice get_name() const;

  template <class ToActorType, class = std::enable_if_t<std::is_base_of<ToActorType, ActorType>::value>>
  explicit operator ActorId<ToActorType>() const {
    return ActorId<ToActorType>(ptr_);
  }

  template <class AsActorType>
  ActorId<AsActorType> as() const {
    return ActorId<AsActorType>(ptr_);
  }

 private:
  ObjectPool<ActorInfo>::WeakPtr ptr_;
};

// threat ActorId as pointer and ActorOwn as
// unique_ptr<ActorId>
template <class ActorType = Actor>
class ActorOwn {
 public:
  using ActorT = ActorType;
  ActorOwn() = default;
  explicit ActorOwn(ActorId<ActorType> id);
  template <class OtherActorType>
  explicit ActorOwn(ActorId<OtherActorType> id);
  template <class OtherActorType>
  explicit ActorOwn(ActorOwn<OtherActorType> &&other);
  template <class OtherActorType>
  ActorOwn &operator=(ActorOwn<OtherActorType> &&other);
  ActorOwn(ActorOwn &&other) noexcept;
  ActorOwn &operator=(ActorOwn &&other) noexcept;
//  ActorOwn(const ActorOwn &other) = delete;
  ActorOwn &operator=(const ActorOwn &other) = delete;
  ~ActorOwn();

  bool empty() const;
  bool is_alive() const {
    return id_.is_alive();
  }
  ActorId<ActorType> get() const;
  ActorId<ActorType> release();
  void reset(ActorId<ActorType> other = ActorId<ActorType>());
  void hangup() const;
  const ActorId<ActorType> *operator->() const;

  using ActorIdConstRef = const ActorId<ActorType> &;
  // operator ActorIdConstRef();

 private:
  ActorId<ActorType> id_;
};

template <class ActorType = Actor>
class ActorShared {
 public:
  using ActorT = ActorType;
  ActorShared() = default;
  template <class OtherActorType>
  ActorShared(ActorId<OtherActorType> id, uint64 token);
  template <class OtherActorType>
  ActorShared(ActorShared<OtherActorType> &&other);
  template <class OtherActorType>
  ActorShared(ActorOwn<OtherActorType> &&other);
  template <class OtherActorType>
  ActorShared &operator=(ActorShared<OtherActorType> &&other);
  ActorShared(ActorShared &&other) noexcept;
  ActorShared &operator=(ActorShared &&other) noexcept;
  ActorShared(const ActorShared &other) = delete;
  ActorShared &operator=(const ActorShared &other) = delete;
  ~ActorShared();

  uint64 token() const;
  bool empty() const;
  bool is_alive() const {
    return id_.is_alive();
  }
  ActorId<ActorType> get() const;
  ActorId<ActorType> release();
  void reset(ActorId<ActorType> other = ActorId<ActorType>());
  template <class OtherActorType>
  void reset(ActorId<OtherActorType> other);
  const ActorId<ActorType> *operator->() const;

 private:
  ActorId<ActorType> id_;
  uint64 token_ = 0;
};

class ActorRef {
 public:
  ActorRef() = default;
  template <class T>
  ActorRef(const ActorId<T> &actor_id);
  template <class T>
  ActorRef(ActorId<T> &&actor_id);
  template <class T>
  ActorRef(const ActorShared<T> &actor_id);
  template <class T>
  ActorRef(ActorShared<T> &&actor_id);
  template <class T>
  ActorRef(const ActorOwn<T> &actor_id);
  template <class T>
  ActorRef(ActorOwn<T> &&actor_id);
  ActorId<> get() const {
    return actor_id_;
  }
  uint64 token() const {
    return token_;
  }

 private:
  ActorId<> actor_id_;
  uint64 token_ = 0;
};

}  // namespace td
