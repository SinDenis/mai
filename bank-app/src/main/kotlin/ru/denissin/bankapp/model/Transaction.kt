package ru.denissin.bankapp.model

import ru.denissin.bankapp.model.converter.InstantToTimestampMillisAttributeConverter
import java.math.BigDecimal
import java.time.Instant
import javax.persistence.*

/**
 * Банковская транзакция со счета на счет
 */
@Entity
@SequenceGenerator(allocationSize = 1, name = "TRANSACTION_SEQ", sequenceName = "TRANSACTION_SEQ")
data class Transaction(

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "TRANSACTION_SEQ")
    @Column(name = "ID")
    var id: Long? = null,

    /**
     * ID клиента, переводящего деньги
     */
    @Column(name = "SRC_COSTUMER_ID")
    var srcCostumerId: Long,

    /**
     * ID клиента, получающего деньги
     */
    @Column(name = "DST_COSTUMER_ID")
    var dstCostumerId: Long,

    /**
     * Сумма транзакции
     */
    @Column(name = "AMOUNT")
    var amount: BigDecimal,

    /**
     * Дата транзакции
     */
    @Column(name = "DATE")
    @Convert(converter = InstantToTimestampMillisAttributeConverter::class)
    var date: Instant

)