package com.example.perfomance_test.ui.main.testing

import android.app.ProgressDialog
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.ArrayAdapter
import androidx.navigation.fragment.findNavController
import com.example.perfomance_test.R
import com.example.perfomance_test.databinding.FragmentRunTestsBinding
import kotlinx.serialization.decodeFromString
import kotlinx.serialization.json.Json
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

/**
 * A simple [Fragment] subclass.
 * Use the [RunTestsFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class RunTestsFragment : Fragment() {
    private var binding: FragmentRunTestsBinding? = null
    private var selectedAlgorithm: String = ""
    private var selectedTestCase: String = ""
    private var resultTest: String = ""
    private var selectedNumberOfIterations: Float = 0.0f
    private lateinit var testCasesArray: MutableList<String>
    private lateinit var algorithmsArray: MutableList<String>
    private lateinit var executorService: ExecutorService
    private lateinit var progressDialog: ProgressDialog

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = FragmentRunTestsBinding.inflate(inflater, container, false)
        return binding!!.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        executorService = Executors.newSingleThreadExecutor()

        val avaliableTCAAJson = getAvaliableTestCasesAndAlgorithms()
        val avaliableTestCasesAndAlgorithms = Json.decodeFromString<TestCasesAndAlgorithmsData>(avaliableTCAAJson)
        testCasesArray = avaliableTestCasesAndAlgorithms.test_cases
        algorithmsArray = avaliableTestCasesAndAlgorithms.algorithms

        algorithmsArray.add(0, "all")

        selectedAlgorithm = algorithmsArray.first()
        selectedTestCase = testCasesArray.first()
        selectedNumberOfIterations = binding!!.selectNumberOfTests.value

        activity?.let {
            ArrayAdapter(it, android.R.layout.simple_spinner_item, testCasesArray).also { adapter ->
                adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
                binding!!.testCaseSpinner.adapter = adapter
            }
        }

        activity?.let {
            ArrayAdapter(it, android.R.layout.simple_spinner_item, algorithmsArray).also { adapter ->
                adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
                binding!!.algorithmSpinner.adapter = adapter
            }
        }

        binding!!.testCaseSpinner.onItemSelectedListener = object :
            AdapterView.OnItemSelectedListener {
            override fun onItemSelected(p0: AdapterView<*>?, p1: View?, p2: Int, p3: Long) {
                selectedTestCase = testCasesArray[p2]
            }
            override fun onNothingSelected(p0: AdapterView<*>?) {
                TODO("Not yet implemented")
            }
        }

        binding!!.algorithmSpinner.onItemSelectedListener = object :
            AdapterView.OnItemSelectedListener {
            override fun onItemSelected(p0: AdapterView<*>?, p1: View?, p2: Int, p3: Long) {
                selectedAlgorithm = algorithmsArray[p2]
            }
            override fun onNothingSelected(p0: AdapterView<*>?) {
                TODO("Not yet implemented")
            }
        }

        binding!!.selectNumberOfTests.addOnChangeListener { slider, value, fromUser ->
            selectedNumberOfIterations = value
        }

        binding!!.buttonRunTest.setOnClickListener {
            executorService.execute {
                activity?.runOnUiThread {
                    progressDialog = ProgressDialog(activity)
                    progressDialog.setMessage("Running tests")
                    progressDialog.setCancelable(false)
                    progressDialog.show()
                }

                setTestCase(selectedTestCase)
                resultTest = runTest(selectedAlgorithm, selectedNumberOfIterations)

                activity?.runOnUiThread {
                    progressDialog.dismiss()
                    val action = RunTestsFragmentDirections.openResult(resultTest)
                    findNavController().navigate(action)
                }
            }
        }
    }

    external fun setTestCase(testCase: String)
    external fun runTest(algorithm: String, numIterations: Float): String
    external fun getAvaliableTestCasesAndAlgorithms(): String
}