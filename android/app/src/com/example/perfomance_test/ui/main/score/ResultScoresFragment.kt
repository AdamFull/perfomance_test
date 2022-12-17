package com.example.perfomance_test.ui.main.score

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.navArgs
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import com.example.perfomance_test.databinding.FragmentResultScoresBinding
import kotlinx.serialization.decodeFromString
import kotlinx.serialization.json.Json

/**
 * A simple [Fragment] subclass.
 * Use the [ResultScoresFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class ResultScoresFragment : Fragment() {
    val args: ResultScoresFragmentArgs by navArgs()
    private var binding: FragmentResultScoresBinding? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = FragmentResultScoresBinding.inflate(inflater, container, false)
        return binding!!.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {

        if (args.result != null) {
            val resultStruct = Json.decodeFromString<Map<String, TestCaseResultData>>(args.result)
            val selection = resultStruct.values.first()

            val newsAdapter = AlgorithmResultAdapter(selection.chunks)
            val layoutManager = LinearLayoutManager(requireActivity(), LinearLayoutManager.VERTICAL, false)

            binding!!.resultsList.adapter = newsAdapter
            binding!!.resultsList.layoutManager = layoutManager
            binding!!.resultsList.addItemDecoration(
                DividerItemDecoration(
                    requireActivity(),
                    layoutManager.orientation
                )
            )
        }
    }
}