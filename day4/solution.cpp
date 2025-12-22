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

void BaseAlgorithm::init() {
    result_ = 0;
}

/**
 * @note Assumes that offset is at the start of a line
 */
void BaseAlgorithm::operator()(const int fd, const off_t length, const off_t offset) {
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
    execute(fd, CHARS_PER_LINE);
}

void SinglePassAlgorithm::countMiddleRow(std::string* buffers) {
    auto top = buffers[0].cbegin() + 1;
    auto mid = buffers[1].begin() + 1;
    auto bot = buffers[2].cbegin() + 1;
    const auto end = buffers[1].cend() - 1;

    for (; mid < end; top++, mid++, bot++) {
        if (!isPaperRoll(mid)) continue;
        int count = 0;
        for (int x = -1; x <= 1; x++) {
            if (isPaperRoll(top + x)) count++;
            if (isPaperRoll(bot + x)) count++;
        }
        if (isPaperRoll(mid - 1)) count++;
        if (isPaperRoll(mid + 1)) count++;
        if (count < UPPER_BOUND_OF_ACCESSIBILITY) markAccessiblePaperRoll(mid);
    }
}

int BaseAlgorithm::result() const {
    return result_;
}

bool BaseAlgorithm::isPaperRoll(const std::string::const_iterator& element) {
    return *element == PAPER_ROLL;
}

void SinglePassAlgorithm::execute(const int fd, const off_t charsPerLine) {
    const off_t COUNTING_BUFFER_SIZE = charsPerLine + 2;
    static constexpr int NUM_BUFFERS = 4;
    std::string buffers[NUM_BUFFERS];
    for (int i = 0; i < NUM_BUFFERS; i++)
        buffers[i].assign(COUNTING_BUFFER_SIZE, EMPTY_SPACE);
    while (charsPerLine == read(fd, &buffers[NUM_BUFFERS - 1][1], charsPerLine)) {
        lseek(fd, 1, SEEK_CUR);
        for (int i = 0; i < 3; i++) buffers[i].swap(buffers[i + 1]);
        countMiddleRow(buffers);
    }
    for (int i = 0; i < 2; i++) buffers[i].swap(buffers[i + 1]);
    buffers[2].assign(COUNTING_BUFFER_SIZE, EMPTY_SPACE);
    countMiddleRow(buffers);
}

void BaseAlgorithm::markAccessiblePaperRoll(std::string::iterator& element) {
    (void)element;
    result_++;
}

void InPlaceDeleteToStability::execute(const int fd, const off_t charsPerLine) {
    const off_t COUNTING_BUFFER_SIZE = charsPerLine + 2;
    std::vector<std::string> rows;
    std::string row;
    row.assign(COUNTING_BUFFER_SIZE, EMPTY_SPACE);
    rows.push_back(row);
    while (charsPerLine == read(fd, &row[1], charsPerLine)) {
        lseek(fd, 1, SEEK_CUR);
        rows.push_back(row);
    }
    row.assign(COUNTING_BUFFER_SIZE, EMPTY_SPACE);
    rows.push_back(row);

    const int numRows = rows.size() - 2;
    std::vector<std::string> copy = rows;
    for (;;) {
        bool changed = false;
        for (int i = 1; i < numRows; i++)
            changed = countRowInplace(i, rows, copy) || changed;

        if (!changed) break;
        rows.swap(copy);
    }
}

bool InPlaceDeleteToStability::countRowInplace(const int rowIndex,
    const std::vector<std::string>& rows, std::vector<std::string>& copy) {
    auto topRow = rows[rowIndex - 1].cbegin() + 1;
    auto midRow = rows[rowIndex].cbegin() + 1;
    auto botRow = rows[rowIndex + 1].cbegin() + 1;
    auto midCopy = copy[rowIndex].begin() + 1;
    const auto midEnd = rows[rowIndex].cend() - 1;

    bool changed = false;
    for (; midRow < midEnd; topRow++, midRow++, botRow++, midCopy++) {
        if (!isPaperRoll(midRow)) continue;
        int count = 0;
        for (int x = -1; x <= 1; x++) {
            if (isPaperRoll(topRow + x)) count++;
            if (isPaperRoll(botRow + x)) count++;
        }
        if (isPaperRoll(midRow - 1)) count++;
        if (isPaperRoll(midRow + 1)) count++;

        if (changed |= count < UPPER_BOUND_OF_ACCESSIBILITY)
            markAccessiblePaperRoll(midCopy);
    }

    return changed;
}

void InPlaceDeleteToStability::markAccessiblePaperRoll(std::string::iterator& element) {
    BaseAlgorithm::markAccessiblePaperRoll(element);
    *element = ACCESSIBLE_PAPER_ROLL;
}

// ============================================================================
// Part-specific overrides
// ============================================================================

// Add Part1/Part2 specific method implementations here

} // namespace solution
