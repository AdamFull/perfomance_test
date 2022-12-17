package com.example.perfomance_test.ui.main.cpuinfo

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import com.example.perfomance_test.databinding.FragmentCpuInfoBinding
import kotlinx.serialization.decodeFromString
import kotlinx.serialization.json.Json

/**
 * A simple [Fragment] subclass.
 * Use the [CpuInfoFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class CpuInfoFragment : Fragment() {
    private var binding: FragmentCpuInfoBinding? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = FragmentCpuInfoBinding.inflate(inflater, container, false)
        return binding!!.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        val deserialized = Json.decodeFromString<ArrayList<CpuInfoItemData>>(getCpuFeatures())

        val newsAdapter = CpuInfoAdapter(deserialized)
        val layoutManager = LinearLayoutManager(requireActivity(), LinearLayoutManager.VERTICAL, false)

        binding!!.featuresList.adapter = newsAdapter
        binding!!.featuresList.layoutManager = layoutManager
        binding!!.featuresList.addItemDecoration(
            DividerItemDecoration(
                requireActivity(),
                layoutManager.orientation
            ))
    }

    external fun getCpuFeatures(): String
}