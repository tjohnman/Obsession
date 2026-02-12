#ifndef TASKIDGENERATOR_H
#define TASKIDGENERATOR_H

#include <QtTypes>

/**
 * @brief Generates unique task IDs for transactions
 * 
 * Simple counter-based ID generator for tracking
 * transaction requests and replies.
 */
class TaskIdGenerator
{
public:
    TaskIdGenerator() : m_counter(0) {}

    /**
     * @brief Generate the next task ID
     * @return Next unique task ID
     */
    qint32 next() {
        return m_counter++;
    }

    /**
     * @brief Reset the counter (typically on disconnect)
     */
    void reset() {
        m_counter = 0;
    }

    /**
     * @brief Get current counter value without incrementing
     */
    qint32 current() const {
        return m_counter;
    }

private:
    qint32 m_counter;
};

#endif // TASKIDGENERATOR_H
