/*
 * uniqueue class provides a unique queue implementation where each element can only
 * exist once in the queue at any given time. It ensures that duplicate elements
 * are not added to the queue. The class also provides functionality to check if
 * an element is currently active (present in the queue).
 *
 * Template parameter:
 * - T: The type of elements stored in the queue. T must be convertible to size_t
 *      for indexing purposes.
 *
 * Public methods:
 * - uniqueue(size_t n): Constructor that initializes the queue with a given size.
 * - bool empty() const noexcept: Checks if the queue is empty.
 * - size_t size() const noexcept: Returns the number of elements in the queue.
 * - void push(T v): Adds an element to the queue if it is not already present.
 * - T pop(): Removes and returns the front element of the queue.
 * - void clear(): Clears all elements from the queue.
 * - bool is_active(T v): Checks if a given element is currently active in the queue.
 *
 * Example usage:
 * uniqueue<int> uq(10);
 * uq.push(3);
 * uq.push(5);
 * uq.push(3); // Duplicate, will not be added.
 * while (!uq.empty()) {
 *     int v = uq.pop();
 *     // Process v
 * }
 */


#pragma once

#include <queue>
#include <vector>

template <typename T>
class uniqueue {
private:
  std::queue<T> _queue;
  std::vector<bool> _isact;

public:
  uniqueue(size_t n) : _queue(), _isact(n) { }

  bool empty() const noexcept {
    return _queue.empty();
  }

  size_t size() const noexcept {
    return _queue.size();
  }

  void push(T v) {
    if (!_isact[v]) {
      _isact[v] = true;
      _queue.push(v);
    }
  }

  T pop() {
    T v = _queue.front();
    _isact[v] = false;
    _queue.pop();
    return v;
  }

  void clear() {
    while (!_queue.empty()) {
      _isact[_queue.front()] = false;
      _queue.pop();
    }
  }

  bool is_active(T v){
    return _isact[v];
  }
};
