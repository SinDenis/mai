package ru.denissin.bankapp.exception

import java.lang.RuntimeException

open class BankAppException(message: String?) : RuntimeException(message) {
}