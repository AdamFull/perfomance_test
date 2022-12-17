package com.example.perfomance_test.ui.main.score

import kotlinx.serialization.Serializable

@Serializable
data class AlgotithmResultBlockData(
    val name: String,
    val deviation: Double,
    val mean: Double,
    val variance: Double,

    val valid: Boolean,
    val sameless: Double,

    val score: Int,
    val place: Int
)