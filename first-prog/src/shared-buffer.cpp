#include "shared-buffer.h"

void SharedBuffer::write(const std::string& data) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        buffer = data;
    }
    cv.notify_all();
}

std::string SharedBuffer::read() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return !buffer.empty(); });

    std::string data = buffer;
    buffer.clear();

    return data;
}

void SharedBuffer::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    buffer.clear();
}
