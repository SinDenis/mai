package ru.denissin.bankapp.model.converter

import java.time.Instant
import javax.persistence.AttributeConverter

class InstantToTimestampMillisAttributeConverter: AttributeConverter<Instant, Long> {

    override fun convertToDatabaseColumn(attribute: Instant?): Long? {
        return attribute?.toEpochMilli()
    }

    override fun convertToEntityAttribute(dbData: Long?): Instant? {
        if (dbData == null) {
            return null
        }
        return Instant.ofEpochMilli(dbData)
    }

}