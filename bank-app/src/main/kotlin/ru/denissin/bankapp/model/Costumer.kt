package ru.denissin.bankapp.model

import java.math.BigDecimal
import javax.persistence.*

/**
 * Клиент банка
 */
@Entity
@SequenceGenerator(allocationSize = 1, name = "COSTUMER_SEQ", sequenceName = "COSTUMER_SEQ")
data class Costumer(

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "COSTUMER_SEQ")
    @Column(name = "ID")
    var id: Long?,

    /**
     * ФИО пользователя
     */
    @Column(name = "FULL_NAME")
    var fullName: String?,

    /**
     * Номер телефона
     */
    @Column(name = "PHONE")
    var phone: String?,

    /**
     * Значение на баленсе
     */
    @Column(name = "BALANCE")
    var balance: BigDecimal?

)