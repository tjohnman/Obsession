#ifndef TRANSACTIONQUEUE_H
#define TRANSACTIONQUEUE_H

#include <vector>
#include "ctransaction.h"

/**
 * @brief Manages pending transactions awaiting server replies
 * 
 * Tracks transactions that expect replies from the server,
 * matches incoming replies to pending requests, and handles cleanup.
 */
class TransactionQueue
{
public:
    TransactionQueue() = default;
    ~TransactionQueue() {
        // Clean up any remaining transactions
        for (auto* trans : m_pendingTransactions) {
            delete trans;
        }
    }

    // No copy or move - owns transaction pointers
    TransactionQueue(const TransactionQueue&) = delete;
    TransactionQueue& operator=(const TransactionQueue&) = delete;

    /**
     * @brief Add a transaction to the pending queue
     */
    void add(CTransaction* transaction) {
        m_pendingTransactions.push_back(transaction);
    }

    /**
     * @brief Find a pending transaction by task ID
     * @return Pointer to transaction if found, nullptr otherwise
     */
    CTransaction* findByTaskId(qint32 taskId) {
        for (auto* trans : m_pendingTransactions) {
            if (trans->taskID() == taskId) {
                return trans;
            }
        }
        return nullptr;
    }

    /**
     * @brief Mark a transaction as complete by task ID
     */
    void markComplete(qint32 taskId) {
        for (auto* trans : m_pendingTransactions) {
            if (trans->taskID() == taskId) {
                trans->done = true;
                break;
            }
        }
    }

    /**
     * @brief Remove and delete all completed transactions
     */
    void removeCompleted() {
        for (size_t i = 0; i < m_pendingTransactions.size(); ) {
            if (m_pendingTransactions[i]->done) {
                delete m_pendingTransactions[i];
                m_pendingTransactions[i] = m_pendingTransactions.back();
                m_pendingTransactions.pop_back();
            } else {
                ++i;
            }
        }
    }

    /**
     * @brief Get count of pending transactions
     */
    size_t size() const {
        return m_pendingTransactions.size();
    }

private:
    std::vector<CTransaction*> m_pendingTransactions;
};

#endif // TRANSACTIONQUEUE_H
