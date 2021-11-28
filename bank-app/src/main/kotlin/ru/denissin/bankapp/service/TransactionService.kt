package ru.denissin.bankapp.service

import org.springframework.stereotype.Service
import ru.denissin.bankapp.model.Transaction
import ru.denissin.bankapp.repository.TransactionRepository
import java.math.BigDecimal
import java.time.Instant

@Service
class TransactionService(
    private val transactionRepository: TransactionRepository
) {

    fun createTransaction(srcId: Long, dstId: Long, amount: BigDecimal): Boolean {
        transactionRepository.save(
            Transaction(
                srcCostumerId = srcId,
                dstCostumerId = dstId,
                amount = amount,
                date = Instant.now()
            )
        )
        return true
    }
}