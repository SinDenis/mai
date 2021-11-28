package ru.denissin.bankapp.service

import org.springframework.stereotype.Service
import ru.denissin.bankapp.exception.BankAppException
import ru.denissin.bankapp.exception.NotFoundException
import ru.denissin.bankapp.model.Costumer
import ru.denissin.bankapp.repository.CostumerRepository
import java.lang.RuntimeException
import javax.transaction.Transactional

@Service
class CostumerService(
    private val costumerRepository: CostumerRepository
) {

    @Transactional
    fun findByPhone(phone: String): Costumer {
        return costumerRepository.findByPhone(phone)
            ?: throw NotFoundException("Not found costumer by phone = $phone")
    }

    @Transactional
    fun find(id: Long): Costumer {
        return costumerRepository.findById(id)
            .orElseThrow { NotFoundException(Costumer::class, id)}
    }

    @Transactional
    fun save(costumer: Costumer): Costumer {
        return costumerRepository.save(costumer)
    }
}