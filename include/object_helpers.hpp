#ifndef OBJECT_HELPERS_HPP
#define OBJECT_HELPERS_HPP

#include <new>
#include <cassert>

template<typename ElemT, typename ArgT>
inline void Construct(ElemT* elem, ArgT&& other) {
  assert(elem != nullptr);

  new (elem) ElemT(std::forward<ArgT>(other));
}

template<typename ElemT>
inline void DefaultConstruct(ElemT* elem) {
  assert(elem != nullptr);

  new (elem) ElemT();
}

template<typename ElemT>
inline void DefaultConstruct(ElemT* buffer, const size_t first, const size_t last) {
  assert(buffer != nullptr);

  for (size_t i = first; i < last; ++i) {
    DefaultConstruct(buffer + i);
  }
}

template<typename ElemT>
inline void DefaultConstruct(ElemT* buffer, const size_t size) {
  DefaultConstruct(buffer, 0, size);
}

template<typename ElemT>
inline void Destruct(ElemT* elem) {
  assert(elem != nullptr);

  elem->~ElemT();
}

template<typename ElemT>
inline void Destruct(ElemT* buffer, const size_t first, const size_t last) {
  assert(buffer != nullptr);

  for (size_t i = first; i < last; ++i) {
    Destruct(buffer + i);
  }
}

template<typename ElemT>
inline void Destruct(ElemT* buffer, const size_t size) {
  Destruct(buffer, 0, size);
}

template<typename ElemT>
inline void DestructAndDelete(ElemT* buffer, const size_t size) {
  Destruct(buffer, size);
  ::operator delete(buffer);
}

template<typename ElemT>
ElemT* SafeCopy(ElemT* src, const size_t dst_size, const size_t src_size) {
  assert(src_size <= dst_size);

  ElemT* dst = static_cast<ElemT*>(::operator new(dst_size * sizeof(ElemT)));
  size_t constructed = 0;
  try {
    for (size_t i = 0; i < src_size; ++i) {
      Construct(dst + i, src[i]);
      ++constructed;
    }
  } catch (...) {
    DestructAndDelete(dst, constructed);
    throw;
  }

  return dst;
}

template<typename ElemT>
ElemT* SafeMove(ElemT* src, const size_t dst_size, const size_t src_size) {
  assert(src_size <= dst_size);

  ElemT* dst = static_cast<ElemT*>(::operator new(dst_size * sizeof(ElemT)));
  size_t constructed = 0;
  try {
    for (size_t i = 0; i < src_size; ++i) {
      Construct(dst + i, std::move(src[i]));
      ++constructed;
    }
  } catch (...) {
    DestructAndDelete(dst, constructed);
    throw;
  }

  return dst;
}

#endif /* object_helpers.hpp */
