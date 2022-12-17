package com.example.perfomance_test.ui.main.cpuinfo
import kotlinx.serialization.*

@Serializable
data class CpuInfoItemData(
    val name: String,
    val status: Boolean
)