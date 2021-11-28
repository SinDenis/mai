package ru.denissin.bankapp.controller

import org.springframework.http.HttpStatus
import org.springframework.web.bind.annotation.*
import ru.denissin.bankapp.service.BalanceApiService
import java.math.BigDecimal

@RestController
@RequestMapping("/api/v1/balance")
class BalanceController(
    private val balanceService: BalanceApiService
) {

    @GetMapping("/{id}")
    suspend fun getBalance(@PathVariable("id") id: Long): BigDecimal {
        return balanceService.getBalance(id)
    }

    @PostMapping("/{id}")
    @ResponseStatus(HttpStatus.OK)
    suspend fun transferAmount(
        @PathVariable("id") id: Long,
        @RequestParam("phone") phone: String,
        @RequestParam("amount") amount: BigDecimal
    ) {
        balanceService.transferAmount(id, phone, amount)
    }
}