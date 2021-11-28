package ru.denissin.bankapp.repository

import org.springframework.data.jpa.repository.JpaRepository
import org.springframework.stereotype.Repository
import ru.denissin.bankapp.model.Transaction

@Repository
interface TransactionRepository: JpaRepository<Transaction, Long>
