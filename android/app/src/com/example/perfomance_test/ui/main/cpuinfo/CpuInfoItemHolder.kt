package com.example.perfomance_test.ui.main.cpuinfo

import android.widget.ListView
import androidx.recyclerview.widget.RecyclerView
import com.example.perfomance_test.databinding.CpuInfoItemBinding

class CpuInfoItemHolder(private val binding: CpuInfoItemBinding) :
    RecyclerView.ViewHolder(binding.root) {
    fun bind(data: CpuInfoItemData) {
        binding.cpuinfo = data

    }
}