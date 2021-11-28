package ru.denissin.bankapp.service

import kotlinx.coroutines.async
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.launch
import org.springframework.stereotype.Service
import ru.denissin.bankapp.exception.BankAppException
import java.math.BigDecimal
import javax.transaction.Transactional

@Service
class BalanceApiService(
    private val costumerService: CostumerService,
    private val transactionService: TransactionService
) {

    suspend fun getBalance(id: Long): BigDecimal {
        return coroutineScope {
            costumerService.find(id).balance
                ?: throw BankAppException("Costumer id = $id has null balance")
        }
    }

    @Transactional
    suspend fun transferAmount(id: Long, phone: String, amount: BigDecimal) = coroutineScope {
        val srcCostumer = async { costumerService.find(id) }.await()
        val dstCostumer = async { costumerService.findByPhone(phone) }.await()
        if (srcCostumer.balance!! < amount) {
            throw BankAppException("Illegal operation source costumer has balance lower than amount")
        }
        transactionService.createTransaction(srcCostumer.id!!, dstCostumer.id!!, amount)
        srcCostumer.balance = srcCostumer.balance!! - amount
        dstCostumer.balance = dstCostumer.balance!! - amount
        costumerService.save(srcCostumer)
        costumerService.save(dstCostumer)
    }

}