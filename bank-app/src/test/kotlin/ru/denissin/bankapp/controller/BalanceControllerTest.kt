package ru.denissin.bankapp.controller

import kotlinx.coroutines.runBlocking
import org.junit.jupiter.api.Test
import org.mockito.Mockito.`when`
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.boot.test.autoconfigure.web.reactive.WebFluxTest
import org.springframework.boot.test.mock.mockito.MockBean
import org.springframework.test.web.reactive.server.WebTestClient
import org.springframework.test.web.reactive.server.expectBody
import ru.denissin.bankapp.service.BalanceApiService
import java.math.BigDecimal

@WebFluxTest
class BalanceControllerTest(
    @Autowired
    private val client: WebTestClient
) {

    @MockBean
    private lateinit var balanceService: BalanceApiService

    @Test
   fun `getBalance should return balance and status ok`(): Unit = runBlocking {
        `when`(balanceService.getBalance(1))
            .thenReturn(BigDecimal("123.23"))
        client.get().uri("/api/v1/balance/1").exchange()
            .expectStatus().isOk
            .expectBody<String>().isEqualTo("123.23")
    }

}