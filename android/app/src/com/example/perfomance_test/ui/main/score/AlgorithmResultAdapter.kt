package com.example.perfomance_test.ui.main.score

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.example.perfomance_test.databinding.AlgorithmResultItemBinding
import com.example.perfomance_test.ui.main.cpuinfo.CpuInfoItemHolder

class AlgorithmResultAdapter(private val dataSource: ArrayList<AlgotithmResultBlockData>) :
    RecyclerView.Adapter<AlgorithmResultHolder>() {

    private lateinit var binding: AlgorithmResultItemBinding

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): AlgorithmResultHolder {
        binding = AlgorithmResultItemBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        return AlgorithmResultHolder(binding)
    }

    override fun onBindViewHolder(holder: AlgorithmResultHolder, position: Int) {
        val largeNews = dataSource[position]
        holder.bind(largeNews)
    }

    override fun getItemCount(): Int = dataSource.size
}