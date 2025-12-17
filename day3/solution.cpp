#include "solution.h"

#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <sys/mman.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace solution {

InputManager::InputManager(const int fd) {
    struct stat sb;
    if (fstat(fd, &sb) < 0) {
        close(fd);
        throw std::system_error();
    }

    fileSize_ = sb.st_size;

    if (fileSize_ == 0) {
        mapped_ = nullptr;
        digitsPerLine_ = 0;
        numThreads_ = 0;
        bytesPerThread_ = 0;
        return;
    }

    mapped_ = (char*)mmap(nullptr, fileSize_, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (mapped_ == MAP_FAILED) throw std::system_error();

    data_ = { mapped_, fileSize_ };
    digitsPerLine_ = data_.find('\n');
    if (digitsPerLine_ == std::string_view::npos) digitsPerLine_ = fileSize_;
    if (digitsPerLine_ == 0) {
        numThreads_ = 0;
        bytesPerThread_ = 0;
        return;
    }

    const size_t bytesPerLine = digitsPerLine_ + 1;
    const size_t numLines = fileSize_ / bytesPerLine;

    if (numLines == 0) {
        numThreads_ = 0;
        bytesPerThread_ = 0;
        return;
    }

    numThreads_ = std::min((size_t)std::thread::hardware_concurrency(), numLines);
    const size_t linesPerThread = (numLines + numThreads_ - 1) / numThreads_;
    bytesPerThread_ = linesPerThread * bytesPerLine;
}

InputManager::~InputManager() {
    if (mapped_) munmap(mapped_, fileSize_);
}

InputManager::BankIterator InputManager::banksOfNextThread() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (nextThread_ >= numThreads_)
        throw std::range_error("All threads have already been dispatched");

    const size_t start = nextThread_ * bytesPerThread_;
    const size_t end = std::min(start + bytesPerThread_, fileSize_);
    nextThread_++;

    return { data_, start, end, digitsPerLine_ };
}

bool InputManager::BankIterator::operator>>(Bank& bank) {
    if (pos_ >= end_) return false;

    const size_t bytesPerLine = digitsPerLine_ + 1;

    bank = Bank(data_.substr(pos_, digitsPerLine_));
    pos_ += bytesPerLine;
    return true;
}

// ============================================================================
// Threading Strategies
// ============================================================================

Joltage SingleThreadStrategy::operator()(const JoltageCalculator& calc) const {
    std::unique_ptr<std::istream> is = solver_->getInputStream();
    Joltage total = 0;
    Bank bank;
    while (std::getline(*is, bank)) total += calc(bank);
    return total;
}

Joltage MultiThreadStrategy::operator()(const JoltageCalculator& calc) const {
    InputManager input = solver_->getFileDescriptor();
    if (input.numThreads() == 0) return 0;
    JoltageMonitor joltage = 0;

    std::vector<std::jthread> threads;
    threads.reserve(std::thread::hardware_concurrency());

    const unsigned numThreads = input.numThreads();
    for (unsigned i = 0; i < numThreads; i++) {
        threads.emplace_back([&]() {
            Joltage threadJoltage = 0;
            InputManager::BankIterator banks = input.banksOfNextThread();
            Bank bank;

            while (banks >> bank) threadJoltage += calc(bank);

            joltage.incrementBy(threadJoltage);
        });
    }

    threads.clear();

    return joltage.get();
}

Joltage MultiThreadStrategy::JoltageMonitor::get() const {
    return joltage_;
}

void MultiThreadStrategy::JoltageMonitor::incrementBy(const Joltage joltage) {
    std::lock_guard<std::mutex> lock(mtx);
    joltage_ += joltage;
}

// ============================================================================
// Day3 Implementation
// ============================================================================

Joltage Day3::solve() const {
    const JoltageCalculator calc = [this](const Bank& b) { return getJoltage(b); };
    return (*strategy_)(calc);
}

} // namespace solution
