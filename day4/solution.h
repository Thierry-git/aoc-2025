#pragma once

#include "../common/aoc.h"

namespace solution {

// ============================================================================
// Day 4 Solution
// ============================================================================

class PrintingDepartmentDispatcher;
class PrintingDepartmentAlgorithm;

constexpr char PAPER_ROLL = '@';
constexpr char EMPTY_SPACE = '.';
constexpr int UPPER_BOUND_OF_ACCESSIBILITY = 4;

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
    virtual std::unique_ptr<PrintingDepartmentAlgorithm> getAlgo() const = 0;

private:
    std::unique_ptr<PrintingDepartmentDispatcher> departmentReader_;
};

class Day4Part1 : public Day4 {
public:
    explicit Day4Part1(const std::string& inputFile) : Day4(inputFile) { }

protected:
    virtual std::unique_ptr<PrintingDepartmentAlgorithm> getAlgo() const override;
};

class Day4Part2 : public Day4 {
public:
    explicit Day4Part2(const std::string& inputFile) : Day4(inputFile) { }

protected:
    virtual std::unique_ptr<PrintingDepartmentAlgorithm> getAlgo() const override;
};

using Day4Part1Test = aoc::TestDecorator<Day4Part1>;
using Day4Part2Test = aoc::TestDecorator<Day4Part2>;

/* === PrintingDepartmentReaders === */

class PrintingDepartmentDispatcher {
public:
    virtual void executeOnDepartment(
        std::unique_ptr<PrintingDepartmentAlgorithm>& algo) const
        = 0;
};

class SingleThreadedDispatcher : public PrintingDepartmentDispatcher {
public:
    SingleThreadedDispatcher(const Day4* solver) : solver_(solver) { }

    virtual void executeOnDepartment(
        std::unique_ptr<PrintingDepartmentAlgorithm>& algo) const override;

private:
    const Day4* solver_;
};

/* === Algorithm === */

class PrintingDepartmentAlgorithm {
public:
    virtual void init() = 0;
    virtual void operator()(const int fd, const off_t length, const off_t offset = 0) = 0;
    virtual int result() const = 0;
};

class BaseAlgorithm : public PrintingDepartmentAlgorithm {
public:
    virtual void init() override;
    virtual void operator()(
        const int fd, const off_t length, const off_t offset = 0) override;
    virtual int result() const override;

protected:
    virtual void execute(const int fd, const off_t charsPerLine) = 0;
    virtual void markAccessiblePaperRoll(std::string::iterator& paperRoll);

    static bool isPaperRoll(const std::string::const_iterator& element);

private:
    int result_ = 0;
};

class SinglePassAlgorithm : public BaseAlgorithm {
protected:
    virtual void execute(const int fd, const off_t charsPerLine) override;

private:
    void countMiddleRow(std::string* buffers);
};

class InPlaceDeleteToStability : public BaseAlgorithm {
protected:
    virtual void execute(const int fd, const off_t charsPerLine) override;
    virtual void markAccessiblePaperRoll(std::string::iterator& paperRoll) override;

private:
    static constexpr char ACCESSIBLE_PAPER_ROLL = 'x';

    bool countRowInplace(const int rowIndex, const std::vector<std::string>& rows,
        std::vector<std::string>& copy);
};

}; // namespace solution
