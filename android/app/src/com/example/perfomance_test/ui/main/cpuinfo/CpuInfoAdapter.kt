package com.example.perfomance_test.ui.main.cpuinfo

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.example.perfomance_test.databinding.CpuInfoItemBinding

class CpuInfoAdapter(private val dataSource: ArrayList<CpuInfoItemData>) :
    RecyclerView.Adapter<CpuInfoItemHolder>() {

    private lateinit var binding: CpuInfoItemBinding

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): CpuInfoItemHolder {
        binding = CpuInfoItemBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        return CpuInfoItemHolder(binding)
    }

    override fun onBindViewHolder(holder: CpuInfoItemHolder, position: Int) {
        val largeNews = dataSource[position]
        holder.bind(largeNews)
    }

    override fun getItemCount(): Int = dataSource.size
}