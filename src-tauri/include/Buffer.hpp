#pragma once

class Buffer {
public:
  Buffer();
  Buffer(Buffer &&) = default;
  Buffer(const Buffer &) = default;
  Buffer &operator=(Buffer &&) = default;
  Buffer &operator=(const Buffer &) = default;
  ~Buffer();

private:
};
