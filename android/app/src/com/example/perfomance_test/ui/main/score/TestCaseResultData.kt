package com.example.perfomance_test.ui.main.score

import kotlinx.serialization.Serializable

@Serializable
data class TestCaseResultData(
    val best: String,
    val worse: String,
    val delta: Double,
    val chunks: ArrayList<AlgotithmResultBlockData>
)
