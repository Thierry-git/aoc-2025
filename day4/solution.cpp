#include "solution.h"

#include <sys/stat.h>

namespace solution {

// ============================================================================
// Day4 Implementation
// ============================================================================

int Day4::solve() const {
    std::unique_ptr<PrintingDepartmentDispatcher> reader
        = std::make_unique<SingleThreadedDispatcher>(this);

    PrintingDepartmentAlgorithm* algo
        = (PrintingDepartmentAlgorithm*)(new SinglePassAlgorithm);
    algo->init();
    reader->executeOnDepartment(algo);
    return algo->result();
}

void SingleThreadedDispatcher::executeOnDepartment(
    PrintingDepartmentAlgorithm* algo) const {
    const int fd = solver_->getFileDescriptor();

    struct stat sb;
    if (fstat(fd, &sb) < 0) {
        close(fd);
        throw std::system_error();
    }

    const off_t length = sb.st_size;

    (*algo)(fd, length);
}

void SinglePassAlgorithm::init() {
    result_ = 0;
}

/**
 * @note Assumes that offset is at the start of a line
 */
void SinglePassAlgorithm::operator()(
    const int fd, const off_t length, const off_t offset) {
    (void)length;
    off_t LINE_SIZE = 0;
    static constexpr int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    off_t pos = 0;
    ssize_t bytes_read;

    while (LINE_SIZE == 0 && (bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                LINE_SIZE = pos + i + 1;
                lseek(fd, offset, SEEK_SET);
                break;
            }
        }
        pos += bytes_read;
    }
    if (LINE_SIZE == 0) return;

    const off_t CHARS_PER_LINE = LINE_SIZE - 1;
    const off_t COUNTING_BUFFER_SIZE = CHARS_PER_LINE + 2;
    static constexpr int NUM_BUFFERS = 4;
    std::string buffers[NUM_BUFFERS];
    for (int i = 0; i < NUM_BUFFERS; i++)
        buffers[i].assign(COUNTING_BUFFER_SIZE, EMPTY_SPACE);
    while (CHARS_PER_LINE == read(fd, &buffers[NUM_BUFFERS - 1][1], CHARS_PER_LINE)) {
        lseek(fd, 1, SEEK_CUR);
        for (int i = 0; i < 3; i++) buffers[i].swap(buffers[i + 1]);
        countMiddleRow(buffers);
    }
    for (int i = 0; i < 2; i++) buffers[i].swap(buffers[i + 1]);
    buffers[2].assign(COUNTING_BUFFER_SIZE, EMPTY_SPACE);
    countMiddleRow(buffers);
}

void SinglePassAlgorithm::countMiddleRow(const std::string* buffers) {
    auto top = buffers[0].cbegin() + 1;
    auto mid = buffers[1].cbegin() + 1;
    auto bot = buffers[2].cbegin() + 1;
    const auto end = buffers[1].cend() - 1;

    for (; mid < end; top++, mid++, bot++) {
        if (*mid != PAPER_ROLL) continue;
        int count = 0;
        for (int x = -1; x <= 1; x++) {
            if (*(top + x) == PAPER_ROLL) count++;
            if (*(bot + x) == PAPER_ROLL) count++;
        }
        if (*(mid - 1) == PAPER_ROLL) count++;
        if (*(mid + 1) == PAPER_ROLL) count++;
        if (count < 4) result_++;
    }
}

int SinglePassAlgorithm::result() const {
    return result_;
}

// ============================================================================
// Part-specific overrides
// ============================================================================

// Add Part1/Part2 specific method implementations here

} // namespace solution
