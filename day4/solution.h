#pragma once

#include "../common/aoc.h"

namespace solution {

// ============================================================================
// Day 4 Solution
// ============================================================================

class PrintingDepartmentDispatcher;
class PrintingDepartmentAlgorithm;

/**
 * @brief Base class for Day 4.
 *
 * Contains shared logic between Part1 and Part2.
 */
class Day4 : public aoc::Solver<int> {
public:
    explicit Day4(const std::string& inputFile) : Solver(inputFile) { }

    int solve() const override;

    void setReader(std::unique_ptr<PrintingDepartmentDispatcher>& reader) {
        departmentReader_ = std::move(reader);
    }

protected:
private:
    std::unique_ptr<PrintingDepartmentDispatcher> departmentReader_;
};

class Day4Part1 : public Day4 {
public:
    explicit Day4Part1(const std::string& inputFile) : Day4(inputFile) { }

protected:
};

class Day4Part2 : public Day4 {
public:
    explicit Day4Part2(const std::string& inputFile) : Day4(inputFile) { }

protected:
};

using Day4Part1Test = aoc::TestDecorator<Day4Part1>;
using Day4Part2Test = aoc::TestDecorator<Day4Part2>;

/* === PrintingDepartmentReaders === */

class PrintingDepartmentDispatcher {
public:
    virtual void executeOnDepartment(PrintingDepartmentAlgorithm* algo) const = 0;
};

class SingleThreadedDispatcher : public PrintingDepartmentDispatcher {
public:
    SingleThreadedDispatcher(const Day4* solver) : solver_(solver) { }

    virtual void executeOnDepartment(PrintingDepartmentAlgorithm* algo) const override;

private:
    const Day4* solver_;
};

/* === Algorithm === */

class PrintingDepartmentAlgorithm {
public:
    virtual void init() = 0;
    virtual void operator()(const int fd, const off_t length, const off_t offset = 0) = 0;
    virtual int result() const = 0;

protected:
    static constexpr char PAPER_ROLL = '@';
    static constexpr char EMPTY_SPACE = '.';
};

class SinglePassAlgorithm : PrintingDepartmentAlgorithm {
public:
    virtual void init() override;
    virtual void operator()(
        const int fd, const off_t length, const off_t offset) override;
    virtual int result() const override;

private:
    int result_;

    void countMiddleRow(const std::string* buffers);
};

}; // namespace solution
