package com.example.perfomance_test.ui.main.testing

import kotlinx.serialization.Serializable

@Serializable
data class TestCasesAndAlgorithmsData(
    val test_cases: MutableList<String>,
    val algorithms: MutableList<String>
)
