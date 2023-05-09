#include "include/Game.hpp"
#include "include/mayama.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <new>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace rust {
inline namespace cxxbridge1 {
// #include "rust/cxx.h"

#ifndef CXXBRIDGE1_PANIC
#define CXXBRIDGE1_PANIC
template <typename Exception> void panic [[noreturn]] (const char *msg);
#endif // CXXBRIDGE1_PANIC

struct unsafe_bitcopy_t;

namespace {
template <typename T> class impl;
} // namespace

class Opaque;

template <typename T>::std::size_t size_of();
template <typename T>::std::size_t align_of();

#ifndef CXXBRIDGE1_RUST_STRING
#define CXXBRIDGE1_RUST_STRING
class String final {
public:
  String() noexcept;
  String(const String &) noexcept;
  String(String &&) noexcept;
  ~String() noexcept;

  String(const std::string &);
  String(const char *);
  String(const char *, std::size_t);
  String(const char16_t *);
  String(const char16_t *, std::size_t);

  static String lossy(const std::string &) noexcept;
  static String lossy(const char *) noexcept;
  static String lossy(const char *, std::size_t) noexcept;
  static String lossy(const char16_t *) noexcept;
  static String lossy(const char16_t *, std::size_t) noexcept;

  String &operator=(const String &) &noexcept;
  String &operator=(String &&) &noexcept;

  explicit operator std::string() const;

  const char *data() const noexcept;
  std::size_t size() const noexcept;
  std::size_t length() const noexcept;
  bool empty() const noexcept;

  const char *c_str() noexcept;

  std::size_t capacity() const noexcept;
  void reserve(size_t new_cap) noexcept;

  using iterator = char *;
  iterator begin() noexcept;
  iterator end() noexcept;

  using const_iterator = const char *;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  bool operator==(const String &) const noexcept;
  bool operator!=(const String &) const noexcept;
  bool operator<(const String &) const noexcept;
  bool operator<=(const String &) const noexcept;
  bool operator>(const String &) const noexcept;
  bool operator>=(const String &) const noexcept;

  void swap(String &) noexcept;

  String(unsafe_bitcopy_t, const String &) noexcept;

private:
  struct lossy_t;
  String(lossy_t, const char *, std::size_t) noexcept;
  String(lossy_t, const char16_t *, std::size_t) noexcept;
  friend void swap(String &lhs, String &rhs) noexcept { lhs.swap(rhs); }

  std::array<std::uintptr_t, 3> repr;
};
#endif // CXXBRIDGE1_RUST_STRING

#ifndef CXXBRIDGE1_RUST_SLICE
#define CXXBRIDGE1_RUST_SLICE
namespace detail {
template <bool> struct copy_assignable_if {};

template <> struct copy_assignable_if<false> {
  copy_assignable_if() noexcept = default;
  copy_assignable_if(const copy_assignable_if &) noexcept = default;
  copy_assignable_if &operator=(const copy_assignable_if &) &noexcept = delete;
  copy_assignable_if &operator=(copy_assignable_if &&) &noexcept = default;
};
} // namespace detail

template <typename T>
class Slice final
    : private detail::copy_assignable_if<std::is_const<T>::value> {
public:
  using value_type = T;

  Slice() noexcept;
  Slice(T *, std::size_t count) noexcept;

  Slice &operator=(const Slice<T> &) &noexcept = default;
  Slice &operator=(Slice<T> &&) &noexcept = default;

  T *data() const noexcept;
  std::size_t size() const noexcept;
  std::size_t length() const noexcept;
  bool empty() const noexcept;

  T &operator[](std::size_t n) const noexcept;
  T &at(std::size_t n) const;
  T &front() const noexcept;
  T &back() const noexcept;

  Slice(const Slice<T> &) noexcept = default;
  ~Slice() noexcept = default;

  class iterator;
  iterator begin() const noexcept;
  iterator end() const noexcept;

  void swap(Slice &) noexcept;

private:
  class uninit;
  Slice(uninit) noexcept;
  friend impl<Slice>;
  friend void sliceInit(void *, const void *, std::size_t) noexcept;
  friend void *slicePtr(const void *) noexcept;
  friend std::size_t sliceLen(const void *) noexcept;

  std::array<std::uintptr_t, 2> repr;
};

template <typename T> class Slice<T>::iterator final {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = typename std::add_pointer<T>::type;
  using reference = typename std::add_lvalue_reference<T>::type;

  reference operator*() const noexcept;
  pointer operator->() const noexcept;
  reference operator[](difference_type) const noexcept;

  iterator &operator++() noexcept;
  iterator operator++(int) noexcept;
  iterator &operator--() noexcept;
  iterator operator--(int) noexcept;

  iterator &operator+=(difference_type) noexcept;
  iterator &operator-=(difference_type) noexcept;
  iterator operator+(difference_type) const noexcept;
  iterator operator-(difference_type) const noexcept;
  difference_type operator-(const iterator &) const noexcept;

  bool operator==(const iterator &) const noexcept;
  bool operator!=(const iterator &) const noexcept;
  bool operator<(const iterator &) const noexcept;
  bool operator<=(const iterator &) const noexcept;
  bool operator>(const iterator &) const noexcept;
  bool operator>=(const iterator &) const noexcept;

private:
  friend class Slice;
  void *pos;
  std::size_t stride;
};

template <typename T> Slice<T>::Slice() noexcept {
  sliceInit(this, reinterpret_cast<void *>(align_of<T>()), 0);
}

template <typename T> Slice<T>::Slice(T *s, std::size_t count) noexcept {
  assert(s != nullptr || count == 0);
  sliceInit(this,
            s == nullptr && count == 0
                ? reinterpret_cast<void *>(align_of<T>())
                : const_cast<typename std::remove_const<T>::type *>(s),
            count);
}

template <typename T> T *Slice<T>::data() const noexcept {
  return reinterpret_cast<T *>(slicePtr(this));
}

template <typename T> std::size_t Slice<T>::size() const noexcept {
  return sliceLen(this);
}

template <typename T> std::size_t Slice<T>::length() const noexcept {
  return this->size();
}

template <typename T> bool Slice<T>::empty() const noexcept {
  return this->size() == 0;
}

template <typename T> T &Slice<T>::operator[](std::size_t n) const noexcept {
  assert(n < this->size());
  auto ptr = static_cast<char *>(slicePtr(this)) + size_of<T>() * n;
  return *reinterpret_cast<T *>(ptr);
}

template <typename T> T &Slice<T>::at(std::size_t n) const {
  if (n >= this->size()) {
    panic<std::out_of_range>("rust::Slice index out of range");
  }
  return (*this)[n];
}

template <typename T> T &Slice<T>::front() const noexcept {
  assert(!this->empty());
  return (*this)[0];
}

template <typename T> T &Slice<T>::back() const noexcept {
  assert(!this->empty());
  return (*this)[this->size() - 1];
}

template <typename T>
typename Slice<T>::iterator::reference
Slice<T>::iterator::operator*() const noexcept {
  return *static_cast<T *>(this->pos);
}

template <typename T>
typename Slice<T>::iterator::pointer
Slice<T>::iterator::operator->() const noexcept {
  return static_cast<T *>(this->pos);
}

template <typename T>
typename Slice<T>::iterator::reference Slice<T>::iterator::operator[](
    typename Slice<T>::iterator::difference_type n) const noexcept {
  auto ptr = static_cast<char *>(this->pos) + this->stride * n;
  return *reinterpret_cast<T *>(ptr);
}

template <typename T>
typename Slice<T>::iterator &Slice<T>::iterator::operator++() noexcept {
  this->pos = static_cast<char *>(this->pos) + this->stride;
  return *this;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::iterator::operator++(int) noexcept {
  auto ret = iterator(*this);
  this->pos = static_cast<char *>(this->pos) + this->stride;
  return ret;
}

template <typename T>
typename Slice<T>::iterator &Slice<T>::iterator::operator--() noexcept {
  this->pos = static_cast<char *>(this->pos) - this->stride;
  return *this;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::iterator::operator--(int) noexcept {
  auto ret = iterator(*this);
  this->pos = static_cast<char *>(this->pos) - this->stride;
  return ret;
}

template <typename T>
typename Slice<T>::iterator &Slice<T>::iterator::operator+=(
    typename Slice<T>::iterator::difference_type n) noexcept {
  this->pos = static_cast<char *>(this->pos) + this->stride * n;
  return *this;
}

template <typename T>
typename Slice<T>::iterator &Slice<T>::iterator::operator-=(
    typename Slice<T>::iterator::difference_type n) noexcept {
  this->pos = static_cast<char *>(this->pos) - this->stride * n;
  return *this;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::iterator::operator+(
    typename Slice<T>::iterator::difference_type n) const noexcept {
  auto ret = iterator(*this);
  ret.pos = static_cast<char *>(this->pos) + this->stride * n;
  return ret;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::iterator::operator-(
    typename Slice<T>::iterator::difference_type n) const noexcept {
  auto ret = iterator(*this);
  ret.pos = static_cast<char *>(this->pos) - this->stride * n;
  return ret;
}

template <typename T>
typename Slice<T>::iterator::difference_type
Slice<T>::iterator::operator-(const iterator &other) const noexcept {
  auto diff = std::distance(static_cast<char *>(other.pos),
                            static_cast<char *>(this->pos));
  return diff / this->stride;
}

template <typename T>
bool Slice<T>::iterator::operator==(const iterator &other) const noexcept {
  return this->pos == other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator!=(const iterator &other) const noexcept {
  return this->pos != other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator<(const iterator &other) const noexcept {
  return this->pos < other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator<=(const iterator &other) const noexcept {
  return this->pos <= other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator>(const iterator &other) const noexcept {
  return this->pos > other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator>=(const iterator &other) const noexcept {
  return this->pos >= other.pos;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::begin() const noexcept {
  iterator it;
  it.pos = slicePtr(this);
  it.stride = size_of<T>();
  return it;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::end() const noexcept {
  iterator it = this->begin();
  it.pos = static_cast<char *>(it.pos) + it.stride * this->size();
  return it;
}

template <typename T> void Slice<T>::swap(Slice &rhs) noexcept {
  std::swap(*this, rhs);
}
#endif // CXXBRIDGE1_RUST_SLICE

#ifndef CXXBRIDGE1_RUST_BITCOPY_T
#define CXXBRIDGE1_RUST_BITCOPY_T
struct unsafe_bitcopy_t final {
  explicit unsafe_bitcopy_t() = default;
};
#endif // CXXBRIDGE1_RUST_BITCOPY_T

#ifndef CXXBRIDGE1_RUST_VEC
#define CXXBRIDGE1_RUST_VEC
template <typename T> class Vec final {
public:
  using value_type = T;

  Vec() noexcept;
  Vec(std::initializer_list<T>);
  Vec(const Vec &);
  Vec(Vec &&) noexcept;
  ~Vec() noexcept;

  Vec &operator=(Vec &&) &noexcept;
  Vec &operator=(const Vec &) &;

  std::size_t size() const noexcept;
  bool empty() const noexcept;
  const T *data() const noexcept;
  T *data() noexcept;
  std::size_t capacity() const noexcept;

  const T &operator[](std::size_t n) const noexcept;
  const T &at(std::size_t n) const;
  const T &front() const noexcept;
  const T &back() const noexcept;

  T &operator[](std::size_t n) noexcept;
  T &at(std::size_t n);
  T &front() noexcept;
  T &back() noexcept;

  void reserve(std::size_t new_cap);
  void push_back(const T &value);
  void push_back(T &&value);
  template <typename... Args> void emplace_back(Args &&...args);
  void truncate(std::size_t len);
  void clear();

  using iterator = typename Slice<T>::iterator;
  iterator begin() noexcept;
  iterator end() noexcept;

  using const_iterator = typename Slice<const T>::iterator;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  void swap(Vec &) noexcept;

  Vec(unsafe_bitcopy_t, const Vec &) noexcept;

private:
  void reserve_total(std::size_t new_cap) noexcept;
  void set_len(std::size_t len) noexcept;
  void drop() noexcept;

  friend void swap(Vec &lhs, Vec &rhs) noexcept { lhs.swap(rhs); }

  std::array<std::uintptr_t, 3> repr;
};

template <typename T> Vec<T>::Vec(std::initializer_list<T> init) : Vec{} {
  this->reserve_total(init.size());
  std::move(init.begin(), init.end(), std::back_inserter(*this));
}

template <typename T> Vec<T>::Vec(const Vec &other) : Vec() {
  this->reserve_total(other.size());
  std::copy(other.begin(), other.end(), std::back_inserter(*this));
}

template <typename T> Vec<T>::Vec(Vec &&other) noexcept : repr(other.repr) {
  new (&other) Vec();
}

template <typename T> Vec<T>::~Vec() noexcept { this->drop(); }

template <typename T> Vec<T> &Vec<T>::operator=(Vec &&other) &noexcept {
  this->drop();
  this->repr = other.repr;
  new (&other) Vec();
  return *this;
}

template <typename T> Vec<T> &Vec<T>::operator=(const Vec &other) & {
  if (this != &other) {
    this->drop();
    new (this) Vec(other);
  }
  return *this;
}

template <typename T> bool Vec<T>::empty() const noexcept {
  return this->size() == 0;
}

template <typename T> T *Vec<T>::data() noexcept {
  return const_cast<T *>(const_cast<const Vec<T> *>(this)->data());
}

template <typename T>
const T &Vec<T>::operator[](std::size_t n) const noexcept {
  assert(n < this->size());
  auto data = reinterpret_cast<const char *>(this->data());
  return *reinterpret_cast<const T *>(data + n * size_of<T>());
}

template <typename T> const T &Vec<T>::at(std::size_t n) const {
  if (n >= this->size()) {
    panic<std::out_of_range>("rust::Vec index out of range");
  }
  return (*this)[n];
}

template <typename T> const T &Vec<T>::front() const noexcept {
  assert(!this->empty());
  return (*this)[0];
}

template <typename T> const T &Vec<T>::back() const noexcept {
  assert(!this->empty());
  return (*this)[this->size() - 1];
}

template <typename T> T &Vec<T>::operator[](std::size_t n) noexcept {
  assert(n < this->size());
  auto data = reinterpret_cast<char *>(this->data());
  return *reinterpret_cast<T *>(data + n * size_of<T>());
}

template <typename T> T &Vec<T>::at(std::size_t n) {
  if (n >= this->size()) {
    panic<std::out_of_range>("rust::Vec index out of range");
  }
  return (*this)[n];
}

template <typename T> T &Vec<T>::front() noexcept {
  assert(!this->empty());
  return (*this)[0];
}

template <typename T> T &Vec<T>::back() noexcept {
  assert(!this->empty());
  return (*this)[this->size() - 1];
}

template <typename T> void Vec<T>::reserve(std::size_t new_cap) {
  this->reserve_total(new_cap);
}

template <typename T> void Vec<T>::push_back(const T &value) {
  this->emplace_back(value);
}

template <typename T> void Vec<T>::push_back(T &&value) {
  this->emplace_back(std::move(value));
}

template <typename T>
template <typename... Args>
void Vec<T>::emplace_back(Args &&...args) {
  auto size = this->size();
  this->reserve_total(size + 1);
  ::new (reinterpret_cast<T *>(reinterpret_cast<char *>(this->data()) +
                               size * size_of<T>()))
      T(std::forward<Args>(args)...);
  this->set_len(size + 1);
}

template <typename T> void Vec<T>::clear() { this->truncate(0); }

template <typename T> typename Vec<T>::iterator Vec<T>::begin() noexcept {
  return Slice<T>(this->data(), this->size()).begin();
}

template <typename T> typename Vec<T>::iterator Vec<T>::end() noexcept {
  return Slice<T>(this->data(), this->size()).end();
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::begin() const noexcept {
  return this->cbegin();
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::end() const noexcept {
  return this->cend();
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::cbegin() const noexcept {
  return Slice<const T>(this->data(), this->size()).begin();
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::cend() const noexcept {
  return Slice<const T>(this->data(), this->size()).end();
}

template <typename T> void Vec<T>::swap(Vec &rhs) noexcept {
  using std::swap;
  swap(this->repr, rhs.repr);
}

template <typename T>
Vec<T>::Vec(unsafe_bitcopy_t, const Vec &bits) noexcept : repr(bits.repr) {}
#endif // CXXBRIDGE1_RUST_VEC

#ifndef CXXBRIDGE1_IS_COMPLETE
#define CXXBRIDGE1_IS_COMPLETE
namespace detail {
namespace {
template <typename T, typename = std::size_t>
struct is_complete : std::false_type {};
template <typename T>
struct is_complete<T, decltype(sizeof(T))> : std::true_type {};
} // namespace
} // namespace detail
#endif // CXXBRIDGE1_IS_COMPLETE

#ifndef CXXBRIDGE1_LAYOUT
#define CXXBRIDGE1_LAYOUT
class layout {
  template <typename T> friend std::size_t size_of();
  template <typename T> friend std::size_t align_of();
  template <typename T>
  static typename std::enable_if<std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_size_of() {
    return T::layout::size();
  }
  template <typename T>
  static typename std::enable_if<!std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_size_of() {
    return sizeof(T);
  }
  template <typename T>
  static
      typename std::enable_if<detail::is_complete<T>::value, std::size_t>::type
      size_of() {
    return do_size_of<T>();
  }
  template <typename T>
  static typename std::enable_if<std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_align_of() {
    return T::layout::align();
  }
  template <typename T>
  static typename std::enable_if<!std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_align_of() {
    return alignof(T);
  }
  template <typename T>
  static
      typename std::enable_if<detail::is_complete<T>::value, std::size_t>::type
      align_of() {
    return do_align_of<T>();
  }
};

template <typename T> std::size_t size_of() { return layout::size_of<T>(); }

template <typename T> std::size_t align_of() { return layout::align_of<T>(); }
#endif // CXXBRIDGE1_LAYOUT

namespace detail {
template <typename T, typename = void *> struct operator_new {
  void *operator()(::std::size_t sz) { return ::operator new(sz); }
};

template <typename T>
struct operator_new<T, decltype(T::operator new(sizeof(T)))> {
  void *operator()(::std::size_t sz) { return T::operator new(sz); }
};
} // namespace detail

template <typename T> union MaybeUninit {
  T value;
  void *operator new(::std::size_t sz) { return detail::operator_new<T>{}(sz); }
  MaybeUninit() {}
  ~MaybeUninit() {}
};
} // namespace cxxbridge1
} // namespace rust

struct Keyword;
struct Tile;
struct Tiles;
struct PreTile;
struct PreLevel;
struct KeywordGroup;
struct Theme;
struct TileDiff;
struct ClickTileResult;
enum class GameEvent : ::std::uint8_t;
enum class GameStatus : ::std::uint8_t;
using Game = ::Game;

#ifndef CXXBRIDGE1_STRUCT_Keyword
#define CXXBRIDGE1_STRUCT_Keyword
struct Keyword final {
  ::rust::String content;
  ::rust::String type;
  ::rust::String color;
  ::rust::String background_color;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_Keyword

#ifndef CXXBRIDGE1_STRUCT_Tile
#define CXXBRIDGE1_STRUCT_Tile
struct Tile final {
  ::rust::String id;
  ::std::int32_t row;
  ::std::int32_t column;
  ::std::int32_t index;
  bool exposed;
  ::Keyword keyword;
  bool on_buffer;
  bool dropped;
  bool selected;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_Tile

#ifndef CXXBRIDGE1_STRUCT_Tiles
#define CXXBRIDGE1_STRUCT_Tiles
struct Tiles final {
  ::rust::Vec<::Tile> value;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_Tiles

#ifndef CXXBRIDGE1_STRUCT_PreTile
#define CXXBRIDGE1_STRUCT_PreTile
struct PreTile final {
  ::std::int32_t column;
  ::std::int32_t row;
  ::std::int32_t index;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_PreTile

#ifndef CXXBRIDGE1_STRUCT_PreLevel
#define CXXBRIDGE1_STRUCT_PreLevel
struct PreLevel final {
  ::rust::String id;
  ::rust::String name;
  ::rust::Vec<::PreTile> tiles;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_PreLevel

#ifndef CXXBRIDGE1_STRUCT_KeywordGroup
#define CXXBRIDGE1_STRUCT_KeywordGroup
struct KeywordGroup final {
  ::rust::Vec<::rust::String> contents;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_KeywordGroup

#ifndef CXXBRIDGE1_STRUCT_Theme
#define CXXBRIDGE1_STRUCT_Theme
struct Theme final {
  ::rust::String id;
  ::rust::String name;
  ::rust::Vec<::Keyword> keywords;
  ::rust::Vec<::KeywordGroup> groups;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_Theme

#ifndef CXXBRIDGE1_STRUCT_TileDiff
#define CXXBRIDGE1_STRUCT_TileDiff
struct TileDiff final {
  ::rust::String id;
  ::rust::Vec<::rust::String> diff;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_TileDiff

#ifndef CXXBRIDGE1_STRUCT_ClickTileResult
#define CXXBRIDGE1_STRUCT_ClickTileResult
struct ClickTileResult final {
  ::rust::Vec<::TileDiff> diffs;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_ClickTileResult

#ifndef CXXBRIDGE1_ENUM_GameEvent
#define CXXBRIDGE1_ENUM_GameEvent
enum class GameEvent : ::std::uint8_t {
  ClickTile = 0,
};
#endif // CXXBRIDGE1_ENUM_GameEvent

#ifndef CXXBRIDGE1_ENUM_GameStatus
#define CXXBRIDGE1_ENUM_GameStatus
enum class GameStatus : ::std::uint8_t {
  Running = 0,
  Winned = 1,
  Losed = 2,
};
#endif // CXXBRIDGE1_ENUM_GameStatus

extern "C" {
::GameStatus cxxbridge1$get_status() noexcept {
  ::GameStatus (*get_status$)() = ::get_status;
  return get_status$();
}

void cxxbridge1$load_tiles(::PreLevel const &level) noexcept {
  void (*load_tiles$)(::PreLevel const &) = ::load_tiles;
  load_tiles$(level);
}

void cxxbridge1$load_theme(::Theme const &theme) noexcept {
  void (*load_theme$)(::Theme const &) = ::load_theme;
  load_theme$(theme);
}

void cxxbridge1$get_tiles(::Tiles *return$) noexcept {
  ::Tiles (*get_tiles$)() = ::get_tiles;
  new (return$)::Tiles(get_tiles$());
}

void cxxbridge1$init_game() noexcept {
  void (*init_game$)() = ::init_game;
  init_game$();
}

::ClickTileResult *cxxbridge1$handle_click_tile(::Tile const &tile) noexcept {
  ::std::unique_ptr<::ClickTileResult> (*handle_click_tile$)(::Tile const &) =
      ::handle_click_tile;
  return handle_click_tile$(tile).release();
}

::std::int32_t cxxbridge1$get_score() noexcept {
  ::std::int32_t (*get_score$)() = ::get_score;
  return get_score$();
}

void cxxbridge1$rust_vec$Tile$new(::rust::Vec<::Tile> const *ptr) noexcept;
void cxxbridge1$rust_vec$Tile$drop(::rust::Vec<::Tile> *ptr) noexcept;
::std::size_t
cxxbridge1$rust_vec$Tile$len(::rust::Vec<::Tile> const *ptr) noexcept;
::std::size_t
cxxbridge1$rust_vec$Tile$capacity(::rust::Vec<::Tile> const *ptr) noexcept;
::Tile const *
cxxbridge1$rust_vec$Tile$data(::rust::Vec<::Tile> const *ptr) noexcept;
void cxxbridge1$rust_vec$Tile$reserve_total(::rust::Vec<::Tile> *ptr,
                                            ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$Tile$set_len(::rust::Vec<::Tile> *ptr,
                                      ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$Tile$truncate(::rust::Vec<::Tile> *ptr,
                                       ::std::size_t len) noexcept;

void cxxbridge1$rust_vec$PreTile$new(
    ::rust::Vec<::PreTile> const *ptr) noexcept;
void cxxbridge1$rust_vec$PreTile$drop(::rust::Vec<::PreTile> *ptr) noexcept;
::std::size_t
cxxbridge1$rust_vec$PreTile$len(::rust::Vec<::PreTile> const *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$PreTile$capacity(
    ::rust::Vec<::PreTile> const *ptr) noexcept;
::PreTile const *
cxxbridge1$rust_vec$PreTile$data(::rust::Vec<::PreTile> const *ptr) noexcept;
void cxxbridge1$rust_vec$PreTile$reserve_total(::rust::Vec<::PreTile> *ptr,
                                               ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$PreTile$set_len(::rust::Vec<::PreTile> *ptr,
                                         ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$PreTile$truncate(::rust::Vec<::PreTile> *ptr,
                                          ::std::size_t len) noexcept;

void cxxbridge1$rust_vec$Keyword$new(
    ::rust::Vec<::Keyword> const *ptr) noexcept;
void cxxbridge1$rust_vec$Keyword$drop(::rust::Vec<::Keyword> *ptr) noexcept;
::std::size_t
cxxbridge1$rust_vec$Keyword$len(::rust::Vec<::Keyword> const *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$Keyword$capacity(
    ::rust::Vec<::Keyword> const *ptr) noexcept;
::Keyword const *
cxxbridge1$rust_vec$Keyword$data(::rust::Vec<::Keyword> const *ptr) noexcept;
void cxxbridge1$rust_vec$Keyword$reserve_total(::rust::Vec<::Keyword> *ptr,
                                               ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$Keyword$set_len(::rust::Vec<::Keyword> *ptr,
                                         ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$Keyword$truncate(::rust::Vec<::Keyword> *ptr,
                                          ::std::size_t len) noexcept;

void cxxbridge1$rust_vec$KeywordGroup$new(
    ::rust::Vec<::KeywordGroup> const *ptr) noexcept;
void cxxbridge1$rust_vec$KeywordGroup$drop(
    ::rust::Vec<::KeywordGroup> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$KeywordGroup$len(
    ::rust::Vec<::KeywordGroup> const *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$KeywordGroup$capacity(
    ::rust::Vec<::KeywordGroup> const *ptr) noexcept;
::KeywordGroup const *cxxbridge1$rust_vec$KeywordGroup$data(
    ::rust::Vec<::KeywordGroup> const *ptr) noexcept;
void cxxbridge1$rust_vec$KeywordGroup$reserve_total(
    ::rust::Vec<::KeywordGroup> *ptr, ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$KeywordGroup$set_len(::rust::Vec<::KeywordGroup> *ptr,
                                              ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$KeywordGroup$truncate(::rust::Vec<::KeywordGroup> *ptr,
                                               ::std::size_t len) noexcept;

void cxxbridge1$rust_vec$TileDiff$new(
    ::rust::Vec<::TileDiff> const *ptr) noexcept;
void cxxbridge1$rust_vec$TileDiff$drop(::rust::Vec<::TileDiff> *ptr) noexcept;
::std::size_t
cxxbridge1$rust_vec$TileDiff$len(::rust::Vec<::TileDiff> const *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$TileDiff$capacity(
    ::rust::Vec<::TileDiff> const *ptr) noexcept;
::TileDiff const *
cxxbridge1$rust_vec$TileDiff$data(::rust::Vec<::TileDiff> const *ptr) noexcept;
void cxxbridge1$rust_vec$TileDiff$reserve_total(::rust::Vec<::TileDiff> *ptr,
                                                ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$TileDiff$set_len(::rust::Vec<::TileDiff> *ptr,
                                          ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$TileDiff$truncate(::rust::Vec<::TileDiff> *ptr,
                                           ::std::size_t len) noexcept;

static_assert(sizeof(::std::unique_ptr<::ClickTileResult>) == sizeof(void *),
              "");
static_assert(alignof(::std::unique_ptr<::ClickTileResult>) == alignof(void *),
              "");
void cxxbridge1$unique_ptr$ClickTileResult$null(
    ::std::unique_ptr<::ClickTileResult> *ptr) noexcept {
  ::new (ptr)::std::unique_ptr<::ClickTileResult>();
}
::ClickTileResult *cxxbridge1$unique_ptr$ClickTileResult$uninit(
    ::std::unique_ptr<::ClickTileResult> *ptr) noexcept {
  ::ClickTileResult *uninit = reinterpret_cast<::ClickTileResult *>(
      new ::rust::MaybeUninit<::ClickTileResult>);
  ::new (ptr)::std::unique_ptr<::ClickTileResult>(uninit);
  return uninit;
}
void cxxbridge1$unique_ptr$ClickTileResult$raw(
    ::std::unique_ptr<::ClickTileResult> *ptr,
    ::ClickTileResult *raw) noexcept {
  ::new (ptr)::std::unique_ptr<::ClickTileResult>(raw);
}
::ClickTileResult const *cxxbridge1$unique_ptr$ClickTileResult$get(
    ::std::unique_ptr<::ClickTileResult> const &ptr) noexcept {
  return ptr.get();
}
::ClickTileResult *cxxbridge1$unique_ptr$ClickTileResult$release(
    ::std::unique_ptr<::ClickTileResult> &ptr) noexcept {
  return ptr.release();
}
void cxxbridge1$unique_ptr$ClickTileResult$drop(
    ::std::unique_ptr<::ClickTileResult> *ptr) noexcept {
  ptr->~unique_ptr();
}
} // extern "C"

namespace rust {
inline namespace cxxbridge1 {
template <> Vec<::Tile>::Vec() noexcept { cxxbridge1$rust_vec$Tile$new(this); }
template <> void Vec<::Tile>::drop() noexcept {
  return cxxbridge1$rust_vec$Tile$drop(this);
}
template <>::std::size_t Vec<::Tile>::size() const noexcept {
  return cxxbridge1$rust_vec$Tile$len(this);
}
template <>::std::size_t Vec<::Tile>::capacity() const noexcept {
  return cxxbridge1$rust_vec$Tile$capacity(this);
}
template <>::Tile const *Vec<::Tile>::data() const noexcept {
  return cxxbridge1$rust_vec$Tile$data(this);
}
template <> void Vec<::Tile>::reserve_total(::std::size_t new_cap) noexcept {
  return cxxbridge1$rust_vec$Tile$reserve_total(this, new_cap);
}
template <> void Vec<::Tile>::set_len(::std::size_t len) noexcept {
  return cxxbridge1$rust_vec$Tile$set_len(this, len);
}
template <> void Vec<::Tile>::truncate(::std::size_t len) {
  return cxxbridge1$rust_vec$Tile$truncate(this, len);
}
template <> Vec<::PreTile>::Vec() noexcept {
  cxxbridge1$rust_vec$PreTile$new(this);
}
template <> void Vec<::PreTile>::drop() noexcept {
  return cxxbridge1$rust_vec$PreTile$drop(this);
}
template <>::std::size_t Vec<::PreTile>::size() const noexcept {
  return cxxbridge1$rust_vec$PreTile$len(this);
}
template <>::std::size_t Vec<::PreTile>::capacity() const noexcept {
  return cxxbridge1$rust_vec$PreTile$capacity(this);
}
template <>::PreTile const *Vec<::PreTile>::data() const noexcept {
  return cxxbridge1$rust_vec$PreTile$data(this);
}
template <> void Vec<::PreTile>::reserve_total(::std::size_t new_cap) noexcept {
  return cxxbridge1$rust_vec$PreTile$reserve_total(this, new_cap);
}
template <> void Vec<::PreTile>::set_len(::std::size_t len) noexcept {
  return cxxbridge1$rust_vec$PreTile$set_len(this, len);
}
template <> void Vec<::PreTile>::truncate(::std::size_t len) {
  return cxxbridge1$rust_vec$PreTile$truncate(this, len);
}
template <> Vec<::Keyword>::Vec() noexcept {
  cxxbridge1$rust_vec$Keyword$new(this);
}
template <> void Vec<::Keyword>::drop() noexcept {
  return cxxbridge1$rust_vec$Keyword$drop(this);
}
template <>::std::size_t Vec<::Keyword>::size() const noexcept {
  return cxxbridge1$rust_vec$Keyword$len(this);
}
template <>::std::size_t Vec<::Keyword>::capacity() const noexcept {
  return cxxbridge1$rust_vec$Keyword$capacity(this);
}
template <>::Keyword const *Vec<::Keyword>::data() const noexcept {
  return cxxbridge1$rust_vec$Keyword$data(this);
}
template <> void Vec<::Keyword>::reserve_total(::std::size_t new_cap) noexcept {
  return cxxbridge1$rust_vec$Keyword$reserve_total(this, new_cap);
}
template <> void Vec<::Keyword>::set_len(::std::size_t len) noexcept {
  return cxxbridge1$rust_vec$Keyword$set_len(this, len);
}
template <> void Vec<::Keyword>::truncate(::std::size_t len) {
  return cxxbridge1$rust_vec$Keyword$truncate(this, len);
}
template <> Vec<::KeywordGroup>::Vec() noexcept {
  cxxbridge1$rust_vec$KeywordGroup$new(this);
}
template <> void Vec<::KeywordGroup>::drop() noexcept {
  return cxxbridge1$rust_vec$KeywordGroup$drop(this);
}
template <>::std::size_t Vec<::KeywordGroup>::size() const noexcept {
  return cxxbridge1$rust_vec$KeywordGroup$len(this);
}
template <>::std::size_t Vec<::KeywordGroup>::capacity() const noexcept {
  return cxxbridge1$rust_vec$KeywordGroup$capacity(this);
}
template <>::KeywordGroup const *Vec<::KeywordGroup>::data() const noexcept {
  return cxxbridge1$rust_vec$KeywordGroup$data(this);
}
template <>
void Vec<::KeywordGroup>::reserve_total(::std::size_t new_cap) noexcept {
  return cxxbridge1$rust_vec$KeywordGroup$reserve_total(this, new_cap);
}
template <> void Vec<::KeywordGroup>::set_len(::std::size_t len) noexcept {
  return cxxbridge1$rust_vec$KeywordGroup$set_len(this, len);
}
template <> void Vec<::KeywordGroup>::truncate(::std::size_t len) {
  return cxxbridge1$rust_vec$KeywordGroup$truncate(this, len);
}
template <> Vec<::TileDiff>::Vec() noexcept {
  cxxbridge1$rust_vec$TileDiff$new(this);
}
template <> void Vec<::TileDiff>::drop() noexcept {
  return cxxbridge1$rust_vec$TileDiff$drop(this);
}
template <>::std::size_t Vec<::TileDiff>::size() const noexcept {
  return cxxbridge1$rust_vec$TileDiff$len(this);
}
template <>::std::size_t Vec<::TileDiff>::capacity() const noexcept {
  return cxxbridge1$rust_vec$TileDiff$capacity(this);
}
template <>::TileDiff const *Vec<::TileDiff>::data() const noexcept {
  return cxxbridge1$rust_vec$TileDiff$data(this);
}
template <>
void Vec<::TileDiff>::reserve_total(::std::size_t new_cap) noexcept {
  return cxxbridge1$rust_vec$TileDiff$reserve_total(this, new_cap);
}
template <> void Vec<::TileDiff>::set_len(::std::size_t len) noexcept {
  return cxxbridge1$rust_vec$TileDiff$set_len(this, len);
}
template <> void Vec<::TileDiff>::truncate(::std::size_t len) {
  return cxxbridge1$rust_vec$TileDiff$truncate(this, len);
}
} // namespace cxxbridge1
} // namespace rust
