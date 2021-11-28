package ru.denissin.bankapp.repository

import org.springframework.data.jpa.repository.JpaRepository
import org.springframework.stereotype.Repository
import ru.denissin.bankapp.model.Costumer

@Repository
interface CostumerRepository: JpaRepository<Costumer, Long> {

    fun findByPhone(phone: String): Costumer?
}
