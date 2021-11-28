package ru.denissin.bankapp.exception

import kotlin.reflect.KClass
import kotlin.reflect.jvm.jvmName

class NotFoundException(message: String?) : BankAppException(message) {
    constructor(clazz: KClass<out Any>, id: Long):
            this("Not found ${clazz.jvmName} by id = $id")
}