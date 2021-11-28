package ru.denissin.bankapp

import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication

@SpringBootApplication
class BankAppApplication

fun main(args: Array<String>) {
    runApplication<BankAppApplication>(*args)
}
