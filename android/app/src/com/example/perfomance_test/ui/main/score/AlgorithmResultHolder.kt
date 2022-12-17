package com.example.perfomance_test.ui.main.score

import androidx.recyclerview.widget.RecyclerView
import com.example.perfomance_test.databinding.AlgorithmResultItemBinding

class AlgorithmResultHolder(private val binding: AlgorithmResultItemBinding) :
    RecyclerView.ViewHolder(binding.root) {
    fun bind(data: AlgotithmResultBlockData) {
        binding.score = data

    }
}