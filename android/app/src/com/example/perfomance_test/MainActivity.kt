package com.example.perfomance_test

import android.app.ProgressDialog
import android.content.res.AssetManager
import android.os.Bundle
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import androidx.appcompat.app.AppCompatActivity
import com.example.perfomance_test.databinding.ActivityMainBinding
import org.json.JSONObject
import java.io.File
import java.io.File.separator
import java.io.FileOutputStream
import java.io.IOException
import java.io.OutputStream
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
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



        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val extFilesPath = getExternalFilesDir("")
        assets.copyAssetFolder("", extFilesPath!!.absolutePath)

        initializeTestEnvironment(assets, extFilesPath!!.absolutePath)

        val avaliableTestCasesAndAlgorithmsJson = JSONObject(getAvaliableTestCasesAndAlgorithms())

        val avaliableTestCases = avaliableTestCasesAndAlgorithmsJson.getJSONArray("test_cases")
        val avaliableAlgorithms = avaliableTestCasesAndAlgorithmsJson.getJSONArray("algorithms")

        executorService = Executors.newSingleThreadExecutor()

        testCasesArray = MutableList(avaliableTestCases.length()) {
            avaliableTestCases.getString(it)
        }

        algorithmsArray = MutableList(avaliableAlgorithms.length()) {
            avaliableAlgorithms.getString(it)
        }

        algorithmsArray.add(0, "all")

        selectedAlgorithm = algorithmsArray.first()
        selectedTestCase = testCasesArray.first()
        selectedNumberOfIterations = binding.selectNumberOfTests.value

        ArrayAdapter(this, android.R.layout.simple_spinner_item, testCasesArray).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            binding.testCaseSpinner.adapter = adapter
        }

        ArrayAdapter(this, android.R.layout.simple_spinner_item, algorithmsArray).also { adapter ->
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
            binding.algorithmSpinner.adapter = adapter
        }

        binding.testCaseSpinner.onItemSelectedListener = object :
        AdapterView.OnItemSelectedListener {
            override fun onItemSelected(p0: AdapterView<*>?, p1: View?, p2: Int, p3: Long) {
                selectedTestCase = testCasesArray[p2]
            }
            override fun onNothingSelected(p0: AdapterView<*>?) {
                TODO("Not yet implemented")
            }
        }

        binding.algorithmSpinner.onItemSelectedListener = object :
            AdapterView.OnItemSelectedListener {
            override fun onItemSelected(p0: AdapterView<*>?, p1: View?, p2: Int, p3: Long) {
                selectedAlgorithm = algorithmsArray[p2]
            }
            override fun onNothingSelected(p0: AdapterView<*>?) {
                TODO("Not yet implemented")
            }
        }

        binding.selectNumberOfTests.addOnChangeListener { slider, value, fromUser ->
            selectedNumberOfIterations = value
        }

        //binding.sampleText.text = resultPath
        binding.buttonRunTest.setOnClickListener {
            executorService.execute {
                runOnUiThread {
                    progressDialog = ProgressDialog(this)
                    progressDialog.setMessage("Running tests")
                    progressDialog.setCancelable(false)
                    progressDialog.show()
                }

                setTestCase(selectedTestCase)
                resultTest = runTest(selectedAlgorithm, selectedNumberOfIterations)

                runOnUiThread {
                    progressDialog.dismiss()
                }

            }
        }
    }

    fun AssetManager.copyAssetFolder(srcName: String, dstName: String): Boolean {
        return try {
            var result = true
            val fileList = this.list(srcName) ?: return false
            if (fileList.isEmpty()) {
                result = copyAssetFile(srcName, dstName)
            } else {
                val file = File(dstName)
                result = file.mkdirs()
                for (filename in fileList) {
                    if(srcName.isEmpty())
                        result = result and copyAssetFolder(
                            filename,
                            dstName + separator.toString() + filename
                        )
                    else
                        result = result and copyAssetFolder(
                            srcName + separator.toString() + filename,
                            dstName + separator.toString() + filename
                    )
                }
            }
            result
        } catch (e: IOException) {
            e.printStackTrace()
            false
        }
    }

    fun AssetManager.copyAssetFile(srcName: String, dstName: String): Boolean {
        return try {
            val inStream = this.open(srcName)
            val outFile = File(dstName)
            val out: OutputStream = FileOutputStream(outFile)
            val buffer = ByteArray(1024)
            var read: Int
            while (inStream.read(buffer).also { read = it } != -1) {
                out.write(buffer, 0, read)
            }
            inStream.close()
            out.close()
            true
        } catch (e: IOException) {
            e.printStackTrace()
            false
        }
    }

    external fun initializeTestEnvironment(assetManager: AssetManager, writablePath: String)
    external fun setTestCase(testCase: String)
    external fun runTest(algorithm: String, numIterations: Float): String
    external fun getAvaliableTestCasesAndAlgorithms(): String

    companion object {
        // Used to load the 'perfomance_test' library on application startup.
        init {
            System.loadLibrary("perfomance_test")
        }
    }
}